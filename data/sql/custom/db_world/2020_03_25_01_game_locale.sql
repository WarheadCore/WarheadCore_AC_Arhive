-- ----------------------------
-- Table structure for string_class
-- ----------------------------
DROP TABLE IF EXISTS `string_class`;
CREATE TABLE `string_class`  (
  `ID` mediumint(8) UNSIGNED NOT NULL DEFAULT 0,
  `locale` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `NameMale` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `NameFemale` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`, `locale`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of string_class
-- ----------------------------
INSERT INTO `string_class` VALUES (1, 'ruRU', 'Воин', NULL);
INSERT INTO `string_class` VALUES (2, 'ruRU', 'Паладин', NULL);
INSERT INTO `string_class` VALUES (3, 'ruRU', 'Охотник', 'Охотница');
INSERT INTO `string_class` VALUES (4, 'ruRU', 'Разбойник', 'Разбойница');
INSERT INTO `string_class` VALUES (5, 'ruRU', 'Жрец', 'Жрица');
INSERT INTO `string_class` VALUES (6, 'ruRU', 'Рыцарь смерти', NULL);
INSERT INTO `string_class` VALUES (7, 'ruRU', 'Шаман', 'Шаманка');
INSERT INTO `string_class` VALUES (8, 'ruRU', 'Маг', NULL);
INSERT INTO `string_class` VALUES (9, 'ruRU', 'Чернокнижник', 'Чернокнижница');
INSERT INTO `string_class` VALUES (11, 'ruRU', 'Друид', NULL);
INSERT INTO `string_class` VALUES (1, 'enUS', 'Warrior', NULL);
INSERT INTO `string_class` VALUES (2, 'enUS', 'Paladin', NULL);
INSERT INTO `string_class` VALUES (3, 'enUS', 'Hunter', NULL);
INSERT INTO `string_class` VALUES (4, 'enUS', 'Rogue', NULL);
INSERT INTO `string_class` VALUES (5, 'enUS', 'Priest', NULL);
INSERT INTO `string_class` VALUES (6, 'enUS', 'DeathKnight', NULL);
INSERT INTO `string_class` VALUES (7, 'enUS', 'Shaman', NULL);
INSERT INTO `string_class` VALUES (8, 'enUS', 'Mage', NULL);
INSERT INTO `string_class` VALUES (9, 'enUS', 'Warlock', NULL);
INSERT INTO `string_class` VALUES (11, 'enUS', 'Druid', NULL);

-- ----------------------------
-- Table structure for string_module
-- ----------------------------
DROP TABLE IF EXISTS `string_module`;
CREATE TABLE `string_module`  (
  `ModuleName` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'system',
  `ID` mediumint(8) UNSIGNED NOT NULL DEFAULT 0,
  `Locale` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `Text` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ModuleName`, `ID`, `Locale`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for string_race
-- ----------------------------
DROP TABLE IF EXISTS `string_race`;
CREATE TABLE `string_race`  (
  `ID` mediumint(8) UNSIGNED NOT NULL DEFAULT 0,
  `locale` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `NameMale` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `NameFemale` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`, `locale`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of string_race
-- ----------------------------
INSERT INTO `string_race` VALUES (2, 'ruRU', 'Орк', NULL);
INSERT INTO `string_race` VALUES (1, 'ruRU', 'Человек', NULL);
INSERT INTO `string_race` VALUES (3, 'ruRU', 'Дворф', NULL);
INSERT INTO `string_race` VALUES (4, 'ruRU', 'Ночной эльф', 'Ночная эльфийка');
INSERT INTO `string_race` VALUES (5, 'ruRU', 'Нежить', NULL);
INSERT INTO `string_race` VALUES (6, 'ruRU', 'Таурен', NULL);
INSERT INTO `string_race` VALUES (7, 'ruRU', 'Гном', NULL);
INSERT INTO `string_race` VALUES (8, 'ruRU', 'Тролль', NULL);
INSERT INTO `string_race` VALUES (9, 'ruRU', 'Гоблин', NULL);
INSERT INTO `string_race` VALUES (10, 'ruRU', 'Эльф крови', 'Эльфийка крови');
INSERT INTO `string_race` VALUES (11, 'ruRU', 'Дреней', NULL);
INSERT INTO `string_race` VALUES (1, 'enUS', 'Human', NULL);
INSERT INTO `string_race` VALUES (2, 'enUS', 'Orc', NULL);
INSERT INTO `string_race` VALUES (3, 'enUS', 'Dwarf', NULL);
INSERT INTO `string_race` VALUES (4, 'enUS', 'NightElf', NULL);
INSERT INTO `string_race` VALUES (5, 'enUS', 'Scourge', NULL);
INSERT INTO `string_race` VALUES (6, 'enUS', 'Tauren', NULL);
INSERT INTO `string_race` VALUES (7, 'enUS', 'Gnome', NULL);
INSERT INTO `string_race` VALUES (8, 'enUS', 'Troll', NULL);
INSERT INTO `string_race` VALUES (9, 'enUS', 'Goblin', NULL);
INSERT INTO `string_race` VALUES (10, 'enUS', 'BloodElf', NULL);
INSERT INTO `string_race` VALUES (11, 'enUS', 'Draenei', NULL);
