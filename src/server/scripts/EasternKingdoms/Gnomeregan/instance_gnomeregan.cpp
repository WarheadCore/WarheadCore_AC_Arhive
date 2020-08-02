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

#include "Player.h"
#include "SpellScript.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "PassiveAI.h"

class instance_gnomeregan : public InstanceMapScript
{
    public:
        instance_gnomeregan() : InstanceMapScript("instance_gnomeregan", 90) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_gnomeregan_InstanceMapScript(map);
        }

        struct instance_gnomeregan_InstanceMapScript : public InstanceScript
        {
            instance_gnomeregan_InstanceMapScript(Map* map) : InstanceScript(map)
            {
            }
        };
};

enum eKernobee
{
    QUEST_A_FINE_MESS           = 2904,
};

class npc_kernobee : public CreatureScript
{
    public:
        npc_kernobee() : CreatureScript("npc_kernobee") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kernobeeAI(creature);
        }

        bool OnQuestAccept(Player* player, Creature* creature, const Quest* quest)
        {
            if (quest->GetQuestId() == QUEST_A_FINE_MESS)
            {
                creature->SetStandState(UNIT_STAND_STATE_STAND);
                creature->AI()->SetGUID(player->GetGUID(), 0);
                creature->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, M_PI, MOTION_SLOT_CONTROLLED);
            }
            return true;
        }

        struct npc_kernobeeAI : public PassiveAI
        {
            npc_kernobeeAI(Creature* creature) : PassiveAI(creature)
            {
                playerGUID = 0;
                checkTimer = 0;
            }

            uint32 checkTimer;
            uint64 playerGUID;

            void SetGUID(uint64 guid, int32)
            {
                playerGUID = guid;
            }

            void UpdateAI(uint32 diff)
            {
                checkTimer += diff;
                if (checkTimer >= 2000)
                {
                    checkTimer = 0;
                    if (me->GetDistance(-332.2f, -2.8f, -152.8f) < 5.0f)
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            player->GroupEventHappens(QUEST_A_FINE_MESS, me);
                        me->DespawnOrUnsummon(1000);
                    }
                }
            }
        };
};

class spell_gnomeregan_radiation_bolt : public SpellScriptLoader
{
    public:
        spell_gnomeregan_radiation_bolt() : SpellScriptLoader("spell_gnomeregan_radiation_bolt") { }

        class spell_gnomeregan_radiation_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gnomeregan_radiation_bolt_SpellScript);

            void HandleTriggerSpell(SpellEffIndex effIndex)
            {
                if (roll_chance_i(80))
                    PreventHitDefaultEffect(effIndex);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_gnomeregan_radiation_bolt_SpellScript::HandleTriggerSpell, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gnomeregan_radiation_bolt_SpellScript;
        }
};

void AddSC_instance_gnomeregan()
{
    new instance_gnomeregan();
    new npc_kernobee();
    new spell_gnomeregan_radiation_bolt();
}
