SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for level_reward
-- ----------------------------
DROP TABLE IF EXISTS `level_reward`;
CREATE TABLE `level_reward`  (
  `Level` int(3) NOT NULL DEFAULT 1,
  `Money` int(20) NOT NULL DEFAULT 0,
  `ItemID` int(20) NOT NULL DEFAULT 0,
  `ItemCount` int(5) NOT NULL DEFAULT 0,
  PRIMARY KEY (`Level`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

SET FOREIGN_KEY_CHECKS = 1;
