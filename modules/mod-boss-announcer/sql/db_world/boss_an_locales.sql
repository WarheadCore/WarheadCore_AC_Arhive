-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = 'mod-boss-announcer'
    AND `ID` BETWEEN 1 AND 40;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            ('mod-boss-announcer', 1, 'ruRU', '|cffff0000# |cff7bbef7%s|r и его группа убили босса |cffff0000%s|r |cff7bbef7в режиме|r %s'), ('mod-boss-announcer', 2, 'ruRU', '10 об.'), ('mod-boss-announcer', 3, 'ruRU', '10 гер.'), ('mod-boss-announcer', 4, 'ruRU', '25 об.'), ('mod-boss-announcer', 5, 'ruRU', '25 гер.'),
            -- enUS
            ('mod-boss-announcer', 1, 'enUS', '|cffff0000# |cff7bbef7%s|r and his group killed boss |cffff0000%s|r |cff7bbef7in difficulty|r %s'), ('mod-boss-announcer', 2, 'enUS', '10 def.'), ('mod-boss-announcer', 3, 'enUS', '10 her.'), ('mod-boss-announcer', 4, 'enUS', '25 def.'), ('mod-boss-announcer', 5, 'enUS', '25 her.');
