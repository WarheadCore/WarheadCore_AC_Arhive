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

#include "GuildLevelSystem.h"
#include "Log.h"
#include "GameConfig.h"
#include "Chat.h"
#include "ScriptedGossip.h"
#include "GameLocale.h"
#include "GameTime.h"
#include "World.h"

void GuildLevel::AddExp(uint32 exp)
{
    if (!CONF_GET_BOOL("GLS.Enable") || exp < 1)
        return;

    uint32 level = GetLevel();
    uint32 curXP = GetExp();
    uint32 nextLvlXP = sGuildLevelSystem->GetExpForNextLevel(level);
    uint32 newXP = curXP + exp;

    while (newXP >= nextLvlXP && Level < CONF_GET_INT("GLS.MaxLevel"))
    {
        newXP -= nextLvlXP;

        if (level < sGuildLevelSystem->GetMaxLevel())
        {
            sGuildLevelSystem->SendGuildFormat(source, "|cffff0000#|r |cff6C8CD5Гильдия достигла|r %u-го |cff6C8CD5уровня.|r", level + 1);

            Level = level + 1;
            sGuildLevelSystem->UpdateGuildVisibleLevel(source->GetId());
            sGuildLevelSystem->UpdateRewardForNewLevel(source->GetId());

            CharacterDatabase.PExecute("UPDATE `gls_guild_level` SET `Level` = %u WHERE `GuildID` = %u", level + 1, source->GetId());
        }

        level = GetLevel();
        nextLvlXP = sGuildLevelSystem->GetExpForNextLevel(level);
    }

    Exp = newXP;
    CharacterDatabase.PExecute("UPDATE `gls_guild_level` SET `Exp` = %u WHERE `GuildID` = %u", newXP, source->GetId());
}

GuildLevelSystem* GuildLevelSystem::instance()
{
    static GuildLevelSystem instance;
    return &instance;
}

void GuildLevelSystem::Init()
{
    LOG_INFO("module", "Loading Guild Level System");

    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(CONF_GET_INT("GLS.Exp.ItemID"));
    if (!itemTemplate)
    {
        LOG_ERROR("module.gls", "> ItemID (%s) for GLS is not valid. Set default (37711)", CONF_GET_INT("GLS.Exp.ItemID"));
        _expItemID = 37711;
    }

    _maxLevel = CONF_GET_INT("GLS.MaxLevel");    

    LoadExpGuildForLevel();
    LoadGuildSetting();
    LoadGuildSpellReward();
    LoadGuildLevels();
    SetGuildLevels();

    LOG_INFO("module", "");
}

void GuildLevelSystem::SetGuildLevels()
{
    for (auto const& itr : sGuildMgr->GetAllGuilds())
        SetFullName(itr.second->GetId());
}

void GuildLevelSystem::LoadGuildLevels()
{
    uint32 oldMSTime = getMSTime();

    _guildstore.clear();

    //                                                    0          1
    QueryResult result = CharacterDatabase.Query("SELECT `GuildID`, `Level`, `Exp` FROM `gls_guild_level`");

    if (!result)
    {
        LOG_ERROR("sql.sql", "> Загружено 0 GLS уровней гильдий. Таблица `gls_guild_level` возможно пустая.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 guildid = fields[0].GetUInt32();
        uint32 level = fields[1].GetUInt32();
        uint32 exp = fields[2].GetUInt32();

        Guild* guild = sGuildMgr->GetGuildById(guildid);
        if (!guild)
        {
            LOG_ERROR("sql.sql", "> Не сущвествует гильдии под номером (%u)", guildid);
            continue;            
        }

        AddGLS(guild, level, exp);

    } while (result->NextRow());

    sLog->outString(">> Загружено %u GLS уровней гильдий за %u мс", static_cast<uint32>(_guildstore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GuildLevelSystem::LoadExpGuildForLevel()
{
    uint32 oldMSTime = getMSTime();

    _guildExpForLevelStore.clear();

    //                                                    0        1
    QueryResult result = CharacterDatabase.Query("SELECT `Level`, `Exp` FROM `gls_exp_for_level`");

    if (!result)
    {
        LOG_ERROR("sql.sql", "> Загружено 0 уровней для системы гильдий. Таблица `gls_exp_for_level` возможно пустая.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 current_level = fields[0].GetUInt32();
        uint32 current_xp = fields[1].GetUInt32();
        uint32 MaxLevelGuild = GetMaxLevel();

        if ((current_level + 1) > MaxLevelGuild)
        {
            sLog->outError("> Уровень гильдии (%u) больше, чем максимальный (%u)", current_level + 1, MaxLevelGuild);
            continue;
        }

        if (!current_xp)
        {
            sLog->outError("> Опыт для следующего уровня на уровне (%u) не может быть равен нулю", current_level);
            continue;
        }

        _guildExpForLevelStore.insert(std::make_pair(current_level, current_xp));

    } while (result->NextRow());

    sLog->outString(">> Загружено %u уровней для системы гильдий за %u мс", static_cast<uint32>(_guildExpForLevelStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GuildLevelSystem::LoadGuildSetting()
{
    uint32 oldMSTime = getMSTime();

    _guildSettingStore.clear();

    //                                                    0          1
    QueryResult result = CharacterDatabase.Query("SELECT `GuildID`, `Flag` FROM `gls_guild_setting`");

    if (!result)
    {
        LOG_ERROR("sql.sql", "> Загружено 0 настроек гильдий. Таблица `gls_guild_setting` возможно пустая.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        _guildSettingStore.insert(std::make_pair(fields[0].GetUInt32(), fields[1].GetUInt32()));

    } while (result->NextRow());

    sLog->outString(">> Загружено %u настроек гильдий за %u мс", static_cast<uint32>(_guildSettingStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

void GuildLevelSystem::LoadGuildSpellReward()
{
    uint32 oldMSTime = getMSTime();

    _guildSpellRewardStore.clear();

    //                                                    0             1
    QueryResult result = CharacterDatabase.Query("SELECT `GuildLevel`, `ListSpells` FROM `gls_reward_spell` ORDER BY GuildLevel");

    if (!result)
    {
        LOG_ERROR("sql.sql", "> Загружено 0 наградных спеллов гильдий. Таблица `gls_reward_spell` возможно пустая.");
        return;
    }

    uint32 spellCount = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 GuildLevel = fields[0].GetUInt32();
        std::string ListSpells = fields[1].GetString();

        Tokenizer SpellList(ListSpells, ',');
        for (uint8 i = 0; i < SpellList.size(); i++)
        {
            uint32 SpellID = (uint32)atoi(SpellList[i]);

            auto spell = sSpellStore.LookupEntry(SpellID);
            if (!spell)
            {
                LOG_ERROR("sql.sql", "Заклинания под номером (%u) не существует. Пропуск.", SpellID);
                continue;
            }

            _guildSpellRewardStore[GuildLevel].push_back(SpellID);
            spellCount++;
        }

        if (_guildSpellRewardStore[GuildLevel].empty())
        {
            LOG_ERROR("sql.sql", "Для уровня (%u) нет заклинаний. Пропуск.", GuildLevel);
            _guildSpellRewardStore[GuildLevel].clear();
            continue;
        }

    } while (result->NextRow());

    sLog->outString(">> Загружено %u наградных спеллов для %u уровней гильдий за %u мс", spellCount, static_cast<uint32>(_guildSpellRewardStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

GuildLevel* GuildLevelSystem::GetGLS(uint32 guildid, bool needCheck /*= true*/)
{
    auto const& itr = _guildstore.find(guildid);
    if (itr != _guildstore.end())
        return itr->second;

    if (needCheck)
        ASSERT(false, "> Guild id (%u) is not found in GLS", guildid);

    return nullptr;
}

void GuildLevelSystem::AddGLS(Guild* guild, uint32 level, uint32 exp)
{
    if (GetGLS(guild->GetId(), false))
        ASSERT(false, "> Guild id (%u) is exist in GLS", guild->GetId());

    _guildstore.insert(std::make_pair(guild->GetId(), new GuildLevel(level, exp, guild)));
}

void GuildLevelSystem::AddEmptyGLS(Guild* guild)
{
    if (GetGLS(guild->GetId(), false))
        ASSERT(false, "> Guild id (%u) is exist in GLS", guild->GetId());

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    trans->PAppend("INSERT INTO `gls_guild_level`(`GuildID`, `Level`) VALUES (%u, 1)", guild->GetId());
    trans->PAppend("INSERT INTO `gls_guild_setting`(`GuildID`, `Flag`) VALUES (%u, 0)", guild->GetId());

    CharacterDatabase.CommitTransaction(trans);

    AddGLS(guild, 1, 0);
}

void GuildLevelSystem::DeleteGLS(Guild* guild)
{
    uint32 guildID = guild->GetId();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    trans->PAppend("DELETE FROM `gls_guild_level` WHERE `GuildID` = %u", guildID);
    trans->PAppend("DELETE FROM `gls_guild_setting` WHERE `GuildID` = %u", guildID);

    CharacterDatabase.CommitTransaction(trans);

    auto const& itr = _guildstore.find(guildID);
    if (itr == _guildstore.end())
        return;

    _guildstore.erase(guildID);
}

uint32 GuildLevelSystem::GetExpForNextLevel(uint32 level)
{
    if (!CONF_GET_BOOL("GLS.Enable"))
        return 0;

    auto const& itr = _guildExpForLevelStore.find(level);
    if (itr != _guildExpForLevelStore.end())
        return itr->second;

    return 0;
}

void GuildLevelSystem::UpdateGuildVisibleLevel(uint32 guildID)
{
    Guild* guild = sGuildMgr->GetGuildById(guildID);
    if (!guild)
        return;

    SetFullName(guildID);

    for (auto const& itr : sWorld->GetAllSessions())
    {
        if (!itr.second)
            continue;

        Player* player = itr.second->GetPlayer();
        if (!player)
            continue;

        if (!player->IsInWorld())
            continue;

        if (player->GetGuildId() != guild->GetId())
            continue;

        guild->HandleQuery(itr.second);
    }
}

void GuildLevelSystem::UpdateRewardForNewLevel(uint32 guildID)
{
    Guild* guild = sGuildMgr->GetGuildById(guildID);
    if (!guild)
        return;

    for (auto const& itr : sWorld->GetAllSessions())
    {
        if (!itr.second)
            continue;

        Player* player = itr.second->GetPlayer();
        if (!player)
            continue;

        if (!player->IsInWorld())
            continue;

        if (player->GetGuildId() != guild->GetId())
            continue;

        RewardSpellGuildMember(guild, player);
    }
}

void GuildLevelSystem::AddGuildExp(Guild* guild, uint32 exp)
{
    auto gls = GetGLS(guild->GetId());
    if (!gls)
        return;

    gls->AddExp(exp);
}

void GuildLevelSystem::GuildInfo(Player* player)
{
    auto gls = GetGLS(player->GetGuildId());
    ChatHandler handler(player->GetSession());
    uint32 level = gls->GetLevel();
    uint32 exp = gls->GetExp();
    uint32 nextLevelExp = GetExpForNextLevel(level);    
    uint32 remainedExp = nextLevelExp - exp;

    if (!nextLevelExp)
        remainedExp = 0;

    std::string info1 = warhead::StringFormat("|cff790C06Уровень -|r |cFF1E90FF%u|r", level);
    std::string info2 = warhead::StringFormat("|cff790C06Опыт гильдии:|r |cFF1E90FF%u|r |cff790C06/|r |cFF1E90FF%u|r", exp, nextLevelExp);
    std::string info3 = warhead::StringFormat("|cff790C06Осталось до|r |cFF1E90FF%u-го|r |cff790C06уровня -|r |cFF1E90FF%u|r |cff790C06опыта|r", level + 1, remainedExp);

    AddGossipItemFor(player, 10, info1, GOSSIP_SENDER_MAIN, 999);
    AddGossipItemFor(player, 10, info2, GOSSIP_SENDER_MAIN, 999);
    AddGossipItemFor(player, 10, info3, GOSSIP_SENDER_MAIN, 999);
    AddGossipItemFor(player, 10, "Обновить меню", GOSSIP_SENDER_MAIN, 1);
    AddGossipItemFor(player, 10, "<- Назад", GOSSIP_SENDER_MAIN, 101);
}

void GuildLevelSystem::InvestExpFull(Player* player)
{
    ChatHandler handler(player->GetSession());
    Guild* guild = player->GetGuild();
    std::string PlayerName = player->GetName();
    int8 Locale_index = handler.GetSessionDbLocaleIndex();
    uint32 guildExpItemID = GetExpItemID();
    std::string const& ItemName = GetItemLocale(guildExpItemID, Locale_index);
    std::string const& ItemLink = GetItemLink(guildExpItemID, Locale_index);
    uint32 ItemCount = player->GetItemCount(guildExpItemID);

    if (!ItemCount)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 У вас нет|r %s", ItemLink.c_str());
        return;
    }

    player->DestroyItemCount(guildExpItemID, ItemCount, true);
    AddGuildExp(guild, ItemCount);
    CharacterDatabase.PExecute("INSERT INTO `gls_invested`(`GuildID`, `PlayerName`, `InvestExp`, `UnixDate`) VALUES (%u, '%s', %u, %u)", guild->GetId(), PlayerName.c_str(), ItemCount, GameTime::GetGameTime());
}

void GuildLevelSystem::InvestExpChoice(Player* player, char const* code)
{
    ChatHandler handler(player->GetSession());
    Guild* guild = player->GetGuild();
    int8 Locale_index = handler.GetSessionDbLocaleIndex();
    uint32 guildExpItemID = GetExpItemID();
    std::string const& ItemName = GetItemLocale(guildExpItemID, Locale_index);
    std::string const& ItemLink = GetItemLink(guildExpItemID, Locale_index);
    int32 InvestCount = atoi(code);
    uint32 ItemCount = player->GetItemCount(guildExpItemID);

    if (!ItemCount)
    {
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 У вас нет|r %s", ItemLink.c_str());
        return;
    }

    if (!player->HasItemCount(guildExpItemID, InvestCount))
    {
        uint32 NeedItemCount = InvestCount - ItemCount;
        handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вам не хватает|r %s - %u", ItemLink.c_str(), NeedItemCount);
        return;
    }

    player->DestroyItemCount(guildExpItemID, InvestCount, true);
    AddGuildExp(guild, ItemCount);
    CharacterDatabase.PExecute("INSERT INTO `gls_invested`(`GuildID`, `PlayerName`, `InvestExp`, `UnixDate`) VALUES (%u, '%s', %u, %u)", guild->GetId(), player->GetName().c_str(), InvestCount, GameTime::GetGameTime());
}

uint32 GuildLevelSystem::GetCountExpItem(Player* player)
{
    return player->GetItemCount(GetExpItemID());
}

bool GuildLevelSystem::IsAsseptSetting(GuildSettingFlag Flag, Guild* guild)
{
    auto const& itr = _guildSettingStore.find(guild->GetId());
    if (itr != _guildSettingStore.end())
    {
        if (itr->second & Flag)
            return true;
    }

    return false;
}

std::string GuildLevelSystem::IsAdmChat(Guild* guild)
{
    if (IsAsseptSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild))
        return "|cff008000[Включено]|r";

    return "|cffff0000[Выключено]|r";
}

std::string GuildLevelSystem::IsAssistantChat(Guild* guild)
{
    if (IsAsseptSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild))
        return "|cff008000[Включено]|r";

    return "|cffff0000[Выключено]|r";
}

void GuildLevelSystem::ChangeGuildSetting(GuildSettingFlag Flag, Guild* guild, bool Apply)
{
    uint32 guildID = guild->GetId();

    if (Apply)
    {
        _guildSettingStore[guildID] = _guildSettingStore[guildID] |= Flag;
        CharacterDatabase.PExecute("UPDATE `gls_guild_setting` SET `FlagSetting` = FlagSetting | %u WHERE `GuildID` = %u", uint32(Flag), guildID);
    }
    else
    {
        _guildSettingStore[guildID] = _guildSettingStore[guildID] &= ~Flag;
        CharacterDatabase.PExecute("UPDATE `gls_guild_setting` SET `FlagSetting` = FlagSetting & ~%u WHERE `GuildID` = %u", uint32(Flag), guildID);
    }
}

void GuildLevelSystem::ChangeSettingChat(Guild* guild, bool IsAdmChat)
{
    uint32 GuildID = guild->GetId();

    if (IsAdmChat)
    {
        if (IsAsseptSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild))
            ChangeGuildSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild, false);
        else
            ChangeGuildSetting(GUILD_SETTING_FLAG_ADM_CHAT, guild, true);
    }
    else
    {
        if (IsAsseptSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild))
            ChangeGuildSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild, false);
        else
            ChangeGuildSetting(GUILD_SETTING_FLAG_ASSISTANT_CHAT, guild, true);
    }
}

void GuildLevelSystem::RewardSpellGuildMember(Guild* guild, Player* player)
{
    ASSERT(player && guild);

    if (_guildSpellRewardStore.empty())
        return;

    auto gls = GetGLS(guild->GetId());

    uint32 guildLevel = gls->GetLevel();

    for (uint32 level = 1; level < guildLevel + 1; level++)
    {
        auto const& itr = _guildSpellRewardStore.find(level);
        if (itr != _guildSpellRewardStore.end())
        {
            for (auto const& i : itr->second)
            {
                if (!player->HasSpell(i))
                    player->learnSpell(i);
            }
        }
    }
}

void GuildLevelSystem::UnRewardSpellGuildMember(Guild* guild, Player* player)
{
    ASSERT(player && guild);

    if (_guildSpellRewardStore.empty())
        return;

    for (uint32 level = 1; level < GetMaxLevel(); level++)
    {
        auto const& itr = _guildSpellRewardStore.find(level);
        if (itr != _guildSpellRewardStore.end())
        {
            for (auto const& i : itr->second)
            {
                if (player->HasSpell(i))
                    player->removeSpell(i, SPEC_MASK_ALL, false);
            }
        }
    }
}

void GuildLevelSystem::SendGuildMessage(Guild* guild, std::string&& message)
{
    WorldPacket data;
    ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, message);

    for (auto const& itr : sWorld->GetAllSessions())
    {
        Player* player = itr.second->GetPlayer();
        if (!player)
            continue;

        if (!player->IsInWorld())
            continue;

        if (player->GetGuild() != guild)
            continue;

        itr.second->SendPacket(&data);
    }
}

std::string const GuildLevelSystem::GetItemLocale(uint32 ItemID, int8 index_loc)
{
    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(ItemID);
    ItemLocale const* itemLocale = sGameLocale->GetItemLocale(ItemID);
    std::string name;

    if (itemLocale)
        name = itemLocale->Name[index_loc];

    if (name.empty() && itemTemplate)
        name = itemTemplate->Name1;

    if (name.empty())
        name = "Unknown item";

    return name;
}

std::string const GuildLevelSystem::GetItemLink(uint32 itemid, int8 index_loc)
{
    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemid);
    std::string name = GetItemLocale(itemid, index_loc);

    std::string color = "cffffffff";

    switch (itemTemplate->Quality)
    {
    case 0:
        color = "cff9d9d9d";
        break;
    case 1:
        color = "cffffffff";
        break;
    case 2:
        color = "cff1eff00";
        break;
    case 3:
        color = "cff0070dd";
        break;
    case 4:
        color = "cffa335ee";
        break;
    case 5:
        color = "cffff8000";
        break;
    case 6:
    case 7:
        color = "cffe6cc80";
        break;
    default:
        break;
    }

    return warhead::StringFormat("|%s|Hitem:%u:0:0:0:0:0:0:0:0|h[%s]|h|r", color.c_str(), itemid, name.c_str());
}

void GuildLevelSystem::SetFullName(uint32 guildID)
{
    Guild* guild = sGuildMgr->GetGuildById(guildID);
    if (!guild)
        return;

    std::string const& fullName = warhead::StringFormat("%s (%u level)", guild->GetName().c_str(), GetGLS(guild->GetId())->GetLevel());

    guild->SetName(fullName, true);

    LOG_WARN("module", "GLS: Add full name %s", fullName.c_str());
}

void GuildLevelSystem::SetFullNameFirstLevel(Guild* guild)
{
    ASSERT(guild);

    std::string const& fullName = warhead::StringFormat("%s (1 level)", guild->GetName().c_str());

    guild->SetName(fullName, true);
}
