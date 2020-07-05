-- Add areatrigger_scripts for nax tp
DELETE FROM `areatrigger_scripts` WHERE `entry` = 4156;
INSERT INTO `areatrigger_scripts`(`entry`, `ScriptName`) VALUES (4156, 'nax_frostwyrm_lair');
