CREATE TABLE IF NOT EXISTS `mod_weapon_visual__enchantments` (
    `item_guid` INT(10) UNSIGNED NOT NULL COMMENT 'item DB guid',
    `display` INT(10) UNSIGNED NOT NULL COMMENT 'enchantID',
    PRIMARY KEY(`item_guid`))
    COMMENT = 'Stores the enchant IDs for the visuals of mod-weapon-visual'
    DEFAULT CHARSET = utf8
    ENGINE = InnoDB;
