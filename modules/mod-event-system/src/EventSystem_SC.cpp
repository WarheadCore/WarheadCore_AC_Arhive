/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#include "EventSystem.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "GameConfig.h"
#include "GameLocale.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "World.h"

struct RewardPlayer
{
    bool IsExistPlayer1;
    bool IsExistPlayer2;
    bool IsExistPlayer3;

    std::string PlayerName1;
    std::string PlayerName2;
    std::string PlayerName3;
};

enum EventCategory
{
    EVENT_EASY,
    EVENT_NORMAL,
    EVENT_HARD
};

class EventSystem_World : public WorldScript
{
public:
    EventSystem_World() : WorldScript("EventSystem_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("ES.Enable");
    }
};

class EventSystem_Creature : public CreatureScript
{
public:
    EventSystem_Creature() : CreatureScript("EventSystem_Creature") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (!CONF_GET_BOOL("ES.Enable"))
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Система выключена!", GOSSIP_SENDER_MAIN, 0);
            SendGossipMenuFor(player, 1, creature->GetGUID());
            return true;
        }

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "> Меню ивента (#main-menu)", GOSSIP_SENDER_MAIN, 100);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "> Выдача наград", GOSSIP_SENDER_MAIN, 2);

        SendGossipMenuFor(player, 1, creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);

        switch (action)
        {
        // Main sector
        case 1:
            OnGossipHello(player, creature);
            break;
        case 2:
            ShowPrizeMenu(player, creature);
            break;
        case 11:
            PlayerEditMenu(player, creature, 1);
            break;
        case 12:
            PlayerEditMenu(player, creature, 2);
            break;
        case 13:
            PlayerEditMenu(player, creature, 3);
            break;

        // Clear sector
        case 20:
            ClearEnters();
            ShowPrizeMenu(player, creature);
            break;

        // EventCat sector
        case 30:
            ShowEventCats(player, creature);
            break;
        case 31:
            SetEventCatName(EVENT_EASY);
            ShowPrizeMenu(player, creature);
            break;
        case 32:
            SetEventCatName(EVENT_NORMAL);
            ShowPrizeMenu(player, creature);
            break;
        case 33:
            SetEventCatName(EVENT_HARD);
            ShowPrizeMenu(player, creature);
            break;

        // Hello sector
        case 100:
            OnGossipHello(player, creature);
            break;

        // Nickname clear
        case 111:
            _rp.PlayerName1 = "";
            PlayerEditMenu(player, creature, 1);
            break;
        case 112:
            _rp.PlayerName1 = "";
            PlayerEditMenu(player, creature, 2);
            break;
        case 113:
            _rp.PlayerName1 = "";
            PlayerEditMenu(player, creature, 3);
            break;
        default:
            break;
        }

        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
    {
        ClearGossipMenuFor(player);

        std::string playerName = code;

        normalizePlayerName(playerName);

        switch (action)
        {
        //// Nickname add
        case 11:
            _rp.PlayerName1 = playerName;
            CheckPlayer(playerName, 1);
            PlayerEditMenu(player, creature, 1);
            break;
        case 12:
            _rp.PlayerName2 = playerName;
            CheckPlayer(playerName, 2);
            PlayerEditMenu(player, creature, 2);
            break;
        case 13:
            _rp.PlayerName3 = playerName;
            CheckPlayer(playerName, 3);
            PlayerEditMenu(player, creature, 3);
            break;

        
        default:
            break;
        }

        return true;
    }
private:
    void ShowPrizeMenu(Player* player, Creature* creature)
    {
        ClearGossipMenuFor(player);

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, GetEventCatName(), GOSSIP_SENDER_MAIN, 30);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "1. " + GetColorPlayerName(1), GOSSIP_SENDER_MAIN, 11);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "2. " + GetColorPlayerName(2), GOSSIP_SENDER_MAIN, 12);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "3. " + GetColorPlayerName(3), GOSSIP_SENDER_MAIN, 13);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Выдать награды", GOSSIP_SENDER_MAIN, 0, "Вы уверены?", 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Очистить поля", GOSSIP_SENDER_MAIN, 20, "Вы уверены?", 0, false);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Обновить меню", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "В главное меню", GOSSIP_SENDER_MAIN, 100);

        SendGossipMenuFor(player, 1, creature->GetGUID());
    }

    void ShowEventCats(Player* player, Creature* creature)
    {
        ClearGossipMenuFor(player);

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cff00ff00Простой|r", GOSSIP_SENDER_MAIN, 31);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cff0000ffСредний|r", GOSSIP_SENDER_MAIN, 32);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cffff0000Сложный|r", GOSSIP_SENDER_MAIN, 33);

        SendGossipMenuFor(player, 1, creature->GetGUID());
    }

    // Player menu
    void CheckPlayer(std::string const& name, uint8 plNum)
    {
        bool isExist = false;

        auto PlayerGuid = sObjectMgr->GetPlayerGUIDByName(name);
        if (PlayerGuid)
            isExist = true;

        switch (plNum)
        {
        case 1:
            _rp.IsExistPlayer1 = isExist ? true : false;
            break;
        case 2:
            _rp.IsExistPlayer2 = isExist ? true : false;
            break;
        case 3:
            _rp.IsExistPlayer3 = isExist ? true : false;
            break;
        default:
            break;
        }
    }

    std::string GetColorPlayerName(uint8 plNum)
    {
        std::string plName;

        switch (plNum)
        {
        case 1: plName = (_rp.IsExistPlayer1 ? _colorGreen : _colorRed) + _rp.PlayerName1; break;
        case 2: plName = (_rp.IsExistPlayer2 ? _colorGreen : _colorRed) + _rp.PlayerName2; break;
        case 3: plName = (_rp.IsExistPlayer3 ? _colorGreen : _colorRed) + _rp.PlayerName3; break;
        default:
            break;
        }

        // if not entered player name
        if (plName == _colorRed)
            plName = "> Введите имя <";

        return plName;
    }

    void PlayerEditMenu(Player* player, Creature* creature, uint8 plNum)
    {
        std::string plName;
        bool isExistPL;

        ClearGossipMenuFor(player);

        switch (plNum)
        {
        case 1:
            plName = _rp.PlayerName1;
            _rp.IsExistPlayer1 ? (isExistPL = true) : (isExistPL = false);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, acore::StringFormat("%u. %s", plNum, GetColorPlayerName(plNum)), GOSSIP_SENDER_MAIN, 11);
            break;
        case 2:
            plName = _rp.PlayerName2;
            _rp.IsExistPlayer2 ? (isExistPL = true) : (isExistPL = false);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, acore::StringFormat("%u. %s", plNum, GetColorPlayerName(plNum)), GOSSIP_SENDER_MAIN, 12);
            break;
        case 3:
            plName = _rp.PlayerName3;
            _rp.IsExistPlayer3 ? (isExistPL = true) : (isExistPL = false);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, acore::StringFormat("%u. %s", plNum, GetColorPlayerName(plNum)), GOSSIP_SENDER_MAIN, 13);
            break;
        default:
            break;
        }
        
        /*if (isExistPL)
        {
            auto playerData = sWorld->GetGlobalPlayerData(static_cast<uint32>(sObjectMgr->GetPlayerGUIDByName(plName)));

            std::string raceStr = sGameLocale->GetRaseString(playerData->race)->GetText(LOCALE_ruRU, playerData->gender);
            std::string classStr = sGameLocale->GetClassString(playerData->playerClass)->GetText(LOCALE_ruRU, playerData->gender);

            AddGossipItemFor(player, GOSSIP_ICON_CHAT, acore::StringFormat("Расса - %s. Класс - %s", raceStr.c_str(), classStr.c_str()), GOSSIP_SENDER_MAIN, 0);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, acore::StringFormat("Уровень - %u", playerData->level), GOSSIP_SENDER_MAIN, 0);
        }*/

        switch (plNum)
        {
        case 1:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Изменить ник", GOSSIP_SENDER_MAIN, 11, "Введите ник", 0, true);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Очистить поле", GOSSIP_SENDER_MAIN, 111);
            break;
        case 2:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Изменить ник", GOSSIP_SENDER_MAIN, 12, "Введите ник", 0, true);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Очистить поле", GOSSIP_SENDER_MAIN, 112);
            break;
        case 3:
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Изменить ник", GOSSIP_SENDER_MAIN, 13, "Введите ник", 0, true);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Очистить поле", GOSSIP_SENDER_MAIN, 113);
            break;
        default:
            break;
        }

        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "<< Назад", GOSSIP_SENDER_MAIN, 2);

        SendGossipMenuFor(player, 1, creature->GetGUID());
    }

    // Clear all
    void ClearEnters()
    {
        _rp.PlayerName1 = "";
        _rp.PlayerName2 = "";
        _rp.PlayerName3 = "";

        _rp.IsExistPlayer1 = false;
        _rp.IsExistPlayer2 = false;
        _rp.IsExistPlayer3 = false;

        _eventCatStr.clear();
    }

    // EventCat
    void SetEventCatName(EventCategory ec)
    {
        // Add event type
        switch (ec)
        {
        case EVENT_EASY:
            _eventCatStr = "|cff00ff00Простой|r";            
            break;
        case EVENT_NORMAL:
            _eventCatStr = "|cff0000ffСредний|r";
            break;
        case EVENT_HARD:
            _eventCatStr = "|cffff0000Сложный|r";
            break;
        default:
            break;
        }

        _eventCatStr += " ивент";

        _eventCat = static_cast<uint8>(ec);
    }

    std::string GetEventCatName()
    {
        if (_eventCatStr.empty())
            return "Выберите тип эвента!";

        return "Тип ивента - " + _eventCatStr;
    }

    RewardPlayer _rp;

    std::string _colorGreen = "|cff00ff00";
    std::string _colorRed = "|cffff0000";

    uint8 _eventCat;
    std::string _eventCatStr;
};

// Group all custom scripts
void AddSC_EventSystem()
{
    new EventSystem_World();
    new EventSystem_Creature();
}
