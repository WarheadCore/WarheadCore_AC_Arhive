-- Set module name
SET @MODULE_NAME := 'mod-player-info-at-login';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME
    AND `ID` BETWEEN 1 AND 6;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            (@MODULE_NAME, 1, 'ruRU', '|cffff0000#|r --'), 
            (@MODULE_NAME, 2, 'ruRU', '|cffff0000# |cff00ff00Привет,|r %s!'),
            (@MODULE_NAME, 3, 'ruRU', '|cffff0000# |cff00ff00Ваш уровень аккаунта:|r %u'),
            (@MODULE_NAME, 4, 'ruRU', '|cffff0000# |cff00ff00Ваш айпи:|r %s'),
            (@MODULE_NAME, 5, 'ruRU', '|cffff0000# |cff00ff00Сейчас|r %u |cff00ff00игроков онлайн|r |cff00ff00(максимум:|r %u|cff00ff00)|r'),
            (@MODULE_NAME, 6, 'ruRU', '|cffff0000# |cff00ff00Время работы мира:|r %s'),
            -- enUS
            (@MODULE_NAME, 1, 'enUS', '|cffff0000#|r --'),
            (@MODULE_NAME, 2, 'enUS', '|cffff0000# |cff00ff00Hi,|r %s'),
            (@MODULE_NAME, 3, 'enUS', '|cffff0000# |cff00ff00You account level:|r %u'),
            (@MODULE_NAME, 4, 'enUS', '|cffff0000# |cff00ff00You IP:|r %s'),
            (@MODULE_NAME, 5, 'enUS', '|cffff0000# |cff00ff00Now|r %u |cff00ff00players online|r |cff00ff00(max:|r %u|cff00ff00)|r'),
            (@MODULE_NAME, 6, 'enUS', '|cffff0000# |cff00ff00Server uptime:|r %s');
