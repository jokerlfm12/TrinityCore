insert into world_trinitycore_ctm.gameobject 
select guid, id, map, zoneid, areaid, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, scriptname, verifiedbuild 
from world_trinitycore_wlk.gameobject where id = 183123;

delete from world_trinitycore_ctm.smart_scripts where entryorguid like '19294%';
insert into world_trinitycore_ctm.smart_scripts 
select entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1,  event_param2,  event_param3,  event_param4,  event_param5, 
action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, 
target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment from world_trinitycore_wlk.smart_scripts where entryorguid like '19294%';

insert into world_trinitycore_ctm.gameobject 
select guid, id, map, zone, area, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, '', 0 
from world_emucoach.gameobject where id = 209347 or id = 209348;