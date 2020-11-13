-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = 'mod-boss-announcer'
    AND `ID` BETWEEN 1 AND 6;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            ('mod-boss-announcer', 1, 'ruRU', '|cffff0000# |cff7bbef7%s|r и его группа убили босса |cffff0000%s|r в режиме |cffff0000%s|r'), ('mod-boss-announcer', 2, 'ruRU', '|cffff0000# |cff7bbef7%s|r и его группа убили мирового босса |cffff0000%s|r'), ('mod-boss-announcer', 3, 'ruRU', '10 об.'), ('mod-boss-announcer', 4, 'ruRU', '10 гер.'), ('mod-boss-announcer', 5, 'ruRU', '25 об.'), ('mod-boss-announcer', 6, 'ruRU', '25 гер.'),
            -- enUS
            ('mod-boss-announcer', 1, 'enUS', '|cffff0000# |cff7bbef7%s|r and his group killed boss |cffff0000%s|r in difficulty |cffff0000%s|r'), ('mod-boss-announcer', 2, 'enUS', '|cffff0000# |cff7bbef7%s|r and his group killed world boss |cffff0000%s|r'), ('mod-boss-announcer', 3, 'enUS', '10 def.'), ('mod-boss-announcer', 4, 'enUS', '10 her.'), ('mod-boss-announcer', 5, 'enUS', '25 def.'), ('mod-boss-announcer', 6, 'enUS', '25 her.');
