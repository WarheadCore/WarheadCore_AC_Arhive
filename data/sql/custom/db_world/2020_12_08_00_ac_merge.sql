/* brewfest camp hostile mobs */
DELETE FROM `game_event_creature` WHERE `guid` IN (7370, 22181, 22188, 22473);
INSERT INTO `game_event_creature` (`eventEntry`, `guid`) VALUES
(-24,7370),
(-24,22181),
(-24,22188),
(-24,22473);

/* brewfest camp non-hostile mobs */
DELETE FROM `game_event_creature` WHERE `guid` IN (12369, 12372, 21020, 21022, 21026);
INSERT INTO `game_event_creature` (`eventEntry`, `guid`) VALUES
(-24,12369),
(-24,12372),
(-24,21020),
(-24,21022),
(-24,21026);

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 18693;

-- Passive
DELETE FROM `creature_text` WHERE  `CreatureID`=18693 AND `GroupID`=0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `id`, `text`, `type`, `language`, `Probability`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(18693, 0, 0, 'Mog\'dorg is a fool to think that one of the smaller races will be our savior.', 12, 0, 16, 21025, 0, 'Speaker Mar\'grom - Say Speech 1'),
(18693, 0, 1, 'Do not listen to Mog\'dorg\'s lies! Ogri\'la is a myth! We make our own heaven here with the blood of any that would divide and subjugate us!', 12, 0, 16, 21029, 0, 'Speaker Mar\'grom - Say Speech 2'),
(18693, 0, 2, 'The time has come. Strike down every creature that stands in the way of the clans!', 12, 0, 16, 21028, 0, 'Speaker Mar\'grom - Say Speech 3'),
(18693, 0, 3, 'Will we never learn to work together? Or will we simply kill each other off and be forgotten in time.', 12, 0, 16, 19882, 0, 'Speaker Mar\'grom - Say Speech 4'),
(18693, 0, 4, 'Do not bow to Mog\'dorg\'s puppet! He would replace our gronn overlords with a liege of lesser stock. Why should we replace one master with another?', 12, 0, 16, 21027, 0, 'Speaker Mar\'grom - Say Speech 5'),
(18693, 0, 5, 'If only the leaders of the clans would listen. Unity is the only answer. All other paths lead to our destruction.', 12, 0, 16, 19883, 0, 'Speaker Mar\'grom - Say Speech 6');

-- Combat
DELETE FROM `creature_text` WHERE  `CreatureID`=18693 AND `GroupID`=1;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `id`, `text`, `type`, `language`, `Probability`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(18693, 1, 0, 'My message of unity is meant for the Ogre clans, not for you, $R!', 12, 0, 25, 19884, 0, 'Speaker Mar\'grom - Say aggro 1'),
(18693, 1, 1, 'I will not allow you to kill any more of my kind! Prepare to die!', 12, 0, 25, 19885, 0, 'Speaker Mar\'grom - Say aggro 2'),
(18693, 1, 2, 'I decree that you are an enemy of the Ogre clans. Punishment is death!', 12, 0, 25, 19886, 0, 'Speaker Mar\'grom - Say aggro 3'),
(18693, 1, 3, 'The Ogre clans will not be subjugated. Not by the gronn, and not by a puny $R like you!', 12, 0, 25, 21024, 0, 'Speaker Mar\'grom - Say aggro 4');

DELETE FROM `smart_scripts` WHERE  `entryorguid`=18693 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES	
-- Speech (Passive)
(18693, 0, 0, 0, 1, 0, 100, 0, 15000, 30000, 40000, 60000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - In Combat - Say Speech'),
-- Quotes (Combat)
(18693, 0, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - In Combat - Say Quote'),
-- Spells at combat per HP
(18693, 0, 2, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 37844, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - On Aggro - Cast 37844'),
(18693, 0, 3, 0, 2, 0, 100, 0, 60, 99, 60, 99, 0, 11, 12466, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - Between 60-99% Health - Cast 12466'),
(18693, 0, 4, 0, 2, 0, 100, 0, 19, 69, 19, 69, 0, 11, 15497, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - Between 19-69% Health - Cast 15497'),
(18693, 0, 5, 0, 2, 0, 100, 0, 0, 19, 0, 19, 0, 11, 15241, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Speaker Mar\'grom - Between 0-19% Health - Cast 15241');

/*
 * Zone: Eastern Plaguelands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 17878;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 17878);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17878, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 7978, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scourge Siege Engineer - On Aggro - Cast \'7978\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 15162;
UPDATE `creature_template` SET `unit_class`= 2 WHERE `entry`= 15162;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 15162);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15162, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 20294, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Inquisitor - On Aggro - Cast \'20294\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8606;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8606);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8606, 0, 0, 0, 2, 0, 100, 1, 40, 80, 0, 0, 11, 11442, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Living Decay - Between 40-80% Health - Cast \'11442\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11290;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11290);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11290, 0, 0, 0, 0, 0, 100, 0, 2300, 3400, 9800, 12600, 11, 3234, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mossflayer Zombie - In Combat - Cast \'3234\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8596;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8596);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8596, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 11200, 13700, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plaguehound Runt - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12387;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12387);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12387, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 13901, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Large Vile Slime - On Aggro - Cast \'13901\''),
(12387, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 16843, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Large Vile Slime - Between 30-60% Health - Cast \'16843\' (No Repeat)'),
(12387, 0, 2, 0, 2, 0, 100, 1, 10, 20, 0, 0, 11, 11975, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Large Vile Slime - Between 10-20% Health - Cast \'11975\' (No Repeat)');

/*
 * Zone: Western Plaguelands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11873;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11873);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11873, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 11443, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spectral Attendant - On Aggro - Cast \'11443\''),
(11873, 0, 1, 0, 0, 0, 100, 0, 2200, 3400, 9800, 12700, 11, 13860, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spectral Attendant - In Combat - Cast \'13860\''),
(11873, 0, 2, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 11981, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spectral Attendant - Between 30-60% Health - Cast \'11981\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10580;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10580);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10580, 0, 0, 0, 0, 0, 100, 0, 2400, 3700, 8600, 9800, 11, 8714, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fetid Zombie - In Combat - Cast \'8714\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4472;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4472);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4472, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 11443, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Haunting Vision - On Aggro - Cast \'11443\''),
(4472, 0, 1, 0, 0, 0, 100, 0, 2200, 3400, 9800, 12700, 11, 13860, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Haunting Vision - In Combat - Cast \'13860\''),
(4472, 0, 2, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 11981, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Haunting Vision - Between 30-60% Health - Cast \'11981\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1813;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1813);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1813, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 32065, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Decaying Horror - On Aggro - Cast \'32065\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1824;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1824);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1824, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3436, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plague Lurker - Between 20-80% Health - Cast \'3436\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11613;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11613);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11613, 0, 0, 0, 0, 0, 100, 0, 2700, 3900, 9700, 12400, 11, 11976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Huntsman Radley - In Combat - Cast \'11976\''),
(11613, 0, 1, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 14443, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Huntsman Radley - Between 20-80% Health - Cast \'14443\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11614;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11614);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11614, 0, 0, 0, 0, 0, 100, 0, 2100, 3200, 17100, 18200, 11, 13692, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bloodshot - In Combat - Cast \'13692\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1836;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1836);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1836, 0, 0, 0, 0, 0, 100, 0, 2500, 3000, 9500, 10000, 11, 16856, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Cavalier - In Combat - Cast \'16856\''),
(1836, 0, 1, 0, 2, 0, 100, 1, 40, 80, 0, 0, 11, 15284, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Cavalier - Between 40-80% Health - Cast \'15284\' (No Repeat)'),
(1836, 0, 2, 0, 2, 0, 100, 1, 10, 30, 0, 0, 11, 6253, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Cavalier - Between 10-30% Health - Cast \'6253\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10608;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10608);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10608, 0, 0, 0, 1, 0, 100, 0, 0, 0, 0, 0, 11, 13864, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Priest - Out of Combat - Cast \'13864\''),
(10608, 0, 1, 0, 0, 0, 100, 0, 2300, 5200, 8600, 9300, 11, 15498, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Priest - In Combat - Cast \'15498\''),
(10608, 0, 2, 0, 2, 0, 100, 1, 60, 90, 0, 0, 11, 8362, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Priest - Between 60-90% Health - Cast \'8362\' (No Repeat)'),
(10608, 0, 3, 0, 2, 0, 100, 1, 5, 25, 0, 0, 11, 12039, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Priest - Between 5-25% Health - Cast \'12039\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 11611;
UPDATE `creature_addon` SET `auras`= 13008 WHERE `guid`=45432;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11611);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11611, 0, 0, 0, 0, 0, 100, 0, 2300, 3100, 8900, 9600, 11, 13953, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cavalier Durgen - In Combat - Cast \'13953\''),
(11611, 0, 1, 0, 2, 0, 100, 1, 10, 30, 0, 0, 11, 13005, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cavalier Durgen - Between 10-30% Health - Cast \'13005\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1884;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1884);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1884, 0, 0, 0, 0, 0, 100, 0, 2500, 3000, 17500, 18000, 11, 11977, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Lumberjack - In Combat - Cast \'11977\''),
(1884, 0, 1, 0, 2, 0, 100, 1, 25, 40, 0, 0, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Lumberjack - Between 25-40% Health - Cast \'15496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1785;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1785);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1785, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 17650, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Terror - On Aggro - Cast \'17650\''),
(1785, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 12542, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skeletal Terror - Between 5-30% Health - Cast \'12542\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10605;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10605);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10605, 0, 0, 0, 1, 0, 100, 0, 0, 0, 0, 0, 11, 17175, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Medic - Out of Combat - Cast \'17175\''),
(10605, 0, 1, 0, 2, 0, 100, 1, 5, 95, 0, 0, 11, 11640, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Medic - Between 5-95% Health - Cast \'11640\' (No Repeat)'),
(10605, 0, 2, 0, 2, 0, 100, 1, 5, 20, 0, 0, 11, 17137, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Medic - Between 5-20% Health - Cast \'17137\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10801;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10801);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10801, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 15716, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jabbering Ghoul - On Aggro - Cast \'15716\''),
(10801, 0, 1, 0, 0, 0, 100, 0, 2500, 3000, 22500, 23000, 11, 12097, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Jabbering Ghoul - In Combat - Cast \'12097\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1822;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1822);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1822, 0, 0, 0, 0, 0, 100, 1, 2700, 4500, 0, 0, 11, 3583, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Venom Mist Lurker - In Combat - Cast \'3583\' (No Repeat)');

/*
 * Zone: Strangethorn Vale
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2635;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2635 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2635,0,0,0,0,0,100,0,3000,8000,13000,24000,11,3604,0,0,0,0,0,2,0,0,0,0,0,0,0,'Elder Saltwater Crocolisk - In Combat - Cast \'Tendon Rip\' (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=595;
DELETE FROM `smart_scripts` WHERE `entryorguid`=595 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(595,0,0,0,0,0,100,0,0,0,2100,4800,11,10277,64,0,0,0,0,2,0,0,0,0,0,0,0,'Bloodscalp Hunter - In Combat CMC - Cast \'Throw\''),
(595,0,1,0,0,0,100,0,9500,24100,18500,39800,11,6533,0,0,0,0,0,2,0,0,0,0,0,0,0,'Bloodscalp Hunter - In Combat - Cast \'Net\''),
(595,0,2,0,2,0,100,1,0,30,0,0,11,8599,1,0,0,0,0,1,0,0,0,0,0,0,0,'Bloodscalp Hunter - Between 0-30% Health - Cast \'Enrage\' (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=873;
DELETE FROM `smart_scripts` WHERE `entryorguid`=873 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(873,0,0,0,0,0,100,0,0,0,3400,4800,11,9532,64,0,0,0,0,2,0,0,0,0,0,0,0,'Saltscale Oracle - In Combat CMC - Cast \'Lightning Bolt\''),
(873,0,1,0,14,0,100,0,700,40,25000,35000,11,11986,1,0,0,0,0,7,0,0,0,0,0,0,0,'Saltscale Oracle - Friendly At 700 Health - Cast \'Healing Wave\''),
(873,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Saltscale Oracle - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=875;
DELETE FROM `smart_scripts` WHERE `entryorguid`=875 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(875,0,0,0,0,0,100,0,1000,1000,1800000,1800000,11,12544,0,0,0,0,0,1,0,0,0,0,0,0,0,'Saltscale Tide Lord - Out of Combat - Cast \'Frost Armor\''),
(875,0,1,0,0,0,100,0,0,0,2400,3800,11,9672,64,0,0,0,0,2,0,0,0,0,0,0,0,'Saltscale Tide Lord - In Combat CMC - Cast \'Frostbolt\''),
(875,0,2,0,0,0,100,0,6000,9000,15000,18500,11,11831,1,0,0,0,0,1,0,0,0,0,0,0,0,'Saltscale Tide Lord - In Combat - Cast \'Frost Nova\''),
(875,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Saltscale Tide Lord - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=877;
DELETE FROM `smart_scripts` WHERE `entryorguid`=877 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(877,0,0,0,9,0,100,0,0,5,20000,30000,11,13443,0,0,0,0,0,2,0,0,0,0,0,0,0,'Saltscale Forager - Within 0-5 Range - Cast \'Rend\''),
(877,0,1,0,0,0,100,0,2000,3000,4000,9000,11,744,32,0,0,0,0,2,0,0,0,0,0,0,0,'Saltscale Forager - In Combat - Cast \'Poison\''),
(877,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Saltscale Forager - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=879;
DELETE FROM `smart_scripts` WHERE `entryorguid`=879 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(879,0,0,0,0,0,100,0,0,0,2600,4800,11,10277,64,0,0,0,0,2,0,0,0,0,0,0,0,'Saltscale Hunter - In Combat CMC - Cast \'Throw\''),
(879,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Saltscale Hunter - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=871;
DELETE FROM `smart_scripts` WHERE `entryorguid`=871 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(871,0,0,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,7,0,0,0,0,0,0,0,'Saltscale Warrior - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=978;
DELETE FROM `smart_scripts` WHERE `entryorguid`=978 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(978,0,0,0,0,0,100,0,6000,11000,20000,30000,11,8053,0,0,0,0,0,2,0,0,0,0,0,0,0,'Kurzen Subchief - In Combat - Cast \'Flame Shock\'');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=979;
DELETE FROM `smart_scripts` WHERE `entryorguid`=979 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(979,0,0,0,0,0,100,0,0,0,2300,3900,11,6660,64,0,0,0,0,2,0,0,0,0,0,0,0,'Kurzen Shadow Hunter - In Combat CMC - Cast \'Shoot\''),
(979,0,1,0,0,0,100,0,9000,14000,9000,14000,11,8806,32,0,0,0,0,5,0,0,0,0,0,0,0,'Kurzen Shadow Hunter - In Combat - Cast \'Poisoned Shot\''),
(979,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Kurzen Shadow Hunter - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=813;
DELETE FROM `smart_scripts` WHERE `entryorguid`=813 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(813,0,0,0,0,0,100,0,8000,12000,18000,25000,11,8817,0,0,0,0,0,1,0,0,0,0,0,0,0,'Colonel Kurzen - In Combat - Cast \'Smoke Bomb\''),
(813,0,1,0,24,0,100,0,8817,0,100,100,11,8818,32,0,0,0,0,2,0,0,0,0,0,0,0,'Colonel Kurzen - On Target Has \'Smoke Bomb\' Aura - Cast \'Garrote\'');

DELETE FROM `creature_text` WHERE `CreatureID`=679;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(679,0,0,'Raaar!!! Me smash $r!',12,0,100,0,0,0,1927,0,'Mosh\'Ogg Shaman'),
(679,0,1,'Me smash! You die!',12,0,100,0,0,0,1926,0,'Mosh\'Ogg Shaman'),
(679,0,2,'I\'ll crush you!',12,0,100,0,0,0,1925,0,'Mosh\'Ogg Shaman');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=679;
DELETE FROM `smart_scripts` WHERE `entryorguid`=679 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(679,0,0,0,0,0,100,0,6500,10000,9500,12500,11,12058,0,0,0,0,0,2,0,0,0,0,0,0,0,'Mosh\'Ogg Shaman - In Combat - Cast \'Chain Lightning\''),
(679,0,1,0,2,0,100,0,0,30,34000,38000,11,6742,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mosh\'Ogg Shaman - Between 0-30% Health - Cast \'Bloodlust\''),
(679,0,2,0,0,0,100,0,9500,18000,33000,45000,11,11899,0,0,0,0,0,1,0,0,0,0,0,0,0,'Mosh\'Ogg Shaman - In Combat - Cast \'Healing Ward\''),
(679,0,3,0,4,0,15,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mosh\'Ogg Shaman - On Aggro - Say Line 0');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=14492;
DELETE FROM `smart_scripts` WHERE `entryorguid`=14492 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14492,0,0,0,9,0,100,0,0,5,23800,28200,11,12097,0,0,0,0,0,2,0,0,0,0,0,0,0,'Verifonix - Within 0-5 Range - Cast \'Pierce Armor\'');

DELETE FROM `creature_text` WHERE `CreatureID`=678;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(678,0,0,'Raaar!!! Me smash $r!',12,0,100,0,0,0,1927,0,'Mosh\'Ogg Mauler'),
(678,0,1,'Me smash! You die!',12,0,100,0,0,0,1926,0,'Mosh\'Ogg Mauler'),
(678,0,2,'I\'ll crush you!',12,0,100,0,0,0,1925,0,'Mosh\'Ogg Mauler');
DELETE FROM `smart_scripts` WHERE `entryorguid`=678 AND `id`=1 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(678,0,1,0,4,0,15,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mosh\'Ogg Mauler - On Aggro - Say Line 0');

DELETE FROM `creature_text` WHERE `CreatureID`=680;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(680,0,0,'Raaar!!! Me smash $r!',12,0,100,0,0,0,1927,0,'Mosh\'Ogg Lord'),
(680,0,1,'Me smash! You die!',12,0,100,0,0,0,1926,0,'Mosh\'Ogg Lord'),
(680,0,2,'I\'ll crush you!',12,0,100,0,0,0,1925,0,'Mosh\'Ogg Lord');
DELETE FROM `smart_scripts` WHERE `entryorguid`=680 AND `id`=1 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(680,0,1,0,4,0,15,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mosh\'Ogg Lord - On Aggro - Say Line 0');

DELETE FROM `creature_text` WHERE `CreatureID`=709;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(709,0,0,'Raaar!!! Me smash $r!',12,0,100,0,0,0,1927,0,'Mosh\'Ogg Warmonger'),
(709,0,1,'Me smash! You die!',12,0,100,0,0,0,1926,0,'Mosh\'Ogg Warmonger'),
(709,0,2,'I\'ll crush you!',12,0,100,0,0,0,1925,0,'Mosh\'Ogg Warmonger');
DELETE FROM `smart_scripts` WHERE `entryorguid`=709 AND `id`=1 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(709,0,1,0,4,0,15,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mosh\'Ogg Warmonger - On Aggro - Say Line 0');

DELETE FROM `creature_text` WHERE `CreatureID`=710;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(710,0,0,'Raaar!!! Me smash $r!',12,0,100,0,0,0,1927,0,'Mosh\'Ogg Spellcrafter'),
(710,0,1,'Me smash! You die!',12,0,100,0,0,0,1926,0,'Mosh\'Ogg Spellcrafter'),
(710,0,2,'I\'ll crush you!',12,0,100,0,0,0,1925,0,'Mosh\'Ogg Spellcrafter');
DELETE FROM `smart_scripts` WHERE `entryorguid`=710 AND `id`=3 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(710,0,3,0,4,0,15,0,0,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Mosh\'Ogg Spellcrafter - On Aggro - Say Line 0');

/*
 * Zone: Hinterlands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2657;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2657 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2657,0,0,0,9,0,100,0,0,5,15000,19000,11,3147,0,0,0,0,0,2,0,0,0,0,0,0,0,'Trained Razorbeak - In Combat - Cast Rend Flesh');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2923;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2923 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2923,0,0,0,0,0,100,0,4000,4500,22000,25000,11,8139,0,0,0,0,0,2,0,0,0,0,0,0,0,'Mangy Silvermane - In Combat - Cast Fevered Fatigue');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2925;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2925 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2925,0,0,0,0,0,100,0,2000,4500,12000,13000,11,32919,0,0,0,0,0,2,0,0,0,0,0,0,0,'Silvermane Howler - In Combat - Cast Snarl'),
(2925,0,1,0,0,0,100,0,6000,9000,18000,22000,11,3149,0,0,0,0,0,1,0,0,0,0,0,0,0,'Silvermane Howler - In Combat - Cast Furious Howl');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2926;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2926 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2926,0,0,0,0,0,100,0,4000,5000,17000,19000,11,7367,0,0,0,0,0,2,0,0,0,0,0,0,0,'Silvermane Stalker - In Combat - Cast Infected Bite'),
(2926,0,1,0,25,0,100,1,0,0,0,0,11,6408,0,0,0,0,0,1,0,0,0,0,0,0,0,'Silvermane Stalker - On Reset - Cast Faded (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8211;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8211 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8211,0,0,0,9,0,100,0,0,8,15000,18000,11,3264,0,0,0,0,0,1,0,0,0,0,0,0,0,'Old Cliff Jumper - Within 0-8 Range - Cast Blood Howl'),
(8211,0,1,0,9,0,100,0,0,5,20000,25000,11,3604,0,0,0,0,0,2,0,0,0,0,0,0,0,'Old Cliff Jumper - Within 0-5 Range - Cast Tendon Rip');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2927;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2927 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2927,0,0,0,9,0,100,0,0,5,18000,24000,11,7938,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vicious Owlbeast - Within 0-5 Range - Cast Fatal Bite');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2928;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2928 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2928,0,0,0,9,0,100,0,0,5,13000,16000,11,3252,0,0,0,0,0,2,0,0,0,0,0,0,0,'Primitive Owlbeast - Within 0-5 Range - Cast Shred');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8210;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8210 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8210,0,0,0,9,0,100,0,0,5,9000,12000,11,13584,0,0,0,0,0,2,0,0,0,0,0,0,0,'Razortalon - Within 0-5 Range - Cast Strike'),
(8210,0,1,0,9,0,100,0,0,5,20000,25000,11,3604,0,0,0,0,0,2,0,0,0,0,0,0,0,'Razortalon - Within 0-5 Range - Cast Tendon Rip'),
(8210,0,2,0,0,0,100,0,6000,11000,18000,21000,11,13443,0,0,0,0,0,2,0,0,0,0,0,0,0,'Razortalon - In Combat - Cast Rend');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2656;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2656 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2656,0,0,0,0,0,100,0,4000,6000,16000,18000,11,6907,0,0,0,0,0,2,0,0,0,0,0,0,0,'Jade Ooze - In Combat - Cast Diseased Slime');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2655;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2655 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2655,0,0,0,0,0,100,0,4000,6000,12000,16000,11,9459,0,0,0,0,0,2,0,0,0,0,0,0,0,'Green Sludge - In Combat - Cast Corrosive Ooze');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8212;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8212 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8212,0,0,0,0,0,100,0,6000,8000,18000,24000,11,7279,0,0,0,0,0,2,0,0,0,0,0,0,0,'The Reak - In Combat - Cast Black Sludge'),
(8212,0,1,0,0,0,100,0,4000,6000,28000,32000,11,21062,0,0,0,0,0,2,0,0,0,0,0,0,0,'The Reak - In Combat - Cast Putrid Breath');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2649;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2649 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2649,0,0,0,0,0,100,0,0,0,2100,4800,11,10277,64,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Scalper - In Combat CMC - Cast Throw'),
(2649,0,1,0,12,0,100,1,0,20,0,0,11,7160,1,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Scalper - Target Between 0-20% Health - Cast Execute (No Repeat)'),
(2649,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Scalper - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2650;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2650 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2650,0,0,0,2,0,100,1,0,30,0,0,11,8599,1,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Zealot - Between 0-30% Health - Cast Enrage (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2651;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2651 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2651,0,0,0,9,0,100,0,0,5,5000,9000,11,8721,0,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Hideskinner - In Combat - Cast Backstab'),
(2651,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Hideskinner - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2652;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2652 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2652,0,0,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Venomblood - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2653;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2653 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2653,0,0,0,1,0,100,1,1000,1000,0,0,11,7165,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Sadist - Out of Combat - Cast Battle Stance (No Repeat)'),
(2653,0,1,0,9,0,100,0,0,5,6000,9000,11,25710,0,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Sadist - Within 0-5 Range - Cast Heroic Strike (No Repeat)'),
(2653,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Sadist - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2654;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2654 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2654,0,0,0,0,0,100,0,0,0,3400,4800,11,20825,64,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Caller - Combat CMC - Cast Shadow Bolt'),
(2654,0,1,0,4,0,100,0,0,0,0,0,11,11017,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Caller - On Aggro - Summon Witherbark Felhunter'),
(2654,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherbark Caller - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=7767;
DELETE FROM `smart_scripts` WHERE `entryorguid`=7767 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7767,0,0,0,9,0,100,0,0,10,8000,14000,11,2691,0,0,0,0,0,2,0,0,0,0,0,0,0,'Witherbark Felhunter - Within 0-10 Range - Cast Mana Burn');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2686;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2686 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2686,0,0,0,6,0,100,0,0,0,0,0,11,11018,0,0,0,0,0,7,0,0,0,0,0,0,0,'Witherbark Broodguard - On Death - Summon Witherbark Bloodlings');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8218;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8218 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8218,0,0,0,25,0,100,0,0,0,0,0,11,22766,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witherheart the Stalker - On Reset - Cast Sneak'),
(8218,0,1,0,67,0,100,0,5000,9000,0,0,11,7159,0,0,0,0,0,2,0,0,0,0,0,0,0,'Witherheart the Stalker - On Behind Target - Cast Backstab');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8219;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8219 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8219,0,0,0,9,0,100,0,0,30,9000,14000,11,17228,0,0,0,0,0,2,0,0,0,0,0,0,0,'Zul\'arek Hatefowler - Within 0-30 Range - Cast Shadow Bolt Volley');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=17235;
DELETE FROM `smart_scripts` WHERE `entryorguid`=17235 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17235,0,0,0,0,0,100,0,15000,25000,30000,45000,11,5605,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witch Doctor Mai\'jin - In Combat - Cast Healing Ward (No Repeat)'),
(17235,0,1,0,0,0,100,0,6000,8000,25000,28000,11,8190,0,0,0,0,0,1,0,0,0,0,0,0,0,'Witch Doctor Mai\'jin - In Combat - Cast Magma Totem (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=17236;
DELETE FROM `smart_scripts` WHERE `entryorguid`=17236 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17236,0,0,0,6,0,100,0,0,0,0,0,11,11018,0,0,0,0,0,7,0,0,0,0,0,0,0,'Tcha\'kaz - On Death - Summon Witherbark Bloodlings');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2691;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2691 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2691,0,0,0,0,0,100,0,0,0,2300,3900,11,6660,64,0,0,0,0,2,0,0,0,0,0,0,0,'Highvale Outrunner - In Combat CMC - Cast Shoot'),
(2691,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Highvale Outrunner - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2692;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2692 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2692,0,0,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Highvale Scout - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2693;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2693 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2693,0,0,0,0,0,100,0,0,0,2300,3900,11,6660,64,0,0,0,0,2,0,0,0,0,0,0,0,'Highvale Marksman - In Combat CMC - Cast Shoot'),
(2693,0,1,0,9,0,100,0,5,30,9000,13000,11,14443,1,0,0,0,0,2,0,0,0,0,0,0,0,'Highvale Marksman - Within 5-30 Range - Cast Multi-Shot'),
(2693,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Highvale Marksman - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2694;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2694 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2694,0,0,0,9,0,100,0,0,30,30000,35000,11,6950,0,0,0,0,0,2,0,0,0,0,0,0,0,'Highvale Ranger - Within 0-30 Range - Cast Faerie Fire');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4467;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4467 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4467,0,0,0,0,0,100,0,0,0,3400,4700,11,20824,64,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Soothsayer - In Combat - Cast Lightning Bolt'),
(4467,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Soothsayer - Between 0-15% Health - Flee For Assist (No Repeat)'),
(4467,0,2,0,14,0,100,0,2500,40,20000,30000,11,10395,1,0,0,0,0,7,0,0,0,0,0,0,0,'Vilebranch Soothsayer - Friendly At 40% Health - Cast Healing Wave');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4466;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4466 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4466,0,0,0,12,0,100,1,0,20,0,0,11,7160,1,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Scalper - Target Between 0-20% Health - Cast Execute (No Repeat)'),
(4466,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Scalper - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=7996;
DELETE FROM `smart_scripts` WHERE `entryorguid`=7996 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7996,0,0,0,0,0,100,0,0,0,3400,4800,11,9613,64,0,0,0,0,2,0,0,0,0,0,0,0,'Qiaga the Keeper - In Combat CMC - Cast Shadow Bolt'),
(7996,0,1,0,0,0,100,0,4200,7700,7100,16200,11,11639,32,0,0,0,0,2,0,0,0,0,0,0,0,'Qiaga the Keeper - In Combat - Cast Shadow Word: Pain'),
(7996,0,2,0,14,0,100,0,1000,40,15000,19000,11,11640,1,0,0,0,0,7,0,0,0,0,0,0,0,'Qiaga the Keeper - Friendly At 40% Health - Cast Renew'),
(7996,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Qiaga the Keeperer - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8636;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8636 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8636,0,0,0,0,0,100,0,0,0,3400,4800,11,9613,64,0,0,0,0,2,0,0,0,0,0,0,0,'Morta\'gya the Keeper - In Combat CMC - Cast Shadow Bolt'),
(8636,0,1,0,0,0,100,0,4200,7700,7100,16200,11,11639,32,0,0,0,0,2,0,0,0,0,0,0,0,'Morta\'gya the Keeper - In Combat - Cast Shadow Word: Pain'),
(8636,0,2,0,14,0,100,0,1000,40,15000,19000,11,11640,1,0,0,0,0,7,0,0,0,0,0,0,0,'Morta\'gya the Keeper - Friendly At 40% Health - Cast Renew'),
(8636,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Morta\'gya the Keeper - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8216;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8216 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8216,0,0,0,2,0,100,1,0,30,0,0,11,8599,0,0,0,0,0,1,0,0,0,0,0,0,0,'Retherokk the Berserker - Between 0-30% Health - Cast Enrage (No Repeat)'),
(8216,0,1,0,0,0,100,0,2000,3000,8000,9000,11,3391,0,0,0,0,0,1,0,0,0,0,0,0,0,'Retherokk the Berserker - In Combat - Cast Thrash');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=7977;
DELETE FROM `smart_scripts` WHERE `entryorguid`=7977 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7977,0,0,0,9,0,100,0,0,5,45000,45000,11,6016,0,0,0,0,0,2,0,0,0,0,0,0,0,'Gammerita - Within 0-5 Range - Cast Pierce Armor');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4468;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4468 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4468,0,0,0,9,0,100,0,0,10,15000,29000,11,6814,0,0,0,0,0,2,0,0,0,0,0,0,0,'Jade Sludge - Within 0-10 Range - Cast Sludge Toxin');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4469;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4469 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4469,0,0,0,9,0,100,0,0,5,12000,19000,11,8245,0,0,0,0,0,2,0,0,0,0,0,0,0,'Emerald Ooze - Within 0-5 Range - Cast Corrosive Acid');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4465;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4465 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4465,0,0,0,9,0,100,0,0,5,5000,9000,11,11976,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Warrior - Within 0-5 Range - Cast Strike'),
(4465,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Warrior - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2640;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2640 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2640,0,0,0,0,0,100,0,0,0,3400,4800,11,9613,64,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Witch Doctor - In Combat CMC - Cast Shadow Bolt'),
(2640,0,1,0,14,0,100,0,1200,40,13000,17000,11,8005,0,0,0,0,0,7,0,0,0,0,0,0,0,'Vilebranch Witch Doctor - Friendly at 1200 Health - Cast Healing Wave'),
(2640,0,2,0,0,0,100,1,9000,13000,22000,28000,11,18503,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Witch Doctor - In Combat CMC - Cast Hex'),
(2640,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Witch Doctor - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2641;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2641 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2641,0,0,0,0,0,100,0,0,0,2300,3900,11,10277,64,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Headhunter - In Combat CMC - Cast Throw'),
(2641,0,1,0,9,0,100,0,0,5,15000,18000,11,7357,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Headhunter - Within 0-5 Range - Cast Poisonous Stab'),
(2641,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Headhunter - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2642;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2642 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2642,0,0,0,0,0,100,0,0,0,3400,4800,11,9613,64,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Shadowcaster - In Combat CMC - Cast Shadow Bolt'),
(2642,0,1,0,1,0,100,1,1000,1000,0,0,11,12746,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Shadowcaster - Out of Combat - Cast Summon Voidwalker'),
(2642,0,2,0,0,0,100,0,2500,10000,35000,40000,11,7289,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Shadowcaster - In Combat - Cast Shrink'),
(2642,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Shadowcaster - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2644;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2644 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2644,0,0,0,67,0,100,0,5000,9000,0,0,11,7159,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Hideskinner - On Behind Target - Cast Backstab'),
(2644,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Hideskinnerr - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2645;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2645 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2645,0,0,0,0,0,100,0,0,0,2300,3900,11,15547,64,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Shadow Hunter - Combat CMC - Cast Shoot'),
(2645,0,1,0,9,0,100,0,0,30,21000,26000,11,14032,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Shadow Hunter - Within 0-30 Range - Cast Shadow Word: Pain'),
(2645,0,2,0,0,0,100,0,4000,9000,15000,21000,11,9657,1,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Shadow Hunter - In Combat - Cast Shadow Shell'),
(2645,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Shadow Hunter - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=8217;
DELETE FROM `smart_scripts` WHERE `entryorguid`=8217 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8217,0,0,0,0,0,100,0,5000,9000,12000,16000,11,11436,0,0,0,0,0,2,0,0,0,0,0,0,0,'Mith\'rethis the Enchanter - In Combat - Cast Slow'),
(8217,0,1,0,9,0,100,0,0,30,24000,29000,11,15654,0,0,0,0,0,2,0,0,0,0,0,0,0,'Mith\'rethis the Enchanter - Within 0-30 Range - Cast Shadow Word: Pain'),
(8217,0,2,0,0,0,100,0,1000,5000,21000,27000,11,3443,1,0,0,0,0,1,0,0,0,0,0,0,0,'Mith\'rethis the Enchanter - In Combat - Cast Enchanted Quickness');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2646;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2646 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2646,0,0,0,9,0,100,0,0,5,7000,15000,11,11015,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Blood Drinker - Within 0-5 Range - Cast Blood Leech'),
(2646,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Blood Drinker - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2647;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2647 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2647,0,0,0,9,0,100,0,0,5,8000,12000,11,11016,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Soul Eater - Within 0-5 Range - Cast Soul Bite'),
(2647,0,1,0,14,0,100,0,600,10,12000,15000,11,7154,1,0,0,0,0,7,0,0,0,0,0,0,0,'Vilebranch Soul Eater - On Friendly at 600 Health - Cast Dark Offering'),
(2647,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Soul Eater - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2648;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2648 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2648,0,0,0,0,0,100,0,4000,7000,8000,11000,11,8242,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Aman\'zasi Guard - In Combat - Cast Shield Slam'),
(2648,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Aman\'zasi Guard - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2681;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2681 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2681,0,0,0,0,0,100,0,8000,12000,15000,20000,11,3391,2,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Raiding Wolf - In Combat - Cast Thrash');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=7995;
DELETE FROM `smart_scripts` WHERE `entryorguid`=7995 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7995,0,0,0,9,0,100,0,0,30,9000,12000,11,11639,0,0,0,0,0,2,0,0,0,0,0,0,0,'Vile Priestess Hexx - Within 0-30 Range - Cast Shadow Word: Pain'),
(7995,0,1,0,0,0,100,0,7000,12000,18000,25000,11,11641,1,0,0,0,0,2,0,0,0,0,0,0,0,'Vile Priestess Hexx - In Combat - Cast Hex'),
(7995,0,2,0,14,0,100,0,1200,40,12000,15000,11,11642,0,0,0,0,0,7,0,0,0,0,0,0,0,'Vile Priestess Hexx - Friendly At 1200 Health - Cast Heal'),
(7995,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vile Priestess Hexx - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=10802;
DELETE FROM `smart_scripts` WHERE `entryorguid`=10802 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10802,0,0,0,9,0,100,0,0,30,21000,25000,11,11639,0,0,0,0,0,2,0,0,0,0,0,0,0,'Hitah\'ya the Keeper - Within 0-30 Range - Cast Shadow Word: Pain'),
(10802,0,1,0,0,0,100,0,0,0,3400,4800,11,9613,0,0,0,0,0,2,0,0,0,0,0,0,0,'Hitah\'ya the Keeper - In Combat - Cast Shadow Bolt'),
(10802,0,2,0,14,0,100,0,1400,40,12000,18000,11,11640,0,0,0,0,0,7,0,0,0,0,0,0,0,'Hitah\'ya the Keeper - Friendly At 1400 Health - Cast Renew'),
(10802,0,3,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Hitah\'ya the Keeper - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=14748;
DELETE FROM `smart_scripts` WHERE `entryorguid`=14748 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14748,0,0,0,12,0,100,1,0,20,0,0,11,7160,1,0,0,0,0,2,0,0,0,0,0,0,0,'Vilebranch Kidnapper - Target Between 0-20% Health - Cast Execute (No Repeat)'),
(14748,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Kidnapper - Between 0-15% Health - Flee For Assist (No Repeat)'),
(14748,0,2,0,4,0,100,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Vilebranch Kidnapper - On Aggro - Say Line 0');

DELETE FROM `creature_text` WHERE `CreatureID`=14748;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(14748,0,0,'Another fool falls for the trap!',12,0,100,0,0,0,10003,0,'Vilebranch Kidnapper'),
(14748,0,1,'Dinner has arrived!',12,0,100,0,0,0,10004,0,'Vilebranch Kidnapper'),
(14748,0,2,'The High Priestess will feast on your bones!',12,0,100,0,0,0,10005,0,'Vilebranch Kidnapper');

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `creature_template` SET `mechanic_immune_mask`=`mechanic_immune_mask`|2|64|4096|8192|131072|524288 WHERE `entry` IN 
(34607, 34648, 35655, 35656);

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

-- https://www.wowhead.com/npc=34607/nerubian-burrower
UPDATE `creature_template` SET `minlevel`=82, `maxlevel`=82 WHERE `entry` IN 
(34607, 34648, 35655, 35656);

-- https://www.wowhead.com/npc=34605/swarm-scarab  (maxlevel is already 80)
UPDATE `creature_template` SET `minlevel`=80 WHERE `entry` IN 
(34605, 34650, 35658, 35659);

/*
 * Zone: Azeroth (Skipped Part II)
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4011;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4011 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4011,0,0,0,0,0,100,0,5000,8000,12000,18000,11,744,32,0,0,0,0,2,0,0,0,0,0,0,0,'Young Pridewing - In Combat - Cast Poison');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4263;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4263 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4263,0,0,0,2,0,50,1,0,30,0,0,11,6536,0,0,0,0,0,1,0,0,0,0,0,0,0,'Deepmoss Hatchling - At 30% HP - Summon Deepmoss Matriarch');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4264;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4264);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4264, 0, 0, 0, 7, 0, 100, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deepmoss Matriarch - On Evade - Despawn In 5000 ms');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2172;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2172 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2172,0,0,0,0,0,100,0,7000,11000,12000,14000,11,7272,0,0,0,0,0,1,0,0,0,0,0,0,0,'Strider Clutchmother - In Combat - Cast Dust Cloud'),
(2172,0,1,0,9,0,100,0,0,5,15000,17000,11,6607,0,0,0,0,0,2,0,0,0,0,0,0,0,'Strider Clutchmother - Within 0-5 Range - Cast Lash'),
(2172,0,2,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Strider Clutchmother - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2170;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2170 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2170,0,0,0,14,0,100,0,200,40,18000,21000,11,1058,0,0,0,0,0,7,0,0,0,0,0,0,0,'Blackwood Ursa - Friendly At 200 Health - Cast Rejuvenation');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2171;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2171 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2171,0,0,0,0,0,100,0,1000,2000,3000,3400,11,9532,64,0,0,0,0,2,0,0,0,0,0,0,0,'Blackwood Shaman - In Combat CMC - Cast Lightning Bolt'),
(2171,0,1,0,0,0,100,0,9000,14000,11000,15000,11,2606,0,0,0,0,0,2,0,0,0,0,0,0,0,'Blackwood Shaman - In Combat - Cast Shock'),
(2171,0,2,0,14,0,100,0,300,40,16000,21000,11,913,0,0,0,0,0,7,0,0,0,0,0,0,0,'Blackwood Shaman - Friendly At 300 Health - Cast Healing Wave');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2186;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2186 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2186,0,0,0,25,0,100,0,0,0,0,0,75,8876,0,0,0,0,0,1,0,0,0,0,0,0,0,'Carnivous the Breaker - On Respawn - Cast Thrash (No Repeat)'),
(2186,0,1,0,9,0,100,0,0,0,15000,20000,11,6016,0,0,0,0,0,2,0,0,0,0,0,0,0,'Carnivous the Breaker - Within 0-5 Range - Cast Pierce Armor');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2183;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2183 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2183,0,0,0,0,0,100,0,4000,8000,10000,12000,11,3248,0,0,0,0,0,1,0,0,0,0,0,0,0,'Stormscale Warrior - In Combat - Cast Improved Blocking');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=7016;
DELETE FROM `smart_scripts` WHERE `entryorguid`=7016 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7016,0,0,0,0,0,100,0,1500,3000,3000,3100,11,6660,64,0,0,0,0,2,0,0,0,0,0,0,0,'Lady Vespira - In Combat - Cast Shoot'),
(7016,0,1,0,9,0,100,0,0,5,9000,14000,11,11428,1,0,0,0,0,2,0,0,0,0,0,0,0,'Lady Vespira - Within 0-5 Range - Cast Knockdown'),
(7016,0,2,0,0,0,100,0,11000,15000,14000,17000,11,12549,1,0,0,0,0,2,0,0,0,0,0,0,0,'Lady Vespira - In Combat - Cast Forked Lightning');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=2175;
DELETE FROM `smart_scripts` WHERE `entryorguid`=2175 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2175,0,0,0,9,0,100,0,0,30,12000,15000,11,17227,0,0,0,0,0,2,0,0,0,0,0,0,0,'Shadowclaw - Within 0-30 Range - Cast Curse of Weakness');

DELETE FROM `creature_text` WHERE `CreatureID`=11915;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(11915,0,0,'You no take candle!',12,0,100,0,0,0,16658,0,'Gogger Rock Keeper'),
(11915,0,1,'Yiieeeee! Me run!',12,0,100,0,0,0,1864,0,'Gogger Rock Keeper'),
(11915,0,2,'No kill me! No kill me!',12,0,100,0,0,0,1863,0,'Gogger Rock Keeper');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4001;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4001 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4001,0,0,0,0,0,100,0,4000,11000,7000,15000,11,8139,32,0,0,0,0,2,0,0,0,0,0,0,0,'Windshear Tunnel Rat - In Combat - Cast Fevered Fatigue');

DELETE FROM `creature_text` WHERE `CreatureID`=11917;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(11917,0,0,'You NO take candle!',12,0,100,0,0,0,16658,0,'Gogger Geomancer'),
(11917,0,1,'Yiieeeee! Me run!',12,0,100,0,0,0,1864,0,'Gogger Geomancer'),
(11917,0,2,'No kill me! No kill me!',12,0,100,0,0,0,1863,0,'Gogger Geomancer');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=4002;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4002 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4002,0,0,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Windshear Stonecutter - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIname`='SmartAI' WHERE `entry`=11912;
DELETE FROM `smart_scripts` WHERE `entryorguid`=11912 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11912,0,0,0,4,0,100,0,0,0,0,0,11,6268,0,0,0,0,0,2,0,0,0,0,0,0,0,'Grimtotem Brute - On Aggro - Cast RushIng Charge'),
(11912,0,1,0,2,0,100,1,0,15,0,0,25,1,0,0,0,0,0,1,0,0,0,0,0,0,0,'Grimtotem Brute - Between 0-15% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=17236;
DELETE FROM `smart_scripts` WHERE `entryorguid`=17236 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17236,0,0,0,6,0,100,0,0,0,0,0,11,11018,0,0,0,0,0,7,0,0,0,0,0,0,0,'Tcha\'kaz - On Death - Summon Witherbark Bloodlings');

DELETE FROM `creature_text` WHERE `CreatureID`=11918;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES
(11918,0,0,'You NO take candle!',12,0,100,0,0,0,16658,0,'Gogger Stonepounder'),
(11918,0,1,'Yiieeeee! Me run!',12,0,100,0,0,0,1864,0,'Gogger Stonepounder'),
(11918,0,2,'No kill me! No kill me!',12,0,100,0,0,0,1863,0,'Gogger Stonepounder');

UPDATE `creature_template` SET `AIname`='SmartAI' WHERE `entry`=4021;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4021 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4021,0,0,0,9,0,100,0,0,30,5000,11000,11,3396,32,0,0,0,0,2,0,0,0,0,0,0,0,'Corrosive Sap Beast - Within 0-30 Range - Cast Corrosive Poison');

UPDATE `creature_template` SET `AIname`='SmartAI' WHERE `entry`=4030;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4030 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4030,0,0,0,9,0,100,0,0,5,17000,25000,11,6909,32,0,0,0,0,2,0,0,0,0,0,0,0,'Vengeful Ancient - Within 0-5 Range - Cast Curse of Thorns');

UPDATE `creature_template` SET `AIname`='SmartAI' WHERE `entry`=4016;
DELETE FROM `smart_scripts` WHERE `entryorguid`=4016 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4016,0,0,0,9,0,100,1,0,30,0,0,11,7994,0,0,0,0,0,2,0,0,0,0,0,0,0,'Fey Dragon - Within 0-30 Range - Cast Nullify Mana (No Repeat)');

DELETE FROM `creature` WHERE `guid` IN (36898, 36904, 36907, 36924, 36938, 36942, 36943, 36944, 36957, 36956, 36958);
DELETE FROM `creature_addon` WHERE `guid` IN (36898, 36904, 36907, 36924, 36938, 36942, 36943, 36944, 36957, 36956, 36958);

UPDATE `creature` SET `position_x`=6594.56, `position_y`=313.645, `position_z`=28.3839 WHERE `guid`=36643;
UPDATE `creature` SET `position_x`=4424.85, `position_y`=422.488, `position_z`=56.4554 WHERE `guid`=36794;
UPDATE `creature` SET `position_x`=7742.69, `position_y`=-1067.14, `position_z`=38.1206 WHERE `guid`=36959;

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7768;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7768);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7768, 0, 0, 0, 7, 0, 100, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Witherbark Bloodling - On Evade - Despawn In 5000 ms');

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
 */

-- source: http://wotlk.cavernoftime.com/npc=11443
UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `AIName`='SmartAI' WHERE  `entry`=11443;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 11443);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(11443, 0, 0, 0, 0, 0, 100, 0, 5000, 12000, 35000, 45000, 0, 75, 6742, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Gordok Ogre-Mage - In Combat - Cast 6742'),
(11443, 0, 1, 0, 0, 0, 100, 0, 4000, 7500, 12500, 17000, 0, 11, 10179, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Gordok Ogre-Mage - In Combat - Cast 10179'),
(11443, 0, 2, 0, 0, 0, 100, 0, 11000, 18000, 25000, 33000, 0, 11, 10149, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Gordok Ogre-Mage - In Combat - Cast 10149');

/*
 * Zone: Defender of Timbermaw
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
 */

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 15720;
UPDATE `creature_template` SET `unit_class` = 8 WHERE `entry` = 15720;

DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 15720);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15720, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 11, 25357, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Timbermaw Ancestor - On Respawn - Cast \'25357\''),
(15720, 0, 1, 0, 0, 0, 100, 0, 2100, 2700, 8600, 9300, 11, 20295, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Timbermaw Ancestor - In Combat - Cast \'20295\''),
(15720, 0, 2, 0, 12, 0, 100, 1, 5, 50, 0, 0, 11, 25357, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 'Timbermaw Ancestor - Target Between 5-50% Health - Cast \'25357\' (No Repeat)');

/*
 * Zone: Tirisfal Glades
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1529;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1529);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1529, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 9400, 11600, 11, 3322, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bleeding Horror - In Combat - Cast \'3322\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1532;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1532);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1532, 0, 0, 0, 0, 0, 100, 0, 2100, 3200, 9100, 10200, 11, 7713, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wandering Spirit - In Combat - Cast \'7713\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1548;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1548);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1548, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cursed Darkhound - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1664;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1664);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1664, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Vachon - On Aggro - Cast \'7164\''),
(1664, 0, 1, 0, 0, 0, 100, 0, 2100, 3700, 9600, 10500, 11, 72, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Vachon - In Combat - Cast \'72\''),
(1664, 0, 2, 0, 2, 0, 100, 1, 10, 30, 0, 0, 11, 3248, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Vachon - Between 10-30% Health - Cast \'3248\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1549;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1549);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1549, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ravenous Darkhound - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4284;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4284);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4284, 0, 0, 0, 0, 0, 100, 0, 2100, 3200, 8700, 9600, 11, 9613, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Augur - In Combat - Cast \'9613\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4280;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4280);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4280, 0, 0, 0, 0, 0, 100, 0, 2100, 3300, 8200, 9300, 11, 13953, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Preserver - In Combat - Cast \'13953\''),
(4280, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 13952, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Preserver - Between 5-30% Health - Cast \'13952\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4282;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4282);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4282, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 8457, 21, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Magician - On Aggro - Cast \'8457\''),
(4282, 0, 1, 0, 0, 0, 100, 0, 2100, 3300, 8600, 9200, 11, 9053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Magician - In Combat - Cast \'9053\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1527;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1527);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1527, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3234, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Hungering Dead - Between 20-80% Health - Cast \'3234\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1656;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1656);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1656, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 7900, 9600, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Thurman Agamand - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4281;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4281);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4281, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 6979, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Scout - On Aggro - Cast \'6979\''),
(4281, 0, 1, 0, 0, 0, 100, 0, 1700, 2300, 6800, 7900, 11, 6660, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Scout - In Combat - Cast \'6660\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1525;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1525);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1525, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3234, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rooting Dead - Between 20-80% Health - Cast \'3234\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1665;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1665);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1665, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 8258, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Melrache - On Aggro - Cast \'8258\''),
(1665, 0, 1, 0, 0, 0, 100, 0, 2100, 2900, 8700, 9200, 11, 11976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Melrache - In Combat - Cast \'11976\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1660;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1660);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1660, 0, 0, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 12169, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Bodyguard - Between 5-30% Health - Cast \'12169\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1547;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1547);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1547, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Decrepit Darkhound - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1655;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1655);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1655, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 7900, 9600, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nissa Agamand - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1538;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1538);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1538, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 11, 1243, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Friar - On Respawn - Cast \'1243\''),
(1538, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 2052, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Friar - Between 30-60% Health - Cast \'2052\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1540;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1540);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1540, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 7164, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Vanguard - On Aggro - Cast \'7164\''),
(1540, 0, 1, 0, 0, 0, 100, 0, 2100, 2900, 9800, 9900, 11, 72, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Vanguard - In Combat - Cast \'72\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1526;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1526);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1526, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3234, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ravaged Corpse - Between 20-80% Health - Cast \'3234\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1941;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1941);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1941, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 3237, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rot Hide Graverobber - Between 30-60% Health - Cast \'3237\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1528;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1528);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1528, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3234, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shambling Horror - Between 20-80% Health - Cast \'3234\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1753;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1753);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1753, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 3237, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Maggot Eye - Between 30-60% Health - Cast \'3237\' (No Repeat)'),
(1753, 0, 1, 0, 2, 0, 100, 1, 5, 20, 0, 0, 11, 3243, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Maggot Eye - Between 5-20% Health - Cast \'3243\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1675;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1675);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1675, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 3237, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rot Hide Mongrel - Between 30-60% Health - Cast \'3237\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1658;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1658);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1658, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 7900, 9600, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Dargol - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1530;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1530);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1530, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 9800, 12100, 11, 3322, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotting Ancestor - In Combat - Cast \'3322\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1934;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1934);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1934, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tirisfal Farmer - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1654;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1654);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1654, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 7900, 9600, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gregor Agamand - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1533;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1533);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1533, 0, 0, 0, 0, 0, 100, 0, 2100, 2400, 9200, 9600, 11, 7713, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tormented Spirit - In Combat - Cast \'7713\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1523;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1523);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1523, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 589, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cracked Skull Soldier - On Aggro - Cast \'589\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1520;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1520);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1520, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rattlecage Soldier - Between 20-80% Health - Cast \'3391\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1545;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1545);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1545, 0, 0, 0, 0, 0, 100, 0, 2500, 4000, 8000, 10000, 11, 7159, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Vile Fin Muckdweller - In Combat - Cast \'7159\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1543;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1543);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1543, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Vile Fin Puddlejumper - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1935;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1935);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1935, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tirisfal Farmhand - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1662;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1662);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1662, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Perrine - Between 20-80% Health - Cast \'3391\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1536;
UPDATE `creature_template` SET `unit_class`= 2 WHERE `entry`= 1536;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1536);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1536, 0, 0, 0, 0, 0, 100, 0, 1700, 2100, 8400, 8800, 11, 9053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Missionary - In Combat - Cast \'9053\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1537;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1537);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1537, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Zealot - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `quest_template_addon` SET `NextQuestID` = 0, `ExclusiveGroup` = 11995 WHERE `id` IN (11995, 12440);
UPDATE `quest_template_addon` SET `NextQuestID` = 12440 WHERE `id` = 12439;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `ConditionTypeOrReference` = 9 AND `SourceEntry` IN (11995, 12439, 12440);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(19, 0, 12439, 0, 0, 9, 0, 12000, 0, 0, 1, 0, 0, '', 'Quest 12439 is not available if 12000 has been taken'),
(19, 0, 11995, 0, 0, 9, 0, 12000, 0, 0, 1, 0, 0, '', 'Quest 11995 is not available if 12000 has been taken'),
(19, 0, 12440, 0, 0, 9, 0, 12000, 0, 0, 1, 0, 0, '', 'Quest 12440 is not available if 12000 has been taken');

DELETE FROM `event_scripts` WHERE `id`=14592;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
(14592, 1, 10, 22890, 300000, 0, -108.252, -510.302, 21.4761, 2.44346);

UPDATE `quest_template_addon` SET `SpecialFlags`=0  WHERE `id`=708;

UPDATE `creature_text` SET `text` = 'Nice. I don\'t want to know what that stain was.' WHERE `CreatureID` = 36856 AND `groupid` = 1 AND `id` = 3;

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `creature_template` SET `AIName`='PassiveAI' WHERE `entry`=32866;
UPDATE `creature_template` SET `unit_flags`=`unit_flags`|33554432 WHERE  `entry` IN
(32866, 33690);

UPDATE `creature` SET `id` = 32315, `modelid` = 0 WHERE `guid` = 200984;
UPDATE `creature_template` SET `modelid1` = 30583, `modelid2` = 27905 WHERE `entry` = 32315;

-- Ulduar Colossus bridge pathing fix --
DELETE FROM `waypoint_data` WHERE `id` = 1374780;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`)  VALUES 
(1374780, 1, 71.3266, -170.786, 410.72),
(1374780, 2, 71.3266, -212.300, 418.99),
(1374780, 3, 71.3266, -232.054, 418.99),
(1374780, 4, 71.3266, -263.742, 413.42),
(1374780, 5, 71.3266, -232.054, 418.99),
(1374780, 6, 71.3266, -212.300, 418.99);

UPDATE `creature` SET `position_x`= -2708.886719, `position_y`= 1149.866333, `position_Z` = 3.801008 WHERE `guid` = 86811;

/*
 * Zone: Burning Steppes
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7029;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7029);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7029, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Battlemaster - On Aggro - Cast \'7165\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7055;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7055);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7055, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Worg - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7032;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7032);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7032, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Greater Obsidian Elemental - Between 20-80% Health - Cast \'13496\' (No Repeat)');

/*
 * Zone: Loch Modan
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1225;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1225);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1225, 0, 0, 0, 0, 0, 100, 0, 2200, 3600, 8400, 9700, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ol\' Sooty - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2477;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2477);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2477, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gradok - On Aggro - Cast \'7165\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1186;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1186);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1186, 0, 0, 0, 0, 0, 100, 0, 2700, 5100, 9900, 12600, 11, 15793, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Elder Black Bear - In Combat - Cast \'15793\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1193;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1193);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1193, 0, 0, 0, 2, 0, 100, 1, 5, 10, 0, 0, 11, 3019, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Loch Frenzy - Between 5-10% Health - Cast \'3019\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1189;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1189);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1189, 0, 0, 0, 0, 0, 100, 0, 2700, 5100, 9900, 12600, 11, 15793, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Black Bear Patriarch - In Combat - Cast \'15793\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2478;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2478);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2478, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Haren Swifthoof - On Aggro - Cast \'7164\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1693;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1693);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1693, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Loch Crocolisk - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14266;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14266);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14266, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 12023, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shanda the Spinner - On Aggro - Cast \'12023\' (No Repeat)'),
(14266, 0, 1, 0, 0, 0, 100, 0, 2700, 3400, 32700, 33400, 11, 13298, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shanda the Spinner - In Combat - Cast \'13298\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14268;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14268);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14268, 0, 0, 0, 0, 0, 100, 0, 4700, 6300, 11700, 14200, 11, 4150, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Condar - In Combat - Cast \'4150\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1188;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1188);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1188, 0, 0, 0, 0, 0, 100, 0, 2700, 5100, 9900, 12600, 11, 15793, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grizzled Black Bear - In Combat - Cast \'15793\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1224;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1224);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1224, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Threshadon - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7170;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7170);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7170, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thragomm - On Aggro - Cast \'7165\' (No Repeat)');

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `creature_template` SET `unit_flags`=`unit_flags`|33554432 WHERE  `entry` IN
(30641, 31521);
UPDATE `creature_template` SET `ScriptName` = 'trigger_periodic' WHERE `entry`= 30641;

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `spelldifficulty_dbc` SET `DifficultySpellID_1`=55543, `DifficultySpellID_2`=29107 WHERE `ID`=29107;

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `creature_template` SET `minlevel`=80, `maxlevel`=80 WHERE `entry`=16697;

/*
 * Zone: Swamp of Sorrows II
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 922;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 922);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(922, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Silt Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1082;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1082);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1082, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 50433, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sawtooth Crocolisk - On Aggro - Cast \'50433\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 757;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 757);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(757, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lost One Fisherman - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14447;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14447);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14447, 0, 0, 0, 0, 0, 100, 0, 2700, 4800, 9600, 12700, 11, 13579, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gilmorian - In Combat - Cast \'13579\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 9916;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 9916);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9916, 0, 0, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jarquia - Between 5-30% Health - Cast \'8599\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 750;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 750);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(750, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 11700, 13400, 11, 9612, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Marsh Inkspewer - In Combat - Cast \'9612\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 752;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 752);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(752, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 12550, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marsh Oracle - On Aggro - Cast \'12550\' (No Repeat)'),
(752, 0, 1, 0, 0, 0, 100, 0, 2300, 2900, 8700, 9600, 11, 9532, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Marsh Oracle - In Combat - Cast \'9532\''),
(752, 0, 2, 0, 2, 0, 100, 1, 20, 40, 0, 0, 11, 11986, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marsh Oracle - Between 20-40% Health - Cast \'11986\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5477;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5477);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5477, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Noboru the Cudgel - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1088;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1088);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1088, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Monstrous Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 768;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 768);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(768, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadow Panther - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1087;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1087);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1087, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 3604, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sawtooth Snapper - Between 30-60% Health - Cast \'3604\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5622;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5622);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5622, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ongeku - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 763;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 763);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(763, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost One Chieftain - On Aggro - Cast \'7165\' (No Repeat)'),
(763, 0, 1, 0, 0, 0, 100, 0, 2700, 3400, 17700, 18400, 11, 11977, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lost One Chieftain - In Combat - Cast \'11977\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 761;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 761);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(761, 0, 0, 0, 2, 0, 100, 1, 50, 90, 0, 0, 11, 8376, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lost One Seer - Between 50-90% Health - Cast \'8376\' (No Repeat)'),
(761, 0, 1, 0, 2, 0, 100, 1, 5, 45, 0, 0, 11, 4971, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost One Seer - Between 5-45% Health - Cast \'4971\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 17115;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 17115);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17115, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 14868, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cursed Lost One - On Aggro - Cast \'14868\' (No Repeat)'),
(17115, 0, 1, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 6713, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cursed Lost One - Between 20-80% Health - Cast \'6713\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 862;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 862);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(862, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stonard Explorer - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14448;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14448);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14448, 0, 0, 0, 0, 0, 100, 0, 3800, 4600, 9800, 13400, 11, 21748, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Molt Thorn - In Combat - Cast \'21748\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1084;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1084);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1084, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 50433, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Young Sawtooth Crocolisk - On Aggro - Cast \'50433\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 858;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 858);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(858, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 745, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sorrow Spinner - On Aggro - Cast \'745\' (No Repeat)'),
(858, 0, 1, 0, 0, 0, 100, 0, 2700, 3100, 32700, 33100, 11, 744, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sorrow Spinner - In Combat - Cast \'744\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 767;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 767);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(767, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Swamp Jaguar - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 764;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 764);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(764, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 9616, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swampwalker - Between 30-60% Health - Cast \'9616\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1081;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1081);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1081, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 8138, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mire Lord - Between 20-80% Health - Cast \'8138\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 765;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 765);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(765, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 9616, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Swampwalker Elder - Between 30-60% Health - Cast \'9616\' (No Repeat)');

/*
 * Zone: Redrige Mountains
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14273;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14273);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14273, 0, 0, 0, 0, 0, 100, 0, 3100, 4700, 9600, 12700, 11, 9483, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Boulderheart - In Combat - Cast \'9483\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14272;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14272);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14272, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 9600, 11200, 11, 8873, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Snarlflare - In Combat - Cast \'8873\'');

UPDATE `creature` SET `spawnMask` = 3 WHERE `id` = 18956;
UPDATE `gameobject` SET `spawnMask` = 3 WHERE `id` = 183051;

UPDATE `creature`SET `position_x` = 1656.397705, `position_y` = -4434.079102, `position_z` = 17.744991 WHERE `guid` = 86234;

/*
 * Object: Dire Maul - Gordok Tribute
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

DELETE FROM `gameobject_loot_template` WHERE (`Entry` = 16577);
INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(16577, 1, 12007, 100, 0, 2, 0, 1, 1, NULL),
(16577, 2, 12007, 100, 0, 4, 0, 1, 1, NULL),
(16577, 3, 12007, 100, 0, 8, 0, 1, 1, NULL),
(16577, 4, 12007, 100, 0, 16, 0, 1, 1, NULL),
(16577, 5, 12007, 100, 0, 32, 0, 1, 1, NULL),
(16577, 8766, 0, 100, 0, 1, 0, 15, 20, NULL),
(16577, 8952, 0, 100, 0, 1, 0, 15, 20, NULL),
(16577, 13444, 0, 9.9, 0, 1, 1, 2, 5, NULL),
(16577, 13446, 0, 8.2, 0, 1, 1, 2, 5, NULL),
(16577, 18495, 0, 8.1, 0, 1, 1, 1, 1, NULL),
(16577, 18499, 0, 9.8, 0, 1, 1, 1, 1, NULL),
(16577, 18500, 0, 8.9, 0, 1, 1, 1, 1, NULL),
(16577, 18528, 0, 8.9, 0, 1, 1, 1, 1, NULL),
(16577, 18529, 0, 4.6, 0, 1, 1, 1, 1, NULL),
(16577, 18530, 0, 6.9, 0, 1, 1, 1, 1, NULL),
(16577, 18531, 0, 7.2, 0, 1, 1, 1, 1, NULL),
(16577, 18532, 0, 7.5, 0, 1, 1, 1, 1, NULL),
(16577, 18533, 0, 6.1, 0, 1, 1, 1, 1, NULL),
(16577, 18534, 0, 6.3, 0, 1, 1, 1, 1, NULL),
(16577, 18537, 0, 4.1, 0, 1, 1, 1, 1, NULL),
(16577, 18655, 0, 3.5, 0, 1, 1, 1, 1, NULL);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|0x00000080 WHERE `entry` IN (24202, 24203, 24204, 24205);

/*
 * Zone: Duskwood
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 898;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 898);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(898, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 58867, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Nightbane Worgen - On Aggro - Cast \'58867\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 213;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 213);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(213, 0, 0, 0, 2, 0, 100, 1, 1, 5, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Starving Dire Wolf - Between 1-5% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 3;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Flash Eater - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1871;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1871);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1871, 0, 0, 0, 0, 0, 100, 0, 5700, 6900, 14500, 17800, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Eliza\'s Guard - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 521;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 521);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(521, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lupos - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 923;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 923);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(923, 0, 0, 0, 2, 0, 100, 1, 1, 5, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Young Black Ravager - Between 1-5% Health - Cast \'3149\' (No Repeat)');

UPDATE `gameobject` SET `spawnMask` = 3 WHERE `id` = 182947;

UPDATE `quest_poi` SET `MapID` = 0, `WorldMapAreaId` = 46 WHERE `questid` = 8925 AND `id` = 0;

/*
 * Zone: Westfall
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7053;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7053);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7053, 0, 0, 0, 0, 0, 100, 0, 3100, 4700, 9600, 12500, 11, 14873, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Klaven Mortwake - In Combat - Cast \'14873\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7056;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7056);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7056, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 11, 22766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Defias Tower Sentry - Out of Combat - Cast \'22766\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6250;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6250);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6250, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7052;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7052);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7052, 0, 0, 0, 0, 0, 100, 0, 3100, 4700, 8600, 10100, 11, 5679, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Defias Tower Patroller - In Combat - Cast \'5679\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 114;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 114);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(114, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Harvest Watcher - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7051;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7051);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7051, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Malformed Defias Drone - On Aggro - Cast \'7165\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 831;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 831);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(831, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sea Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7067;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7067);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7067, 0, 0, 0, 0, 0, 100, 0, 3800, 4900, 9800, 13800, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Venture Co. Drone - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 830;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 830);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(830, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sand Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 36;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 36);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Harvest Golem - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 596;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 596);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(596, 0, 0, 0, 0, 0, 100, 0, 2300, 4100, 7400, 9800, 11, 20883, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Brainwashed Noble - In Combat - Cast \'20883\''),
(596, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 6117, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Brainwashed Noble - Between 30-60% Health - Cast \'6117\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1216;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1216);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1216, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shore Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 199;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 199);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(199, 0, 0, 0, 2, 0, 100, 1, 1, 5, 0, 0, 11, 31273, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Fleshripper - Between 1-5% Health - Cast \'31273\' (No Repeat)');

/*
 * Zone: Blasted Lands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7670;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7670);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7670, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Servant of Allistarj - On Aggro - Cast \'7164\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7669;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7669);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7669, 0, 0, 0, 0, 0, 100, 0, 3600, 4200, 9800, 12700, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Servant of Grol - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6011;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6011);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6011, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 22120, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Felguard Sentry - On Aggro - Cast \'22120\' (No Repeat)'),
(6011, 0, 1, 0, 0, 0, 100, 0, 2700, 4100, 9800, 12900, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Felguard Sentry - In Combat - Cast \'15496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7668;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7668);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7668, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7165, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Servant of Razelikh - On Aggro - Cast \'7165\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5977;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5977);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5977, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 11960, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Mauler - On Aggro - Cast \'11960\' (No Repeat)'),
(5977, 0, 1, 0, 0, 0, 100, 0, 2700, 4300, 9800, 12400, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Mauler - In Combat - Cast \'3391\''),
(5977, 0, 2, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Mauler - Between 5-30% Health - Cast \'8599\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7671;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7671);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7671, 0, 0, 0, 0, 0, 100, 0, 3600, 4200, 9800, 12700, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Servant of Sevine - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5984;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5984);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5984, 0, 0, 0, 0, 0, 100, 0, 2700, 4300, 32700, 34300, 11, 7367, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Starving Snickerfang - In Combat - Cast \'7367\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5974;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5974);
UPDATE `creature_template` SET `unit_class`= 2 WHERE `entry`= 5974;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5974, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 11960, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Ogre - On Aggro - Cast \'11960\' (No Repeat)'),
(5974, 0, 1, 0, 0, 0, 100, 0, 2700, 3200, 8700, 11200, 11, 20793, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Ogre - In Combat - Cast \'20793\''),
(5974, 0, 2, 0, 2, 0, 100, 1, 10, 40, 0, 0, 11, 6742, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Ogre - Between 10-40% Health - Cast \'6742\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 751;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 751);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(751, 0, 0, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marsh Flesheater - Between 5-30% Health - Cast \'8599\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5976;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5976);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5976, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 11960, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Brute - On Aggro - Cast \'11960\' (No Repeat)'),
(5976, 0, 1, 0, 0, 0, 100, 0, 2700, 4300, 9800, 12400, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Brute - In Combat - Cast \'3391\''),
(5976, 0, 2, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreadmaul Brute - Between 5-30% Health - Cast \'8599\' (No Repeat)');

/*
 * Zone: Elwynn Forest
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 285;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 285);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(285, 0, 0, 0, 2, 0, 100, 1, 1, 5, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murloc - Between 1-5% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 13159;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 13159);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(13159, 0, 0, 0, 0, 0, 100, 0, 4800, 6900, 11700, 14500, 11, 3391, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'James Clark - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 735;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 735);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(735, 0, 0, 0, 2, 0, 100, 1, 1, 5, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murloc Streamrunner - Between 1-5% Health - Flee For Assist (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 525;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 525);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(525, 0, 0, 0, 2, 0, 100, 1, 1, 3, 0, 0, 11, 3604, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mangy Wolf - Between 1-3% Health - Cast \'3604\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6093;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6093);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6093, 0, 0, 0, 0, 0, 100, 0, 4800, 6900, 11700, 14500, 11, 3391, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dead-Tooth Jack - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 118;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 118);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(118, 0, 0, 0, 2, 0, 100, 1, 1, 3, 0, 0, 11, 3604, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Prowler - Between 1-3% Health - Cast \'3604\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 880;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 880);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(880, 0, 0, 0, 0, 0, 100, 0, 4800, 6900, 11700, 14500, 11, 3391, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Erlan Drudgemoor - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 822;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 822);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(822, 0, 0, 0, 2, 0, 100, 0, 1, 3, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Forest Bear - Between 1-3% Health - Cast \'13496\'');

/*
 * Zone: Stranglethorn Vale II
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2541;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2541);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2541, 0, 0, 0, 0, 0, 100, 0, 3700, 4200, 9700, 12200, 11, 15496, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Sakrasis - In Combat - Cast \'15496\''),
(2541, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 3583, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Sakrasis - Between 30-60% Health - Cast \'3583\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2535;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2535);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2535, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Maury "Club Foot" Wilkins - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2545;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2545);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2545, 0, 0, 0, 0, 0, 100, 0, 2700, 4600, 8700, 9600, 11, 14873, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - In Combat - Cast \'14873\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 687;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 687);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(687, 0, 0, 0, 0, 0, 100, 0, 2400, 3200, 17400, 18200, 11, 11977, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Jungle Stalker - In Combat - Cast \'11977\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2551;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2551);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2551, 0, 0, 0, 0, 0, 100, 0, 3200, 4800, 9200, 11800, 11, 33661, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Brutus - In Combat - Cast \'33661\''),
(2551, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 38784, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Brutus - Between 5-30% Health - Cast \'38784\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4506;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4506);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4506, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 7357, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bloodsail Swabby - Between 20-80% Health - Cast \'7357\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2544;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2544);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2544, 0, 0, 0, 0, 0, 100, 0, 2800, 3400, 8800, 9400, 11, 50245, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Southern Sand Crawler - In Combat - Cast \'50245\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2522;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2522);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2522, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 11, 22766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jaguero Stalker - Out of Combat - Cast \'22766\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 685;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 685);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(685, 0, 0, 0, 0, 0, 100, 0, 2400, 3200, 17400, 18200, 11, 11977, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stranglethorn Raptor - In Combat - Cast \'11977\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14491;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14491);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14491, 0, 0, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kurmokk - Between 5-30% Health - Cast \'8599\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1557;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1557);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1557, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 15615, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Elder Mistvale Gorilla - Between 30-60% Health - Cast \'15615\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 976;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 976);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(976, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kurzen War Tiger - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1713;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1713);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1713, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 11, 22766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Elder Shadowmaw Panther - Out of Combat - Cast \'22766\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2536;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2536);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2536, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Jon-Jon the Crow - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 854;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 854);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(854, 0, 0, 0, 0, 0, 100, 0, 2400, 3200, 17400, 18200, 11, 11977, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Jungle Stalker - In Combat - Cast \'11977\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 676;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 676);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(676, 0, 0, 0, 0, 0, 100, 0, 2700, 4800, 9600, 12100, 11, 11969, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Venture Co. Surveyor - In Combat - Cast \'11969\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 781;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 781);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(781, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 6533, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Headhunter - On Aggro - Cast \'6533\' (No Repeat)'),
(781, 0, 1, 0, 0, 0, 100, 0, 1000, 1500, 6500, 7000, 11, 10277, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Headhunter - In Combat - Cast \'10277\''),
(781, 0, 2, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3148, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Headhunter - Between 20-80% Health - Cast \'3148\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1060;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1060);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1060, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 11, 20798, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mogh the Undying - Out of Combat - Cast \'20798\' (No Repeat)'),
(1060, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 11, 8813, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mogh the Undying - On Respawn - Cast \'8813\''),
(1060, 0, 2, 0, 0, 0, 100, 0, 2700, 3100, 9800, 10200, 11, 12471, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mogh the Undying - In Combat - Cast \'12471\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 682;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 682);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(682, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stranglethorn Tiger - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6366;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6366);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6366, 0, 0, 0, 2, 0, 100, 1, 5, 20, 0, 0, 11, 11642, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kurzen Mindslave - Between 5-20% Health - Cast \'11642\' (No Repeat)'),
(6366, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 7964, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kurzen Mindslave - Between 30-60% Health - Cast \'7964\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 669;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 669);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(669, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 3621, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Hunter - On Aggro - Cast \'3621\' (No Repeat)'),
(669, 0, 1, 0, 0, 0, 100, 0, 2700, 4100, 9600, 12800, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Hunter - In Combat - Cast \'3391\''),
(669, 0, 2, 0, 2, 0, 100, 1, 5, 25, 0, 0, 11, 3148, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Hunter - Between 5-25% Health - Cast \'3148\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 756;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 756);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(756, 0, 0, 0, 1, 0, 100, 1, 2000, 3000, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skullsplitter Panther - Out of Combat - Despawn In 3000 ms (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1114;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1114);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1114, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 15615, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Jungle Thunderer - Between 30-60% Health - Cast \'15615\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1142;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1142);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1142, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mosh\'Ogg Brute - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 691;
UPDATE `creature_template` SET `unit_class`= 2 WHERE `entry`= 691;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 691);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(691, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 38033, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lesser Water Elemental - Between 20-80% Health - Cast \'38033\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 855;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 855);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(855, 0, 0, 0, 0, 0, 100, 0, 2400, 3200, 17400, 18200, 11, 11977, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Stranglethorn Raptor - In Combat - Cast \'11977\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1094;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1094);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1094, 0, 0, 0, 0, 0, 100, 0, 3400, 4800, 23400, 24800, 11, 6016, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Venture Co. Miner - In Combat - Cast \'6016\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 681;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 681);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(681, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Stranglethorn Tiger - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1108;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1108);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1108, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 15615, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mistvale Gorilla - Between 30-60% Health - Cast \'15615\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 905;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 905);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(905, 0, 0, 0, 2, 0, 100, 1, 5, 10, 0, 0, 11, 3019, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sharptooth Frenzy - Between 5-10% Health - Cast \'3019\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4723;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4723);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4723, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 184, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Foreman Cozzle - On Aggro - Cast \'184\' (No Repeat)'),
(4723, 0, 1, 0, 0, 0, 100, 0, 2100, 3400, 9700, 11800, 11, 9532, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foreman Cozzle - In Combat - Cast \'9532\''),
(4723, 0, 2, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 4979, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Foreman Cozzle - Between 5-30% Health - Cast \'4979\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4260;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4260);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4260, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 36203, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Venture Co. Shredder - Between 20-80% Health - Cast \'36203\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 772;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 772);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(772, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 9700, 12700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stranglethorn Tigress - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 684;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 684);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(684, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 11, 22766, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmaw Panther - Out of Combat - Cast \'22766\' (No Repeat)');

/*
 * Zone: Searing Gorge
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2736;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2736);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2736, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Greater Rock Elemental - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 9318;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 9318);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9318, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 9275, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Incendosaur - On Aggro - Cast \'9275\' (No Repeat)'),
(9318, 0, 1, 0, 0, 0, 100, 0, 2700, 4300, 9700, 12300, 11, 11985, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Incendosaur - In Combat - Cast \'11985\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5850;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5850);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5850, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blazing Elemental - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14621;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14621);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14621, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 11639, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Overseer Maltorius - On Aggro - Cast \'11639\' (No Repeat)'),
(14621, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 11974, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Overseer Maltorius - Between 30-60% Health - Cast \'11974\' (No Repeat)'),
(14621, 0, 2, 0, 0, 0, 100, 0, 2700, 4100, 9700, 13100, 11, 9613, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Overseer Maltorius - In Combat - Cast \'9613\''),
(14621, 0, 3, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 13323, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Overseer Maltorius - Between 5-30% Health - Cast \'13323\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8447;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8447);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8447, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Clunk - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8444;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8444);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8444, 0, 0, 0, 0, 0, 100, 0, 3200, 4100, 9600, 10400, 11, 9053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Trade Master Kovic - In Combat - Cast \'9053\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5843;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5843);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5843, 0, 0, 0, 0, 0, 100, 0, 2900, 4700, 9900, 13700, 11, 11971, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slave Worker - In Combat - Cast \'11971\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8283;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8283);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8283, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 6533, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slave Master Blackheart - On Aggro - Cast \'6533\' (No Repeat)'),
(8283, 0, 1, 0, 0, 0, 100, 0, 1000, 1500, 7600, 8200, 11, 6660, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Slave Master Blackheart - In Combat - Cast \'6660\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5833;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5833);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5833, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 15595, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Margol the Rager - On Aggro - Cast \'15595\' (No Repeat)'),
(5833, 0, 1, 0, 0, 0, 100, 0, 2700, 3100, 8700, 10100, 11, 15549, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Margol the Rager - In Combat - Cast \'15549\''),
(5833, 0, 2, 0, 2, 0, 100, 1, 20, 40, 0, 0, 11, 8147, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Margol the Rager - Between 20-40% Health - Cast \'8147\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5856;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5856);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5856, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 745, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Glassweb Spider - On Aggro - Cast \'745\' (No Repeat)'),
(5856, 0, 1, 0, 0, 0, 100, 0, 3100, 3700, 15100, 15700, 11, 6751, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Glassweb Spider - In Combat - Cast \'6751\''),
(5856, 0, 2, 0, 2, 0, 100, 1, 10, 20, 0, 0, 11, 744, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Glassweb Spider - Between 10-20% Health - Cast \'744\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5853;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5853);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5853, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tempered War Golem - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8280;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8280);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8280, 0, 0, 0, 0, 0, 100, 0, 2700, 4200, 9700, 11200, 11, 13321, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shleipnarr - In Combat - Cast \'13321\''),
(8280, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 7399, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shleipnarr - Between 5-30% Health - Cast \'7399\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5857;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5857);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5857, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 4167, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Searing Lava Spider - On Aggro - Cast \'4167\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5854;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5854);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5854, 0, 0, 0, 0, 0, 100, 0, 2900, 3700, 9700, 11900, 11, 5568, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Heavy War Golem - In Combat - Cast \'5568\''),
(5854, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 12612, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Heavy War Golem - Between 5-30% Health - Cast \'12612\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5852;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5852);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5852, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 10733, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Inferno Elemental - Between 20-80% Health - Cast \'10733\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5855;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5855);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5855, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 11970, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Magma Elemental - Between 20-80% Health - Cast \'11970\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5858;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5858);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5858, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 9700, 11400, 11, 11985, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Greater Lava Spider - In Combat - Cast \'11985\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8278;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8278);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8278, 0, 0, 0, 0, 0, 100, 0, 2600, 3100, 17600, 18100, 11, 5213, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Smoldar - In Combat - Cast \'5213\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8279;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8279);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8279, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 9576, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Faulty War Golem - Between 30-60% Health - Cast \'9576\' (No Repeat)');

/*
 * Zone: Badlands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2753;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2753);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2753, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Barnabus - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 7057;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 7057);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7057, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Digmaster Shovelphlange - On Aggro - Cast \'7164\' (No Repeat)'),
(7057, 0, 1, 0, 0, 0, 100, 0, 2700, 3100, 8600, 9400, 11, 8380, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Digmaster Shovelphlange - In Combat - Cast \'8380\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 4872;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 4872);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4872, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Obsidian Golem - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2735;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2735);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2735, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lesser Rock Elemental - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2751;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2751);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2751, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 9576, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'War Golem - Between 30-60% Health - Cast \'9576\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2829;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2829);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2829, 0, 0, 0, 0, 0, 100, 0, 2700, 3200, 32700, 33200, 11, 3427, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Starving Buzzard - In Combat - Cast \'3427\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2727;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2727);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2727, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crag Coyote - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2723;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2723);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2723, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stone Golem - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2850;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2850);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2850, 0, 0, 0, 0, 0, 100, 0, 2800, 3600, 17800, 18600, 11, 36590, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Broken Tooth - In Combat - Cast \'36590\''),
(2850, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 18501, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Broken Tooth - Between 5-30% Health - Cast \'18501\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2749;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2749);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2749, 0, 0, 0, 0, 0, 100, 0, 3400, 4200, 9600, 12800, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Siege Golem - In Combat - Cast \'3391\''),
(2749, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 9576, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Siege Golem - Between 5-30% Health - Cast \'9576\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2732;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2732);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2732, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 8700, 9700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ridge Huntress - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2754;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2754);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2754, 0, 0, 0, 0, 0, 100, 0, 3100, 4700, 9600, 12800, 11, 23392, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Anathemus - In Combat - Cast \'23392\''),
(2754, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 21909, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Anathemus - Between 30-60% Health - Cast \'21909\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2944;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2944);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2944, 0, 0, 0, 0, 0, 100, 0, 2800, 4400, 9200, 9900, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Boss Tho\'grun - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 92;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 92);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(92, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rock Elemental - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2734;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2734);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2734, 0, 0, 0, 0, 0, 100, 0, 2300, 3700, 15200, 17700, 11, 36590, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ridge Stalker Patriarch - In Combat - Cast \'36590\'');

/*
 * Zone: Wetlands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 5683;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 5683);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5683, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 8900, 9600, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Comar Villard - In Combat - Cast \'3391\''),
(5683, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 12540, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Comar Villard - Between 5-30% Health - Cast \'12540\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1400;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1400);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1400, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 50433, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wetlands Crocolisk - On Aggro - Cast \'50433\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1020;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1020);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1020, 0, 0, 0, 0, 0, 100, 0, 2300, 3000, 8400, 9100, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mottled Raptor - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1039;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1039);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1039, 0, 0, 0, 0, 0, 100, 0, 3800, 6900, 13800, 15700, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fen Dweller - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1417;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1417);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1417, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 50433, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Young Wetlands Crocolisk - On Aggro - Cast \'50433\' (No Repeat)');

/*
 * Zone: Dun Morogh
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1131;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1131);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1131, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Winter Wolf - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1199;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1199);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1199, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Juvenile Snow Leopard - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6124;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6124);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6124, 0, 0, 0, 0, 0, 100, 0, 3700, 5200, 10100, 12300, 11, 58461, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Captain Beld - In Combat - Cast \'58461\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1128;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1128);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1128, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Black Bear - Between 1-5% Health - Cast \'13496\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1201;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1201);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1201, 0, 0, 0, 2, 0, 100, 0, 1, 5, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Snow Leopard - Between 1-5% Health - Cast \'13496\'');

/*
 * Zone: Arathi Highlands
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2773;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2773);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2773, 0, 0, 0, 0, 0, 100, 0, 2300, 2900, 8600, 9700, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Or\'Kalar - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2781;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2781);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2781, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Caretaker Weston - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2581;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2581);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2581, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 7164, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dabyrie Militia - On Aggro - Cast \'7164\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2782;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2782);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2782, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Caretaker Alaric - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2559;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2559);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2559, 0, 0, 0, 0, 0, 100, 0, 2300, 3000, 8400, 9100, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Highland Strider - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2780;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2780);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2780, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Caretaker Nevlin - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2582;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2582);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2582, 0, 0, 0, 0, 0, 100, 0, 2700, 3600, 9800, 12400, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dabyrie Laborer - In Combat - Cast \'3391\'');

/*
 * Zone: Alterac Mountains
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2320;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2320);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2320, 0, 0, 0, 0, 0, 100, 0, 2500, 3000, 6500, 7000, 11, 18797, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Nagaz - In Combat - Cast \'18797\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14223;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14223);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14223, 0, 0, 0, 2, 0, 100, 1, 10, 40, 0, 0, 11, 26064, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cranky Benj - Between 10-40% Health - Cast \'26064\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2351;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2351);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2351, 0, 0, 0, 0, 0, 100, 0, 2200, 2600, 7400, 8100, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gray Bear - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2779;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2779);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2779, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 11, 43107, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Prince Nazjak - On Aggro - Cast \'43107\' (No Repeat)'),
(2779, 0, 1, 0, 0, 0, 100, 0, 3800, 5400, 10900, 13100, 11, 17687, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prince Nazjak - In Combat - Cast \'17687\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2578;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2578);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2578, 0, 0, 0, 0, 0, 100, 0, 2300, 3000, 8400, 9100, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Young Mesa Buzzard - In Combat - Cast \'51772\'');

/*
 * Zone: Silverpine Forest
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1971;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1971);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1971, 0, 0, 0, 0, 0, 100, 0, 2700, 3100, 5400, 5800, 11, 7668, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ivar the Foul - In Combat - Cast \'7668\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1869;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1869);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1869, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ravenclaw Champion - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1780;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1780);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1780, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 744, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Moss Stalker - Between 20-80% Health - Cast \'744\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1766;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1766);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1766, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3150, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mottled Worg - Between 20-80% Health - Cast \'3150\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1770;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1770);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1770, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Moonrage Darkrunner - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1769;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1769);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1769, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 12544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonrage Whitescalp - On Aggro - Cast \'12544\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1778;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1778);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1778, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 7700, 8000, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ferocious Grizzled Bear - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1765;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1765);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1765, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Worg - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1972;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1972);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1972, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Grimson the Pale - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1782;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1782);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1782, 0, 0, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Moonrage Darksoul - Between 5-30% Health - Cast \'8599\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1779;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1779);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1779, 0, 0, 0, 0, 0, 100, 0, 3100, 3900, 8700, 10200, 11, 6958, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Moonrage Glutton - In Combat - Cast \'6958\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2332;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2332);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2332, 0, 0, 0, 0, 0, 100, 0, 2400, 2700, 8800, 9400, 11, 14873, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Valdred Moray - In Combat - Cast \'14873\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2053;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2053);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2053, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 22700, 23000, 11, 3261, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Haggard Refugee - In Combat - Cast \'3261\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2054;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2054);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2054, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 5164, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sickly Refugee - Between 20-80% Health - Cast \'5164\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1923;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1923);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1923, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodsnout Worg - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1924;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1924);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1924, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 17700, 18000, 11, 3264, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Moonrage Bloodhowler - In Combat - Cast \'3264\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1797;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1797);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1797, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 7700, 8000, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Giant Grizzled Bear - In Combat - Cast \'31279\'');

/*
 * Zone: Hillsbrad Foothills
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 232;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 232);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(232, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 8600, 9200, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Farmer Ray - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2473;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2473);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2473, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Granistad - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2385;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2385);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2385, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 8700, 9700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Feral Mountain Lion - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2384;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2384);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2384, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 8600, 11400, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Starving Mountain Lion - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2474;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2474);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2474, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Kurdos - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2354;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2354);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2354, 0, 0, 0, 0, 0, 100, 0, 2200, 2600, 7400, 8100, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Vicious Gray Bear - In Combat - Cast \'31279\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2408;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2408);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2408, 0, 0, 0, 2, 0, 100, 1, 10, 40, 0, 0, 11, 26064, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Snapjaw - Between 10-40% Health - Cast \'26064\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2406;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2406);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2406, 0, 0, 0, 0, 0, 100, 0, 2300, 2700, 8700, 9700, 11, 51772, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mountain Lion - In Combat - Cast \'51772\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14276;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14276);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14276, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 8400, 8700, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scargil - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2347;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2347);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2347, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wild Gryphon - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2407;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2407);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2407, 0, 0, 0, 0, 0, 100, 0, 2400, 2600, 11400, 11600, 11, 36332, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Hulking Mountain Lion - In Combat - Cast \'36332\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2248;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2248);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2248, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 13496, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cave Yeti - Between 20-80% Health - Cast \'13496\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14278;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14278);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14278, 0, 0, 0, 0, 0, 100, 0, 2400, 4100, 10800, 13700, 11, 48288, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ro\'Bark - In Combat - Cast \'48288\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14275;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14275);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14275, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 8600, 9400, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tamra Stormpike - In Combat - Cast \'3391\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2356;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2356);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2356, 0, 0, 0, 0, 0, 100, 0, 2200, 2600, 7400, 8100, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Elder Gray Bear - In Combat - Cast \'31279\'');

/*
 * Zone: Hinterlands II
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPT */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2680;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2680);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2680, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vilebranch Wolf Pup - Between 20-80% Health - Cast \'3149\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2929;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2929);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2929, 0, 0, 0, 0, 0, 100, 0, 3200, 3900, 9200, 9800, 11, 54663, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Savage Owlbeast - In Combat - Cast \'54663\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2505;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2505);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2505, 0, 0, 0, 2, 0, 100, 1, 10, 40, 0, 0, 11, 26064, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Saltwater Snapjaw - Between 10-40% Health - Cast \'26064\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12496;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12496);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12496, 0, 0, 0, 0, 0, 100, 0, 2700, 3100, 32700, 33100, 11, 16359, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreamtracker - In Combat - Cast \'16359\''),
(12496, 0, 1, 0, 2, 0, 100, 1, 5, 30, 0, 0, 11, 6605, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreamtracker - Between 5-30% Health - Cast \'6605\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12478;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12478);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12478, 0, 0, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 38657, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Verdantine Oracle - Between 30-60% Health - Cast \'38657\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12479;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12479);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12479, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 16498, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Verdantine Tree Warder - On Aggro - Cast \'16498\''),
(12479, 0, 1, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 12747, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Verdantine Tree Warder - Between 20-80% Health - Cast \'12747\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12477;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12477);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12477, 0, 0, 0, 0, 0, 100, 0, 2700, 3000, 12700, 15600, 11, 12021, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Verdantine Boughguard - In Combat - Cast \'12021\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 2924;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 2924);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2924, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 3149, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Silvermane Wolf - Between 20-80% Health - Cast \'3149\' (No Repeat)');

/*
 * Zone: Azeroth [Part II]
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* SMARTSCRIPTS */
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1848;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1848);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1848, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 17204, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lord Maldazzar - On Aggro - Cast \'17204\''),
(1848, 0, 1, 0, 0, 0, 100, 0, 1700, 2400, 8700, 10300, 11, 12471, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Maldazzar - In Combat - Cast \'12471\''),
(1848, 0, 2, 0, 2, 0, 100, 1, 20, 40, 0, 0, 11, 17173, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lord Maldazzar - Between 20-40% Health - Cast \'17173\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 6412;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 6412);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(6412, 0, 0, 0, 1, 0, 100, 1, 1000, 1500, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Skeleton - Out of Combat - Despawn In 5000 ms (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1850;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1850);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1850, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 17650, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridius - On Aggro - Cast \'17650\''),
(1850, 0, 1, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 12946, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridius - Between 20-80% Health - Cast \'12946\' (No Repeat)'),
(1850, 0, 2, 0, 0, 0, 100, 0, 3400, 5200, 12700, 16800, 11, 10101, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridius - In Combat - Cast \'10101\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1851;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1851);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1851, 0, 0, 0, 2, 0, 100, 1, 20, 80, 0, 0, 11, 17230, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'The Husk - Between 20-80% Health - Cast \'17230\' (No Repeat)'),
(1851, 0, 1, 0, 0, 0, 100, 0, 2400, 3800, 14900, 16800, 11, 3604, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'The Husk - In Combat - Cast \'3604\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1837;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1837);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1837, 0, 0, 0, 0, 0, 100, 0, 2300, 3000, 9800, 10200, 11, 14518, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Judge - In Combat - Cast \'14518\''),
(1837, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 13953, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Judge - Between 30-60% Health - Cast \'13953\' (No Repeat)'),
(1837, 0, 2, 0, 2, 0, 100, 1, 10, 25, 0, 0, 11, 13005, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Judge - Between 10-25% Health - Cast \'13005\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1841;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1841);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1841, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Executioner - On Aggro - Cast \'8599\''),
(1841, 0, 1, 0, 0, 0, 100, 0, 2500, 3000, 9500, 10000, 11, 16856, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Executioner - In Combat - Cast \'16856\''),
(1841, 0, 2, 0, 2, 0, 100, 1, 40, 80, 0, 0, 11, 15284, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Executioner - Between 40-80% Health - Cast \'15284\' (No Repeat)'),
(1841, 0, 3, 0, 12, 0, 100, 1, 5, 19, 0, 0, 11, 7160, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Executioner - Target Between 5-19% Health - Cast \'7160\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1838;
UPDATE `creature_template` SET `unit_class`= 2 WHERE `entry`= 1838;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1838);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1838, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 20294, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Scarlet Interrogator - On Aggro - Cast \'20294\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1843;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1843);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1843, 0, 0, 0, 0, 0, 100, 0, 2500, 4000, 11500, 13000, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foreman Jerris - In Combat - Cast \'3391\''),
(1843, 0, 1, 0, 2, 0, 100, 1, 10, 30, 0, 0, 11, 15618, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foreman Jerris - Between 10-30% Health - Cast \'15618\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1844;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1844);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1844, 0, 0, 0, 0, 0, 100, 0, 2450, 3800, 8700, 9200, 11, 11976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foreman Marcrid - In Combat - Cast \'11976\''),
(1844, 0, 1, 0, 12, 0, 100, 1, 20, 50, 0, 0, 11, 15284, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foreman Marcrid - Target Between 20-50% Health - Cast \'15284\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1847;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1847);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1847, 0, 0, 0, 2, 0, 100, 1, 5, 95, 0, 0, 11, 3427, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foulmane - Between 5-95% Health - Cast \'3427\' (No Repeat)'),
(1847, 0, 1, 0, 0, 0, 100, 0, 3100, 4900, 9600, 11200, 11, 3391, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foulmane - In Combat - Cast \'3391\''),
(1847, 0, 2, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 13445, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Foulmane - Between 30-60% Health - Cast \'13445\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10358;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10358);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10358, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 7068, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fellicent\'s Shade - On Aggro - Cast \'7068\''),
(10358, 0, 1, 0, 0, 0, 100, 0, 2300, 3200, 8900, 9600, 11, 13901, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fellicent\'s Shade - In Combat - Cast \'13901\''),
(10358, 0, 2, 0, 2, 0, 100, 1, 20, 40, 0, 0, 11, 11975, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fellicent\'s Shade - Between 20-40% Health - Cast \'11975\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 10359;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 10359);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10359, 0, 0, 0, 0, 0, 100, 1, 2700, 3900, 0, 0, 11, 3583, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sri\'skulk - In Combat - Cast \'3583\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1531;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1531);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1531, 0, 0, 0, 0, 0, 100, 0, 2200, 2700, 9200, 9700, 11, 7713, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lost Soul - In Combat - Cast \'7713\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1910;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1910);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1910, 0, 0, 0, 0, 0, 100, 0, 2400, 3000, 8700, 10100, 11, 2607, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Muad - In Combat - Cast \'2607\''),
(1910, 0, 1, 0, 2, 0, 100, 1, 30, 60, 0, 0, 11, 332, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Muad - Between 30-60% Health - Cast \'332\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1936;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 1936);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1936, 0, 0, 0, 0, 0, 100, 0, 2400, 2900, 8700, 9200, 11, 11976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Farmer Solliden - In Combat - Cast \'11976\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12431;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12431);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12431, 0, 0, 0, 0, 0, 100, 0, 2400, 2700, 17500, 17800, 11, 13445, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Gorefang - In Combat - Cast \'13445\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 12433;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 12433);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(12433, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 11, 12040, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Krethis Shadowspinner - On Aggro - Cast \'12040\''),
(12433, 0, 1, 0, 0, 0, 100, 0, 2400, 2800, 6900, 7300, 11, 17439, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Krethis Shadowspinner - In Combat - Cast \'17439\'');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 8213;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 8213);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(8213, 0, 0, 0, 2, 0, 100, 1, 10, 40, 0, 0, 11, 26064, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ironback - Between 10-40% Health - Cast \'26064\' (No Repeat)');

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 14280;
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 14280);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(14280, 0, 0, 0, 0, 0, 100, 0, 2700, 3400, 8600, 11400, 11, 31279, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Big Samras - In Combat - Cast \'31279\'');

UPDATE `creature` SET `position_x` = 2972.916748, `position_y` = -4553.171875, `position_z` = 292.601624 WHERE `guid` = 203163;

UPDATE `quest_poi_points` SET `X` = 2025, `Y` = -4519 WHERE `QuestID` = 11314 AND `Idx1` = 0;

UPDATE `quest_poi_points` SET `X` = 2427, `Y` = -4664 WHERE `QuestID` = 11319 AND `Idx1` = 0;

UPDATE `creature` SET `position_x` = -14.866208, `position_y` = -3560.556641, `position_z` = 34.651653 WHERE `guid` = 101032;

UPDATE `gameobject_template` SET `data1` = 4 WHERE `entry` = 30;
UPDATE `gameobject` SET `position_x` = 5911.18 WHERE `guid` = 5272;
UPDATE `gameobject` SET `position_x` = 5904.73 WHERE `guid` = 5273;

-- ---------------- --
-- Maker's Overlook --
-- ---------------- --

-- Sholazar Guardian 111415
DELETE FROM `creature` WHERE `guid`=111415;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (111415, 28069, 571, 0, 0, 1, 1, 26140, 0, 5738.87, 3156.67, 293.833, 4.8144, 300, 0, 0, 11379, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature_addon` WHERE `guid`=111415;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `islarge`, `auras`) VALUES (111415, 847234, 0, 0, 4097, 0, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`=847234 AND `point` IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(847234, 1, 5737.64, 3168.66, 293.833, 0, 0, 0, 0, 100, 0),
(847234, 2, 5734.83, 3190.47, 294.084, 0, 0, 0, 0, 100, 0),
(847234, 3, 5733.38, 3205.57, 295.698, 0, 0, 0, 0, 100, 0),
(847234, 4, 5734.83, 3190.47, 294.084, 0, 0, 0, 0, 100, 0),
(847234, 5, 5737.64, 3168.66, 293.833, 0, 0, 0, 0, 100, 0),
(847234, 6, 5739.32, 3152.2, 293.833, 0, 0, 0, 0, 100, 0),
(847234, 7, 5742.78, 3128.64, 294.013, 0, 0, 0, 0, 100, 0),
(847234, 8, 5744.99, 3100.41, 287.95, 0, 0, 0, 0, 100, 0),
(847234, 9, 5746.91, 3085.88, 287.758, 0, 0, 0, 0, 100, 0),
(847234, 10, 5744.99, 3100.41, 287.95, 0, 0, 0, 0, 100, 0),
(847234, 11, 5742.78, 3128.64, 294.013, 0, 0, 0, 0, 100, 0),
(847234, 12, 5739.32, 3152.2, 293.833, 0, 0, 0, 0, 100, 0);

-- Sholazar Guardian 111416, 111416, 111417, 111418, 111419, 111426, 111427
DELETE FROM `creature` WHERE `guid` IN (111416, 111417, 111418, 111419, 111426, 111427);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
(111416, 28069, 571, 0, 0, 1, 1, 26140, 0, 5752.53, 3141.5, 294.167, 3.35103, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0),
(111417, 28069, 571, 0, 0, 1, 1, 26140, 0, 5728.32, 3137.71, 294.163, 0.715585, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0),
(111418, 28069, 571, 0, 0, 1, 1, 26140, 0, 5737.38, 3051.35, 288.07, 1.23918, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0),
(111419, 28069, 571, 0, 0, 1, 1, 26140, 0, 5781.28, 3081.08, 288.07, 3.08923, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0),
(111426, 28069, 571, 0, 0, 1, 1, 26140, 0, 5717.86, 3074.3, 288.07, 0.401426, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0),
(111427, 28069, 571, 0, 0, 1, 1, 26140, 0, 5767.67, 3052.91, 288.07, 2.18166, 300, 0, 0, 11379, 0, 0, 0, 0, 0, '', 0);

-- Sholazar Guardian 111422
UPDATE `creature` SET `position_x` = 5704.08, `position_y` = 3429.4, `position_z` = 300.842, `orientation` = 1.23918, `MovementType` = 2 WHERE `guid` = 111422;

DELETE FROM `creature_addon` WHERE `guid`=111422;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `islarge`, `auras`) VALUES (111422, 847243, 0, 0, 4097, 0, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`=847243 AND `point` IN (1, 2, 3, 4, 5, 6);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(847243, 1, 5709.81, 3394.62, 300.842, 0, 0, 0, 0, 100, 0),
(847243, 2, 5713.21, 3367.16, 300.253, 0, 0, 0, 0, 100, 0),
(847243, 3, 5715.42, 3346.88, 300.04, 0, 0, 0, 0, 100, 0),
(847243, 4, 5718.08, 3324.58, 299.805, 0, 0, 0, 0, 100, 0),
(847243, 5, 5715.42, 3346.88, 300.04, 0, 0, 0, 0, 100, 0),
(847243, 6, 5713.21, 3367.16, 300.253, 0, 0, 0, 0, 100, 0);

-- Sholazar Guardian 111423 & 111424
UPDATE `creature` SET `position_x` = 5706.99, `position_y` = 3306.82, `position_z` = 299.718, `orientation` = 1.51844 WHERE `guid` = 111423;
UPDATE `creature` SET `position_x` = 5734.7, `position_y` = 3308.84, `position_z` = 299.826, `orientation` = 1.8675 WHERE `guid` = 111424;

-- Titan Sentry
UPDATE `creature_template` SET `InhabitType`= 4 WHERE  `entry`=29066;

-- Overlook Sentry 111399
DELETE FROM `creature` WHERE `guid`=111399;
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES (111399, 28840, 571, 0, 0, 1, 1, 26156, 0, 5737.76, 3273.59, 299.117, 2.79445, 300, 0, 0, 58850, 0, 2, 0, 0, 0, '', 0);

DELETE FROM `creature_addon` WHERE `guid`=111399;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `islarge`, `auras`) VALUES (111399, 847236, 0, 0, 4097, 0, 0, NULL);

DELETE FROM `waypoint_data` WHERE `id`=847236 AND `point` IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES 
(847236, 1, 5712.78, 3273.75, 299.128, 0, 0, 0, 0, 100, 0),
(847236, 2, 5705.58, 3264.4, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 3, 5704.04, 3252.41, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 4, 5711.5, 3241.04, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 5, 5725.47, 3233.01, 299.114, 0, 0, 0, 0, 100, 0),
(847236, 6, 5738.39, 3238.95, 299.115, 0, 0, 0, 0, 100, 0),
(847236, 7, 5744.12, 3248.18, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 8, 5747.94, 3262.18, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 9, 5737.76, 3273.59, 299.117, 0, 0, 0, 0, 100, 0),
(847236, 10, 5723.77, 3278.65, 299.138, 0, 0, 0, 0, 100, 0);

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

-- fix models
UPDATE `creature_template` SET `modelid1`=23258, `modelid2`=0 WHERE `entry` IN
(33395, 33402);

UPDATE `quest_template_addon` SET `ExclusiveGroup` = 12582 WHERE `id` IN (12582,12689);
UPDATE `quest_template_addon` SET `PrevQuestID` = 0 WHERE `id` IN (12692,12695);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceGroup`=0 AND `SourceEntry` IN (12692,12695);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(19, 0, 12692, 0, 0, 43, 0, 12582, 0, 0, 0, 0, 0, '', 'Quest \'Return of the Lich Hunter\' available when \'Frenzyheart Champion\' has been rewarded'),
(19, 0, 12695, 0, 0, 43, 0, 12689, 0, 0, 0, 0, 0, '', 'Quest \'Return of the Friendly Dryskin\' available when \'Hand of the Oracles\' has been rewarded');

UPDATE `spell_proc_event` SET `procFlags` = 0, `Cooldown` = 50000 WHERE `entry` = 67653;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 15998;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(17, 0, 15998, 0, 0, 29, 0, 10221, 3, 0, 0, 0, 0, '', 'Empty Worg Pup Cage requires Bloodaxe Worg Pup (3y)');

/* Add extra mobs to The Antechamber in 25-man version of Ulduar */

DELETE FROM `creature` WHERE `guid` IN (136770,136771,136772,136773,136774,136775);
INSERT INTO `creature` VALUES 

/* Near Teleporter */

    /* Iron Golem */
    (136770, 34190, 603, 0, 0, 2, 1, 26155, 0, 1548.84, -10.5333, 420.967, 5.04629, 604800, 0, 0, 404430, 0, 0, 0, 0, 0, '', 0),
    /* Lightning Charged Iron Dwarf */
    (136771, 34199, 603, 0, 0, 2, 1, 26239, 1, 1549.15, -19.0012, 420.967, 5.12484, 604800, 5, 0, 471835, 0, 1, 0, 0, 0, '', 0),
    /* Iron Mender */
    (136772, 34198, 603, 0, 0, 2, 1, 26218, 1, 1552.57, -14.2679, 420.967, 3.94674, 604800, 0, 0, 337025, 62535, 0, 0, 0, 0, '', 0),

/* Near Stairs */

    /* Iron Golem */
    (136773, 34190, 603, 0, 0, 2, 1, 26155, 0, 1682.97, -11.0873, 427.312, 3.93888, 604800, 0, 0, 404430, 0, 0, 0, 0, 0, '', 0),
    /* Lightning Charged Iron Dwarf */
    (136774, 34199, 603, 0, 0, 2, 1, 26239, 1, 1679.91, -19.6422, 427.31, 3.19275, 604800, 0, 0, 471835, 0, 0, 0, 0, 0, '', 0),
    /* Iron Mender */
    (136775, 34198, 603, 0, 0, 2, 1, 26218, 1, 1676.98, -11.2869, 427.31, 5.76657, 604800, 0, 0, 337025, 62535, 0, 0, 0, 0, '', 0);
    
/* Lightning Charged Iron Dwarf unsheath weapons and emote ready */
DELETE FROM `creature_addon` WHERE `guid` IN (136609,136610,136771,136774);
INSERT INTO `creature_addon` (`guid`, `bytes2`, `emote`) VALUES 
(136609, 1, 333),
(136610, 1, 0),
(136771, 1, 0),
(136774, 1, 333);

DELETE `creature`, `creature_addon`, `waypoint_data`
FROM
    `creature`, `creature_addon`, `waypoint_data`
WHERE
    `creature`.`guid` = 203340 AND `creature`.`guid` = `creature_addon`.`guid` AND `creature_addon`.`path_id` = `waypoint_data`.`id`;

/*
 * Update by Silker | <www.azerothcore.org> | Copyright (C)
*/

UPDATE `creature` SET `position_x`=10329, `position_y`=826.056, `position_z`=1326.37, `orientation`=2.72516 WHERE `guid`=46205; -- Ilthalaine
UPDATE `creature` SET `position_x`=-8933.93, `position_y`=-137.472, `position_z`=83.2960, `orientation`=2.2803 WHERE  `guid`=79942; -- Deputy Willem

/* Copy FemaleText into missing MaleText for Computer NPC in Mimiron fight, Ulduar. */
UPDATE `broadcast_text` SET `MaleText`=`FemaleText` WHERE `ID` IN (34284,34283,34282,34281,34280,34279,34278,34277,34276,34275,34274,34273,34268);
UPDATE `broadcast_text_locale` SET `MaleText`=`FemaleText` WHERE `ID` IN (34284,34283,34282,34281,34280,34279,34278,34277,34276,34275,34274,34273,34268);

UPDATE `creature_text` SET `Text`='This is our final stand. What happens here will echo through the ages. Regardless of outcome, they will know that we fought with honor. That we fought for the freedom and safety of our people!' WHERE  `CreatureID`=37119 AND `GroupID`=0 AND `ID`=0;
UPDATE `broadcast_text` SET `MaleText`='This is our final stand. What happens here will echo through the ages. Regardless of outcome, they will know that we fought with honor. That we fought for the freedom and safety of our people!' WHERE  `ID`=36923;

-- Remove Quest Giver & Gossip flags
UPDATE `creature_template` SET `npcflag` = 0 WHERE (`entry` = 1747);

-- Remove Cataclysm gossip from DB
DELETE FROM `gossip_menu` WHERE (`MenuID` = 11874) AND (`TextID` IN (16642));

-- Pyrobuffet (57557)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=7 AND `SourceEntry`=57557 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=26 AND `ConditionTarget`=0 AND `ConditionValue1`=1 AND `ConditionValue2`=0 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 7, 57557, 0, 0, 26, 0, 1, 0, 0, 0, 0, 0, '', 'Pyrobuffet - Target players which has phase mask 1');

-- Twilight Torment (57935)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=7 AND `SourceEntry`=57935 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=26 AND `ConditionTarget`=0 AND `ConditionValue1`=1 AND `ConditionValue2`=0 AND `ConditionValue3`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=17 AND `SourceGroup`=0 AND `SourceEntry`=57935 AND `SourceId`=0 AND `ElseGroup`=0 AND `ConditionTypeOrReference`=26 AND `ConditionTarget`=1 AND `ConditionValue1`=1 AND `ConditionValue2`=0 AND `ConditionValue3`=0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 57935, 0, 0, 26, 1, 1, 0, 0, 0, 0, 0, '', 'Twilight Torment - Target players which has phase mask 1');

-- Twilight Shift aura removes Twilight Torment
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=57620 AND `spell_effect`=-57935 AND `type`=0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(57620, -57935, 0, 'Twilight Shift aura removes Twilight Torment');

-- The Lich King Timed Actionlist
DELETE FROM `smart_scripts` WHERE (`source_type` = 9 AND `entryorguid` = 25462 * 100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(25462 * 100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 4, 14734, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'In Service of the Lich King - Quest Accept - Play Sound 14734'),
(25462 * 100, 9, 1, 0, 0, 0, 100, 0, 21000, 21000, 0, 0, 4, 14735, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'In Service of the Lich King - Quest Accept - Play Sound 14735'),
(25462 * 100, 9, 2, 0, 0, 0, 100, 0, 26000, 26000, 0, 0, 4, 14736, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'In Service of the Lich King - Quest Accept - Play Sound 14736');

-- The Lich King Timed Actionlist Trigger
DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` = 25462);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(25462, 0, 0, 0, 19, 0, 100, 0, 12593, 0, 0, 0, 80, 2546200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'In Service of the Lich King - Quest Accept - Timed Actionlist');

-- 'In Service of The Lich King' quest details
DELETE FROM `quest_details` WHERE `ID` = 12593;
INSERT INTO `quest_details` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `EmoteDelay1`, `VerifiedBuild`) VALUES
(12593, 396, 397, 396, 396, 500, 1000, 1000, 1000, 20886);

UPDATE `creature_template` SET `type_flags`=`type_flags`&~ 4 WHERE `entry` IN (11520,4887,4831,6243,4830,4832,4829,16042,8923,8929,8983,9016,9017,9018,9019,9026,9027,9028,9029,9030,9031,9032,9033,9034,9035,9036,9037,9038,9039,9040,9056,9156,9319,9499,9502,9537,9543,9938,16059,16080,644,642,643,1763,647,639,646,645,14354,14327,13280,11490,11492,14322,14326,14321,14323,14325,14324,11501,11489,11487,11488,11496,11486,14506,7361,7079,6235,6229,7800,16387,9196,9237,9236,9736,10596,10584,10220,10268,9568,13282,13596,13601,12258,12236,12225,12203,12201,11520,11518,11519,11517,7355,7357,8567,7356,7358,4424,4428,4420,4422,4421,6168,3983,4543,3974,6487,3975,4542,3976,3977,10506,11622,10433,10432,10508,10503,10901,10502,10504,10507,11261,10505,1853,3914,3865,3886,3887,4278,4279,4274,3927,4275,1696,1666,1663,1716,1717,10435,10808,11032,10997,10811,10812,10813,10436,10437,10438,10439,10440,8580,8443,5709,5710,5711,5712,5713,5714,5715,5716,5717,5719,5720,5721,5722,6910,6906,6907,6908,7206,7291,4854,7023,7228,2748,9816,10264,10429,10430,10339,103363,3671,3670,3674,3673,5775,3653,3669,3654,7272,8127,7271,7604,7795,7796,7275,7273,7797,7267,18371,20318,18373,20306,17381,18621,17380,18601,17377,18607,17879,20738,17880,20745,17881,20737,17876,20548,17848,20535,17862,20521,18096,20531,17306,18436,17308,18433,17536,18432,17537,18434,24723,25562,24744,25573,24559,25574,24558,25549,24554,25550,24555,25555,24561,25578,24553,25541,24556,25579,24557,25556,24560,25560,24664,24857,18341,20267,18343,20268,18344,20266,22930,18472,20690,18473,20706,23035,18731,20636,18667,20637,18732,20653,18708,20657,16807,20568,16809,20596,16808,20597,20923,20993,17941,19893,17991,19895,17942,19894,17797,20629,17796,20630,17798,20633,20870,21626,20885,21590,20886,21624,20912,21601,21466,21600,21467,21599,17976,21551,17975,21558,17978,21581,17980,21559,17977,21582,19710,21526,19219,21533,19218,21525,19221,21536,19220,21537,17770,20169,18105,20168,17826,20183,17882,20184);

-- Arthas - Frostmourne Improvements

UPDATE `creature_template_addon` SET `bytes2` = 1 WHERE (`entry` = 36954);
UPDATE `creature_equip_template` SET `ItemID1` = 0 WHERE (`CreatureID` = 37226);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 37226;
DELETE FROM `smart_scripts` WHERE (`entryorguid` = 37226) AND (`source_type` = 0) AND (`id` IN (0));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37226, 0, 0, 0, 8, 0, 100, 0, 72729, 0, 0, 0, 0, 71, 0, 0, 49706, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Halls of Reflection - Arthas Frostmourne Equip');

-- Escape from Arthas (Final Encounter) Text Improvements

UPDATE `creature_text` SET `Text`='Your allies have arrived Jaina, just as you promised. You will all become powerful agents of the Scourge.' WHERE  `CreatureID`=36954 AND `GroupID`=101 AND `ID`=0;
UPDATE `creature_text` SET `Text`='Your allies have arrived Jaina, just as you promised. You will all become powerful agents of the Scourge.' WHERE  `CreatureID`=36954 AND `GroupID`=0 AND `ID`=0;
UPDATE `creature_text` SET `Text`='He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!' WHERE  `CreatureID`=36955 AND `GroupID`=0 AND `ID`=0;
UPDATE `creature_text` SET `Text`='He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!' WHERE  `CreatureID`=36955 AND `GroupID`=103 AND `ID`=0;
UPDATE `creature_text` SET `Text`='He\'s... too powerful. Heroes, quickly... come to me! We must leave this place at once! I will do what I can do hold him in place while we flee.' WHERE  `CreatureID`=37554 AND `GroupID`=0 AND `ID`=0;
UPDATE `creature_text` SET `Text`='He\'s... too powerful. Heroes, quickly... come to me! We must leave this place at once! I will do what I can do hold him in place while we flee.' WHERE  `CreatureID`=37554 AND `GroupID`=102 AND `ID`=0;
UPDATE `broadcast_text` SET `FemaleText`='He\'s... too powerful. Heroes, quickly... come to me! We must leave this place at once! I will do what I can do hold him in place while we flee.' WHERE  `ID`=37168;
UPDATE `broadcast_text` SET `FemaleText`='He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!' WHERE  `ID`=36821;
UPDATE `broadcast_text` SET `MaleText`='Your allies have arrived Jaina, just as you promised. You will all become powerful agents of the Scourge.' WHERE  `ID`=37172;

-- Open Door on Sneed's Death (Deadmines)

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 643;

DELETE FROM `smart_scripts` WHERE (`entryorguid` = 643) AND (`source_type` = 0) AND (`id` IN (1));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(643, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 14, 26185, 17153, 0, 0, 0, 0, 0, 0, 'Sneed - Open Door on Death');

-- Saronite Animus 10 player and 25 player
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83 WHERE `entry` IN (33524,34152);

-- Snobold Vassal 10 Normal & Heroic, and 25 Normal & Heroic
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82 WHERE `entry` IN (34800,35441,35442,35443);

-- Update level range for Val'kyr Battle-maiden (59-60)
UPDATE `creature_template` SET `maxlevel`=60 WHERE `entry`=31095;

-- Update Terrifying Abomination wander distance
UPDATE `creature` SET `wander_distance`=1 WHERE `guid` IN 
(130505, 130506, 130507, 130508, 130511, 130512, 130513, 130515, 130516, 130517, 130519, 130521, 130522, 130524, 130525, 130526, 130527, 130528, 130530, 130531, 130532, 130535, 130536, 130537, 130539);

-- Update level range for Terrifying Abomination (57-58)
UPDATE `creature_template` SET `minlevel`=57 WHERE `entry`=31098;

-- Fix Terrifying Abomination's SAI
DELETE FROM `smart_scripts` WHERE `entryorguid`=31098 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(31098, 0, 0, 0, 9, 0, 50, 1, 8, 20, 0, 0, 0, 11, 50335, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Terrifying Abomination - Within 8-20 Range - Cast Scourge Hook'),
(31098, 0, 1, 0, 0, 0, 80, 0, 0, 0, 7000, 9000, 0, 11, 15496, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Terrifying Abomination - In Combat - Cast Cleave');

-- Update SAI, unit_flag and update level range for Knight of the Ebon Blade (57-58)
UPDATE `creature_template` SET `minlevel`=57, `unit_flags`=557056, `AIName`='SmartAI' WHERE `entry`=31094;
DELETE FROM `smart_scripts` WHERE `entryorguid`=31094 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(31094, 0, 0, 0, 0, 0, 100, 0, 9250, 14750, 10250, 15750, 0, 11, 52372, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Knight of the Ebon Blade - In Combat - Cast Icy Touch'),
(31094, 0, 1, 0, 0, 0, 100, 0, 4500, 6200, 5500, 7200, 0, 11, 52374, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Knight of the Ebon Blade - In Combat - Cast Blood Strike'),
(31094, 0, 2, 0, 0, 0, 100, 0, 3500, 9100, 4500, 11000, 0, 11, 52373, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Knight of the Ebon Blade - In Combat - Cast Plague Strike'),
(31094, 0, 3, 0, 0, 0, 100, 0, 5200, 15500, 6200, 17500, 0, 11, 52375, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Knight of the Ebon Blade - In Combat - Cast Death Coil'),
(31094, 0, 4, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 70, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Knight of the Ebon Blade - On Death - Respawn');

-- Add missing Knight of the Ebon Blade corpses on lower level
DELETE FROM `creature` WHERE `guid` IN (200153, 200154, 200155, 200156, 200157);
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `VerifiedBuild`) VALUES 
(200153, 31094, 0, 0, 0, 1, 448, 0, 1, 2425.68, -5617.98, 420.644, 1.325, 360, 0, 0, 15955, 11445, 0, 0, 537166592, 32, 0),
(200154, 31094, 0, 0, 0, 1, 448, 0, 1, 2435.938, -5620.978, 420.421, 3.319, 360, 0, 0, 15955, 11445, 0, 0, 537166592, 32, 0),
(200155, 31094, 0, 0, 0, 1, 448, 0, 1, 2415.195, -5601.245, 420.643, 0.833, 360, 0, 0, 15955, 11445, 0, 0, 537166592, 32, 0),
(200156, 31094, 0, 0, 0, 1, 448, 0, 1, 2396.438, -5628.738, 420.702, 3.523, 360, 0, 0, 15955, 11445, 0, 0, 537166592, 32, 0),
(200157, 31094, 0, 0, 0, 1, 448, 0, 1, 2403.937, -5642.425, 420.678, 0.478, 360, 0, 0, 15955, 11445, 0, 0, 537166592, 32, 0);

DELETE FROM `creature_addon` WHERE `guid` IN (200153, 200154, 200155, 200156, 200157);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `isLarge`, `auras`) VALUES 
(200153, 0, 0, 0, 1, 0, 0, 29266),
(200154, 0, 0, 0, 1, 0, 0, 29266),
(200155, 0, 0, 0, 1, 0, 0, 29266),
(200156, 0, 0, 0, 1, 0, 0, 29266),
(200157, 0, 0, 0, 1, 0, 0, 29266);

-- Update Knight of the Ebon Blade positions
UPDATE `creature` SET `position_x`=2410.179, `position_y`=-5621.223, `position_z`=420.653 WHERE `guid`=130488;
UPDATE `creature` SET `position_x`=2411.769, `position_y`=-5624.546, `position_z`=420.652 WHERE `guid`=130485;
UPDATE `creature` SET `position_x`=2413.665, `position_y`=-5627.342, `position_z`=420.652 WHERE `guid`=130487;
UPDATE `creature` SET `position_x`=2415.735, `position_y`=-5629.859, `position_z`=420.651 WHERE `guid`=130486;

-- Add Patchwerk texts
DELETE FROM `creature_text` WHERE `CreatureID`=31099 AND `GroupID`=0;
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES 
(31099, 0, 0, '%s goes into a frenzy!', 41, 0, 100, 0, 0, 0, 38630, 0, 'Patchwerk emote Frenzy - Ebon Hold');

-- Update AIName to Patchwerk and build SAI
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=31099;
DELETE FROM `smart_scripts` WHERE `entryorguid`=31099 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(31099, 0, 0, 0, 0, 0, 100, 0, 1000, 2000, 1500, 1500, 0, 11, 58412, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 'Patchwerk - In Combat - Cast Hateful Strike'),
(31099, 0, 1, 0, 2, 0, 100, 1, 0, 4, 0, 0, 0, 11, 28131, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Patchwerk - On 5% HP - Cast self Frenzy'),
(31099, 0, 2, 0, 2, 0, 100, 1, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Patchwerk - On 5% HP - Say Line 0');

-- Make spell 'Scourge hook' to only target players
DELETE FROM `conditions` WHERE `SourceEntry`=50335 AND `ConditionTypeOrReference`=31;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `Comment`) VALUES 
(17, 0, 50335, 0, 0, 31, 1, 4, 0, 0, 0, 0, 0, 'Spell force target - \'Scourge Hook\' - Player only');

-- Adjust Patchwerk spawn time
UPDATE `creature` SET `spawntimesecs`=120 WHERE `guid`=130542;

-- Reduce cooldown
UPDATE `smart_scripts` SET `event_param3`=10000, `event_param4`=15000, `comment`='Stitched Giant - In combat - Cast target Knockback' WHERE `entryorguid`=16025 AND `source_type`=0 AND `id`=0 AND `link`=0;

-- Fix enrage
DELETE FROM `smart_scripts` WHERE `entryorguid`=16025 AND `id`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_param4`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(16025, 0, 1, 0, 2, 0, 100, 1, 0, 29, 0, 0, 0, 11, 54356, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Stitched Giant - On 30% HP - Cast self Unstoppable Enrage');

UPDATE `creature` SET `spawntimesecs` = 0 WHERE `id` IN (8901);
UPDATE `creature` SET `spawntimesecs` = 300 WHERE `id` IN (9956);

DELETE FROM `creature_formations` WHERE `memberGUID` IN (9203, 9204, 9205) AND `leaderGUID` = 9203;
INSERT INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`, `point_1`, `point_2`) VALUES
(9203, 9203, 0, 0, 2, 0, 0),
(9203, 9204, 3, 45, 2, 0, 0),
(9203, 9205, 3, 315, 2, 0, 0);

DELETE FROM `creature_addon` WHERE guid IN (9204,9205);
DELETE FROM `waypoint_data` WHERE id IN (92040,92050);
UPDATE `creature` SET `MovementType`=0, `wander_distance`=0 WHERE `id` IN (2477,7170);
UPDATE `creature_template` SET `MovementType`=0 WHERE `entry` IN (2477,7170);
UPDATE `creature_template` SET `Speed_Walk`=1.47, `Speed_Run`=1.14286 WHERE `entry` IN (2478);
UPDATE `creature_template` SET `Speed_Walk`=1.48, `Speed_Run`=1.14286 WHERE `entry` IN (2477,7170);

/*
 * Raid: The Obsidian Sanctum
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* REGULAR */
UPDATE `creature_template` SET `mindmg` = 3293, `maxdmg` = 4560, `DamageModifier` = 1.03 WHERE `entry` = 30682;
UPDATE `creature_template` SET `mindmg` = 6586, `maxdmg` = 9120, `DamageModifier` = 1.03 WHERE `entry` = 31000;
UPDATE `creature_template` SET `mindmg` = 3254, `maxdmg` = 4522, `DamageModifier` = 1.03 WHERE `entry` = 30681;
UPDATE `creature_template` SET `mindmg` = 6508, `maxdmg` = 9044, `DamageModifier` = 1.03 WHERE `entry` = 30998;
UPDATE `creature_template` SET `mindmg` = 3293, `maxdmg` = 4790, `DamageModifier` = 1.03 WHERE `entry` = 30680;
UPDATE `creature_template` SET `mindmg` = 6586, `maxdmg` = 9580, `DamageModifier` = 1.03 WHERE `entry` = 30999;
UPDATE `creature_template` SET `mindmg` = 3287, `maxdmg` = 4670, `DamageModifier` = 1.03 WHERE `entry` = 30453;
UPDATE `creature_template` SET `mindmg` = 6574, `maxdmg` = 9340, `DamageModifier` = 1.03 WHERE `entry` = 31001;

/* BOSS */ 
UPDATE `creature_template` SET `mindmg` = 17815, `maxdmg` = 23905, `DamageModifier` = 1.01 WHERE `entry` = 28860;
UPDATE `creature_template` SET `mindmg` = 35630, `maxdmg` = 47810, `DamageModifier` = 1.01 WHERE `entry` = 31311;
/* MINI BOSS */ 
UPDATE `creature_template` SET `mindmg` = 13743, `maxdmg` = 18441, `DamageModifier` = 1.01 WHERE `entry` = 30449;
UPDATE `creature_template` SET `mindmg` = 27486, `maxdmg` = 36882, `DamageModifier` = 1.01 WHERE `entry` = 31535;
UPDATE `creature_template` SET `mindmg` = 13743, `maxdmg` = 18441, `DamageModifier` = 1.01 WHERE `entry` = 30451;
UPDATE `creature_template` SET `mindmg` = 27486, `maxdmg` = 36882, `DamageModifier` = 1.01 WHERE `entry` = 31520;
UPDATE `creature_template` SET `mindmg` = 13743, `maxdmg` = 18441, `DamageModifier` = 1.01 WHERE `entry` = 30452;
UPDATE `creature_template` SET `mindmg` = 27486, `maxdmg` = 36882, `DamageModifier` = 1.01 WHERE `entry` = 31534;

/*
 * Raid: The Eye of Eternity 
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* REGULAR */ 
UPDATE `creature_template` SET `mindmg` = 4137, `maxdmg` = 6356, `DamageModifier` = 1.03 WHERE `entry` = 30245;
UPDATE `creature_template` SET `mindmg` = 8274, `maxdmg` = 12712, `DamageModifier` = 1.03 WHERE `entry` = 31750;
UPDATE `creature_template` SET `mindmg` = 3514, `maxdmg` = 5614, `DamageModifier` = 1.03 WHERE `entry` = 30249;
UPDATE `creature_template` SET `mindmg` = 7028, `maxdmg` = 11228, `DamageModifier` = 1.03 WHERE `entry` = 31751;

/* BOSS */ 
UPDATE `creature_template` SET `mindmg` = 18815, `maxdmg` = 24905, `DamageModifier` = 1.01 WHERE `entry` = 28859;
UPDATE `creature_template` SET `mindmg` = 37630, `maxdmg` = 49810, `DamageModifier` = 1.01 WHERE `entry` = 31734;

/*
 * Raid: Vault of Archavon
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* REGULAR */
UPDATE `creature_template` SET `mindmg` = 3162, `maxdmg` = 4399, `DamageModifier` = 1.03 WHERE `entry` = 32353;
UPDATE `creature_template` SET `mindmg` = 6324, `maxdmg` = 8798, `DamageModifier` = 1.03 WHERE `entry` = 32368;
UPDATE `creature_template` SET `mindmg` = 3112, `maxdmg` = 4170, `DamageModifier` = 1.03 WHERE `entry` = 35143;
UPDATE `creature_template` SET `mindmg` = 6224, `maxdmg` = 8340, `DamageModifier` = 1.03 WHERE `entry` = 35359;
UPDATE `creature_template` SET `mindmg` = 3337, `maxdmg` = 4676, `DamageModifier` = 1.03 WHERE `entry` = 34015;
UPDATE `creature_template` SET `mindmg` = 6674, `maxdmg` = 9352, `DamageModifier` = 1.03 WHERE `entry` = 34016;
UPDATE `creature_template` SET `mindmg` = 3438, `maxdmg` = 4724, `DamageModifier` = 1.03 WHERE `entry` = 33998;
UPDATE `creature_template` SET `mindmg` = 6876, `maxdmg` = 9448, `DamageModifier` = 1.03 WHERE `entry` = 34200;
UPDATE `creature_template` SET `mindmg` = 3433, `maxdmg` = 4738, `DamageModifier` = 1.03 WHERE `entry` = 38482;
UPDATE `creature_template` SET `mindmg` = 6866, `maxdmg` = 9476, `DamageModifier` = 1.03 WHERE `entry` = 38483;

/* BOSS */ 
/* 1st: Available at opening - Good values atm. */
UPDATE `creature_template` SET `mindmg` = 17815, `maxdmg` = 23905, `DamageModifier` = 1.01 WHERE `entry` = 31125; 
UPDATE `creature_template` SET `mindmg` = 35630, `maxdmg` = 47810, `DamageModifier` = 1.01 WHERE `entry` = 31722;
/* 2nd: Second one to appear, towards Tier 8 - During Ulduar. */
UPDATE `creature_template` SET `mindmg` = 25450, `maxdmg` = 34700, `DamageModifier` = 1.01 WHERE `entry` = 33993;
UPDATE `creature_template` SET `mindmg` = 50900, `maxdmg` = 69400, `DamageModifier` = 1.01 WHERE `entry` = 33994;
 /* 3rd: Towards Tier 8.5 / 9 - During Trial of Crusader. */ 
UPDATE `creature_template` SET `mindmg` = 27250, `maxdmg` = 37750, `DamageModifier` = 1.01 WHERE `entry` = 35013;
UPDATE `creature_template` SET `mindmg` = 54500, `maxdmg` = 75500, `DamageModifier` = 1.01 WHERE `entry` = 35360;
 /* 4th: Towards Tier 10 and last PvP Season - During Icecrown Citadel (Need testing) */
UPDATE `creature_template` SET `mindmg` = 30380, `maxdmg` = 41445, `DamageModifier` = 1.01 WHERE `entry` = 38433;
UPDATE `creature_template` SET `mindmg` = 60760, `maxdmg` = 82890, `DamageModifier` = 1.01 WHERE `entry` = 38462;

/*
 * Raid: Onyxia Lair
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* REGULAR */
UPDATE `creature_template` SET `mindmg` = 4293, `maxdmg` = 6560, `DamageModifier` = 1.03 WHERE `entry` = 12129;
UPDATE `creature_template` SET `mindmg` = 8586, `maxdmg` = 13120, `DamageModifier` = 1.03 WHERE `entry` = 36572;
UPDATE `creature_template` SET `mindmg` = 776, `maxdmg` = 912, `DamageModifier` = 1.03 WHERE `entry` = 11262;
UPDATE `creature_template` SET `mindmg` = 1552, `maxdmg` = 1824, `DamageModifier` = 1.03 WHERE `entry` = 36566;
UPDATE `creature_template` SET `mindmg` = 3165, `maxdmg` = 4395, `DamageModifier` = 1.03 WHERE `entry` = 36561;
UPDATE `creature_template` SET `mindmg` = 6330, `maxdmg` = 8790, `DamageModifier` = 1.03 WHERE `entry` = 36571;

/* BOSS */ 
UPDATE `creature_template` SET `mindmg` = 27860, `maxdmg` = 38650, `DamageModifier` = 1.01 WHERE `entry` = 10184;
UPDATE `creature_template` SET `mindmg` = 55720, `maxdmg` = 77300, `DamageModifier` = 1.01 WHERE `entry` = 36538;