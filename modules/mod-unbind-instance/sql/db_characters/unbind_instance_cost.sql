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
-- Records of unbind_instance_cost
-- ----------------------------

INSERT INTO `unbind_instance_cost`
        VALUES (37711, 5, 10, 25, 20, 50);
