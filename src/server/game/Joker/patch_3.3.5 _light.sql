ALTER TABLE `world_merge`.`smart_scripts` 
ADD COLUMN `target_param4` INT NOT NULL AFTER `target_param3`;

delete FROM world_merge.creature where guid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland));
insert into world_merge.creature 
select guid,id,map,zoneid,areaid,spawnmask,0,phasemask,169,0,-1,modelid,equipment_id,position_x,position_y,position_z,orientation,
spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,movementtype,npcflag,unit_flags,dynamicflags,scriptname,verifiedbuild from world_trinity_core.creature 
where map in (SELECT id FROM misc.batch_value_map_outland);
update world_merge.creature set equipment_id = 0 where equipment_id = -1;
update world_merge.creature set spawnmask = 1 where map = 607 or map = 628;
delete FROM world_merge.creature_addon where guid not in (SELECT guid FROM world_merge.creature);
delete FROM world_merge.spawn_group where spawnType = 0 and spawnid not in (SELECT guid FROM world_merge.creature);


delete FROM world_merge.creature_addon where guid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland));
insert into world_merge.creature_addon 
select guid, path_id,mount,bytes1,bytes2,emote,0,0,0,visibilitydistancetype,auras
from world_trinity_core.creature_addon 
where guid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland));
UPDATE `world_merge`.`creature_addon` SET `auras` = '' WHERE (`auras` = '35061');


delete FROM world_merge.creature_formations where leaderguid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland));
insert into world_merge.creature_formations 
select leaderguid,memberguid,dist,angle,groupai,point_1,point_2 
from world_trinity_core.creature_formations 
where leaderguid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland));


update world_merge.creature_template MC join world_trinity_core.creature_template TC on MC.entry = TC.entry 
set MC.gossip_menu_id = TC.gossip_menu_id,MC.minlevel = TC.minlevel,MC.maxlevel = TC.maxlevel,MC.speed_walk = TC.speed_walk,MC.speed_run = TC.speed_run,MC.scale = TC.scale,MC.rank = TC.rank,
MC.dmgschool = TC.dmgschool,MC.baseattacktime = TC.baseattacktime,MC.rangeattacktime = TC.rangeattacktime,MC.basevariance = TC.basevariance,MC.rangevariance = TC.rangevariance,
MC.unit_class = TC.unit_class,MC.unit_flags = TC.unit_flags,MC.unit_flags2 = TC.unit_flags2,MC.dynamicflags = TC.dynamicflags,MC.family = TC.family,MC.type = TC.type,MC.type_flags = TC.type_flags,
MC.petspelldataid = TC.petspelldataid,MC.vehicleid = TC.vehicleid,MC.mingold = TC.mingold,MC.maxgold = TC.maxgold,MC.ainame = TC.ainame,MC.movementtype = TC.movementtype,MC.hoverheight = TC.hoverheight,
MC.healthmodifier = TC.healthmodifier,MC.manamodifier = TC.manamodifier,MC.armormodifier = TC.armormodifier,MC.damagemodifier = TC.damagemodifier,MC.experiencemodifier = TC.experiencemodifier,
MC.racialleader = TC.racialleader,MC.movementid = TC.movementid,MC.regenhealth = TC.regenhealth,MC.mechanic_immune_mask = TC.mechanic_immune_mask,MC.spell_school_immune_mask = TC.spell_school_immune_mask,MC.verifiedbuild = TC.verifiedbuild 
where MC.ainame = '' and MC.entry in (SELECT entry FROM misc.batch_value_creature_outland);

update world_merge.creature_template MC join world_trinity_core.creature_template TC on MC.entry = TC.entry 
set MC.gossip_menu_id = TC.gossip_menu_id,MC.minlevel = TC.minlevel,MC.maxlevel = TC.maxlevel,MC.speed_walk = TC.speed_walk,MC.speed_run = TC.speed_run,MC.scale = TC.scale,MC.rank = TC.rank,
MC.dmgschool = TC.dmgschool,MC.baseattacktime = TC.baseattacktime,MC.rangeattacktime = TC.rangeattacktime,MC.basevariance = TC.basevariance,MC.rangevariance = TC.rangevariance,
MC.unit_class = TC.unit_class,MC.unit_flags = TC.unit_flags,MC.unit_flags2 = TC.unit_flags2,MC.dynamicflags = TC.dynamicflags,MC.family = TC.family,MC.type = TC.type,MC.type_flags = TC.type_flags,
MC.petspelldataid = TC.petspelldataid,MC.vehicleid = TC.vehicleid,MC.mingold = TC.mingold,MC.maxgold = TC.maxgold,MC.movementtype = TC.movementtype,MC.hoverheight = TC.hoverheight,
MC.healthmodifier = TC.healthmodifier,MC.manamodifier = TC.manamodifier,MC.armormodifier = TC.armormodifier,MC.damagemodifier = TC.damagemodifier,MC.experiencemodifier = TC.experiencemodifier,
MC.racialleader = TC.racialleader,MC.movementid = TC.movementid,MC.regenhealth = TC.regenhealth,MC.mechanic_immune_mask = TC.mechanic_immune_mask,MC.spell_school_immune_mask = TC.spell_school_immune_mask,MC.verifiedbuild = TC.verifiedbuild 
where MC.ainame <> '' and MC.entry in (SELECT entry FROM misc.batch_value_creature_outland);

update world_merge.creature_template MC join world_azerothcore.creature_template AC on MC.entry = AC.entry 
set MC.trainer_type = AC.trainer_type,MC.trainer_class = AC.trainer_class,MC.trainer_race = AC.trainer_race,
MC.resistance1 = AC.resistance1,MC.resistance2 = AC.resistance2,MC.resistance3 = AC.resistance3,MC.resistance4 = AC.resistance4,MC.resistance5 = AC.resistance5,MC.resistance6 = AC.resistance6
where MC.entry in (SELECT entry FROM misc.batch_value_creature_outland);

UPDATE `world_merge`.`creature_template` SET `ScriptName` = 'npc_rockjaw_invader' WHERE (`entry` = '37070');
UPDATE `world_merge`.`creature_template` SET `AIName` = '', `ScriptName` = 'npc_matt' WHERE (`entry` = '794');

update world_merge.creature_template set AIName = '' where ScriptName <> '';


delete FROM world_merge.creature_template_addon where entry in (SELECT entry FROM misc.batch_value_creature_outland);
insert into world_merge.creature_template_addon 
select entry,path_id,mount,bytes1,bytes2,emote,0,0,0,visibilitydistancetype,auras 
from world_trinity_core.creature_template_addon 
where entry in (SELECT entry FROM misc.batch_value_creature_outland);


delete FROM world_merge.creature_equip_template where CreatureID in (SELECT entry FROM misc.batch_value_creature_outland);
insert into world_merge.creature_equip_template 
select * 
from world_trinity_core.creature_equip_template 
where CreatureID in (SELECT entry FROM misc.batch_value_creature_outland);


delete FROM world_merge.creature_template_movement where CreatureId in (SELECT entry FROM misc.batch_value_creature_outland);
insert into world_merge.creature_template_movement 
select CreatureId,ground,swim,flight,rooted,random 
from world_trinity_core.creature_template_movement 
where CreatureId in (SELECT entry FROM misc.batch_value_creature_outland);


delete from world_merge.creature_text 
where CreatureID in (SELECT entry FROM misc.batch_value_creature_outland) and CreatureID in (SELECT CreatureID FROM world_trinity_core.creature_text) and CreatureID <> 19227;
insert into world_merge.creature_text 
select CreatureId,groupid,id,text,type,language,probability,emote,duration,sound,0,broadcasttextid,textrange,comment 
from world_trinity_core.creature_text 
where CreatureId not in (SELECT CreatureId FROM world_merge.creature_text);
UPDATE `world_merge`.`creature_text` SET `Sound` = '0' WHERE `Sound` = '11198';
UPDATE `world_trinity_core_cataclysm`.`creature_text` SET `GroupID` = '1', `ID` = '0' WHERE (`CreatureID` = '18428') and (`GroupID` = '0') and (`ID` = '1');
UPDATE `world_trinity_core_cataclysm`.`creature_text` SET `GroupID` = '2', `ID` = '0' WHERE (`CreatureID` = '18428') and (`GroupID` = '0') and (`ID` = '2');
UPDATE `world_trinity_core_cataclysm`.`creature_text` SET `GroupID` = '3', `ID` = '0' WHERE (`CreatureID` = '18428') and (`GroupID` = '0') and (`ID` = '3');
UPDATE `world_trinity_core_cataclysm`.`creature_text` SET `GroupID` = '4', `ID` = '0' WHERE (`CreatureID` = '18428') and (`GroupID` = '0') and (`ID` = '4');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '0' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '5');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '1' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '6');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '2' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '7');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '3' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '8');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '4' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '9');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '5' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '10');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '6' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '11');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '7' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '12');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '8' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '13');
UPDATE `world_trinity_core`.`creature_text` SET `GroupID` = '1', `ID` = '9' WHERE (`CreatureID` = '18688') and (`GroupID` = '0') and (`ID` = '14');



delete FROM world_merge.gameobject where guid in (SELECT guid FROM world_trinity_core.gameobject where map in (SELECT id FROM misc.batch_value_map_outland));
insert into world_merge.gameobject 
select guid,id,map,zoneid,areaid,spawnmask,0,phasemask,169,0,-1,position_x,position_y,position_z,orientation,rotation0,rotation1,rotation2,rotation3,
spawntimesecs,animprogress,state,scriptname,verifiedbuild 
from world_trinity_core.gameobject 
where map in (SELECT id FROM misc.batch_value_map_outland);
delete FROM world_merge.spawn_group where spawnType = 1 and spawnid not in (SELECT guid FROM world_merge.gameobject);


delete FROM world_merge.gameobject_addon where guid in (SELECT guid FROM world_trinity_core.gameobject where map in (SELECT id FROM misc.batch_value_map_outland));
insert into world_merge.gameobject_addon 
select * 
from world_trinity_core.gameobject_addon 
where guid in (SELECT guid FROM world_trinity_core.gameobject where map in (SELECT id FROM misc.batch_value_map_outland));


update world_merge.gameobject_template MG join world_trinity_core.gameobject_template TG on MG.entry = TG.entry 
set MG.ainame = TG.ainame,MG.verifiedbuild = TG.verifiedbuild 
where MG.ainame = '' and MG.entry in (SELECT entry FROM misc.batch_value_gameobject_outland);

update world_merge.gameobject_template MG join world_trinity_core.gameobject_template TG on MG.entry = TG.entry 
set MG.verifiedbuild = TG.verifiedbuild 
where MG.ainame <> '' and MG.entry in (SELECT entry FROM misc.batch_value_gameobject_outland);

update world_merge.gameobject_template set displayid = 336 where displayId = 1287;
update world_merge.gameobject_template set AIName = '' where ScriptName <> '';


delete FROM world_merge.gameobject_template_addon where entry in (SELECT entry FROM misc.batch_value_gameobject_outland);
insert into world_merge.gameobject_template_addon 
select entry,faction,flags,mingold,maxgold,0,0,0,0,0
from world_trinity_core.gameobject_template_addon 
where entry in (SELECT entry FROM misc.batch_value_gameobject_outland);


delete FROM world_merge.gossip_menu where MenuID in (SELECT menuid FROM misc.menu);
insert into world_merge.gossip_menu 
select * 
from world_trinity_core.gossip_menu 
where MenuID in (SELECT menuid FROM misc.menu);


delete FROM world_merge.gossip_menu_option where MenuID in (SELECT menuid FROM misc.menu);
insert into world_merge.gossip_menu_option 
select menuid,optionid,optionicon,optiontext,optionbroadcasttextid,optiontype,optionnpcflag,verifiedbuild 
from world_trinity_core.gossip_menu_option 
where MenuID in (SELECT menuid FROM misc.menu);


delete FROM world_merge.gossip_menu_option_action where MenuID in (SELECT menuid FROM misc.menu);
insert into world_merge.gossip_menu_option_action 
select menuid,optionid,ActionMenuID, 0 
from world_trinity_core.gossip_menu_option 
where ActionMenuID <> 0 and MenuID in (SELECT menuid FROM misc.menu);


delete FROM world_merge.gossip_menu_option_box where MenuID in (SELECT menuid FROM misc.menu);
insert into world_merge.gossip_menu_option_box 
select menuid,optionid,boxcoded,boxmoney,boxtext,boxbroadcasttextid 
from world_trinity_core.gossip_menu_option 
where (boxcoded <> 0 or boxmoney <> 0 or boxtext <> '') and MenuID in (SELECT menuid FROM misc.menu);


delete FROM world_merge.linked_respawn 
where 
(guid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland))) 
or 
(linkedGuid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland)));
insert into world_merge.linked_respawn 
select * 
from world_trinity_core.linked_respawn 
where 
(guid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland))) 
or 
(linkedGuid in (SELECT guid FROM world_trinity_core.creature where map in (SELECT id FROM misc.batch_value_map_outland)));


delete FROM world_merge.mail_level_reward where level >= 60;
insert into world_merge.mail_level_reward 
select * 
from world_trinity_core.mail_level_reward 
where level >= 60;


delete FROM world_merge.npc_spellclick_spells where npc_entry in (SELECT entry FROM misc.batch_value_creature_outland);
insert into world_merge.npc_spellclick_spells 
select * 
from world_trinity_core.npc_spellclick_spells 
where npc_entry in (SELECT entry FROM misc.batch_value_creature_outland);


delete FROM world_merge.quest_details where id in (SELECT id FROM world_trinity_core.quest_template where id not in (SELECT entry FROM world_vmangos.quest_template));
insert into world_merge.quest_details 
select * 
from world_trinity_core.quest_details 
where id in (SELECT id FROM world_trinity_core.quest_template where id not in (SELECT entry FROM world_vmangos.quest_template));


delete FROM world_merge.quest_greeting where id in (SELECT id FROM world_trinity_core.quest_template where id not in (SELECT entry FROM world_vmangos.quest_template));
insert into world_merge.quest_greeting 
select * 
from world_trinity_core.quest_greeting 
where id in (SELECT id FROM world_trinity_core.quest_template where id not in (SELECT entry FROM world_vmangos.quest_template));


delete FROM world_merge.script_waypoint where entry in (SELECT entry FROM misc.batch_value_creature_outland);
insert into world_merge.script_waypoint 
select * 
from world_trinity_core.script_waypoint 
where entry in (SELECT entry FROM misc.batch_value_creature_outland);


insert into world_merge.smart_scripts 
select * 
from world_trinity_core.smart_scripts 
where source_type = 0 and entryorguid in (SELECT entry FROM misc.batch_value_creature_outland) and entryorguid not in (SELECT entryorguid FROM world_merge.smart_scripts where source_type = 0);


insert into world_merge.smart_scripts 
select * 
from world_trinity_core.smart_scripts 
where source_type = 0 and entryorguid in (SELECT guid FROM misc.batch_value_creature_minus_guid_outland) and entryorguid not in (SELECT entryorguid FROM world_merge.smart_scripts where source_type = 0);


insert into world_merge.smart_scripts 
select * 
from world_trinity_core.smart_scripts 
where source_type = 1 and entryorguid in (SELECT entry FROM misc.batch_value_gameobject_outland) and entryorguid not in (SELECT entryorguid FROM world_merge.smart_scripts where source_type = 1);


insert into world_merge.smart_scripts 
select * 
from world_trinity_core.smart_scripts 
where source_type = 1 and entryorguid in (SELECT guid FROM misc.batch_value_gameobject_minus_guid_outland) and entryorguid not in (SELECT entryorguid FROM world_merge.smart_scripts where source_type = 1);


insert into world_merge.smart_scripts
select * from world_trinity_core.smart_scripts where source_type = 9 and entryorguid in 
(select entry from misc.batch_value_timed_335) and entryorguid not in (SELECT entryorguid FROM world_merge.smart_scripts where source_type = 9);

delete FROM world_merge.smart_scripts where source_type = 0 and entryorguid in (SELECT entry FROM world_merge.creature_template where ScriptName <> '');
delete FROM world_merge.smart_scripts where source_type = 1 and entryorguid in (SELECT entry FROM world_merge.gameobject_template where ScriptName <> '');

UPDATE `world_merge`.`creature_template` SET `AIName` = 'SmartAI' WHERE (`entry` = '17147');
UPDATE `world_merge`.`smart_scripts` SET `action_param1` = '170' WHERE (`entryorguid` = '-130968') and (`source_type` = '0') and (`id` = '0') and (`link` = '1');
UPDATE `world_merge`.`smart_scripts` SET `action_param1` = '170' WHERE (`entryorguid` = '-130968') and (`source_type` = '0') and (`id` = '4') and (`link` = '5');
UPDATE `world_merge`.`smart_scripts` SET `action_param1` = '170' WHERE (`entryorguid` = '-130968') and (`source_type` = '0') and (`id` = '3') and (`link` = '4');
UPDATE `world_merge`.`creature_template` SET `AIName` = '' WHERE (`entry` = '23383');
DELETE FROM `world_merge`.`smart_scripts` WHERE `entryorguid` = '23383' and source_type = 0;
DELETE FROM `world_merge`.`smart_scripts` WHERE `entryorguid` like '23383%' and source_type = 9;

insert into world_merge.waypoints 
select * 
from world_trinity_core.waypoints 
where entry not in (SELECT entry FROM world_merge.waypoints);


insert into world_merge.waypoint_data 
select * 
from world_trinity_core.waypoint_data 
where id not in (SELECT id FROM world_merge.waypoint_data);


insert into world_merge.npc_text 
SELECT * FROM world_trinity_core.npc_text where id not in (SELECT id FROM world_merge.npc_text);