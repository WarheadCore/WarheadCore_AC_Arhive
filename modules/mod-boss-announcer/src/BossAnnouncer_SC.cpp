/*
 * Copyright (C) since 2020 Andrei Guluaev (Winfidonarleyan/Kargatum) https://github.com/Winfidonarleyan
 * Licence MIT https://opensource.org/MIT
 */

#include "Log.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "StringFormat.h"
#include "GameConfig.h"
#include "GameLocale.h"

class Boss_Announcer_Player : public PlayerScript
{
public:
    Boss_Announcer_Player() : PlayerScript("Boss_Announcer_Player") {}
	
	void OnCreatureKill(Player* player, Creature* creature) override
	{
		if (!CONF_GET_BOOL("BossAnnouncer.Enable"))
            return;

        if (!creature->isWorldBoss())
            return;

        auto creatureTemplate = sObjectMgr->GetCreatureTemplate(creature->GetEntry());
        auto cretureLocale = sGameLocale->GetCreatureLocale(creature->GetEntry());
        char const* name = nullptr;
        if (cretureLocale)
            name = cretureLocale->Name[player->GetSession()->GetSessionDbcLocale()].c_str();
        if ((!name || !*name) && creatureTemplate)
            name = creatureTemplate->Name.c_str();
        if (!name)
            name = "Unknown creature";

        std::string message = acore::StringFormat("|cffff0000# |cff7bbef7%s|r и его группа убили мирового босса |cffff0000%s|r", player->GetName(), name);       

        sWorld->SendServerMessage(SERVER_MSG_STRING, message.c_str());
	};
};

class Boss_Announcer_World : public WorldScript
{
public:
    Boss_Announcer_World() : WorldScript("Boss_Announcer_World") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sGameConfig->AddBoolConfig("BossAnnouncer.Enable");
    }
};

// Group all custom scripts
void AddSC_BossAnnouncer()
{
    new Boss_Announcer_Player();
    new Boss_Announcer_World();
}
