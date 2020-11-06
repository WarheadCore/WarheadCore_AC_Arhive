-- Set module name
SET @MODULE_NAME := 'mod-buff-command';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME
    AND `ID` BETWEEN 1 AND 2;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            (@MODULE_NAME, 1, 'ruRU', 'Команда отключена'), (@MODULE_NAME, 2, 'ruRU', 'Сейчас вы не можете этого сделать'),
            -- enUS
            (@MODULE_NAME, 1, 'enUS', 'Command is disabled'), (@MODULE_NAME, 2, 'enUS', 'You can not do it now');
