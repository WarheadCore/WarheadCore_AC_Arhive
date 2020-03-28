/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#ifndef _CFBG_H_
#define _CFBG_H_

#include "Common.h"
#include "Player.h"
#include "Battleground.h"
#include "BattlegroundQueue.h"
#include <unordered_map>

enum FakeMorphs
{
    FAKE_F_TAUREN = 20584,
    FAKE_M_TAUREN = 20585,
    FAKE_M_NELF = 20318,
    FAKE_F_DRAENEI = 20323
};

struct FakePlayer
{
    // Fake
    uint8   FakeRace;
    uint32  FakeMorph;
    TeamId  FakeTeamID;

    // Real
    uint8   RealRace;
    uint32  RealMorph;
    TeamId  RealTeamID;
};

enum CFBGBattlegroundQueueGroupTypes
{
    BG_QUEUE_CFBG = 4
};

class CFBG
{
public:
    static CFBG* instance();

    void LoadConfig();

    bool IsEnableSystem();
    bool IsEnableAvgIlvl();
    uint32 GetMaxPlayersCountInGroup();

    uint32 GetBGTeamAverageItemLevel(Battleground* bg, TeamId team);
    uint32 GetAllPlayersCountInBG(Battleground* bg);

    TeamId GetLowerTeamIdInBG(Battleground* bg);
    TeamId GetLowerAvgIlvlTeamInBg(Battleground* bg);

    bool IsAvgIlvlTeamsInBgEqual(Battleground* bg);
    bool SendRealNameQuery(Player* player);
    bool IsPlayerFake(Player* player);
    bool ShouldForgetInListPlayers(Player* player);
    bool IsPlayingNative(Player* player);

    void ValidatePlayerForBG(Battleground* bg, Player* player, TeamId teamId);
    void SetFakeRaceAndMorph(Player* player);
    void SetFactionForRace(Player* player, uint8 Race);
    void ClearFakePlayer(Player* player);
    void DoForgetPlayersInList(Player* player);
    void FitPlayerInTeam(Player* player, bool action, Battleground* bg);
    void DoForgetPlayersInBG(Player* player, Battleground* bg);
    void SetForgetBGPlayers(Player* player, bool value);
    bool ShouldForgetBGPlayers(Player* player);
    void SetForgetInListPlayers(Player* player, bool value);
    void UpdateForget(Player* player);
    bool SendMessageQueue(BattlegroundQueue* bgqueue, Battleground* bg, PvPDifficultyEntry const* bracketEntry, Player* leader);

    bool FillPlayersToCFBGWithSpecific(BattlegroundQueue* bgqueue, Battleground* bg, const int32 aliFree, const int32 hordeFree, BattlegroundBracketId thisBracketId, BattlegroundQueue* specificQueue, BattlegroundBracketId specificBracketId);
    bool FillPlayersToCFBG(BattlegroundQueue* bgqueue, Battleground* bg, const int32 aliFree, const int32 hordeFree, BattlegroundBracketId bracket_id);

private:
    typedef std::unordered_map<Player*, FakePlayer> FakePlayersContainer;
    typedef std::unordered_map<Player*, uint64> FakeNamePlayersContainer;
    typedef std::unordered_map<Player*, bool> ForgetBGPlayersContainer;
    typedef std::unordered_map<Player*, bool> ForgetInListPlayersContainer;

    FakePlayersContainer _fakePlayerStore;
    FakeNamePlayersContainer _fakeNamePlayersStore;
    ForgetBGPlayersContainer _forgetBGPlayersStore;
    ForgetInListPlayersContainer _forgetInListPlayersStore;

    // For config
    bool _IsEnableSystem;
    bool _IsEnableAvgIlvl;
    uint32 _MaxPlayersCountInGroup;
};

#define sCFBG CFBG::instance()

#endif /* _CFBG_H_ */
