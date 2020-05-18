-- ----------------------------
-- Table structure for quest_buff
-- ----------------------------
DROP TABLE IF EXISTS `quest_buff`;
CREATE TABLE `quest_buff`  (
  `QuestID` int(11) NOT NULL,
  `SpellID` int(11) NOT NULL,
  `SpellRank` int(11) NOT NULL,
  `Category` int(11) NOT NULL,
  PRIMARY KEY (`QuestID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
