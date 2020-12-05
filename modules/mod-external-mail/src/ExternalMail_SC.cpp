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

#include "ScriptMgr.h"
#include "ExternalMail.h"
#include "Log.h"
#include "GameConfig.h"
#include "TaskScheduler.h"

class ExternalMail_World : public WorldScript
{
public:
    ExternalMail_World() : WorldScript("ExternalMail_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddOption<bool>("External.Mail.Enable");
    }

    void OnStartup() override
    {
        if (!CONF_GET_BOOL("External.Mail.Enable"))
            return;

        scheduler.Schedule(5s, [this](TaskContext context)
        {
            sEM->GetMailsFromDB();
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
