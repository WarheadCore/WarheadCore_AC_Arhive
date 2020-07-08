-- ----------------------------
-- Table structure for online_reward
-- ----------------------------
DROP TABLE IF EXISTS `online_reward`;
CREATE TABLE `online_reward` (
  `RewardPlayedTime` int(20) NOT NULL DEFAULT 0,
  `ItemID` int(11) NOT NULL DEFAULT 1,
  `Count` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`RewardPlayedTime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Table structure for online_reward_history
-- ----------------------------
DROP TABLE IF EXISTS  `online_reward_history`;
CREATE TABLE `online_reward_history` (
  `PlayerGuid` int(20) NOT NULL DEFAULT 0,
  `RewardedPerOnline` varchar(255) DEFAULT NULL,
  `RewardedPerHour` int(20) DEFAULT 0,
  PRIMARY KEY (`PlayerGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

