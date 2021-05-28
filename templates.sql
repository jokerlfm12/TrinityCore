30619 cleave
11976 strike
12461 backhand 
15165 pummel
11972 shield bash
15655 shield slam
25712 heroic strike
22911 charge
9080 hamstring
insert into world_trinitycore_ctm.gameobject 
select guid, id, map, zoneid, areaid, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, scriptname, verifiedbuild 
from world_trinitycore_wlk.gameobject where id = 183123;

delete from world_trinitycore_ctm.smart_scripts where entryorguid like '19294%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_param4, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '19294%';

delete from world_trinitycore_ctm.smart_scripts where entryorguid = 20906;
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_param4, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid = 20906;

insert into world_trinitycore_ctm.gameobject 
select guid, id, map, zone, area, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, '', 0 
from world_emucoach.gameobject where id = 209347 or id = 209348;

INSERT INTO `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES ('215419', '54603', '543', '3562', '3562', '3', '0', '1', '169', '0', '-1', '0', '0', '-1343.26', '1650.59', '68.9473', '0.6687', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');

insert into npc_vendor 
select entry, slot, item, maxcount, incrtime, extendedcost, 1, 0, verifiedbuild 
from world_trinitycore_wlk.npc_vendor where entry = 25195;

delete from world_trinitycore_ctm.waypoints where entry = 18180;
insert into world_trinitycore_ctm.waypoints 
select entry, pointid, position_x, position_y, position_z, orientation, 0, delay, point_comment 
from world_trinitycore_wlk.waypoints where entry = 18180;

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

delete from world_trinitycore_ctm.creature_text where creatureid = 18319;
insert into world_trinitycore_ctm.creature_text 
select creatureid, groupid, id, text, type, language, probability, emote, duration, sound, 0, broadcasttextid, textrange, comment 
from world_trinitycore_wlk.creature_text where creatureid = 18319;

INSERT INTO world_trinitycore_ctm.creature (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseUseFlags`, `phaseMask`, `PhaseId`, `PhaseGroup`, `terrainSwapMap`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`) VALUES 
('117621', '20114', '530', '3523', '3722', '1', '0', '1', '169', '0', '-1', '0', '0', '3923.06', '3873.36', '180.752', '0', '300', '0', '0', '1', '0', '0', '0', '898', '0', '', '0');