CREATE TABLE IF NOT EXISTS `mod_weapon_visual_enchantments` (
    `item_guid` INT(10) UNSIGNED NOT NULL,
    `enchant_id` INT(10) UNSIGNED NOT NULL,
    PRIMARY KEY(`item_guid`))
    COMMENT = 'Stores the enchant IDs for the visuals of mod-weapon-visual'
    DEFAULT CHARSET = utf8
    ENGINE = InnoDB;
