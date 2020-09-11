-- DB update 2020_09_10_00 -> 2020_09_10_01
DROP PROCEDURE IF EXISTS `updateDb`;
DELIMITER //
CREATE PROCEDURE updateDb ()
proc:BEGIN DECLARE OK VARCHAR(100) DEFAULT 'FALSE';
SELECT COUNT(*) INTO @COLEXISTS
FROM information_schema.COLUMNS
WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'version_db_world' AND COLUMN_NAME = '2020_09_10_00';
IF @COLEXISTS = 0 THEN LEAVE proc; END IF;
START TRANSACTION;
ALTER TABLE version_db_world CHANGE COLUMN 2020_09_10_00 2020_09_10_01 bit;
SELECT sql_rev INTO OK FROM version_db_world WHERE sql_rev = '1598880666303368400'; IF OK <> 'FALSE' THEN LEAVE proc; END IF;
--
-- START UPDATING QUERIES
--

INSERT INTO `version_db_world` (`sql_rev`) VALUES ('1598880666303368400');
/*
 * General: Build Update
 * Update by Knindza | <www.azerothcore.org>
 * Copyright (C) <www.shadowburn.net> & <www.lichbane.com>
*/

/* Content 3.2.2 */ 
SET @Build := 10505;

UPDATE `item_template` SET `VerifiedBuild` = @Build WHERE `entry` IN (47545, 47546, 47547, 47548, 47549, 47550, 47553, 47554, 47154, 47157, 47190, 47191, 47237, 47240, 47472, 47474, 47479, 47484, 47485, 47491, 47515, 47517, 47518, 47519, 49464, 49465, 49466, 49467, 49468, 49469, 49470, 49471, 49472, 49473, 49474, 49475, 49476, 49477, 49478, 49479, 49480, 49481, 49482, 49483, 49484, 49485, 49486, 49487, 49488, 49489, 49490, 49491, 49492, 49493, 49494, 49495, 49496, 49497, 49498, 49499, 49500, 49501, 49296, 49297, 49298, 49299, 49301, 49302, 49303, 49304, 49305, 49306, 49307, 49308, 49309, 49310, 49315, 49316, 49317, 49318, 49319, 49320, 49321, 49322, 49323, 49324, 49325, 49326, 49327, 49328, 49329, 49330, 49331, 49332, 49333, 49437, 49463, 49682, 49121, 49123, 49124, 49126, 49128, 37348, 49643, 49644, 49667, 57142, 49632, 49634, 49633, 49636, 44627, 46887, 44861, 46807, 49295, 44859, 46806, 49046, 44858, 46805, 46861, 49703, 44862, 46804, 57147, 57148, 57143, 57144, 57145, 57146, 37907, 37908, 46710, 49287, 49294, 49704, 41133, 44810, 49343, 49665, 49693, 46809, 46810, 49288, 49289, 49702, 34044, 34117, 43486, 43489, 44785, 44788, 44812, 44844, 44860, 46690, 46711, 46718, 46723, 46800, 46803, 46824, 46831, 46860, 46888, 49335, 49362, 49676, 49678);

--
-- END UPDATING QUERIES
--
COMMIT;
END //
DELIMITER ;
CALL updateDb();
DROP PROCEDURE IF EXISTS `updateDb`;
