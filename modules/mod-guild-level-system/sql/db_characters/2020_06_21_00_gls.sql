-- ----------------------------
-- Table structure for gls_criteria
-- ----------------------------
DROP TABLE IF EXISTS `gls_criteria`;
CREATE TABLE `gls_criteria`  (
  `CriteriaID` int(10) NOT NULL AUTO_INCREMENT,
  `StageID` int(10) NOT NULL DEFAULT 1,
  `ListItemID` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `ListItemCount` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `MinPlayersCount` int(10) NOT NULL,
  `Coef` float(10, 5) NOT NULL DEFAULT 1.00000,
  `ListRewardSpells` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`CriteriaID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_criteria
-- ----------------------------
INSERT INTO `gls_criteria` VALUES (1, 1, '37711, 40265, 37711', '100, 2, 2000', 1, 0.40000, '26662, 62555');
INSERT INTO `gls_criteria` VALUES (2, 2, '37711', '100000', 1, 0.70000, '50494');

-- ----------------------------
-- Table structure for gls_criteria_progress
-- ----------------------------
DROP TABLE IF EXISTS `gls_criteria_progress`;
CREATE TABLE `gls_criteria_progress`  (
  `GuildID` int(10) NOT NULL DEFAULT 0,
  `CriteriaID` int(10) NOT NULL DEFAULT 0,
  `ItemCount` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `SelectedSpell` int(10) NOT NULL,
  `IsDone` tinyint(1) NOT NULL,
  PRIMARY KEY (`GuildID`, `CriteriaID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_criteria_progress
-- ----------------------------

-- ----------------------------
-- Table structure for gls_stages_progress
-- ----------------------------
DROP TABLE IF EXISTS `gls_stages_progress`;
CREATE TABLE `gls_stages_progress`  (
  `GuildID` int(10) NOT NULL DEFAULT 1,
  `StageID` int(10) NOT NULL DEFAULT 1,
  PRIMARY KEY (`GuildID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of gls_stages_progress
-- ----------------------------
