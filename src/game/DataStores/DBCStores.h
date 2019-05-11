/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef WARHEAD_DBCSTORES_H
#define WARHEAD_DBCSTORES_H

#include "DBCStore.h"
#include "DBCStructure.h"
#include "SharedDefines.h"
#include <list>
#include <map>
#include <unordered_map>

enum LocaleConstant : uint8;

 // temporary hack until includes are sorted out (don't want to pull in Windows.h)
#ifdef GetClassName
#undef GetClassName
#endif

typedef std::list<uint32> SimpleFactionsList;
WC_GAME_API SimpleFactionsList const* GetFactionTeamList(uint32 faction);

WC_GAME_API char* GetPetName(uint32 petfamily, uint32 dbclang);
WC_GAME_API uint32 GetTalentSpellCost(uint32 spellId);
WC_GAME_API TalentSpellPos const* GetTalentSpellPos(uint32 spellId);

WC_GAME_API char const* GetRaceName(uint8 race, uint8 locale);
WC_GAME_API char const* GetClassName(uint8 class_, uint8 locale);

WC_GAME_API WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);

WC_GAME_API uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);

enum ContentLevels : uint8
{
    CONTENT_1_60 = 0,
    CONTENT_61_70,
    CONTENT_71_80
};
WC_GAME_API ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId);

WC_GAME_API bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId);

WC_GAME_API void Zone2MapCoordinates(float &x, float &y, uint32 zone);
WC_GAME_API void Map2ZoneCoordinates(float &x, float &y, uint32 zone);

typedef std::map<uint32/*pair32(map, diff)*/, MapDifficulty> MapDifficultyMap;
WC_GAME_API MapDifficulty const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty);
WC_GAME_API MapDifficulty const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty);

WC_GAME_API uint32 const* /*[MAX_TALENT_TABS]*/ GetTalentTabPages(uint8 cls);

WC_GAME_API uint32 GetLiquidFlags(uint32 liquidType);

WC_GAME_API PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level);
WC_GAME_API PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id);

WC_GAME_API CharacterFacialHairStylesEntry const* GetCharFacialHairEntry(uint8 race, uint8 gender, uint8 facialHairID);
WC_GAME_API CharSectionsEntry const* GetCharSectionEntry(uint8 race, CharSectionType genType, uint8 gender, uint8 type, uint8 color);
WC_GAME_API CharStartOutfitEntry const* GetCharStartOutfitEntry(uint8 race, uint8 class_, uint8 gender);

WC_GAME_API LFGDungeonEntry const* GetLFGDungeon(uint32 mapId, Difficulty difficulty);

WC_GAME_API uint32 GetDefaultMapLight(uint32 mapId);

typedef std::unordered_multimap<uint32, SkillRaceClassInfoEntry const*> SkillRaceClassInfoMap;
typedef std::pair<SkillRaceClassInfoMap::iterator, SkillRaceClassInfoMap::iterator> SkillRaceClassInfoBounds;
WC_GAME_API SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_);

WC_GAME_API ResponseCodes ValidateName(std::wstring const& name, LocaleConstant locale);

WC_GAME_API EmotesTextSoundEntry const* FindTextSoundEmoteFor(uint32 emote, uint32 race, uint32 gender);

WC_GAME_API extern DBCStorage <AchievementEntry>             sAchievementStore;
WC_GAME_API extern DBCStorage <AchievementCriteriaEntry>     sAchievementCriteriaStore;
WC_GAME_API extern DBCStorage <AreaTableEntry>               sAreaTableStore;
WC_GAME_API extern DBCStorage <AreaGroupEntry>               sAreaGroupStore;
WC_GAME_API extern DBCStorage <AreaPOIEntry>                 sAreaPOIStore;
WC_GAME_API extern DBCStorage <AreaTriggerEntry>             sAreaTriggerStore;
WC_GAME_API extern DBCStorage <AuctionHouseEntry>            sAuctionHouseStore;
WC_GAME_API extern DBCStorage <BankBagSlotPricesEntry>       sBankBagSlotPricesStore;
WC_GAME_API extern DBCStorage <BannedAddOnsEntry>            sBannedAddOnsStore;
WC_GAME_API extern DBCStorage <BarberShopStyleEntry>         sBarberShopStyleStore;
WC_GAME_API extern DBCStorage <BattlemasterListEntry>        sBattlemasterListStore;
WC_GAME_API extern DBCStorage <ChatChannelsEntry>            sChatChannelsStore;
WC_GAME_API extern DBCStorage <CharacterFacialHairStylesEntry> sCharacterFacialHairStylesStore;
WC_GAME_API extern DBCStorage <CharSectionsEntry>            sCharSectionsStore;
WC_GAME_API extern DBCStorage <CharStartOutfitEntry>         sCharStartOutfitStore;
WC_GAME_API extern DBCStorage <CharTitlesEntry>              sCharTitlesStore;
WC_GAME_API extern DBCStorage <ChrClassesEntry>              sChrClassesStore;
WC_GAME_API extern DBCStorage <ChrRacesEntry>                sChrRacesStore;
WC_GAME_API extern DBCStorage <CinematicCameraEntry>         sCinematicCameraStore;
WC_GAME_API extern DBCStorage <CinematicSequencesEntry>      sCinematicSequencesStore;
WC_GAME_API extern DBCStorage <CreatureDisplayInfoEntry>     sCreatureDisplayInfoStore;
WC_GAME_API extern DBCStorage <CreatureDisplayInfoExtraEntry> sCreatureDisplayInfoExtraStore;
WC_GAME_API extern DBCStorage <CreatureFamilyEntry>          sCreatureFamilyStore;
WC_GAME_API extern DBCStorage <CreatureModelDataEntry>       sCreatureModelDataStore;
WC_GAME_API extern DBCStorage <CreatureSpellDataEntry>       sCreatureSpellDataStore;
WC_GAME_API extern DBCStorage <CreatureTypeEntry>            sCreatureTypeStore;
WC_GAME_API extern DBCStorage <CurrencyTypesEntry>           sCurrencyTypesStore;
WC_GAME_API extern DBCStorage <DestructibleModelDataEntry>   sDestructibleModelDataStore;
WC_GAME_API extern DBCStorage <DungeonEncounterEntry>        sDungeonEncounterStore;
WC_GAME_API extern DBCStorage <DurabilityCostsEntry>         sDurabilityCostsStore;
WC_GAME_API extern DBCStorage <DurabilityQualityEntry>       sDurabilityQualityStore;
WC_GAME_API extern DBCStorage <EmotesEntry>                  sEmotesStore;
WC_GAME_API extern DBCStorage <EmotesTextEntry>              sEmotesTextStore;
WC_GAME_API extern DBCStorage <EmotesTextSoundEntry>         sEmotesTextSoundStore;
WC_GAME_API extern DBCStorage <FactionEntry>                 sFactionStore;
WC_GAME_API extern DBCStorage <FactionTemplateEntry>         sFactionTemplateStore;
WC_GAME_API extern DBCStorage <GameObjectDisplayInfoEntry>   sGameObjectDisplayInfoStore;
WC_GAME_API extern DBCStorage <GemPropertiesEntry>           sGemPropertiesStore;
WC_GAME_API extern DBCStorage <GlyphPropertiesEntry>         sGlyphPropertiesStore;
WC_GAME_API extern DBCStorage <GlyphSlotEntry>               sGlyphSlotStore;

WC_GAME_API extern DBCStorage <GtBarberShopCostBaseEntry>    sGtBarberShopCostBaseStore;
WC_GAME_API extern DBCStorage <GtCombatRatingsEntry>         sGtCombatRatingsStore;
WC_GAME_API extern DBCStorage <GtChanceToMeleeCritBaseEntry> sGtChanceToMeleeCritBaseStore;
WC_GAME_API extern DBCStorage <GtChanceToMeleeCritEntry>     sGtChanceToMeleeCritStore;
WC_GAME_API extern DBCStorage <GtChanceToSpellCritBaseEntry> sGtChanceToSpellCritBaseStore;
WC_GAME_API extern DBCStorage <GtChanceToSpellCritEntry>     sGtChanceToSpellCritStore;
WC_GAME_API extern DBCStorage <GtNPCManaCostScalerEntry>     sGtNPCManaCostScalerStore;
WC_GAME_API extern DBCStorage <GtOCTClassCombatRatingScalarEntry> sGtOCTClassCombatRatingScalarStore;
WC_GAME_API extern DBCStorage <GtOCTRegenHPEntry>            sGtOCTRegenHPStore;
//WC_GAME_API extern DBCStorage <GtOCTRegenMPEntry>            sGtOCTRegenMPStore; -- not used currently
WC_GAME_API extern DBCStorage <GtRegenHPPerSptEntry>         sGtRegenHPPerSptStore;
WC_GAME_API extern DBCStorage <GtRegenMPPerSptEntry>         sGtRegenMPPerSptStore;
WC_GAME_API extern DBCStorage <HolidaysEntry>                sHolidaysStore;
WC_GAME_API extern DBCStorage <ItemEntry>                    sItemStore;
WC_GAME_API extern DBCStorage <ItemBagFamilyEntry>           sItemBagFamilyStore;
//WC_GAME_API extern DBCStorage <ItemDisplayInfoEntry>      sItemDisplayInfoStore; -- not used currently
WC_GAME_API extern DBCStorage <ItemExtendedCostEntry>        sItemExtendedCostStore;
WC_GAME_API extern DBCStorage <ItemLimitCategoryEntry>       sItemLimitCategoryStore;
WC_GAME_API extern DBCStorage <ItemRandomPropertiesEntry>    sItemRandomPropertiesStore;
WC_GAME_API extern DBCStorage <ItemRandomSuffixEntry>        sItemRandomSuffixStore;
WC_GAME_API extern DBCStorage <ItemSetEntry>                 sItemSetStore;
WC_GAME_API extern DBCStorage <LFGDungeonEntry>              sLFGDungeonStore;
WC_GAME_API extern DBCStorage <LiquidTypeEntry>              sLiquidTypeStore;
WC_GAME_API extern DBCStorage <LockEntry>                    sLockStore;
WC_GAME_API extern DBCStorage <MailTemplateEntry>            sMailTemplateStore;
WC_GAME_API extern DBCStorage <MapEntry>                     sMapStore;
//WC_GAME_API extern DBCStorage <MapDifficultyEntry>           sMapDifficultyStore; -- use GetMapDifficultyData insteed
WC_GAME_API extern MapDifficultyMap                          sMapDifficultyMap;
WC_GAME_API extern DBCStorage <MovieEntry>                   sMovieStore;
WC_GAME_API extern DBCStorage <OverrideSpellDataEntry>       sOverrideSpellDataStore;
WC_GAME_API extern DBCStorage <PowerDisplayEntry>            sPowerDisplayStore;
WC_GAME_API extern DBCStorage <QuestSortEntry>               sQuestSortStore;
WC_GAME_API extern DBCStorage <QuestXPEntry>                 sQuestXPStore;
WC_GAME_API extern DBCStorage <QuestFactionRewEntry>         sQuestFactionRewardStore;
WC_GAME_API extern DBCStorage <RandomPropertiesPointsEntry>  sRandomPropertiesPointsStore;
WC_GAME_API extern DBCStorage <ScalingStatDistributionEntry> sScalingStatDistributionStore;
WC_GAME_API extern DBCStorage <ScalingStatValuesEntry>       sScalingStatValuesStore;
WC_GAME_API extern DBCStorage <SkillLineEntry>               sSkillLineStore;
WC_GAME_API extern DBCStorage <SkillLineAbilityEntry>        sSkillLineAbilityStore;
WC_GAME_API extern DBCStorage <SkillTiersEntry>              sSkillTiersStore;
WC_GAME_API extern DBCStorage <SoundEntriesEntry>            sSoundEntriesStore;
WC_GAME_API extern DBCStorage <SpellCastTimesEntry>          sSpellCastTimesStore;
WC_GAME_API extern DBCStorage <SpellCategoryEntry>           sSpellCategoryStore;
WC_GAME_API extern DBCStorage <SpellDifficultyEntry>         sSpellDifficultyStore;
WC_GAME_API extern DBCStorage <SpellDurationEntry>           sSpellDurationStore;
WC_GAME_API extern DBCStorage <SpellFocusObjectEntry>        sSpellFocusObjectStore;
WC_GAME_API extern DBCStorage <SpellItemEnchantmentEntry>    sSpellItemEnchantmentStore;
WC_GAME_API extern DBCStorage <SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore;
WC_GAME_API extern PetFamilySpellsStore                      sPetFamilySpellsStore;
WC_GAME_API extern DBCStorage <SpellRadiusEntry>             sSpellRadiusStore;
WC_GAME_API extern DBCStorage <SpellRangeEntry>              sSpellRangeStore;
WC_GAME_API extern DBCStorage <SpellRuneCostEntry>           sSpellRuneCostStore;
WC_GAME_API extern DBCStorage <SpellShapeshiftEntry>         sSpellShapeshiftStore;
WC_GAME_API extern DBCStorage <SpellEntry>                   sSpellStore;
WC_GAME_API extern DBCStorage <SpellVisualEntry>             sSpellVisualStore;
WC_GAME_API extern DBCStorage <StableSlotPricesEntry>        sStableSlotPricesStore;
WC_GAME_API extern DBCStorage <SummonPropertiesEntry>        sSummonPropertiesStore;
WC_GAME_API extern DBCStorage <TalentEntry>                  sTalentStore;
WC_GAME_API extern DBCStorage <TalentTabEntry>               sTalentTabStore;
WC_GAME_API extern DBCStorage <TaxiNodesEntry>               sTaxiNodesStore;
WC_GAME_API extern DBCStorage <TaxiPathEntry>                sTaxiPathStore;
WC_GAME_API extern TaxiMask                                  sTaxiNodesMask;
WC_GAME_API extern TaxiMask                                  sOldContinentsNodesMask;
WC_GAME_API extern TaxiMask                                  sHordeTaxiNodesMask;
WC_GAME_API extern TaxiMask                                  sAllianceTaxiNodesMask;
WC_GAME_API extern TaxiMask                                  sDeathKnightTaxiNodesMask;
WC_GAME_API extern TaxiPathSetBySource                       sTaxiPathSetBySource;
WC_GAME_API extern TaxiPathNodesByPath                       sTaxiPathNodesByPath;
WC_GAME_API extern DBCStorage <TransportAnimationEntry>      sTransportAnimationStore;
WC_GAME_API extern DBCStorage <TransportRotationEntry>       sTransportRotationStore;
WC_GAME_API extern DBCStorage <TeamContributionPointsEntry>  sTeamContributionPointsStore;
WC_GAME_API extern DBCStorage <TotemCategoryEntry>           sTotemCategoryStore;
WC_GAME_API extern DBCStorage <VehicleEntry>                 sVehicleStore;
WC_GAME_API extern DBCStorage <VehicleSeatEntry>             sVehicleSeatStore;
WC_GAME_API extern DBCStorage <WMOAreaTableEntry>            sWMOAreaTableStore;
//WC_GAME_API extern DBCStorage <WorldMapAreaEntry>           sWorldMapAreaStore; -- use Zone2MapCoordinates and Map2ZoneCoordinates
WC_GAME_API extern DBCStorage <WorldMapOverlayEntry>         sWorldMapOverlayStore;
WC_GAME_API extern DBCStorage <WorldSafeLocsEntry>           sWorldSafeLocsStore;

WC_GAME_API void LoadDBCStores(const std::string& dataPath);

#endif
