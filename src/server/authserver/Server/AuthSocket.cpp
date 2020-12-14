/*
 * This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <openssl/md5.h>

#include "Common.h"
#include "CryptoGenerics.h"
#include "CryptoRandom.h"
#include "CryptoHash.h"
#include "DatabaseEnv.h"
#include "ByteBuffer.h"
#include "Config.h"
#include "Log.h"
#include "RealmList.h"
#include "AuthSocket.h"
#include "AuthCodes.h"
#include "TOTP.h"
#include "openssl/crypto.h"
#include "Util.h"

#define ChunkSize 2048

enum AccountFlags
{
    ACCOUNT_FLAG_GM         = 0x00000001,
    ACCOUNT_FLAG_TRIAL      = 0x00000008,
    ACCOUNT_FLAG_PROPASS    = 0x00800000,
};

enum SecurityFlags
{
    SECURITY_FLAG_NONE          = 0x00,
    SECURITY_FLAG_PIN           = 0x01,
    SECURITY_FLAG_UNK           = 0x02,
    SECURITY_FLAG_AUTHENTICATOR = 0x04
};

enum eAuthCmd
{
    AUTH_LOGON_CHALLENGE                         = 0x00,
    AUTH_LOGON_PROOF                             = 0x01,
    AUTH_RECONNECT_CHALLENGE                     = 0x02,
    AUTH_RECONNECT_PROOF                         = 0x03,
    REALM_LIST                                   = 0x10,
    XFER_INITIATE                                = 0x30,
    XFER_DATA                                    = 0x31,
    XFER_ACCEPT                                  = 0x32,
    XFER_RESUME                                  = 0x33,
    XFER_CANCEL                                  = 0x34
};

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some paltform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

typedef struct AUTH_LOGON_CHALLENGE_C
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint8   gamename[4];
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint8   platform[4];
    uint8   os[4];
    uint8   country[4];
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[1];
} sAuthLogonChallenge_C;

typedef struct AUTH_LOGON_PROOF_C
{
    uint8   cmd;
    uint8   A[32];
    Crypto::SHA1::Digest M1, crc_hash;
    uint8   number_of_keys;
    uint8   securityFlags;                                  // 0x00-0x04
} sAuthLogonProof_C;

typedef struct AUTH_LOGON_PROOF_S
{
    uint8   cmd;
    uint8   error;
    Crypto::SHA1::Digest M2;
    uint32  unk1;
    uint32  unk2;
    uint16  unk3;
} sAuthLogonProof_S;

typedef struct AUTH_LOGON_PROOF_S_OLD
{
    uint8   cmd;
    uint8   error;
    Crypto::SHA1::Digest M2;
    uint32  unk2;
} sAuthLogonProof_S_Old;

typedef struct AUTH_RECONNECT_PROOF_C
{
    uint8   cmd;
    uint8   R1[16];
    Crypto::SHA1::Digest R2, R3;
    uint8   number_of_keys;
} sAuthReconnectProof_C;

typedef struct XFER_INIT
{
    uint8 cmd;                                              // XFER_INITIATE
    uint8 fileNameLen;                                      // strlen(fileName);
    uint8 fileName[5];                                      // fileName[fileNameLen]
    uint64 file_size;                                       // file size (bytes)
    uint8 md5[MD5_DIGEST_LENGTH];                           // MD5
} XFER_INIT;

typedef struct XFER_DATA
{
    uint8 opcode;
    uint16 data_size;
    uint8 data[ChunkSize];
} XFER_DATA_STRUCT;

typedef struct AuthHandler
{
    eAuthCmd cmd;
    uint32 status;
    bool (AuthSocket::*handler)(void);
} AuthHandler;

std::array<uint8, 16> VersionChallenge = { { 0xBA, 0xA3, 0x1E, 0x99, 0xA0, 0x0B, 0x21, 0x57, 0xFC, 0x37, 0x3F, 0xB3, 0x69, 0xCD, 0xD2, 0xF1 } };

struct BufferSizes
{
    static constexpr size_t SRP_6_V = 0x20;
    static constexpr size_t SRP_6_S = 0x20;
};

#define MAX_ACCEPTED_CHALLENGE_SIZE (sizeof(AUTH_LOGON_CHALLENGE_C) + 16)
#define AUTH_LOGON_CHALLENGE_INITIAL_SIZE 4
#define REALM_LIST_PACKET_SIZE 5

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some paltform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

// Launch a thread to transfer a patch to the client
class PatcherRunnable: public Warhead::Runnable
{
public:
    PatcherRunnable(class AuthSocket*);
    void run();

private:
    AuthSocket* mySocket;
};

typedef struct PATCH_INFO
{
    uint8 md5[MD5_DIGEST_LENGTH];
} PATCH_INFO;

// Caches MD5 hash of client patches present on the server
class Patcher
{
public:
    typedef std::map<std::string, PATCH_INFO*> Patches;
    ~Patcher();
    Patcher();
    Patches::const_iterator begin() const { return _patches.begin(); }
    Patches::const_iterator end() const { return _patches.end(); }
    void LoadPatchMD5(char*);
    bool GetHash(char* pat, uint8 mymd5[16]);

private:
    void LoadPatchesInfo();
    Patches _patches;
};

const AuthHandler table[] =
{
    { AUTH_LOGON_CHALLENGE,     STATUS_CHALLENGE,   &AuthSocket::_HandleLogonChallenge      },
    { AUTH_LOGON_PROOF,         STATUS_LOGON_PROOF, &AuthSocket::_HandleLogonProof          },
    { AUTH_RECONNECT_CHALLENGE, STATUS_CHALLENGE,   &AuthSocket::_HandleReconnectChallenge  },
    { AUTH_RECONNECT_PROOF,     STATUS_RECON_PROOF, &AuthSocket::_HandleReconnectProof      },
    { REALM_LIST,               STATUS_AUTHED,      &AuthSocket::_HandleRealmList           },
    { XFER_ACCEPT,              STATUS_PATCH,       &AuthSocket::_HandleXferAccept          },
    { XFER_RESUME,              STATUS_PATCH,       &AuthSocket::_HandleXferResume          },
    { XFER_CANCEL,              STATUS_PATCH,       &AuthSocket::_HandleXferCancel          }
};

#define AUTH_TOTAL_COMMANDS 8

// Holds the MD5 hash of client patches present on the server
Patcher PatchesCache;

// Constructor - set the N and g values for SRP6
AuthSocket::AuthSocket(RealmSocket& socket) :
    pPatch(nullptr), socket_(socket), _status(STATUS_CHALLENGE), _build(0),
    _expversion(0), _accountSecurityLevel(SEC_PLAYER) { }

// Close patch file descriptor before leaving
AuthSocket::~AuthSocket(void) { }

// Accept the connection
void AuthSocket::OnAccept(void)
{
    LOG_INFO("network", "'%s:%d' Accepting connection", socket().getRemoteAddress().c_str(), socket().getRemotePort());
}

void AuthSocket::OnClose(void)
{
    LOG_TRACE("network", "AuthSocket::OnClose");
}

// Read the packet from the client
void AuthSocket::OnRead()
{
#define MAX_AUTH_LOGON_CHALLENGES_IN_A_ROW 3
    uint32 challengesInARow = 0;

#define MAX_AUTH_GET_REALM_LIST 10
    uint32 challengesInARowRealmList = 0;

    uint8 _cmd;
    while (1)
    {
        if (!socket().recv_soft((char*)&_cmd, 1))
            return;

        if (_cmd == AUTH_LOGON_CHALLENGE)
        {
            ++challengesInARow;
            if (challengesInARow == MAX_AUTH_LOGON_CHALLENGES_IN_A_ROW)
            {
                LOG_INFO("network", "Got %u AUTH_LOGON_CHALLENGE in a row from '%s', possible ongoing DoS", challengesInARow, socket().getRemoteAddress().c_str());
                socket().shutdown();
                return;
            }
        }
        else if (_cmd == REALM_LIST)
        {
            challengesInARowRealmList++;
            if (challengesInARowRealmList == MAX_AUTH_GET_REALM_LIST)
            {
                LOG_INFO("network", "Got %u REALM_LIST in a row from '%s', possible ongoing DoS", challengesInARowRealmList, socket().getRemoteAddress().c_str());
                socket().shutdown();
                return;
            }
        }

        size_t i;

        // Circle through known commands and call the correct command handler
        for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
        {
            if ((uint8)table[i].cmd == _cmd && (table[i].status == _status))
            {
                LOG_DEBUG("network", "Got data for cmd %u recv length %u", (uint32)_cmd, (uint32)socket().recv_len());

                if (!(*this.*table[i].handler)())
                {
                    LOG_DEBUG("network", "Command handler failed for cmd %u recv length %u", (uint32)_cmd, (uint32)socket().recv_len());
                    return;
                }

                break;
            }
        }

        // Report unknown packets in the error log
        if (i == AUTH_TOTAL_COMMANDS)
        {
            LOG_DEBUG("network", "Got unknown packet from '%s'", socket().getRemoteAddress().c_str());
            socket().shutdown();
            return;
        }
    }
}

std::map<std::string, uint32> LastLoginAttemptTimeForIP;
uint32 LastLoginAttemptCleanTime = 0;
std::mutex LastLoginAttemptMutex;

// Logon Challenge command handler
bool AuthSocket::_HandleLogonChallenge()
{
    LOG_TRACE("network", "Entering _HandleLogonChallenge");

    if (socket().recv_len() < sizeof(sAuthLogonChallenge_C))
        return false;

    ///- Session is closed unless overriden
    _status = STATUS_CLOSED;

    // Read the first 4 bytes (header) to get the length of the remaining of the packet
    std::vector<uint8> buf;
    buf.resize(4);

    socket().recv((char*)&buf[0], 4);
    EndianConvertPtr<uint16>(&buf[0]);
    uint16 remaining = ((sAuthLogonChallenge_C*)&buf[0])->size;
    LOG_DEBUG("network", "[AuthChallenge] got header, body is %#04x bytes", remaining);
    if ((remaining < sizeof(sAuthLogonChallenge_C) - buf.size()) || (socket().recv_len() < remaining))
        return false;

    //No big fear of memory outage (size is int16, i.e. < 65536)
    buf.resize(remaining + buf.size() + 1);
    buf[buf.size() - 1] = 0;
    sAuthLogonChallenge_C* ch = (sAuthLogonChallenge_C*)&buf[0];

    // Read the remaining of the packet
    socket().recv((char*)&buf[4], remaining);

    LOG_DEBUG("network", "[AuthChallenge] got full packet, %#04x bytes", ch->size);
    LOG_DEBUG("network", "[AuthChallenge] name(%d): '%s'", ch->I_len, ch->I);

    // BigEndian code, nop in little endian case
    // size already converted
    EndianConvertPtr<uint32>(&ch->gamename[0]);
    EndianConvert(ch->build);
    EndianConvertPtr<uint32>(&ch->platform[0]);
    EndianConvertPtr<uint32>(&ch->os[0]);
    EndianConvertPtr<uint32>(&ch->country[0]);
    EndianConvert(ch->timezone_bias);
    EndianConvert(ch->ip);

    ByteBuffer pkt;

    _login = (const char*)ch->I;
    _build = ch->build;
    _expversion = uint8(AuthHelper::IsPostBCAcceptedClientBuild(_build) ? POST_BC_EXP_FLAG : (AuthHelper::IsPreBCAcceptedClientBuild(_build) ? PRE_BC_EXP_FLAG : NO_VALID_EXP_FLAG));
    _os = (const char*)ch->os;

    if (_os.size() > 4)
        return false;

    // Restore string order as its byte order is reversed
    std::reverse(_os.begin(), _os.end());

    pkt << uint8(AUTH_LOGON_CHALLENGE);
    pkt << uint8(0x00);

    // Verify that this IP is not in the ip_banned table
    LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_DEL_EXPIRED_IP_BANS));

    std::string const& ip_address = socket().getRemoteAddress();
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_IP_BANNED);
    stmt->setString(0, ip_address);
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (result)
    {
        pkt << uint8(WOW_FAIL_BANNED);

        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] Banned ip tries to login!", socket().getRemoteAddress().c_str(), socket().getRemotePort());
    }
    else
    {
        // Get the account details from the account table
        // No SQL injection (prepared statement)
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGONCHALLENGE);
        stmt->setString(0, _login);

        PreparedQueryResult res2 = LoginDatabase.Query(stmt);
        if (res2)
        {
            Field* fields = res2->Fetch();

            // If the IP is 'locked', check that the player comes indeed from the correct IP address
            bool locked = false;
            if (fields[2].GetUInt8() == 1)                  // if ip is locked
            {
                LOG_DEBUG("network", "[AuthChallenge] Account '%s' is locked to IP - '%s'", _login.c_str(), fields[3].GetCString());
                LOG_DEBUG("network", "[AuthChallenge] Player address is '%s'", ip_address.c_str());

                if (strcmp(fields[4].GetCString(), ip_address.c_str()) != 0)
                {
                    LOG_DEBUG("network", "[AuthChallenge] Account IP differs");
                    pkt << uint8(WOW_FAIL_LOCKED_ENFORCED);
                    locked = true;
                }
                else
                    LOG_DEBUG("network", "[AuthChallenge] Account IP matches");
            }
            else
            {
                LOG_DEBUG("network", "[AuthChallenge] Account '%s' is not locked to ip", _login.c_str());
                std::string accountCountry = fields[3].GetString();
                if (accountCountry.empty() || accountCountry == "00")
                    LOG_DEBUG("network", "[AuthChallenge] Account '%s' is not locked to country", _login.c_str());
                else if (!accountCountry.empty())
                {
                    uint32 ip = inet_addr(ip_address.c_str());
                    EndianConvertReverse(ip);

                    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGON_COUNTRY);
                    stmt->setUInt32(0, ip);
                    if (PreparedQueryResult sessionCountryQuery = LoginDatabase.Query(stmt))
                    {
                        std::string loginCountry = (*sessionCountryQuery)[0].GetString();
                        LOG_DEBUG("network", "[AuthChallenge] Account '%s' is locked to country: '%s' Player country is '%s'", _login.c_str(), accountCountry.c_str(), loginCountry.c_str());
                        if (loginCountry != accountCountry)
                        {
                            LOG_DEBUG("network", "[AuthChallenge] Account country differs.");
                            pkt << uint8(WOW_FAIL_UNLOCKABLE_LOCK);
                            locked = true;
                        }
                        else
                            LOG_DEBUG("network", "[AuthChallenge] Account country matches");
                    }
                    else
                        LOG_DEBUG("network", "[AuthChallenge] IP2NATION Table empty");
                }
            }

            if (!locked)
            {
                //set expired bans to inactive
                LoginDatabase.DirectExecute(LoginDatabase.GetPreparedStatement(LOGIN_UPD_EXPIRED_ACCOUNT_BANS));

                // If the account is banned, reject the logon attempt
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BANNED);
                stmt->setUInt32(0, fields[1].GetUInt32());
                PreparedQueryResult banresult = LoginDatabase.Query(stmt);
                if (banresult)
                {
                    if ((*banresult)[0].GetUInt32() == (*banresult)[1].GetUInt32())
                    {
                        pkt << uint8(WOW_FAIL_BANNED);
                        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] Banned account %s tried to login!", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str ());
                    }
                    else
                    {
                        pkt << uint8(WOW_FAIL_SUSPENDED);
                        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] Temporarily banned account %s tried to login!", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str ());
                    }
                }
                else
                {
                    // Get the password from the account table, upper it, and make the SRP6 calculation
                    std::string rI = fields[0].GetString();

                    // Don't calculate (v, s) if there are already some in the database
                    std::string databaseV = fields[6].GetString();
                    std::string databaseS = fields[7].GetString();

                    LOG_DEBUG("network", "database authentication values: v='%s' s='%s'", databaseV.c_str(), databaseS.c_str());

                    // multiply with 2 since bytes are stored as hexstring
                    if (databaseV.size() != size_t(BufferSizes::SRP_6_V) * 2 || databaseS.size() != size_t(BufferSizes::SRP_6_S) * 2)
                        SetVSFields(rI);
                    else
                    {
                        s.SetHexStr(databaseS.c_str());
                        v.SetHexStr(databaseV.c_str());
                    }

                    b.SetRand(19 * 8);
                    BigNumber gmod = g.ModExp(b, N);
                    B = ((v * 3) + gmod) % N;

                    ASSERT(gmod.GetNumBytes() <= 32);

                    // Fill the response packet with the result
                    if (AuthHelper::IsAcceptedClientBuild(_build))
                        pkt << uint8(WOW_SUCCESS);
                    else
                        pkt << uint8(WOW_FAIL_VERSION_INVALID);

                    // B may be calculated < 32B so we force minimal length to 32B
                    pkt.append(B.ToByteArray<32>());      // 32 bytes
                    pkt << uint8(1);
                    pkt.append(g.ToByteArray<1>());
                    pkt << uint8(32);
                    pkt.append(N.ToByteArray<32>());
                    pkt.append(s.ToByteArray<BufferSizes::SRP_6_S>());   // 32 bytes
                    pkt.append(VersionChallenge.data(), VersionChallenge.size());
                    uint8 securityFlags = 0;

                    // Check if token is used
                    _tokenKey = fields[8].GetString();
                    if (!_tokenKey.empty())
                        securityFlags = 4;

                    pkt << uint8(securityFlags);            // security flags (0x0...0x04)

                    if (securityFlags & SECURITY_FLAG_PIN)          // PIN input
                    {
                        pkt << uint32(0);
                        pkt << uint64(0);
                        pkt << uint64(0);
                    }

                    if (securityFlags & SECURITY_FLAG_UNK)          // Matrix input
                    {
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint64(0);
                    }

                    if (securityFlags & SECURITY_FLAG_AUTHENTICATOR)    // Authenticator input
                        pkt << uint8(1);s

                    uint8 secLevel = fields[5].GetUInt8();
                    _accountSecurityLevel = secLevel <= SEC_ADMINISTRATOR ? AccountTypes(secLevel) : SEC_ADMINISTRATOR;

                    _localizationName.resize(4);

                    for (int i = 0; i < 4; ++i)
                        _localizationName[i] = ch->country[4 - i - 1];

                    LOG_DEBUG("network", "'%s:%d' [AuthChallenge] account %s is using '%c%c%c%c' locale (%u)", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str (), ch->country[3], ch->country[2], ch->country[1], ch->country[0], GetLocaleByName(_localizationName));

                    ///- All good, await client's proof
                    _status = STATUS_LOGON_PROOF;
                }
            }
        }
        else                                                //no account
            pkt << uint8(WOW_FAIL_UNKNOWN_ACCOUNT);
    }

    socket().send((char const*)pkt.contents(), pkt.size());
    return true;
}

// Logon Proof command handler
bool AuthSocket::_HandleLogonProof()
{
    LOG_TRACE("network", "Entering _HandleLogonProof");

    // Read the packet
    sAuthLogonProof_C lp;

    if (!socket().recv((char*)&lp, sizeof(sAuthLogonProof_C)))
        return false;

    _status = STATUS_CLOSED;

    // If the client has no valid version
    if (_expversion == NO_VALID_EXP_FLAG)
    {
        // Check if we have the appropriate patch on the disk
        LOG_DEBUG("network", "Client with invalid version, patching is not implemented");
        socket().shutdown();
        return true;
    }

    // Continue the SRP6 calculation based on data received from the client
    BigNumber A;

    A.SetBinary(lp.A, 32);

    // SRP safeguard: abort if A == 0
    if ((A % N).isZero())
    {
        socket().shutdown();
        return true;
    }

    BigNumber u(Crypto::SHA1::GetDigestOf(A.ToByteArray<32>(), B.ToByteArray<32>()));
    BigNumber S = (A * (v.ModExp(u, N))).ModExp(b, N);

    std::array<uint8, 32> t = S.ToByteArray<32>();
    std::array<uint8, 16> buf;
    Crypto::SHA1::Digest part;
    std::array<uint8, 40> sessionKey;

    for (size_t i = 0; i < 16; ++i)
        buf[i] = t[i * 2];
    part = Crypto::SHA1::GetDigestOf(buf);
    for (size_t i = 0; i < Crypto::SHA1::DIGEST_LENGTH; ++i)
        sessionKey[i * 2] = part[i];

    for (size_t i = 0; i < 16; ++i)
        buf[i] = t[i * 2 + 1];
    part = Crypto::SHA1::GetDigestOf(buf);
    for (size_t i = 0; i < Crypto::SHA1::DIGEST_LENGTH; ++i)
        sessionKey[i * 2 + 1] = part[i];

    Crypto::SHA1::Digest hash = Crypto::SHA1::GetDigestOf(N.ToByteArray<32>());
    Crypto::SHA1::Digest hash2 = Crypto::SHA1::GetDigestOf(g.ToByteArray<1>());
    std::transform(hash.begin(), hash.end(), hash2.begin(), hash.begin(), std::bit_xor<>()); // hash = H(N) xor H(g)

    Crypto::SHA1 sha;
    sha.UpdateData(hash);
    sha.UpdateData(Crypto::SHA1::GetDigestOf(_accountInfo.Login));
    sha.UpdateData(s.ToByteArray<BufferSizes::SRP_6_S>());
    sha.UpdateData(A.ToByteArray<32>());
    sha.UpdateData(B.ToByteArray<32>());
    sha.UpdateData(sessionKey);
    sha.Finalize();

    Crypto::SHA1::Digest M = sha.GetDigest();

    // Check if SRP6 results match (password is correct), else send an error
    if (M == lp.M1)
    {
        LOG_DEBUG("network", "'%s:%d' User '%s' successfully authenticated", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());

        // Update the sessionkey, last_ip, last login time and reset number of failed logins in the account table for this account
        // No SQL injection (escaped user name) and IP address as received by socket
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGONPROOF);
        stmt->setString(0, ByteArrayToHexStr(sessionKey));
        stmt->setString(1, socket().getRemoteAddress().c_str());
        stmt->setUInt32(2, GetLocaleByName(_localizationName));
        stmt->setString(3, _os);
        stmt->setString(4, _login);
        LoginDatabase.DirectExecute(stmt);

        // Finish SRP6 and send the final result to the client
        Crypto::SHA1::Digest M2 = Crypto::SHA1::GetDigestOf(A.ToByteArray<32>(), M, sessionKey);

        // Check auth token
        if ((lp.securityFlags & 0x04) || !_tokenKey.empty())
        {
            uint8 size;
            socket().recv((char*)&size, 1);
            char* token = new char[size + 1];
            token[size] = '\0';
            socket().recv(token, size);
            unsigned int validToken = TOTP::GenerateToken(_tokenKey.c_str());
            unsigned int incomingToken = atoi(token);
            delete[] token;
            if (validToken != incomingToken)
            {
                char data[] = { AUTH_LOGON_PROOF, WOW_FAIL_UNKNOWN_ACCOUNT, 3, 0 };
                socket().send(data, sizeof(data));
                return false;
            }
        }

        if (_expversion & POST_BC_EXP_FLAG)                 // 2.x and 3.x clients
        {
            sAuthLogonProof_S proof;
            proof.M2 = M2;
            proof.cmd = AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.unk1 = 0x00800000;    // Accountflags. 0x01 = GM, 0x08 = Trial, 0x00800000 = Pro pass (arena tournament)
            proof.unk2 = 0x00;          // SurveyId
            proof.unk3 = 0x00;
            socket().send((char*)&proof, sizeof(proof));
        }
        else
        {
            sAuthLogonProof_S_Old proof;
            proof.M2 = M2;
            proof.cmd = AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.unk2 = 0x00;
            socket().send((char*)&proof, sizeof(proof));
        }

        ///- Set _status to authed!
        _status = STATUS_AUTHED;
    }
    else
    {
        char data[4] = { AUTH_LOGON_PROOF, WOW_FAIL_UNKNOWN_ACCOUNT, 3, 0 };
        socket().send(data, sizeof(data));

        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] account %s tried to login with invalid password!", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());

        uint32 MaxWrongPassCount = sConfigMgr->GetIntDefault("WrongPass.MaxCount", 0);

        // We can not include the failed account login hook. However, this is a workaround to still log this.
        if (sConfigMgr->GetBoolDefault("WrongPass.Logging", false))
        {
            PreparedStatement* logstmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_FALP_IP_LOGGING);
            logstmt->setString(0, _login);
            logstmt->setString(1, socket().getRemoteAddress());
            logstmt->setString(2, "Logged on failed AccountLogin due wrong password");

            LoginDatabase.Execute(logstmt);
        }

        if (MaxWrongPassCount > 0)
        {
            //Increment number of failed logins by one and if it reaches the limit temporarily ban that account or IP
            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_FAILEDLOGINS);
            stmt->setString(0, _login);
            LoginDatabase.Execute(stmt);

            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_FAILEDLOGINS);
            stmt->setString(0, _login);

            if (PreparedQueryResult loginfail = LoginDatabase.Query(stmt))
            {
                uint32 failed_logins = (*loginfail)[1].GetUInt32();

                if (failed_logins >= MaxWrongPassCount)
                {
                    uint32 WrongPassBanTime = sConfigMgr->GetIntDefault("WrongPass.BanTime", 600);
                    bool WrongPassBanType = sConfigMgr->GetBoolDefault("WrongPass.BanType", false);

                    if (WrongPassBanType)
                    {
                        uint32 acc_id = (*loginfail)[0].GetUInt32();
                        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_AUTO_BANNED);
                        stmt->setUInt32(0, acc_id);
                        stmt->setUInt32(1, WrongPassBanTime);
                        LoginDatabase.Execute(stmt);

                        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] account %s got banned for '%u' seconds because it failed to authenticate '%u' times", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str(), WrongPassBanTime, failed_logins);
                    }
                    else
                    {
                        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_IP_AUTO_BANNED);
                        stmt->setString(0, socket().getRemoteAddress());
                        stmt->setUInt32(1, WrongPassBanTime);
                        LoginDatabase.Execute(stmt);

                        LOG_DEBUG("network", "'%s:%d' [AuthChallenge] IP %s got banned for '%u' seconds because account %s failed to authenticate '%u' times",
                                  socket().getRemoteAddress().c_str(), socket().getRemotePort(), socket().getRemoteAddress().c_str(), WrongPassBanTime, _login.c_str(), failed_logins);
                    }
                }
            }
        }
    }

    return true;
}

// Reconnect Challenge command handler
bool AuthSocket::_HandleReconnectChallenge()
{
    LOG_TRACE("network", "Entering _HandleReconnectChallenge");

    if (socket().recv_len() < sizeof(sAuthLogonChallenge_C))
        return false;

    // Read the first 4 bytes (header) to get the length of the remaining of the packet
    std::vector<uint8> buf;
    buf.resize(4);

    socket().recv((char*)&buf[0], 4);

    EndianConvertPtr<uint16>(&buf[0]);

    uint16 remaining = ((sAuthLogonChallenge_C*)&buf[0])->size;

    LOG_DEBUG("network", "[ReconnectChallenge] got header, body is %#04x bytes", remaining);

    if ((remaining < sizeof(sAuthLogonChallenge_C) - buf.size()) || (socket().recv_len() < remaining))
        return false;

    ///- Session is closed unless overriden
    _status = STATUS_CLOSED;

    // No big fear of memory outage (size is int16, i.e. < 65536)
    buf.resize(remaining + buf.size() + 1);
    buf[buf.size() - 1] = 0;
    sAuthLogonChallenge_C* ch = (sAuthLogonChallenge_C*)&buf[0];

    // Read the remaining of the packet
    socket().recv((char*)&buf[4], remaining);

    LOG_DEBUG("network", "[ReconnectChallenge] got full packet, %#04x bytes", ch->size);
    LOG_DEBUG("network", "[ReconnectChallenge] name(%d): '%s'", ch->I_len, ch->I);

    _login = (const char*)ch->I;

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SESSIONKEY);
    stmt->setString(0, _login);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    // Stop if the account is not found
    if (!result)
    {
        LOG_ERROR("network", "'%s:%d' [ERROR] user %s tried to login and we cannot find his session key in the database.", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
        socket().shutdown();
        return false;
    }

    // Reinitialize build, expansion and the account securitylevel
    _build = ch->build;
    _expversion = uint8(AuthHelper::IsPostBCAcceptedClientBuild(_build) ? POST_BC_EXP_FLAG : (AuthHelper::IsPreBCAcceptedClientBuild(_build) ? PRE_BC_EXP_FLAG : NO_VALID_EXP_FLAG));
    _os = (const char*)ch->os;

    if (_os.size() > 4)
        return false;

    // Restore string order as its byte order is reversed
    std::reverse(_os.begin(), _os.end());

    Field* fields = result->Fetch();
    uint8 secLevel = fields[2].GetUInt8();
    _accountSecurityLevel = secLevel <= SEC_ADMINISTRATOR ? AccountTypes(secLevel) : SEC_ADMINISTRATOR;

    HexStrToByteArray((*result)[0].GetCString(), sessionKey.data());

    ///- All good, await client's proof
    _status = STATUS_RECON_PROOF;

    // Sending response
    ByteBuffer pkt;
    pkt << uint8(AUTH_RECONNECT_CHALLENGE);
    pkt << uint8(0x00);
    Crypto::GetRandomBytes(_reconnectProof);    // 16 bytes random
    pkt.append(_reconnectProof);
    pkt << uint64(0x00) << uint64(0x00);        // 16 bytes zeros
    socket().send((char const*)pkt.contents(), pkt.size());
    return true;
}

// Reconnect Proof command handler
bool AuthSocket::_HandleReconnectProof()
{
    LOG_TRACE("network", "Entering _HandleReconnectProof");

    // Read the packet
    sAuthReconnectProof_C lp;
    if (!socket().recv((char*)&lp, sizeof(sAuthReconnectProof_C)))
        return false;

    _status = STATUS_CLOSED;

    if (_login.empty())
        return false;

    BigNumber t1;
    t1.SetBinary(lp.R1, 16);

    Crypto::SHA1 sha;
    sha.UpdateData(_login);
    sha.UpdateData(t1.ToByteArray<16>());
    sha.UpdateData(_reconnectProof);
    sha.UpdateData(sessionKey);
    sha.Finalize();

    if (sha.GetDigest() == reconnectProof->R2)
    {
        // Sending response
        ByteBuffer pkt;
        pkt << uint8(AUTH_RECONNECT_PROOF);
        pkt << uint8(0x00);
        pkt << uint16(0x00);                               // 2 bytes zeros
        socket().send((char const*)pkt.contents(), pkt.size());

        ///- Set _status to authed!
        _status = STATUS_AUTHED;

        return true;
    }
    else
    {
        LOG_ERROR("network", "'%s:%d' [ERROR] user %s tried to login, but session is invalid.", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
        socket().shutdown();
        return false;
    }
}

ACE_INET_Addr const& AuthSocket::GetAddressForClient(Realm const& realm, ACE_INET_Addr const& clientAddr)
{
    // Attempt to send best address for client
    if (clientAddr.is_loopback())
    {
        // Try guessing if realm is also connected locally
        if (realm.LocalAddress.is_loopback() || realm.ExternalAddress.is_loopback())
            return clientAddr;

        // Assume that user connecting from the machine that authserver is located on
        // has all realms available in his local network
        return realm.LocalAddress;
    }

    // Check if connecting client is in the same network
    if (IsIPAddrInNetwork(realm.LocalAddress, clientAddr, realm.LocalSubnetMask))
        return realm.LocalAddress;

    // Return external IP
    return realm.ExternalAddress;
}

// Realm List command handler
bool AuthSocket::_HandleRealmList()
{
    LOG_TRACE("network", "Entering _HandleRealmList");

    if (socket().recv_len() < 5)
        return false;

    socket().recv_skip(5);

    // Get the user id (else close the connection)
    // No SQL injection (prepared statement)
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_NAME);
    stmt->setString(0, _login);
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (!result)
    {
        LOG_ERROR("network", "'%s:%d' [ERROR] user %s tried to login but we cannot find him in the database.", socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
        socket().shutdown();
        return false;
    }

    Field* fields = result->Fetch();
    uint32 id = fields[0].GetUInt32();

    // Update realm list if need
    sRealmList->UpdateIfNeed();

    ACE_INET_Addr clientAddr;
    socket().peer().get_remote_addr(clientAddr);

    // Circle through realms in the RealmList and construct the return packet (including # of user characters in each realm)
    ByteBuffer pkt;

    size_t RealmListSize = 0;
    for (const auto& i : sRealmList)
    {
        const Realm& realm = i.second;
        // don't work with realms which not compatible with the client
        bool okBuild = ((_expversion & POST_BC_EXP_FLAG) && realm.gamebuild == _build) || ((_expversion & PRE_BC_EXP_FLAG) && !AuthHelper::IsPreBCAcceptedClientBuild(realm.gamebuild));

        // No SQL injection. id of realm is controlled by the database.
        uint32 flag = realm.flag;
        RealmBuildInfo const* buildInfo = AuthHelper::GetBuildInfo(realm.gamebuild);
        if (!okBuild)
        {
            if (!buildInfo)
                continue;

            flag |= REALM_FLAG_OFFLINE | REALM_FLAG_SPECIFYBUILD;   // tell the client what build the realm is for
        }

        if (!buildInfo)
            flag &= ~REALM_FLAG_SPECIFYBUILD;

        std::string name = i.first;
        if (_expversion & PRE_BC_EXP_FLAG && flag & REALM_FLAG_SPECIFYBUILD)
        {
            std::ostringstream ss;
            ss << name << " (" << buildInfo->MajorVersion << '.' << buildInfo->MinorVersion << '.' << buildInfo->BugfixVersion << ')';
            name = ss.str();
        }

        // We don't need the port number from which client connects with but the realm's port
        clientAddr.set_port_number(realm.ExternalAddress.get_port_number());

        uint8 lock = (realm.allowedSecurityLevel > _accountSecurityLevel) ? 1 : 0;

        uint8 AmountOfCharacters = 0;
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_NUM_CHARS_ON_REALM);
        stmt->setUInt32(0, realm.m_ID);
        stmt->setUInt32(1, id);
        result = LoginDatabase.Query(stmt);
        if (result)
            AmountOfCharacters = (*result)[0].GetUInt8();

        pkt << realm.icon;                                  // realm type
        if (_expversion & POST_BC_EXP_FLAG)                 // only 2.x and 3.x clients
            pkt << lock;                                    // if 1, then realm locked
        pkt << uint8(flag);                                 // RealmFlags
        pkt << name;
        pkt << GetAddressString(GetAddressForClient(realm, clientAddr));
        pkt << realm.populationLevel;
        pkt << AmountOfCharacters;
        pkt << realm.timezone;                              // realm category
        if (_expversion & POST_BC_EXP_FLAG)                 // 2.x and 3.x clients
            pkt << uint8(realm.m_ID);
        else
            pkt << uint8(0x0);                              // 1.12.1 and 1.12.2 clients

        if (_expversion & POST_BC_EXP_FLAG && flag & REALM_FLAG_SPECIFYBUILD)
        {
            pkt << uint8(buildInfo->MajorVersion);
            pkt << uint8(buildInfo->MinorVersion);
            pkt << uint8(buildInfo->BugfixVersion);
            pkt << uint16(buildInfo->Build);
        }

        ++RealmListSize;
    }

    if (_expversion & POST_BC_EXP_FLAG)                     // 2.x and 3.x clients
    {
        pkt << uint8(0x10);
        pkt << uint8(0x00);
    }
    else                                                    // 1.12.1 and 1.12.2 clients
    {
        pkt << uint8(0x00);
        pkt << uint8(0x02);
    }

    // make a ByteBuffer which stores the RealmList's size
    ByteBuffer RealmListSizeBuffer;
    RealmListSizeBuffer << uint32(0);
    if (_expversion & POST_BC_EXP_FLAG)                     // only 2.x and 3.x clients
        RealmListSizeBuffer << uint16(RealmListSize);
    else
        RealmListSizeBuffer << uint32(RealmListSize);

    ByteBuffer hdr;
    hdr << uint8(REALM_LIST);
    hdr << uint16(pkt.size() + RealmListSizeBuffer.size());
    hdr.append(RealmListSizeBuffer);                        // append RealmList's size buffer
    hdr.append(pkt);                                        // append realms in the realmlist

    socket().send((char const*)hdr.contents(), hdr.size());

    return true;
}

// Resume patch transfer
bool AuthSocket::_HandleXferResume()
{
    LOG_TRACE("network", "Entering _HandleXferResume");

    // Check packet length and patch existence
    if (socket().recv_len() < 9 || !pPatch) // FIXME: pPatch is never used
    {
        LOG_ERROR("network", "Error while resuming patch transfer (wrong packet)");
        return false;
    }

    // Launch a PatcherRunnable thread starting at given patch file offset
    uint64 start;
    socket().recv_skip(1);
    socket().recv((char*)&start, sizeof(start));
    fseek(pPatch, long(start), 0);

    Warhead::Thread u(new PatcherRunnable(this));
    return true;
}

// Cancel patch transfer
bool AuthSocket::_HandleXferCancel()
{
    LOG_TRACE("network", "Entering _HandleXferCancel");

    // Close and delete the socket
    socket().recv_skip(1);                                         //clear input buffer
    socket().shutdown();

    return true;
}

// Accept patch transfer
bool AuthSocket::_HandleXferAccept()
{
    LOG_TRACE("network", "Entering _HandleXferAccept");

    // Check packet length and patch existence
    if (!pPatch)
    {
        LOG_ERROR("network", "Error while accepting patch transfer (wrong packet)");
        return false;
    }

    // Launch a PatcherRunnable thread, starting at the beginning of the patch file
    socket().recv_skip(1);                                         // clear input buffer
    fseek(pPatch, 0, 0);

    Warhead::Thread u(new PatcherRunnable(this));
    return true;
}

// Make the SRP6 calculation from hash in dB
void AuthSocket::SetVSFields(const std::string& rI)
{
    s.SetRand(int32(BufferSizes::SRP_6_S) * 8);

    BigNumber I;
    I.SetHexStr(rI.c_str());

    // In case of leading zeros in the rI hash, restore them
    std::array<uint8, Crypto::SHA1::DIGEST_LENGTH> mDigest = I.ToByteArray<Crypto::SHA1::DIGEST_LENGTH>(false);

    Crypto::SHA1 sha;
    sha.UpdateData(s.ToByteArray<BufferSizes::SRP_6_S>());
    sha.UpdateData(mDigest);
    sha.Finalize();
    BigNumber x;
    x.SetBinary(sha.GetDigest());
    v = g.ModExp(x, N);

    // No SQL injection (username escaped)
    LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_VS);
    stmt->setString(0, v.AsHexStr());
    stmt->setString(1, s.AsHexStr());
    stmt->setString(2, _accountInfo.Login);
    LoginDatabase.Execute(stmt);
}

PatcherRunnable::PatcherRunnable(class AuthSocket* as)
{
    mySocket = as;
}

// Send content of patch file to the client
void PatcherRunnable::run() { }

// Preload MD5 hashes of existing patch files on server
#ifndef _WIN32
#include <dirent.h>
#include <errno.h>
void Patcher::LoadPatchesInfo()
{
    DIR* dirp;
    struct dirent* dp;
    dirp = opendir("./patches/");

    if (!dirp)
        return;

    while (dirp)
    {
        errno = 0;
        if ((dp = readdir(dirp)) != nullptr)
        {
            int l = strlen(dp->d_name);

            if (l < 8)
                continue;

            if (!memcmp(&dp->d_name[l - 4], ".mpq", 4))
                LoadPatchMD5(dp->d_name);
        }
        else
        {
            if (errno != 0)
            {
                closedir(dirp);
                return;
            }
            break;
        }
    }

    if (dirp)
        closedir(dirp);
}
#else
void Patcher::LoadPatchesInfo()
{
    WIN32_FIND_DATA fil;
    HANDLE hFil = FindFirstFile("./patches/*.mpq", &fil);
    if (hFil == INVALID_HANDLE_VALUE)
        return;                                             // no patches were found

    do
        LoadPatchMD5(fil.cFileName);
    while (FindNextFile(hFil, &fil));
}
#endif

// Calculate and store MD5 hash for a given patch file
void Patcher::LoadPatchMD5(char* szFileName)
{
    // Try to open the patch file
    std::string path = "./patches/";
    path += szFileName;
    FILE* pPatch = fopen(path.c_str(), "rb");

    LOG_DEBUG("network", "Loading patch info from %s\n", path.c_str());

    if (!pPatch)
    {
        LOG_ERROR("network", "Error loading patch %s\n", path.c_str());
        return;
    }

    // Calculate the MD5 hash
    MD5_CTX ctx;
    MD5_Init(&ctx);
    uint8* buf = new uint8[512 * 1024];

    while (!feof(pPatch))
    {
        size_t read = fread(buf, 1, 512 * 1024, pPatch);
        MD5_Update(&ctx, buf, read);
    }

    delete [] buf;
    fclose(pPatch);

    // Store the result in the internal patch hash map
    _patches[path] = new PATCH_INFO;
    MD5_Final((uint8*)&_patches[path]->md5, &ctx);
}

// Get cached MD5 hash for a given patch file
bool Patcher::GetHash(char* pat, uint8 mymd5[16])
{
    for (Patches::iterator i = _patches.begin(); i != _patches.end(); ++i)
        if (!stricmp(pat, i->first.c_str()))
        {
            memcpy(mymd5, i->second->md5, 16);
            return true;
        }

    return false;
}

// Launch the patch hashing mechanism on object creation
Patcher::Patcher()
{
    LoadPatchesInfo();
}

// Empty and delete the patch map on termination
Patcher::~Patcher()
{
    for (Patches::iterator i = _patches.begin(); i != _patches.end(); ++i)
        delete i->second;
}
