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
#include "TaskScheduler.h"

class ExternalMail
{
public:
    static ExternalMail* instance()
    {
        static ExternalMail instance;
        return &instance;
    }

    void SendMails()
    {
        if (!CONF_GET_BOOL("External.Mail.Enable"))
            return;

        QueryResult result = CharacterDatabase.PQuery("SELECT ID, PlayerName, Subject, Message, Money, ItemID, ItemCount, CreatureEntry FROM mail_external ORDER BY id ASC");
        if (!result)
            return;

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        MailDraft* mail = nullptr;

        uint32 MessagesCount = 0;

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
                sLog->outErrorDb("> External Mail: Неверное имя персонажа (%s)", PlayerName.c_str());
                continue;
            }

            uint64 PlayerGuid = sObjectMgr->GetPlayerGUIDByName(PlayerName);
            if (!PlayerGuid)
                continue;

            Player* receiver = ObjectAccessor::FindPlayer(PlayerGuid);
            
            mail = new MailDraft(Subject, Body);

            if (Money)
                mail->AddMoney(Money);

            bool _error = false;

            // Проверка
            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(ItemID);
            if (!itemTemplate)
            {
                sLog->outErrorDb("> External Mail: Предмета под номером %u не существует. Пропуск", ItemID);
                _error = true;
            }

            auto const* creature = sObjectMgr->GetCreatureTemplate(CreatureEntry);
            if (!creature)
            {
                sLog->outErrorDb("> External Mail: НПС под номером %u не существует. Пропуск", CreatureEntry);
                _error = true;
            }

            if (_error)
            {
                CharacterDatabase.PExecute("UPDATE `mail_external` SET `SystemComment` = '%s' WHERE `ID` = %u", "Предмета либо НПС не существует!", ID);
                continue;
            }

            if (ItemID)
            {
                // extract items
                typedef std::pair<uint32, uint32> ItemPair;
                typedef std::list< ItemPair > ItemPairs;
                ItemPairs items;

                if (ItemCount < 1 || (itemTemplate->MaxCount > 0 && ItemCount > static_cast<uint32>(itemTemplate->MaxCount)))
                {
                    LOG_ERROR("modules", "> External Mail: Некорректное количество (%u) для предмета (%u)", ItemCount, ItemID);
                    continue;
                }

                while (ItemCount > itemTemplate->GetMaxStackSize())
                {
                    items.push_back(ItemPair(ItemID, itemTemplate->GetMaxStackSize()));
                    ItemCount -= itemTemplate->GetMaxStackSize();
                }

                items.push_back(ItemPair(ItemID, ItemCount));

                if (items.size() > MAX_MAIL_ITEMS)
                {
                    LOG_ERROR("modules", "> External Mail: Превышен лимит количества предметов для ID - %u", ID);
                    continue;
                }

                for (auto const& itr : items)
                {
                    if (Item* mailItem = Item::CreateItem(itr.first, itr.second))
                    {
                        mailItem->SaveToDB(trans);
                        mail->AddItem(mailItem);
                    }
                }
            }

            mail->SendMailTo(trans, receiver ? receiver : MailReceiver(PlayerGuid), MailSender(MAIL_CREATURE, CreatureEntry, MAIL_STATIONERY_DEFAULT), MAIL_CHECK_MASK_RETURNED);
            delete mail;

            trans->PAppend("DELETE FROM mail_external WHERE id = %u", ID);

            MessagesCount++;

        } while (result->NextRow());

        CharacterDatabase.CommitTransaction(trans);

        if (MessagesCount)
        {
            LOG_INFO("modules", "> External Mail: Отправлено (%u) писем", MessagesCount);
            LOG_INFO("modules", "");
        }
    }
};

#define sEM ExternalMail::instance()

class ExternalMail_World : public WorldScript
{
public:
    ExternalMail_World() : WorldScript("ExternalMail_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("External.Mail.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("External.Mail.Enable"))
            return;

        scheduler.Schedule(1min, [this](TaskContext context)
        {
            LOG_TRACE("modules.em", "> External Mail: Send mails");

            sEM->SendMails();

            context.Repeat();
        });
    }

    void OnUpdate(uint32 diff) override
    {
        if (!CONF_GET_BOOL("External.Mail.Enable"))
            return;

        scheduler.Update(diff);
    }
private:
    TaskScheduler scheduler;
};

// Group all custom scripts
void AddSC_ExternalMail()
{
    new ExternalMail_World();
}
