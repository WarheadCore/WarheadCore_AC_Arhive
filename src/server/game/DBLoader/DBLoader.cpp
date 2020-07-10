/*
 * Copyright (C)
 */

#include "DBLoader.h"
#include "AchievementMgr.h"
#include "AuctionHouseMgr.h"
#include "Log.h"
#include "DisableMgr.h"
#include "GameEventMgr.h"
#include "GameLocale.h"
#include "GameGraveyard.h"
#include "GuildMgr.h"
#include "LFGMgr.h"
#include "LootMgr.h"
#include "TransportMgr.h"
#include "ObjectMgr.h"
#include "PoolMgr.h"
#include "SpellMgr.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "WeatherMgr.h"

DBLoader* DBLoader::instance()
{
    static DBLoader instance;
    return &instance;
}

void DBLoader::LoadDefaultTables()
{
    AddTable("acore_string", "Acore strings", []() { sGameLocale->LoadAcoreStrings(); });
    AddTable("game_graveyard", "Game graveyard", []() { sGraveyard->LoadGraveyardFromDB(); });
    AddTable("spell_ranks", "Spell Rank Data", []() { sSpellMgr->LoadSpellRanks(); });
    //AddTable("_script_name", "Script names", []() { sObjectMgr->LoadScriptNames(); });
    AddTable("instance_template", "Instance template", []() { sObjectMgr->LoadInstanceTemplate(); });
    AddTable("characters", "Global Player Data", []() { sWorld->LoadGlobalPlayerDataStore(); });

    AddTable("broadcast_text", "Broadcast texts", []() { sGameLocale->LoadBroadcastTexts(); });
    AddTable("broadcast_text_locale", "Broadcast texts locale", []() { sGameLocale->LoadBroadcastTextLocales(); });
    AddTable("creature_template_locale", "Creature template locale", []() { sGameLocale->LoadCreatureLocales(); });
    //AddTable("creature_text_locale", "", []() { sGameLocale->load(); });
    AddTable("gameobject_template_locale", "GO template locale", []() { sGameLocale->LoadGameObjectLocales(); });
    AddTable("gossip_menu_option_locale", "Gossip menu option locale", []() { sGameLocale->LoadGossipMenuItemsLocales(); });
    AddTable("item_set_names_locale", "Item set names locale", []() { sGameLocale->LoadItemSetNameLocales(); });
    AddTable("item_template_locale", "Item template locale", []() { sGameLocale->LoadItemLocales(); });
    AddTable("npc_text_locale", "Npc text locale", []() { sGameLocale->LoadNpcTextLocales(); });
    AddTable("page_text_locale", "Page text locale", []() { sGameLocale->LoadPageTextLocales(); });
    AddTable("points_of_interest_locale", "POI locale", []() { sGameLocale->LoadPointOfInterestLocales(); });
    AddTable("quest_offer_reward_locale", "Quest offer reward locale", []() { sGameLocale->LoadQuestOfferRewardLocale(); });
    AddTable("quest_request_items_locale", "Quest request items locale", []() { sGameLocale->LoadQuestRequestItemsLocale(); });
    AddTable("quest_template_locale", "Quest template locale", []() { sGameLocale->LoadQuestLocales(); });

    AddTable("page_text", "Page texts", []() { sObjectMgr->LoadPageTexts(); });
    AddTable("gameobject_template", "GO templates", []() { sObjectMgr->LoadGameObjectTemplate(); sTransportMgr->LoadTransportTemplates(); });
    AddTable("gameobject_template_addon", "GO template addons", []() { sObjectMgr->LoadGameObjectTemplateAddons(); });
    AddTable("spell_required", "Spell Required Data", []() { sSpellMgr->LoadSpellRequired(); });
    AddTable("spell_group", "Spell Group types", []() { sSpellMgr->LoadSpellGroups(); });
    AddTable("spell_proc_event", "Spell Proc Event conditions", []() { sSpellMgr->LoadSpellProcEvents(); });
    AddTable("spell_proc", "Spell Proc conditions and data", []() { sSpellMgr->LoadSpellProcs(); });
    AddTable("spell_bonus_data", "Spell Bonus Data", []() { sSpellMgr->LoadSpellBonusess(); });
    AddTable("spell_threat", "Aggro Spells Definitions", []() { sSpellMgr->LoadSpellThreats(); });
    AddTable("spell_mixology", "Mixology bonuses", []() { sSpellMgr->LoadSpellMixology(); });
    AddTable("spell_group_stack_rules", "Spell Group Stack Rules", []() { sSpellMgr->LoadSpellGroupStackRules(); });
    AddTable("npc_text", "NPC Texts", []() { sObjectMgr->LoadGossipText(); });
    AddTable("spell_enchant_proc_data", "Enchant Spells Proc datas", []() { sSpellMgr->LoadSpellEnchantProcData(); });
    AddTable("item_enchantment_template", "Item Random Enchantments Table", []() { LoadRandomEnchantmentsTable(); });
    AddTable("disables", "Disables", []() { DisableMgr::LoadDisables(); });
    AddTable("item_template", "Items", []() { sObjectMgr->LoadItemTemplates(); });
    AddTable("item_set_names", "Item set names", []() { sObjectMgr->LoadItemSetNames(); });
    AddTable("creature_model_info", "Creature Model Based Info Data", []() { sObjectMgr->LoadCreatureModelInfo(); });
    AddTable("creature_template", "Creature templates", []() { sObjectMgr->LoadCreatureTemplates(); });
    AddTable("creature_equip_template", "Equipment templates", []() { sObjectMgr->LoadEquipmentTemplates(); });
    AddTable("creature_template_addon", "Creature template addons", []() { sObjectMgr->LoadCreatureTemplateAddons(); });
    AddTable("reputation_reward_rate", "Reputation Reward Rates", []() { sObjectMgr->LoadReputationRewardRate(); });
    AddTable("creature_onkill_reputation", "Creature Reputation OnKill Data", []() { sObjectMgr->LoadReputationOnKill(); });
    AddTable("reputation_spillover_template", "Reputation Spillover Data", []() { sObjectMgr->LoadReputationSpilloverTemplate(); });
    AddTable("points_of_interest", "Points Of Interest Data", []() { sObjectMgr->LoadPointsOfInterest(); });
    AddTable("creature_classlevelstats", "Creature Base Stats", []() { sObjectMgr->LoadCreatureClassLevelStats(); });
    AddTable("creature", "Creature Data", []() { sObjectMgr->LoadCreatures(); });
    AddTable("creature_summon_groups", "Temporary Summon Data", []() { sObjectMgr->LoadTempSummons(); });
    AddTable("creature_addon", "Creature Addon Data", []() { sObjectMgr->LoadCreatureAddons(); });
    AddTable("gameobject", "Gameobject Data", []() { sObjectMgr->LoadGameobjects(); });
    AddTable("gameobject_addon", "GameObject Addon Data", []() { sObjectMgr->LoadGameObjectAddons(); });
    AddTable("gameobject_questitem", "GameObject Quest Items", []() { sObjectMgr->LoadGameObjectQuestItems(); });
    AddTable("creature_questitem", "Creature Quest Items", []() { sObjectMgr->LoadCreatureQuestItems(); });
    AddTable("linked_respawn", "Creature Linked Respawn", []() { sObjectMgr->LoadLinkedRespawn(); });
    AddTable("game_weather", "Loading Weather Data", []() { WeatherMgr::LoadWeatherData(); });
    AddTable("quest_template", "Loading Weather Data", []() { sObjectMgr->LoadQuests(); });
    AddTable("quest_poi", "Quest POI", []() { sObjectMgr->LoadQuestPOI(); });
    AddTable("gameobject_queststarter", "Quests Starters GO", []() { sObjectMgr->LoadGameobjectQuestStarters(); });
    AddTable("gameobject_questender", "Quests Enders GO", []() { sObjectMgr->LoadGameobjectQuestEnders(); });
    AddTable("creature_queststarter", "Quests Starters creature", []() { sObjectMgr->LoadCreatureQuestStarters(); });
    AddTable("creature_questender", "Quests Enders creature", []() { sObjectMgr->LoadCreatureQuestEnders(); });
    AddTable("pool_template", "Objects Pooling Data", []() { sPoolMgr->LoadFromDB(); });
    AddTable("holiday_dates", "Holyday dates", []() { sGameEventMgr->LoadHolidayDates(); });
    AddTable("game_event", "Game events", []() { sGameEventMgr->LoadFromDB(); });
    AddTable("npc_spellclick_spells", "UNIT_NPC_FLAG_SPELLCLICK data", []() { sObjectMgr->LoadNPCSpellClickSpells(); });
    AddTable("vehicle_template_accessory", "Vehicle Template Accessories", []() { sObjectMgr->LoadVehicleTemplateAccessories(); });
    AddTable("vehicle_accessory", "Vehicle Accessories", []() { sObjectMgr->LoadVehicleAccessories(); });
    AddTable("spell_area", "SpellArea Data", []() { sSpellMgr->LoadSpellAreas(); });
    AddTable("areatrigger", "Area Trigger definitions", []() { sObjectMgr->LoadAreaTriggers(); });
    AddTable("areatrigger_teleport", "Area Trigger Teleport definitions", []() { sObjectMgr->LoadAreaTriggerTeleports(); });
    AddTable("access_requirement", "Access Requirements", []() { sObjectMgr->LoadAccessRequirements(); });
    AddTable("areatrigger_involvedrelation", "Quest Area Triggers", []() { sObjectMgr->LoadQuestAreaTriggers(); });
    AddTable("areatrigger_tavern", "Tavern Area Triggers", []() { sObjectMgr->LoadTavernAreaTriggers(); });
    AddTable("areatrigger_scripts", "AreaTrigger script names", []() { sObjectMgr->LoadAreaTriggerScripts(); });
    AddTable("lfg_dungeon_template", "LFG entrance positions", []() { sLFGMgr->LoadLFGDungeons(); });
    AddTable("lfg_dungeon_rewards", "LFG rewards", []() { sLFGMgr->LoadRewards(); });
    AddTable("instance_encounters", "Dungeon boss data", []() { sObjectMgr->LoadInstanceEncounters(); });
    AddTable("instance_encounters", "Graveyard-zone links", []() { sGraveyard->LoadGraveyardZones(); });
    AddTable("graveyard_zone", "Graveyard-zone links", []() { sGraveyard->LoadGraveyardZones(); });
    AddTable("spell_pet_auras", "Spell pet auras", []() { sSpellMgr->LoadSpellPetAuras(); });
    AddTable("spell_target_position", "Spell target coordinates", []() { sSpellMgr->LoadSpellTargetPositions(); });
    AddTable("spell_linked_spell", "Linked spells", []() { sSpellMgr->LoadSpellLinked(); });
    AddTable("playercreateinfo", "Player Create Data", []() { sObjectMgr->LoadPlayerInfo(); });
    AddTable("exploration_basexp", "Exploration BaseXP Data", []() { sObjectMgr->LoadExplorationBaseXP(); });
    AddTable("pet_name_generation", "Pet Name Parts", []() { sObjectMgr->LoadPetNames(); });
    AddTable("pet_levelstats", "Pet level stats", []() { sObjectMgr->LoadPetLevelInfo(); });
    AddTable("corpse", "Player Corpses", []() { sObjectMgr->LoadCorpses(); });
    AddTable("mail_level_reward", "Player level dependent mail rewards", []() { sObjectMgr->LoadMailLevelRewards(); });

    AddTable("loot_template", "Player level dependent mail rewards", []() { LoadLootTables(); });
    AddTable("creature_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Creature(); });
    AddTable("disenchant_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Disenchant(); });
    AddTable("fishing_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Fishing(); });
    AddTable("gameobject_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Gameobject(); });
    AddTable("item_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Item(); });
    AddTable("mail_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Mail(); });
    AddTable("milling_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Milling(); });
    AddTable("pickpocketing_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Pickpocketing(); });
    AddTable("prospecting_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Prospecting(); });
    AddTable("reference_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Reference(); });
    AddTable("skinning_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Skinning(); });
    AddTable("spell_loot_template", "Player level dependent mail rewards", []() { LoadLootTemplates_Spell(); });

    AddTable("skill_discovery_template", "Skill Discovery Table", []() { LoadSkillDiscoveryTable(); });
    AddTable("skill_extra_item_template", "Skill Extra Item Table", []() { LoadSkillExtraItemTable(); });
    AddTable("skill_perfect_item_template", "Skill Perfection Data Table", []() { LoadSkillPerfectItemTable(); });
    AddTable("skill_fishing_base_level", "Skill Fishing base level requirements", []() { sObjectMgr->LoadFishingBaseSkillLevel(); });
    AddTable("achievement_criteria_data", "Achievement Criteria Data", []() { sAchievementMgr->LoadAchievementCriteriaData(); });
    AddTable("character_achievement", "Completed Achievements", []() { sAchievementMgr->LoadCompletedAchievements(); });
    AddTable("auctionhouse", "Auctions", []() { sAuctionMgr->LoadAuctionItems(); sAuctionMgr->LoadAuctions(); });
    AddTable("guild", "Guild", []() { sGuildMgr->LoadGuilds(); });
}

bool DBLoader::AddTable(std::string dbName, std::string const& message, std::function<void()> function)
{
    // Check exist
    auto const& itr = _store.find(dbName);
    if (itr != _store.end())
    {
        LOG_ERROR("dbloader", "> DB table (%s) is already exists!", dbName.c_str());
        return false;
    }

    DBLoaderSctuct _data;
    _data.Message = message;
    _data.Function = function;

    _store.insert(std::make_pair(dbName, _data));

    return true;
}

void DBLoader::LoadTable(std::string const& dbName)
{
    // Check exist
    auto const& itr = _store.find(dbName);
    if (itr == _store.end())
    {
        LOG_ERROR("dbloader", "> DB table (%s) is not exists!", dbName.c_str());
        return;
    }

    LOG_INFO("server.loading", "Re-Loading %s...", itr->second.Message.c_str());
    
    itr->second.Function();
}
