-- Set module name
SET @MODULE_NAME := 'mod-level-reward';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME
    AND `ID` BETWEEN 1 AND 3;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            (@MODULE_NAME, 1, 'ruRU', 'Награда за повышение уровня до %u'), (@MODULE_NAME, 2, 'ruRU', 'Вы повысили свой уровень до %u и получаете награду!'), (@MODULE_NAME, 3, 'ruRU', '|cffff0000#|r |cff7bbef7Вы повысили свой уровень до|r %u |cff7bbef7и получаете награду!|r'),
            -- enUS
            (@MODULE_NAME, 1, 'enUS', 'Reward for level up to %u'), (@MODULE_NAME, 2, 'enUS', 'You increased level to %u and get a reward!'), (@MODULE_NAME, 3, 'enUS', '|cffff0000#|r |cff7bbef7You increased level to|r %u |cff7bbef7and get a reward!|r');
