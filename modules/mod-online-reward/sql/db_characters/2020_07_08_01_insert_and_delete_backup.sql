-- Insert data from backup
INSERT INTO `online_reward` SELECT * FROM `_online_reward`;
INSERT INTO `online_reward_history` SELECT * FROM `_online_reward_history`;

-- Delete backup
DROP TABLE IF EXISTS `_online_reward`;
DROP TABLE IF EXISTS `_online_reward_history`;
