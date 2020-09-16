-- Solo queue
-- NPC

SET @Entry = 190014, @Name = "Solo Queue";

DELETE FROM `creature_template`
WHERE `entry` = @Entry;

INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `scale`, `rank`, `dmgschool`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`)
        VALUES (@Entry, 19646, 0, @Name, NULL, NULL, 0, 80, 80, 2, 35, 1, 1, 0, 0, 2000, 0, 1, 0, 7, 138936390, 0, 0, 0, '', 0, 3, 1, 0, 0, 1, 0, 0, 'ArenaSolo_Creature');
-- Trinity String

DELETE FROM `acore_string`
WHERE entry = 11200;

INSERT INTO `acore_string`
        VALUES (11200, 'Arena disabled', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11201;

INSERT INTO `acore_string`
        VALUES (11201, 'Arena team successful created', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11202;

INSERT INTO `acore_string`
        VALUES (11202, 'You must have at least 15 minutes of played time before you may queue. Please spend a little more time setting up your character, and try again soon.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11203;

INSERT INTO `acore_string`
        VALUES (11203, 'Your current played time: %u minutes, but need 15.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11204;

INSERT INTO `acore_string`
        VALUES (11204, 'You cannot join Arena now because there is already someone queued with the same IP address.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11205;

INSERT INTO `acore_string`
        VALUES (11205, 'Your character is not fully equipped.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11206;

INSERT INTO `acore_string`
        VALUES (11206, 'You are in Arena Queue', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11207;

INSERT INTO `acore_string`
        VALUES (11207, 'Something went wrong while joining queue. Already in another queue?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11208;

INSERT INTO `acore_string`
        VALUES (11208, 'You have currently ', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11209;

INSERT INTO `acore_string`
        VALUES (11209, 'free talent points.Please spend all your talent points before queueing arena.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11210;

INSERT INTO `acore_string`
        VALUES (11210, 'You need level %u to create an arena team.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11211;

INSERT INTO `acore_string`
        VALUES (11211, 'You can not join, because you have invested too many points in a forbidden talent. Please edit your talents.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `acore_string`
WHERE entry = 11212;

INSERT INTO `acore_string`
        VALUES (11212, 'You can not change your talents while in queue for 1v1 or 3v3 Solo.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO npc_text (ID,
    Text0_0,
    Text0_1)
VALUES (32000,
    ' ',
    ' ');

REPLACE gossip_menu
VALUES (32000,
    32000);

