-- baskup old data
DROP TABLE IF EXISTS `_online_reward`;
DROP TABLE IF EXISTS `_online_reward_history`;

CREATE TABLE `_online_reward` (
  `RewardPlayedTime` int(20) NOT NULL DEFAULT 0,
  `ItemID` int(11) NOT NULL DEFAULT 1,
  `Count` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`RewardPlayedTime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

CREATE TABLE `_online_reward_history` (
  `PlayerGuid` int(20) NOT NULL DEFAULT 0,
  `RewardedPerOnline` varchar(255) DEFAULT NULL,
  `RewardedPerHour` int(20) DEFAULT 0,
  PRIMARY KEY (`PlayerGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

CREATE TABLE IF NOT EXISTS `online_reward` (
  `RewardPlayedTime` int(20) NOT NULL DEFAULT 0,
  `ItemID` int(11) NOT NULL DEFAULT 1,
  `Count` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`RewardPlayedTime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

CREATE TABLE IF NOT EXISTS `online_reward_history` (
  `PlayerGuid` int(20) NOT NULL DEFAULT 0,
  `RewardedPerOnline` varchar(255) DEFAULT NULL,
  `RewardedPerHour` int(20) DEFAULT 0,
  PRIMARY KEY (`PlayerGuid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

INSERT INTO `_online_reward` SELECT * FROM `online_reward`;
INSERT INTO `_online_reward_history` SELECT * FROM `online_reward_history`;
