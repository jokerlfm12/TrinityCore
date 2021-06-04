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

 /* ScriptData
 SDName: Terokkar_Forest
 SD%Complete: 85
 SDComment: Quest support: 9889, 10009, 10873, 10896, 11096, 10051, 10052. Skettis->Ogri'la Flight
 SDCategory: Terokkar Forest
 EndScriptData */

 /* ContentData
 npc_unkor_the_ruthless
 npc_infested_root_walker
 npc_rotting_forest_rager
 npc_floon
 npc_isla_starmane
 npc_slim
 EndContentData */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "WorldSession.h"

#include "SmartScriptMgr.h"

#include "GridNotifiers.h"

 /*######
 ## npc_unkor_the_ruthless
 ######*/

enum UnkorTheRuthless
{
    SAY_SUBMIT = 0,
    REQUIRED_KILL_COUNT = 10,
    SPELL_PULVERIZE = 2676,
    QUEST_DONTKILLTHEFATONE = 9889,
    NPC_BOULDERFIST_INVADER = 18260
};

class npc_unkor_the_ruthless : public CreatureScript
{
public:
    npc_unkor_the_ruthless() : CreatureScript("npc_unkor_the_ruthless") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_unkor_the_ruthlessAI(creature);
    }

    struct npc_unkor_the_ruthlessAI : public ScriptedAI
    {
        npc_unkor_the_ruthlessAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            CanDoQuest = false;
            UnkorUnfriendly_Timer = 0;
            Pulverize_Timer = 3000;
        }

        bool CanDoQuest;
        uint32 UnkorUnfriendly_Timer;
        uint32 Pulverize_Timer;

        void Reset() override
        {
            Initialize();
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetFaction(FACTION_OGRE);
        }

        void JustEngagedWith(Unit* /*who*/) override { }

        void DoNice()
        {
            Talk(SAY_SUBMIT);
            me->SetFaction(FACTION_FRIENDLY);
            me->SetStandState(UNIT_STAND_STATE_SIT);
            me->RemoveAllAuras();
            me->CombatStop(true);
            EngagementOver();
            UnkorUnfriendly_Timer = 60000;
        }

        void DamageTaken(Unit* done_by, uint32& damage) override
        {
            Player* player = done_by->ToPlayer();

            if (player && me->HealthBelowPctDamaged(30, damage))
            {
                if (Group* group = player->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
                    {
                        Player* groupie = itr->GetSource();
                        if (groupie && groupie->IsInMap(player) &&
                            groupie->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                            groupie->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, NPC_BOULDERFIST_INVADER) == REQUIRED_KILL_COUNT)
                        {
                            groupie->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                            if (!CanDoQuest)
                                CanDoQuest = true;
                        }
                    }
                }
                else if (player->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                    player->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, NPC_BOULDERFIST_INVADER) == REQUIRED_KILL_COUNT)
                {
                    player->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                    CanDoQuest = true;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (CanDoQuest)
            {
                if (!UnkorUnfriendly_Timer)
                {
                    //DoCast(me, SPELL_QUID9889);        //not using spell for now
                    DoNice();
                }
                else
                {
                    if (UnkorUnfriendly_Timer <= diff)
                    {
                        EnterEvadeMode();
                        return;
                    }
                    else UnkorUnfriendly_Timer -= diff;
                }
            }

            if (!UpdateVictim())
                return;

            if (Pulverize_Timer <= diff)
            {
                DoCast(me, SPELL_PULVERIZE);
                Pulverize_Timer = 9000;
            }
            else Pulverize_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_infested_root_walker
######*/

enum InfestedRootWalker
{
    SPELL_SUMMON_WOOD_MITES = 39130
};

class npc_infested_root_walker : public CreatureScript
{
public:
    npc_infested_root_walker() : CreatureScript("npc_infested_root_walker") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_infested_root_walkerAI(creature);
    }

    struct npc_infested_root_walkerAI : public ScriptedAI
    {
        npc_infested_root_walkerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }
        void JustEngagedWith(Unit* /*who*/) override { }

        void DamageTaken(Unit* done_by, uint32& damage) override
        {
            if (done_by && done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand32() % 100 < 75)
                        //Summon Wood Mites
                        DoCast(me, SPELL_SUMMON_WOOD_MITES, true);
        }
    };
};

/*######
## npc_rotting_forest_rager
######*/

enum RottingForestRager
{
    SPELL_SUMMON_LOTS_OF_WOOD_MITES = 39134
};

class npc_rotting_forest_rager : public CreatureScript
{
public:
    npc_rotting_forest_rager() : CreatureScript("npc_rotting_forest_rager") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rotting_forest_ragerAI(creature);
    }

    struct npc_rotting_forest_ragerAI : public ScriptedAI
    {
        npc_rotting_forest_ragerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }
        void JustEngagedWith(Unit* /*who*/) override { }

        void DamageTaken(Unit* done_by, uint32& damage) override
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand32() % 100 < 75)
                        //Summon Lots of Wood Mites
                        DoCast(me, SPELL_SUMMON_LOTS_OF_WOOD_MITES, true);
        }
    };
};

/*######
## npc_floon
######*/

enum Floon
{
    SAY_FLOON_ATTACK = 0,
    OPTION_ID_PAY_UP_OR_DIE = 0,
    OPTION_ID_COLLECT_A_DEBT = 0,
    MENU_ID_PAY_UP_OR_DIE = 7731,
    MENU_ID_COLLECT_A_DEBT = 7732,
    GOSSIP_FLOON_STRANGE_SOUNDS = 9442,
    GOSSIP_HE_ALREADY_KILLED_ME = 9443,

    SPELL_SILENCE = 6726,
    SPELL_FROSTBOLT = 9672,
    SPELL_FROST_NOVA = 11831,

    QUEST_CRACKIN_SOME_SKULLS = 10009
};

class npc_floon : public CreatureScript
{
public:
    npc_floon() : CreatureScript("npc_floon") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_floonAI(creature);
    }

    struct npc_floonAI : public ScriptedAI
    {
        npc_floonAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
            m_uiNormFaction = creature->GetFaction();
        }

        void Initialize()
        {
            Silence_Timer = 2000;
            Frostbolt_Timer = 4000;
            FrostNova_Timer = 9000;
        }

        uint32 m_uiNormFaction;
        uint32 Silence_Timer;
        uint32 Frostbolt_Timer;
        uint32 FrostNova_Timer;

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            uint32 const action = player->PlayerTalkClass->GetGossipOptionAction(gossipListId);
            ClearGossipMenuFor(player);
            if (action == GOSSIP_ACTION_INFO_DEF)
            {
                AddGossipItemFor(player, MENU_ID_PAY_UP_OR_DIE, OPTION_ID_PAY_UP_OR_DIE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                SendGossipMenuFor(player, GOSSIP_HE_ALREADY_KILLED_ME, me->GetGUID());
            }
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                CloseGossipMenuFor(player);
                me->SetFaction(FACTION_ARAKKOA);
                Talk(SAY_FLOON_ATTACK, player);
                AttackStart(player);
            }
            return true;
        }

        bool GossipHello(Player* player) override
        {
            if (player->GetQuestStatus(QUEST_CRACKIN_SOME_SKULLS) == QUEST_STATUS_INCOMPLETE)
                AddGossipItemFor(player, MENU_ID_COLLECT_A_DEBT, OPTION_ID_COLLECT_A_DEBT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

            SendGossipMenuFor(player, GOSSIP_FLOON_STRANGE_SOUNDS, me->GetGUID());
            return true;
        }

        void Reset() override
        {
            Initialize();
            if (me->GetFaction() != m_uiNormFaction)
                me->SetFaction(m_uiNormFaction);
        }

        void JustEngagedWith(Unit* /*who*/) override { }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (Silence_Timer <= diff)
            {
                DoCastVictim(SPELL_SILENCE);
                Silence_Timer = 30000;
            }
            else Silence_Timer -= diff;

            if (FrostNova_Timer <= diff)
            {
                DoCast(me, SPELL_FROST_NOVA);
                FrostNova_Timer = 20000;
            }
            else FrostNova_Timer -= diff;

            if (Frostbolt_Timer <= diff)
            {
                DoCastVictim(SPELL_FROSTBOLT);
                Frostbolt_Timer = 5000;
            }
            else Frostbolt_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_isla_starmane
######*/
enum IslaStarmaneData
{
    SAY_PROGRESS_1 = 0,
    SAY_PROGRESS_2 = 1,
    SAY_PROGRESS_3 = 2,
    SAY_PROGRESS_4 = 3,
    GO_DISTANCE = 10,
    ESCAPE_FROM_FIREWING_POINT_A = 10051,
    ESCAPE_FROM_FIREWING_POINT_H = 10052,
    SPELL_TRAVEL_FORM_CAT = 32447,
    GO_CAGE = 182794
};

class npc_isla_starmane : public CreatureScript
{
public:
    npc_isla_starmane() : CreatureScript("npc_isla_starmane") { }

    struct npc_isla_starmaneAI : public EscortAI
    {
        npc_isla_starmaneAI(Creature* creature) : EscortAI(creature) { }

        void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
            case 0:
                if (GameObject* Cage = me->FindNearestGameObject(GO_CAGE, GO_DISTANCE))
                    Cage->SetGoState(GO_STATE_ACTIVE);
                break;
            case 2:
                Talk(SAY_PROGRESS_1, player);
                break;
            case 5:
                Talk(SAY_PROGRESS_2, player);
                break;
            case 6:
                Talk(SAY_PROGRESS_3, player);
                break;
            case 29:
                Talk(SAY_PROGRESS_4, player);
                if (player->GetTeam() == ALLIANCE)
                    player->GroupEventHappens(ESCAPE_FROM_FIREWING_POINT_A, me);
                else if (player->GetTeam() == HORDE)
                    player->GroupEventHappens(ESCAPE_FROM_FIREWING_POINT_H, me);
                me->SetOrientationTowards(player);
                break;
            case 30:
                me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                break;
            case 31:
                DoCast(me, SPELL_TRAVEL_FORM_CAT);
                me->SetWalk(false);
                break;
            }
        }

        void Reset() override
        {
            me->RestoreFaction();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Player* player = GetPlayerForEscort())
            {
                if (player->GetTeam() == ALLIANCE)
                    player->FailQuest(ESCAPE_FROM_FIREWING_POINT_A);
                else if (player->GetTeam() == HORDE)
                    player->FailQuest(ESCAPE_FROM_FIREWING_POINT_H);
            }
        }

        void QuestAccept(Player* player, Quest const* quest) override
        {
            if (quest->GetQuestId() == ESCAPE_FROM_FIREWING_POINT_H || quest->GetQuestId() == ESCAPE_FROM_FIREWING_POINT_A)
            {
                Start(true, false, player->GetGUID());
                me->SetFaction(FACTION_ESCORTEE_N_NEUTRAL_PASSIVE);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_isla_starmaneAI(creature);
    }
};

/*######
## go_skull_pile
######*/

enum SkullPile
{
    OPTION_ID_GEZZARAK_THE_HUNTRESS = 0,
    OPTION_ID_DARKSCREECHER_AKKARAI = 1,
    OPTION_ID_KARROG = 2,
    OPTION_ID_VAKKIZ_THE_WINDRAGER = 3,
    GOSSIP_MENU_ID_SKULL_PILE = 8660,
    ADVERSARIAL_BLOOD = 11885,
    SUMMON_GEZZARAK_THE_HUNTRESS = 40632,
    SUMMON_KARROG = 40640,
    SUMMON_DARKSCREECHER_AKKARAI = 40642,
    SUMMON_VAKKIZ_THE_WINDRAGER = 40644
};

class go_skull_pile : public GameObjectScript
{
public:
    go_skull_pile() : GameObjectScript("go_skull_pile") { }

    struct go_skull_pileAI : public GameObjectAI
    {
        go_skull_pileAI(GameObject* go) : GameObjectAI(go) { }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            ClearGossipMenuFor(player);
            switch (gossipListId)
            {
            case GOSSIP_SENDER_MAIN:
                SendActionMenu(player, me, gossipListId);
                break;
            }
            return true;
        }

        bool GossipHello(Player* player) override
        {
            if ((player->GetQuestStatus(ADVERSARIAL_BLOOD) == QUEST_STATUS_INCOMPLETE) || player->GetQuestRewardStatus(ADVERSARIAL_BLOOD))
            {
                AddGossipItemFor(player, GOSSIP_MENU_ID_SKULL_PILE, OPTION_ID_GEZZARAK_THE_HUNTRESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                AddGossipItemFor(player, GOSSIP_MENU_ID_SKULL_PILE, OPTION_ID_DARKSCREECHER_AKKARAI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                AddGossipItemFor(player, GOSSIP_MENU_ID_SKULL_PILE, OPTION_ID_KARROG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                AddGossipItemFor(player, GOSSIP_MENU_ID_SKULL_PILE, OPTION_ID_VAKKIZ_THE_WINDRAGER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            }

            SendGossipMenuFor(player, me->GetGOInfo()->questgiver.gossipID, me->GetGUID());
            return true;
        }

    private:
        void SendActionMenu(Player* player, GameObject* /*go*/, uint32 action) const
        {
            switch (action)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->CastSpell(player, SUMMON_GEZZARAK_THE_HUNTRESS, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                player->CastSpell(player, SUMMON_DARKSCREECHER_AKKARAI, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                player->CastSpell(player, SUMMON_KARROG, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                player->CastSpell(player, SUMMON_VAKKIZ_THE_WINDRAGER, false);
                break;
            }
        }
    };

    GameObjectAI* GetAI(GameObject* go) const override
    {
        return new go_skull_pileAI(go);
    }
};

/*######
## npc_slim
######*/

enum Slim
{
    FACTION_CONSORTIUM = 933,
    NPC_TEXT_NEITHER_SLIM_NOR_SHADY = 9895,
    NPC_TEXT_I_SEE_YOU_ARE_A_FRIEND = 9896
};

class npc_slim : public CreatureScript
{
public:
    npc_slim() : CreatureScript("npc_slim") { }

    struct npc_slimAI : public ScriptedAI
    {
        npc_slimAI(Creature* creature) : ScriptedAI(creature) { }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            ClearGossipMenuFor(player);
            if (gossipListId == GOSSIP_ACTION_TRADE)
                player->GetSession()->SendListInventory(me->GetGUID());

            return true;
        }

        bool GossipHello(Player* player) override
        {
            if (me->IsVendor() && player->GetReputationRank(FACTION_CONSORTIUM) >= REP_FRIENDLY)
            {
                AddGossipItemFor(player, GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
                SendGossipMenuFor(player, NPC_TEXT_I_SEE_YOU_ARE_A_FRIEND, me->GetGUID());
            }
            else
                SendGossipMenuFor(player, NPC_TEXT_NEITHER_SLIM_NOR_SHADY, me->GetGUID());

            return true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_slimAI(creature);
    }
};

/*########
####npc_akuno
#####*/

enum Akuno
{
    QUEST_ESCAPING_THE_TOMB = 10887,
    NPC_CABAL_SKRIMISHER = 21661
};

// lfm scripts 
class npc_teribus_the_cursed : public CreatureScript
{
public:
    npc_teribus_the_cursed() : CreatureScript("npc_teribus_the_cursed") { }

    struct npc_teribus_the_cursedAI : public ScriptedAI
    {
        npc_teribus_the_cursedAI(Creature* creature) : ScriptedAI(creature)
        {
            called = false;
            destination.m_positionX = -3418.21f;
            destination.m_positionY = 4453.82f;
            destination.m_positionZ = -7.96f;
            movementCheckDelay = 0;
            soundDelay = urand(20000, 40000);
            calledDelay = 300000;
            shadowShockDelay = urand(3000, 8000);
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    // start summoning
                    me->SetVisible(false);
                    if (TempSummon* summonedMe = me->ToTempSummon())
                    {
                        if (Unit* summoner = summonedMe->GetSummoner())
                        {
                            summoner->CastSpell(summoner, 39180);
                        }
                    }
                    std::list<Creature*> teribusList;
                    me->GetCreatureListWithEntryInGrid(teribusList, 22441, 500.0f);
                    for (Creature* teribus : teribusList)
                    {
                        if (teribus->IsAlive())
                        {
                            if (teribus->GetGUID() != me->GetGUID())
                            {
                                teribus->AI()->SetData(1, 2);
                                me->TextEmote("Teribus the Cursed heard the summons.");
                                break;
                            }
                        }
                    }
                    me->DespawnOrUnsummon(100);
                }
                else if (data == 2)
                {
                    // called
                    called = true;
                    me->SetDefaultMovementType(MovementGeneratorType::IDLE_MOTION_TYPE);
                    me->GetMotionMaster()->Initialize();
                    me->GetMotionMaster()->MovePoint(0, destination);
                    movementCheckDelay = 5000;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (called)
            {
                if (calledDelay >= 0)
                {
                    calledDelay -= diff;
                }
                if (calledDelay < 0)
                {
                    if (me->IsInCombat())
                    {
                        me->DespawnOrUnsummon(100, 10s);
                    }
                }
                if (UpdateVictim())
                {
                    if (shadowShockDelay >= 0)
                    {
                        shadowShockDelay -= diff;
                    }
                    if (shadowShockDelay < 0)
                    {
                        shadowShockDelay = urand(5000, 8000);
                        DoCastVictim(24458);
                    }
                    DoMeleeAttackIfReady();
                }
                else
                {
                    if (movementCheckDelay >= 0)
                    {
                        movementCheckDelay -= diff;
                    }
                    if (movementCheckDelay < 0)
                    {
                        movementCheckDelay = 5000;
                        if (!me->isMoving())
                        {
                            float destDistance = me->GetDistance(destination);
                            if (destDistance > 2.0f)
                            {
                                me->SetDefaultMovementType(MovementGeneratorType::IDLE_MOTION_TYPE);
                                me->GetMotionMaster()->Initialize();
                                me->GetMotionMaster()->MovePoint(0, destination);
                            }
                            else
                            {
                                me->SetHomePosition(destination);
                                me->SetImmuneToAll(false);
                                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            }
                        }
                    }
                }
            }
            else
            {
                if (movementCheckDelay >= 0)
                {
                    movementCheckDelay -= diff;
                }
                if (soundDelay >= 0)
                {
                    soundDelay -= diff;
                }
                if (movementCheckDelay < 0)
                {
                    movementCheckDelay = 5000;
                    if (!me->isMoving())
                    {
                        me->GetMotionMaster()->MovePath(6022441, true);
                    }
                }
                if (soundDelay < 0)
                {
                    soundDelay = urand(20000, 40000);
                    me->PlayDirectSound(7274);
                }
            }
        }

        bool called;
        Position destination;
        int movementCheckDelay;
        int soundDelay;
        int calledDelay;
        int shadowShockDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_teribus_the_cursedAI(creature);
    }
};

class npc_adyen_the_lightwarden : public CreatureScript
{
public:
    npc_adyen_the_lightwarden() : CreatureScript("npc_adyen_the_lightwarden") { }

    struct npc_adyen_the_lightwardenAI : public ScriptedAI
    {
        npc_adyen_the_lightwardenAI(Creature* creature) : ScriptedAI(creature)
        {
            deathblowStatus = 0;
            eventID = 0;
            eventDelay = 0;
            hammerDelay = urand(5000, 8000);
            crusaderDelay = urand(2000, 5000);
            holyDelay = urand(10000, 15000);
        }

        void Reset() override
        {
            hammerDelay = urand(5000,8000);
            crusaderDelay = urand(2000, 5000);
            holyDelay = urand(10000, 15000);
        }

        uint32 GetData(uint32 type) const override
        {
            if (type == 1)
            {
                return deathblowStatus;
            }

            return 0;
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 0)
                {
                    deathblowStatus = 0;
                }
                else if (data == 1)
                {
                    deathblowStatus = 1;
                }
                else if (data == 2)
                {
                    eventID = 22;
                    eventDelay = 5000;
                }
            }
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            if (deathblowStatus == 1)
            {
                me->SetHomePosition(me->GetPosition());
            }
        }

        bool GossipHello(Player* player) override
        {
            if (deathblowStatus == 1)
            {                
                AddGossipItemFor(player, 100000, 0, 1, 0);
                SendGossipMenuFor(player, 10210, me->GetGUID());
                return true;
            }            
            return false;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            if (gossipListId == 0)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetImmuneToAll(false);
                me->SetFaction(250);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->SetWalk(true);
                if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                {
                    orelis->SetImmuneToAll(false);
                    orelis->SetFaction(250);
                    orelis->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    orelis->SetWalk(true);
                }
                if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                {
                    karja->SetImmuneToAll(false);
                    karja->SetFaction(250);
                    karja->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    karja->SetWalk(true);
                }

                eventID = 1;
                eventDelay = 2000;
                CloseGossipMenuFor(player);
            }
            return true;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (hammerDelay >= 0)
                {
                    hammerDelay -= diff;
                }
                if (crusaderDelay >= 0)
                {
                    crusaderDelay -= diff;
                }
                if (holyDelay >= 0)
                {
                    holyDelay -= diff;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (hammerDelay < 0)
                    {
                        hammerDelay = 1000;
                        DoCastVictim(13005);
                        hammerDelay = urand(20000, 30000);
                        return;
                    }
                    if (crusaderDelay < 0)
                    {
                        crusaderDelay = 1000;
                        DoCastVictim(14518);
                        crusaderDelay = urand(5000, 8000);
                        return;
                    }
                }
                if (holyDelay < 0)
                {
                    holyDelay = 2000;
                    std::list<Unit*> unitList;
                    Trinity::AnyUnitInObjectRangeCheck go_check(me, 20.0f);
                    Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> go_search(me, unitList, go_check);
                    Cell::VisitGridObjects(me, go_search, 20.0f);
                    if (!unitList.empty())
                    {
                        for (std::list<Unit*>::iterator uIT = unitList.begin(); uIT != unitList.end(); uIT++)
                        {
                            if (Unit* eachUnit = *uIT)
                            {
                                if (eachUnit->IsFriendlyTo(me))
                                {
                                    float eachPCT = eachUnit->GetHealthPct();
                                    if (eachPCT < 40.0f)
                                    {
                                        DoCast(eachUnit, 13952);
                                        holyDelay = urand(10000, 15000);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }
            else
            {
                if (eventDelay >= 0)
                {
                    eventDelay -= diff;
                }
                if (eventDelay < 0)
                {
                    eventDelay = 2000;
                    switch (eventID)
                    {
                    case 0:
                    {
                        eventDelay = 10000;
                        break;
                    }
                    case 1:
                    {
                        me->GetMotionMaster()->MovePath(18537, false);
                        if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                        {
                            orelis->GetMotionMaster()->MoveFollow(me, 2.0f, M_PI * 3 / 4);
                        }
                        if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                        {
                            karja->GetMotionMaster()->MoveFollow(me, 2.0f, M_PI * 5 / 4);
                        }
                        eventID = 2;
                        eventDelay = 5000;
                        break;
                    }
                    case 2:
                    {
                        if (!me->isMoving())
                        {
                            if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                            {
                                eventID = 3;
                                eventDelay = 1000;
                            }
                        }
                        else
                        {
                            me->SetHomePosition(me->GetPosition());
                            if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                            {
                                orelis->SetHomePosition(orelis->GetPosition());
                                if (!orelis->isMoving())
                                {
                                    orelis->GetMotionMaster()->MoveFollow(me, 2.0f, M_PI * 5 / 4);
                                }
                            }
                            if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                            {
                                karja->SetHomePosition(karja->GetPosition());
                                if (!karja->isMoving())
                                {
                                    karja->GetMotionMaster()->MoveFollow(me, 2.0f, M_PI * 3 / 4);
                                }
                            }
                        }
                        break;
                    }
                    case 3:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            me->SetFacingToObject(socrethar);
                            socrethar->SetFacingToObject(me);
                            Talk(0);
                            if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                            {
                                orelis->SetHomePosition(orelis->GetPosition());
                                orelis->StopMoving();
                                orelis->GetMotionMaster()->Initialize();
                                orelis->SetFacingToObject(socrethar);
                            }
                            if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                            {
                                karja->SetHomePosition(karja->GetPosition());
                                karja->StopMoving();
                                karja->GetMotionMaster()->Initialize();
                                karja->SetFacingToObject(socrethar);
                            }
                            eventID = 4;
                            eventDelay = 11000;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->Talk(0);
                            eventID = 5;
                            eventDelay = 6000;
                        }
                        break;
                    }
                    case 5:
                    {
                        Talk(1);
                        eventID = 6;
                        eventDelay = 9000;
                        break;
                    }
                    case 6:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->Talk(1);
                            eventID = 7;
                            eventDelay = 5000;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 100.0f))
                        {
                            if (!kaylaan->isMoving())
                            {
                                kaylaan->SetVisible(true);
                                float destX = 4940.22f, destY = 3847.43f, destZ = 211.53f;
                                float destinationDistance = kaylaan->GetDistance(destX, destY, destZ);
                                if (destinationDistance > 2.0f)
                                {
                                    kaylaan->GetMotionMaster()->MovePoint(1, destX, destY, destZ);
                                }
                                else
                                {
                                    if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                                    {
                                        kaylaan->SetFacingToObject(socrethar);
                                        eventID = 8;
                                        eventDelay = 1000;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 8:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                            me->SetFacingToObject(kaylaan);
                            if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                            {
                                orelis->SetFacingToObject(kaylaan);
                            }
                            if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                            {
                                karja->SetFacingToObject(kaylaan);
                            }
                            eventID = 9;
                            eventDelay = 1000;
                        }
                        break;
                    }
                    case 9:
                    {
                        if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                        {
                            orelis->AI()->Talk(2);
                            eventID = 10;
                            eventDelay = 2000;
                        }
                        break;
                    }
                    case 10:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                            eventID = 11;
                            eventDelay = 1000;
                        }
                        break;
                    }
                    case 11:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->SetFacingToObject(me);
                            eventID = 12;
                            eventDelay = 1000;
                        }
                        break;
                    }
                    case 12:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->AI()->Talk(0);
                            eventID = 14;
                            eventDelay = 12000;
                        }
                        break;
                    }
                    case 13:
                    {
                        break;
                    }
                    case 14:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->AI()->Talk(1);
                            eventID = 15;
                            eventDelay = 11000;
                        }
                        break;
                    }
                    case 15:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->AI()->Talk(2);
                            eventID = 16;
                            eventDelay = 12000;
                        }
                        break;
                    }
                    case 16:
                    {
                        Talk(2);
                        eventID = 17;
                        eventDelay = 9000;
                        break;
                    }
                    case 17:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->AI()->Talk(3);
                            eventID = 18;
                            eventDelay = 5000;
                        }
                        break;
                    }
                    case 18:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                            {
                                kaylaan->SetFaction(socrethar->GetFaction());
                                socrethar->CastSpell(kaylaan, 35596);
                                eventID = 19;
                                eventDelay = 1000;
                            }
                        }
                        break;
                    }
                    case 19:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->Talk(2);
                            eventID = 20;
                            eventDelay = 5000;
                        }
                        break;
                    }
                    case 20:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            kaylaan->SetHomePosition(kaylaan->GetPosition());
                            kaylaan->SetImmuneToAll(false);
                            kaylaan->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            kaylaan->SetFaction(14);
                            eventID = 21;
                            eventDelay = 500;
                        }
                        break;
                    }
                    case 21:
                    {
                        if (Creature* kaylaan = me->FindNearestCreature(20794, 50.0f))
                        {
                            AttackStart(kaylaan);
                            kaylaan->AI()->AttackStart(me);
                            if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                            {
                                orelis->AI()->AttackStart(kaylaan);
                            }
                            if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                            {
                                karja->AI()->AttackStart(kaylaan);
                            }
                        }
                        break;
                    }
                    case 22:
                    {
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

        int deathblowStatus;
        int hammerDelay;
        int crusaderDelay;
        int holyDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_adyen_the_lightwardenAI(creature);
    }
};

class npc_ishanah : public CreatureScript
{
public:
    npc_ishanah() : CreatureScript("npc_ishanah") { }

    struct npc_ishanahAI : public ScriptedAI
    {
        npc_ishanahAI(Creature* creature) : ScriptedAI(creature)
        {
            shieldDelay = urand(5000, 8000);
            smiteDelay = 2000;
            holyDelay = urand(10000, 15000);
        }

        void Reset() override
        {
            shieldDelay = urand(5000, 8000);
            smiteDelay = 2000;
            holyDelay = urand(10000, 15000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (holyDelay >= 0)
            {
                holyDelay -= diff;
            }
            if (holyDelay < 0)
            {
                holyDelay = 2000;
                std::list<Unit*> unitList;
                Trinity::AnyUnitInObjectRangeCheck go_check(me, 20.0f);
                Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> go_search(me, unitList, go_check);
                Cell::VisitGridObjects(me, go_search, 20.0f);
                if (!unitList.empty())
                {
                    for (std::list<Unit*>::iterator uIT = unitList.begin(); uIT != unitList.end(); uIT++)
                    {
                        if (Unit* eachUnit = *uIT)
                        {
                            if (eachUnit->IsFriendlyTo(me))
                            {
                                float eachPCT = eachUnit->GetHealthPct();
                                if (eachPCT < 40.0f)
                                {
                                    DoCast(eachUnit, 35096);
                                    holyDelay = urand(10000, 15000);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            if (UpdateVictim())
            {
                if (smiteDelay >= 0)
                {
                    smiteDelay -= diff;
                }
                if (shieldDelay >= 0)
                {
                    shieldDelay -= diff;
                }
                if (smiteDelay < 0)
                {
                    smiteDelay = 2000;
                    DoCastVictim(15238);
                    return;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (shieldDelay < 0)
                    {
                        shieldDelay = 1000;
                        if (Unit* victimVictim = victim->GetVictim())
                        {
                            DoCast(victimVictim, 22187);
                            shieldDelay = urand(30000, 40000);
                            return;
                        }
                    }
                }
                DoMeleeAttackIfReady();
            }
        }
        
        int shieldDelay;
        int smiteDelay;
        int holyDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_ishanahAI(creature);
    }
};

void AddSC_terokkar_forest()
{
    new npc_unkor_the_ruthless();
    new npc_infested_root_walker();
    new npc_rotting_forest_rager();
    new npc_floon();
    new npc_isla_starmane();
    new go_skull_pile();
    new npc_slim();

    // lfm scripts
    new npc_teribus_the_cursed();
    new npc_adyen_the_lightwarden();
    new npc_ishanah();
}
