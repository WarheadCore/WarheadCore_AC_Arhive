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

#include "GameConfig.h"
#include "Config.h"
#include "Object.h"
#include "Log.h"
#include "World.h"

GameConfig* GameConfig::instance()
{
    static GameConfig instance;
    return &instance;
}

void GameConfig::Load(bool reload)
{
    LOG_INFO("config", "%s game configuraton:", reload ? "Reloading" : "Loading");

    LoadBoolConfigs(reload);
    LoadStringConfigs(reload);
    LoadIntConfigs(reload);
    LoadFloatConfigs(reload);

    LOG_INFO("config", "");
}

// Add's options
void GameConfig::AddBoolConfig(std::string const& optionName, bool const& def /*= false*/)
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr != _boolConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is already exists", optionName.c_str());
        return;
    }

    _boolConfigs.insert(std::make_pair(optionName, sConfigMgr->GetBoolDefault(optionName, def)));
}

void GameConfig::AddStringConfig(std::string const& optionName, std::string const& def /*= ""*/)
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr != _stringConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is already exists", optionName.c_str());
        return;
    }

    _stringConfigs.insert(std::make_pair(optionName, sConfigMgr->GetStringDefault(optionName, def)));
}

void GameConfig::AddIntConfig(std::string const& optionName, int32 const& def /*= 0*/)
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr != _intConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is already exists", optionName.c_str());
        return;
    }

    _intConfigs.insert(std::make_pair(optionName, sConfigMgr->GetIntDefault(optionName, def)));
}

void GameConfig::AddFloatConfig(std::string const& optionName, float const& def /*= 1.0f*/)
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr != _floatConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is already exists", optionName.c_str());
        return;
    }

    _floatConfigs.insert(std::make_pair(optionName, sConfigMgr->GetFloatDefault(optionName, def)));
}

bool GameConfig::GetBoolConfig(std::string const& optionName)
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr == _boolConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return false;
    }

    return _boolConfigs[optionName];
}

std::string GameConfig::GetStringConfig(std::string const& optionName)
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr == _stringConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return "";
    }

    return _stringConfigs[optionName];
}

int32 GameConfig::GetIntConfig(std::string const& optionName)
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr == _intConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return 0;
    }

    return _intConfigs[optionName];
}

float GameConfig::GetFloatConfig(std::string const& optionName)
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr == _floatConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return 1.0f;
    }

    return _floatConfigs[optionName];
}

// Sets
void GameConfig::SetBool(std::string const& optionName, bool const& value)
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr == _boolConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return;
    }

    _boolConfigs[optionName] = value;
}

void GameConfig::SetString(std::string const& optionName, std::string const& value)
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr == _stringConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return;
    }

    _stringConfigs[optionName] = value;
}

void GameConfig::SetInt(std::string const& optionName, int32 const& value)
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr == _intConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return;
    }

    _intConfigs[optionName] = value;
}

void GameConfig::SetFloat(std::string const& optionName, float const& value)
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr == _floatConfigs.end())
    {
        LOG_ERROR("config", "> Option (%s) is not exists", optionName.c_str());
        return;
    }

    _floatConfigs[optionName] = value;
}

// Loading
void GameConfig::LoadBoolConfigs(bool reload /*= false*/)
{
    if (reload)
        _boolConfigs.clear();

    AddBoolConfig("AllowTickets", true);
    AddBoolConfig("DeletedCharacterTicketTrace");
    AddBoolConfig("DurabilityLoss.InPvP");
    AddBoolConfig("AddonChannel", true);
    AddBoolConfig("CleanCharacterDB");
    AddBoolConfig("AllowPlayerCommands", true);
    AddBoolConfig("PreserveCustomChannels");
    AddBoolConfig("PlayerSave.Stats.SaveOnlyOnLogout", true);
    AddBoolConfig("CloseIdleConnections", true);

    AddBoolConfig("AllowTwoSide.Accounts", true);
    AddBoolConfig("AllowTwoSide.Interaction.Calendar");
    AddBoolConfig("AllowTwoSide.Interaction.Chat");
    AddBoolConfig("AllowTwoSide.Interaction.Channel");
    AddBoolConfig("AllowTwoSide.Interaction.Group");
    AddBoolConfig("AllowTwoSide.Interaction.Guild");
    AddBoolConfig("AllowTwoSide.Interaction.Auction");
    AddBoolConfig("AllowTwoSide.Interaction.Mail");
    AddBoolConfig("AllowTwoSide.WhoList");
    AddBoolConfig("AllowTwoSide.AddFriend");
    AddBoolConfig("AllowTwoSide.trade");
    AddBoolConfig("AllowTwoSide.Interaction.Emote");

    AddBoolConfig("AllFlightPaths");

    AddBoolConfig("Instance.IgnoreLevel");
    AddBoolConfig("Instance.IgnoreRaid");
    AddBoolConfig("Instance.GMSummonPlayer");
    AddBoolConfig("Instance.SharedNormalHeroicId");

    AddBoolConfig("GM.LogTrade");
    AddBoolConfig("GM.AllowInvite");
    AddBoolConfig("GM.AllowFriend");
    AddBoolConfig("GM.LowerSecurity");

    AddBoolConfig("EnableLoginAfterDC", true);
    AddBoolConfig("DontCacheRandomMovementPaths", true);

    AddBoolConfig("SkillChance.Prospecting");
    AddBoolConfig("SkillChance.Milling");

    AddBoolConfig("SaveRespawnTimeImmediately", true);
    AddBoolConfig("ActivateWeather", true);
    AddBoolConfig("AlwaysMaxSkillForLevel");
    AddBoolConfig("Chat.MuteFirstLogin");
    AddBoolConfig("Quests.EnableQuestTracker");
    AddBoolConfig("Quests.IgnoreRaid");
    AddBoolConfig("Quests.IgnoreAutoAccept");
    AddBoolConfig("Quests.IgnoreAutoComplete");
    AddBoolConfig("DetectPosCollision", true);
    AddBoolConfig("Channel.RestrictedLfg", true);
    AddBoolConfig("Channel.SilentlyGMJoin");
    AddBoolConfig("TalentsInspecting", true);
    AddBoolConfig("ChatFakeMessagePreventing");

    AddBoolConfig("Death.CorpseReclaimDelay.PvP", true);
    AddBoolConfig("Death.CorpseReclaimDelay.PvE", true);
    AddBoolConfig("Death.Bones.World", true);
    AddBoolConfig("Death.Bones.BattlegroundOrArena", true);
    AddBoolConfig("Die.Command.Mode", true);

    // always use declined names in the russian client
    AddBoolConfig("DeclinedNames");

    AddBoolConfig("Battleground.CastDeserter", true);
    AddBoolConfig("Battleground.QueueAnnouncer.Enable");
    AddBoolConfig("Battleground.QueueAnnouncer.PlayerOnly");
    AddBoolConfig("Battleground.StoreStatistics.Enable");
    AddBoolConfig("Battleground.TrackDeserters.Enable");
    AddBoolConfig("Battleground.GiveXPForKills");
    AddBoolConfig("Battleground.DisableQuestShareInBG");
    AddBoolConfig("Battleground.DisableReadyCheckInBG");

    AddBoolConfig("Arena.AutoDistributePoints");
    AddBoolConfig("Arena.ArenaSeason.InProgress", true);
    AddBoolConfig("Arena.QueueAnnouncer.Enable");

    // AddBoolConfig("OffhandCheckAtSpellUnlearn", true); not used

    AddBoolConfig("ItemDelete.Method");
    AddBoolConfig("ItemDelete.Vendor");

    AddBoolConfig("vmap.enableIndoorCheck", true);
    AddBoolConfig("vmap.enableLOS", true);
    AddBoolConfig("vmap.enableHeight", true);
    AddBoolConfig("vmap.petLOS", true);

    AddBoolConfig("PlayerStart.AllSpells");
    AddBoolConfig("PlayerStart.MapsExplored");
    AddBoolConfig("PlayerStart.AllReputation");

    AddBoolConfig("AlwaysMaxWeaponSkill");
    AddBoolConfig("PvPToken.Enable");
    AddBoolConfig("NoResetTalentsCost");

    // AddBoolConfig("ShowKickInWorld"); //not used
    AddBoolConfig("ShowBanInWorld");
    AddBoolConfig("ShowMuteInWorld");

    AddBoolConfig("Warden.Enabled");
    AddBoolConfig("AutoBroadcast.On");

    // PlayerDump
    AddBoolConfig("PlayerDump.DisallowPaths", true);
    AddBoolConfig("PlayerDump.DisallowOverwrite", true);

    // Misc
    AddBoolConfig("MoveMaps.Enable", true);
    AddBoolConfig("Wintergrasp.Enable");
    AddBoolConfig("Minigob.Manabonk.Enable", true);

    AddBoolConfig("IsContinentTransport.Enabled", true);
    AddBoolConfig("IsPreloadedContinentTransport.Enabled");

    AddBoolConfig("Allow.IP.Based.Action.Logging");

    // Whether to use LoS from game objects
    AddBoolConfig("CheckGameObjectLoS", true);

    AddBoolConfig("Calculate.Creature.Zone.Area.Data");
    AddBoolConfig("Calculate.Gameoject.Zone.Area.Data");

    // Player can join LFG anywhere
    AddBoolConfig("LFG.Location.All");

    // Preload all grids of all non-instanced maps
    AddBoolConfig("PreloadAllNonInstancedMapGrids");

    // DB logging
    AddBoolConfig("EnableLogDB");

    AddBoolConfig("SetAllCreaturesWithWaypointMovementActive");

    LOG_INFO("config", "> Loaded %u bool configs", static_cast<uint32>(_boolConfigs.size()));
}

void GameConfig::LoadStringConfigs(bool reload /*= false*/)
{
    if (reload)
        _stringConfigs.clear();

    ///- Read the "Data" directory from the config file
    AddStringConfig("DataDir", "./");

    AddStringConfig("PlayerStart.String", "");
    AddStringConfig("Motd", "Welcome to an WarheadCore server");

    LOG_INFO("config", "> Loaded %u string configs", static_cast<uint32>(_stringConfigs.size()));
}

void GameConfig::LoadIntConfigs(bool reload /*= false*/)
{
    std::unordered_map<std::string, int32> _notChangeConfigs;

    if (reload)
    {
        _notChangeConfigs =
        {
            { "WorldServerPort", GetIntConfig("WorldServerPort") },
            { "GameType", GetIntConfig("GameType") },
            { "RealmZone", GetIntConfig("RealmZone") },
            { "MaxPlayerLevel", GetIntConfig("MaxPlayerLevel") },
            { "Expansion", GetIntConfig("Expansion") }
        };
    }

    if (reload)
        _intConfigs.clear();

    AddIntConfig("Server.LoginInfo");
    AddIntConfig("RealmID", 1);
    AddIntConfig("Compression", 1);
    AddIntConfig("DBC.Locale", 255);
    AddIntConfig("PlayerLimit", 100);
    AddIntConfig("PersistentCharacterCleanFlags");

    AddIntConfig("ChatLevelReq.Channel", 1);
    AddIntConfig("ChatLevelReq.Whisper", 1);
    AddIntConfig("ChatLevelReq.Say", 1);

    AddIntConfig("PartyLevelReq", 1);
    AddIntConfig("LevelReq.Trade", 1);
    AddIntConfig("LevelReq.Ticket", 1);
    AddIntConfig("LevelReq.Auction", 1);
    AddIntConfig("LevelReq.Mail", 1);

    AddIntConfig("PreserveCustomChannelDuration", 14);
    AddIntConfig("DisconnectToleranceInterval");
    AddIntConfig("PlayerSave.Stats.MinLevel");
    AddIntConfig("MapUpdateInterval");
    AddIntConfig("ChangeWeatherInterval", 10 * MINUTE * IN_MILLISECONDS);

    AddIntConfig("WorldServerPort", 8085);
    AddIntConfig("GameType", 0);
    AddIntConfig("RealmZone", REALM_ZONE_DEVELOPMENT);

    AddIntConfig("SocketTimeOutTime", 900000);
    AddIntConfig("SocketTimeOutTimeActive", 60000);
    
    AddIntConfig("SessionAddDelay", 10000);

    AddIntConfig("StrictPlayerNames");
    AddIntConfig("StrictCharterNames");
    AddIntConfig("StrictChannelNames");
    AddIntConfig("StrictPetNames");

    AddIntConfig("MinPlayerName", 2);
    AddIntConfig("MinCharterName", 2);
    AddIntConfig("MinPetName", 2);

    AddIntConfig("CharacterCreating.Disabled");
    AddIntConfig("CharacterCreating.Disabled.RaceMask");
    AddIntConfig("CharacterCreating.Disabled.ClassMask");
    AddIntConfig("CharactersPerRealm", 10);
    AddIntConfig("CharactersPerAccount", 50);

    AddIntConfig("HeroicCharactersPerRealm", 1);
    AddIntConfig("CharacterCreating.MinLevelForHeroicCharacter", 55);
    AddIntConfig("SkipCinematics");
    AddIntConfig("MaxPlayerLevel", DEFAULT_MAX_LEVEL);
    AddIntConfig("MinDualSpecLevel", 40);

    AddIntConfig("StartPlayerLevel", 1);
    AddIntConfig("StartHeroicPlayerLevel", 55);
    AddIntConfig("StartPlayerMoney");
    AddIntConfig("MaxHonorPoints", 75000);
    AddIntConfig("StartHonorPoints");
    AddIntConfig("MaxArenaPoints", 10000);
    AddIntConfig("StartArenaPoints");
    AddIntConfig("RecruitAFriend.MaxLevel", 60);
    AddIntConfig("RecruitAFriend.MaxDifference", 4);
    AddIntConfig("InstantFlightPaths");
    AddIntConfig("Instance.ResetTimeHour", 4);
    AddIntConfig("Instance.ResetTimeRelativeTimestamp", 1135814400);
    AddIntConfig("Instance.UnloadDelay", 30 * MINUTE * IN_MILLISECONDS);
    AddIntConfig("MaxPrimaryTradeSkill", 2);
    AddIntConfig("MinPetitionSigns", 9);
    AddIntConfig("GM.LoginState", 2);
    AddIntConfig("GM.Visible", 2);
    AddIntConfig("GM.Chat", 2);
    AddIntConfig("GM.WhisperingTo", 2);
    AddIntConfig("GM.InGMList.Level", SEC_ADMINISTRATOR);
    AddIntConfig("GM.InWhoList.Level", SEC_ADMINISTRATOR);
    AddIntConfig("GM.StartLevel", 1);
    AddIntConfig("Visibility.GroupMode", 1);
    AddIntConfig("MailDeliveryDelay", HOUR);
    AddIntConfig("UpdateUptimeInterval", 10);

    AddIntConfig("LogDB.Opt.ClearInterval", 10);
    AddIntConfig("LogDB.Opt.ClearTime", 1209600);
    AddIntConfig("TeleportTimeoutNear", 25);
    AddIntConfig("TeleportTimeoutFar", 45);
    AddIntConfig("MaxAllowedMMRDrop", 500);

    AddIntConfig("SkillChance.Orange", 100);
    AddIntConfig("SkillChance.Yellow", 75);
    AddIntConfig("SkillChance.Green", 25);
    AddIntConfig("SkillChance.Grey");
    AddIntConfig("SkillChance.MiningSteps", 75);
    AddIntConfig("SkillChance.SkinningSteps", 75);
    AddIntConfig("SkillGain.Crafting", 1);
    AddIntConfig("SkillGain.Defense", 1);
    AddIntConfig("SkillGain.Gathering", 1);
    AddIntConfig("SkillGain.Weapon", 1);

    AddIntConfig("MaxOverspeedPings", 2);
    AddIntConfig("DisableWaterBreath", SEC_CONSOLE);
    AddIntConfig("Expansion", 2);

    AddIntConfig("ChatFlood.MessageCount", 10);
    AddIntConfig("ChatFlood.MessageDelay", 1);
    AddIntConfig("ChatFlood.MuteTime", 10);
    AddIntConfig("Chat.MuteTimeFirstLogin", 120);

    AddIntConfig("Event.Announce");
    AddIntConfig("CreatureFamilyAssistanceDelay", 1500);
    AddIntConfig("CreatureFamilyFleeDelay", 7000);

    AddIntConfig("WorldBossLevelDiff", 3);
    AddIntConfig("Quests.LowLevelHideDiff", 4);
    AddIntConfig("Quests.HighLevelHideDiff", 7);
    AddIntConfig("Battleground.Random.ResetHour", 6);
    AddIntConfig("Guild.ResetHour", 6);
    AddIntConfig("ChatStrictLinkChecking.Severity");
    AddIntConfig("ChatStrictLinkChecking.Kick");

    AddIntConfig("Corpse.Decay.NORMAL", 60);
    AddIntConfig("Corpse.Decay.RARE", 300);
    AddIntConfig("Corpse.Decay.ELITE", 300);
    AddIntConfig("Corpse.Decay.RAREELITE", 300);
    AddIntConfig("Corpse.Decay.WORLDBOSS", 3600);

    AddIntConfig("Death.SicknessLevel", 11);

    AddIntConfig("Battleground.PrematureFinishTimer", 5 * MINUTE * IN_MILLISECONDS);
    AddIntConfig("Battleground.InvitationType");
    AddIntConfig("Battleground.PremadeGroupWaitForMatch", 30 * MINUTE * IN_MILLISECONDS);

    AddIntConfig("Arena.MaxRatingDifference", 150);
    AddIntConfig("Arena.RatingDiscardTimer", 10 * MINUTE * IN_MILLISECONDS);
    AddIntConfig("Arena.AutoDistributeInterval", 7);
    AddIntConfig("Arena.ArenaSeason.ID", 1);
    AddIntConfig("Arena.ArenaStartRating");
    AddIntConfig("Arena.ArenaStartPersonalRating", 1000);
    AddIntConfig("Arena.ArenaStartMatchmakerRating", 1500);

    if (reload)
        AddIntConfig("ClientCacheVersion");

    AddIntConfig("Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS);
    AddIntConfig("Guild.BankEventLogRecordsCount", GUILD_BANKLOG_MAX_RECORDS);

    AddIntConfig("InstantLogout", SEC_MODERATOR);    

    ///- Load the CharDelete related config options
    AddIntConfig("CharDelete.Method");
    AddIntConfig("CharDelete.MinLevel");
    AddIntConfig("CharDelete.KeepDays", 30);

    ///- Load the ItemDelete related config options
    AddIntConfig("ItemDelete.Quality", 3);
    AddIntConfig("ItemDelete.ItemLevel", 80);

    AddIntConfig("HonorPointsAfterDuel");

    AddIntConfig("PvPToken.MapAllowType", 4);
    AddIntConfig("PvPToken.ItemID", 29434);
    AddIntConfig("PvPToken.ItemCount", 1);

    AddIntConfig("MapUpdate.Threads", 1);
    AddIntConfig("Command.LookupMaxResults");

    // Warden
    AddIntConfig("Warden.NumMemChecks", 3);
    AddIntConfig("Warden.NumOtherChecks", 7);
    AddIntConfig("Warden.BanDuration", DAY);
    AddIntConfig("Warden.ClientCheckHoldOff", 30);
    AddIntConfig("Warden.ClientCheckFailAction");
    AddIntConfig("Warden.ClientResponseDelay", 600);

    // Dungeon finder
    AddIntConfig("DungeonFinder.OptionsMask", 3);

    // Max instances per hour
    AddIntConfig("AccountInstancesPerHour", 5);

    // AutoBroadcast
    AddIntConfig("AutoBroadcast.Center");
    AddIntConfig("AutoBroadcast.Timer", 60000);

    // MySQL ping time interval
    AddIntConfig("MaxPingTime", 30);

    // Wintergrasp
    AddIntConfig("Wintergrasp.PlayerMax", 100);
    AddIntConfig("Wintergrasp.PlayerMin");
    AddIntConfig("Wintergrasp.PlayerMinLvl", 77);
    AddIntConfig("Wintergrasp.BattleTimer", 30);
    AddIntConfig("Wintergrasp.NoBattleTimer", 150);
    AddIntConfig("Wintergrasp.CrashRestartTimer", 10);
    AddIntConfig("BirthdayTime", 1222964635);

    // Prevent players AFK from being logged out
    AddIntConfig("PreventAFKLogout");

    // ICC buff override
    AddIntConfig("ICC.Buff.Horde", 73822);
    AddIntConfig("ICC.Buff.Alliance", 73828);

    // Packet spoof punishment
    AddIntConfig("PacketSpoof.Policy", 1);
    AddIntConfig("PacketSpoof.BanMode");
    AddIntConfig("PacketSpoof.BanDuration", 86400);

    //
    AddIntConfig("WaypointMovementStopTimeForPlayer", 120);

    // Calendar
    AddIntConfig("Calendar.DeleteOldEventsHour", 6);

    // Random Battleground Rewards
    AddIntConfig("Battleground.RewardWinnerHonorFirst", 30);
    AddIntConfig("Battleground.RewardWinnerArenaFirst", 25);
    AddIntConfig("Battleground.RewardWinnerHonorLast", 15);
    AddIntConfig("Battleground.RewardWinnerArenaLast");
    AddIntConfig("Battleground.RewardLoserHonorFirst", 5);
    AddIntConfig("Battleground.RewardLoserHonorLast", 5);

    // CharterCost
    AddIntConfig("Guild.CharterCost", 1000);
    AddIntConfig("ArenaTeam.CharterCost.2v2", 800000);
    AddIntConfig("ArenaTeam.CharterCost.3v3", 1200000);
    AddIntConfig("ArenaTeam.CharterCost.5v5", 2000000);

    // Check options can't be changed at worldserver.conf reload
    if (reload)
    {
        for (auto const& itr : _notChangeConfigs)
        {
            uint32 val = sConfigMgr->GetIntDefault(itr.first, itr.second);
            uint32 tempIntOption = itr.second;

            if (val != tempIntOption)
                LOG_ERROR("config", "%s option can't be changed at worldserver.conf reload, using current value (%u)", itr.first.c_str(), tempIntOption);

            SetInt(itr.first, tempIntOption);
        }
    }

    LOG_INFO("config", "> Loaded %u int configs", static_cast<uint32>(_intConfigs.size()));
}

void GameConfig::LoadFloatConfigs(bool reload /*= false*/)
{
    if (reload)
        _floatConfigs.clear();

    AddFloatConfig("Rate.Health");
    AddFloatConfig("Rate.Mana");

    AddFloatConfig("Rate.Rage.Income");
    AddFloatConfig("Rate.Rage.Loss");

    AddFloatConfig("Rate.RunicPower.Income");
    AddFloatConfig("Rate.RunicPower.Loss");

    AddFloatConfig("Rate.Focus");
    AddFloatConfig("Rate.Energy");
    AddFloatConfig("Rate.Skill.Discovery");

    // Drop rates
    AddFloatConfig("Rate.Drop.Item.Poor");
    AddFloatConfig("Rate.Drop.Item.Normal");
    AddFloatConfig("Rate.Drop.Item.Uncommon");
    AddFloatConfig("Rate.Drop.Item.Rare");
    AddFloatConfig("Rate.Drop.Item.Epic");
    AddFloatConfig("Rate.Drop.Item.Legendary");
    AddFloatConfig("Rate.Drop.Item.Artifact");
    AddFloatConfig("Rate.Drop.Item.Referenced");
    AddFloatConfig("Rate.Drop.Item.ReferencedAmount");
    AddFloatConfig("Rate.Drop.Money");

    // XP rates
    AddFloatConfig("Rate.XP.Kill");
    AddFloatConfig("Rate.XP.BattlegroundKill");
    AddFloatConfig("Rate.XP.Quest");
    AddFloatConfig("Rate.XP.Explore");

    AddFloatConfig("Rate.RepairCost");

    // Reputation rates
    AddFloatConfig("Rate.Reputation.Gain");
    AddFloatConfig("Rate.Reputation.LowLevel.Kill");
    AddFloatConfig("Rate.Reputation.LowLevel.Quest");
    AddFloatConfig("Rate.Reputation.RecruitAFriendBonus");

    // Creature damage rates
    AddFloatConfig("Rate.Creature.Normal.Damage");
    AddFloatConfig("Rate.Creature.Elite.Elite.Damage");
    AddFloatConfig("Rate.Creature.Elite.RAREELITE.Damage");
    AddFloatConfig("Rate.Creature.Elite.WORLDBOSS.Damage");
    AddFloatConfig("Rate.Creature.Elite.RARE.Damage");

    // Creature HP rates
    AddFloatConfig("Rate.Creature.Normal.HP");
    AddFloatConfig("Rate.Creature.Elite.Elite.HP");
    AddFloatConfig("Rate.Creature.Elite.RAREELITE.HP");
    AddFloatConfig("Rate.Creature.Elite.WORLDBOSS.HP");
    AddFloatConfig("Rate.Creature.Elite.RARE.HP");

    // Creature SpellDamage rates
    AddFloatConfig("Rate.Creature.Normal.SpellDamage");
    AddFloatConfig("Rate.Creature.Elite.Elite.SpellDamage");
    AddFloatConfig("Rate.Creature.Elite.RAREELITE.SpellDamage");
    AddFloatConfig("Rate.Creature.Elite.WORLDBOSS.SpellDamage");
    AddFloatConfig("Rate.Creature.Elite.RARE.SpellDamage");

    AddFloatConfig("Rate.Creature.Aggro");

    // Rest rates
    AddFloatConfig("Rate.Rest.InGame");
    AddFloatConfig("Rate.Rest.Offline.InTavernOrCity");
    AddFloatConfig("Rate.Rest.Offline.InWilderness");

    AddFloatConfig("Rate.Damage.Fall");

    // Auction rates
    AddFloatConfig("Rate.Auction.Time");
    AddFloatConfig("Rate.Auction.Deposit");
    AddFloatConfig("Rate.Auction.Cut");

    AddFloatConfig("Rate.Honor");
    AddFloatConfig("Rate.ArenaPoints");
    AddFloatConfig("Rate.InstanceResetTime");
    AddFloatConfig("Rate.Talent");
    AddFloatConfig("Rate.MoveSpeed");
    AddFloatConfig("Rate.Corpse.Decay.Looted", 0.5f);
    AddFloatConfig("TargetPosRecalculateRange", 1.5f);

    // DurabilityLoss rates
    AddFloatConfig("DurabilityLoss.OnDeath", 10.0f);
    AddFloatConfig("DurabilityLossChance.Damage", 0.5f);
    AddFloatConfig("DurabilityLossChance.Absorb", 0.5f);
    AddFloatConfig("DurabilityLossChance.Parry", 0.5f);
    AddFloatConfig("DurabilityLossChance.Block", 0.5f);

    AddFloatConfig("MaxGroupXPDistance", 74.0f);
    AddFloatConfig("MaxRecruitAFriendBonusDistance", 100.0f);
    AddFloatConfig("MonsterSight", 50.0f);
    AddFloatConfig("GM.TicketSystem.ChanceOfGMSurvey", 50.0f);

    AddFloatConfig("CreatureFamilyFleeAssistanceRadius", 30.0f);
    AddFloatConfig("CreatureFamilyAssistanceRadius", 10.0f);

    AddFloatConfig("ListenRange.Say", 25.0f);
    AddFloatConfig("ListenRange.TextEmote", 25.0f);
    AddFloatConfig("ListenRange.Yell", 300.0f);

    AddFloatConfig("Arena.ArenaWinRatingModifier1", 48.0f);
    AddFloatConfig("Arena.ArenaWinRatingModifier2", 24.0f);
    AddFloatConfig("Arena.ArenaLoseRatingModifier", 24.0f);
    AddFloatConfig("Arena.ArenaMatchmakerRatingModifier", 24.0f);

    // Visibility
    AddFloatConfig("Visibility.Distance.Continents", DEFAULT_VISIBILITY_DISTANCE);
    AddFloatConfig("Visibility.Distance.Instances", DEFAULT_VISIBILITY_INSTANCE);
    AddFloatConfig("Visibility.Distance.BGArenas", DEFAULT_VISIBILITY_BGARENAS);

    // Rate.SellValue.Item
    AddFloatConfig("Rate.SellValue.Item.Poor");
    AddFloatConfig("Rate.SellValue.Item.Normal");
    AddFloatConfig("Rate.SellValue.Item.Uncommon");
    AddFloatConfig("Rate.SellValue.Item.Rare");
    AddFloatConfig("Rate.SellValue.Item.Epic");
    AddFloatConfig("Rate.SellValue.Item.Legendary");
    AddFloatConfig("Rate.SellValue.Item.Artifact");
    AddFloatConfig("Rate.SellValue.Item.Heirloom");

    // Rate.BuyValue.Item
    AddFloatConfig("Rate.BuyValue.Item.Poor");
    AddFloatConfig("Rate.BuyValue.Item.Normal");
    AddFloatConfig("Rate.BuyValue.Item.Uncommon");
    AddFloatConfig("Rate.BuyValue.Item.Rare");
    AddFloatConfig("Rate.BuyValue.Item.Epic");
    AddFloatConfig("Rate.BuyValue.Item.Legendary");
    AddFloatConfig("Rate.BuyValue.Item.Artifact");
    AddFloatConfig("Rate.BuyValue.Item.Heirloom");

    LOG_INFO("config", "> Loaded %u float configs", static_cast<uint32>(_floatConfigs.size()));
}
