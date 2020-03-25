-- ----------------------------
-- Table structure for anti_ad_messages
-- ----------------------------
DROP TABLE IF EXISTS `anti_ad_messages`;
CREATE TABLE `anti_ad_messages`  (
  `message` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`message`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of anti_ad_messages
-- ----------------------------
INSERT INTO `anti_ad_messages` VALUES ('(w)(o)(w)');
INSERT INTO `anti_ad_messages` VALUES ('.com');
INSERT INTO `anti_ad_messages` VALUES ('.eu');
INSERT INTO `anti_ad_messages` VALUES ('.fr');
INSERT INTO `anti_ad_messages` VALUES ('.info');
INSERT INTO `anti_ad_messages` VALUES ('.net');
INSERT INTO `anti_ad_messages` VALUES ('.org');
INSERT INTO `anti_ad_messages` VALUES ('.ru');
INSERT INTO `anti_ad_messages` VALUES ('.zapto');
INSERT INTO `anti_ad_messages` VALUES ('.рф');
INSERT INTO `anti_ad_messages` VALUES ('://');
INSERT INTO `anti_ad_messages` VALUES ('http');
INSERT INTO `anti_ad_messages` VALUES ('no-ip');
INSERT INTO `anti_ad_messages` VALUES ('w-o-w');
INSERT INTO `anti_ad_messages` VALUES ('wow');
INSERT INTO `anti_ad_messages` VALUES ('www');

-- ----------------------------
-- Records of string_module
-- ----------------------------
INSERT INTO `string_module` VALUES ('mod-anti-ad', 1, 'enUS', '|cFFFF0000[AntiAD]:|r %s |cff6C8CD5said a bad word:|r %s');
INSERT INTO `string_module` VALUES ('mod-anti-ad', 2, 'enUS', '|cFFFF0000[AntiAD]:|r %s |cff6C8CD5wanted to say:|r %s');
INSERT INTO `string_module` VALUES ('mod-anti-ad', 3, 'enUS', '|cFFFF0000[AntiAD]:|cff6C8CD5 You chat muted on |r %u |cff6C8CD5minutes.');
INSERT INTO `string_module` VALUES ('mod-anti-ad', 1, 'ruRU', '|cFFFF0000[Антиреклама]:|r %s |cff6C8CD5сказал запрещённое слово:|r %s');
INSERT INTO `string_module` VALUES ('mod-anti-ad', 2, 'ruRU', '|cFFFF0000[Антиреклама]:|r %s |cff6C8CD5хотел сказать:|r %s');
INSERT INTO `string_module` VALUES ('mod-anti-ad', 3, 'ruRU', '|cFFFF0000[Антиреклама]:|cff6C8CD5 Ваш чат заблокирован на|r %u |cff6C8CD5минут.');
