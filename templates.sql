insert into world_trinitycore_ctm.gameobject 
select guid, id, map, zoneid, areaid, spawnmask, 0, phasemask, 169, 0, -1, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, scriptname, verifiedbuild 
from world_trinitycore_wlk.gameobject where id = 183123;