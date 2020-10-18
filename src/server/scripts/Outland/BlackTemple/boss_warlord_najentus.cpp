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
#include "black_temple.h"

enum Yells
{
    SAY_AGGRO                       = 0,
    SAY_NEEDLE                      = 1,
    SAY_SLAY                        = 2,
    SAY_SPECIAL                     = 3,
    SAY_ENRAGE                      = 4,
    SAY_DEATH                       = 5
};

enum Spells
{
    SPELL_NEEDLE_SPINE              = 39992,
    SPELL_NEEDLE_SPINE_DAMAGE       = 39835,
    SPELL_TIDAL_BURST               = 39878,
    SPELL_TIDAL_SHIELD              = 39872,
    SPELL_IMPALING_SPINE            = 39837,
    SPELL_SUMMON_IMPALING_SPINE     = 39929,
    SPELL_BERSERK                   = 26662
};

enum Events
{
    EVENT_SPELL_BERSERK             = 1,
    EVENT_YELL                      = 2,
    EVENT_SPELL_NEEDLE              = 3,
    EVENT_SPELL_SPINE               = 4,
    EVENT_SPELL_SHIELD              = 5,
    EVENT_KILL_SPEAK                = 6
};

class boss_najentus : public CreatureScript
{
public:
    boss_najentus() : CreatureScript("boss_najentus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetInstanceAI<boss_najentusAI>(creature);
    }

    struct boss_najentusAI : public BossAI
    {
        boss_najentusAI(Creature* creature) : BossAI(creature, DATA_HIGH_WARLORD_NAJENTUS)
        {
        }

        void Reset()
        {
            BossAI::Reset();
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() == TYPEID_PLAYER && events.GetNextEventTime(EVENT_KILL_SPEAK) == 0)
            {
                Talk(SAY_SLAY);
                events.ScheduleEvent(EVENT_KILL_SPEAK, 5s);
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
            events.ScheduleEvent(EVENT_SPELL_BERSERK, 8min);
            events.ScheduleEvent(EVENT_YELL, 25s, 100s);
            events.RescheduleEvent(EVENT_SPELL_NEEDLE, 10s);
            events.RescheduleEvent(EVENT_SPELL_SPINE, 20001ms);
            events.RescheduleEvent(EVENT_SPELL_SHIELD, 1min);
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
                case EVENT_SPELL_SHIELD:
                    me->CastSpell(me, SPELL_TIDAL_SHIELD, false);
                    events.DelayEvents(10s);
                    events.ScheduleEvent(EVENT_SPELL_SHIELD, 1min);
                    break;
                case EVENT_SPELL_BERSERK:
                    Talk(SAY_ENRAGE);
                    me->CastSpell(me, SPELL_BERSERK, true);
                    break;
                case EVENT_SPELL_NEEDLE:
                    me->CastCustomSpell(SPELL_NEEDLE_SPINE, SPELLVALUE_MAX_TARGETS, 3, me, false);
                    events.ScheduleEvent(EVENT_SPELL_NEEDLE, 15s);
                    break;
                case EVENT_SPELL_SPINE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    {
                        me->CastSpell(target, SPELL_IMPALING_SPINE, false);
                        target->CastSpell(target, SPELL_SUMMON_IMPALING_SPINE, true);
                        Talk(SAY_NEEDLE);
                    }
                    events.ScheduleEvent(EVENT_SPELL_SPINE, 20s);
                    return;
                case EVENT_YELL:
                    Talk(SAY_SPECIAL);
                    events.ScheduleEvent(EVENT_YELL, 25s, 100s);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };
};

class spell_najentus_needle_spine : public SpellScriptLoader
{
public:
    spell_najentus_needle_spine() : SpellScriptLoader("spell_najentus_needle_spine") { }

    class spell_najentus_needle_spine_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_najentus_needle_spine_SpellScript);

        void HandleDummy(SpellEffIndex  /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, SPELL_NEEDLE_SPINE_DAMAGE, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_najentus_needle_spine_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_najentus_needle_spine_SpellScript();
    }
};

class spell_najentus_hurl_spine : public SpellScriptLoader
{
public:
    spell_najentus_hurl_spine() : SpellScriptLoader("spell_najentus_hurl_spine") { }

    class spell_najentus_hurl_spine_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_najentus_hurl_spine_SpellScript);

        void HandleSchoolDamage(SpellEffIndex  /*effIndex*/)
        {
            Unit* target = GetHitUnit();
            if (target && target->HasAura(SPELL_TIDAL_SHIELD))
            {
                target->RemoveAurasDueToSpell(SPELL_TIDAL_SHIELD);
                target->CastSpell(target, SPELL_TIDAL_BURST, true);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_najentus_hurl_spine_SpellScript::HandleSchoolDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_najentus_hurl_spine_SpellScript();
    }
};

void AddSC_boss_najentus()
{
    new boss_najentus();
    new spell_najentus_needle_spine();
    new spell_najentus_hurl_spine();
}
