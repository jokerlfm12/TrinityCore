UPDATE `quest_template_addon` SET `PrevQuestID` = '12720', `ExclusiveGroup` = '-12716' WHERE (`ID` = '12723');
UPDATE `gossip_menu_option` SET `OptionType` = '3' WHERE (`MenuId` = '9755') and (`OptionIndex` = '0');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12751' WHERE (`ID` = '12754');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12723' WHERE (`ID` = '12725');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12739');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12742');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12743');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12744');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12745');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12746');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12747');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12748');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12749');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '12750');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '28649');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12738' WHERE (`ID` = '28650');
UPDATE `quest_template` SET `AllowableRaces` = '1' WHERE (`ID` = '12742');
UPDATE `quest_template` SET `AllowableRaces` = '8' WHERE (`ID` = '12743');
UPDATE `quest_template` SET `AllowableRaces` = '4' WHERE (`ID` = '12744');
UPDATE `quest_template` SET `AllowableRaces` = '512' WHERE (`ID` = '12747');
UPDATE `quest_template` SET `AllowableRaces` = '128' WHERE (`ID` = '12749');
UPDATE `quest_template` SET `AllowableRaces` = '2097152' WHERE (`ID` = '28649');
UPDATE `creature_queststarter` SET `id` = '29110' WHERE (`id` = '25462') and (`quest` = '12779');
UPDATE `creature_questender` SET `id` = '29110' WHERE (`id` = '25462') and (`quest` = '12779');
UPDATE `quest_template_addon` SET `PrevQuestID` = '12800' WHERE (`ID` = '12801');
DELETE FROM `gossip_menu_option` WHERE (`MenuId` = '10027') and (`OptionIndex` = '1');
DELETE FROM `gossip_menu_option` WHERE (`MenuId` = '10027') and (`OptionIndex` = '2');
UPDATE `gossip_menu_option` SET `OptionType` = '3' WHERE (`MenuId` = '12759') and (`OptionIndex` = '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('10400', '0', '0', '0', '10395', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('10079', '0', '0', '0', '10142', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7865', '9632', '0', '0', '8', '0', '10099', '0', '0', '0', '0', '0', '', '');
INSERT INTO `gossip_menu_option_action` (`MenuId`, `OptionIndex`, `ActionMenuId`, `ActionPoiId`) 
SELECT menuid, 2, 10371, 0 FROM gossip_menu_option where optiontype = 20 and optionindex = 2 and menuid not in (SELECT menuid FROM gossip_menu_option_action where actionmenuid = 10371);
UPDATE `quest_template_addon` SET `PrevQuestID` = '10144' WHERE (`ID` = '10146');
DELETE FROM `gossip_menu_option` WHERE (`MenuId` = '7970') and (`OptionIndex` = '2');
DELETE FROM `gossip_menu_option` WHERE (`MenuId` = '7970') and (`OptionIndex` = '3');
UPDATE `conditions` SET `NegativeCondition` = '0' WHERE (`SourceTypeOrReferenceId` = '15') and (`SourceGroup` = '7970') and (`SourceEntry` = '1') and (`SourceId` = '0') and (`ElseGroup` = '0') and (`ConditionTypeOrReference` = '8') and (`ConditionTarget` = '0') and (`ConditionValue1` = '10340') and (`ConditionValue2` = '0') and (`ConditionValue3` = '0');
UPDATE `conditions` SET `ConditionValue1` = '10146' WHERE (`SourceTypeOrReferenceId` = '15') and (`SourceGroup` = '7970') and (`SourceEntry` = '1') and (`SourceId` = '0') and (`ElseGroup` = '0') and (`ConditionTypeOrReference` = '8') and (`ConditionTarget` = '0') and (`ConditionValue1` = '10340') and (`ConditionValue2` = '0') and (`ConditionValue3` = '0');
UPDATE `smart_scripts` SET `target_type` = '2' WHERE (`entryorguid` = '19424') and (`source_type` = '0') and (`id` = '0') and (`link` = '0');
UPDATE `mail_level_reward` SET `level` = '70' WHERE (`level` = '60') and (`raceMask` = '690');
UPDATE `mail_level_reward` SET `level` = '70' WHERE (`level` = '60') and (`raceMask` = '1101');
UPDATE `trainer_spell` SET `ReqLevel` = '70' WHERE (`TrainerId` = '39') and (`SpellId` = '33950');
UPDATE `trainer_spell` SET `ReqLevel` = '70' WHERE (`TrainerId` = '386') and (`SpellId` = '34092');
UPDATE `trainer_spell` SET `ReqLevel` = '70' WHERE (`TrainerId` = '137') and (`SpellId` = '34092');
UPDATE `trainer_spell` SET `ReqLevel` = '85' WHERE (`TrainerId` = '386') and (`SpellId` = '90269');
UPDATE `trainer_spell` SET `ReqLevel` = '85' WHERE (`TrainerId` = '137') and (`SpellId` = '90269');
UPDATE `trainer_spell` SET `ReqLevel` = '80' WHERE (`TrainerId` = '386') and (`SpellId` = '54198');
UPDATE `trainer_spell` SET `ReqLevel` = '80' WHERE (`TrainerId` = '137') and (`SpellId` = '54198');
UPDATE `trainer_spell` SET `ReqLevel` = '70' WHERE (`TrainerId` = '386') and (`SpellId` = '90266');
UPDATE `trainer_spell` SET `ReqLevel` = '70' WHERE (`TrainerId` = '137') and (`SpellId` = '90266');
UPDATE `smart_scripts` SET `event_flags` = '1' WHERE (`entryorguid` = '19440') and (`source_type` = '0') and (`id` = '0') and (`link` = '0');
UPDATE `smart_scripts` SET `event_flags` = '0' WHERE (`entryorguid` = '19440') and (`source_type` = '0') and (`id` = '1') and (`link` = '0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('19440', '0', '2', '0', '0', '0', '100', '0', '1000', '1500', '0', '0', '0', '11', '40861', '0', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'lfm - Eye of Grillok - Wicked Beam');
UPDATE `trainer_spell` SET `MoneyCost` = '400000' WHERE (`TrainerId` = '386') and (`SpellId` = '33389');
UPDATE `trainer_spell` SET `MoneyCost` = '400000' WHERE (`TrainerId` = '137') and (`SpellId` = '33389');
UPDATE `trainer_spell` SET `MoneyCost` = '400000' WHERE (`TrainerId` = '46') and (`SpellId` = '33389');
UPDATE `trainer_spell` SET `MoneyCost` = '5000000' WHERE (`TrainerId` = '386') and (`SpellId` = '33392');
UPDATE `trainer_spell` SET `MoneyCost` = '5000000' WHERE (`TrainerId` = '137') and (`SpellId` = '33392');
UPDATE `trainer_spell` SET `MoneyCost` = '5000000' WHERE (`TrainerId` = '46') and (`SpellId` = '33392');
UPDATE `trainer_spell` SET `MoneyCost` = '50000000', `ReqLevel` = '60' WHERE (`TrainerId` = '39') and (`SpellId` = '33950');
UPDATE `trainer_spell` SET `MoneyCost` = '50000000', `ReqLevel` = '60' WHERE (`TrainerId` = '386') and (`SpellId` = '34092');
UPDATE `trainer_spell` SET `MoneyCost` = '50000000', `ReqLevel` = '60' WHERE (`TrainerId` = '137') and (`SpellId` = '34092');
UPDATE `trainer_spell` SET `MoneyCost` = '100000000', `ReqLevel` = '70' WHERE (`TrainerId` = '386') and (`SpellId` = '34093');
UPDATE `trainer_spell` SET `MoneyCost` = '100000000', `ReqLevel` = '70' WHERE (`TrainerId` = '137') and (`SpellId` = '34093');
UPDATE `trainer_spell` SET `MoneyCost` = '150000000', `ReqLevel` = '80' WHERE (`TrainerId` = '386') and (`SpellId` = '90266');
UPDATE `trainer_spell` SET `MoneyCost` = '150000000', `ReqLevel` = '80' WHERE (`TrainerId` = '137') and (`SpellId` = '90266');
UPDATE `trainer_spell` SET `MoneyCost` = '100000000', `ReqLevel` = '68' WHERE (`TrainerId` = '386') and (`SpellId` = '54198');
UPDATE `trainer_spell` SET `MoneyCost` = '100000000', `ReqLevel` = '68' WHERE (`TrainerId` = '137') and (`SpellId` = '54198');
UPDATE `trainer_spell` SET `MoneyCost` = '150000000', `ReqLevel` = '60' WHERE (`TrainerId` = '386') and (`SpellId` = '90269');
UPDATE `trainer_spell` SET `MoneyCost` = '150000000', `ReqLevel` = '60' WHERE (`TrainerId` = '137') and (`SpellId` = '90269');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '16');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '128');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '32');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '2');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '8');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '1');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '64');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '4');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '1024');
UPDATE `mail_level_reward` SET `level` = '90' WHERE (`level` = '20') and (`raceMask` = '512');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '512');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '1024');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '4');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '64');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '1');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '8');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '2');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '32');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '128');
UPDATE `mail_level_reward` SET `level` = '91' WHERE (`level` = '40') and (`raceMask` = '16');
UPDATE `mail_level_reward` SET `level` = '92' WHERE (`level` = '60') and (`raceMask` = '690');
UPDATE `mail_level_reward` SET `level` = '92' WHERE (`level` = '60') and (`raceMask` = '1101');
UPDATE `mail_level_reward` SET `level` = '93' WHERE (`level` = '70') and (`raceMask` = '690');
UPDATE `mail_level_reward` SET `level` = '93' WHERE (`level` = '70') and (`raceMask` = '1101');
UPDATE `smart_scripts` SET `action_param1` = '39415' WHERE (`entryorguid` = '19424') and (`source_type` = '0') and (`id` = '0') and (`link` = '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('10382', '0', '0', '0', '10163', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
insert into gameobject 
select guid, id, map, zoneid, areaid, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, scriptname, verifiedbuild 
from gameobject where id = 183123;
UPDATE `gameobject_template` SET `displayId` = '0' WHERE (`entry` = '183123');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('9543', '0', '0', '0', '9424', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
UPDATE `creature_model_info` SET `CombatReach` = '5' WHERE (`DisplayID` = '20746');
UPDATE `smart_scripts` SET `action_param1` = '30093' WHERE (`entryorguid` = '16977') and (`source_type` = '0') and (`id` = '12') and (`link` = '0');
UPDATE `creature_template` SET `npcflag` = '83' WHERE (`entry` = '21209');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('9385', '0', '0', '0', '9558', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('9563', '0', '0', '0', '9558', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
UPDATE `smart_scripts` SET `action_type` = '41' WHERE (`entryorguid` = '181582') and (`source_type` = '1') and (`id` = '1') and (`link` = '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('10132', '0', '0', '0', '10443', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('10159', '0', '0', '0', '10443', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('9912', '0', '0', '0', '10443', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('1740400', '9', '7', '0', '0', '0', '100', '0', '9000', '9000', '0', '0', '0', '37', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', 'Vindicator Sedai - Die');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('17000', '0', '0', '0', '0', '0', '100', '0', '2000', '4000', '10000', '15000', '0', '11', '21068', '32', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'corruption');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('17000', '0', '1', '0', '0', '0', '100', '0', '8000', '12000', '10000', '15000', '0', '11', '22678', '0', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'fear');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '17000');
UPDATE `smart_scripts` SET `event_flags` = '513' WHERE (`entryorguid` = '7385') and (`source_type` = '0') and (`id` = '1') and (`link` = '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('215419', '54603', '543', '3562', '3562', '3', '0', '1', '169', '0', '-1', '0', '0', '-1343.26', '1650.59', '68.9473', '0.6687', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('226493', '54629', '542', '3713', '3713', '3', '0', '1', '169', '0', '-1', '0', '0', '-0.834323', '-6.44807', '-43.1633', '4.83269', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
DELETE FROM `creature_loot_template` WHERE (`Entry` = '17537') and (`Item` = '23892');
DELETE FROM `creature_loot_template` WHERE (`Entry` = '17537') and (`Item` = '23890');
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = '1') and (`SourceGroup` = '17537') and (`SourceEntry` = '23892') and (`SourceId` = '0') and (`ElseGroup` = '0') and (`ConditionTypeOrReference` = '6') and (`ConditionTarget` = '0') and (`ConditionValue1` = '67') and (`ConditionValue2` = '0') and (`ConditionValue3` = '0');
DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = '1') and (`SourceGroup` = '17537') and (`SourceEntry` = '23890') and (`SourceId` = '0') and (`ElseGroup` = '0') and (`ConditionTypeOrReference` = '6') and (`ConditionTarget` = '0') and (`ConditionValue1` = '469') and (`ConditionValue2` = '0') and (`ConditionValue3` = '0');
insert into gameobject 
select guid, id, map, zone, area, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, '', 0 
from world_emucoach.gameobject where id = 209347 or id = 209348;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('80240', 'spell_warl_bane_of_havoc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('85466', 'spell_warl_bane_of_havoc_track');
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('29539', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2');
UPDATE `areatrigger_involvedrelation` SET `quest` = '29539' WHERE (`id` = '4200');
UPDATE `creature_template` SET `unit_flags` = '0' WHERE (`entry` = '17398');
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('17381', '73200', '0', '100', '1', '0', '1', '0', '1', '1');
UPDATE `gossip_menu_option` SET `OptionType` = '3' WHERE (`MenuId` = '7396') and (`OptionIndex` = '0');
UPDATE `gossip_menu_option` SET `OptionType` = '3' WHERE (`MenuId` = '10437') and (`OptionIndex` = '1');
UPDATE `creature_template` SET `unit_flags` = '0' WHERE (`entry` = '19733');
UPDATE `creature_template` SET `ScriptName` = 'npc_dk_ghoul' WHERE (`entry` = '26125');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64859');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64860');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64861');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64862');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64863');
UPDATE `creature` SET `spawndist` = '3', `MovementType` = '1' WHERE (`guid` = '64864');
update creature set spawndist = 3, movementtype = 1 where id = 18912 and movementtype = 0;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES ('31746', '31773', '0', 'Whisper Aura 0');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES ('-31773', '31774', '0', 'Whisper Aura 1');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES ('-31774', '31775', '0', 'Whisper Aura 2');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES ('-31775', '31776', '0', 'Whisper Aura 3');
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES ('-31776', '31777', '0', 'Whisper Aura 4');
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('11286', '0', '0', '4', '0', '0', '0', '0', '0', '0');
UPDATE `event_scripts` SET `dataint` = '14706' WHERE (`entry` = '464');
UPDATE `event_scripts` SET `command` = '10', `datalong` = '17841', `datalong2` = '100', `dataint` = '0' WHERE (`entry` = '464');
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('11289', '0', '10', '17841', '100', '0', '0', '0', '0', '0');
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('11290', '0', '10', '17841', '100', '0', '0', '0', '0', '0');
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES ('11291', '0', '10', '17841', '100', '0', '0', '0', '0', '0');
delete from creature_text where creatureid = 17841;
insert into creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 17841;
UPDATE `smart_scripts` SET `comment` = 'Ysiel Windsinger - Just summoned - invisible' WHERE (`entryorguid` = '17841') and (`source_type` = '0') and (`id` = '1') and (`link` = '0');
UPDATE `smart_scripts` SET `event_type` = '38', `event_param1` = '1', `event_param2` = '1', `comment` = 'Ysiel Windsinger - Just summoned - Say tex 0' WHERE (`entryorguid` = '17841') and (`source_type` = '0') and (`id` = '2') and (`link` = '0');
UPDATE `smart_scripts` SET `event_type` = '38', `event_param1` = '1', `event_param2` = '2', `comment` = 'Ysiel Windsinger - Just summoned - Say tex 1' WHERE (`entryorguid` = '17841') and (`source_type` = '0') and (`id` = '3') and (`link` = '0');
UPDATE `smart_scripts` SET `event_type` = '38', `event_param1` = '1', `event_param2` = '3', `comment` = 'Ysiel Windsinger - Just summoned - Say tex 2' WHERE (`entryorguid` = '17841') and (`source_type` = '0') and (`id` = '4') and (`link` = '0');
UPDATE `smart_scripts` SET `event_type` = '38', `event_param1` = '1', `event_param2` = '4', `comment` = 'Ysiel Windsinger - Just summoned - Say tex 3' WHERE (`entryorguid` = '17841') and (`source_type` = '0') and (`id` = '5') and (`link` = '0');
UPDATE `event_scripts` SET `dataint` = '1' WHERE (`entry` = '464');
UPDATE `event_scripts` SET `dataint` = '2' WHERE (`entry` = '465');
UPDATE `event_scripts` SET `dataint` = '3' WHERE (`entry` = '466');
UPDATE `event_scripts` SET `dataint` = '4' WHERE (`entry` = '467');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('18214', '0', '0', '0', '0', '0', '100', '0', '3000', '5000', '10000', '15000', '0', '11', '6016', '0', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'Fenclaw Thrasher - IC - Cast \'Pierce Armor\'');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '18214');
UPDATE `gossip_menu_option` SET `OptionIcon` = '0', `OptionBroadcastTextId` = '10693' WHERE (`MenuId` = '7481') and (`OptionIndex` = '0');
UPDATE `gossip_menu_option` SET `OptionIcon` = '5', `OptionText` = 'Make this inn your home.', `OptionBroadcastTextId` = '2822' WHERE (`MenuId` = '7481') and (`OptionIndex` = '1');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7574', '9204', '0', '0', '8', '0', '9743', '0', '0', '0', '0', '0', '', '');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7574', '9204', '0', '0', '8', '0', '9739', '0', '0', '0', '0', '0', '', '');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7574', '9204', '0', '0', '5', '0', '970', '248', '0', '0', '0', '0', '', '');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('215090', '54674', '546', '3716', '3716', '1', '0', '31', '169', '0', '-1', '0', '0', '31.4086', '-32.1966', '-2.74908', '5.777', '604800', '0', '0', '1', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('500864', '54667', '547', '3717', '3717', '1', '0', '1', '169', '0', '-1', '0', '0', '135.137', '-114.338', '-1.59057', '3.79424', '604800', '0', '0', '1', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('215088', '54675', '546', '3716', '3716', '1', '0', '31', '169', '0', '-1', '0', '0', '33.3729', '-25.4775', '-2.75236', '5.52369', '604800', '0', '0', '1', '0', '0', '0', '0', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('215200', '54848', '545', '3715', '3715', '1', '0', '31', '169', '0', '-1', '0', '0', '-0.948525', '-15.462633', '-4.962857', '4.743805', '604800', '0', '0', '1', '0', '0', '0', '0', '0', '', '0');
DELETE FROM `creature` WHERE (`id` = '17884');
UPDATE `creature_template` SET `unit_flags` = '768' WHERE (`entry` = '17885');
UPDATE `conditions` SET `ConditionValue1` = '29563', `Comment` = 'Gossip option 0 requires quest 29563 active' WHERE (`SourceTypeOrReferenceId` = '15') and (`SourceGroup` = '7520') and (`SourceEntry` = '0') and (`SourceId` = '0') and (`ElseGroup` = '0') and (`ConditionTypeOrReference` = '9') and (`ConditionTarget` = '0') and (`ConditionValue1` = '9738') and (`ConditionValue2` = '0') and (`ConditionValue3` = '0');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('15', '7540', '0', '0', '0', '1', '0', '34906', '0', '0', '1', '0', '0', '', 'Gossip option menu shows only when player does not have spell 34906');
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '17893%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '17893%';
UPDATE `creature_template` SET `gossip_menu_id` = '7520' WHERE (`entry` = '17893');
INSERT INTO `quest_offer_reward` (`ID`, `Emote1`, `Emote2`, `Emote3`, `Emote4`, `EmoteDelay1`, `EmoteDelay2`, `EmoteDelay3`, `EmoteDelay4`, `RewardText`, `VerifiedBuild`) VALUES ('29566', '0', '0', '0', '0', '0', '0', '0', '0', 'What\'re you doing all the way down here? As you can see, it isn\'t exactly safe!', '15595');
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('19895', '72118', '0', '100', '1', '0', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('17991', '72118', '0', '100', '1', '0', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('19894', '72119', '0', '100', '1', '0', '1', '0', '1', '1');
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('17942', '72119', '0', '100', '1', '0', '1', '0', '1', '1');
UPDATE `quest_template_addon` SET `PrevQuestID` = '9990' WHERE (`ID` = '9994');
UPDATE `creature_template_addon` SET `emote` = '233' WHERE (`entry` = '18595');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('18449', '0', '1', '0', '0', '0', '100', '0', '1000', '5000', '5000', '10000', '0', '11', '54185', '1', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'Shienor Talonite - In Combat - Cast \'Claw Slash\'');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('18449', '0', '0', '0', '0', '0', '100', '0', '6000', '10000', '9000', '14000', '0', '11', '11428', '1', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'Shienor Talonite - In Combat - Cast \'Knockdown\'');
UPDATE `smart_scripts` SET `event_param3` = '15000', `event_param4` = '18000', `action_param1` = '30285', `comment` = 'Shienor Talonite - In Combat - Cast \'Eagle Claw\'' WHERE (`entryorguid` = '18449') and (`source_type` = '0') and (`id` = '1') and (`link` = '0');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('19943', '0', '0', '0', '0', '0', '100', '0', '6000', '10000', '9000', '14000', '0', '11', '11428', '1', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'Lash Talonite - In Combat - Cast \'Knockdown\'');
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES ('19943', '0', '1', '0', '0', '0', '100', '0', '1000', '3000', '4000', '8000', '0', '11', '37685', '1', '0', '0', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', 'Lash Talonite - In Combat - Cast \'Backstab\'');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '18449');
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '19943');
insert into npc_vendor select entry, slot, item, maxcount, incrtime, extendedcost, 1, 0, verifiedbuild from world_trinitycore_wlk.npc_vendor where entry = 25195;
UPDATE `smart_scripts` SET `target_type` = '19', `target_param2` = '50', `target_param3` = '0' WHERE (`entryorguid` = '184725') and (`source_type` = '1') and (`id` = '2') and (`link` = '0');
UPDATE `creature_template` SET `unit_flags` = '32832', `type_flags` = '0' WHERE (`entry` = '18707');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('400000', '22441', '530', '0', '0', '1', '0', '1', '169', '0', '-1', '0', '0', '-3536.95', '4552.85', '83.9206', '1.40712', '300', '0', '0', '1', '0', '2', '0', '0', '0', '', '0');
UPDATE `world_trinitycore_ctm`.`creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '22441');
INSERT INTO `world_trinitycore_ctm`.`creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Random`) VALUES ('22441', '1', '0', '1', '0', '0');
UPDATE `world_trinitycore_ctm`.`event_scripts` SET `dataint` = '1' WHERE (`entry` = '181');
insert into world_trinitycore_ctm.waypoint_data select entry + 6000000, pointid, position_x, position_y, position_z, orientation, velocity, delay, 0, 0, 100, 0 from world_trinitycore_ctm.waypoints where entry = 22441;
UPDATE `world_trinitycore_ctm`.`creature_template` SET `DamageModifier` = '4' WHERE (`entry` = '22441');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('223859', '54840', '556', '3791', '3791', '3', '0', '1', '169', '0', '-1', '0', '0', '-0.53787', '-0.352111', '0.0062', '6.25175', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('223860', '54847', '556', '3791', '3791', '3', '0', '1', '169', '0', '-1', '0', '0', '-143.996', '213.04', '0.0107748', '4.71238', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '18956%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '18956%';
insert into waypoints select entry, pointid, position_x, position_y, position_z, orientation, 0, delay, point_comment from world_trinitycore_wlk.waypoints where entry = 18956;
UPDATE `world_trinitycore_ctm`.`quest_template` SET `RequiredNpcOrGo1` = '18956', `RequiredNpcOrGo2` = '0', `RequiredNpcOrGoCount1` = '1', `RequiredNpcOrGoCount2` = '0', `ObjectiveText1` = 'Free Lakka', `ObjectiveText2` = '' WHERE (`ID` = '29605');
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '18319%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '18319%';
INSERT INTO `world_trinitycore_ctm`.`creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('19428', '72480', '0', '50', '1', '0', '1', '0', '1', '1');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('400003', '54668', '547', '3717', '3717', '3', '0', '1', '169', '0', '-1', '0', '0', '131.382', '-104.577', '-1.59057', '3.82016', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('400001', '54692', '557', '3792', '3792', '3', '0', '1', '169', '0', '-1', '0', '0', '-13.5759', '7.64209', '-0.954327', '4.39333', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('400002', '54694', '557', '3792', '3792', '3', '0', '1', '169', '0', '-1', '0', '0', '-14.5759', '7.64209', '-0.954327', '4.39333', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');
INSERT INTO `world_trinitycore_ctm`.`creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `IsCurrency`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES ('18343', '72127', '0', '100', '1', '0', '1', '0', '1', '1');
INSERT INTO `world_trinitycore_ctm`.`conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7891', '9654', '0', '0', '8', '0', '10109', '0', '0', '1', '0', '0', '', 'Group 0: Show gossip text 9654 if quest \'I Must Have Them!\' is not rewarded');
INSERT INTO `world_trinitycore_ctm`.`conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('14', '7891', '9659', '0', '0', '8', '0', '10109', '0', '0', '0', '0', '0', '', 'Group 0: Show gossip text 9659 if quest \'I Must Have Them!\' is rewarded');
INSERT INTO `world_trinitycore_ctm`.`conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('15', '7891', '0', '0', '0', '8', '0', '10109', '0', '0', '0', '0', '0', '', 'Group 0: Show Gossip Option 0 if quest \'I Must Have Them!\' is rewarded');
INSERT INTO `world_trinitycore_ctm`.`conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('15', '7891', '0', '0', '0', '2', '0', '27808', '1', '1', '1', '0', '0', '', 'Group 0: Show Gossip Option 0 if player does not have item \'Jump-a-tron 4000 Key\'');
DELETE FROM `world_trinitycore_ctm`.`creature` WHERE (`guid` = '86598');
DELETE FROM `world_trinitycore_ctm`.`creature` WHERE (`guid` = '86599');
UPDATE `world_trinitycore_ctm`.`creature` SET `MovementType` = '1' WHERE (`guid` = '65624');
UPDATE `world_trinitycore_ctm`.`creature` SET `MovementType` = '1' WHERE (`guid` = '86752');
UPDATE `world_trinitycore_ctm`.`creature` SET `spawndist` = '5' WHERE (`guid` = '65624');
UPDATE `world_trinitycore_ctm`.`creature` SET `spawndist` = '5' WHERE (`guid` = '86752');
UPDATE `world_trinitycore_ctm`.`creature` SET `spawndist` = '0', `MovementType` = '0' WHERE (`guid` = '65624');
UPDATE `world_trinitycore_ctm`.`creature` SET `spawndist` = '0', `MovementType` = '0' WHERE (`guid` = '86752');
UPDATE `world_trinitycore_ctm`.`creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '18297');
insert into world_trinitycore_ctm.waypoints select entry, pointid, position_x, position_y, position_z, orientation, 0, delay, point_comment from world_trinitycore_wlk.waypoints where entry = 18297;
insert into world_trinitycore_ctm.waypoints select entry, pointid, position_x, position_y, position_z, orientation, 0, delay, point_comment from world_trinitycore_wlk.waypoints where entry = 18294;
UPDATE `world_trinitycore_ctm`.`creature` SET `position_x` = '-1463.63', `position_y` = '6363.4', `position_z` = '36.9237', `orientation` = '0' WHERE (`guid` = '86752');
delete from world_trinitycore_ctm.creature_text where creatureid = 18297;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18297;delete from world_trinitycore_ctm.creature_text where creatureid = 18294;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18294;
DELETE FROM `world_trinitycore_ctm`.`creature` WHERE (`guid` = '86752');
UPDATE `world_trinitycore_ctm`.`smart_scripts` SET `target_type` = '1', `target_param1` = '0' WHERE (`entryorguid` = '1829701') and (`source_type` = '9') and (`id` = '6') and (`link` = '0');
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '18200%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '18200%';
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '18180%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '18180%';
delete from world_trinitycore_ctm.smart_scripts where entryorguid like '18218%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '18218%';
delete from world_trinitycore_ctm.creature_text where creatureid = 18218;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18218;
delete from world_trinitycore_ctm.creature_text where creatureid = 18180;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18180;
delete from world_trinitycore_ctm.creature_text where creatureid = 18200;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18200;
DELETE FROM `world_trinitycore_ctm`.`creature_template_addon` WHERE (`entry` = '18297');
DELETE FROM `world_trinitycore_ctm`.`creature_template_addon` WHERE (`entry` = '19055');use world_trinitycore_ctm;
INSERT INTO `quest_template_addon` (`ID`, `MaxLevel`, `AllowableClasses`, `SourceSpellID`, `PrevQuestID`, `NextQuestID`, `ExclusiveGroup`, `BreadcrumbForQuestId`, `RewardMailTemplateID`, `RewardMailDelay`, `RequiredSkillID`, `RequiredSkillPoints`, `RequiredMinRepFaction`, `RequiredMaxRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepValue`, `ProvidedItemCount`, `SpecialFlags`) VALUES ('29692', '0', '0', '0', '29691', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO world_trinitycore_ctm.creature (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
('215086', '54678', '546', '3716', '3716', '1', '0', '31', '169', '0', '-1', '0', '0', '40.226', '-21.8615', '-2.75393', '5.26059', '300', '0', '0', '1', '0', '0', '0', '0', '0', '', '0');
delete from world_trinitycore_ctm.creature_formations where leaderguid in (SELECT guid FROM world_trinitycore_ctm.creature where map = 546) or memberguid in (SELECT guid FROM world_trinitycore_ctm.creature where map = 546);
insert into world_trinitycore_ctm.creature_formations 
select leaderguid + 1000000, memberguid + 1000000, dist, angle, groupai, point_1, point_2 from 
world_trinitycore_wlk.creature_formations where leaderguid in (SELECT guid FROM world_trinitycore_wlk.creature where map = 546) or memberguid in (SELECT guid FROM world_trinitycore_wlk.creature where map = 546);
delete from world_trinitycore_ctm.creature_addon where guid in (SELECT guid FROM world_trinitycore_ctm.creature where map = 546 and movementtype = 2);
insert into world_trinitycore_ctm.creature_addon 
select guid + 1000000, path_id + 10000000, 0, mount, bytes1, bytes2, emote, 0, 0, 0, visibilitydistancetype, auras 
from world_trinitycore_wlk.creature_addon where guid in (SELECT guid FROM world_trinitycore_wlk.creature where map = 546 and movementtype = 2);
SELECT waypointpathid FROM world_trinitycore_ctm.creature_addon where guid in (SELECT guid FROM world_trinitycore_ctm.creature where map = 546 and movementtype = 2);
delete from world_trinitycore_ctm.waypoint_data where id in ();
insert into world_trinitycore_ctm.waypoint_data 
select id + 10000000, point, position_x, position_y, position_z, orientation, 0, delay, move_type, action, action_chance, wpguid 
FROM world_trinitycore_wlk.waypoint_data where id in (SELECT path_id FROM world_trinitycore_wlk.creature_addon where guid in (SELECT guid FROM world_trinitycore_wlk.creature where map = 546 and movementtype = 2));
delete from world_trinitycore_ctm.creature_loot_template where entry = 17537;
insert into world_trinitycore_ctm.creature_loot_template 
select entry, item, reference, chance, questrequired, 0, lootmode, groupid, mincount, maxcount, comment 
from world_trinitycore_wlk.creature_loot_template where entry = 17537;
delete from world_trinitycore_ctm.creature_text where creatureid = 1834;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 1834;
UPDATE `world_trinitycore_ctm`.`creature_template` SET `npcflag` = '129' WHERE (`entry` = '15351');
use world_trinitycore_ctm;
DELETE FROM `waypoint_data` WHERE id = 20985;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,orientation, velocity, delay, move_type, action, action_chance, wpguid) VALUES
(20985,1,4254.147,2106.288,144.2298, 0, 0, 0, 0,0, 100, 0),
(20985,2,4246.492,2112.086,144.4964, 0, 0, 0, 0,0, 100, 0),
(20985,3,4238.827,2117.891,146.9114, 0, 0, 0, 0,0, 100, 0),
(20985,4,4231.703,2123.257,149.594, 0, 0, 0, 0,0, 100, 0),
(20985,5,4221.862,2120.859,152.6682, 0, 0, 0, 0,0, 100, 0),
(20985,6,4213.184,2118.046,155.2705, 0, 0, 0, 0,0, 100, 0),
(20985,7,4204.313,2114.872,160.2466, 0, 0, 0, 0,0, 100, 0),
(20985,8,4199.398,2108.358,159.4444, 0, 0, 0, 0,0, 100, 0),
(20985,9,4192.858,2101.281,158.2625, 0, 0, 0, 0,0, 100, 0),
(20985,10,4186.512,2094.448,159.0823, 0, 0, 0, 0,0, 100, 0),
(20985,11,4182.779,2085.444,160.7918, 0, 0, 0, 0,0, 100, 0),
(20985,12,4178.139,2077.174,162.841, 0, 0, 0, 0,0, 100, 0),
(20985,13,4175.919,2068.12,165.2072, 0, 0, 0, 0,0, 100, 0),
(20985,14,4173.777,2059.35,168.7659, 0, 0, 0, 0,0, 100, 0),
(20985,15,4178.967,2051.906,172.6272, 0, 0, 0, 0,0, 100, 0),
(20985,16,4183.223,2044.395,176.2537, 0, 0, 0, 0,0, 100, 0),
(20985,17,4185.664,2035.887,179.8787, 0, 0, 0, 0,0, 100, 0),
(20985,18,4187.085,2027,183.3519, 0, 0, 0, 0,0, 100, 0),
(20985,19,4188.41,2018.588,187.4369, 0, 0, 0, 0,0, 100, 0),
(20985,20,4182.815,2011.061,192.7514, 0, 0, 0, 0,0, 100, 0),
(20985,21,4179.333,2003.723,197.7348, 0, 0, 0, 0,0, 100, 0),
(20985,22,4174.904,1997.026,202.2797, 0, 0, 0, 0,0, 100, 0),
(20985,23,4169.787,1990.367,206.382, 0, 0, 0, 0,0, 100, 0),
(20985,24,4162.798,1985.149,209.8569, 0, 0, 0, 0,0, 100, 0),
(20985,25,4155.636,1979.895,213.2113, 0, 0, 0, 0,0, 100, 0),
(20985,26,4147.501,1976.145,216.0639, 0, 0, 0, 0,0, 100, 0),
(20985,27,4139.255,1972.165,218.7715, 0, 0, 0, 0,0, 100, 0),
(20985,28,4131.65,1973.73,220.7531, 0, 0, 0, 0,0, 100, 0),
(20985,29,4114.891,1986.891,224.7545, 0, 0, 0, 0,0, 100, 0),
(20985,30,4104.1,2010.53,231.2639, 0, 0, 0, 0,0, 100, 0),
(20985,31,4083.664,2038.201,240.4482, 0, 0, 0, 0,0, 100, 0),
(20985,32,4064.037,2055.238,248.2406, 0, 0, 0, 0,0, 100, 0),
(20985,33,4045.779,2066.237,253.3096, 0, 0, 0, 0,0, 100, 0),
(20985,34,4027.714,2079.644,254.3991, 0, 0, 0, 0,0, 100, 0),
(20985,35,4017.093,2090.409,254.2741, 0, 0, 0, 0,0, 100, 0),
(20985,36,4003.741,2094.716,254.2741, 0, 0, 0, 0,0, 100, 0),
(20985,37,3996.484,2092.74,254.4492, 0, 0, 0, 0,0, 100, 0); 