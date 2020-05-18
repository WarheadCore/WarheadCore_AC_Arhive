-- ----------------------------
-- Table structure for mail_external
-- ----------------------------
DROP TABLE IF EXISTS `mail_external`;
CREATE TABLE `mail_external`  (
  `ID` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `PlayerName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '1',
  `Subject` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT 'Support Message',
  `Message` varchar(500) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT 'Support Message',
  `Money` int(20) UNSIGNED NOT NULL DEFAULT 0,
  `ItemID` int(20) UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount` int(20) UNSIGNED NOT NULL DEFAULT 1,
  `CreatureEntry` int(20) UNSIGNED NOT NULL DEFAULT 95008,
  `SystemComment` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 501 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;
