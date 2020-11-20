-- Set module name
SET @MODULE_NAME := 'mod-item-level-up';

-- ----------------------------
-- Records of string_module
-- ----------------------------

DELETE FROM `string_module`
WHERE `ModuleName` = @MODULE_NAME
    AND `ID` BETWEEN 1 AND 2;

INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`)
        VALUES
            -- ruRU
            (@MODULE_NAME, 1, 'ruRU', '|cffff0000# |cff00ff00Вы уже имеете максимально возможный уровень в игре:|r %u'), (@MODULE_NAME, 2, 'ruRU', '|cffff0000# |cff00ff00Вы получили максимальный уровень в игре:|r %u'),
            -- enUS
            (@MODULE_NAME, 1, 'enUS', '|cffff0000# |cff00ff00You already have the highest possible level in the game:|r %s'), (@MODULE_NAME, 2, 'enUS', '|cffff0000# |cff00ff00You got the maximum level in game:|r %s');
-- Add item

SET @ITEM_ID := 113874;

DELETE FROM `item_template`
WHERE `entry` = @ITEM_ID;

INSERT INTO `item_template`
        VALUES (@ITEM_ID, 0, 0, - 1, 'Level-Up Token', 46787, 6, 0, 0, 1, 0, 0, 0, - 1, - 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 18282, 0, 0, 0, 8000, 0, - 1, 0, 0, 0, 0, - 1, 0, - 1, 0, 0, 0, 0, - 1, 0, - 1, 0, 0, 0, 0, - 1, 0, - 1, 0, 0, 0, 0, - 1, 0, - 1, 0, '|cff00FF00Use: Instantly levels your character to the next level.|r', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, - 1, 0, 0, 0, 0, 'ItemLevelUp_Item', 0, 0, 0, 0, 0, - 4);
