/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "TemporarySummon.h"

#include "GridNotifiers.h"

 //#define LESS_MOB // if you do not have a good server and do not want it to be laggy as hell
  //Light of Dawn
enum mograine
{
    ENCOUNTER_DK_NUMBER = 5,  // how many player queue to start the quest, or -
    ENCOUNTER_DK_TIMER = 10, // *every 5 minutes. These have to be done in instance data
    ENCOUNTER_DEFENDER_NUMBER = 4, // how many of defender    
    ENCOUNTER_ARGENT_PROTECTOR = 20, // how many of argent protector    
    ENCOUNTER_ABOMINATION_NUMBER = 6,  // how many of abomination
    ENCOUNTER_BEHEMOTH_NUMBER = 2,  // how many of behemoth
    ENCOUNTER_GHOUL_NUMBER = 30, // how many of ghoul
    ENCOUNTER_WARRIOR_NUMBER = 8,  // how many of warrior

    ENCOUNTER_TOTAL_DAWN = 300,  // Total number
    ENCOUNTER_TOTAL_SCOURGE = 10000,

    WORLD_STATE_REMAINS = 3592,
    WORLD_STATE_COUNTDOWN = 3603,
    WORLD_STATE_EVENT_BEGIN = 3605,

    SAY_LIGHT_OF_DAWN01 = 0, // pre text
    SAY_LIGHT_OF_DAWN02 = 1,
    SAY_LIGHT_OF_DAWN03 = 2,
    SAY_LIGHT_OF_DAWN04 = 3, // intro
    SAY_LIGHT_OF_DAWN05 = 4,
    SAY_LIGHT_OF_DAWN06 = 5,
    SAY_LIGHT_OF_DAWN07 = 6, // During the fight - Korfax, Champion of the Light
    SAY_LIGHT_OF_DAWN08 = 7, // Lord Maxwell Tyrosus
    SAY_LIGHT_OF_DAWN09 = 8, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN25 = 24, // After the fight
    SAY_LIGHT_OF_DAWN26 = 25, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN27 = 26, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN28 = 27, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN29 = 28, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN30 = 29, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN31 = 30, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN32 = 31, // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN33 = 32, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN34 = 33, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN35 = 34, // Darion Mograine
    SAY_LIGHT_OF_DAWN36 = 35, // Darion Mograine
    SAY_LIGHT_OF_DAWN37 = 36, // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN38 = 37, // Darion Mograine
    SAY_LIGHT_OF_DAWN39 = 38, // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN40 = 39, // Darion Mograine
    SAY_LIGHT_OF_DAWN41 = 40, // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN42 = 41, // Highlord Alexandros Mograine
    SAY_LIGHT_OF_DAWN43 = 42, // The Lich King
    SAY_LIGHT_OF_DAWN44 = 43, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN45 = 44, // The Lich King
    SAY_LIGHT_OF_DAWN46 = 45, // The Lich King
    SAY_LIGHT_OF_DAWN47 = 46, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN48 = 47, // The Lich King
    SAY_LIGHT_OF_DAWN49 = 48, // The Lich King
    SAY_LIGHT_OF_DAWN50 = 49, // Lord Maxwell Tyrosus
    SAY_LIGHT_OF_DAWN51 = 50, // The Lich King
    SAY_LIGHT_OF_DAWN52 = 51, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN53 = 52, // Highlord Darion Mograine
    SAY_LIGHT_OF_DAWN54 = 53, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN55 = 54, // The Lich King
    SAY_LIGHT_OF_DAWN56 = 55, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN57 = 56, // The Lich King
    SAY_LIGHT_OF_DAWN58 = 57, // The Lich King
    SAY_LIGHT_OF_DAWN59 = 58, // The Lich King
    SAY_LIGHT_OF_DAWN60 = 59, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN61 = 60, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN62 = 61, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN63 = 62, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN64 = 63, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN65 = 64, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN66 = 65, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN67 = 66, // Highlord Tirion Fordring
    SAY_LIGHT_OF_DAWN68 = 67, // Highlord Darion Mograine

    EMOTE_LIGHT_OF_DAWN01 = 68,  // Emotes
    EMOTE_LIGHT_OF_DAWN02 = 69,
    EMOTE_LIGHT_OF_DAWN03 = 70,
    EMOTE_LIGHT_OF_DAWN04 = 71,
    EMOTE_LIGHT_OF_DAWN05 = 72,
    EMOTE_LIGHT_OF_DAWN06 = 73,
    EMOTE_LIGHT_OF_DAWN07 = 74,
    EMOTE_LIGHT_OF_DAWN08 = 75,
    EMOTE_LIGHT_OF_DAWN09 = 76,
    EMOTE_LIGHT_OF_DAWN10 = 77,
    EMOTE_LIGHT_OF_DAWN11 = 78,
    EMOTE_LIGHT_OF_DAWN12 = 79,
    EMOTE_LIGHT_OF_DAWN13 = 80,
    EMOTE_LIGHT_OF_DAWN14 = 81,
    EMOTE_LIGHT_OF_DAWN15 = 82,
    EMOTE_LIGHT_OF_DAWN16 = 83,
    EMOTE_LIGHT_OF_DAWN17 = 84,
    EMOTE_LIGHT_OF_DAWN18 = 85,

    GO_LIGHT_OF_DAWN = 191330,
    SPELL_THE_LIGHT_OF_DAWN_Q = 53606, // quest credit

    // ---- Dark Knight npc --------------------
    // Highlord Darion Mograine
    NPC_HIGHLORD_DARION_MOGRAINE = 29173,
    SPELL_ANTI_MAGIC_ZONE1 = 52893,
    SPELL_DEATH_STRIKE = 53639,
    SPELL_DEATH_EMBRACE = 53635,
    SPELL_ICY_TOUCH1 = 49723,
    SPELL_THE_LIGHT_OF_DAWN = 53658,
    SPELL_THE_MIGHT_OF_MOGRAINE = 53642, // on players when begins
    SPELL_UNHOLY_BLIGHT = 53640,
    SPELL_ALEXANDROS_MOGRAINE_SPAWN = 53667,
    SPELL_MOGRAINE_CHARGE = 53679,
    SPELL_ASHBRINGER = 53701,

    // Koltira Deathweaver & Orbaz Bloodbane are using the same abilities
    NPC_KOLTIRA_DEATHWEAVER = 29199,
    NPC_ORBAZ_BLOODBANE = 29204, // this guy fleed
    NPC_THASSARIAN = 29200, // he also does SPELL_THE_LIGHT_OF_DAWN 53658
    SPELL_BLOOD_STRIKE1 = 52374,
    SPELL_DEATH_GRIP = 49576,
    SPELL_ICY_TOUCH2 = 52372,
    SPELL_PLAGUE_STRIKE1 = 50668,
    // all do SPELL_HERO_AGGRO_AURA    53627

    // Lich King
    NPC_THE_LICH_KING = 29183, // show up at end
    SPELL_APOCALYPSE = 53210,
    SPELL_TELEPORT_VISUAL = 52233,
    SPELL_SOUL_FEAST_ALEX = 53677, // on Alexandros
    SPELL_SOUL_FEAST_TIRION = 53685, // on Tirion
    SPELL_ICEBOUND_VISAGE = 53274, // not sure what is it for
    SPELL_REBUKE = 53680,

    // others
    NPC_RAMPAGING_ABOMINATION = 29186,
    SPELL_CLEAVE1 = 53633,
    SPELL_SCOURGE_HOOK = 50335,
    SPELL_SCOURGE_AGGRO_AURA = 53624,

    NPC_FLESH_BEHEMOTH = 29190, // giant guy
    SPELL_STOMP = 53634,
    SPELL_THUNDERCLAP = 36706,
    SPELL_HERO_AGGRO_AURA = 53627,

    NPC_ACHERUS_GHOUL = 29185, // just ghoul....
    SPELL_GHOULPLOSION = 53632,

    NPC_WARRIOR_OF_THE_FROZEN_WASTES = 29206, // use SPELL_CLEAVE 53631

    NPC_HIGHLORD_ALEXANDROS_MOGRAINE = 29227, // ghost
    NPC_DARION_MOGRAINE = 29228, // ghost

    // ---- Dawn npc --------------------
    // Highlord Tirion Fordring
    NPC_HIGHLORD_TIRION_FORDRING = 29175,
    EQUIP_HIGHLORD_TIRION_FORDRING = 13262,
    SPELL_LAY_ON_HANDS = 53778,
    SPELL_REBIRTH_OF_THE_ASHBRINGER = 53702,
    SPELL_TIRION_CHARGE = 53705,
    SPELL_TIRION_CHARGE_VISUAL = 53706,

    // others
    NPC_KORFAX_CHAMPION_OF_THE_LIGHT = 29176,
    SPELL_CLEAVE = 53631,
    SPELL_HEROIC_LEAP = 53625,

    NPC_LORD_MAXWELL_TYROSUS = 29178,
    NPC_LEONID_BARTHALOMEW_THE_REVERED = 29179,
    NPC_DUKE_NICHOLAS_ZVERENHOFF = 29180,

    NPC_COMMANDER_ELIGOR_DAWNBRINGER = 29177,
    SPELL_HOLY_LIGHT2 = 37979,

    NPC_RAYNE = 29181,
    SPELL_REJUVENATION = 20664,
    SPELL_STARFALL = 20678,
    SPELL_TRANQUILITY = 25817,
    SPELL_WRATH = 21807,

    NPC_ARGENT_PROTECTOR = 19320,
    NPC_DEFENDER_OF_THE_LIGHT = 29174, // also does SPELL_HEROIC_LEAP 53625
    SPELL_HOLY_LIGHT1 = 29427,
    SPELL_HOLY_STRIKE = 53643,
    SPELL_HOLY_WRATH = 53638,
    SPELL_UPPERCUT = 53629,

    NPC_RIMBLAT_EARTHSHATTER = 29182,
    SPELL_CHAIN_HEAL = 33642,
    SPELL_THUNDER = 53630
};

void UpdateWorldState(Map* map, uint32 id, uint32 state)
{
    Map::PlayerList const& players = map->GetPlayers();

    if (!players.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* player = itr->GetSource())
                player->SendUpdateWorldState(id, state);
        }
    }
}

Position const LightofDawnLoc[] =
{
    {2281.335f, -5300.409f, 85.170f, 0},     // 0 Tirion Fordring loc
    {2283.896f, -5287.914f, 83.066f, 1.55f}, // 1 Tirion Fordring loc2
    {2281.461f, -5263.014f, 81.164f, 0},     // 2 Tirion charges
    {2262.277f, -5293.477f, 82.167f, 0},     // 3 Tirion run
    {2270.286f, -5287.73f, 82.262f, 0},      // 4 Tirion relocate
    {2269.511f, -5288.289f, 82.225f, 0},     // 5 Tirion forward
    {2262.277f, -5293.477f, 82.167f, 0},     // 6 Tirion runs to Darion
    {2270.286f, -5287.73f, 82.262f, 0},
    {2269.511f, -5288.289f, 82.225f, 0},
    {2273.205f, -5288.848f, 82.617f, 0},     // 9 Korfax loc1
    {2274.739f, -5287.926f, 82.684f, 0},     // 10 Korfax loc2
    {2253.673f, -5318.004f, 81.724f, 0},     // 11 Korfax kicked
    {2287.028f, -5309.644f, 87.253f, 0},     // 12 Maxwell loc1
    {2286.978f, -5308.025f, 86.83f, 0},      // 13 Maxwell loc2
    {2248.877f, -5307.586f, 82.166f, 0},     // 14 maxwell kicked
    {2278.58f, -5316.933f, 88.319f, 0},      // 15 Eligor loc1
    {2278.535f, -5315.479f, 88.08f, 0},      // 16 Eligor loc2
    {2259.416f, -5304.505f, 82.149f, 0},     // 17 eligor kicked
    {2289.259f, -5280.355f, 82.112f, 0},     // 18 Koltira loc1
    {2289.02f, -5281.985f, 82.207f, 0},      // 19 Koltira loc2
    {2273.289f, -5273.675f, 81.701f, 0},     // 20 Thassarian loc1
    {2273.332f, -5275.544f, 81.849f, 0},     // 21 Thassarian loc2
    {2281.198f, -5257.397f, 80.224f, 4.66f}, // 22 Alexandros loc1
    {2281.156f, -5259.934f, 80.647f, 0},     // 23 Alexandros loc2
    {2281.294f, -5281.895f, 82.445f, 1.35f}, // 24 Darion loc1
    {2281.093f, -5263.013f, 81.125f, 0},     // 25 Darion loc1
    {2281.313f, -5250.282f, 79.322f, 4.69f}, // 26 Lich King spawns
    {2281.523f, -5261.058f, 80.877f, 0},     // 27 Lich king move forwards
    {2272.709f, -5255.552f, 78.226f, 0},     // 28 Lich king kicked
    {2273.972f, -5257.676f, 78.862f, 0},     // 29 Lich king moves forward
    {2254.0f, -5298.0f, 82.2f, 0},     // 30 darion last
    {2630.0f, -5243.0f, 108.6f, 0},     // 31 scurge rise point 
};

class npc_highlord_darion_mograine : public CreatureScript
{
public:
    npc_highlord_darion_mograine() : CreatureScript("npc_highlord_darion_mograine") { }

    struct npc_highlord_darion_mograineAI : public ScriptedAI
    {
        npc_highlord_darion_mograineAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        bool bIsBattle;
        uint32 dawnLineDelay;

        uint32 uiStep;
        uint32 uiPhase_timer;
        uint32 uiTotal_dawn;
        uint32 uiTotal_scourge;

        // Darion Mograine
        uint32 uiAnti_magic_zone;
        uint32 uiDeath_strike;
        uint32 uiDeath_embrace;
        uint32 uiIcy_touch;
        uint32 uiUnholy_blight;
        uint32 uiFight_speech;        
        uint32 uiNPCEngagingCheck;

        // Dawn
        ObjectGuid uiTirionGUID;
        ObjectGuid uiAlexandrosGUID;
        ObjectGuid uiDarionGUID;
        ObjectGuid uiKorfaxGUID;
        ObjectGuid uiMaxwellGUID;
        ObjectGuid uiEligorGUID;
        ObjectGuid uiRayneGUID;
        ObjectGuid uiRimblatGUID;

        // Death
        ObjectGuid uiKoltiraGUID;
        ObjectGuid uiOrbazGUID;
        ObjectGuid uiThassarianGUID;
        ObjectGuid uiLichKingGUID;
        std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>> scurgeSpawnsGUIDMap;
        std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>> dawnSpawnsGUIDMap;
        std::unordered_map<uint32, uint32> scurgeSpawnsAmountLimitMap;
        std::unordered_map<uint32, uint32> dawnSpawnsAmountLimitMap;

        void Initialize()
        {
            bIsBattle = false;
            uiStep = 0;
            uiPhase_timer = 3000;
            dawnLineDelay = 5000;
            uiTotal_dawn = ENCOUNTER_TOTAL_DAWN;
            uiTotal_scourge = ENCOUNTER_TOTAL_SCOURGE;

            uiAnti_magic_zone = urand(1000, 6000);
            uiDeath_strike = urand(5000, 10000);
            uiDeath_embrace = urand(5000, 10000);
            uiIcy_touch = urand(5000, 10000);
            uiUnholy_blight = urand(5000, 10000);

            uiFight_speech = 15000;
            uiNPCEngagingCheck = 5000;

            uiTirionGUID.Clear();
            uiKorfaxGUID.Clear();
            uiMaxwellGUID.Clear();
            uiEligorGUID.Clear();
            uiRayneGUID.Clear();
            uiRimblatGUID.Clear();
            uiKoltiraGUID.Clear();
            uiOrbazGUID.Clear();
            uiThassarianGUID.Clear();
            uiLichKingGUID.Clear();
            scurgeSpawnsGUIDMap.clear();
            dawnSpawnsGUIDMap.clear();

            scurgeSpawnsGUIDMap.clear();
            dawnSpawnsGUIDMap.clear();
            scurgeSpawnsAmountLimitMap.clear();

            scurgeSpawnsAmountLimitMap[NPC_ACHERUS_GHOUL] = ENCOUNTER_GHOUL_NUMBER;
            scurgeSpawnsAmountLimitMap[NPC_RAMPAGING_ABOMINATION] = ENCOUNTER_ABOMINATION_NUMBER;
            scurgeSpawnsAmountLimitMap[NPC_WARRIOR_OF_THE_FROZEN_WASTES] = ENCOUNTER_WARRIOR_NUMBER;
            scurgeSpawnsAmountLimitMap[NPC_FLESH_BEHEMOTH] = ENCOUNTER_BEHEMOTH_NUMBER;

            dawnSpawnsAmountLimitMap[NPC_ARGENT_PROTECTOR] = ENCOUNTER_ARGENT_PROTECTOR;
            dawnSpawnsAmountLimitMap[NPC_DEFENDER_OF_THE_LIGHT] = ENCOUNTER_DEFENDER_NUMBER;

            UpdateWorldState(me->GetMap(), WORLD_STATE_REMAINS, 0);
            //UpdateWorldState(me->GetMap(), WORLD_STATE_COUNTDOWN, 0);
            UpdateWorldState(me->GetMap(), WORLD_STATE_EVENT_BEGIN, 0);

            while (true)
            {
                uint32 summonEntry = 0;
                for (std::unordered_map<uint32, uint32>::iterator saIT = scurgeSpawnsAmountLimitMap.begin(); saIT != scurgeSpawnsAmountLimitMap.end(); saIT++)
                {
                    uint32 eachEntry = saIT->first;
                    if (eachEntry == NPC_RAMPAGING_ABOMINATION || eachEntry == NPC_FLESH_BEHEMOTH)
                    {
                        uint32 eachAmount = saIT->second;
                        if (scurgeSpawnsGUIDMap.find(eachEntry) == scurgeSpawnsGUIDMap.end())
                        {
                            summonEntry = eachEntry;
                            break;
                        }
                        else if (scurgeSpawnsGUIDMap[eachEntry].size() < eachAmount)
                        {
                            summonEntry = eachEntry;
                            break;
                        }
                    }
                }
                if (summonEntry > 0)
                {
                    if (Unit* newSummoned = SummonCombatUnit(summonEntry, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), FACTION_UNDEAD_SCOURGE_3))
                    {
                        scurgeSpawnsGUIDMap[summonEntry][scurgeSpawnsGUIDMap[summonEntry].size()] = newSummoned->GetGUID();
                    }
                }
                else
                {
                    break;
                }
            }
        }

        void Reset() override
        {

        }

        void UpdateAI(uint32 diff) override
        {
            if (!bIsBattle)
            {
                if (uiPhase_timer <= diff)
                {
                    // ******* Before battle *****************************************************************
                    switch (uiStep)
                    {
                    case 0:  // countdown
                        //UpdateWorldState(me->GetMap(), WORLD_STATE_COUNTDOWN, 1);
                        break;

                    case 1:  // just delay
                        //UpdateWorldState(me->GetMap(), WORLD_STATE_REMAINS, 1);
                        UpdateWorldState(me->GetMap(), WORLD_STATE_COUNTDOWN, 0);
                        UpdateWorldState(me->GetMap(), WORLD_STATE_EVENT_BEGIN, 1);
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                        // spawn dawns
                        while (true)
                        {
                            uint32 summonEntry = 0;
                            for (std::unordered_map<uint32, uint32>::iterator saIT = dawnSpawnsAmountLimitMap.begin(); saIT != dawnSpawnsAmountLimitMap.end(); saIT++)
                            {
                                uint32 eachEntry = saIT->first;
                                uint32 eachAmount = saIT->second;
                                if (dawnSpawnsGUIDMap.find(eachEntry) == dawnSpawnsGUIDMap.end())
                                {
                                    summonEntry = eachEntry;
                                    break;
                                }
                                else if (dawnSpawnsGUIDMap[eachEntry].size() < eachAmount)
                                {
                                    summonEntry = eachEntry;
                                    break;
                                }
                            }
                            if (summonEntry > 0)
                            {
                                if(Unit* newSummoned= SummonCombatUnit(summonEntry, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                                {                                    
                                    dawnSpawnsGUIDMap[summonEntry][dawnSpawnsGUIDMap[summonEntry].size()] = newSummoned->GetGUID();
                                }
                            }
                            else
                            {
                                break;
                            }
                        }

                        if (Unit* newSummoned = SummonCombatUnit(NPC_KORFAX_CHAMPION_OF_THE_LIGHT, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                        {
                            uiKorfaxGUID = newSummoned->GetGUID();
                        }
                        if (Unit* newSummoned = SummonCombatUnit(NPC_LORD_MAXWELL_TYROSUS, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                        {
                            uiMaxwellGUID = newSummoned->GetGUID();
                        }
                        if (Unit* newSummoned = SummonCombatUnit(NPC_COMMANDER_ELIGOR_DAWNBRINGER, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                        {
                            uiEligorGUID = newSummoned->GetGUID();
                        }
                        if (Unit* newSummoned = SummonCombatUnit(NPC_RAYNE, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                        {
                            uiRayneGUID = newSummoned->GetGUID();
                        }
                        if (Unit* newSummoned = SummonCombatUnit(NPC_RIMBLAT_EARTHSHATTER, LightofDawnLoc[0].GetPositionX(), LightofDawnLoc[0].GetPositionY(), LightofDawnLoc[0].GetPositionZ(), 0.6f, FACTION_SCARLET_CRUSADE))
                        {
                            uiRimblatGUID = newSummoned->GetGUID();
                        }

                        JumpToNextStep(3000);
                        break;

                    case 2:
                        Talk(SAY_LIGHT_OF_DAWN04);
                        if (Creature* temp = GetClosestCreatureWithEntry(me, NPC_KOLTIRA_DEATHWEAVER, 50.0f))
                        {
                            uiKoltiraGUID = temp->GetGUID();
                        }
                        if (Creature* temp = GetClosestCreatureWithEntry(me, NPC_ORBAZ_BLOODBANE, 50.0f))
                        {
                            uiOrbazGUID = temp->GetGUID();
                        }
                        if (Creature* temp = GetClosestCreatureWithEntry(me, NPC_THASSARIAN, 50.0f))
                        {
                            uiThassarianGUID = temp->GetGUID();
                        }
                        JumpToNextStep(10000);
                        break;

                    case 3: // rise
                    {
                        Talk(SAY_LIGHT_OF_DAWN05);
                        me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        JumpToNextStep(2000);
                        break;
                    }
                    case 4:
                    {
                        uint32 summonEntry = 0;
                        for (std::unordered_map<uint32, uint32>::iterator saIT = scurgeSpawnsAmountLimitMap.begin(); saIT != scurgeSpawnsAmountLimitMap.end(); saIT++)
                        {
                            uint32 eachEntry = saIT->first;
                            uint32 eachAmount = saIT->second;
                            if (scurgeSpawnsGUIDMap.find(eachEntry) == scurgeSpawnsGUIDMap.end())
                            {
                                summonEntry = eachEntry;
                                break;
                            }
                            else if (scurgeSpawnsGUIDMap[eachEntry].size() < eachAmount)
                            {
                                summonEntry = eachEntry;
                                break;
                            }
                        }
                        if (summonEntry > 0)
                        {
                            uiPhase_timer = 500;
                            DoCast(me, 33271); // shack effect
                            if (Unit* newSummoned = SummonCombatUnit(summonEntry, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), FACTION_UNDEAD_SCOURGE_3))
                            {
                                newSummoned->HandleEmoteCommand(Emote::EMOTE_ONESHOT_EMERGE);
                                scurgeSpawnsGUIDMap[summonEntry][scurgeSpawnsGUIDMap[summonEntry].size()] = newSummoned->GetGUID();
                            }
                        }
                        else
                        {
                            JumpToNextStep(500);
                        }
                        break;
                    }
                    case 5:
                    {
                        JumpToNextStep(500);
                        break;
                    }
                    case 6: // summon announce
                    {
                        Talk(SAY_LIGHT_OF_DAWN06);
                        JumpToNextStep(11000);
                        break;
                    }
                    case 7: // charge begins
                    {
                        float destX = frand(LightofDawnLoc[0].GetPositionX() - 5.0f, LightofDawnLoc[0].GetPositionX() + 5.0f);
                        float destY = frand(LightofDawnLoc[0].GetPositionY() - 5.0f, LightofDawnLoc[0].GetPositionY() + 5.0f);
                        float destZ = LightofDawnLoc[0].GetPositionZ();
                        me->SetWalk(false);
                        me->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                        {
                            destX = frand(LightofDawnLoc[0].GetPositionX() - 5.0f, LightofDawnLoc[0].GetPositionX() + 5.0f);
                            destY = frand(LightofDawnLoc[0].GetPositionY() - 5.0f, LightofDawnLoc[0].GetPositionY() + 5.0f);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                        {
                            destX = frand(LightofDawnLoc[0].GetPositionX() - 5.0f, LightofDawnLoc[0].GetPositionX() + 5.0f);
                            destY = frand(LightofDawnLoc[0].GetPositionY() - 5.0f, LightofDawnLoc[0].GetPositionY() + 5.0f);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                        {
                            destX = frand(LightofDawnLoc[0].GetPositionX() - 5.0f, LightofDawnLoc[0].GetPositionX() + 5.0f);
                            destY = frand(LightofDawnLoc[0].GetPositionY() - 5.0f, LightofDawnLoc[0].GetPositionY() + 5.0f);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                        }
                        for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = scurgeSpawnsGUIDMap.begin(); sMapIT != scurgeSpawnsGUIDMap.end(); sMapIT++)
                        {
                            std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                            for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                            {
                                if (ObjectGuid eachOG = git->second)
                                {
                                    if (Creature* temp = ObjectAccessor::GetCreature(*me, eachOG))
                                    {
                                        if (temp->GetEntry() == NPC_FLESH_BEHEMOTH)
                                        {
                                            temp->SetSpeedRate(MOVE_RUN, 1.0f);
                                        }
                                        float destX = frand(LightofDawnLoc[0].GetPositionX() - 5.0f, LightofDawnLoc[0].GetPositionX() + 5.0f);
                                        float destY = frand(LightofDawnLoc[0].GetPositionY() - 5.0f, LightofDawnLoc[0].GetPositionY() + 5.0f);
                                        float destZ = LightofDawnLoc[0].GetPositionZ();
                                        temp->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                                    }
                                }
                            }
                        }
                        JumpToNextStep(5000);
                        break;
                    }
                    case 8:
                    {
                        uiPhase_timer = 1000;
                        bool toEngage = false;
                        if (Creature* korfax = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            float lodDistance = me->GetDistance(LightofDawnLoc[0]);
                            if (lodDistance < VISIBILITY_DISTANCE_TINY)
                            {
                                toEngage = true;
                                korfax->AI()->Talk(SAY_LIGHT_OF_DAWN07);
                            }
                        }
                        else
                        {
                            if (me->IsInCombat())
                            {
                                toEngage = true;
                            }
                        }
                        if (toEngage)
                        {
                            me->SetImmuneToAll(false);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->Dismount();
                            me->mounting = false;
                            me->CastSpell(me, SPELL_THE_MIGHT_OF_MOGRAINE, true); // need to fix, on player only
                            StartEngage(me->GetGUID());
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                            {
                                temp->SetImmuneToAll(false);
                                temp->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                temp->Dismount();
                                temp->mounting = false;
                                StartEngage(uiKoltiraGUID);
                            }
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                            {
                                temp->SetImmuneToAll(false);
                                temp->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                temp->Dismount();
                                temp->mounting = false;
                                StartEngage(uiThassarianGUID);
                            }
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                            {
                                temp->SetImmuneToAll(false);
                                temp->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                temp->Dismount();
                                temp->mounting = false;
                                StartEngage(uiOrbazGUID);
                            }
                            for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = scurgeSpawnsGUIDMap.begin(); sMapIT != scurgeSpawnsGUIDMap.end(); sMapIT++)
                            {
                                std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                                for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                                {
                                    if (ObjectGuid eachOG = git->second)
                                    {
                                        StartEngage(eachOG);
                                    }
                                }
                            }

                            StartEngage(uiKorfaxGUID);
                            StartEngage(uiMaxwellGUID);
                            StartEngage(uiEligorGUID);
                            StartEngage(uiRayneGUID);
                            StartEngage(uiRimblatGUID);
                            for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = dawnSpawnsGUIDMap.begin(); sMapIT != dawnSpawnsGUIDMap.end(); sMapIT++)
                            {
                                std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                                for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                                {
                                    if (ObjectGuid eachOG = git->second)
                                    {
                                        StartEngage(eachOG);
                                    }
                                }
                            }                            
                            bIsBattle = true;
                        }
                        break;
                    }
                    // ******* After battle *****************************************************************
                    case 9:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        JumpToNextStep(500);
                        break;
                    }
                    case 10:
                    {
                        JumpToNextStep(500);
                        break;
                    }
                    case 11:
                    {
                        JumpToNextStep(2000);
                        break;
                    }
                    case 12:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN26);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                        {
                            float spawnX = 0.0f, spawnY = 0.0f, spawnZ = 0.0f;
                            temp->GetRespawnPosition(spawnX, spawnY, spawnZ);
                            temp->SetHomePosition(spawnX, spawnY, spawnZ, 0);
                            temp->SetWalk(false);
                            temp->Mount(25278);
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN04);
                            temp->GetMotionMaster()->MovePoint(0, spawnX, spawnY, spawnZ);
                        }
                        me->SetHomePosition(LightofDawnLoc[24]);
                        me->SetWalk(false);
                        me->GetMotionMaster()->MovePoint(0, LightofDawnLoc[24]);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[19]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[19]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[21]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[21]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[9]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[9]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[12]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[12]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[15]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[15]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRayneGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[17]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[17]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRimblatGUID))
                        {
                            temp->SetHomePosition(LightofDawnLoc[5]);
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[5]);
                        }
                        JumpToNextStep(20000);
                        break;
                    }
                    case 13:
                    {
                        Talk(EMOTE_LIGHT_OF_DAWN05);
                        me->RemoveAurasDueToSpell(SPELL_THE_LIGHT_OF_DAWN);
                        me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                        {
                            temp->RemoveAurasDueToSpell(SPELL_THE_LIGHT_OF_DAWN);
                            temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                        {
                            temp->RemoveAurasDueToSpell(SPELL_THE_LIGHT_OF_DAWN);
                            temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRayneGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRimblatGUID))
                        {
                            temp->SetFacingToObject(me);
                        }
                        JumpToNextStep(5000);
                        break;
                    }
                    case 14:
                    {
                        Talk(SAY_LIGHT_OF_DAWN27);
                        JumpToNextStep(15000);
                        break;
                    }
                    // Tirion starts to speak
                    case 15:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN28);
                        }
                        JumpToNextStep(21000);
                        break;
                    }
                    case 16:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN29);
                        }
                        JumpToNextStep(13000);
                        break;
                    }
                    case 17:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN30);
                        JumpToNextStep(13000);
                        break;
                    }
                    case 18:
                    {
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        Talk(SAY_LIGHT_OF_DAWN31);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            temp->SetWalk(false);
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[10]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[13]);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[16]);
                        }
                        JumpToNextStep(10000);
                        break;
                    }
                    // summon gate
                    case 19:
                    {
                        if (Creature* temp = me->SummonCreature(NPC_HIGHLORD_ALEXANDROS_MOGRAINE, LightofDawnLoc[22], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS))
                        {
                            temp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            temp->CastSpell(temp, SPELL_ALEXANDROS_MOGRAINE_SPAWN, true);
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN06);
                            uiAlexandrosGUID = temp->GetGUID();
                        }
                        JumpToNextStep(5000);
                        break;
                    }
                    // Alexandros out
                    case 20:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                        {
                            temp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[23]);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN32);
                            me->SetFacingToObject(temp);
                        }
                        JumpToNextStep(6000);
                        break;
                    }
                    case 21:
                    {
                        Talk(SAY_LIGHT_OF_DAWN33);
                        JumpToNextStep(2000);
                        break;
                    }
                    case 22:
                    {
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        Talk(SAY_LIGHT_OF_DAWN34);
                        JumpToNextStep(5000);
                        break;
                    }
                    case 23: // Darion's spirit out
                    {
                        if (Creature* temp = me->SummonCreature(NPC_DARION_MOGRAINE, LightofDawnLoc[24], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN35);
                            temp->SetWalk(false);
                            uiDarionGUID = temp->GetGUID();
                        }
                        JumpToNextStep(4000);
                        break;
                    }
                    case 24: // runs to father
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                        {
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN07);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[25]);
                        }
                        JumpToNextStep(4000);
                        break;
                    }
                    case 25:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN36);
                        JumpToNextStep(4000);
                        break;
                    }
                    case 26:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN08);
                        JumpToNextStep(4000);
                        break;
                    }
                    case 27:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN37);
                        JumpToNextStep(8000);
                        break;
                    }
                    case 28:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN38);
                        JumpToNextStep(8000);
                        break;
                    }
                    case 29:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN39);

                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID)) // Tirion moves forward here
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[1]);

                        JumpToNextStep(15000);
                        break;
                    }
                    case 30:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN40);
                        JumpToNextStep(11000);
                        break;
                    }
                    case 31:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN41);
                        JumpToNextStep(28000);
                        break;
                    }
                    case 32:
                    {
                        JumpToNextStep(100);
                        break;
                    }
                    case 33:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN42);
                        JumpToNextStep(6000);
                        break;
                    }
                    case 34: // lich king spawns
                    {
                        if (Creature* temp = me->SummonCreature(NPC_THE_LICH_KING, LightofDawnLoc[26], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN43);
                            uiLichKingGUID = temp->GetGUID();
                            if (Unit* pAlex = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID))
                                temp->CastSpell(pAlex, SPELL_SOUL_FEAST_ALEX, false);
                        }
                        JumpToNextStep(2000);
                        break;
                    }
                    case 35:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiAlexandrosGUID)) // just hide him
                        {
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN09);
                            temp->DespawnOrUnsummon(500);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiDarionGUID))
                        {
                            temp->DespawnOrUnsummon(500);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            temp->InterruptNonMeleeSpells(false);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN45);
                        }
                        JumpToNextStep(3000);
                        break;
                    }
                    case 36:
                    {
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        Talk(EMOTE_LIGHT_OF_DAWN10);
                        Talk(SAY_LIGHT_OF_DAWN44);
                        JumpToNextStep(3000);
                        break;
                    }
                    case 37:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[27]);
                        JumpToNextStep(6000);
                        break;
                    }
                    case 38: // Darion supports to jump to lich king here
                    {
                        if (ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            me->SetSpeedRate(MOVE_RUN, 3.0f);
                            me->SetWalk(false);
                            DoCast(me, SPELL_MOGRAINE_CHARGE); // jumping charge
                        }
                        JumpToNextStep(500);
                        break;
                    }
                    case 39:
                    {
                        me->HandleEmoteCommand(EMOTE_ONESHOT_SPECIALATTACK1H);
                        JumpToNextStep(500);
                        break;
                    }
                    case 40: // Lich king counterattacks
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            temp->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN46);
                        }
                        me->SetSpeedRate(MOVE_RUN, 6.0f);
                        me->GetMotionMaster()->MovePoint(0, LightofDawnLoc[30]);
                        me->SetStandState(UNIT_STAND_STATE_DEAD);
                        JumpToNextStep(100);
                        break;
                    }
                    case 41:
                    {
                        JumpToNextStep(500);
                        break;
                    }
                    case 42:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            me->SetFacingToObject(temp);
                        }
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        JumpToNextStep(3000);
                        break;
                    }
                    case 43:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN47);
                        JumpToNextStep(5000);
                        break;
                    }
                    case 44:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN48);
                        JumpToNextStep(12000);
                        break;
                    }
                    case 45:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN49);
                        JumpToNextStep(16000);
                        break;
                    }
                    case 46: // Lich king - Apocalypse
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN11);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN51);
                            if (Creature* pTirion = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            {
                                pTirion->SetStandState(UNIT_STAND_STATE_KNEEL);
                                //temp->CastSpell(pTirion, SPELL_APOCALYPSE, false); // not working
                                temp->CastSpell(pTirion, SPELL_SOUL_FEAST_TIRION, false);
                                pTirion->AI()->Talk(EMOTE_LIGHT_OF_DAWN12);
                            }
                        }
                        JumpToNextStep(2000);
                        break;
                    }
                    case 47: // Maxwell yells for attack
                    {
                        float fLichPositionX = 0,
                            fLichPositionY = 0,
                            fLichPositionZ = 0;
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            fLichPositionX = temp->GetPositionX();
                            fLichPositionY = temp->GetPositionY();
                            fLichPositionZ = temp->GetPositionZ();
                        }

                        if (fLichPositionX && fLichPositionY)
                        {
                            Unit* temp = me->SummonCreature(NPC_DEFENDER_OF_THE_LIGHT, LightofDawnLoc[0].GetPositionWithOffset({ float(rand32() % 10), float(rand32() % 10), 0.0f, 0.0f }), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::IN_MILLISECONDS);
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ATTACK_UNARMED);
                            temp->SetWalk(false);
                            temp->SetSpeedRate(MOVE_RUN, 2.0f);
                            temp->SetFaction(me->GetFaction());
                            temp->GetMotionMaster()->MovePoint(0, fLichPositionX, fLichPositionY, fLichPositionZ);
                            dawnSpawnsGUIDMap[NPC_DEFENDER_OF_THE_LIGHT][dawnSpawnsGUIDMap[NPC_DEFENDER_OF_THE_LIGHT].size()] = temp->GetGUID();

                            temp = me->SummonCreature(NPC_RIMBLAT_EARTHSHATTER, LightofDawnLoc[0].GetPositionWithOffset({ float(rand32() % 10), float(rand32() % 10), 0.0f, 0.0f }), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::IN_MILLISECONDS);
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ATTACK_UNARMED);
                            temp->SetWalk(false);
                            temp->SetSpeedRate(MOVE_RUN, 2.0f);
                            temp->SetFaction(me->GetFaction());
                            temp->GetMotionMaster()->MovePoint(0, fLichPositionX, fLichPositionY, fLichPositionZ);
                            dawnSpawnsGUIDMap[NPC_RIMBLAT_EARTHSHATTER][dawnSpawnsGUIDMap[NPC_RIMBLAT_EARTHSHATTER].size()] = temp->GetGUID();
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ATTACK_UNARMED);
                            temp->SetWalk(false);
                            temp->SetSpeedRate(MOVE_RUN, 2.0f);
                            temp->GetMotionMaster()->MovePoint(0, fLichPositionX, fLichPositionY, fLichPositionZ);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN50);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ATTACK_UNARMED);
                            temp->SetWalk(false);
                            temp->SetSpeedRate(MOVE_RUN, 2.0f);
                            temp->HandleEmoteCommand(EMOTE_STATE_ATTACK_UNARMED);
                            temp->GetMotionMaster()->MovePoint(0, fLichPositionX, fLichPositionY, fLichPositionZ);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_ATTACK_UNARMED);
                            temp->SetWalk(false);
                            temp->SetSpeedRate(MOVE_RUN, 2.0f);
                            temp->GetMotionMaster()->MovePoint(0, fLichPositionX, fLichPositionY, fLichPositionZ);
                        }
                        JumpToNextStep(3000);
                        break;
                    }
                    case 48: // They all got kicked
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN13);

                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            temp->SetSpeedRate(MOVE_RUN, 6.0f);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[14]);
                            temp->SetStandState(UNIT_STAND_STATE_DEAD);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            temp->SetSpeedRate(MOVE_RUN, 6.0f);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[11]);
                            temp->SetStandState(UNIT_STAND_STATE_DEAD);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            temp->SetSpeedRate(MOVE_RUN, 6.0f);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[17]);
                            temp->SetStandState(UNIT_STAND_STATE_DEAD);
                        }

                        for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = dawnSpawnsGUIDMap.begin(); sMapIT != dawnSpawnsGUIDMap.end(); sMapIT++)
                        {
                            std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                            for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                            {
                                if (ObjectGuid eachOG = git->second)
                                {
                                    if (Creature* temp = ObjectAccessor::GetCreature(*me, eachOG))
                                    {
                                        temp->SetSpeedRate(MOVE_RUN, 6.0f);
                                        temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[0].GetPositionWithOffset({ float(rand32() % 10), float(rand32() % 10), 0.0f, 0.0f }));
                                        temp->SetStandState(UNIT_STAND_STATE_DEAD);
                                    }
                                }
                            }
                        }
                        JumpToNextStep(3000);
                        break;
                    }
                    case 49: // make them stand up
                    {
                        if (Creature* lk = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                            {
                                temp->SetStandState(UNIT_STAND_STATE_STAND);
                                temp->SetFacingToObject(lk);
                            }
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                            {
                                temp->SetStandState(UNIT_STAND_STATE_STAND);
                                temp->SetFacingToObject(lk);
                            }
                            if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                            {
                                temp->SetStandState(UNIT_STAND_STATE_STAND);
                                temp->SetFacingToObject(lk);
                            }
                        }
                        JumpToNextStep(1000);
                        break;
                    }
                    case 50:
                        Talk(SAY_LIGHT_OF_DAWN52);
                        JumpToNextStep(5000);
                        break;

                    case 51: // Darion stand up, "not today"
                    {
                        me->SetSpeedRate(MOVE_RUN, 1.0f);
                        me->SetWalk(true);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            me->SetFacingToObject(temp);
                        }
                        Talk(SAY_LIGHT_OF_DAWN53);
                        JumpToNextStep(1000);
                        break;
                    }
                    case 52: // Ashbringer rebirth
                    {
                        me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, uint32(EQUIP_UNEQUIP));
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            me->CastSpell(temp, SPELL_ASHBRINGER, true);
                        Talk(EMOTE_LIGHT_OF_DAWN14);
                        JumpToNextStep(3000);
                        break;
                    }
                    case 53: // Ashbringer rebirth
                    {
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        Talk(EMOTE_LIGHT_OF_DAWN15);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->SetStandState(UNIT_STAND_STATE_STAND);
                            temp->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, uint32(EQUIP_HIGHLORD_TIRION_FORDRING));
                            temp->CastSpell(temp, SPELL_REBIRTH_OF_THE_ASHBRINGER, false);
                        }
                        JumpToNextStep(1000);
                        break;
                    }
                    case 54:
                    {
                        me->SummonGameObject(GO_LIGHT_OF_DAWN, 2283.896f, -5287.914f, 83.066f, 0.f, QuaternionData(), 30 * TimeConstants::IN_MILLISECONDS);
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            if (temp->HasAura(SPELL_REBIRTH_OF_THE_ASHBRINGER))
                                temp->RemoveAurasDueToSpell(SPELL_REBIRTH_OF_THE_ASHBRINGER);
                            temp->CastSpell(temp, 41542, false); // workarounds, light expoded, makes it cool
                            temp->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->InterruptNonMeleeSpells(false);
                        JumpToNextStep(2500);
                        break;
                    }
                    case 55:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN54);
                        JumpToNextStep(4000);
                        break;
                    }
                    case 56:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN55);
                        JumpToNextStep(4000);
                        break;
                    }
                    case 57:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN56);
                        JumpToNextStep(1000);
                        break;
                    }
                    case 58: // Tiron charges
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN16);
                            temp->CastSpell(temp, SPELL_TIRION_CHARGE, false); // jumping charge
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                            temp->SetSpeedRate(MOVE_RUN, 3.0f); // workarounds, make Tirion still running
                            temp->SetWalk(false);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[2]);
                        }
                        JumpToNextStep(1000);
                        break;
                    }
                    case 59:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            me->SetFacingToObject(temp);
                            temp->Relocate(LightofDawnLoc[28]); // workarounds, he should kick back by Tirion, but here we relocate him
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN57);
                        }
                        JumpToNextStep(500);
                        break;
                    }
                    case 60:
                    {
                        JumpToNextStep(500);
                        break;
                    }
                    case 61:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->SetStandState(UNIT_STAND_STATE_KNEEL);
                        JumpToNextStep(1000);
                        break;
                    }
                    case 62:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->SetStandState(UNIT_STAND_STATE_STAND);
                        JumpToNextStep(1000);
                        break;
                    }
                    case 63:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN58);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 64:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN59);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 65:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                        {
                            temp->CastSpell(temp, SPELL_TELEPORT_VISUAL, false);
                        }
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID)) // Tirion runs to Darion
                        {
                            temp->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            temp->SetSpeedRate(MOVE_RUN, 1.0f);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[6]);
                        }
                        JumpToNextStep(2500);
                        break;
                    }
                    case 66:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID)) // Lich king disappears here
                        {
                            temp->AI()->Talk(EMOTE_LIGHT_OF_DAWN17);
                            temp->DespawnOrUnsummon(500);
                        }
                        JumpToNextStep(5000);
                        break;
                    }
                    case 67:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN60);
                        JumpToNextStep(3000);
                        break;
                    }
                    case 68:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->SetWalk(true);
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[7]);
                        }
                        JumpToNextStep(5500);
                        break;
                    }
                    case 69:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            temp->GetMotionMaster()->MovePoint(0, LightofDawnLoc[8]);
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN61);
                        }
                        JumpToNextStep(15000);
                        break;
                    }
                    case 70:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN62);
                        JumpToNextStep(7000);
                        break;
                    }
                    case 71:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN63);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 72:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN64);
                        JumpToNextStep(11000);
                        break;
                    }
                    case 73:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN65);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 74:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN66);
                        JumpToNextStep(8000);
                        break;
                    }
                    case 75:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN67);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 76:
                    {
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        Talk(SAY_LIGHT_OF_DAWN68);
                        JumpToNextStep(10000);
                        break;
                    }
                    case 77:
                    {
                        // search players with in 50 yards for quest credit
                        Map::PlayerList const& PlayerList = me->GetMap()->GetPlayers();
                        if (!PlayerList.isEmpty())
                        {
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                if (me->IsWithinDistInMap(i->GetSource(), 500))
                                    i->GetSource()->CastSpell(i->GetSource(), SPELL_THE_LIGHT_OF_DAWN_Q, false);
                        }
                        me->SetVisible(false); // respawns another Darion for quest turn in
                        me->SummonCreature(NPC_HIGHLORD_DARION_MOGRAINE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);
                        JumpToNextStep(1000);
                        break;
                    }
                    case 78:
                    {
                        JumpToNextStep(25000);
                        break;
                    }
                    case 79:
                    {
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                            temp->DespawnOrUnsummon();
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                            temp->DespawnOrUnsummon();
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                            temp->DespawnOrUnsummon();
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiLichKingGUID))
                            temp->DespawnOrUnsummon();
                        me->DespawnOrUnsummon();
                        break;
                    }
                    }
                }
                else
                {
                    uiPhase_timer -= diff;
                }
            }

            // ******* During battle *****************************************************************
            else
            {
                if (uiAnti_magic_zone <= diff)
                {
                    DoCast(me, SPELL_ANTI_MAGIC_ZONE1);
                    uiAnti_magic_zone = urand(25000, 30000);
                }
                else
                {
                    uiAnti_magic_zone -= diff;
                }
                if (uiDeath_strike <= diff)
                {
                    DoCastVictim(SPELL_DEATH_STRIKE);
                    uiDeath_strike = urand(5000, 10000);
                }
                else
                {
                    uiDeath_strike -= diff;
                }
                if (uiDeath_embrace <= diff)
                {
                    DoCastVictim(SPELL_DEATH_EMBRACE);
                    uiDeath_embrace = urand(5000, 10000);
                }
                else
                {
                    uiDeath_embrace -= diff;
                }
                if (uiIcy_touch <= diff)
                {
                    DoCastVictim(SPELL_ICY_TOUCH1);
                    uiIcy_touch = urand(5000, 10000);
                }
                else
                {
                    uiIcy_touch -= diff;
                }
                if (uiUnholy_blight <= diff)
                {
                    DoCastVictim(SPELL_UNHOLY_BLIGHT);
                    uiUnholy_blight = urand(5000, 10000);
                }
                else
                {
                    uiUnholy_blight -= diff;
                }
                if (uiFight_speech <= diff)
                {
                    Talk(SAY_LIGHT_OF_DAWN09);
                    uiFight_speech = urand(5000, 10000);
                }
                else
                {
                    uiFight_speech -= diff;
                }
                if (uiNPCEngagingCheck <= diff)
                {
                    for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = scurgeSpawnsGUIDMap.begin(); sMapIT != scurgeSpawnsGUIDMap.end(); sMapIT++)
                    {
                        std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                        for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                        {
                            if (ObjectGuid eachOG = git->second)
                            {
                                StartEngage(eachOG);
                            }
                        }
                    }
                    for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = dawnSpawnsGUIDMap.begin(); sMapIT != dawnSpawnsGUIDMap.end(); sMapIT++)
                    {
                        std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                        for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                        {
                            if (ObjectGuid eachOG = git->second)
                            {
                                StartEngage(eachOG);
                            }
                        }
                    }
                    uint32 dawnEngagingUnitsCount = 0;
                    uint32 scurgeEngagingUnitsCount = 0;
                    if (NPCShieldCheck(me->GetGUID()))
                    {
                        StartEngage(me->GetGUID());
                        scurgeEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiKoltiraGUID))
                    {
                        StartEngage(uiKoltiraGUID);
                        scurgeEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiOrbazGUID))
                    {
                        StartEngage(uiOrbazGUID);
                        scurgeEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiThassarianGUID))
                    {
                        StartEngage(uiThassarianGUID);
                        scurgeEngagingUnitsCount++;
                    }

                    if (NPCShieldCheck(uiKorfaxGUID))
                    {
                        StartEngage(uiKorfaxGUID);
                        dawnEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiMaxwellGUID))
                    {
                        StartEngage(uiMaxwellGUID);
                        dawnEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiEligorGUID))
                    {
                        StartEngage(uiEligorGUID);
                        dawnEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiRayneGUID))
                    {
                        StartEngage(uiRayneGUID);
                        dawnEngagingUnitsCount++;
                    }
                    if (NPCShieldCheck(uiRimblatGUID))
                    {
                        StartEngage(uiRimblatGUID);
                        dawnEngagingUnitsCount++;
                    }
                    if (scurgeEngagingUnitsCount < 2 || dawnEngagingUnitsCount < 2)
                    {
                        if (Creature* tirion = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            if (scurgeEngagingUnitsCount == 0)
                            {
                                me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                if (me->HasAura(SPELL_THE_MIGHT_OF_MOGRAINE))
                                {
                                    me->RemoveAurasDueToSpell(SPELL_THE_MIGHT_OF_MOGRAINE);
                                }
                                me->RemoveAurasDueToSpell(31635);
                                me->CastSpell(me, SPELL_THE_LIGHT_OF_DAWN, false);
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                    temp->CastSpell(temp, SPELL_THE_LIGHT_OF_DAWN, false);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                    temp->CastSpell(temp, SPELL_THE_LIGHT_OF_DAWN, false);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRayneGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                if (Creature* temp = ObjectAccessor::GetCreature(*me, uiRimblatGUID))
                                {
                                    temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                                    temp->RemoveAurasDueToSpell(31635);
                                }
                                tirion->GetMotionMaster()->MovePoint(0, LightofDawnLoc[0]);
                                bIsBattle = false;
                                uiStep = 9;
                                uiPhase_timer = 4000;
                            }
                            else
                            {
                                StartEngage(uiTirionGUID);
                                if (tirion->GetReactState() != ReactStates::REACT_AGGRESSIVE)
                                {
                                    tirion->SetWalk(false);
                                    tirion->SetImmuneToAll(false);
                                    tirion->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                                    tirion->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                }
                            }
                        }
                        else
                        {
                            if (Creature* temp = me->SummonCreature(NPC_HIGHLORD_TIRION_FORDRING, LightofDawnLoc[0].GetPositionWithOffset({ 0.0f, 0.0f, 0.0f, 1.528f }), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS))
                            {
                                temp->SetFaction(FACTION_SCARLET_CRUSADE);
                                temp->SetReactState(ReactStates::REACT_PASSIVE);
                                temp->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, uint32(EQUIP_UNEQUIP));
                                temp->SetExtraUnitMovementFlags
                                temp->AI()->Talk(SAY_LIGHT_OF_DAWN25);
                                uiTirionGUID = temp->GetGUID();
                            }
                        }
                    }
                    uiNPCEngagingCheck = 5000;
                }
                else
                {
                    uiNPCEngagingCheck -= diff;
                }

                if (dawnLineDelay > diff)
                {
                    dawnLineDelay -= diff;
                    if (dawnLineDelay < diff)
                    {
                        dawnLineDelay = 0;
                        if (Creature* temp = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            temp->AI()->Talk(SAY_LIGHT_OF_DAWN08);
                        }
                    }
                }

                if (UpdateVictim())
                {
                    DoMeleeAttackIfReady();
                }
            }
        }

        void JumpToNextStep(uint32 uiTimer)
        {
            uiPhase_timer = uiTimer;
            ++uiStep;
        }

        bool NPCShieldCheck(ObjectGuid pmOGCreature)
        {
            bool engagingResult = true;

            if (Creature* temp = ObjectAccessor::GetCreature(*me, pmOGCreature))
            {
                if (temp->IsAlive())
                {
                    temp->SetHomePosition(temp->GetPosition());
                    if (temp->GetReactState() != ReactStates::REACT_PASSIVE)
                    {
                        if (temp->GetHealthPct() < 30.0f)
                        {
                            temp->ClearInCombat();
                            temp->SetImmuneToAll(true);
                            temp->CastSpell(temp, 31635);
                            temp->SetReactState(ReactStates::REACT_PASSIVE);
                            temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                            engagingResult = false;
                        }
                    }
                    else
                    {
                        engagingResult = false;
                        if (temp->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_KNEEL)
                        {
                            temp->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        }
                    }
                }
            }

            return engagingResult;
        }

        void StartEngage(ObjectGuid pmOGCreature)
        {
            if (Creature* temp = ObjectAccessor::GetCreature(*me, pmOGCreature))
            {
                if (temp->IsAlive())
                {
                    temp->SetHomePosition(temp->GetPosition());
                    if (temp->IsHostileTo(me))
                    {
                        for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = scurgeSpawnsGUIDMap.begin(); sMapIT != scurgeSpawnsGUIDMap.end(); sMapIT++)
                        {
                            std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                            for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                            {
                                if (ObjectGuid eachOG = git->second)
                                {
                                    if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, eachOG))
                                    {
                                        temp->EngageWithTarget(eachEnemy);
                                    }
                                }
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiKoltiraGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiOrbazGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiThassarianGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (temp->IsValidAttackTarget(me))
                        {
                            temp->EngageWithTarget(me);
                        }
                    }
                    else
                    {
                        for (std::unordered_map<uint32, std::unordered_map<uint32, ObjectGuid>>::iterator sMapIT = dawnSpawnsGUIDMap.begin(); sMapIT != dawnSpawnsGUIDMap.end(); sMapIT++)
                        {
                            std::unordered_map<uint32, ObjectGuid> eachGUIDMap = sMapIT->second;
                            for (std::unordered_map<uint32, ObjectGuid>::iterator git = eachGUIDMap.begin(); git != eachGUIDMap.end(); git++)
                            {
                                if (ObjectGuid eachOG = git->second)
                                {
                                    if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, eachOG))
                                    {
                                        temp->EngageWithTarget(eachEnemy);
                                    }
                                }
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiKorfaxGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }                            
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiMaxwellGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiEligorGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiRayneGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiRimblatGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                        if (Creature* eachEnemy = ObjectAccessor::GetCreature(*me, uiTirionGUID))
                        {
                            if (temp->IsValidAttackTarget(eachEnemy))
                            {
                                temp->EngageWithTarget(eachEnemy);
                            }
                        }
                    }
                }
            }
        }

        Unit* SummonCombatUnit(uint32 pmEntry, float pmBaseDestX, float pmBaseDestY, float pmBaseDestZ, float pmOrientation, uint32 pmFaction)
        {
            float destX = frand(pmBaseDestX - 20.0f, pmBaseDestX + 20.0f);
            float destY = frand(pmBaseDestY - 20.0f, pmBaseDestY + 20.0f);
            float destZ = pmBaseDestZ;
            me->UpdateGroundPositionZ(destX, destY, destZ);

            if (TempSummon* summoned = me->SummonCreature(pmEntry, destX, destY, destZ, pmOrientation, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 2 * TimeConstants::IN_MILLISECONDS))
            {
                summoned->SetWalk(false);
                summoned->SetFaction(pmFaction);
                summoned->SetImmuneToAll(false);
                summoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                summoned->SetReactState(ReactStates::REACT_AGGRESSIVE);
                return summoned;
            }

            return nullptr;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                CloseGossipMenuFor(player);
                uiStep = 1;
                break;
            }
            return true;
        }

        bool GossipHello(Player* player) override
        {
            if (me->IsQuestGiver())
                player->PrepareQuestMenu(me->GetGUID());

            if (player->GetQuestStatus(12801) == QUEST_STATUS_INCOMPLETE)
                AddGossipItemFor(player, 0, "I am ready.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            SendGossipMenuFor(player, player->GetGossipTextId(me), me->GetGUID());

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_highlord_darion_mograineAI(creature);
    }
};

/*######
## npc the lich king in dawn of light
######*/
class npc_the_lich_king_tirion_dawn : public CreatureScript
{
public:
    npc_the_lich_king_tirion_dawn() : CreatureScript("npc_the_lich_king_tirion_dawn") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_the_lich_king_tirion_dawnAI(creature);
    }

    struct npc_the_lich_king_tirion_dawnAI : public ScriptedAI
    {
        npc_the_lich_king_tirion_dawnAI(Creature* creature) : ScriptedAI(creature) { }
        void Reset() override { }
        void AttackStart(Unit* /*who*/) override { } // very simple, just don't make them aggreesive
        void UpdateAI(uint32 /*diff*/) override { }
        void JustDied(Unit* /*killer*/) override { }
    };

};

// 58418 - Portal to Orgrimmar
// 58420 - Portal to Stormwind
class spell_teleport_leaders_blessing : public SpellScript
{
    void HandleScriptEffect(SpellEffIndex /* effIndex */)
    {
        Player* target = GetHitPlayer();
        if (!target)
            return;

        uint32 spellID = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
        uint32 questID = GetSpellInfo()->Effects[EFFECT_1].CalcValue();

        if (target->GetQuestStatus(questID) == QUEST_STATUS_COMPLETE)
        {
            target->CastSpell(target, spellID, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget.Register(&spell_teleport_leaders_blessing::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

class npc_volatile_ghoul : public CreatureScript
{
public:
    npc_volatile_ghoul() : CreatureScript("npc_volatile_ghoul") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_volatile_ghoulAI(creature);
    }

    struct npc_volatile_ghoulAI : public ScriptedAI
    {
        npc_volatile_ghoulAI(Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        void Reset() override
        {
            rendDelay = 5000;
            DoCastSelf(12787);
        }

        void UpdateAI(uint32 diff) override
        {
            if (UpdateVictim())
            {
                if (rendDelay >= 0)
                {
                    rendDelay -= diff;
                    if (rendDelay < 0)
                    {
                        rendDelay = 5000;
                        if (Unit* victim = me->GetVictim())
                        {
                            if (!victim->HasAura(13738, me->GetGUID()))
                            {
                                DoCastVictim(13738);
                            }
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }
        }

        int rendDelay;
    };
};

class npc_argent_protector : public CreatureScript
{
public:
    npc_argent_protector() : CreatureScript("npc_argent_protector") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_argent_protectorAI(creature);
    }

    struct npc_argent_protectorAI : public ScriptedAI
    {
        npc_argent_protectorAI(Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        void Reset() override
        {
            crusaderDelay = 5000;
        }

        void UpdateAI(uint32 diff) override
        {
            if (UpdateVictim())
            {
                if (crusaderDelay >= 0)
                {
                    crusaderDelay -= diff;
                }
                if (crusaderDelay < 0)
                {
                    if (Unit* victim = me->GetVictim())
                    {
                        float victimDistance = me->GetDistance(victim);
                        if (victimDistance < INTERACTION_DISTANCE)
                        {
                            crusaderDelay = urand(6000, 10000);
                            me->CastSpell(victim, 19643);
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }
        }

        int crusaderDelay;
    };
};

void AddSC_the_scarlet_enclave_chapter_5()
{
    new npc_highlord_darion_mograine();
    new npc_the_lich_king_tirion_dawn();
    new npc_volatile_ghoul();
    new npc_argent_protector();
    RegisterSpellScript(spell_teleport_leaders_blessing);
}
