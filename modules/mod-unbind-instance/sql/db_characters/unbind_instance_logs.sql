-- ----------------------------
-- Table structure for unbind_instance_cost
-- ----------------------------

DROP TABLE IF EXISTS `unbind_instance_cost`;

CREATE TABLE `unbind_instance_cost` (
    `ItemID` int (10) NOT NULL COMMENT 'Номер предмета',
    `CountForDungeonHeroic` int (10) NOT NULL COMMENT 'Количество предметов за сброс кд 5 гер',
    `CountForRaid10Normal` int (10) NOT NULL COMMENT 'Количество предметов за сброс кд 10 об',
    `CountForRaid25Normal` int (10) NOT NULL COMMENT 'Количество предметов за сброс кд 25 об',
    `CountForRaid10Heroic` int (10) NOT NULL COMMENT 'Количество предметов за сброс кд 10 гер',
    `CountForRaid25Heroic` int (10) NOT NULL COMMENT 'Количество предметов за сброс кд 25 гер',
    PRIMARY KEY (`ItemID`)
    USING BTREE) ENGINE = InnoDB CHARACTER
SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for unbind_instance_logs
-- ----------------------------

DROP TABLE IF EXISTS `unbind_instance_logs`;

CREATE TABLE `unbind_instance_logs` (
    `ID` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Инкремент',
    `Info` varchar(255) DEFAULT NULL,
    `PlayerGuid` int(11) NOT NULL,
    `MapID` int(10) NOT NULL,
    `Difficulty` int(5) NOT NULL,
    `ItemID` int(10) NOT NULL,
    PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
