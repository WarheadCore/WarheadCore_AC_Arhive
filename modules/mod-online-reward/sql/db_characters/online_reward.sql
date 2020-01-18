SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for online_reward
-- ----------------------------
DROP TABLE IF EXISTS `online_reward`;
CREATE TABLE `online_reward`  (
  `RewardPlayedTime` int(20) NOT NULL DEFAULT 0,
  `ItemID` int(11) NOT NULL DEFAULT 1,
  `Count` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`RewardPlayedTime`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Table structure for online_reward_history
-- ----------------------------
DROP TABLE IF EXISTS `online_reward_history`;
CREATE TABLE `online_reward_history`  (
  `PlayedGuid` bigint(20) NOT NULL DEFAULT 0,
  `Rewarded` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `RewardedPerHour` int(20) NULL DEFAULT 0,
  PRIMARY KEY (`PlayedGuid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

SET FOREIGN_KEY_CHECKS = 1;
