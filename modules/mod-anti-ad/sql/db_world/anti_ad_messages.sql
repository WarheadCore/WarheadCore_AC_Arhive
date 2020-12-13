-- ----------------------------
-- Table structure for anti_ad_messages
-- ----------------------------

DROP TABLE IF EXISTS `anti_ad_messages`;

-- ----------------------------
-- Table structure for anti_ad_patterns
-- ----------------------------

DROP TABLE IF EXISTS `anti_ad_patterns`;

CREATE TABLE `anti_ad_patterns` (
    `Pattern` varchar(255) CHARACTER
    SET utf8 COLLATE utf8_general_ci NOT NULL,
    PRIMARY KEY (`Pattern`)
    USING BTREE) ENGINE = InnoDB CHARACTER
SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of anti_ad_patterns
-- ----------------------------

INSERT INTO `anti_ad_patterns` (`Pattern`)
        VALUES ('?:[-a-z0-9@:%_\\+~.#=]{2,256}\\.)?([-a-z0-9@:%_\\+~#=]*)\\.[a-z]{2,6}\\b(?:[-a-z0-9@:%_\\+.~#?&\\/\\/=]*'), ('м\\s*а\\s*м\\s*у'), ('м\\s*а\\s*т\\s*ь'), ('w\\s*o\\s*w');
-- Set module name

SET @MODULE_NAME := 'mod-anti-ad';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME;

INSERT INTO `string_module`
        VALUES (@MODULE_NAME, 1, 'enUS', '|cFFFF0000[AntiAD]:|r %s |cff6C8CD5wanted to say:|r %s'), (@MODULE_NAME, 2, 'enUS', '|cFFFF0000[AntiAD]:|cff6C8CD5 You chat muted on |r %u |cff6C8CD5minutes.'), (@MODULE_NAME, 1, 'ruRU', '|cFFFF0000[Антиреклама]:|r %s |cff6C8CD5хотел сказать:|r %s'), (@MODULE_NAME, 2, 'ruRU', '|cFFFF0000[Антиреклама]:|cff6C8CD5 Ваш чат заблокирован на|r %u |cff6C8CD5минут.');
