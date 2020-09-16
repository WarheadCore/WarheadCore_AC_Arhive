-- Solo queue
-- NPC
-- example 19925

UPDATE
    creature_template
SET
    scriptname = 'ArenaSolo_Creature'
WHERE
    entry IN (
        SELECT
            entry
        FROM
            battlemaster_entry
        WHERE
            bg_template = 6);
