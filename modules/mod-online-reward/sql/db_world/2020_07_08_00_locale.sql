-- Add locales
DELETE FROM `string_module` WHERE `ModuleName` = 'mod-online-reward' AND `ID` BETWEEN 1 AND 3;
INSERT INTO `string_module` (`ModuleName`, `ID`, `Locale`, `Text`) VALUES 

-- enUS
('mod-online-reward', 1, 'enUS', 'Reward for online %s'),
('mod-online-reward', 2, 'enUS', 'Hi, %s!\nYou been playing on our server for over %s. Please accept a gift from us.'),
('mod-online-reward', 3, 'enUS', 'You were rewarded for online (%s). You can get the award at the post office.'),

-- ruRU
('mod-online-reward', 1, 'ruRU', 'Награда за онлайн %s'),
('mod-online-reward', 2, 'ruRU', 'Привет, %s!\nВы играете на нашем сервере уже более %s. Пожалуйста примите подарок'),
('mod-online-reward', 3, 'ruRU', 'Вы были вознаграждены за онлайн (%s). Получить награду можно на почте.');
