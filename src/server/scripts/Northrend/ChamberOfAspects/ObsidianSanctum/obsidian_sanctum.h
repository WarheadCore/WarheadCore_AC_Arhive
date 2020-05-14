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

#ifndef DEF_OBSIDIAN_SANCTUM_H
#define DEF_OBSIDIAN_SANCTUM_H

enum Data
{
    // Encounter
    BOSS_SARTHARION_EVENT           = 0,
    BOSS_TENEBRON_EVENT             = 1,
    BOSS_VESPERON_EVENT             = 2,
    BOSS_SHADRON_EVENT              = 3,
    MAX_ENCOUNTERS                  = 4,

    // GUIDs
    DATA_SARTHARION                 = 10,
    DATA_TENEBRON                   = 11,
    DATA_SHADRON                    = 12,
    DATA_VESPERON                   = 13,

    // Portal
    DATA_CLEAR_PORTAL               = 20,
    DATA_ADD_PORTAL                 = 21,

    // Achievements
    DATA_ACHIEVEMENT_DRAGONS_COUNT  = 30,
    DATA_VOLCANO_BLOWS              = 31,

    // NPCs
    NPC_SARTHARION                  = 28860,
    NPC_TENEBRON                    = 30452,
    NPC_SHADRON                     = 30451,
    NPC_VESPERON                    = 30449,
    NPC_FIRE_CYCLONE                = 30648,

    // GOs
    GO_TWILIGHT_PORTAL              = 193988,
    GO_NORMAL_PORTAL                = 193989,

    // Spells
    SPELL_TWILIGHT_SHIFT            = 57620,
    SPELL_TWILIGHT_TORMENT_SARTHARION = 58835,
};

#endif
