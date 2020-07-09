-- ----------------------------
-- Table structure for ips_shop
-- ----------------------------
DROP TABLE IF EXISTS `ips_shop`;
CREATE TABLE `ips_shop`  (
  `ID` int(10) NOT NULL DEFAULT 1,
  `Type` int(10) NOT NULL DEFAULT 0,
  `Value` int(10) NOT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of ips_shop
-- ----------------------------
INSERT INTO `ips_shop` VALUES (9, 0, 37711);
INSERT INTO `ips_shop` VALUES (10, 1, 0);
