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
#include "sethekk_halls.h"

enum TailonkingIkiss
{
    SAY_INTRO                   = 0,
    SAY_AGGRO                   = 1,
    SAY_SLAY                    = 2,
    SAY_DEATH                   = 3,
    EMOTE_ARCANE_EXP            = 4,

    SPELL_BLINK                 = 38194,
    SPELL_BLINK_TELEPORT        = 38203,
    SPELL_MANA_SHIELD           = 38151,
    SPELL_ARCANE_BUBBLE         = 9438,
    SPELL_SLOW                  = 35032,
    SPELL_POLYMORPH_N           = 38245, // Difficulty data
    SPELL_POLYMORPH_H           = 43309,
    SPELL_ARCANE_VOLLEY_N       = 35059, // Difficulty data
    SPELL_ARCANE_VOLLEY_H       = 40424,
    SPELL_ARCANE_EXPLOSION_N    = 38197, // Difficulty data
    SPELL_ARCANE_EXPLOSION_H    = 40425,

    EVENT_SPELL_BLINK           = 1,
    EVENT_SPELL_POLYMORPH       = 2,
    EVENT_SPELL_SLOW            = 3,
    EVENT_SPELL_ARCANE_VOLLEY   = 4,
    EVENT_SPELL_ARCANE_EXPLO    = 5,
    EVENT_HEALTH_CHECK          = 6,
    EVENT_SPELL_BLINK_2         = 7
};

class boss_talon_king_ikiss : public CreatureScript
{
public:
    boss_talon_king_ikiss() : CreatureScript("boss_talon_king_ikiss") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_talon_king_ikissAI (creature);
    }

    struct boss_talon_king_ikissAI : public ScriptedAI
    {
        boss_talon_king_ikissAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;
        bool _spoken;

        void Reset()
        {
            _spoken = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!_spoken && who->GetTypeId() == TYPEID_PLAYER)
            {
                Talk(SAY_INTRO);
                _spoken = true;
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);

            events.ScheduleEvent(EVENT_SPELL_BLINK, 35s);
            events.ScheduleEvent(EVENT_SPELL_ARCANE_VOLLEY, 5s);
            events.ScheduleEvent(EVENT_SPELL_POLYMORPH, 8s);
            events.ScheduleEvent(EVENT_HEALTH_CHECK, 2s);
            
            if (IsHeroic())
                events.ScheduleEvent(EVENT_SPELL_SLOW, 15s, 25s);
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);

            if (InstanceScript* instance = me->GetInstanceScript())
                instance->SetData(DATA_IKISSDOOREVENT, DONE);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            if (urand(0,1))
                Talk(SAY_SLAY);
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
                case EVENT_SPELL_ARCANE_VOLLEY:
                    me->CastSpell(me, SPELL_ARCANE_VOLLEY_N, false);
                    events.Repeat(7s, 12s);
                    break;
                case EVENT_SPELL_POLYMORPH:
                    if (Unit* target = (IsHeroic() ? SelectTarget(SELECT_TARGET_RANDOM, 0) : SelectTarget(SELECT_TARGET_TOPAGGRO, 1)))
                        me->CastSpell(target, SPELL_POLYMORPH_N, false);
                    events.Repeat(15s, 17500ms);
                    break;
                case EVENT_SPELL_SLOW:
                    me->CastSpell(me, SPELL_SLOW, false);
                    events.Repeat(15s, 30s);
                    break;
                case EVENT_HEALTH_CHECK:
                    if (me->HealthBelowPct(20))
                    {
                        me->CastSpell(me, SPELL_MANA_SHIELD, false);
                        return;
                    }
                    events.Repeat(1s);
                    break;
                case EVENT_SPELL_BLINK:
                    Talk(EMOTE_ARCANE_EXP);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    {
                        me->CastSpell(target, SPELL_BLINK, false);
                        me->NearTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation());

                        DoCast(target, SPELL_BLINK_TELEPORT);
                    }
                    events.Repeat(35s, 40s);
                    events.DelayEvents(500);
                    events.ScheduleEvent(EVENT_SPELL_BLINK_2, 0s);
                    return;
                case EVENT_SPELL_BLINK_2:
                    me->CastSpell(me, SPELL_ARCANE_EXPLOSION_N, false);
                    me->CastSpell(me, SPELL_ARCANE_BUBBLE, true);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };
};

enum Anzu
{
    SAY_ANZU_INTRO1             = 0,
    SAY_ANZU_INTRO2             = 1,
    SAY_SUMMON                  = 2,

    SPELL_PARALYZING_SCREECH    = 40184,
    SPELL_SPELL_BOMB            = 40303,
    SPELL_CYCLONE               = 40321,
    SPELL_BANISH_SELF           = 42354,
    SPELL_SHADOWFORM            = 40973,

    EVENT_SPELL_SCREECH         = 1,
    EVENT_SPELL_BOMB            = 2,
    EVENT_SPELL_CYCLONE         = 3,
    EVENT_ANZU_HEALTH1          = 4,
    EVENT_ANZU_HEALTH2          = 5
};

class boss_anzu : public CreatureScript
{
public:
    boss_anzu() : CreatureScript("boss_anzu") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_anzuAI (creature);
    }

    struct boss_anzuAI : public ScriptedAI
    {
        boss_anzuAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            talkTimer = 1;
            me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->AddAura(SPELL_SHADOWFORM, me);
        }

        EventMap events;
        SummonList summons;
        uint32 talkTimer;

        void Reset()
        {
            summons.DespawnAll();
            if (InstanceScript* instance = me->GetInstanceScript())
                if (instance->GetData(TYPE_ANZU_ENCOUNTER) != DONE)
                    instance->SetData(TYPE_ANZU_ENCOUNTER, NOT_STARTED);
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
            summon->AI()->AttackStart(me->GetVictim());
        }

        void SummonedCreatureDies(Creature* summon, Unit*)
        {
            summons.Despawn(summon);
            summons.RemoveNotExisting();
            if (summons.empty())
                me->RemoveAurasDueToSpell(SPELL_BANISH_SELF);
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SPELL_SCREECH, 14s);
            events.ScheduleEvent(EVENT_SPELL_BOMB, 5s);
            events.ScheduleEvent(EVENT_SPELL_CYCLONE, 8s);
            events.ScheduleEvent(EVENT_ANZU_HEALTH1, 2s);
            events.ScheduleEvent(EVENT_ANZU_HEALTH2, 2001ms);

            if (InstanceScript* instance = me->GetInstanceScript())
                instance->SetData(TYPE_ANZU_ENCOUNTER, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (InstanceScript* instance = me->GetInstanceScript())
                instance->SetData(TYPE_ANZU_ENCOUNTER, DONE);
        }

        void SummonBroods()
        {
            Talk(SAY_SUMMON);
            me->CastSpell(me, SPELL_BANISH_SELF, true);
            for (uint8 i = 0; i < 5; ++i)
                me->SummonCreature(23132 /*NPC_BROOD_OF_ANZU*/, me->GetPositionX()+20*cos((float)i), me->GetPositionY()+20*sin((float)i), me->GetPositionZ()+25.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
        }

        void UpdateAI(uint32 diff)
        {
            if (talkTimer)
            {
                talkTimer += diff;
                if (talkTimer >= 1000 && talkTimer < 10000)
                {
                    Talk(SAY_ANZU_INTRO1);
                    talkTimer = 10000;
                }
                else if (talkTimer >= 16000)
                {
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
                    me->RemoveAurasDueToSpell(SPELL_SHADOWFORM);
                    Talk(SAY_ANZU_INTRO2);
                    talkTimer = 0;
                }
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING|UNIT_STATE_STUNNED))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_SPELL_SCREECH:
                    me->CastSpell(me, SPELL_PARALYZING_SCREECH, false);
                    events.Repeat(23s);
                    events.DelayEvents(3000);
                    break;
                case EVENT_SPELL_BOMB:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_SPELL_BOMB, false);
                    events.Repeat(16s, 24500ms);
                    events.DelayEvents(3000);
                    break;
                case EVENT_SPELL_CYCLONE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true))
                        me->CastSpell(target, SPELL_CYCLONE, false);
                    events.Repeat(22s, 27s);
                    events.DelayEvents(3000);
                    break;
                case EVENT_ANZU_HEALTH1:
                    if (me->HealthBelowPct(66))
                    {
                        SummonBroods();
                        events.DelayEvents(10000);
                        return;
                    }
                    events.Repeat(1s);
                    break;
                case EVENT_ANZU_HEALTH2:
                    if (me->HealthBelowPct(33))
                    {
                        SummonBroods();
                        events.DelayEvents(10000);
                        return;
                    }
                    events.Repeat(1s);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_talon_king_ikiss()
{
    new boss_talon_king_ikiss();
    new boss_anzu();
}
