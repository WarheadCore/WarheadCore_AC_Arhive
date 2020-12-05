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
#include "Player.h"
#include "ServerMotd.h"
#include "World.h"
#include <unordered_map>

namespace
{
    std::unordered_map<std::string, bool> _boolConfigs;
    std::unordered_map<std::string, std::string> _stringConfigs;
    std::unordered_map<std::string, int32> _intConfigs;
    std::unordered_map<std::string, float> _floatConfigs;
}

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

    CheckOptions(reload);

    LOG_INFO("config", "");
}

template<typename T>
void GameConfig::AddOption(std::string const& optionName, std::optional<T> const& def /*= std::nullopt*/) const
{
    static_assert(false, "Unsupported config type option");
}

template<>
void GameConfig::AddOption<bool>(std::string const& optionName, std::optional<bool> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr != _boolConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is already exists", optionName.c_str());
        return;
    }

    _boolConfigs.emplace(optionName, sConfigMgr->GetBoolDefault(optionName, def == std::nullopt ? false : *def));
}

template<>
void GameConfig::AddOption<std::string>(std::string const& optionName, std::optional<std::string> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr != _stringConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is already exists", optionName.c_str());
        return;
    }

    _stringConfigs.emplace(std::make_pair(optionName, sConfigMgr->GetStringDefault(optionName, def == std::nullopt ? "" : *def)));
}

template<>
void GameConfig::AddOption<int32>(std::string const& optionName, std::optional<int32> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr != _intConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is already exists", optionName.c_str());
        return;
    }

    _intConfigs.emplace(std::make_pair(optionName, sConfigMgr->GetIntDefault(optionName, def == std::nullopt ? 0 : *def)));
}

template<>
void GameConfig::AddOption<float>(std::string const& optionName, std::optional<float> const& def /*= std::nulloptf*/) const
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr != _floatConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is already exists", optionName.c_str());
        return;
    }

    _floatConfigs.emplace(std::make_pair(optionName, sConfigMgr->GetFloatDefault(optionName, def == std::nullopt ? 1.0f : *def)));
}

template<>
void GameConfig::AddOption<uint32>(std::string const& optionName, std::optional<uint32> const& def /*= std::nullopt*/) const
{
    AddOption<int32>(optionName, def);
}

template<>
void GameConfig::AddOption<double>(std::string const& optionName, std::optional<double> const& def /*= std::nullopt*/) const
{
    AddOption<float>(optionName, def);
}

template<typename T>
T GameConfig::GetOption(std::string const& optionName, std::optional<T> const& def /*= std::nullopt*/) const
{
    static_assert(false, "Unsupported config type option");
}

template<>
bool GameConfig::GetOption<bool>(std::string const& optionName, std::optional<bool> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr == _boolConfigs.end())
    {
        auto retValue = def == std::nullopt ? false : *def;
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists. Returned (%s)", optionName.c_str(), retValue ? "true" : "false");
        return retValue;
    }

    return _boolConfigs.at(optionName);
}

template<>
std::string GameConfig::GetOption<std::string>(std::string const& optionName, std::optional<std::string> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr == _stringConfigs.end())
    {
        auto retValue = def == std::nullopt ? "" : *def;
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists. Returned (%s)", optionName.c_str(), retValue);
        return retValue;
    }

    return _stringConfigs.at(optionName);
}

template<>
int32 GameConfig::GetOption<int32>(std::string const& optionName, std::optional<int32> const& def /*= std::nullopt*/) const
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr == _intConfigs.end())
    {
        auto retValue = def == std::nullopt ? 0 : *def;
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists. Returned (%u)", optionName.c_str(), retValue);
        return retValue;
    }

    return _intConfigs.at(optionName);
}

template<>
float GameConfig::GetOption<float>(std::string const& optionName, std::optional<float> const& def /*= std::nulloptf*/) const
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr == _floatConfigs.end())
    {
        auto retValue = def == std::nullopt ? 1.0f : *def;
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists. Returned (%f)", optionName.c_str(), retValue);
        return retValue;
    }

    return _floatConfigs.at(optionName);
}

template<>
uint32 GameConfig::GetOption<uint32>(std::string const& optionName, std::optional<uint32> const& def /*= std::nullopt*/) const
{
    return GetOption<int32>(optionName, def);
}

template<>
double GameConfig::GetOption<double>(std::string const& optionName, std::optional<double> const& def /*= std::nulloptf*/) const
{
    return GetOption<float>(optionName, def);
}

// Set option
template<typename T>
void GameConfig::SetOption(std::string const& optionName, T const& value) const
{
    static_assert(false, "Unsupported config type option");
}

template<>
void GameConfig::SetOption<bool>(std::string const& optionName, bool const& value) const
{
    // Check exist option
    auto itr = _boolConfigs.find(optionName);
    if (itr == _boolConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists", optionName.c_str());
        return;
    }

    _boolConfigs.erase(optionName);
    _boolConfigs.emplace(optionName, value);
}

template<>
void GameConfig::SetOption<std::string>(std::string const& optionName, std::string const& value) const
{
    // Check exist option
    auto itr = _stringConfigs.find(optionName);
    if (itr == _stringConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists", optionName.c_str());
        return;
    }

    _stringConfigs.erase(optionName);
    _stringConfigs.emplace(optionName, value);
}

template<>
void GameConfig::SetOption<int32>(std::string const& optionName, int32 const& value) const
{
    // Check exist option
    auto itr = _intConfigs.find(optionName);
    if (itr == _intConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists", optionName.c_str());
        return;
    }

    _intConfigs.erase(optionName);
    _intConfigs.emplace(optionName, value);
}

template<>
void GameConfig::SetOption<float>(std::string const& optionName, float const& value) const
{
    // Check exist option
    auto itr = _floatConfigs.find(optionName);
    if (itr == _floatConfigs.end())
    {
        LOG_ERROR("config", "> GameConfig: option (%s) is not exists", optionName.c_str());
        return;
    }

    _floatConfigs.erase(optionName);
    _floatConfigs.emplace(optionName, value);
}

// Loading
void GameConfig::LoadBoolConfigs(bool reload /*= false*/)
{
    if (reload)
        _boolConfigs.clear();

    AddOption<bool>("AllowTickets", true);
    AddOption<bool>("DeletedCharacterTicketTrace");
    AddOption<bool>("DurabilityLoss.InPvP");
    AddOption<bool>("AddonChannel", true);
    AddOption<bool>("CleanCharacterDB");
    AddOption<bool>("AllowPlayerCommands", true);
    AddOption<bool>("PreserveCustomChannels");
    AddOption<bool>("PlayerSave.Stats.SaveOnlyOnLogout", true);
    AddOption<bool>("CloseIdleConnections", true);

    AddOption<bool>("AllowTwoSide.Accounts", true);
    AddOption<bool>("AllowTwoSide.Interaction.Calendar");
    AddOption<bool>("AllowTwoSide.Interaction.Chat");
    AddOption<bool>("AllowTwoSide.Interaction.Channel");
    AddOption<bool>("AllowTwoSide.Interaction.Group");
    AddOption<bool>("AllowTwoSide.Interaction.Guild");
    AddOption<bool>("AllowTwoSide.Interaction.Auction");
    AddOption<bool>("AllowTwoSide.Interaction.Mail");
    AddOption<bool>("AllowTwoSide.WhoList");
    AddOption<bool>("AllowTwoSide.AddFriend");
    AddOption<bool>("AllowTwoSide.trade");
    AddOption<bool>("AllowTwoSide.Interaction.Emote");

    AddOption<bool>("AllFlightPaths");

    AddOption<bool>("Instance.IgnoreLevel");
    AddOption<bool>("Instance.IgnoreRaid");
    AddOption<bool>("Instance.GMSummonPlayer");
    AddOption<bool>("Instance.SharedNormalHeroicId");

    AddOption<bool>("GM.LogTrade");
    AddOption<bool>("GM.AllowInvite");
    AddOption<bool>("GM.AllowFriend");
    AddOption<bool>("GM.LowerSecurity");

    AddOption<bool>("EnableLoginAfterDC", true);
    AddOption<bool>("DontCacheRandomMovementPaths", true);

    AddOption<bool>("SkillChance.Prospecting");
    AddOption<bool>("SkillChance.Milling");

    AddOption<bool>("SaveRespawnTimeImmediately", true);
    AddOption<bool>("ActivateWeather", true);
    AddOption<bool>("AlwaysMaxSkillForLevel");
    AddOption<bool>("Chat.MuteFirstLogin");
    AddOption<bool>("Quests.EnableQuestTracker");
    AddOption<bool>("Quests.IgnoreRaid");
    AddOption<bool>("Quests.IgnoreAutoAccept");
    AddOption<bool>("Quests.IgnoreAutoComplete");
    AddOption<bool>("DetectPosCollision", true);
    AddOption<bool>("Channel.RestrictedLfg", true);
    AddOption<bool>("Channel.SilentlyGMJoin");
    AddOption<bool>("TalentsInspecting", true);
    AddOption<bool>("ChatFakeMessagePreventing");

    AddOption<bool>("Death.CorpseReclaimDelay.PvP", true);
    AddOption<bool>("Death.CorpseReclaimDelay.PvE", true);
    AddOption<bool>("Death.Bones.World", true);
    AddOption<bool>("Death.Bones.BattlegroundOrArena", true);
    AddOption<bool>("Die.Command.Mode", true);

    // always use declined names in the russian client
    AddOption<bool>("DeclinedNames");

    AddOption<bool>("Battleground.CastDeserter", true);
    AddOption<bool>("Battleground.QueueAnnouncer.Enable");
    AddOption<bool>("Battleground.QueueAnnouncer.PlayerOnly");
    AddOption<bool>("Battleground.StoreStatistics.Enable");
    AddOption<bool>("Battleground.TrackDeserters.Enable");
    AddOption<bool>("Battleground.GiveXPForKills");
    AddOption<bool>("Battleground.DisableQuestShareInBG");
    AddOption<bool>("Battleground.DisableReadyCheckInBG");

    AddOption<bool>("Arena.AutoDistributePoints");
    AddOption<bool>("Arena.ArenaSeason.InProgress", true);
    AddOption<bool>("Arena.QueueAnnouncer.Enable");

    // AddOption<bool>("OffhandCheckAtSpellUnlearn", true); not used

    AddOption<bool>("ItemDelete.Method");
    AddOption<bool>("ItemDelete.Vendor");

    AddOption<bool>("vmap.enableIndoorCheck", true);
    AddOption<bool>("vmap.enableLOS", true);
    AddOption<bool>("vmap.enableHeight", true);
    AddOption<bool>("vmap.petLOS", true);

    AddOption<bool>("PlayerStart.AllSpells");
    AddOption<bool>("PlayerStart.MapsExplored");
    AddOption<bool>("PlayerStart.AllReputation");

    AddOption<bool>("AlwaysMaxWeaponSkill");
    AddOption<bool>("PvPToken.Enable");
    AddOption<bool>("NoResetTalentsCost");

    // AddOption<bool>("ShowKickInWorld"); //not used
    AddOption<bool>("ShowBanInWorld");
    AddOption<bool>("ShowMuteInWorld");
    AddOption<bool>("Mute.AddAfterLogin.Enable");

    AddOption<bool>("Warden.Enabled");
    AddOption<bool>("AutoBroadcast.On");

    // PlayerDump
    AddOption<bool>("PlayerDump.DisallowPaths", true);
    AddOption<bool>("PlayerDump.DisallowOverwrite", true);

    // Misc
    AddOption<bool>("MoveMaps.Enable", true);
    AddOption<bool>("Wintergrasp.Enable");
    AddOption<bool>("Minigob.Manabonk.Enable", true);

    AddOption<bool>("IsContinentTransport.Enabled", true);
    AddOption<bool>("IsPreloadedContinentTransport.Enabled");

    AddOption<bool>("Allow.IP.Based.Action.Logging");

    // Whether to use LoS from game objects
    AddOption<bool>("CheckGameObjectLoS", true);

    AddOption<bool>("Calculate.Creature.Zone.Area.Data");
    AddOption<bool>("Calculate.Gameoject.Zone.Area.Data");

    // Player can join LFG anywhere
    AddOption<bool>("LFG.Location.All");

    // Preload all grids of all non-instanced maps
    AddOption<bool>("PreloadAllNonInstancedMapGrids");

    AddOption<bool>("SetAllCreaturesWithWaypointMovementActive");

    // Debug
    AddOption<bool>("Debug.Battleground");
    AddOption<bool>("Debug.Arena");

    LOG_INFO("config", "> Loaded %u bool configs", static_cast<uint32>(_boolConfigs.size()));
}

void GameConfig::LoadStringConfigs(bool reload /*= false*/)
{
    if (reload)
        _stringConfigs.clear();

    ///- Read the "Data" directory from the config file
    AddOption<std::string>("DataDir", "./");

    AddOption<std::string>("PlayerStart.String", "");
    AddOption<std::string>("Motd", "Welcome to an WarheadCore server");

    LOG_INFO("config", "> Loaded %u string configs", static_cast<uint32>(_stringConfigs.size()));
}

void GameConfig::LoadIntConfigs(bool reload /*= false*/)
{
    std::unordered_map<std::string, int32> _notChangeConfigs;

    if (reload)
    {
        _notChangeConfigs =
        {
            { "WorldServerPort", GetOption<int32>("WorldServerPort") },
            { "GameType", GetOption<int32>("GameType") },
            { "RealmZone", GetOption<int32>("RealmZone") },
            { "MaxPlayerLevel", GetOption<int32>("MaxPlayerLevel") },
            { "Expansion", GetOption<int32>("Expansion") }
        };
    }

    if (reload)
        _intConfigs.clear();

    AddOption<int32>("Server.LoginInfo");
    AddOption<int32>("RealmID", 1);
    AddOption<int32>("Compression", 1);
    AddOption<int32>("DBC.Locale", 255);
    AddOption<int32>("PlayerLimit", 100);
    AddOption<int32>("PersistentCharacterCleanFlags");

    AddOption<int32>("ChatLevelReq.Channel", 1);
    AddOption<int32>("ChatLevelReq.Whisper", 1);
    AddOption<int32>("ChatLevelReq.Say", 1);

    AddOption<int32>("PartyLevelReq", 1);
    AddOption<int32>("LevelReq.Trade", 1);
    AddOption<int32>("LevelReq.Ticket", 1);
    AddOption<int32>("LevelReq.Auction", 1);
    AddOption<int32>("LevelReq.Mail", 1);

    AddOption<int32>("PreserveCustomChannelDuration", 14);
    AddOption<int32>("DisconnectToleranceInterval");
    AddOption<int32>("PlayerSave.Stats.MinLevel");
    AddOption<int32>("MapUpdateInterval");
    AddOption<int32>("ChangeWeatherInterval", 10 * MINUTE * IN_MILLISECONDS);

    AddOption<int32>("WorldServerPort", 8085);
    AddOption<int32>("GameType", 0);
    AddOption<int32>("RealmZone", REALM_ZONE_DEVELOPMENT);

    AddOption<int32>("SocketTimeOutTime", 900000);
    AddOption<int32>("SocketTimeOutTimeActive", 60000);

    AddOption<int32>("SessionAddDelay", 10000);

    AddOption<int32>("StrictPlayerNames");
    AddOption<int32>("StrictCharterNames");
    AddOption<int32>("StrictChannelNames");
    AddOption<int32>("StrictPetNames");

    AddOption<int32>("MinPlayerName", 2);
    AddOption<int32>("MinCharterName", 2);
    AddOption<int32>("MinPetName", 2);

    AddOption<int32>("CharacterCreating.Disabled");
    AddOption<int32>("CharacterCreating.Disabled.RaceMask");
    AddOption<int32>("CharacterCreating.Disabled.ClassMask");
    AddOption<int32>("CharactersPerRealm", 10);
    AddOption<int32>("CharactersPerAccount", 50);

    AddOption<int32>("HeroicCharactersPerRealm", 1);
    AddOption<int32>("CharacterCreating.MinLevelForHeroicCharacter", 55);
    AddOption<int32>("SkipCinematics");
    AddOption<int32>("MaxPlayerLevel", DEFAULT_MAX_LEVEL);
    AddOption<int32>("MinDualSpecLevel", 40);

    AddOption<int32>("StartPlayerLevel", 1);
    AddOption<int32>("StartHeroicPlayerLevel", 55);
    AddOption<int32>("StartPlayerMoney");
    AddOption<int32>("MaxHonorPoints", 75000);
    AddOption<int32>("StartHonorPoints");
    AddOption<int32>("MaxArenaPoints", 10000);
    AddOption<int32>("StartArenaPoints");
    AddOption<int32>("RecruitAFriend.MaxLevel", 60);
    AddOption<int32>("RecruitAFriend.MaxDifference", 4);
    AddOption<int32>("InstantFlightPaths");
    AddOption<int32>("Instance.ResetTimeHour", 4);
    AddOption<int32>("Instance.ResetTimeRelativeTimestamp", 1135814400);
    AddOption<int32>("Instance.UnloadDelay", 30 * MINUTE * IN_MILLISECONDS);
    AddOption<int32>("MaxPrimaryTradeSkill", 2);
    AddOption<int32>("MinPetitionSigns", 9);
    AddOption<int32>("GM.LoginState", 2);
    AddOption<int32>("GM.Visible", 2);
    AddOption<int32>("GM.Chat", 2);
    AddOption<int32>("GM.WhisperingTo", 2);
    AddOption<int32>("GM.InGMList.Level", SEC_ADMINISTRATOR);
    AddOption<int32>("GM.InWhoList.Level", SEC_ADMINISTRATOR);
    AddOption<int32>("GM.StartLevel", 1);
    AddOption<int32>("Visibility.GroupMode", 1);
    AddOption<int32>("MailDeliveryDelay", HOUR);
    AddOption<int32>("UpdateUptimeInterval", 10);

    AddOption<int32>("LogDB.Opt.ClearInterval", 10);
    AddOption<int32>("LogDB.Opt.ClearTime", 1209600);
    AddOption<int32>("TeleportTimeoutNear", 25);
    AddOption<int32>("TeleportTimeoutFar", 45);
    AddOption<int32>("MaxAllowedMMRDrop", 500);

    AddOption<int32>("SkillChance.Orange", 100);
    AddOption<int32>("SkillChance.Yellow", 75);
    AddOption<int32>("SkillChance.Green", 25);
    AddOption<int32>("SkillChance.Grey");
    AddOption<int32>("SkillChance.MiningSteps", 75);
    AddOption<int32>("SkillChance.SkinningSteps", 75);
    AddOption<int32>("SkillGain.Crafting", 1);
    AddOption<int32>("SkillGain.Defense", 1);
    AddOption<int32>("SkillGain.Gathering", 1);
    AddOption<int32>("SkillGain.Weapon", 1);

    AddOption<int32>("MaxOverspeedPings", 2);
    AddOption<int32>("DisableWaterBreath", SEC_CONSOLE);
    AddOption<int32>("Expansion", 2);

    AddOption<int32>("ChatFlood.MessageCount", 10);
    AddOption<int32>("ChatFlood.MessageDelay", 1);
    AddOption<int32>("ChatFlood.MuteTime", 10);
    AddOption<int32>("Chat.MuteTimeFirstLogin", 120);

    AddOption<int32>("Event.Announce");
    AddOption<int32>("CreatureFamilyAssistanceDelay", 1500);
    AddOption<int32>("CreatureFamilyFleeDelay", 7000);

    AddOption<int32>("WorldBossLevelDiff", 3);
    AddOption<int32>("Quests.LowLevelHideDiff", 4);
    AddOption<int32>("Quests.HighLevelHideDiff", 7);
    AddOption<int32>("Battleground.Random.ResetHour", 6);
    AddOption<int32>("Guild.ResetHour", 6);
    AddOption<int32>("ChatStrictLinkChecking.Severity");
    AddOption<int32>("ChatStrictLinkChecking.Kick");

    AddOption<int32>("Corpse.Decay.NORMAL", 60);
    AddOption<int32>("Corpse.Decay.RARE", 300);
    AddOption<int32>("Corpse.Decay.ELITE", 300);
    AddOption<int32>("Corpse.Decay.RAREELITE", 300);
    AddOption<int32>("Corpse.Decay.WORLDBOSS", 3600);

    AddOption<int32>("Death.SicknessLevel", 11);

    AddOption<int32>("Battleground.PrematureFinishTimer", 5 * MINUTE * IN_MILLISECONDS);
    AddOption<int32>("Battleground.InvitationType");
    AddOption<int32>("Battleground.ReportAFK.Timer", 4);
    AddOption<int32>("Battleground.ReportAFK", 3);
    AddOption<int32>("Battleground.PremadeGroupWaitForMatch", 30 * MINUTE * IN_MILLISECONDS);

    AddOption<int32>("Arena.MaxRatingDifference", 150);
    AddOption<int32>("Arena.RatingDiscardTimer", 10 * MINUTE * IN_MILLISECONDS);
    AddOption<int32>("Arena.AutoDistributeInterval", 7);
    AddOption<int32>("Arena.ArenaSeason.ID", 1);
    AddOption<int32>("Arena.ArenaStartRating");
    AddOption<int32>("Arena.ArenaStartPersonalRating", 1000);
    AddOption<int32>("Arena.ArenaStartMatchmakerRating", 1500);

    if (reload)
        AddOption<int32>("ClientCacheVersion");

    AddOption<int32>("Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS);
    AddOption<int32>("Guild.BankEventLogRecordsCount", GUILD_BANKLOG_MAX_RECORDS);

    AddOption<int32>("InstantLogout", SEC_MODERATOR);

    ///- Load the CharDelete related config options
    AddOption<int32>("CharDelete.Method");
    AddOption<int32>("CharDelete.MinLevel");
    AddOption<int32>("CharDelete.KeepDays", 30);

    ///- Load the ItemDelete related config options
    AddOption<int32>("ItemDelete.Quality", 3);
    AddOption<int32>("ItemDelete.ItemLevel", 80);

    AddOption<int32>("HonorPointsAfterDuel");

    AddOption<int32>("PvPToken.MapAllowType", 4);
    AddOption<int32>("PvPToken.ItemID", 29434);
    AddOption<int32>("PvPToken.ItemCount", 1);

    AddOption<int32>("MapUpdate.Threads", 1);
    AddOption<int32>("Command.LookupMaxResults");

    // Warden
    AddOption<int32>("Warden.NumMemChecks", 3);
    AddOption<int32>("Warden.NumOtherChecks", 7);
    AddOption<int32>("Warden.BanDuration", DAY);
    AddOption<int32>("Warden.ClientCheckHoldOff", 30);
    AddOption<int32>("Warden.ClientCheckFailAction");
    AddOption<int32>("Warden.ClientResponseDelay", 600);

    // Dungeon finder
    AddOption<int32>("DungeonFinder.OptionsMask", 3);

    // Max instances per hour
    AddOption<int32>("AccountInstancesPerHour", 5);

    // AutoBroadcast
    AddOption<int32>("AutoBroadcast.Center");
    AddOption<int32>("AutoBroadcast.Timer", 60000);

    // MySQL ping time interval
    AddOption<int32>("MaxPingTime", 30);

    // Wintergrasp
    AddOption<int32>("Wintergrasp.PlayerMax", 100);
    AddOption<int32>("Wintergrasp.PlayerMin");
    AddOption<int32>("Wintergrasp.PlayerMinLvl", 77);
    AddOption<int32>("Wintergrasp.BattleTimer", 30);
    AddOption<int32>("Wintergrasp.NoBattleTimer", 150);
    AddOption<int32>("Wintergrasp.CrashRestartTimer", 10);
    AddOption<int32>("BirthdayTime", 1222964635);

    // Prevent players AFK from being logged out
    AddOption<int32>("PreventAFKLogout");

    // ICC buff override
    AddOption<int32>("ICC.Buff.Horde", 73822);
    AddOption<int32>("ICC.Buff.Alliance", 73828);

    // Packet spoof punishment
    AddOption<int32>("PacketSpoof.Policy", 1);
    AddOption<int32>("PacketSpoof.BanMode");
    AddOption<int32>("PacketSpoof.BanDuration", 86400);

    //
    AddOption<int32>("WaypointMovementStopTimeForPlayer", 120);

    // Calendar
    AddOption<int32>("Calendar.DeleteOldEventsHour", 6);

    // Random Battleground Rewards
    AddOption<int32>("Battleground.RewardWinnerHonorFirst", 30);
    AddOption<int32>("Battleground.RewardWinnerArenaFirst", 25);
    AddOption<int32>("Battleground.RewardWinnerHonorLast", 15);
    AddOption<int32>("Battleground.RewardWinnerArenaLast");
    AddOption<int32>("Battleground.RewardLoserHonorFirst", 5);
    AddOption<int32>("Battleground.RewardLoserHonorLast", 5);

    // CharterCost
    AddOption<int32>("Guild.CharterCost", 1000);
    AddOption<int32>("ArenaTeam.CharterCost.2v2", 800000);
    AddOption<int32>("ArenaTeam.CharterCost.3v3", 1200000);
    AddOption<int32>("ArenaTeam.CharterCost.5v5", 2000000);

    // Misc
    AddOption<int32>("MaxWhoListReturns", 49);
    AddOption<int32>("PlayerSaveInterval", Milliseconds(15min).count());

    // Check options can't be changed at worldserver.conf reload
    if (reload)
    {
        for (auto const& itr : _notChangeConfigs)
        {
            uint32 val = sConfigMgr->GetIntDefault(itr.first, itr.second);
            uint32 tempIntOption = itr.second;

            if (val != tempIntOption)
                LOG_ERROR("config", "%s option can't be changed at worldserver.conf reload, using current value (%u)", itr.first.c_str(), tempIntOption);

            SetOption<int32>(itr.first, tempIntOption);
        }
    }

    LOG_INFO("config", "> Loaded %u int configs", static_cast<uint32>(_intConfigs.size()));
}

void GameConfig::LoadFloatConfigs(bool reload /*= false*/)
{
    if (reload)
        _floatConfigs.clear();

    AddOption<float>("Rate.Health");
    AddOption<float>("Rate.Mana");

    AddOption<float>("Rate.Rage.Income");
    AddOption<float>("Rate.Rage.Loss");

    AddOption<float>("Rate.RunicPower.Income");
    AddOption<float>("Rate.RunicPower.Loss");

    AddOption<float>("Rate.Focus");
    AddOption<float>("Rate.Energy");
    AddOption<float>("Rate.Skill.Discovery");

    // Drop rates
    AddOption<float>("Rate.Drop.Item.Poor");
    AddOption<float>("Rate.Drop.Item.Normal");
    AddOption<float>("Rate.Drop.Item.Uncommon");
    AddOption<float>("Rate.Drop.Item.Rare");
    AddOption<float>("Rate.Drop.Item.Epic");
    AddOption<float>("Rate.Drop.Item.Legendary");
    AddOption<float>("Rate.Drop.Item.Artifact");
    AddOption<float>("Rate.Drop.Item.Referenced");
    AddOption<float>("Rate.Drop.Item.ReferencedAmount");
    AddOption<float>("Rate.Drop.Money");

    // XP rates
    AddOption<float>("Rate.XP.Kill");
    AddOption<float>("Rate.XP.BattlegroundKill");
    AddOption<float>("Rate.XP.Quest");
    AddOption<float>("Rate.XP.Explore");

    AddOption<float>("Rate.RepairCost");

    // Reputation rates
    AddOption<float>("Rate.Reputation.Gain");
    AddOption<float>("Rate.Reputation.LowLevel.Kill");
    AddOption<float>("Rate.Reputation.LowLevel.Quest");
    AddOption<float>("Rate.Reputation.RecruitAFriendBonus");

    // Creature damage rates
    AddOption<float>("Rate.Creature.Normal.Damage");
    AddOption<float>("Rate.Creature.Elite.Elite.Damage");
    AddOption<float>("Rate.Creature.Elite.RAREELITE.Damage");
    AddOption<float>("Rate.Creature.Elite.WORLDBOSS.Damage");
    AddOption<float>("Rate.Creature.Elite.RARE.Damage");

    // Creature HP rates
    AddOption<float>("Rate.Creature.Normal.HP");
    AddOption<float>("Rate.Creature.Elite.Elite.HP");
    AddOption<float>("Rate.Creature.Elite.RAREELITE.HP");
    AddOption<float>("Rate.Creature.Elite.WORLDBOSS.HP");
    AddOption<float>("Rate.Creature.Elite.RARE.HP");

    // Creature SpellDamage rates
    AddOption<float>("Rate.Creature.Normal.SpellDamage");
    AddOption<float>("Rate.Creature.Elite.Elite.SpellDamage");
    AddOption<float>("Rate.Creature.Elite.RAREELITE.SpellDamage");
    AddOption<float>("Rate.Creature.Elite.WORLDBOSS.SpellDamage");
    AddOption<float>("Rate.Creature.Elite.RARE.SpellDamage");

    AddOption<float>("Rate.Creature.Aggro");

    // Rest rates
    AddOption<float>("Rate.Rest.InGame");
    AddOption<float>("Rate.Rest.Offline.InTavernOrCity");
    AddOption<float>("Rate.Rest.Offline.InWilderness");

    AddOption<float>("Rate.Damage.Fall");

    // Auction rates
    AddOption<float>("Rate.Auction.Time");
    AddOption<float>("Rate.Auction.Deposit");
    AddOption<float>("Rate.Auction.Cut");

    AddOption<float>("Rate.Honor");
    AddOption<float>("Rate.ArenaPoints");
    AddOption<float>("Rate.InstanceResetTime");
    AddOption<float>("Rate.Talent");
    AddOption<float>("Rate.MoveSpeed");
    AddOption<float>("Rate.Corpse.Decay.Looted", 0.5f);
    AddOption<float>("TargetPosRecalculateRange", 1.5f);

    // DurabilityLoss rates
    AddOption<float>("DurabilityLoss.OnDeath", 10.0f);
    AddOption<float>("DurabilityLossChance.Damage", 0.5f);
    AddOption<float>("DurabilityLossChance.Absorb", 0.5f);
    AddOption<float>("DurabilityLossChance.Parry", 0.5f);
    AddOption<float>("DurabilityLossChance.Block", 0.5f);

    AddOption<float>("MaxGroupXPDistance", 74.0f);
    AddOption<float>("MaxRecruitAFriendBonusDistance", 100.0f);
    AddOption<float>("MonsterSight", 50.0f);
    AddOption<float>("GM.TicketSystem.ChanceOfGMSurvey", 50.0f);

    AddOption<float>("CreatureFamilyFleeAssistanceRadius", 30.0f);
    AddOption<float>("CreatureFamilyAssistanceRadius", 10.0f);

    AddOption<float>("ListenRange.Say", 25.0f);
    AddOption<float>("ListenRange.TextEmote", 25.0f);
    AddOption<float>("ListenRange.Yell", 300.0f);

    AddOption<float>("Arena.ArenaWinRatingModifier1", 48.0f);
    AddOption<float>("Arena.ArenaWinRatingModifier2", 24.0f);
    AddOption<float>("Arena.ArenaLoseRatingModifier", 24.0f);
    AddOption<float>("Arena.ArenaMatchmakerRatingModifier", 24.0f);

    // Visibility
    AddOption<float>("Visibility.Distance.Continents", DEFAULT_VISIBILITY_DISTANCE);
    AddOption<float>("Visibility.Distance.Instances", DEFAULT_VISIBILITY_INSTANCE);
    AddOption<float>("Visibility.Distance.BGArenas", DEFAULT_VISIBILITY_BGARENAS);

    // Rate.SellValue.Item
    AddOption<float>("Rate.SellValue.Item.Poor");
    AddOption<float>("Rate.SellValue.Item.Normal");
    AddOption<float>("Rate.SellValue.Item.Uncommon");
    AddOption<float>("Rate.SellValue.Item.Rare");
    AddOption<float>("Rate.SellValue.Item.Epic");
    AddOption<float>("Rate.SellValue.Item.Legendary");
    AddOption<float>("Rate.SellValue.Item.Artifact");
    AddOption<float>("Rate.SellValue.Item.Heirloom");

    // Rate.BuyValue.Item
    AddOption<float>("Rate.BuyValue.Item.Poor");
    AddOption<float>("Rate.BuyValue.Item.Normal");
    AddOption<float>("Rate.BuyValue.Item.Uncommon");
    AddOption<float>("Rate.BuyValue.Item.Rare");
    AddOption<float>("Rate.BuyValue.Item.Epic");
    AddOption<float>("Rate.BuyValue.Item.Legendary");
    AddOption<float>("Rate.BuyValue.Item.Artifact");
    AddOption<float>("Rate.BuyValue.Item.Heirloom");

    LOG_INFO("config", "> Loaded %u float configs", static_cast<uint32>(_floatConfigs.size()));
}

void GameConfig::CheckOptions(bool reload /*= false*/)
{
    ///- Read the player limit and the Message of the day from the config file
    if (!reload)
        sWorld->SetPlayerAmountLimit(CONF_GET_INT("PlayerLimit"));

    Motd::SetMotd(CONF_GET_STR("Motd"));

    ///- Get string for new logins (newly created characters)
    sWorld->SetNewCharString(CONF_GET_STR("PlayerStart.String"));

    ///- Read all rates from the config file
    auto CheckRate = [](std::string const& optionName)
    {
        if (CONF_GET_FLOAT(optionName) < 0.0f)
        {
            LOG_ERROR("config", "%s (%f) must be > 0. Using 1 instead.", optionName.c_str(), CONF_GET_FLOAT(optionName));
            sGameConfig->SetOption<float>(optionName, 1.0f);
        }
    };

    CheckRate("Rate.Health");
    CheckRate("Rate.Mana");
    CheckRate("Rate.Rage.Loss");
    CheckRate("Rate.RunicPower.Loss");
    CheckRate("Rate.RepairCost");
    CheckRate("Rate.Talent");
    CheckRate("Rate.MoveSpeed");

    int32 tempIntOption = 0;
    float tempFloatOption = 1.0f;

    for (uint8 i = 0; i < MAX_MOVE_TYPE; ++i)
        playerBaseMoveSpeed[i] = baseMoveSpeed[i] * CONF_GET_FLOAT("Rate.MoveSpeed");

    tempFloatOption = CONF_GET_FLOAT("TargetPosRecalculateRange");
    if (tempFloatOption < CONTACT_DISTANCE)
    {
        LOG_ERROR("config", "TargetPosRecalculateRange (%f) must be >= %f. Using %f instead.",
            tempFloatOption, CONTACT_DISTANCE, CONTACT_DISTANCE);

        sGameConfig->SetOption<float>("TargetPosRecalculateRange", CONTACT_DISTANCE);
    }
    else if (tempFloatOption > NOMINAL_MELEE_RANGE)
    {
        LOG_ERROR("config", "TargetPosRecalculateRange (%f) must be <= %f. Using %f instead",
            tempFloatOption, NOMINAL_MELEE_RANGE, NOMINAL_MELEE_RANGE);

        sGameConfig->SetOption<float>("TargetPosRecalculateRange", NOMINAL_MELEE_RANGE);
    }

    tempFloatOption = CONF_GET_FLOAT("DurabilityLoss.OnDeath");
    if (tempFloatOption < 0.0f)
    {
        LOG_ERROR("config", "DurabilityLoss.OnDeath (%f) must be >= 0. Using 0.0 instead", tempFloatOption);
        sGameConfig->SetOption<float>("DurabilityLoss.OnDeath", 0.0f);
    }
    else if (tempFloatOption > 100.0f)
    {
        LOG_ERROR("config", "DurabilityLoss.OnDeath (%f) must be <= 100. Using 100.0 instead", tempFloatOption);
        sGameConfig->SetOption<float>("DurabilityLoss.OnDeath", 0.0f);
    }

    // ???
    sGameConfig->SetOption<float>("DurabilityLoss.OnDeath", tempFloatOption / 100.0f);

    auto CheckDurabilityLossChance = [&tempFloatOption](std::string const& optionName)
    {
        tempFloatOption = CONF_GET_FLOAT(optionName);
        if (tempFloatOption < 0.0f)
        {
            LOG_ERROR("config", "%s (%f) must be >= 0. Using 0.0 instead", optionName.c_str(), tempFloatOption);
            sGameConfig->SetOption<float>(optionName, 1.0f);
        }
    };

    CheckDurabilityLossChance("DurabilityLossChance.Damage");
    CheckDurabilityLossChance("DurabilityLossChance.Absorb");
    CheckDurabilityLossChance("DurabilityLossChance.Parry");
    CheckDurabilityLossChance("DurabilityLossChance.Block");

    ///- Read other configuration items from the config file
    tempIntOption = CONF_GET_INT("Compression");
    if (tempIntOption < 1 || tempIntOption > 9)
    {
        LOG_ERROR("config", "Compression level (%i) must be in range 1..9. Using default compression level (1).", tempIntOption);
        sGameConfig->SetOption<int32>("Compression", 1);
    }

    tempIntOption = CONF_GET_INT("PlayerSave.Stats.MinLevel");
    if (tempIntOption > MAX_LEVEL)
    {
        LOG_ERROR("game.config", "PlayerSave.Stats.MinLevel (%i) must be in range 0..80. Using default, do not save character stats (0).", tempIntOption);
        sGameConfig->SetOption<int32>("PlayerSave.Stats.MinLevel", 0);
    }

    tempIntOption = CONF_GET_INT("MapUpdateInterval");
    if (tempIntOption < MIN_MAP_UPDATE_DELAY)
    {
        LOG_ERROR("config", "MapUpdateInterval (%i) must be greater %u. Use this minimal value.", tempIntOption, MIN_MAP_UPDATE_DELAY);
        sGameConfig->SetOption<int32>("MapUpdateInterval", MIN_MAP_UPDATE_DELAY);
    }

    if (reload)
        sMapMgr->SetMapUpdateInterval(tempIntOption);

    auto CheckMinName = [](std::string const& optionName, int32 const& maxNameSymols)
    {
        int32 confSymbols = CONF_GET_INT(optionName);
        if (confSymbols < 1 || confSymbols > maxNameSymols)
        {
            LOG_ERROR("config", "%s (%u) must be in range 1..%u. Set to 2.", optionName.c_str(), confSymbols, maxNameSymols);
            sGameConfig->SetOption<int32>(optionName, 2);
        }
    };

    CheckMinName("MinPlayerName", MAX_PLAYER_NAME);
    CheckMinName("MinCharterName", MAX_CHARTER_NAME);
    CheckMinName("MinPetName", MAX_PET_NAME);

    int32 charactersPerRealm = CONF_GET_INT("CharactersPerRealm");
    if (charactersPerRealm < 1 || charactersPerRealm > 10)
    {
        LOG_ERROR("config", "CharactersPerRealm (%i) must be in range 1..10. Set to 10.", charactersPerRealm);
        sGameConfig->SetOption<int32>("CharactersPerRealm", 10);
    }

    // must be after "CharactersPerRealm"
    tempIntOption = CONF_GET_INT("CharactersPerAccount");
    if (tempIntOption < charactersPerRealm)
    {
        LOG_ERROR("config", "CharactersPerAccount (%i) can't be less than CharactersPerRealm (%i).", tempIntOption, charactersPerRealm);
        sGameConfig->SetOption<int32>("CharactersPerAccount", charactersPerRealm);
    }

    tempIntOption = CONF_GET_INT("HeroicCharactersPerRealm");
    if (tempIntOption < 0 || tempIntOption > 10)
    {
        LOG_ERROR("config", "HeroicCharactersPerRealm (%i) must be in range 0..10. Set to 1.", tempIntOption);
        sGameConfig->SetOption<int32>("HeroicCharactersPerRealm", 1);
    }

    tempIntOption = CONF_GET_INT("SkipCinematics");
    if (tempIntOption < 0 || tempIntOption > 2)
    {
        LOG_ERROR("config", "SkipCinematics (%i) must be in range 0..2. Set to 0.", tempIntOption);
        sGameConfig->SetOption<int32>("SkipCinematics", 0);
    }

    int32 maxPlayerLevel = CONF_GET_INT("MaxPlayerLevel");
    if (maxPlayerLevel > MAX_LEVEL)
    {
        LOG_ERROR("config", "MaxPlayerLevel (%i) must be in range 1..%u. Set to %u.", maxPlayerLevel, MAX_LEVEL, MAX_LEVEL);
        sGameConfig->SetOption<int32>("MaxPlayerLevel", MAX_LEVEL);
    }

    int32 startPlayerLevel = CONF_GET_INT("StartPlayerLevel");
    if (startPlayerLevel < 1)
    {
        LOG_ERROR("config", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 1.", startPlayerLevel, maxPlayerLevel);
        sGameConfig->SetOption<int32>("StartPlayerLevel", 1);
    }
    else if (startPlayerLevel > maxPlayerLevel)
    {
        LOG_ERROR("config", "StartPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.", tempIntOption, maxPlayerLevel, maxPlayerLevel);
        sGameConfig->SetOption<int32>("StartPlayerLevel", maxPlayerLevel);
    }

    tempIntOption = CONF_GET_INT("StartHeroicPlayerLevel");
    if (tempIntOption < 1)
    {
        LOG_ERROR("config", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to 55.", tempIntOption, maxPlayerLevel);
        sGameConfig->SetOption<int32>("StartHeroicPlayerLevel", 55);
    }
    else if (tempIntOption > maxPlayerLevel)
    {
        LOG_ERROR("config", "StartHeroicPlayerLevel (%i) must be in range 1..MaxPlayerLevel(%u). Set to %u.", tempIntOption, maxPlayerLevel, maxPlayerLevel);
        sGameConfig->SetOption<int32>("StartHeroicPlayerLevel", maxPlayerLevel);
    }

    tempIntOption = CONF_GET_INT("StartPlayerMoney");
    if (tempIntOption < 0)
    {
        LOG_ERROR("config", "StartPlayerMoney (%i) must be in range 0..%u. Set to %u.", tempIntOption, MAX_MONEY_AMOUNT, 0);
        sGameConfig->SetOption<int32>("StartPlayerMoney", 0);
    }
    else if (tempIntOption > MAX_MONEY_AMOUNT)
    {
        LOG_ERROR("config", "StartPlayerMoney (%i) must be in range 0..%u. Set to %u.", tempIntOption, MAX_MONEY_AMOUNT, MAX_MONEY_AMOUNT);
        sGameConfig->SetOption<int32>("StartPlayerMoney", MAX_MONEY_AMOUNT);
    }

    auto CheckPoints = [](std::string const& startPointsOptionName, std::string const& maxPointsOptionName)
    {
        int32 maxPoints = CONF_GET_INT(maxPointsOptionName);
        if (maxPoints < 0)
        {
            LOG_ERROR("config", "%s (%i) can't be negative. Set to 0.", maxPointsOptionName.c_str(), maxPoints);
            sGameConfig->SetOption<int32>(maxPointsOptionName, 0);
        }

        int32 startPoints = CONF_GET_INT(startPointsOptionName);
        if (startPoints < 0)
        {
            LOG_ERROR("config", "%s (%i) must be in range 0..%s(%u). Set to %u.", startPointsOptionName.c_str(), startPoints, maxPointsOptionName.c_str(), maxPoints, 0);
            sGameConfig->SetOption<int32>(startPointsOptionName, 0);
        }
        else if (startPoints > maxPoints)
        {
            LOG_ERROR("config", "%s (%i) must be in range 0..%s(%u). Set to %u.", startPointsOptionName.c_str(), startPoints, maxPointsOptionName.c_str(), maxPoints, maxPoints);
            sGameConfig->SetOption<int32>(startPointsOptionName, maxPoints);
        }
    };

    CheckPoints("StartHonorPoints", "MaxHonorPoints");
    CheckPoints("StartArenaPoints", "MaxArenaPoints");

    tempIntOption = CONF_GET_INT("RecruitAFriend.MaxLevel");
    if (tempIntOption > maxPlayerLevel)
    {
        LOG_ERROR("config", "RecruitAFriend.MaxLevel (%i) must be in the range 0..MaxLevel(%u). Set to %u.",
            tempIntOption, maxPlayerLevel, 60);

        sGameConfig->SetOption<int32>("RecruitAFriend.MaxLevel", 60);
    }

    tempIntOption = CONF_GET_INT("MinPetitionSigns");
    if (tempIntOption > 9)
    {
        LOG_ERROR("config", "MinPetitionSigns (%i) must be in range 0..9. Set to 9.", tempIntOption);
        sGameConfig->SetOption<int32>("MinPetitionSigns", 9);
    }

    tempIntOption = CONF_GET_INT("GM.StartLevel");
    if (tempIntOption < startPlayerLevel)
    {
        LOG_ERROR("config", "GM.StartLevel (%i) must be in range StartPlayerLevel(%u)..%u. Set to %u.", tempIntOption, tempIntOption, MAX_LEVEL, tempIntOption);
        sGameConfig->SetOption<int32>("GM.StartLevel", startPlayerLevel);
    }
    else if (tempIntOption > MAX_LEVEL)
    {
        LOG_ERROR("config", "GM.StartLevel (%i) must be in range 1..%u. Set to %u.", tempIntOption, MAX_LEVEL, MAX_LEVEL);
        sGameConfig->SetOption<int32>("GM.StartLevel", MAX_LEVEL);
    }

    tempIntOption = CONF_GET_INT("UpdateUptimeInterval");
    if (tempIntOption <= 0)
    {
        LOG_ERROR("config", "UpdateUptimeInterval (%i) must be > 0, set to default 10.", tempIntOption);
        sGameConfig->SetOption<int32>("UpdateUptimeInterval", 10); // 10
    }

    // log db cleanup interval
    tempIntOption = CONF_GET_INT("LogDB.Opt.ClearInterval");
    if (tempIntOption <= 0)
    {
        LOG_ERROR("config", "LogDB.Opt.ClearInterval (%i) must be > 0, set to default 10.", tempIntOption);
        sGameConfig->SetOption<int32>("LogDB.Opt.ClearInterval", 10);
    }

    LOG_TRACE("server.loading", "Will clear `logs` table of entries older than %i seconds every %u minutes.",
        CONF_GET_INT("LogDB.Opt.ClearTime"), CONF_GET_INT("LogDB.Opt.ClearInterval"));

    tempIntOption = CONF_GET_INT("MaxOverspeedPings");
    if (tempIntOption != 0 && tempIntOption < 2)
    {
        LOG_ERROR("config", "MaxOverspeedPings (%i) must be in range 2..infinity (or 0 to disable check). Set to 2.", tempIntOption);
        sGameConfig->SetOption<int32>("MaxOverspeedPings", 2);
    }

    auto CheckResetTime = [](std::string const& optionName)
    {
        int32 hours = CONF_GET_INT(optionName);
        if (hours > 23)
        {
            LOG_ERROR("config", "%s (%i) can't be load. Set to 6.", optionName.c_str(), hours);
            sGameConfig->SetOption<int32>(optionName, 6);
        }
    };

    CheckResetTime("Battleground.Random.ResetHour");
    CheckResetTime("Guild.ResetHour");

    // always use declined names in the russian client
    sGameConfig->SetOption<bool>("DeclinedNames", CONF_GET_INT("RealmZone") == REALM_ZONE_RUSSIAN ? true : CONF_GET_BOOL("DeclinedNames"));

    if (int32 clientCacheId = CONF_GET_INT("ClientCacheVersion"))
    {
        // overwrite DB/old value
        if (clientCacheId)
        {
            sGameConfig->SetOption<int32>("ClientCacheVersion", clientCacheId);
            LOG_INFO("server.loading", "Client cache version set to: %u", clientCacheId);
        }
        else
            LOG_ERROR("config", "ClientCacheVersion can't be negative %d, ignored.", clientCacheId);
    }

    auto CheckLogRecordsCount = [](std::string const& optionName, int32 const& maxRecords)
    {
        int32 records = CONF_GET_INT(optionName);
        if (records > maxRecords)
            sGameConfig->SetOption<int32>(optionName, maxRecords);
    };

    CheckLogRecordsCount("Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS);
    CheckLogRecordsCount("Guild.BankEventLogRecordsCount", GUILD_BANKLOG_MAX_RECORDS);

    if (CONF_GET_BOOL("PlayerStart.AllSpells"))
        LOG_INFO("server.loading", "WORLD: WARNING: PlayerStart.AllSpells enabled - may not function as intended!");

    tempIntOption = CONF_GET_INT("PvPToken.ItemCount");
    if (tempIntOption < 1)
        sGameConfig->SetOption<int32>("PvPToken.ItemCount", 1);

    tempIntOption = CONF_GET_INT("PacketSpoof.BanMode");
    if (tempIntOption == 1 || tempIntOption > 2)
    {
        LOG_ERROR("config", "> AntiDOS: Invalid ban mode %u. Set 0", tempIntOption);
        sGameConfig->SetOption<int32>("PacketSpoof.BanMode", 0);
    }

    tempIntOption = CONF_GET_INT("Calendar.DeleteOldEventsHour");
    if (tempIntOption > 23)
    {
        LOG_ERROR("config", "Calendar.DeleteOldEventsHour (%i) can't be load. Set to 6.", tempIntOption);
        sGameConfig->SetOption<int32>("Calendar.DeleteOldEventsHour", 6);
    }
}
