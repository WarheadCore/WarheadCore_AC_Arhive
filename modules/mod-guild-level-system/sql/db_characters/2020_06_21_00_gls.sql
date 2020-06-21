-- ----------------------------
-- Table structure for gls_exp_for_level
-- ----------------------------
DROP TABLE IF EXISTS `gls_exp_for_level`;
CREATE TABLE `gls_exp_for_level`  (
  `Level` int(10) NOT NULL DEFAULT 0,
  `Exp` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`Level`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_exp_for_level
-- ----------------------------
INSERT INTO `gls_exp_for_level` VALUES (1, 100);
INSERT INTO `gls_exp_for_level` VALUES (2, 200);
INSERT INTO `gls_exp_for_level` VALUES (3, 400);
INSERT INTO `gls_exp_for_level` VALUES (4, 800);
INSERT INTO `gls_exp_for_level` VALUES (5, 1000);

-- ----------------------------
-- Table structure for gls_guild_level
-- ----------------------------
DROP TABLE IF EXISTS `gls_guild_level`;
CREATE TABLE `gls_guild_level`  (
  `GuildID` int(10) NOT NULL DEFAULT 0,
  `Level` int(10) NOT NULL DEFAULT 0,
  `Exp` int(10) NOT NULL DEFAULT 0,
  PRIMARY KEY (`GuildID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_guild_level
-- ----------------------------
INSERT INTO `gls_guild_level` SELECT `guildid`, 1, 0 FROM `guild`;

-- ----------------------------
-- Table structure for gls_guild_setting
-- ----------------------------
DROP TABLE IF EXISTS `gls_guild_setting`;
CREATE TABLE `gls_guild_setting`  (
  `GuildID` int(10) NOT NULL DEFAULT 0,
  `Flag` int(255) NOT NULL DEFAULT 0,
  PRIMARY KEY (`GuildID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_guild_setting
-- ----------------------------
INSERT INTO `gls_guild_setting` SELECT `guildid`, 0 FROM `guild`;

-- ----------------------------
-- Table structure for gls_invested
-- ----------------------------
DROP TABLE IF EXISTS `gls_invested`;
CREATE TABLE `gls_invested`  (
  `GuildID` int(10) NOT NULL,
  `PlayerName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `InvestExp` int(10) NOT NULL,
  `UnixDate` int(10) NOT NULL,
  PRIMARY KEY (`GuildID`, `PlayerName`, `InvestExp`, `UnixDate`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for gls_reward_spell
-- ----------------------------
DROP TABLE IF EXISTS `gls_reward_spell`;
CREATE TABLE `gls_reward_spell`  (
  `GuildLevel` int(10) NOT NULL DEFAULT 0,
  `ListSpells` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`GuildLevel`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
