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

#include "ExternalMail.h"
#include "Log.h"
#include "GameConfig.h"
#include "DatabaseEnv.h"
#include "Mail.h"
#include "ObjectMgr.h"

bool ExMail::AddItems(uint32 itemID, uint32 itemCount)
{
    ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemID);
    if (!itemTemplate)
    {
        LOG_ERROR("module", "> External Mail: Предмета под номером %u не существует. ID (%u)", itemID, ID);
        return false;
    }

    if (itemCount < 1 || (itemTemplate->MaxCount > 0 && itemCount > static_cast<uint32>(itemTemplate->MaxCount)))
    {
        LOG_ERROR("module", "> External Mail: Некорректное количество (%u) для предмета (%u). ID (%u)", itemCount, itemID, ID);
        return false;
    }

    while (itemCount > itemTemplate->GetMaxStackSize())
    {
        if (Items.size() <= MAX_MAIL_ITEMS)
        {
            Items.emplace_back(itemID, itemTemplate->GetMaxStackSize());
            itemCount -= itemTemplate->GetMaxStackSize();
        }
        else
        {
            _overCountItems.emplace_back(Items);
            Items.clear();
        }
    }

    Items.emplace_back(itemID, itemCount);
    _overCountItems.emplace_back(Items);

    return true;
}

ExternalMail* ExternalMail::instance()
{
    static ExternalMail instance;
    return &instance;
}

void ExternalMail::GetMailsFromDB()
{
    QueryResult result = CharacterDatabase.PQuery("SELECT ID, PlayerName, Subject, Message, Money, ItemID, ItemCount, CreatureEntry FROM mail_external ORDER BY id ASC");
    if (!result)
        return;

    LOG_TRACE("module.em", "> External Mail: GetMailsFromDB");

    do
    {
        Field* fields = result->Fetch();

        uint32 ID = fields[0].GetUInt32();
        std::string PlayerName = fields[1].GetString();
        std::string Subject = fields[2].GetString();
        std::string Body = fields[3].GetString();
        uint32 Money = fields[4].GetUInt32();
        uint32 ItemID = fields[5].GetUInt32();
        uint32 ItemCount = fields[6].GetUInt32();
        uint32 CreatureEntry = fields[7].GetUInt32();

        if (!normalizePlayerName(PlayerName))
        {
            LOG_ERROR("module", "> External Mail: Неверное имя персонажа (%s)", PlayerName.c_str());
            continue;
        }

        uint64 PlayerGuid = sObjectMgr->GetPlayerGUIDByName(PlayerName);
        if (!PlayerGuid)
            continue;

        bool _error = false;

        // Проверка
        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(ItemID);
        if (!itemTemplate)
        {
            LOG_ERROR("module", "> External Mail: Предмета под номером %u не существует. Пропуск", ItemID);
            _error = true;
        }

        auto const* creature = sObjectMgr->GetCreatureTemplate(CreatureEntry);
        if (!creature)
        {
            LOG_ERROR("module", "> External Mail: НПС под номером %u не существует. Пропуск", CreatureEntry);
            _error = true;
        }

        if (_error)
        {
            CharacterDatabase.PExecute("UPDATE `mail_external` SET `SystemComment` = '%s' WHERE `ID` = %u", "Предмета либо НПС не существует!", ID);
            continue;
        }

        ExMail _data;
        _data.ID = ID;
        _data.PlayerGuid = PlayerGuid;
        _data.Subject = Subject;
        _data.Body = Body;
        _data.Money = Money;
        _data.CreatureEntry = CreatureEntry;

        if (!_data.AddItems(ItemID, ItemCount))
            continue;

        _store.insert(std::make_pair(_data.ID, _data));

    } while (result->NextRow());
}

void ExternalMail::SendMails()
{
    // Check mails
    if (_store.empty())
        return;

    LOG_TRACE("module.em", "> External Mail: SendMails");

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    for (auto const& itr : _store)
    {
        auto exmail = itr.second;

        for (auto const& items : exmail._overCountItems)
        {
            Player* receiver = ObjectAccessor::FindPlayer(exmail.PlayerGuid);
            MailDraft* mail = new MailDraft(exmail.Subject, exmail.Body);

            if (exmail.Money)
                mail->AddMoney(exmail.Money);

            for (auto const& itr : items)
            {
                if (Item* mailItem = Item::CreateItem(itr.first, itr.second))
                {
                    mailItem->SaveToDB(trans);
                    mail->AddItem(mailItem);
                }
            }

            mail->SendMailTo(trans, receiver ? receiver : MailReceiver(exmail.PlayerGuid), MailSender(MAIL_CREATURE, exmail.CreatureEntry, MAIL_STATIONERY_DEFAULT), MAIL_CHECK_MASK_RETURNED);
            delete mail;
        }

        trans->PAppend("DELETE FROM mail_external WHERE id = %u", exmail.ID);
    }

    CharacterDatabase.CommitTransaction(trans);

    LOG_DEBUG("module", "> External Mail: Отправлено (%u) писем", static_cast<uint32>(_store.size()));
    LOG_DEBUG("module", "");

    // Clear for next time
    _store.clear();
}

void ExternalMail::AddMail(std::string charName, std::string const thanksSubject, std::string const thanksText, uint32 itemID, uint32 itemCount, uint32 creatureEntry)
{
    // Add mail item
    CharacterDatabase.PExecute("INSERT INTO `mail_external` (PlayerName, Subject, ItemID, ItemCount, Message, CreatureEntry) VALUES ('%s', '%s', %u, %u, '%s', %u)",
                               charName.c_str(), thanksSubject.c_str(), itemID, itemCount, thanksText.c_str(), creatureEntry);
}
