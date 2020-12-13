-- Add Lich king to all difficulties
DELETE FROM `creature`
WHERE (`id` = 16980)
    AND (`guid` IN (1971311));

INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`)
        VALUES (1971311, 16980, 533, 0, 0, 15, 1, 0, 0, 3749.68, - 5114.06, 142.115, 2.93215, 604800, 0, 0, 1, 0, 0, 0, 0, 0, '', 0);
