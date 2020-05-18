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

#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include <vector>

enum DonateIPSVariant
{
    DONATE_IPS_DONATE_MONETA = 9,
    DONATE_IPS_RENAME
};

struct DonateIPSStruct
{
    std::string Nickname;
    DonateIPSVariant Variant;
    uint32 ItemID;
    uint32 ItemCount;
    uint32 ID;
};

class DonateIPS
{
public:
    static DonateIPS* instance()
    {
        static DonateIPS instance;
        return &instance;
    }

    void LoadDonate()
    {
        if (!CONF_GET_BOOL("IPS.Donate.Enable"))
            return;

        _store.clear();

        QueryResult result = CharacterDatabase.PQuery("SELECT id, nickname, item_id, item_quantity FROM `shop_purchase` WHERE flag = 0");
        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();
            uint32 ID = fields[0].GetUInt32();
            std::string PlayerName = fields[1].GetString();
            DonateIPSVariant Variant = static_cast<DonateIPSVariant>(fields[2].GetUInt32());
            uint32 Count = fields[3].GetUInt32();

            if (!normalizePlayerName(PlayerName))
            {
                sLog->outErrorDb("> DonateIPS: Некорректное имя персонажа (%s)", PlayerName.c_str());
                continue;
            }

            uint64 PlayerGuid = sObjectMgr->GetPlayerGUIDByName(PlayerName);
            if (!PlayerGuid)
            {
                sLog->outErrorDb("> DonateIPS: Неверное имя персонажа (%s)", PlayerName.c_str());
                continue;
            }

            if (!sObjectMgr->GetPlayerNameByGUID(PlayerGuid, PlayerName))
            {
                sLog->outErrorDb("> DonateIPS: Ошибка получения данных о персонаже (%s - %u)", PlayerName.c_str(), PlayerGuid);
                continue;
            }

            DonateIPSStruct _data;
            _data.Nickname = PlayerName;
            _data.Variant = Variant;
            _data.ID = ID;

            switch (Variant)
            {
            case DONATE_IPS_DONATE_MONETA:
                _data.ItemID = 37711;
                _data.ItemCount = Count;
                break;
            case DONATE_IPS_RENAME:                
                break;
            default:
                sLog->outErrorDb("> DonateIPS: Неизвестный тип доната (%u)", static_cast<uint32>(Variant));
                continue;
            }

            _store.push_back(_data);

        } while (result->NextRow());
    }

    void SendDonate()
    {
        LoadDonate();

        for (auto const& itr : _store)
            SendRewardForPlayer(&itr);
    }

    void SendRewardForPlayer(const DonateIPSStruct* _data)
    {
        std::string ThanksText = "Спасибо за покупку!";
        std::string ThanksSubject = "Донат магазин";

        if (_data->Variant == DONATE_IPS_DONATE_MONETA)
        {
            // Add mail item
            CharacterDatabase.PExecute("INSERT INTO `mail_external` (PlayerName, Subject, ItemID, ItemCount, Message, CreatureEntry) VALUES ('%s', '%s', %u, %u, '%s', 37688)",
                _data->Nickname.c_str(), ThanksSubject.c_str(), _data->ItemID, _data->ItemCount, ThanksText.c_str());
        }
        else if (_data->Variant == DONATE_IPS_RENAME)
        {
            uint64 targetGuid = sObjectMgr->GetPlayerGUIDByName(_data->Nickname);
            if (!targetGuid)
                return;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_RENAME));
            stmt->setUInt32(1, GUID_LOPART(targetGuid));
            CharacterDatabase.Execute(stmt);
        }

        CharacterDatabase.PExecute("UPDATE `shop_purchase` SET `flag` = 1 WHERE `id` = %u", _data->ID);
    }

private:
    std::vector<DonateIPSStruct> _store;
};

#define sDonateIPS DonateIPS::instance()

class DonateIPS_World : public WorldScript
{
public:
    DonateIPS_World() : WorldScript("DonateIPS_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("IPS.Donate.Enable");
    }

    void OnUpdate(uint32 Diff) override
    {
        if (!CONF_GET_BOOL("IPS.Donate.Enable"))
            return;
        
        if (CheckTimeInterval < Diff)
        {
            sDonateIPS->SendDonate();
            CheckTimeInterval = TimeInterval;
        }
        else
            CheckTimeInterval -= Diff;
    }
private:
    uint32 TimeInterval = 15 * IN_MILLISECONDS;
    uint32 CheckTimeInterval = TimeInterval;
};

// Group all custom scripts
void AddSC_DonateIPS()
{
    new DonateIPS_World();
}
