DELETE FROM spell_group WHERE id=1500;
INSERT INTO spell_group (Id, spell_id) VALUES
(1500, 14204),
(1500, 14203),
(1500, 14202),
(1500, 14201),
(1500, 12880),
(1500, 57521),
(1500, 57520),
(1500, 57519),
(1500, 57518),
(1500, 57522);
 
DELETE FROM spell_group_stack_rules WHERE group_id=1500;
INSERT INTO spell_group_stack_rules (group_id, stack_rule) VALUES
(1500, 1);
