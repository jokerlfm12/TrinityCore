#include "NingerManager.h"
#include "AccountMgr.h"
#include "Awareness_Base.h"
#include "NingerConfig.h"
#include "Group.h"
#include "Script_Paladin.h"
#include "Script_Hunter.h"
#include "Script_Warlock.h"
#include "Chat.h"
#include "SpellMgr.h"
#include "SpellAuras.h"
#include "MotionMaster.h"
#include "Map.h"
#include "Pet.h"
#include "GridNotifiers.h"

NingerManager::NingerManager()
{
    onlineCheckDelay = 0;
    offlineCheckDelay = 0;
    nameIndex = 0;
    ningerEntityMap.clear();
    deleteningerAccountSet.clear();
    onlinePlayerIDMap.clear();
    tamableBeastEntryMap.clear();
    spellNameEntryMap.clear();
}

void NingerManager::InitializeManager()
{
    sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Initialize ninger");

    onlineCheckDelay = sNingerConfig->OnlineCheckDelay;
    offlineCheckDelay = sNingerConfig->OfflineCheckDelay;

    QueryResult ningerNamesQR = WorldDatabase.Query("SELECT name FROM ninger_names order by rand()");
    if (!ningerNamesQR)
    {
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_ERROR, "Found zero ninger names");
        sNingerConfig->Enable = false;
        return;
    }
    do
    {
        Field* fields = ningerNamesQR->Fetch();
        std::string eachName = fields[0].GetString();
        ningerNameMap[ningerNameMap.size()] = eachName;
    } while (ningerNamesQR->NextRow());

    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_HUMAN;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_NIGHTELF;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_GNOME;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_DWARF;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_ORC;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = Races::RACE_UNDEAD_PLAYER;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_TAUREN;
    availableRaces[CLASS_WARRIOR][availableRaces[CLASS_WARRIOR].size()] = RACE_TROLL;

    availableRaces[CLASS_PALADIN][availableRaces[CLASS_PALADIN].size()] = RACE_HUMAN;
    availableRaces[CLASS_PALADIN][availableRaces[CLASS_PALADIN].size()] = RACE_DWARF;

    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_HUMAN;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_DWARF;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_NIGHTELF;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_GNOME;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_ORC;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = RACE_TROLL;
    availableRaces[CLASS_ROGUE][availableRaces[CLASS_ROGUE].size()] = Races::RACE_UNDEAD_PLAYER;

    availableRaces[CLASS_PRIEST][availableRaces[CLASS_PRIEST].size()] = RACE_HUMAN;
    availableRaces[CLASS_PRIEST][availableRaces[CLASS_PRIEST].size()] = RACE_DWARF;
    availableRaces[CLASS_PRIEST][availableRaces[CLASS_PRIEST].size()] = RACE_NIGHTELF;
    availableRaces[CLASS_PRIEST][availableRaces[CLASS_PRIEST].size()] = RACE_TROLL;
    availableRaces[CLASS_PRIEST][availableRaces[CLASS_PRIEST].size()] = Races::RACE_UNDEAD_PLAYER;

    availableRaces[CLASS_MAGE][availableRaces[CLASS_MAGE].size()] = RACE_HUMAN;
    availableRaces[CLASS_MAGE][availableRaces[CLASS_MAGE].size()] = RACE_GNOME;
    availableRaces[CLASS_MAGE][availableRaces[CLASS_MAGE].size()] = Races::RACE_UNDEAD_PLAYER;
    availableRaces[CLASS_MAGE][availableRaces[CLASS_MAGE].size()] = RACE_TROLL;

    availableRaces[CLASS_WARLOCK][availableRaces[CLASS_WARLOCK].size()] = RACE_HUMAN;
    availableRaces[CLASS_WARLOCK][availableRaces[CLASS_WARLOCK].size()] = RACE_GNOME;
    availableRaces[CLASS_WARLOCK][availableRaces[CLASS_WARLOCK].size()] = Races::RACE_UNDEAD_PLAYER;
    availableRaces[CLASS_WARLOCK][availableRaces[CLASS_WARLOCK].size()] = RACE_ORC;

    availableRaces[CLASS_SHAMAN][availableRaces[CLASS_SHAMAN].size()] = RACE_ORC;
    availableRaces[CLASS_SHAMAN][availableRaces[CLASS_SHAMAN].size()] = RACE_TAUREN;
    availableRaces[CLASS_SHAMAN][availableRaces[CLASS_SHAMAN].size()] = RACE_TROLL;

    availableRaces[CLASS_HUNTER][availableRaces[CLASS_HUNTER].size()] = RACE_DWARF;
    availableRaces[CLASS_HUNTER][availableRaces[CLASS_HUNTER].size()] = RACE_NIGHTELF;
    availableRaces[CLASS_HUNTER][availableRaces[CLASS_HUNTER].size()] = RACE_ORC;
    availableRaces[CLASS_HUNTER][availableRaces[CLASS_HUNTER].size()] = RACE_TAUREN;
    availableRaces[CLASS_HUNTER][availableRaces[CLASS_HUNTER].size()] = RACE_TROLL;

    availableRaces[CLASS_DRUID][availableRaces[CLASS_DRUID].size()] = RACE_NIGHTELF;
    availableRaces[CLASS_DRUID][availableRaces[CLASS_DRUID].size()] = RACE_TAUREN;

    characterTalentTabNameMap.clear();
    characterTalentTabNameMap[Classes::CLASS_WARRIOR][0] = "Arms";
    characterTalentTabNameMap[Classes::CLASS_WARRIOR][1] = "Fury";
    characterTalentTabNameMap[Classes::CLASS_WARRIOR][2] = "Protection";

    characterTalentTabNameMap[Classes::CLASS_HUNTER][0] = "Beast Mastery";
    characterTalentTabNameMap[Classes::CLASS_HUNTER][1] = "Marksmanship";
    characterTalentTabNameMap[Classes::CLASS_HUNTER][2] = "Survival";

    characterTalentTabNameMap[Classes::CLASS_SHAMAN][0] = "Elemental";
    characterTalentTabNameMap[Classes::CLASS_SHAMAN][1] = "Enhancement";
    characterTalentTabNameMap[Classes::CLASS_SHAMAN][2] = "Restoration";

    characterTalentTabNameMap[Classes::CLASS_PALADIN][0] = "Holy";
    characterTalentTabNameMap[Classes::CLASS_PALADIN][1] = "Protection";
    characterTalentTabNameMap[Classes::CLASS_PALADIN][2] = "Retribution";

    characterTalentTabNameMap[Classes::CLASS_WARLOCK][0] = "Affliction";
    characterTalentTabNameMap[Classes::CLASS_WARLOCK][1] = "Demonology";
    characterTalentTabNameMap[Classes::CLASS_WARLOCK][2] = "Destruction";

    characterTalentTabNameMap[Classes::CLASS_PRIEST][0] = "Descipline";
    characterTalentTabNameMap[Classes::CLASS_PRIEST][1] = "Holy";
    characterTalentTabNameMap[Classes::CLASS_PRIEST][2] = "Shadow";

    characterTalentTabNameMap[Classes::CLASS_ROGUE][0] = "Assassination";
    characterTalentTabNameMap[Classes::CLASS_ROGUE][1] = "Combat";
    characterTalentTabNameMap[Classes::CLASS_ROGUE][2] = "subtlety";

    characterTalentTabNameMap[Classes::CLASS_MAGE][0] = "Arcane";
    characterTalentTabNameMap[Classes::CLASS_MAGE][1] = "Fire";
    characterTalentTabNameMap[Classes::CLASS_MAGE][2] = "Frost";

    characterTalentTabNameMap[Classes::CLASS_DRUID][0] = "Balance";
    characterTalentTabNameMap[Classes::CLASS_DRUID][1] = "Feral";
    characterTalentTabNameMap[Classes::CLASS_DRUID][2] = "Restoration";

    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        CreatureTemplate cInfo = itr->second;
        if (cInfo.IsTameable(false))
        {
            tamableBeastEntryMap[tamableBeastEntryMap.size()] = cInfo.Entry;
        }
    }
    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); i++)
    {
        SpellInfo const* pS = sSpellMgr->GetSpellInfo(i);
        if (!pS)
        {
            continue;
        }
        spellNameEntryMap[pS->SpellName].insert(pS->Id);
    }

    characterTalentLearningMap.clear();
    std::unordered_set<uint32> rogueTalentSet_combat0;
    rogueTalentSet_combat0.insert(13732);
    rogueTalentSet_combat0.insert(13705);
    rogueTalentSet_combat0.insert(79007);
    rogueTalentSet_combat0.insert(18427);
    rogueTalentSet_combat0.insert(14165);
    rogueTalentSet_combat0.insert(13754);
    rogueTalentSet_combat0.insert(84617);
    rogueTalentSet_combat0.insert(13712);
    rogueTalentSet_combat0.insert(35541);
    rogueTalentSet_combat0.insert(31124);
    rogueTalentSet_combat0.insert(13750);
    rogueTalentSet_combat0.insert(51682);
    rogueTalentSet_combat0.insert(84652);
    rogueTalentSet_combat0.insert(51690);
    std::unordered_map<uint32, std::unordered_set<uint32>> rogueTalentMap_combat;
    rogueTalentMap_combat[rogueTalentMap_combat.size()] = rogueTalentSet_combat0;
    std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_set<uint32>>> rogueTalentMap;
    // combat 
    rogueTalentMap[1] = rogueTalentMap_combat;
    characterTalentLearningMap[Classes::CLASS_ROGUE] = rogueTalentMap;
    std::unordered_set<uint32> priestTalentSet_discipline0;
    priestTalentSet_discipline0.insert(14748);
    priestTalentSet_discipline0.insert(47586);
    priestTalentSet_discipline0.insert(14520);
    priestTalentSet_discipline0.insert(63574);
    priestTalentSet_discipline0.insert(14747);
    priestTalentSet_discipline0.insert(89485);
    priestTalentSet_discipline0.insert(10060);
    priestTalentSet_discipline0.insert(57470);
    priestTalentSet_discipline0.insert(47535);
    priestTalentSet_discipline0.insert(52795);
    priestTalentSet_discipline0.insert(33206);
    priestTalentSet_discipline0.insert(89488);
    priestTalentSet_discipline0.insert(47509);
    priestTalentSet_discipline0.insert(47516);
    priestTalentSet_discipline0.insert(62618);
    std::unordered_map<uint32, std::unordered_set<uint32>> priestTalentMap_discipline;
    priestTalentMap_discipline[priestTalentMap_discipline.size()] = priestTalentSet_discipline0;
    std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_set<uint32>>> priestTalentMap;
    // discipline 
    priestTalentMap[0] = priestTalentMap_discipline;
    characterTalentLearningMap[Classes::CLASS_PRIEST] = priestTalentMap;
    std::unordered_set<uint32> warlockTalentSet_destruction0;
    warlockTalentSet_destruction0.insert(17788);
    warlockTalentSet_destruction0.insert(17793);
    warlockTalentSet_destruction0.insert(17815);
    warlockTalentSet_destruction0.insert(17954);
    warlockTalentSet_destruction0.insert(17877);
    warlockTalentSet_destruction0.insert(47258);
    warlockTalentSet_destruction0.insert(18119);
    warlockTalentSet_destruction0.insert(91713);
    warlockTalentSet_destruction0.insert(30293);
    warlockTalentSet_destruction0.insert(91986);
    warlockTalentSet_destruction0.insert(30283);
    warlockTalentSet_destruction0.insert(47266);
    warlockTalentSet_destruction0.insert(30299);
    warlockTalentSet_destruction0.insert(80240);
    warlockTalentSet_destruction0.insert(47220);
    warlockTalentSet_destruction0.insert(50796);
    std::unordered_map<uint32, std::unordered_set<uint32>> warlockTalentMap_destruction;
    warlockTalentMap_destruction[warlockTalentMap_destruction.size()] = warlockTalentSet_destruction0;
    std::unordered_map<uint32, std::unordered_map<uint32, std::unordered_set<uint32>>> warlockTalentMap;
    // destruction 
    warlockTalentMap[2] = warlockTalentMap_destruction;
    characterTalentLearningMap[Classes::CLASS_WARLOCK] = warlockTalentMap;

    instantPoisonEntryMap.clear();
    instantPoisonEntryMap[10] = 6947;
    instantPoisonEntryMap[28] = 6949;
    instantPoisonEntryMap[36] = 6950;
    instantPoisonEntryMap[44] = 8926;
    instantPoisonEntryMap[52] = 8927;
    instantPoisonEntryMap[60] = 8928;
    instantPoisonEntryMap[68] = 21927;
    instantPoisonEntryMap[73] = 43230;
    instantPoisonEntryMap[79] = 43231;


    QueryResult ningerQR = CharacterDatabase.Query("SELECT ninger_id, account_name, character_id, target_level FROM ninger order by rand()");
    if (ningerQR)
    {
        do
        {
            Field* fields = ningerQR->Fetch();
            uint32 ninger_id = fields[0].GetUInt32();
            std::string account_name = fields[1].GetString();
            uint32 character_id = fields[2].GetUInt32();
            uint32 target_level = fields[3].GetUInt32();
            NingerEntity* re = new NingerEntity(ninger_id);
            re->account_name = account_name;
            re->character_id = character_id;
            re->target_level = target_level;
            ningerEntityMap[account_name] = re;
        } while (ningerQR->NextRow());
    }

    sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "ninger initialized");
}

NingerManager* NingerManager::instance()
{
    static NingerManager instance;
    return &instance;
}

void NingerManager::UpdateNingerManager(uint32 pmDiff)
{
    if (sNingerConfig->Enable == 0)
    {
        return;
    }

    if (onlineCheckDelay >= 0)
    {
        onlineCheckDelay -= pmDiff;
    }
    if (onlineCheckDelay < 0)
    {
        onlineCheckDelay = sNingerConfig->OnlineCheckDelay;
        std::unordered_set<uint32> onlinePlayerLevelSet;
        onlinePlayerLevelSet.clear();
        std::unordered_map<uint32, WorldSession*> allSessions = sWorld->GetAllSessions();
        for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
        {
            if (WorldSession* eachWS = wsIT->second)
            {
                if (!eachWS->isNinger)
                {
                    if (Player* eachPlayer = eachWS->GetPlayer())
                    {
                        uint32 eachLevel = eachPlayer->getLevel();
                        if (onlinePlayerLevelSet.find(eachLevel) == onlinePlayerLevelSet.end())
                        {
                            onlinePlayerLevelSet.insert(eachLevel);
                        }
                    }
                }
            }
        }
        for (std::unordered_set<uint32>::iterator levelIT = onlinePlayerLevelSet.begin(); levelIT != onlinePlayerLevelSet.end(); levelIT++)
        {
            uint32 eachLevel = *levelIT;
            LoginNingers(eachLevel);
        }
    }

    if (offlineCheckDelay >= 0)
    {
        offlineCheckDelay -= pmDiff;
    }
    if (offlineCheckDelay < 0)
    {
        offlineCheckDelay = sNingerConfig->OfflineCheckDelay;
        std::unordered_set<uint32> onlinePlayerLevelSet;
        onlinePlayerLevelSet.clear();
        std::unordered_map<uint32, WorldSession*> allSessions = sWorld->GetAllSessions();
        for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
        {
            if (WorldSession* eachWS = wsIT->second)
            {
                if (!eachWS->isNinger)
                {
                    if (Player* eachPlayer = eachWS->GetPlayer())
                    {
                        uint32 eachLevel = eachPlayer->getLevel();
                        if (onlinePlayerLevelSet.find(eachLevel) == onlinePlayerLevelSet.end())
                        {
                            onlinePlayerLevelSet.insert(eachLevel);
                        }
                    }
                }
            }
        }
        for (uint32 checkLevel = 10; checkLevel <= 60; checkLevel++)
        {
            if (onlinePlayerLevelSet.find(checkLevel) == onlinePlayerLevelSet.end())
            {
                LogoutNingers(checkLevel);
            }
        }
    }

    for (std::unordered_map<std::string, NingerEntity*>::iterator reIT = ningerEntityMap.begin(); reIT != ningerEntityMap.end(); reIT++)
    {
        if (NingerEntity* eachRE = reIT->second)
        {
            eachRE->Update(pmDiff);
        }
    }
}

bool NingerManager::Deleteningers()
{
    CharacterDatabase.DirectExecute("delete from ninger");

    std::ostringstream sqlStream;
    sqlStream << "SELECT id, username FROM account where username like '" << sNingerConfig->AccountNamePrefix << "%'";
    std::string sql = sqlStream.str();
    QueryResult accountQR = LoginDatabase.Query(sql.c_str());

    if (accountQR)
    {
        do
        {
            Field* fields = accountQR->Fetch();
            uint32 id = fields[0].GetUInt32();
            std::string userName = fields[1].GetString();
            deleteningerAccountSet.insert(id);
            sAccountMgr->DeleteAccount(id);
            sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Delete ninger account %d - %s", id, userName.c_str());
        } while (accountQR->NextRow());
    }

    ningerEntityMap.clear();

    return true;
}

uint32 NingerManager::CheckNingerAccount(std::string pmAccountName)
{
    uint32 accountID = 0;

    QueryResult accountQR = LoginDatabase.PQuery("SELECT id FROM account where username = '%s'", pmAccountName.c_str());
    if (accountQR)
    {
        Field* idFields = accountQR->Fetch();
        accountID = idFields[0].GetUInt32();
    }

    return accountID;
}

bool NingerManager::CreateNingerAccount(std::string pmAccountName)
{
    bool result = false;

    AccountOpResult aor = sAccountMgr->CreateAccount(pmAccountName, NINGER_PASSWORD);
    if (aor == AccountOpResult::AOR_NAME_ALREADY_EXIST)
    {
        result = true;
    }
    else if (aor == AccountOpResult::AOR_OK)
    {
        result = true;
    }

    return result;
}

uint32 NingerManager::CheckAccountCharacter(uint32 pmAccountID)
{
    uint32 result = 0;

    QueryResult characterQR = CharacterDatabase.PQuery("SELECT guid FROM characters where account = '%d'", pmAccountID);
    if (characterQR)
    {
        Field* characterFields = characterQR->Fetch();
        result = characterFields[0].GetUInt32();
    }

    return result;
}

uint32 NingerManager::GetCharacterRace(uint32 pmCharacterID)
{
    uint32 result = 0;

    QueryResult characterQR = CharacterDatabase.PQuery("SELECT race FROM characters where guid = '%d'", pmCharacterID);
    if (characterQR)
    {
        Field* characterFields = characterQR->Fetch();
        result = characterFields[0].GetUInt32();
    }

    return result;
}

uint32 NingerManager::CreateNingerCharacter(uint32 pmAccountID)
{
    uint32  targetClass = Classes::CLASS_ROGUE;
    uint32 classRandom = urand(0, 100);
    if (classRandom < 40)
    {
        targetClass = Classes::CLASS_ROGUE;
    }
    else if (classRandom < 75)
    {
        targetClass = Classes::CLASS_WARLOCK;
    }
    else
    {
        targetClass = Classes::CLASS_PRIEST;
    }
    uint32 raceIndex = 0;
    uint32 targetRace = 0;
    raceIndex = urand(0, availableRaces[targetClass].size() - 1);
    targetRace = availableRaces[targetClass][raceIndex];

    return CreateNingerCharacter(pmAccountID, targetClass, targetRace);
}

uint32 NingerManager::CreateNingerCharacter(uint32 pmAccountID, uint32 pmCharacterClass, uint32 pmCharacterRace)
{
    uint32 result = 0;

    std::string currentName = "";
    bool nameValid = false;
    while (nameIndex < ningerNameMap.size())
    {
        currentName = ningerNameMap[nameIndex];
        QueryResult checkNameQR = CharacterDatabase.PQuery("SELECT count(*) FROM characters where name = '%s'", currentName.c_str());

        if (!checkNameQR)
        {
            sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Name %s is available", currentName.c_str());
            nameValid = true;
        }
        else
        {
            Field* nameCountFields = checkNameQR->Fetch();
            uint32 nameCount = nameCountFields[0].GetUInt32();
            if (nameCount == 0)
            {
                nameValid = true;
            }
        }

        nameIndex++;
        if (nameValid)
        {
            break;
        }
    }
    if (!nameValid)
    {
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_ERROR, "No available names");
        return false;
    }

    uint8 gender = 0, skin = 0, face = 0, hairStyle = 0, hairColor = 0, facialHair = 0;
    while (true)
    {
        gender = urand(0, 100);
        if (gender < 50)
        {
            gender = 0;
        }
        else
        {
            gender = 1;
        }
        face = urand(0, 5);
        hairStyle = urand(0, 5);
        hairColor = urand(0, 5);
        facialHair = urand(0, 5);

        CharacterCreateInfo* cci = new CharacterCreateInfo();
        cci->Name = currentName;
        cci->Race = pmCharacterRace;
        cci->Class = pmCharacterClass;
        cci->Gender = gender;
        cci->Skin = skin;
        cci->Face = face;
        cci->HairStyle = hairStyle;
        cci->HairColor = hairColor;
        cci->FacialHair = facialHair;
        cci->OutfitId = 0;

        WorldSession* eachSession = new WorldSession(pmAccountID, "ninger", 0, NULL, SEC_PLAYER, 3, 0, LOCALE_enUS, 0, false);
        Player* newPlayer = new Player(eachSession);
        if (!newPlayer->Create(sObjectMgr->GetGenerator<HighGuid::Player>().Generate(), cci))
        {
            newPlayer->CleanupsBeforeDelete();
            delete eachSession;
            delete newPlayer;
            sLog->outMessage("ninger", LogLevel::LOG_LEVEL_ERROR, "Character create failed, %s %d %d", currentName.c_str(), pmCharacterRace, pmCharacterClass);
            sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Try again");
            continue;
        }
        newPlayer->GetMotionMaster()->Initialize();
        newPlayer->setCinematic(2);
        newPlayer->SetAtLoginFlag(AT_LOGIN_NONE);
        newPlayer->SaveToDB(true);
        result = newPlayer->GetGUID().GetCounter();
        eachSession->isNinger = true;
        sWorld->AddSession(eachSession);
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Create character %d - %s for account %d", newPlayer->GetGUID().GetCounter(), currentName.c_str(), pmAccountID);
        break;
    }

    return result;
}

Player* NingerManager::CheckLogin(uint32 pmAccountID, uint32 pmCharacterID)
{
    ObjectGuid guid = ObjectGuid(HighGuid::Player, pmCharacterID);
    Player* currentPlayer = ObjectAccessor::FindPlayer(guid);
    if (currentPlayer)
    {
        return currentPlayer;
    }
    return NULL;
}

bool NingerManager::LoginNinger(uint32 pmAccountID, uint32 pmCharacterID)
{
    ObjectGuid playerGuid = ObjectGuid(HighGuid::Player, pmCharacterID);
    if (Player* currentPlayer = ObjectAccessor::FindPlayer(playerGuid))
    {
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "ninger %d %s is already in world", pmCharacterID, currentPlayer->GetName());
        return false;
    }
    WorldSession* loginSession = sWorld->FindSession(pmAccountID);
    if (!loginSession)
    {
        loginSession = new WorldSession(pmAccountID, "ninger", 0, NULL, SEC_PLAYER, 3, 0, LOCALE_enUS, 0, false);
        sWorld->AddSession(loginSession);
    }
    loginSession->isNinger = true;    
    loginSession->HandlePlayerLogin(playerGuid);
    sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Log in character %d %d", pmAccountID, pmCharacterID);

    return true;
}

bool NingerManager::LoginNingers(uint32 pmLevel, uint32 pmCount)
{
    if (pmLevel >= 10)
    {
        // current count 
        uint32 currentCount = 0;
        QueryResult levelNingerQR = CharacterDatabase.PQuery("SELECT count(*) FROM ninger where target_level = %d", pmLevel);
        if (levelNingerQR)
        {
            Field* fields = levelNingerQR->Fetch();
            currentCount = fields[0].GetUInt32();
        }
        uint32 totalCount = pmCount;
        if (totalCount == 0)
        {
            totalCount = sNingerConfig->NingerCountEachLevel;
        }
        if (currentCount < totalCount)
        {
            int toAdd = totalCount - currentCount;
            uint32 checkNumber = 0;
            while (toAdd > 0)
            {
                std::string checkAccountName = "";
                while (true)
                {
                    std::ostringstream accountNameStream;
                    accountNameStream << "NINGERL" << pmLevel << "N" << checkNumber;
                    checkAccountName = accountNameStream.str();
                    std::ostringstream querySQLStream;
                    querySQLStream << "SELECT * FROM account where username ='" << checkAccountName << "'";
                    std::string querySQL = querySQLStream.str();
                    QueryResult accountNameQR = LoginDatabase.Query(querySQL.c_str());
                    if (!accountNameQR)
                    {
                        break;
                    }
                    sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Account %s exists, try again", checkAccountName.c_str());
                    checkNumber++;
                }
                uint32 ningerID = pmLevel * 10000 + checkNumber;
                std::ostringstream sqlStream;
                sqlStream << "INSERT INTO ninger (ninger_id, account_name, character_id, target_level) VALUES (" << ningerID << ", '" << checkAccountName << "', 0, " << pmLevel << ")";
                std::string sql = sqlStream.str();
                CharacterDatabase.DirectExecute(sql.c_str());
                std::ostringstream replyStream;
                replyStream << "ninger " << checkAccountName << " created";
                sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str());
                checkNumber++;
                toAdd--;
            }
        }
        uint32 onlineCount = 0;
        for (std::unordered_map<std::string, NingerEntity*>::iterator reIT = ningerEntityMap.begin(); reIT != ningerEntityMap.end(); reIT++)
        {
            if (NingerEntity* eachRE = reIT->second)
            {
                if (eachRE->target_level == pmLevel)
                {
                    if (eachRE->entityState != NingerEntityState::NingerEntityState_OffLine && eachRE->entityState != NingerEntityState::NingerEntityState_None)
                    {
                        onlineCount++;
                    }
                }
            }
        }
        uint32 toOnline = 0;
        if (totalCount > onlineCount)
        {
            toOnline = totalCount - onlineCount;
        }
        QueryResult toOnLineQR = CharacterDatabase.PQuery("SELECT ninger_id, account_name, character_id FROM ninger where target_level = %d", pmLevel);
        if (toOnLineQR)
        {
            do
            {
                Field* fields = toOnLineQR->Fetch();
                uint32 ninger_id = fields[0].GetUInt32();
                std::string account_name = fields[1].GetString();
                uint32 character_id = fields[2].GetUInt32();
                if (ningerEntityMap.find(account_name) != ningerEntityMap.end())
                {
                    if (ningerEntityMap[account_name]->entityState == NingerEntityState::NingerEntityState_OffLine)
                    {
                        if (toOnline > 0)
                        {
                            ningerEntityMap[account_name]->entityState = NingerEntityState::NingerEntityState_Enter;
                            uint32 onlineWaiting = urand(5, 20);
                            ningerEntityMap[account_name]->checkDelay = onlineWaiting * TimeConstants::IN_MILLISECONDS;
                            std::ostringstream replyStream;
                            replyStream << "ninger " << account_name << " ready to go online";
                            sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str());
                            toOnline--;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    NingerEntity* re = new NingerEntity(ninger_id);
                    re->account_id = 0;
                    re->account_name = account_name;
                    re->character_id = character_id;
                    re->target_level = pmLevel;
                    re->entityState = NingerEntityState::NingerEntityState_Enter;
                    re->checkDelay = 5 * TimeConstants::IN_MILLISECONDS;
                    ningerEntityMap[account_name] = re;
                    std::ostringstream replyStream;
                    replyStream << "ninger " << account_name << " entity created, ready to go online";
                    sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str());
                }
            } while (toOnLineQR->NextRow());
        }
    }
    return true;
}

void NingerManager::LogoutNinger(uint32 pmCharacterID)
{
    ObjectGuid guid = ObjectGuid(HighGuid::Player, pmCharacterID);
    if (Player* checkP = ObjectAccessor::FindPlayer(guid))
    {
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Log out ninger %s", checkP->GetName());
        std::ostringstream msgStream;
        msgStream << checkP->GetName() << " logged out";
        sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, msgStream.str().c_str());
        if (WorldSession* checkWS = checkP->GetSession())
        {
            checkWS->LogoutPlayer(true);
        }
    }
}

void NingerManager::LogoutNingers(uint32 pmLevel)
{
    for (std::unordered_map<std::string, NingerEntity*>::iterator reIT = ningerEntityMap.begin(); reIT != ningerEntityMap.end(); reIT++)
    {
        if (NingerEntity* eachRE = reIT->second)
        {
            if (pmLevel > 0)
            {
                if (eachRE->target_level != pmLevel)
                {
                    continue;
                }
            }
            ObjectGuid guid = ObjectGuid(HighGuid::Player, eachRE->character_id);
            if (Player* checkP = ObjectAccessor::FindPlayer(guid))
            {
                if (Map* ningerMap = checkP->GetMap())
                {
                    if (ningerMap->Instanceable())
                    {
                        checkP->TeleportTo(checkP->m_homebindMapId, checkP->m_homebindX, checkP->m_homebindY, checkP->m_homebindZ, 0);
                    }
                }
                eachRE->entityState = NingerEntityState::NingerEntityState_DoLogoff;
                uint32 offlineWaiting = urand(1 * TimeConstants::IN_MILLISECONDS, 2 * TimeConstants::IN_MILLISECONDS);
                eachRE->checkDelay = offlineWaiting;
            }
        }
    }
}

bool NingerManager::PrepareNinger(Player* pmNinger)
{
    if (!pmNinger)
    {
        return false;
    }
    InitializeEquipments(pmNinger, false);
    pmNinger->DurabilityRepairAll(false, 0, false);

    //if (pmNinger->getClass() == Classes::CLASS_HUNTER)
    //{
    //    uint32 ammoEntry = 0;
    //    Item* weapon = pmNinger->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
    //    if (weapon)
    //    {
    //        if (const ItemTemplate* it = weapon->GetTemplate())
    //        {
    //            uint32 subClass = it->GetSubClass();
    //            uint8 playerLevel = pmNinger->getLevel();
    //            if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW || subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW)
    //            {
    //                if (playerLevel >= 40)
    //                {
    //                    ammoEntry = 11285;
    //                }
    //                else if (playerLevel >= 25)
    //                {
    //                    ammoEntry = 3030;
    //                }
    //                else
    //                {
    //                    ammoEntry = 2515;
    //                }
    //            }
    //            else if (subClass == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN)
    //            {
    //                if (playerLevel >= 40)
    //                {
    //                    ammoEntry = 11284;
    //                }
    //                else if (playerLevel >= 25)
    //                {
    //                    ammoEntry = 3033;
    //                }
    //                else
    //                {
    //                    ammoEntry = 2519;
    //                }
    //            }
    //            if (ammoEntry > 0)
    //            {
    //                if (!pmNinger->HasItemCount(ammoEntry, 100))
    //                {
    //                    pmNinger->StoreNewItemInBestSlots(ammoEntry, 1000);
    //                }
    //            }
    //        }
    //    }
    //}
    //else if (pmNinger->getClass() == Classes::CLASS_SHAMAN)
    //{
    //    if (!pmNinger->HasItemCount(5175))
    //    {
    //        pmNinger->StoreNewItemInBestSlots(5175, 1);
    //    }
    //    if (!pmNinger->HasItemCount(5176))
    //    {
    //        pmNinger->StoreNewItemInBestSlots(5176, 1);
    //    }
    //}
    //Pet* checkPet = pmNinger->GetPet();
    //if (checkPet)
    //{
    //    checkPet->SetReactState(REACT_DEFENSIVE);
    //    std::unordered_map<uint32, PetSpell> petSpellMap = checkPet->m_spells;
    //    for (std::unordered_map<uint32, PetSpell>::iterator it = petSpellMap.begin(); it != petSpellMap.end(); it++)
    //    {
    //        if (it->second.active == ACT_DISABLED || it->second.active == ACT_ENABLED)
    //        {
    //            const SpellInfo* pS = sSpellMgr->GetSpellInfo(it->first);
    //            if (pS)
    //            {
    //                std::string checkNameStr = std::string(pS->SpellName);
    //                if (checkNameStr == "Prowl")
    //                {
    //                    checkPet->ToggleAutocast(pS, false);
    //                }
    //                else if (checkNameStr == "Phase Shift")
    //                {
    //                    checkPet->ToggleAutocast(pS, false);
    //                }
    //                else if (checkNameStr == "Cower")
    //                {
    //                    checkPet->ToggleAutocast(pS, false);
    //                }
    //                else if (checkNameStr == "Growl")
    //                {
    //                    if (pmNinger->GetGroup())
    //                    {
    //                        checkPet->ToggleAutocast(pS, false);
    //                    }
    //                    else
    //                    {
    //                        checkPet->ToggleAutocast(pS, true);
    //                    }
    //                }
    //                else
    //                {
    //                    checkPet->ToggleAutocast(pS, true);
    //                }
    //            }
    //        }
    //    }
    //}

    pmNinger->Say("Ready", Language::LANG_UNIVERSAL);

    return true;
}

std::unordered_set<uint32> NingerManager::GetUsableEquipSlot(const ItemTemplate* pmIT)
{
    std::unordered_set<uint32> resultSet;

    switch (pmIT->GetInventoryType())
    {
    case INVTYPE_HEAD:
    {
        resultSet.insert(EQUIPMENT_SLOT_HEAD);
        break;
    }
    case INVTYPE_NECK:
    {
        resultSet.insert(EQUIPMENT_SLOT_NECK);
        break;
    }
    case INVTYPE_SHOULDERS:
    {
        resultSet.insert(EQUIPMENT_SLOT_SHOULDERS);
        break;
    }
    case INVTYPE_BODY:
    {
        resultSet.insert(EQUIPMENT_SLOT_BODY);
        break;
    }
    case INVTYPE_CHEST:
    {
        resultSet.insert(EQUIPMENT_SLOT_CHEST);
        break;
    }
    case INVTYPE_ROBE:
    {
        resultSet.insert(EQUIPMENT_SLOT_CHEST);
        break;
    }
    case INVTYPE_WAIST:
    {
        resultSet.insert(EQUIPMENT_SLOT_WAIST);
        break;
    }
    case INVTYPE_LEGS:
    {
        resultSet.insert(EQUIPMENT_SLOT_LEGS);
        break;
    }
    case INVTYPE_FEET:
    {
        resultSet.insert(EQUIPMENT_SLOT_FEET);
        break;
    }
    case INVTYPE_WRISTS:
    {
        resultSet.insert(EQUIPMENT_SLOT_WRISTS);
        break;
    }
    case INVTYPE_HANDS:
    {
        resultSet.insert(EQUIPMENT_SLOT_HANDS);
        break;
    }
    case INVTYPE_FINGER:
    {
        resultSet.insert(EQUIPMENT_SLOT_FINGER1);
        resultSet.insert(EQUIPMENT_SLOT_FINGER2);
        break;
    }
    case INVTYPE_TRINKET:
    {
        resultSet.insert(EQUIPMENT_SLOT_TRINKET1);
        resultSet.insert(EQUIPMENT_SLOT_TRINKET2);
        break;
    }
    case INVTYPE_CLOAK:
    {
        resultSet.insert(EQUIPMENT_SLOT_BACK);
        break;
    }
    case INVTYPE_WEAPON:
    {
        resultSet.insert(EQUIPMENT_SLOT_MAINHAND);
        resultSet.insert(EQUIPMENT_SLOT_OFFHAND);
        break;
    }
    case INVTYPE_SHIELD:
    {
        resultSet.insert(EQUIPMENT_SLOT_OFFHAND);
        break;
    }
    case INVTYPE_RANGED:
    {
        resultSet.insert(EQUIPMENT_SLOT_RANGED);
        break;
    }
    case INVTYPE_2HWEAPON:
    {
        resultSet.insert(EQUIPMENT_SLOT_MAINHAND);
        break;
    }
    case INVTYPE_TABARD:
    {
        resultSet.insert(EQUIPMENT_SLOT_TABARD);
        break;
    }
    case INVTYPE_WEAPONMAINHAND:
    {
        resultSet.insert(EQUIPMENT_SLOT_MAINHAND);
        break;
    }
    case INVTYPE_WEAPONOFFHAND:
    {
        resultSet.insert(EQUIPMENT_SLOT_OFFHAND);
        break;
    }
    case INVTYPE_HOLDABLE:
    {
        resultSet.insert(EQUIPMENT_SLOT_OFFHAND);
        break;
    }
    case INVTYPE_THROWN:
    {
        resultSet.insert(EQUIPMENT_SLOT_RANGED);
        break;
    }
    case INVTYPE_RANGEDRIGHT:
    {
        resultSet.insert(EQUIPMENT_SLOT_RANGED);
        break;
    }
    case INVTYPE_BAG:
    {
        resultSet.insert(INVENTORY_SLOT_BAG_START);
        break;
    }
    case INVTYPE_RELIC:
    {
        break;
    }
    default:
    {
        break;
    }
    }

    return resultSet;
}

void NingerManager::HandlePlayerSay(Player* pmPlayer, std::string pmContent)
{
    if (!pmPlayer)
    {
        return;
    }
    std::vector<std::string> commandVector = SplitString(pmContent, " ", true);
    std::string commandName = commandVector.at(0);
    if (commandName == "role")
    {
        if (Awareness_Base* playerAI = pmPlayer->awarenessMap[pmPlayer->activeAwarenessIndex])
        {
            if (commandVector.size() > 1)
            {
                std::string newRole = commandVector.at(1);
                playerAI->SetGroupRole(newRole);
            }
            std::ostringstream replyStream;
            replyStream << "Your group role is ";
            replyStream << playerAI->GetGroupRoleName();
            sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
        }
    }
    else if (commandName == "arrangement")
    {
        std::ostringstream replyStream;
        if (Group* myGroup = pmPlayer->GetGroup())
        {
            if (myGroup->GetLeaderGUID() == pmPlayer->GetGUID())
            {
                bool paladinAura_concentration = false;
                bool paladinAura_devotion = false;
                bool paladinAura_retribution = false;
                bool paladinAura_fire = false;
                bool paladinAura_frost = false;
                bool paladinAura_shadow = false;

                bool paladinBlessing_kings = false;
                bool paladinBlessing_might = false;
                bool paladinBlessing_wisdom = false;
                bool paladinBlessing_salvation = false;

                bool paladinSeal_Justice = false;

                bool warlockCurse_Weakness = false;
                bool warlockCurse_Tongues = false;
                bool warlockCurse_Element = false;

                int rtiIndex = 0;

                bool hunterAspect_wild = false;

                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    Player* member = groupRef->GetSource();
                    if (member)
                    {
                        if (member->GetMapId() == 619)
                        {
                            member->activeAwarenessIndex = 619;
                        }
                        else if (member->GetMapId() == 555)
                        {
                            member->activeAwarenessIndex = 555;
                        }
                        else if (member->GetMapId() == 585)
                        {
                            member->activeAwarenessIndex = 585;
                        }
                        else
                        {
                            member->activeAwarenessIndex = 0;
                        }
                        if (Awareness_Base* memberAwareness = member->awarenessMap[member->activeAwarenessIndex])
                        {
                            memberAwareness->Reset();
                            if (Script_Base* sb = memberAwareness->sb)
                            {
                                switch (member->getClass())
                                {
                                case Classes::CLASS_WARRIOR:
                                {
                                    memberAwareness->groupRole = GroupRole::GroupRole_Tank;
                                    break;
                                }
                                case Classes::CLASS_SHAMAN:
                                {
                                    memberAwareness->groupRole = GroupRole::GroupRole_Healer;
                                    break;
                                }
                                case Classes::CLASS_PALADIN:
                                {
                                    memberAwareness->groupRole = GroupRole::GroupRole_Healer;
                                    break;
                                }
                                case Classes::CLASS_PRIEST:
                                {
                                    memberAwareness->groupRole = GroupRole::GroupRole_Healer;
                                    break;
                                }
                                case Classes::CLASS_DRUID:
                                {
                                    memberAwareness->groupRole = GroupRole::GroupRole_Tank;
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                                if (member->getClass() == Classes::CLASS_PALADIN)
                                {
                                    if (Script_Paladin* sp = (Script_Paladin*)sb)
                                    {
                                        if (memberAwareness->groupRole != GroupRole::GroupRole_Healer)
                                        {
                                            if (!paladinSeal_Justice)
                                            {
                                                sp->sealType = PaladinSealType::PaladinSealType_Justice;
                                                paladinSeal_Justice = true;
                                            }
                                            else
                                            {
                                                sp->sealType = PaladinSealType::PaladinSealType_Righteousness;
                                            }
                                        }
                                        if (!paladinBlessing_salvation)
                                        {
                                            sp->blessingType = PaladinBlessingType::PaladinBlessingType_Salvation;
                                            paladinBlessing_salvation = true;
                                        }
                                        else if (!paladinBlessing_might)
                                        {
                                            sp->blessingType = PaladinBlessingType::PaladinBlessingType_Might;
                                            paladinBlessing_might = true;
                                        }
                                        else if (!paladinBlessing_wisdom)
                                        {
                                            sp->blessingType = PaladinBlessingType::PaladinBlessingType_Wisdom;
                                            paladinBlessing_wisdom = true;
                                        }
                                        else if (!paladinBlessing_kings)
                                        {
                                            sp->blessingType = PaladinBlessingType::PaladinBlessingType_Kings;
                                            paladinBlessing_kings = true;
                                        }
                                        else
                                        {
                                            sp->blessingType = PaladinBlessingType::PaladinBlessingType_Might;
                                            paladinBlessing_might = true;
                                        }

                                        if (!paladinAura_devotion)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_Devotion;
                                            paladinAura_devotion = true;
                                        }
                                        else if (!paladinAura_retribution)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_Retribution;
                                            paladinAura_retribution = true;
                                        }
                                        else if (!paladinAura_concentration)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_Concentration;
                                            paladinAura_concentration = true;
                                        }
                                        else if (!paladinAura_fire)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_FireResistant;
                                            paladinAura_fire = true;
                                        }
                                        else if (!paladinAura_frost)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_FrostResistant;
                                            paladinAura_frost = true;
                                        }
                                        else if (!paladinAura_shadow)
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_ShadowResistant;
                                            paladinAura_shadow = true;
                                        }
                                        else
                                        {
                                            sp->auraType = PaladinAuraType::PaladinAuraType_Devotion;
                                            paladinAura_devotion = true;
                                        }
                                    }
                                }
                                if (member->getClass() == Classes::CLASS_MAGE)
                                {
                                    if (rtiIndex >= 0 && rtiIndex < TARGETICONCOUNT)
                                    {
                                        sb->rti = rtiIndex;
                                        rtiIndex++;
                                    }
                                }
                                if (member->getClass() == Classes::CLASS_HUNTER)
                                {
                                    if (Script_Hunter* sh = (Script_Hunter*)sb)
                                    {
                                        if (hunterAspect_wild)
                                        {
                                            sh->aspectType = HunterAspectType::HunterAspectType_Hawk;
                                        }
                                        else
                                        {
                                            sh->aspectType = HunterAspectType::HunterAspectType_Wild;
                                            hunterAspect_wild = true;
                                        }
                                    }
                                }
                                if (member->getClass() == Classes::CLASS_WARLOCK)
                                {
                                    if (Script_Warlock* swl = (Script_Warlock*)sb)
                                    {
                                        if (!warlockCurse_Weakness)
                                        {
                                            swl->curseType = WarlockCurseType::WarlockCurseType_Weakness;
                                            warlockCurse_Weakness = true;
                                        }
                                        else if (!warlockCurse_Tongues)
                                        {
                                            swl->curseType = WarlockCurseType::WarlockCurseType_Tongues;
                                            warlockCurse_Tongues = true;
                                        }
                                        else if (!warlockCurse_Element)
                                        {
                                            swl->curseType = WarlockCurseType::WarlockCurseType_Element;
                                            warlockCurse_Element = true;
                                        }
                                        else
                                        {
                                            swl->curseType = WarlockCurseType::WarlockCurseType_Weakness;
                                        }
                                    }
                                }
                            }
                            memberAwareness->Report();
                        }
                    }
                }
                replyStream << "Arranged";
            }
            else
            {
                replyStream << "You are not leader";
            }
        }
        else
        {
            replyStream << "Not in a group";
        }
        sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
    }
    else if (commandName == "join")
    {
        std::ostringstream replyStream;
        Group* myGroup = pmPlayer->GetGroup();
        if (myGroup)
        {
            ObjectGuid targetGUID = pmPlayer->GetTarget();
            if (!targetGUID.IsEmpty())
            {
                bool validTarget = false;
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    Player* member = groupRef->GetSource();
                    if (member)
                    {
                        if (member->GetGUID() != pmPlayer->GetGUID())
                        {
                            if (member->GetGUID() == targetGUID)
                            {
                                validTarget = true;
                                replyStream << "Joining " << member->GetName();
                                pmPlayer->TeleportTo(member->GetMapId(), member->GetPositionX(), member->GetPositionY(), member->GetPositionZ(), member->GetOrientation());
                            }
                        }
                    }
                }
                if (!validTarget)
                {
                    replyStream << "Target is no group member";
                }
            }
            else
            {
                replyStream << "You have no target";
            }
        }
        else
        {
            replyStream << "You are not in a group";
        }
        sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
    }
    else if (commandName == "leader")
    {
        if (Group* myGroup = pmPlayer->GetGroup())
        {
            if (myGroup->GetLeaderGUID() != pmPlayer->GetGUID())
            {
                bool change = true;
                if (Player* leader = ObjectAccessor::FindPlayer(myGroup->GetLeaderGUID()))
                {
                    if (WorldSession* leaderSession = leader->GetSession())
                    {
                        if (!leaderSession->isNinger)
                        {
                            change = false;
                        }
                    }
                }
                if (change)
                {
                    myGroup->ChangeLeader(pmPlayer->GetGUID());
                }
                else
                {
                    sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, "Leader is valid", pmPlayer);
                }
            }
            else
            {
                sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, "You are the leader", pmPlayer);
            }
        }
        else
        {
            sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, "You are not in a group", pmPlayer);
        }
    }
    else if (commandName == "ninger")
    {
        if (commandVector.size() > 1)
        {
            std::string ningerAction = commandVector.at(1);
            if (ningerAction == "delete")
            {
                std::ostringstream replyStream;
                bool allOffline = true;
                for (std::unordered_map<std::string, NingerEntity*>::iterator reIT = ningerEntityMap.begin(); reIT != ningerEntityMap.end(); reIT++)
                {
                    NingerEntity* eachRE = reIT->second;
                    if (eachRE->entityState != NingerEntityState::NingerEntityState_None && eachRE->entityState != NingerEntityState::NingerEntityState_OffLine)
                    {
                        allOffline = false;
                        replyStream << "Not all ningers are offline. Going offline first";
                        LogoutNingers();
                        break;
                    }
                }
                if (allOffline)
                {
                    replyStream << "All ningers are offline. Ready to delete";
                    Deleteningers();
                }
                sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
            }
            else if (ningerAction == "offline")
            {
                std::ostringstream replyStream;
                replyStream << "All ningers are going offline";
                LogoutNingers();
                sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
            }
            else if (ningerAction == "online")
            {
                uint32 playerLevel = pmPlayer->getLevel();
                if (playerLevel < 10)
                {
                    std::ostringstream replyStream;
                    replyStream << "You level is too low";
                    sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
                }
                else
                {
                    uint32 ningerCount = sNingerConfig->NingerCountEachLevel;
                    if (commandVector.size() > 2)
                    {
                        ningerCount = atoi(commandVector.at(2).c_str());
                    }
                    if (ningerCount > 0)
                    {
                        std::ostringstream replyTitleStream;
                        replyTitleStream << "ninger count to go online : " << ningerCount;
                        sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyTitleStream.str().c_str(), pmPlayer);
                        LoginNingers(playerLevel, ningerCount);
                    }
                }
            }
        }
    }
    else if (commandName == "train")
    {
        LearnPlayerSpells(pmPlayer);
    }
    else if (commandName == "talent")
    {
        if (LearnPlayerTalents(pmPlayer))
        {
            if (WorldSession* ws = pmPlayer->GetSession())
            {
                ws->LogoutPlayer(true);
            }
        }
    }
    else if (commandName == "equip")
    {
        InitializeEquipments(pmPlayer, true);
    }
    else if (commandName == "tcast")
    {
        if (commandVector.size() > 1)
        {
            if (Unit* myTarget = pmPlayer->GetSelectedUnit())
            {
                std::string spellIDStr = commandVector.at(1);
                int32 spellID = atoi(spellIDStr.c_str());
                myTarget->CastSpell(nullptr, spellID);
            }
        }
    }
    else if (commandName == "tcastt")
    {
        if (commandVector.size() > 1)
        {
            if (Unit* myTarget = pmPlayer->GetSelectedUnit())
            {
                if (Unit* targetsTarget = ObjectAccessor::GetUnit(*myTarget, myTarget->GetTarget()))
                {
                    std::string spellIDStr = commandVector.at(1);
                    int32 spellID = atoi(spellIDStr.c_str());
                    myTarget->CastSpell(targetsTarget, spellID);
                }
            }
        }
    }
    else if (commandName == "castt")
    {
        if (commandVector.size() > 1)
        {
            if (Unit* myTarget = pmPlayer->GetSelectedUnit())
            {
                std::string spellIDStr = commandVector.at(1);
                int32 spellID = atoi(spellIDStr.c_str());
                pmPlayer->CastSpell(myTarget, spellID);
            }
        }
    }
    else if (commandName == "poison")
    {
        if (Awareness_Base* ab = pmPlayer->awarenessMap[pmPlayer->activeAwarenessIndex])
        {
            if (Script_Base* sb = ab->sb)
            {
                uint32 maxInstancePoisonLevel = 0;
                for (std::unordered_map<uint32, uint32>::iterator ipIT = sNingerManager->instantPoisonEntryMap.begin(); ipIT != sNingerManager->instantPoisonEntryMap.end(); ipIT++)
                {
                    if (ipIT->first <= pmPlayer->getLevel())
                    {
                        if (ipIT->first > maxInstancePoisonLevel)
                        {
                            maxInstancePoisonLevel = ipIT->first;
                        }
                    }
                }
                if (maxInstancePoisonLevel > 0)
                {
                    uint32 instancePoisonEntry = sNingerManager->instantPoisonEntryMap[maxInstancePoisonLevel];
                    if (!pmPlayer->HasItemCount(instancePoisonEntry, 1))
                    {
                        pmPlayer->StoreNewItemInBestSlots(instancePoisonEntry, 20);
                    }
                    Item* instancePoison = sb->GetItemInInventory(instancePoisonEntry);
                    if (instancePoison && !instancePoison->IsInTrade())
                    {
                        if (Item* weapon_mh = pmPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
                        {
                            if (sb->UseItem(instancePoison, weapon_mh))
                            {
                                if (Item* weapon_oh = pmPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
                                {
                                    if (sb->UseItem(instancePoison, weapon_oh))
                                    {
                                        sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, "Added instance poison to weapons", pmPlayer);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (commandName == "aura")
    {
        Unit* checkTarget = pmPlayer->GetSelectedUnit();
        if (commandVector.size() > 1)
        {
            std::string checkType = commandVector.at(1);
            if (checkType == "has")
            {
                if (commandVector.size() > 2)
                {
                    std::string spellIDStr = commandVector.at(2);
                    uint32 spellID = atoi(spellIDStr.c_str());
                    if (!checkTarget)
                    {
                        checkTarget = pmPlayer;
                    }
                    std::ostringstream replyStream;
                    if (checkTarget->HasAura(spellID))
                    {
                        replyStream << "has aura " << spellID;
                    }
                    else
                    {
                        replyStream << "no aura " << spellID;
                    }
                    sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, replyStream.str().c_str(), pmPlayer);
                }
            }
        }
    }
}

bool NingerManager::StringEndWith(const std::string& str, const std::string& tail)
{
    return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
}

bool NingerManager::StringStartWith(const std::string& str, const std::string& head)
{
    return str.compare(0, head.size(), head) == 0;
}

std::vector<std::string> NingerManager::SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored)
{
    std::vector<std::string> resultStringVector;
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}/*pred*/, delimStr.at(0));
    size_t pos = srcStr.find(delimStr.at(0));
    std::string addedString = "";
    while (pos != std::string::npos) {
        addedString = srcStr.substr(0, pos);
        if (!addedString.empty() || !repeatedCharIgnored) {
            resultStringVector.push_back(addedString);
        }
        srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
        pos = srcStr.find(delimStr.at(0));
    }
    addedString = srcStr;
    if (!addedString.empty() || !repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
    }
    return resultStringVector;
}

std::string NingerManager::TrimString(std::string srcStr)
{
    std::string result = srcStr;
    if (!result.empty())
    {
        result.erase(0, result.find_first_not_of(" "));
        result.erase(result.find_last_not_of(" ") + 1);
    }

    return result;
}

void NingerManager::HandlePacket(WorldSession* pmSession, WorldPacket pmPacket)
{
    if (pmSession)
    {
        if (!pmSession->isNinger)
        {
            return;
        }
        if (Player* me = pmSession->GetPlayer())
        {
            if (Awareness_Base* ningerAI = me->awarenessMap[me->activeAwarenessIndex])
            {
                switch (pmPacket.GetOpcode())
                {
                case SMSG_SPELL_FAILURE:
                {
                    break;
                }
                case SMSG_SPELL_DELAYED:
                {
                    break;
                }
                case SMSG_PARTY_INVITE:
                {
                    Group* grp = me->GetGroupInvite();
                    if (!grp)
                    {
                        break;
                    }
                    Player* inviter = ObjectAccessor::FindPlayer(grp->GetLeaderGUID());
                    if (!inviter)
                    {
                        break;
                    }
                    me->GetSession()->HandlePartyInviteResponse(true);
                    std::ostringstream replyStream_Talent;
                    if (Awareness_Base* ningerAI = me->awarenessMap[me->activeAwarenessIndex])
                    {
                        ningerAI->Reset();
                        if (Script_Base* sb = ningerAI->sb)
                        {
                            replyStream_Talent << "My talent category is " << characterTalentTabNameMap[me->getClass()][sb->maxTalentTab];
                            WhisperTo(inviter, replyStream_Talent.str(), Language::LANG_UNIVERSAL, me);
                        }
                    }
                    break;
                }
                case BUY_ERR_NOT_ENOUGHT_MONEY:
                {
                    break;
                }
                case BUY_ERR_REPUTATION_REQUIRE:
                {
                    break;
                }
                case MSG_RAID_READY_CHECK:
                {
                    ningerAI->readyCheckDelay = urand(2000, 6000);
                    break;
                }
                case SMSG_GROUP_SET_LEADER:
                {
                    //std::string leaderName = "";
                    //pmPacket >> leaderName;
                    //Player* newLeader = ObjectAccessor::FindPlayerByName(leaderName);
                    //if (newLeader)
                    //{
                    //    if (newLeader->GetGUID() == me->GetGUID())
                    //    {
                    //        WorldPacket data(CMSG_GROUP_SET_LEADER, 8);
                    //        data << master->GetGUID().WriteAsPacked();
                    //        me->GetSession()->HandleGroupSetLeaderOpcode(data);
                    //    }
                    //    else
                    //    {
                    //        if (!newLeader->isninger)
                    //        {
                    //            master = newLeader;
                    //        }
                    //    }
                    //}
                    break;
                }
                case SMSG_RESURRECT_REQUEST:
                {
                    if (me->IsResurrectRequested())
                    {
                        me->RemoveAllAttackers();
                        me->ClearInCombat();
                        ningerAI->sb->rm->ResetMovement();
                        ningerAI->sb->ClearTarget();
                        me->ResurrectUsingRequestData();
                    }
                    break;
                }
                case SMSG_INVENTORY_CHANGE_FAILURE:
                {
                    break;
                }
                case SMSG_TRADE_STATUS:
                {
                    break;
                }
                case SMSG_LOOT_RESPONSE:
                {
                    break;
                }
                case SMSG_ITEM_PUSH_RESULT:
                {
                    break;
                }
                case SMSG_PARTY_COMMAND_RESULT:
                {
                    break;
                }
                case SMSG_DUEL_REQUESTED:
                {
                    if (!me->duel)
                    {
                        break;
                    }
                    me->DuelComplete(DuelCompleteType::DUEL_INTERRUPTED);
                    WhisperTo(me->duel->opponent, "Not interested", Language::LANG_UNIVERSAL, me);
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
    }
}

void NingerManager::WhisperTo(Player* pmTarget, std::string pmContent, Language pmLanguage, Player* pmSender)
{
    if (pmSender && pmTarget)
    {
        pmSender->Whisper(pmContent, pmLanguage, pmTarget);
    }
}

void NingerManager::HandleChatCommand(Player* pmSender, std::string pmCMD, Player* pmReceiver)
{
    if (!pmSender)
    {
        return;
    }
    std::vector<std::string> commandVector = SplitString(pmCMD, " ", true);
    std::string commandName = commandVector.at(0);
    if (pmReceiver)
    {
        if (WorldSession* receiverSession = pmReceiver->GetSession())
        {
            if (receiverSession->isNinger)
            {
                if (Awareness_Base* receiverAI = pmReceiver->awarenessMap[pmReceiver->activeAwarenessIndex])
                {
#pragma region command handling
                    if (commandName == "role")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            std::string newRole = commandVector.at(1);
                            receiverAI->SetGroupRole(newRole);
                        }
                        replyStream << "My group role is ";
                        replyStream << receiverAI->GetGroupRoleName();
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "engage")
                    {
                        receiverAI->staying = false;
                        if (Unit* target = pmSender->GetSelectedUnit())
                        {
                            if (receiverAI->Engage(target))
                            {
                                if (Group* myGroup = pmReceiver->GetGroup())
                                {
                                    if (myGroup->GetTargetIconByOG(target->GetGUID()) == -1)
                                    {
                                        myGroup->SetTargetIcon(7, pmSender->GetGUID(), target->GetGUID());
                                    }
                                }
                                receiverAI->ogEngageTarget = target->GetGUID();
                                int engageDelay = 5000;
                                if (commandVector.size() > 1)
                                {
                                    std::string checkStr = commandVector.at(1);
                                    engageDelay = atoi(checkStr.c_str());
                                }
                                receiverAI->engageDelay = engageDelay;
                                std::ostringstream replyStream;
                                replyStream << "Try to engage " << target->GetName();
                                WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                            }
                        }
                    }
                    else if (commandName == "tank")
                    {
                        if (Unit* target = pmSender->GetSelectedUnit())
                        {
                            if (receiverAI->groupRole == GroupRole::GroupRole_Tank)
                            {
                                if (receiverAI->Tank(target))
                                {
                                    if (Group* myGroup = pmReceiver->GetGroup())
                                    {
                                        if (myGroup->GetTargetIconByOG(target->GetGUID()) == -1)
                                        {
                                            myGroup->SetTargetIcon(7, pmSender->GetGUID(), target->GetGUID());
                                        }
                                    }
                                    receiverAI->staying = false;
                                    receiverAI->ogEngageTarget = target->GetGUID();
                                    int engageDelay = 5000;
                                    if (commandVector.size() > 1)
                                    {
                                        std::string checkStr = commandVector.at(1);
                                        engageDelay = atoi(checkStr.c_str());
                                    }
                                    receiverAI->engageDelay = engageDelay;
                                    std::ostringstream replyStream;
                                    replyStream << "Try to tank " << target->GetName();
                                    WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                                }
                            }
                            else
                            {
                                receiverAI->staying = false;
                            }
                        }
                    }
                    else if (commandName == "revive")
                    {
                        if (pmReceiver->IsAlive())
                        {
                            receiverAI->reviveDelay = 2000;
                            if (Script_Base* sb = receiverAI->sb)
                            {
                                if (NingerMovement* rm = sb->rm)
                                {
                                    rm->ResetMovement();
                                }
                            }
                        }
                    }
                    else if (commandName == "follow")
                    {
                        std::ostringstream replyStream;
                        bool takeAction = true;
                        if (commandVector.size() > 1)
                        {
                            std::string cmdDistanceStr = commandVector.at(1);
                            float cmdDistance = atof(cmdDistanceStr.c_str());
                            if (cmdDistance == 0.0f)
                            {
                                receiverAI->following = false;
                                replyStream << "Stop following. ";
                                takeAction = false;
                            }
                            else if (cmdDistance >= FOLLOW_MIN_DISTANCE && cmdDistance <= FOLLOW_MAX_DISTANCE)
                            {
                                receiverAI->followDistance = cmdDistance;
                                replyStream << "Distance updated. ";
                            }
                            else
                            {
                                replyStream << "Distance is not valid. ";
                                takeAction = false;
                            }
                        }
                        if (takeAction)
                        {
                            receiverAI->eatDelay = 0;
                            receiverAI->drinkDelay = 0;
                            receiverAI->staying = false;
                            receiverAI->holding = false;
                            receiverAI->following = true;
                            if (receiverAI->Follow())
                            {
                                replyStream << "Following " << receiverAI->followDistance;
                            }
                            else
                            {
                                replyStream << "can not follow";
                            }
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "chase")
                    {
                        std::ostringstream replyStream;
                        bool takeAction = true;
                        if (commandVector.size() > 2)
                        {
                            std::string cmdMinDistanceStr = commandVector.at(1);
                            float cmdMinDistance = atof(cmdMinDistanceStr.c_str());
                            if (cmdMinDistance < MELEE_MIN_DISTANCE)
                            {
                                cmdMinDistance = MELEE_MIN_DISTANCE;
                            }
                            std::string cmdMaxDistanceStr = commandVector.at(2);
                            float cmdMaxDistance = atof(cmdMaxDistanceStr.c_str());
                            if (cmdMaxDistance > RANGE_HEAL_DISTANCE)
                            {
                                cmdMaxDistance = RANGE_HEAL_DISTANCE;
                            }
                            else if (cmdMaxDistance < MELEE_MAX_DISTANCE)
                            {
                                cmdMaxDistance = MELEE_MAX_DISTANCE;
                            }
                            if (cmdMinDistance > cmdMaxDistance)
                            {
                                cmdMinDistance = cmdMaxDistance - MELEE_MIN_DISTANCE;
                            }
                            receiverAI->chaseDistanceMin = cmdMinDistance;
                            receiverAI->chaseDistanceMax = cmdMaxDistance;
                            replyStream << "Chase distance range updated. " << receiverAI->chaseDistanceMin << " " << receiverAI->chaseDistanceMax;
                        }
                        else
                        {
                            replyStream << "Chase distance range is " << receiverAI->chaseDistanceMin << " " << receiverAI->chaseDistanceMax;
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "stay")
                    {
                        std::string targetGroupRole = "";
                        if (commandVector.size() > 1)
                        {
                            targetGroupRole = commandVector.at(1);
                        }
                        if (receiverAI->Stay(targetGroupRole))
                        {
                            WhisperTo(pmSender, "Staying", Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "hold")
                    {
                        std::string targetGroupRole = "";
                        if (commandVector.size() > 1)
                        {
                            targetGroupRole = commandVector.at(1);
                        }
                        if (receiverAI->Hold(targetGroupRole))
                        {
                            WhisperTo(pmReceiver, "Holding", Language::LANG_UNIVERSAL, pmSender);
                        }
                    }
                    else if (commandName == "rest")
                    {
                        std::ostringstream replyStream;
                        if (receiverAI->sb->Eat(true))
                        {
                            receiverAI->eatDelay = DEFAULT_REST_DELAY;
                            receiverAI->drinkDelay = 1000;
                            replyStream << "Resting";
                        }
                        else
                        {
                            replyStream << "Can not rest";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "who")
                    {
                        if (Awareness_Base* ningerAI = pmReceiver->awarenessMap[pmReceiver->activeAwarenessIndex])
                        {
                            if (Script_Base* sb = ningerAI->sb)
                            {
                                WhisperTo(pmSender, characterTalentTabNameMap[pmReceiver->getClass()][sb->maxTalentTab], Language::LANG_UNIVERSAL, pmReceiver);
                            }
                        }
                    }
                    else if (commandName == "assemble")
                    {
                        std::ostringstream replyStream;
                        if (receiverAI->teleportAssembleDelay > 0)
                        {
                            replyStream << "I am on the way";
                        }
                        else
                        {
                            if (pmReceiver->IsAlive())
                            {
                                if (pmReceiver->GetDistance(pmSender) < VISIBILITY_DISTANCE_TINY)
                                {
                                    receiverAI->teleportAssembleDelay = urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS);
                                    replyStream << "We are close, I will teleport to you in " << receiverAI->teleportAssembleDelay / 1000 << " seconds";
                                }
                                else
                                {
                                    receiverAI->teleportAssembleDelay = urand(30 * TimeConstants::IN_MILLISECONDS, 1 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
                                    replyStream << "I will teleport to you in " << receiverAI->teleportAssembleDelay / 1000 << " seconds";
                                }
                            }
                            else
                            {
                                receiverAI->teleportAssembleDelay = urand(1 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS, 2 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
                                replyStream << "I will teleport to you and revive in " << receiverAI->teleportAssembleDelay / 1000 << " seconds";
                            }
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "gather")
                    {
                        std::ostringstream replyStream;
                        if (receiverAI->moveDelay > 0)
                        {
                            replyStream << "I am on the way";
                        }
                        else
                        {
                            if (pmReceiver->IsAlive())
                            {
                                if (pmReceiver->GetDistance(pmSender) < RANGE_HEAL_DISTANCE)
                                {
                                    if (pmReceiver->IsNonMeleeSpellCast(false))
                                    {
                                        pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
                                        pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
                                    }
                                    pmReceiver->GetMotionMaster()->Initialize();
                                    pmReceiver->StopMoving();
                                    receiverAI->eatDelay = 0;
                                    receiverAI->drinkDelay = 0;
                                    int moveDelay = 1000;
                                    if (commandVector.size() > 1)
                                    {
                                        std::string moveDelayStr = commandVector.at(1);
                                        moveDelay = atoi(moveDelayStr.c_str());
                                        if (moveDelay < 1000 || moveDelay > 6000)
                                        {
                                            moveDelay = 1000;                                            
                                        }
                                    }
                                    receiverAI->moveDelay = moveDelay;
                                    receiverAI->sb->rm->MovePoint(pmSender->GetPosition(), moveDelay);
                                    replyStream << "I will move to you";
                                }
                                else
                                {
                                    replyStream << "too far away";
                                }
                            }
                            else
                            {
                                replyStream << "I am dead ";
                            }
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "cast")
                    {
                        std::ostringstream replyStream;
                        if (pmReceiver->IsAlive())
                        {
                            if (commandVector.size() > 1)
                            {
                                std::ostringstream targetStream;
                                uint8 arrayCount = 0;
                                for (std::vector<std::string>::iterator it = commandVector.begin(); it != commandVector.end(); it++)
                                {
                                    if (arrayCount > 0)
                                    {
                                        targetStream << (*it) << " ";
                                    }
                                    arrayCount++;
                                }
                                std::string spellName = TrimString(targetStream.str());
                                Unit* senderTarget = pmSender->GetSelectedUnit();
                                if (!senderTarget)
                                {
                                    senderTarget = pmReceiver;
                                }
                                if (receiverAI->sb->CastSpell(senderTarget, spellName, VISIBILITY_DISTANCE_NORMAL))
                                {
                                    replyStream << "Cast spell " << spellName << " on " << senderTarget->GetName();
                                }
                                else
                                {
                                    replyStream << "Can not cast spell " << spellName << " on " << senderTarget->GetName();
                                }
                            }
                        }
                        else
                        {
                            replyStream << "I am dead";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "cancel")
                    {
                        std::ostringstream replyStream;
                        if (pmReceiver->IsAlive())
                        {
                            if (commandVector.size() > 1)
                            {
                                std::ostringstream targetStream;
                                uint8 arrayCount = 0;
                                for (std::vector<std::string>::iterator it = commandVector.begin(); it != commandVector.end(); it++)
                                {
                                    if (arrayCount > 0)
                                    {
                                        targetStream << (*it) << " ";
                                    }
                                    arrayCount++;
                                }
                                std::string spellName = TrimString(targetStream.str());
                                if (receiverAI->sb->CancelAura(spellName))
                                {
                                    replyStream << "Aura canceled " << spellName;
                                }
                                else
                                {
                                    replyStream << "Can not cancel aura " << spellName;
                                }
                            }
                        }
                        else
                        {
                            replyStream << "I am dead";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "use")
                    {
                        std::ostringstream replyStream;
                        if (pmReceiver->IsAlive())
                        {
                            if (commandVector.size() > 1)
                            {
                                std::string useType = commandVector.at(1);
                                if (useType == "go")
                                {
                                    if (commandVector.size() > 2)
                                    {
                                        std::ostringstream goNameStream;
                                        uint32 checkPos = 2;
                                        while (checkPos < commandVector.size())
                                        {
                                            goNameStream << commandVector.at(checkPos) << " ";
                                            checkPos++;
                                        }
                                        std::string goName = TrimString(goNameStream.str());
                                        bool validToUse = false;
                                        std::list<GameObject*> nearGOList;
                                        pmReceiver->GetGameObjectListWithEntryInGrid(nearGOList, 0, MELEE_MAX_DISTANCE);
                                        for (std::list<GameObject*>::iterator it = nearGOList.begin(); it != nearGOList.end(); it++)
                                        {
                                            if ((*it)->GetName() == goName)
                                            {
                                                pmReceiver->SetFacingToObject((*it));
                                                pmReceiver->StopMoving();
                                                pmReceiver->GetMotionMaster()->Initialize();
                                                (*it)->Use(pmReceiver);
                                                replyStream << "Use game object : " << goName;
                                                validToUse = true;
                                                break;
                                            }
                                        }
                                        if (!validToUse)
                                        {
                                            replyStream << "No go with name " << goName << " nearby";
                                        }
                                    }
                                    else
                                    {
                                        replyStream << "No go name";
                                    }
                                }
                                else if (useType == "item")
                                {

                                }
                                else
                                {
                                    replyStream << "Unknown type";
                                }
                            }
                            else
                            {
                                replyStream << "Use what?";
                            }
                        }
                        else
                        {
                            replyStream << "I am dead";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "stop")
                    {
                        std::ostringstream replyStream;
                        if (pmReceiver->IsAlive())
                        {
                            pmReceiver->StopMoving();
                            pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL);
                            pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
                            pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
                            pmReceiver->InterruptSpell(CurrentSpellTypes::CURRENT_MELEE_SPELL);
                            pmReceiver->AttackStop();
                            if (Script_Base* sb = receiverAI->sb)
                            {
                                sb->PetStop();
                                sb->ClearTarget();
                            }
                            receiverAI->moveDelay = 2000;
                            replyStream << "Stopped";
                        }
                        else
                        {
                            replyStream << "I am dead";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "delay")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            int delayMS = std::stoi(commandVector.at(1));
                            receiverAI->dpsDelay = delayMS;
                            replyStream << "DPS delay set to : " << delayMS;
                        }
                        else
                        {
                            replyStream << "DPS delay is : " << receiverAI->dpsDelay;
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "attackers")
                    {
                        std::ostringstream replyStream;
                        if (pmReceiver->IsAlive())
                        {
                            replyStream << "attackers list : ";
                            std::set<Unit*> attackers = pmReceiver->getAttackers();
                            for (std::set<Unit*>::iterator aIT = attackers.begin(); aIT != attackers.end(); aIT++)
                            {
                                if (Unit* eachAttacker = *aIT)
                                {
                                    replyStream << eachAttacker->GetName() << ", ";
                                }
                            }
                        }
                        else
                        {
                            replyStream << "I am dead";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "cure")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            std::string cureCMD = commandVector.at(1);
                            if (cureCMD == "on")
                            {
                                receiverAI->cure = true;
                            }
                            else if (cureCMD == "off")
                            {
                                receiverAI->cure = false;
                            }
                            else
                            {
                                replyStream << "Unknown state";
                            }
                        }
                        if (receiverAI->cure)
                        {
                            replyStream << "auto cure is on";
                        }
                        else
                        {
                            replyStream << "auto cure is off";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "petting")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            std::string cureCMD = commandVector.at(1);
                            if (cureCMD == "on")
                            {
                                receiverAI->petting = true;
                            }
                            else if (cureCMD == "off")
                            {
                                receiverAI->petting = false;
                            }
                            else
                            {
                                replyStream << "Unknown state";
                            }
                        }
                        if (receiverAI->petting)
                        {
                            replyStream << "petting is on";
                        }
                        else
                        {
                            replyStream << "petting is off";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "aoe")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            std::string on = commandVector.at(1);
                            if (on == "on")
                            {
                                receiverAI->aoe = true;
                            }
                            else if (on == "off")
                            {
                                receiverAI->aoe = false;
                            }
                        }
                        if (receiverAI->aoe)
                        {
                            replyStream << "AOE is on";
                        }
                        else
                        {
                            replyStream << "AOE is off";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "mark")
                    {
                        std::ostringstream replyStream;
                        if (commandVector.size() > 1)
                        {
                            std::string markCMD = commandVector.at(1);
                            if (markCMD == "on")
                            {
                                receiverAI->mark = true;
                            }
                            else if (markCMD == "off")
                            {
                                receiverAI->mark = false;
                            }
                            else
                            {
                                replyStream << "Unknown state";
                            }
                        }
                        if (receiverAI->mark)
                        {
                            replyStream << "Mark is on";
                        }
                        else
                        {
                            replyStream << "Mark is off";
                        }
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "emote")
                    {
                        if (pmReceiver->IsAlive())
                        {
                            if (commandVector.size() > 1)
                            {
                                int emoteCMD = std::stoi(commandVector.at(1));
                                pmReceiver->HandleEmoteCommand((Emote)emoteCMD);
                            }
                            else
                            {
                                pmReceiver->AttackStop();
                                pmReceiver->CombatStop();
                            }
                        }
                        else
                        {
                            WhisperTo(pmSender, "I am dead", Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "pa")
                    {
                        if (pmReceiver->getClass() == Classes::CLASS_PALADIN)
                        {
                            std::ostringstream replyStream;
                            if (Script_Paladin* sp = (Script_Paladin*)receiverAI->sb)
                            {
                                if (commandVector.size() > 1)
                                {
                                    std::string auratypeName = commandVector.at(1);
                                    if (auratypeName == "concentration")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_Concentration;
                                    }
                                    else if (auratypeName == "devotion")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_Devotion;
                                    }
                                    else if (auratypeName == "retribution")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_Retribution;
                                    }
                                    else if (auratypeName == "fire")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_FireResistant;
                                    }
                                    else if (auratypeName == "frost")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_FrostResistant;
                                    }
                                    else if (auratypeName == "shadow")
                                    {
                                        sp->auraType = PaladinAuraType::PaladinAuraType_ShadowResistant;
                                    }
                                    else
                                    {
                                        replyStream << "Unknown type";
                                    }
                                }
                                switch (sp->auraType)
                                {
                                case PaladinAuraType::PaladinAuraType_Concentration:
                                {
                                    replyStream << "concentration";
                                    break;
                                }
                                case PaladinAuraType::PaladinAuraType_Devotion:
                                {
                                    replyStream << "devotion";
                                    break;
                                }
                                case PaladinAuraType::PaladinAuraType_Retribution:
                                {
                                    replyStream << "retribution";
                                    break;
                                }
                                case PaladinAuraType::PaladinAuraType_FireResistant:
                                {
                                    replyStream << "fire";
                                    break;
                                }
                                case PaladinAuraType::PaladinAuraType_FrostResistant:
                                {
                                    replyStream << "frost";
                                    break;
                                }
                                case PaladinAuraType::PaladinAuraType_ShadowResistant:
                                {
                                    replyStream << "shadow";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                            }
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "pb")
                    {
                        if (pmReceiver->getClass() == Classes::CLASS_PALADIN)
                        {
                            std::ostringstream replyStream;
                            if (Script_Paladin* sp = (Script_Paladin*)receiverAI->sb)
                            {
                                if (commandVector.size() > 1)
                                {
                                    std::string blessingTypeName = commandVector.at(1);
                                    if (blessingTypeName == "kings")
                                    {
                                        sp->blessingType = PaladinBlessingType::PaladinBlessingType_Kings;
                                    }
                                    else if (blessingTypeName == "might")
                                    {
                                        sp->blessingType = PaladinBlessingType::PaladinBlessingType_Might;
                                    }
                                    else if (blessingTypeName == "wisdom")
                                    {
                                        sp->blessingType = PaladinBlessingType::PaladinBlessingType_Wisdom;
                                    }
                                    else if (blessingTypeName == "salvation")
                                    {
                                        sp->blessingType = PaladinBlessingType::PaladinBlessingType_Salvation;
                                    }
                                    else
                                    {
                                        replyStream << "Unknown type";
                                    }
                                }
                                switch (sp->blessingType)
                                {
                                case PaladinBlessingType::PaladinBlessingType_Kings:
                                {
                                    replyStream << "kings";
                                    break;
                                }
                                case PaladinBlessingType::PaladinBlessingType_Might:
                                {
                                    replyStream << "might";
                                    break;
                                }
                                case PaladinBlessingType::PaladinBlessingType_Wisdom:
                                {
                                    replyStream << "wisdom";
                                    break;
                                }
                                case PaladinBlessingType::PaladinBlessingType_Salvation:
                                {
                                    replyStream << "salvation";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                            }
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "ps")
                    {
                        if (pmReceiver->getClass() == Classes::CLASS_PALADIN)
                        {
                            std::ostringstream replyStream;
                            if (Script_Paladin* sp = (Script_Paladin*)receiverAI->sb)
                            {
                                if (commandVector.size() > 1)
                                {
                                    std::string sealTypeName = commandVector.at(1);
                                    if (sealTypeName == "righteousness")
                                    {
                                        sp->sealType = PaladinSealType::PaladinSealType_Righteousness;
                                    }
                                    else if (sealTypeName == "justice")
                                    {
                                        sp->sealType = PaladinSealType::PaladinSealType_Justice;
                                    }
                                    else if (sealTypeName == "command")
                                    {
                                        sp->sealType = PaladinSealType::PaladinSealType_Command;
                                    }
                                    else
                                    {
                                        replyStream << "Unknown type";
                                    }
                                }
                                switch (sp->sealType)
                                {
                                case PaladinSealType::PaladinSealType_Righteousness:
                                {
                                    replyStream << "righteousness";
                                    break;
                                }
                                case PaladinSealType::PaladinSealType_Justice:
                                {
                                    replyStream << "justice";
                                    break;
                                }
                                case PaladinSealType::PaladinSealType_Command:
                                {
                                    replyStream << "command";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                            }
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "ha")
                    {
                        if (pmReceiver->getClass() == Classes::CLASS_HUNTER)
                        {
                            std::ostringstream replyStream;
                            if (Script_Hunter* sh = (Script_Hunter*)receiverAI->sb)
                            {
                                if (commandVector.size() > 1)
                                {
                                    std::string aspectName = commandVector.at(1);
                                    if (aspectName == "hawk")
                                    {
                                        sh->aspectType = HunterAspectType::HunterAspectType_Hawk;
                                    }
                                    else if (aspectName == "monkey")
                                    {
                                        sh->aspectType = HunterAspectType::HunterAspectType_Monkey;
                                    }
                                    else if (aspectName == "wild")
                                    {
                                        sh->aspectType = HunterAspectType::HunterAspectType_Wild;
                                    }
                                    else if (aspectName == "pack")
                                    {
                                        sh->aspectType = HunterAspectType::HunterAspectType_Pack;
                                    }
                                    else
                                    {
                                        replyStream << "Unknown type";
                                    }
                                }
                                switch (sh->aspectType)
                                {
                                case HunterAspectType::HunterAspectType_Hawk:
                                {
                                    replyStream << "hawk";
                                    break;
                                }
                                case HunterAspectType::HunterAspectType_Monkey:
                                {
                                    replyStream << "monkey";
                                    break;
                                }
                                case HunterAspectType::HunterAspectType_Wild:
                                {
                                    replyStream << "wild";
                                    break;
                                }
                                case HunterAspectType::HunterAspectType_Pack:
                                {
                                    replyStream << "pack";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                            }
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "equip")
                    {
                        if (commandVector.size() > 1)
                        {
                            std::string equipType = commandVector.at(1);
                            if (equipType == "molten core")
                            {
                                if (pmReceiver->getClass() == Classes::CLASS_DRUID)
                                {
                                    for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
                                    {
                                        if (Item* currentEquip = pmReceiver->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
                                        {
                                            pmReceiver->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                                        }
                                    }
                                    EquipNewItem(pmReceiver, 16983, EquipmentSlots::EQUIPMENT_SLOT_HEAD);
                                    EquipNewItem(pmReceiver, 19139, EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS);
                                    EquipNewItem(pmReceiver, 16833, EquipmentSlots::EQUIPMENT_SLOT_CHEST);
                                    EquipNewItem(pmReceiver, 11764, EquipmentSlots::EQUIPMENT_SLOT_WRISTS);
                                    EquipNewItem(pmReceiver, 16831, EquipmentSlots::EQUIPMENT_SLOT_HANDS);
                                    EquipNewItem(pmReceiver, 19149, EquipmentSlots::EQUIPMENT_SLOT_WAIST);
                                    EquipNewItem(pmReceiver, 15054, EquipmentSlots::EQUIPMENT_SLOT_LEGS);
                                    EquipNewItem(pmReceiver, 16982, EquipmentSlots::EQUIPMENT_SLOT_FEET);
                                    EquipNewItem(pmReceiver, 18803, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND);
                                    EquipNewItem(pmReceiver, 2802, EquipmentSlots::EQUIPMENT_SLOT_TRINKET1);
                                    EquipNewItem(pmReceiver, 18406, EquipmentSlots::EQUIPMENT_SLOT_TRINKET2);
                                    EquipNewItem(pmReceiver, 18398, EquipmentSlots::EQUIPMENT_SLOT_FINGER1);
                                    EquipNewItem(pmReceiver, 18813, EquipmentSlots::EQUIPMENT_SLOT_FINGER2);
                                    EquipNewItem(pmReceiver, 18811, EquipmentSlots::EQUIPMENT_SLOT_BACK);
                                    EquipNewItem(pmReceiver, 16309, EquipmentSlots::EQUIPMENT_SLOT_NECK);
                                    std::ostringstream replyStream;
                                    replyStream << "Equip all fire resistance gears.";
                                    WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                                }
                            }
                            else if (equipType == "reset")
                            {
                                InitializeEquipments(pmReceiver, true);
                                std::ostringstream replyStream;
                                replyStream << "All my equipments are reset.";
                                WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                            }
                        }
                    }
                    else if (commandName == "rti")
                    {
                        int targetIcon = -1;
                        if (commandVector.size() > 1)
                        {
                            std::string iconIndex = commandVector.at(1);
                            targetIcon = atoi(iconIndex.c_str());
                        }
                        if (targetIcon >= 0 && targetIcon < TARGETICONCOUNT)
                        {
                            receiverAI->sb->rti = targetIcon;
                        }
                        std::ostringstream replyStream;
                        replyStream << "RTI is " << receiverAI->sb->rti;
                        WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                    }
                    else if (commandName == "assist")
                    {
                        if (receiverAI->sb->Assist(nullptr))
                        {
                            receiverAI->assistDelay = 5000;
                            std::ostringstream replyStream;
                            replyStream << "Try to pin down my RTI : " << receiverAI->sb->rti;
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
                    else if (commandName == "prepare")
                    {
                        sNingerManager->PrepareNinger(pmReceiver);
                    }
                    else if (commandName == "wlc")
                    {
                        if (pmReceiver->getClass() == Classes::CLASS_WARLOCK)
                        {
                            std::ostringstream replyStream;
                            if (Script_Warlock* swl = (Script_Warlock*)receiverAI->sb)
                            {
                                if (commandVector.size() > 1)
                                {
                                    std::string curseName = commandVector.at(1);
                                    if (curseName == "none")
                                    {
                                        swl->curseType = WarlockCurseType::WarlockCurseType_None;
                                    }
                                    else if (curseName == "element")
                                    {
                                        swl->curseType = WarlockCurseType::WarlockCurseType_Element;
                                    }
                                    else if (curseName == "weakness")
                                    {
                                        swl->curseType = WarlockCurseType::WarlockCurseType_Weakness;
                                    }
                                    else if (curseName == "tongues")
                                    {
                                        swl->curseType = WarlockCurseType::WarlockCurseType_Tongues;
                                    }
                                    else
                                    {
                                        replyStream << "Unknown type";
                                    }
                                }
                                switch (swl->curseType)
                                {
                                case WarlockCurseType::WarlockCurseType_None:
                                {
                                    replyStream << "none";
                                    break;
                                }
                                case WarlockCurseType::WarlockCurseType_Element:
                                {
                                    replyStream << "element";
                                    break;
                                }
                                case WarlockCurseType::WarlockCurseType_Weakness:
                                {
                                    replyStream << "weakness";
                                    break;
                                }
                                case WarlockCurseType::WarlockCurseType_Tongues:
                                {
                                    replyStream << "tongues";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                                }
                            }
                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, pmReceiver);
                        }
                    }
#pragma endregion
                }
            }
        }
    }
    else
    {
        if (Group* myGroup = pmSender->GetGroup())
        {
            if (commandName == "revive")
            {
                std::unordered_set<ObjectGuid> deadOGSet;
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (!member->IsAlive())
                        {
                            deadOGSet.insert(member->GetGUID());
                        }
                    }
                }
                std::unordered_set<ObjectGuid> revivingOGSet;
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (member->IsAlive())
                        {
                            if (!member->IsInCombat())
                            {
                                if (member->getClass() == Classes::CLASS_DRUID || member->getClass() == Classes::CLASS_PRIEST || member->getClass() == Classes::CLASS_PALADIN || member->getClass() == Classes::CLASS_SHAMAN)
                                {
                                    if (WorldSession* memberSession = member->GetSession())
                                    {
                                        if (memberSession->isNinger)
                                        {
                                            float manaRate = member->GetPower(Powers::POWER_MANA) * 100 / member->GetMaxPower(Powers::POWER_MANA);
                                            if (manaRate > 40)
                                            {
                                                for (std::unordered_set<ObjectGuid>::iterator dIT = deadOGSet.begin(); dIT != deadOGSet.end(); dIT++)
                                                {
                                                    if (ObjectGuid ogEachDead = *dIT)
                                                    {
                                                        if (revivingOGSet.find(ogEachDead) == revivingOGSet.end())
                                                        {
                                                            if (Player* eachDead = ObjectAccessor::FindPlayer(ogEachDead))
                                                            {
                                                                if (member->GetDistance(eachDead) < RANGE_HEAL_DISTANCE)
                                                                {
                                                                    if (Awareness_Base* memberAwareness = member->awarenessMap[member->activeAwarenessIndex])
                                                                    {
                                                                        if (Script_Base* sb = memberAwareness->sb)
                                                                        {
                                                                            sb->ogReviveTarget = eachDead->GetGUID();
                                                                            HandleChatCommand(pmSender, pmCMD, member);
                                                                            revivingOGSet.insert(eachDead->GetGUID());
                                                                            std::ostringstream replyStream;
                                                                            replyStream << "Try to revive ";
                                                                            replyStream << eachDead->GetName();
                                                                            WhisperTo(pmSender, replyStream.str(), Language::LANG_UNIVERSAL, member);
                                                                            break;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        HandleChatCommand(pmSender, pmCMD, member);
                    }
                }
            }
        }
    }
}

bool NingerManager::LearnPlayerTalents(Player* pmTargetPlayer)
{
    if (!pmTargetPlayer)
    {
        return false;
    }
    int freePoints = pmTargetPlayer->GetFreeTalentPoints();
    if (freePoints > 0)
    {
        pmTargetPlayer->ResetTalents(true);
        uint8 specialty = urand(0, 2);
        // EJ fixed specialty
        uint32 targetClass = pmTargetPlayer->getClass();
        if (targetClass == Classes::CLASS_MAGE)
        {
            specialty = 2;
        }
        else if (targetClass == Classes::CLASS_ROGUE)
        {
            specialty = 1;
        }
        else if (targetClass == Classes::CLASS_WARRIOR)
        {
            specialty = 2;
        }
        else if (targetClass == Classes::CLASS_SHAMAN)
        {
            specialty = 2;
        }
        else if (targetClass == Classes::CLASS_PRIEST)
        {
            specialty = 0;
        }
        else if (targetClass == Classes::CLASS_WARLOCK)
        {
            specialty = 2;
        }
        else if (targetClass == Classes::CLASS_PALADIN)
        {
            specialty = 0;
        }
        else if (targetClass == Classes::CLASS_DRUID)
        {
            specialty = 1;
        }
        else if (targetClass == Classes::CLASS_HUNTER)
        {
            specialty = 1;
        }
        std::unordered_map<uint32, std::unordered_set<uint32>> targetTalentSpecialtyMap = characterTalentLearningMap[pmTargetPlayer->getClass()][specialty];
        for (std::unordered_map<uint32, std::unordered_set<uint32>>::iterator talentSpecialtyMapIT = targetTalentSpecialtyMap.begin(); talentSpecialtyMapIT != targetTalentSpecialtyMap.end(); talentSpecialtyMapIT++)
        {
            std::unordered_set<uint32> talentSet = talentSpecialtyMapIT->second;
            std::map<uint32, std::vector<TalentEntry const*> > talentsMap;
            for (uint32 i = 0; i < sTalentStore.GetNumRows(); ++i)
            {
                TalentEntry const* talentInfo = sTalentStore.LookupEntry(i);
                if (talentInfo)
                {
                    if (TalentTabEntry const* talentTabInfo = sTalentTabStore.LookupEntry(talentInfo->TabID))
                    {
                        if (pmTargetPlayer->getClassMask() & talentTabInfo->ClassMask)
                        {
                            if (talentSet.find(talentInfo->SpellRank[0]) != talentSet.end())
                            {
                                talentsMap[talentInfo->TierID].push_back(talentInfo);
                            }                            
                        }
                    }
                }
            }
            for (std::map<uint32, std::vector<TalentEntry const*> >::iterator i = talentsMap.begin(); i != talentsMap.end(); ++i)
            {
                std::vector<TalentEntry const*> eachRowTalents = i->second;
                for (std::vector<TalentEntry const*>::iterator it = eachRowTalents.begin(); it != eachRowTalents.end(); it++)
                {
                    freePoints = pmTargetPlayer->GetFreeTalentPoints();
                    if (freePoints > 0)
                    {
                        if (const TalentEntry* eachTE = *it)
                        {
                            uint8 maxRank = 4;
                            if (eachTE->SpellRank[4] > 0)
                            {
                                maxRank = 4;
                            }
                            else if (eachTE->SpellRank[3] > 0)
                            {
                                maxRank = 3;
                            }
                            else if (eachTE->SpellRank[2] > 0)
                            {
                                maxRank = 2;
                            }
                            else if (eachTE->SpellRank[1] > 0)
                            {
                                maxRank = 1;
                            }
                            else
                            {
                                maxRank = 0;
                            }
                            if (maxRank > freePoints - 1)
                            {
                                maxRank = freePoints - 1;
                            }
                            pmTargetPlayer->LearnTalent(eachTE->ID, maxRank);
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        pmTargetPlayer->SaveToDB();
        return true;
    }

    return false;
}

void NingerManager::LearnPlayerSpells(Player* pmTargetPlayer)
{
    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        CreatureTemplate cInfo = itr->second;
        if (cInfo.trainer_class == pmTargetPlayer->getClass())
        {
            uint32 trainerID = sObjectMgr->GetCreatureTrainerForGossipOption(cInfo.Entry, cInfo.GossipMenuId, 0);
            if (const Trainer::Trainer* eachTrainer = sObjectMgr->GetTrainer(trainerID))
            {
                if (!eachTrainer->IsTrainerValidForPlayer(pmTargetPlayer))
                {
                    continue;
                }
                for (Trainer::Spell const& eachSpell : eachTrainer->_spells)
                {
                    if (!eachTrainer->CanTeachSpell(pmTargetPlayer, &eachSpell))
                    {
                        continue;
                    }
                    if (eachSpell.IsCastable())
                    {
                        pmTargetPlayer->CastSpell(pmTargetPlayer, eachSpell.SpellId, true);
                    }
                    else
                    {
                        pmTargetPlayer->LearnSpell(eachSpell.SpellId, false);
                    }                    
                }
            }
        }
    }
}

bool NingerManager::InitializeCharacter(Player* pmTargetPlayer, uint32 pmTargetLevel)
{
    if (!pmTargetPlayer)
    {
        return false;
    }
    pmTargetPlayer->RemoveAllAttackers();
    pmTargetPlayer->ClearInCombat();
    bool needToLogout = false;
    if (pmTargetPlayer->getLevel() != pmTargetLevel)
    {
        pmTargetPlayer->GiveLevel(pmTargetLevel);
        pmTargetPlayer->LearnDefaultSkills();
        pmTargetPlayer->LearnCustomSpells();

        for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; i++)
        {
            if (Item* pItem = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, i, true);
            }
        }
    }
    if (LearnPlayerTalents(pmTargetPlayer))
    {
        needToLogout = true;
    }
    LearnPlayerSpells(pmTargetPlayer);    

    bool resetEquipments = false;
    if (needToLogout)
    {
        resetEquipments = true;
    }
    InitializeEquipments(pmTargetPlayer, resetEquipments);
    std::ostringstream msgStream;
    msgStream << pmTargetPlayer->GetName() << " initialized";
    sWorld->SendServerMessage(ServerMessageType::SERVER_MSG_STRING, msgStream.str().c_str());

    return needToLogout;
}

void NingerManager::InitializeEquipments(Player* pmTargetPlayer, bool pmReset)
{
    if (pmReset)
    {
        for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; ++slot)
        {
            if (Item* inventoryItem = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            {
                pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
            }
        }
        for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
        {
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
            }
        }
    }
    uint32 minQuality = ItemQualities::ITEM_QUALITY_UNCOMMON;
    if (pmTargetPlayer->getLevel() < 20)
    {
        minQuality = ItemQualities::ITEM_QUALITY_POOR;
    }
    for (uint32 checkEquipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD; checkEquipSlot < EquipmentSlots::EQUIPMENT_SLOT_TABARD; checkEquipSlot++)
    {
        if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HEAD || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_CHEST || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WAIST || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_LEGS || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FEET || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_WRISTS || checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HANDS)
        {
            if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_HEAD)
            {
                if (pmTargetPlayer->getLevel() < 30)
                {
                    continue;
                }
            }
            else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS)
            {
                if (pmTargetPlayer->getLevel() < 20)
                {
                    continue;
                }
            }
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            std::unordered_set<uint32> usableItemClass;
            std::unordered_set<uint32> usableItemSubClass;
            usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
            usableItemSubClass.insert(GetUsableArmorSubClass(pmTargetPlayer));
            TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
        }
        else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_MAINHAND)
        {
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            int weaponSubClass_mh = -1;
            int weaponSubClass_oh = -1;
            int weaponSubClass_r = -1;
            switch (pmTargetPlayer->getClass())
            {
            case Classes::CLASS_WARRIOR:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
                weaponSubClass_oh = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD;
                break;
            }
            case Classes::CLASS_PALADIN:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE;
                uint32 weaponType = urand(0, 100);
                if (weaponType < 50)
                {
                    weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
                }
                weaponSubClass_oh = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD;
                break;
            }
            case Classes::CLASS_HUNTER:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_AXE2;
                uint32 rType = urand(0, 2);
                if (rType == 0)
                {
                    weaponSubClass_r = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_BOW;
                }
                else if (rType == 1)
                {
                    weaponSubClass_r = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_CROSSBOW;
                }
                else
                {
                    weaponSubClass_r = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_GUN;
                }
                break;
            }
            case Classes::CLASS_ROGUE:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
                weaponSubClass_oh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_SWORD;
                break;
            }
            case Classes::CLASS_PRIEST:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
                break;
            }
            case Classes::CLASS_SHAMAN:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_MACE;
                weaponSubClass_oh = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_SHIELD;
                break;
            }
            case Classes::CLASS_MAGE:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
                break;
            }
            case Classes::CLASS_WARLOCK:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
                break;
            }
            case Classes::CLASS_DRUID:
            {
                weaponSubClass_mh = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
                break;
            }
            default:
            {
                continue;
            }
            }
            if (weaponSubClass_mh >= 0)
            {
                std::unordered_set<uint32> usableItemClass;
                std::unordered_set<uint32> usableItemSubClass;
                usableItemClass.insert(ItemClass::ITEM_CLASS_WEAPON);
                usableItemSubClass.insert(weaponSubClass_mh);
                TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
            }
            if (weaponSubClass_oh >= 0)
            {
                std::unordered_set<uint32> usableItemClass;
                std::unordered_set<uint32> usableItemSubClass;
                usableItemClass.insert(ItemClass::ITEM_CLASS_WEAPON);
                usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
                usableItemSubClass.insert(weaponSubClass_oh);
                TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND);
            }
            if (weaponSubClass_r >= 0)
            {
                std::unordered_set<uint32> usableItemClass;
                std::unordered_set<uint32> usableItemSubClass;
                usableItemClass.insert(ItemClass::ITEM_CLASS_WEAPON);
                usableItemSubClass.insert(weaponSubClass_r);
                TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, EquipmentSlots::EQUIPMENT_SLOT_RANGED);
            }
        }
        else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_BACK)
        {
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            std::unordered_set<uint32> usableItemClass;
            std::unordered_set<uint32> usableItemSubClass;
            usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
            usableItemSubClass.insert(ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH);
            TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
        }
        else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER1)
        {
            if (pmTargetPlayer->getLevel() < 20)
            {
                continue;
            }
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            std::unordered_set<uint32> usableItemClass;
            std::unordered_set<uint32> usableItemSubClass;
            usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
            usableItemSubClass.insert(ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISCELLANEOUS);
            TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
        }
        else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_FINGER2)
        {
            if (pmTargetPlayer->getLevel() < 20)
            {
                continue;
            }
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            std::unordered_set<uint32> usableItemClass;
            std::unordered_set<uint32> usableItemSubClass;
            usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
            usableItemSubClass.insert(ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISCELLANEOUS);
            TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
        }
        else if (checkEquipSlot == EquipmentSlots::EQUIPMENT_SLOT_NECK)
        {
            if (pmTargetPlayer->getLevel() < 30)
            {
                continue;
            }
            if (Item* currentEquip = pmTargetPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, checkEquipSlot))
            {
                if (const ItemTemplate* checkIT = currentEquip->GetTemplate())
                {
                    if (checkIT->GetQuality() >= minQuality)
                    {
                        continue;
                    }
                    else
                    {
                        pmTargetPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, checkEquipSlot, true);
                    }
                }
            }
            std::unordered_set<uint32> usableItemClass;
            std::unordered_set<uint32> usableItemSubClass;
            usableItemClass.insert(ItemClass::ITEM_CLASS_ARMOR);
            usableItemSubClass.insert(ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MISCELLANEOUS);
            TryEquip(pmTargetPlayer, usableItemClass, usableItemSubClass, checkEquipSlot);
        }
    }
}

uint32 NingerManager::GetUsableArmorSubClass(Player* pmTargetPlayer)
{
    if (!pmTargetPlayer)
    {
        return false;
    }
    uint32 resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
    switch (pmTargetPlayer->getClass())
    {
    case Classes::CLASS_WARRIOR:
    {
        if (pmTargetPlayer->getLevel() < 40)
        {
            // use mail armor
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
        }
        else
        {
            // use plate armor
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE;
        }
        break;
    }
    case Classes::CLASS_PALADIN:
    {
        if (pmTargetPlayer->getLevel() < 40)
        {
            // use mail armor
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
        }
        else
        {
            // use plate armor
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_PLATE;
        }
        break;
    }
    case Classes::CLASS_HUNTER:
    {
        if (pmTargetPlayer->getLevel() < 40)
        {
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
        }
        else
        {
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
        }
        break;
    }
    case Classes::CLASS_ROGUE:
    {
        resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
        break;
    }
    case Classes::CLASS_PRIEST:
    {
        resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
        break;
    }
    case Classes::CLASS_SHAMAN:
    {
        if (pmTargetPlayer->getLevel() < 40)
        {
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
        }
        else
        {
            resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_MAIL;
        }
        break;
    }
    case Classes::CLASS_MAGE:
    {
        resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
        break;
    }
    case Classes::CLASS_WARLOCK:
    {
        resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
        break;
    }
    case Classes::CLASS_DRUID:
    {
        resultArmorSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_LEATHER;
        break;
    }
    default:
    {
        break;
    }
    }

    return resultArmorSubClass;
}

bool NingerManager::EquipNewItem(Player* pmTargetPlayer, uint32 pmItemEntry, uint8 pmEquipSlot)
{
    if (!pmTargetPlayer)
    {
        return false;
    }
    uint16 eDest;
    InventoryResult tryEquipResult = pmTargetPlayer->CanEquipNewItem(NULL_SLOT, eDest, pmItemEntry, false);
    if (tryEquipResult == EQUIP_ERR_OK)
    {
        ItemPosCountVec sDest;
        InventoryResult storeResult = pmTargetPlayer->CanStoreNewItem(INVENTORY_SLOT_BAG_0, NULL_SLOT, sDest, pmItemEntry, 1);
        if (storeResult == EQUIP_ERR_OK)
        {
            Item* pItem = pmTargetPlayer->StoreNewItem(sDest, pmItemEntry, true, GenerateItemRandomPropertyId(pmItemEntry));
            if (pItem)
            {
                InventoryResult equipResult = pmTargetPlayer->CanEquipItem(NULL_SLOT, eDest, pItem, false);
                if (equipResult == EQUIP_ERR_OK)
                {
                    pmTargetPlayer->RemoveItem(INVENTORY_SLOT_BAG_0, pItem->GetSlot(), true);
                    pmTargetPlayer->EquipItem(pmEquipSlot, pItem, true);
                    return true;
                }
                else
                {
                    pItem->DestroyForPlayer(pmTargetPlayer);
                }
            }
        }
    }

    return false;
}

void NingerManager::TryEquip(Player* pmTargetPlayer, std::unordered_set<uint32> pmClassSet, std::unordered_set<uint32> pmSubClassSet, uint32 pmTargetSlot)
{
    if (!pmTargetPlayer)
    {
        return;
    }
    uint32 minQuality = ItemQualities::ITEM_QUALITY_UNCOMMON;
    if (pmTargetPlayer->getLevel() < 20)
    {
        minQuality = ItemQualities::ITEM_QUALITY_POOR;
    }
    std::unordered_map<uint32, uint32> validEquipSet;
    const ItemTemplateContainer* its = sObjectMgr->GetItemTemplateStore();
    for (ItemTemplateContainer::const_iterator itr = its->begin(); itr != its->end(); ++itr)
    {
        const ItemTemplate* proto = sObjectMgr->GetItemTemplate(itr->first);
        if (!proto)
        {
            continue;
        }
        if (pmClassSet.find(proto->GetClass()) == pmClassSet.end())
        {
            continue;
        }
        if (pmSubClassSet.find(proto->GetSubClass()) == pmSubClassSet.end())
        {
            continue;
        }
        if (proto->GetQuality() < minQuality || proto->GetQuality() > ItemQualities::ITEM_QUALITY_EPIC)
        {
            continue;
        }
        // test items
        if (proto->GetId() == 19879)
        {
            continue;
        }
        if (proto->GetClass() == ItemClass::ITEM_CLASS_WEAPON)
        {
            float minDMG = 0.0f, maxDMG = 0.0f, dps = 0.0f;
            proto->GetWeaponDamage(nullptr, minDMG, maxDMG, dps);
            if (minDMG > 10.0f && maxDMG > 10.0f)
            {
                // valid weapon 
            }
            else
            {
                continue;
            }
            if (proto->GetSubClass() == ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF)
            {
                if (pmTargetPlayer->getClass() == Classes::CLASS_WARLOCK || pmTargetPlayer->getClass() == Classes::CLASS_PRIEST || pmTargetPlayer->getClass() == Classes::CLASS_MAGE)
                {
                    bool hasIT = false;
                    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
                    {
                        if (proto->GetItemStatType(i) == ItemModType::ITEM_MOD_SPELL_POWER)
                        {
                            hasIT = true;
                            break;
                        }
                    }
                    if (!hasIT)
                    {
                        continue;
                    }
                }
            }
        }
        std::unordered_set<uint32> usableSlotSet = GetUsableEquipSlot(proto);
        if (usableSlotSet.find(pmTargetSlot) != usableSlotSet.end())
        {
            uint32 checkMinRequiredLevel = pmTargetPlayer->getLevel();
            if (checkMinRequiredLevel > 10)
            {
                checkMinRequiredLevel = checkMinRequiredLevel - 5;
            }
            else
            {
                checkMinRequiredLevel = 5;
            }
            if (proto->GetRequiredLevel() <= pmTargetPlayer->getLevel() && proto->GetRequiredLevel() >= checkMinRequiredLevel)
            {
                validEquipSet[validEquipSet.size()] = proto->GetId();
            }
        }
    }
    if (validEquipSet.size() > 0)
    {
        int tryTimes = 5;
        while (tryTimes > 0)
        {
            uint32 equipEntry = urand(0, validEquipSet.size() - 1);
            equipEntry = validEquipSet[equipEntry];
            if (EquipNewItem(pmTargetPlayer, equipEntry, pmTargetSlot))
            {
                break;
            }
            tryTimes--;
        }
    }
}

void NingerManager::RandomTeleport(Player* pmTargetPlayer)
{
    if (!pmTargetPlayer)
    {
        return;
    }
    if (pmTargetPlayer->IsBeingTeleported())
    {
        return;
    }
    if (!pmTargetPlayer->IsAlive())
    {
        pmTargetPlayer->ResurrectPlayer(1.0f);
        pmTargetPlayer->SpawnCorpseBones();
    }
    pmTargetPlayer->ClearInCombat();
    pmTargetPlayer->StopMoving();
    pmTargetPlayer->GetMotionMaster()->Initialize();
    std::unordered_map<uint32, ObjectGuid> sameLevelPlayerOGMap;
    std::unordered_map<uint32, WorldSession*> allSessions = sWorld->GetAllSessions();
    for (std::unordered_map<uint32, WorldSession*>::iterator wsIT = allSessions.begin(); wsIT != allSessions.end(); wsIT++)
    {
        if (WorldSession* eachWS = wsIT->second)
        {
            if (!eachWS->isNinger)
            {
                if (Player* eachPlayer = eachWS->GetPlayer())
                {
                    if (eachPlayer->getLevel() == pmTargetPlayer->getLevel())
                    {
                        if (!eachPlayer->IsBeingTeleported())
                        {
                            if (Map* eachMap = eachPlayer->GetMap())
                            {
                                if (!eachMap->Instanceable())
                                {
                                    if (AreaTableEntry const* zone = sAreaTableStore.LookupEntry(eachPlayer->GetAreaId()))
                                    {
                                        FactionTemplateEntry const* factionTemplate = eachPlayer->GetFactionTemplateEntry();
                                        if (!factionTemplate || factionTemplate->FriendGroup & zone->FactionGroupMask)
                                        {
                                            // friendly realm
                                        }
                                        else if (factionTemplate->EnemyGroup & zone->FactionGroupMask)
                                        {
                                            // hostile realm 
                                        }
                                        else
                                        {
                                            // neutral realm
                                            sameLevelPlayerOGMap[sameLevelPlayerOGMap.size()] = eachPlayer->GetGUID();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Player* destPlayer = NULL;
    if (sameLevelPlayerOGMap.size() > 0)
    {
        uint32 destPlayerIndex = urand(0, sameLevelPlayerOGMap.size() - 1);
        destPlayer = ObjectAccessor::FindPlayer(sameLevelPlayerOGMap[destPlayerIndex]);
    }
    else
    {
        destPlayer = pmTargetPlayer;
    }
    if (destPlayer)
    {
        float destX = 0.0f, destY = 0.0f, destZ = 0.0f;
        if (Unit* nearbyUnit = GetAnyUnitInRange(destPlayer, sNingerConfig->NingerRandomTeleportMinRange, sNingerConfig->NingerRandomTeleportMaxRange))
        {
            destX = nearbyUnit->GetPositionX();
            destY = nearbyUnit->GetPositionY();
            destZ = nearbyUnit->GetPositionZ();
        }
        else
        {
            float angle = frand(0, 2 * M_PI);
            float distance = frand(sNingerConfig->NingerRandomTeleportMinRange, sNingerConfig->NingerRandomTeleportMaxRange);
            destPlayer->GetNearPoint(destPlayer, destX, destY, destZ, distance, angle);
        }
        pmTargetPlayer->TeleportTo(destPlayer->GetMapId(), destX, destY, destZ, 0.0f);
        sLog->outMessage("ninger", LogLevel::LOG_LEVEL_INFO, "Teleport ninger %s (level %d)", pmTargetPlayer->GetName(), pmTargetPlayer->getLevel());
    }
}

bool NingerManager::TankThreatOK(Player* pmTankPlayer, Unit* pmVictim)
{
    if (pmTankPlayer && pmVictim)
    {
        if (pmTankPlayer->IsAlive() && pmVictim->IsAlive())
        {
            switch (pmTankPlayer->getClass())
            {
            case Classes::CLASS_WARRIOR:
            {
                if (HasAura(pmVictim, "Sunder Armor"))
                {
                    return true;
                }
                //if (GetAuraStack(pmVictim, "Sunder Armor", pmTankPlayer) > 2)
                //{
                //	return true;
                //}
                break;
            }
            case Classes::CLASS_PALADIN:
            {
                if (HasAura(pmVictim, "Judgement of the Crusader"))
                {
                    return true;
                }
                break;
            }
            case Classes::CLASS_DRUID:
            {
                return true;
            }
            default:
            {
                break;
            }
            }
        }
    }
    return false;
}

bool NingerManager::HasAura(Unit* pmTarget, std::string pmSpellName, Unit* pmCaster)
{
    if (!pmTarget)
    {
        return false;
    }
    std::set<uint32> spellIDSet = spellNameEntryMap[pmSpellName];
    for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
    {
        uint32 spellID = *it;
        ObjectGuid ogCaster = ObjectGuid::Empty;
        if (pmCaster)
        {
            ogCaster = pmCaster->GetGUID();
        }
        if (pmTarget->HasAura(spellID, ogCaster))
        {
            return true;
        }
    }

    return false;
}

uint32 NingerManager::GetAuraDuration(Unit* pmTarget, std::string pmSpellName, Unit* pmCaster)
{
    if (!pmTarget)
    {
        return false;
    }
    uint32 duration = 0;
    std::set<uint32> spellIDSet = spellNameEntryMap[pmSpellName];
    for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
    {
        uint32 spellID = *it;
        ObjectGuid ogCaster = ObjectGuid::Empty;
        if (pmCaster)
        {
            ogCaster = pmCaster->GetGUID();
        }
        if (Aura* targetAura = pmTarget->GetAura(spellID, ogCaster))
        {
            duration = targetAura->GetDuration();
            if (duration > 0)
            {
                break;
            }
        }
    }

    return duration;
}

uint32 NingerManager::GetAuraStack(Unit* pmTarget, std::string pmSpellName, Unit* pmCaster)
{
    uint32 auraStack = 0;
    if (!pmTarget)
    {
        return false;
    }
    std::set<uint32> spellIDSet = spellNameEntryMap[pmSpellName];
    for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
    {
        uint32 spellID = *it;
        ObjectGuid ogCaster = ObjectGuid::Empty;
        if (pmCaster)
        {
            ogCaster = pmCaster->GetGUID();
        }
        if (Aura* targetAura = pmTarget->GetAura(spellID, ogCaster))
        {
            auraStack = targetAura->GetStackAmount();
            if (auraStack > 0)
            {
                break;
            }
        }
    }

    return auraStack;
}

Player* NingerManager::GetNearbyHostilePlayer(Player* pmSearcher, float pmRange)
{
    std::list<Player*> players;
    Trinity::AnyPlayerInObjectRangeCheck checker(pmSearcher, pmRange);
    Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(pmSearcher, players, checker);
    Cell::VisitWorldObjects(pmSearcher, searcher, pmRange);
    if (!players.empty())
    {
        for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* eachPlayer = *itr)
            {
                if (eachPlayer->IsAlive())
                {
                    if (pmSearcher->IsValidAttackTarget(eachPlayer))
                    {
                        return eachPlayer;
                    }
                }
            }
        }
    }

    return nullptr;
}

Unit* NingerManager::GetAnyUnitInRange(Player* pmSearcher, float pmMinRange, float pmMaxRange)
{
    std::list<Unit*> unitList;
    Trinity::AnyUnitInObjectRangeCheck go_check(pmSearcher, pmMaxRange);
    Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> go_search(pmSearcher, unitList, go_check);
    Cell::VisitGridObjects(pmSearcher, go_search, pmMaxRange);
    if (!unitList.empty())
    {
        std::unordered_map<uint32, Unit*> unitsMap;
        unitsMap.clear();
        for (std::list<Unit*>::iterator uIT = unitList.begin(); uIT != unitList.end(); uIT++)
        {
            if (Unit* eachUnit = *uIT)
            {
                float eachDistance = pmSearcher->GetDistance(eachUnit);
                if (eachDistance > pmMinRange)
                {
                    unitsMap[unitsMap.size()] = eachUnit;
                }
            }
        }
        if (unitsMap.size() > 0)
        {
            uint32 targetIndex = urand(0, unitsMap.size() - 1);
            return unitsMap[targetIndex];
        }
    }

    return nullptr;
}
