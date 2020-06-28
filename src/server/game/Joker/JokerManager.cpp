#include "JokerManager.h"

JokerManager::JokerManager()
{
    updateDelay = JOKER_MANAGER_SAVE_GAP;

    if (!sJokerConfig->Enable)
    {
        return;
    }

    // EJ instance_encounters and unique elite and raid elite
    ieSet.clear();
    ueSet.clear();
    reSet.clear();
    QueryResult ieQR = WorldDatabase.Query("SELECT creditEntry FROM instance_encounters");
    if (ieQR)
    {
        do
        {
            Field* ieField = ieQR->Fetch();
            uint32 eachEntry = ieField[0].GetUInt32();
            ieSet.insert(eachEntry);
        } while (ieQR->NextRow());
    }
    QueryResult ueQR = WorldDatabase.Query("SELECT id, count(*) cc FROM creature where id in (SELECT entry FROM creature_template where rank = 1) group by id");
    if (ueQR)
    {
        do
        {
            Field* ueField = ueQR->Fetch();
            uint32 eachEntry = ueField[0].GetUInt32();
            uint32 eachCount = ueField[1].GetUInt32();
            if (eachCount == 1)
            {
                ueSet.insert(eachEntry);
            }
        } while (ueQR->NextRow());
    }
    QueryResult reQR = WorldDatabase.Query("SELECT distinct id FROM world_trinity_core.creature where map = 409");
    if (reQR)
    {
        do
        {
            Field* reField = reQR->Fetch();
            uint32 eachEntry = reField[0].GetUInt32();
            reSet.insert(eachEntry);
        } while (reQR->NextRow());
    }
    reSet.insert(11663);
    reSet.insert(11664);
}

void JokerManager::UpdateJoker(uint32 pmDiff)
{
    if (!sJokerConfig->Enable)
    {
        return;
    }
    updateDelay -= pmDiff;
    if (updateDelay < 0)
    {
        updateDelay = JOKER_MANAGER_SAVE_GAP;
    }
}

JokerManager* JokerManager::instance()
{
    static JokerManager instance;
    return &instance;
}
