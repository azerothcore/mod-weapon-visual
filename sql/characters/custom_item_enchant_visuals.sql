CREATE TABLE IF NOT EXISTS `custom_item_enchant_visuals` (
    `iguid` INT(10) UNSIGNED NOT NULL COMMENT 'item DB guid',
    `display` INT(10) UNSIGNED NOT NULL COMMENT 'enchantID',
    `PlayerName` varchar(50) NOT NULL,
    PRIMARY KEY(`iguid`))
    COMMENT = 'Stores the enchant IDs for the visuals of mod-weapon-visual'
    DEFAULT CHARSET = utf8
    ENGINE = InnoDB;
