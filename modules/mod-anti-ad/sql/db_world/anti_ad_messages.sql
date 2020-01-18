SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

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

SET FOREIGN_KEY_CHECKS = 1;
