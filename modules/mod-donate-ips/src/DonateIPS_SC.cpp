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
#include "ExternalMail.h"
#include "TaskScheduler.h"
#include "Player.h"
#include <vector>
#include <unordered_map>

enum IPSShopType
{
    IPS_SHOP_TYPE_ITEM,             // Item id
    IPS_SHOP_TYPE_CHAR_RENAME       // Character rename
};

struct IPSShop
{
    uint32 ShopID;
    IPSShopType Type;
    uint32 Value;
};

struct DonateIPSStruct
{
    uint32 ID;
    std::string CharName;
    IPSShop* ShopID;
    uint32 Value;
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
        if (!CONF_GET_BOOL("IPSShop.Enable"))
            return;

        _store.clear();

        QueryResult result = CharacterDatabase.PQuery("SELECT id, nickname, item_id, item_quantity FROM `shop_purchase` WHERE flag = 0");
        if (!result)
            return;

        LOG_TRACE("modules.ips", "> DonateIPS: SendDonate");

        do
        {
            Field* fields = result->Fetch();
            uint32 id = fields[0].GetUInt32();
            std::string playerName = fields[1].GetString();
            uint32 shopID = fields[2].GetUInt32();
            uint32 value = fields[3].GetUInt32();

            if (!normalizePlayerName(playerName))
            {
                LOG_ERROR("modules.ips", "> DonateIPS: Некорректное имя персонажа (%s)", playerName.c_str());
                continue;
            }

            uint64 playerGuid = sObjectMgr->GetPlayerGUIDByName(playerName);
            if (!playerGuid)
            {
                LOG_ERROR("modules.ips", "> DonateIPS: Неверное имя персонажа (%s)", playerName.c_str());
                continue;
            }

            if (!sObjectMgr->GetPlayerNameByGUID(playerGuid, playerName))
            {
                LOG_ERROR("modules.ips", "> DonateIPS: Ошибка получения данных о персонаже (%s)", playerName.c_str());
                continue;
            }

            auto _shopID = GetShop(shopID);
            if (!_shopID)
                continue;

            DonateIPSStruct _data;
            _data.ID = id;
            _data.CharName = playerName;
            _data.ShopID = _shopID;
            _data.Value = value;

            _store.push_back(_data);

        } while (result->NextRow());
    }

    void LoadShopStore()
    {
        if (!CONF_GET_BOOL("IPSShop.Enable"))
            return;

        _store.clear();

        QueryResult result = CharacterDatabase.PQuery("SELECT `ID`, `Type`, `Value` FROM `ips_shop` ORDER BY `ID`");
        if (!result)
            return;

        do
        {
            Field* fields = result->Fetch();

            IPSShop _data;
            _data.ShopID = fields[0].GetUInt32();
            _data.Type = static_cast<IPSShopType>(fields[1].GetUInt32());
            _data.Value = fields[2].GetUInt32();

            if (_data.Type == IPSShopType::IPS_SHOP_TYPE_ITEM)
            {
                ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(_data.Value);
                if (!itemTemplate)
                {
                    LOG_ERROR("sql.sql", "> IPS Shop: Предмета под номером %u не существует. Пропуск", _data.Value);
                    continue;
                }
            }
            else if (_data.Type != IPSShopType::IPS_SHOP_TYPE_ITEM && _data.Value)
                LOG_ERROR("sql.sql", "> IPS Shop: Шоп айди (%u) не является предметом, для него не нужно указывать количество. Установлено 0", _data.ShopID);

            _shopStore.insert(std::make_pair(_data.ShopID, _data));

        } while (result->NextRow());
    }

    void SendDonate()
    {
        LoadDonate();

        for (auto const& itr : _store)
            SendRewardForPlayer(&itr);
    }
private:
    void SendRewardForPlayer(const DonateIPSStruct* ipsData)
    {
        auto shopID = ipsData->ShopID;
        if (!shopID)
            LOG_FATAL("modules.ips", "> DonateIPS: невозможно найти данные шоп айди для номера (%u)", ipsData->ID);

        switch (shopID->Type)
        {
        case IPSShopType::IPS_SHOP_TYPE_ITEM:
            SendRewardItem(ipsData->CharName, shopID->Value, ipsData->Value);
            break;
        case IPSShopType::IPS_SHOP_TYPE_CHAR_RENAME:
            SendRewardRename(ipsData->CharName);
            break;
        default:
            LOG_FATAL("modules.ips", "> DonateIPS: Неверый тип шоп айди (%u)", static_cast<uint32>(shopID->Type));
            return;
        }

        CharacterDatabase.PExecute("UPDATE `shop_purchase` SET `flag` = 1 WHERE `id` = %u", ipsData->ID);
    }

    void SendRewardItem(std::string charName, uint32 itemID, uint32 itemCount)
    {
        sEM->AddMail(charName, thanksSubject, thanksText, itemID, itemCount, 37688);
    }

    void SendRewardRename(std::string charName)
    {
        uint64 targetGuid = sObjectMgr->GetPlayerGUIDByName(charName);
        if (!targetGuid)
            return;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setUInt16(0, uint16(AT_LOGIN_RENAME));
        stmt->setUInt32(1, GUID_LOPART(targetGuid));
        CharacterDatabase.Execute(stmt);
    }

    IPSShop* GetShop(uint32 shopID)
    {
        auto const& itr = _shopStore.find(shopID);
        if (itr != _shopStore.end())
            return &itr->second;

        LOG_FATAL("modules.ips", "> DonateIPS: невозможно найти данные для шоп айди (%u)", shopID);

        return nullptr;
    }

    std::vector<DonateIPSStruct> _store;
    std::unordered_map<uint32 /*shop id*/, IPSShop> _shopStore;

    std::string const thanksSubject = "Донат магазин";
    std::string const thanksText = "Спасибо за покупку!";
};

#define sDonateIPS DonateIPS::instance()

class DonateIPS_World : public WorldScript
{
public:
    DonateIPS_World() : WorldScript("DonateIPS_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("IPSShop.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("IPSShop.Enable"))
            return;

        sDonateIPS->LoadShopStore();

        scheduler.Schedule(15s, [this](TaskContext context)
        {
            sDonateIPS->SendDonate();

            context.Repeat();
        });
    }

    void OnUpdate(uint32 diff) override
    {
        if (!CONF_GET_BOOL("IPSShop.Enable"))
            return;

        scheduler.Update(diff);
    }
    
private:
    TaskScheduler scheduler;
};

// Group all custom scripts
void AddSC_DonateIPS()
{
    new DonateIPS_World();
}
