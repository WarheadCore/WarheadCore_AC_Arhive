SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for player_buff
-- ----------------------------
DROP TABLE IF EXISTS `player_buff`;
CREATE TABLE `player_buff`  (
  `SpellID` int(10) NOT NULL COMMENT 'Spell ID',
  `Comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`SpellID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of player_buff
-- ----------------------------
INSERT INTO `player_buff` VALUES (25898, '');
INSERT INTO `player_buff` VALUES (48074, '');
INSERT INTO `player_buff` VALUES (48162, '');
INSERT INTO `player_buff` VALUES (48470, '');
INSERT INTO `player_buff` VALUES (57623, '');
INSERT INTO `player_buff` VALUES (61316, '');

SET FOREIGN_KEY_CHECKS = 1;
