-- Set module name
SET @MODULE_NAME := 'mod-notify-muted';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME
    AND `ID` BETWEEN 1 AND 1;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            (@MODULE_NAME, 1, 'ruRU', '|cffff0000# |cff00ff00Игрок|r %s |cff00ff00не сможет говорить ещё:|r %s'),
            -- enUS
            (@MODULE_NAME, 1, 'enUS', '|cffff0000# |cff00ff00Playe|r %s |cff00ff00will not be able to speak yet:|r %s');
