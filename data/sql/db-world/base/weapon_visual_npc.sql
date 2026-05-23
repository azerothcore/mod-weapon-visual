Set 
@NpcName = "Eron Glowpride",
@NpcSubname = "Weapon Glow Specialist",
@NpcEntry = 900000,
@NpcDisplayID = 31833,
@NpcLevel = 80;

DELETE FROM `creature_template` WHERE `entry` = @NpcEntry;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
    (@NpcEntry, 0, 0, 0, 0, 0, @NpcName, @NpcSubname, NULL, 0, @NpcLevel, @NpcLevel, 0, 35, 1, 1, 1.14286, 1, 1, 20, 1, 0, 1, 2000, 2000, 1, 1, 2, 0, 2048, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 50, 50, 1, 1, 0, 0, 1, 0, 'npc_visualweapon', 12340);


DELETE FROM `creature_template_model` WHERE `CreatureID` = @NpcEntry;
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`) VALUES
(@NpcEntry, 0, @NpcDisplayID, 1, 1);
