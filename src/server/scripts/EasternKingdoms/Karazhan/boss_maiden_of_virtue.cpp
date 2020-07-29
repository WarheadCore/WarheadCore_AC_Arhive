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
#include "ScriptedCreature.h"
#include "karazhan.h"

enum MaidenOfVirtue
{
    SAY_AGGRO                   = 0,
    SAY_SLAY                    = 1,
    SAY_REPENTANCE              = 2,
    SAY_DEATH                   = 3,

    SPELL_REPENTANCE            = 29511,
    SPELL_HOLY_FIRE             = 29522,
    SPELL_HOLY_WRATH            = 32445,
    SPELL_HOLY_GROUND           = 29523,
    SPELL_BERSERK               = 26662,

    EVENT_SPELL_REPENTANCE      = 1,
    EVENT_SPELL_HOLY_FIRE       = 2,
    EVENT_SPELL_HOLY_WRATH      = 3,
    EVENT_SPELL_ENRAGE          = 4,
    EVENT_KILL_TALK             = 5
};

class boss_maiden_of_virtue : public CreatureScript
{
    public:
        boss_maiden_of_virtue() : CreatureScript("boss_maiden_of_virtue") { }

        struct boss_maiden_of_virtueAI : public BossAI
        {
            boss_maiden_of_virtueAI(Creature* creature) : BossAI(creature, DATA_MAIDEN) { }

            void Reset()
            {
                BossAI::Reset();
            }

            void KilledUnit(Unit* /*victim*/)
            {
                if (events.GetNextEventTime(EVENT_KILL_TALK) == 0)
                {
                    Talk(SAY_SLAY);
                    events.ScheduleEvent(EVENT_KILL_TALK, 5s);
                }
            }

            void JustDied(Unit* killer)
            {
                BossAI::JustDied(killer);
                Talk(SAY_DEATH);
            }

            void EnterCombat(Unit* who)
            {
                BossAI::EnterCombat(who);
                Talk(SAY_AGGRO);

                me->CastSpell(me, SPELL_HOLY_GROUND, true);
                events.ScheduleEvent(EVENT_SPELL_REPENTANCE, 25s);
                events.ScheduleEvent(EVENT_SPELL_HOLY_FIRE, 8s);
                events.ScheduleEvent(EVENT_SPELL_HOLY_WRATH, 15s);
                events.ScheduleEvent(EVENT_SPELL_ENRAGE, 10min);
                DoZoneInCombat();
            }

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case EVENT_SPELL_REPENTANCE:
                        me->CastSpell(me, SPELL_REPENTANCE, true);
                        events.ScheduleEvent(EVENT_SPELL_REPENTANCE, 25s, 35s);
                        break;
                    case EVENT_SPELL_HOLY_FIRE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            me->CastSpell(target, SPELL_HOLY_FIRE, true);
                        events.ScheduleEvent(EVENT_SPELL_HOLY_FIRE, 8s, 18s);
                        break;
                    case EVENT_SPELL_HOLY_WRATH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                            me->CastSpell(target, SPELL_HOLY_WRATH, true);
                        events.ScheduleEvent(EVENT_SPELL_HOLY_WRATH, 20s, 25s);
                        break;
                    case EVENT_SPELL_ENRAGE:
                        me->CastSpell(me, SPELL_BERSERK, true);
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetInstanceAI<boss_maiden_of_virtueAI>(creature);
        }
};

void AddSC_boss_maiden_of_virtue()
{
    new boss_maiden_of_virtue();
}
