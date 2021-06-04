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
SDName: Netherstorm
SD%Complete: 80
SDComment: Quest support: 10337, 10652 (special flight paths), 10198, 10191
SDCategory: Netherstorm
EndScriptData */

/* ContentData
npc_commander_dawnforge
npc_bessy
npc_maxx_a_million
go_captain_tyralius_prison
EndContentData */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"

#include "GridNotifiers.h"

/*######
## npc_commander_dawnforge
######*/

// The Speech of Dawnforge, Ardonis & Pathaleon
enum CommanderDawnforgeData
{
    SAY_COMMANDER_DAWNFORGE_1       = 0,
    SAY_COMMANDER_DAWNFORGE_2       = 1,
    SAY_COMMANDER_DAWNFORGE_3       = 2,
    SAY_COMMANDER_DAWNFORGE_4       = 3,
    SAY_COMMANDER_DAWNFORGE_5       = 4,

    SAY_ARCANIST_ARDONIS_1          = 0,
    SAY_ARCANIST_ARDONIS_2          = 1,

    SAY_PATHALEON_CULATOR_IMAGE_1   = 0,
    SAY_PATHALEON_CULATOR_IMAGE_2   = 1,
    SAY_PATHALEON_CULATOR_IMAGE_2_1 = 2,
    SAY_PATHALEON_CULATOR_IMAGE_2_2 = 3,

    QUEST_INFO_GATHERING            = 10198,
    SPELL_SUNFURY_DISGUISE          = 34603,
};

// Entries of Arcanist Ardonis, Commander Dawnforge, Pathaleon the Curators Image
const uint32 CreatureEntry[3] =
{
    19830,                                                // Ardonis
    19831,                                                // Dawnforge
    21504                                                 // Pathaleon
};

class npc_commander_dawnforge : public CreatureScript
{
public:
    npc_commander_dawnforge() : CreatureScript("npc_commander_dawnforge") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_commander_dawnforgeAI(creature);
    }

    struct npc_commander_dawnforgeAI : public ScriptedAI
    {
        npc_commander_dawnforgeAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            PlayerGUID.Clear();
            ardonisGUID.Clear();
            pathaleonGUID.Clear();

            Phase = 1;
            PhaseSubphase = 0;
            Phase_Timer = 4000;
            isEvent = false;
        }

        ObjectGuid PlayerGUID;
        ObjectGuid ardonisGUID;
        ObjectGuid pathaleonGUID;

        uint32 Phase;
        uint32 PhaseSubphase;
        uint32 Phase_Timer;
        bool isEvent;

        void Reset() override
        {
            Initialize();
        }

        void JustEngagedWith(Unit* /*who*/) override { }

        void JustSummoned(Creature* summoned) override
        {
            pathaleonGUID = summoned->GetGUID();
        }

        // Emote Ardonis and Pathaleon
        void Turn_to_Pathaleons_Image()
        {
            Creature* ardonis = ObjectAccessor::GetCreature(*me, ardonisGUID);
            Creature* pathaleon = ObjectAccessor::GetCreature(*me, pathaleonGUID);

            if (!ardonis || !pathaleon)
                return;

            // Turn Dawnforge
            me->SetFacingToObject(pathaleon);

            // Turn Ardonis
            ardonis->SetFacingToObject(pathaleon);

            //Set them to kneel
            me->SetStandState(UNIT_STAND_STATE_KNEEL);
            ardonis->SetStandState(UNIT_STAND_STATE_KNEEL);
        }

        //Set them back to each other
        void Turn_to_eachother()
        {
            if (Unit* ardonis = ObjectAccessor::GetUnit(*me, ardonisGUID))
            {
                // Turn Dawnforge
                me->SetFacingToObject(ardonis);

                // Turn Ardonis
                ardonis->SetFacingToObject(me);

                //Set state
                me->SetStandState(UNIT_STAND_STATE_STAND);
                ardonis->SetStandState(UNIT_STAND_STATE_STAND);
            }
        }

        bool CanStartEvent(Player* player)
        {
            if (!isEvent)
            {
                Creature* ardonis = me->FindNearestCreature(CreatureEntry[0], 10.0f);
                if (!ardonis)
                    return false;

                ardonisGUID = ardonis->GetGUID();
                PlayerGUID = player->GetGUID();

                isEvent = true;

                Turn_to_eachother();
                return true;
            }

            TC_LOG_DEBUG("scripts", "npc_commander_dawnforge event already in progress, need to wait.");
            return false;
        }

        void UpdateAI(uint32 diff) override
        {
            //Is event even running?
            if (!isEvent)
                return;

            //Phase timing
            if (Phase_Timer >= diff)
            {
                Phase_Timer -= diff;
                return;
            }

            Creature* ardonis = ObjectAccessor::GetCreature(*me, ardonisGUID);
            Creature* pathaleon = ObjectAccessor::GetCreature(*me, pathaleonGUID);
            Player* player = ObjectAccessor::GetPlayer(*me, PlayerGUID);

            if (!ardonis || !player)
            {
                Reset();
                return;
            }

            if (Phase > 4 && !pathaleon)
            {
                Reset();
                return;
            }

            //Phase 1 Dawnforge say
            switch (Phase)
            {
            case 1:
                Talk(SAY_COMMANDER_DAWNFORGE_1);
                ++Phase;
                Phase_Timer = 16000;
                break;
                //Phase 2 Ardonis say
            case 2:
                ardonis->AI()->Talk(SAY_ARCANIST_ARDONIS_1);
                ++Phase;
                Phase_Timer = 16000;
                break;
                //Phase 3 Dawnforge say
            case 3:
                Talk(SAY_COMMANDER_DAWNFORGE_2);
                ++Phase;
                Phase_Timer = 16000;
                break;
                //Phase 4 Pathaleon spawns up to phase 9
            case 4:
                //spawn pathaleon's image
                me->SummonCreature(CreatureEntry[2], 2325.851563f, 2799.534668f, 133.084229f, 6.038996f, TEMPSUMMON_TIMED_DESPAWN, 90000);
                ++Phase;
                Phase_Timer = 500;
                break;
                //Phase 5 Pathaleon say
            case 5:
                pathaleon->AI()->Talk(SAY_PATHALEON_CULATOR_IMAGE_1);
                ++Phase;
                Phase_Timer = 6000;
                break;
                //Phase 6
            case 6:
                switch (PhaseSubphase)
                {
                    //Subphase 1: Turn Dawnforge and Ardonis
                case 0:
                    Turn_to_Pathaleons_Image();
                    ++PhaseSubphase;
                    Phase_Timer = 8000;
                    break;
                    //Subphase 2 Dawnforge say
                case 1:
                    Talk(SAY_COMMANDER_DAWNFORGE_3);
                    PhaseSubphase = 0;
                    ++Phase;
                    Phase_Timer = 8000;
                    break;
                }
                break;
                //Phase 7 Pathaleons say 3 Sentence, every sentence need a subphase
            case 7:
                switch (PhaseSubphase)
                {
                    //Subphase 1
                case 0:
                    pathaleon->AI()->Talk(SAY_PATHALEON_CULATOR_IMAGE_2);
                    ++PhaseSubphase;
                    Phase_Timer = 12000;
                    break;
                    //Subphase 2
                case 1:
                    pathaleon->AI()->Talk(SAY_PATHALEON_CULATOR_IMAGE_2_1);
                    ++PhaseSubphase;
                    Phase_Timer = 16000;
                    break;
                    //Subphase 3
                case 2:
                    pathaleon->AI()->Talk(SAY_PATHALEON_CULATOR_IMAGE_2_2);
                    PhaseSubphase = 0;
                    ++Phase;
                    Phase_Timer = 10000;
                    break;
                }
                break;
                //Phase 8 Dawnforge & Ardonis say
            case 8:
                Talk(SAY_COMMANDER_DAWNFORGE_4);
                ardonis->AI()->Talk(SAY_ARCANIST_ARDONIS_2);
                ++Phase;
                Phase_Timer = 4000;
                break;
                //Phase 9 Pathaleons Despawn, Reset Dawnforge & Ardonis angle
            case 9:
                Turn_to_eachother();
                //hide pathaleon, unit will despawn shortly
                pathaleon->SetVisible(false);
                PhaseSubphase = 0;
                ++Phase;
                Phase_Timer = 3000;
                break;
                //Phase 10 Dawnforge say
            case 10:
                Talk(SAY_COMMANDER_DAWNFORGE_5);
                player->AreaExploredOrEventHappens(QUEST_INFO_GATHERING);
                Reset();
                break;
            }
         }
    };
};

class at_commander_dawnforge : public AreaTriggerScript
{
public:
    at_commander_dawnforge() : AreaTriggerScript("at_commander_dawnforge") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*at*/) override
    {
        //if player lost aura or not have at all, we should not try start event.
        if (!player->HasAura(SPELL_SUNFURY_DISGUISE))
            return false;

        if (player->IsAlive() && player->GetQuestStatus(QUEST_INFO_GATHERING) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* Dawnforge = player->FindNearestCreature(CreatureEntry[1], 30.0f);
            if (!Dawnforge)
                return false;

            if (ENSURE_AI(npc_commander_dawnforge::npc_commander_dawnforgeAI, Dawnforge->AI())->CanStartEvent(player))
                return true;
        }
        return false;
    }
};

/*######
## npc_phase_hunter
######*/

enum PhaseHunterData
{
    QUEST_RECHARGING_THE_BATTERIES = 10190,

    NPC_PHASE_HUNTER_ENTRY = 18879,
    NPC_DRAINED_PHASE_HUNTER_ENTRY = 19595,

    EMOTE_WEAK = 0,

    // Spells
    SPELL_RECHARGING_BATTERY = 34219,
    SPELL_PHASE_SLIP = 36574,
    SPELL_MANA_BURN = 37176,
    SPELL_ARCANE_EXPLOSION = 25679,
    SPELL_MATERIALIZE = 34804,
};

class npc_phase_hunter : public CreatureScript
{
public:
    npc_phase_hunter() : CreatureScript("npc_phase_hunter") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_phase_hunterAI(creature);
    }

    struct npc_phase_hunterAI : public ScriptedAI
    {
        npc_phase_hunterAI(Creature* creature) : ScriptedAI(creature)
        {
            Weak = false;
            Materialize = false;
            Drained = false;
            WeakPercent = 25;
            slipDelay = 0;
            arcaneExplosionDelay = urand(5000, 8000);
            manaBurnDelay = urand(8000, 12000);
        }

        bool Weak;
        bool Materialize;
        bool Drained;
        uint8 WeakPercent;

        ObjectGuid PlayerGUID;

        void Reset() override
        {
            Weak = false;
            Materialize = false;
            Drained = false;
            WeakPercent = 25 + (rand32() % 16); // 25-40

            PlayerGUID.Clear();

            slipDelay = 0;
            arcaneExplosionDelay = urand(2000, 5000);
            manaBurnDelay = urand(5000, 8000);

            if (me->GetEntry() == NPC_DRAINED_PHASE_HUNTER_ENTRY)
                me->UpdateEntry(NPC_PHASE_HUNTER_ENTRY);
        }

        void JustEngagedWith(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                PlayerGUID = who->GetGUID();
        }

        void UpdateAI(uint32 diff) override
        {
            if (!Materialize)
            {
                DoCast(me, SPELL_MATERIALIZE);
                Materialize = true;
            }

            if (!UpdateVictim())
                return;

            if (slipDelay >= 0)
            {
                slipDelay -= diff;
            }
            if (arcaneExplosionDelay >= 0)
            {
                arcaneExplosionDelay -= diff;
            }
            if (manaBurnDelay >= 0)
            {
                manaBurnDelay -= diff;
            }
            if (slipDelay < 0)
            {
                slipDelay = urand(15000, 20000);
                DoCastSelf(SPELL_PHASE_SLIP);
            }
            if (arcaneExplosionDelay < 0)
            {
                arcaneExplosionDelay = urand(10000, 15000);
                DoCastVictim(SPELL_ARCANE_EXPLOSION);
            }
            if (manaBurnDelay < 0)
            {
                manaBurnDelay = urand(15000, 20000);
                DoCastVictim(SPELL_MANA_BURN);
            }

            if (Player* player = ObjectAccessor::GetPlayer(*me, PlayerGUID)) // start: support for quest 10190
            {
                if (!Weak && HealthBelowPct(WeakPercent)
                    && player->GetQuestStatus(QUEST_RECHARGING_THE_BATTERIES) == QUEST_STATUS_INCOMPLETE)
                {
                    Talk(EMOTE_WEAK);
                    Weak = true;
                }
                if (Weak && !Drained && me->HasAura(SPELL_RECHARGING_BATTERY))
                {
                    Drained = true;
                    int32 uHpPct = int32(me->GetHealthPct());

                    me->UpdateEntry(NPC_DRAINED_PHASE_HUNTER_ENTRY);

                    me->SetHealth(me->CountPctFromMaxHealth(uHpPct));
                    me->LowerPlayerDamageReq(me->GetMaxHealth() - me->GetHealth());
                    me->SetInCombatWith(player);
                }
            } // end: support for quest 10190

            DoMeleeAttackIfReady();
        }

        int arcaneExplosionDelay;
        int manaBurnDelay;
        int slipDelay;
    };
};

/*######
## npc_bessy
######*/
enum BessyData
{
    Q_ALMABTRIEB    = 10337,
    N_THADELL       = 20464,
    SPAWN_FIRST     = 20512,
    SPAWN_SECOND    = 19881,
    SAY_THADELL_1   = 0,
    SAY_THADELL_2   = 1,
};

class npc_bessy : public CreatureScript
{
    public:
        npc_bessy() : CreatureScript("npc_bessy") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_bessyAI(creature);
        }

        struct npc_bessyAI : public EscortAI
        {
            npc_bessyAI(Creature* creature) : EscortAI(creature) { }

            void QuestAccept(Player* player, Quest const* quest) override
            {
                if (quest->GetQuestId() == Q_ALMABTRIEB)
                {
                    me->SetFaction(FACTION_ESCORTEE_N_NEUTRAL_PASSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    Start(true, false, player->GetGUID());
                }
            }

            void JustDied(Unit* /*killer*/) override
            {
                if (Player* player = GetPlayerForEscort())
                    player->FailQuest(Q_ALMABTRIEB);
            }

            void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
            {
                Player* player = GetPlayerForEscort();
                if (!player)
                    return;

                switch (waypointId)
                {
                    case 3: //first spawn
                        me->SummonCreature(SPAWN_FIRST, 2449.67f, 2183.11f, 96.85f, 6.20f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_FIRST, 2449.53f, 2184.43f, 96.36f, 6.27f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_FIRST, 2449.85f, 2186.34f, 97.57f, 6.08f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        break;
                    case 7:
                        me->SummonCreature(SPAWN_SECOND, 2309.64f, 2186.24f, 92.25f, 6.06f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        me->SummonCreature(SPAWN_SECOND, 2309.25f, 2183.46f, 91.75f, 6.22f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                        break;
                    case 12:
                        player->GroupEventHappens(Q_ALMABTRIEB, me);
                        if (Creature* thadell = me->FindNearestCreature(N_THADELL, 30))
                        {
                            thadell->AI()->Talk(SAY_THADELL_1);
                        }
                        break;
                    case 13:
                        if (Creature* thadell = me->FindNearestCreature(N_THADELL, 30))
                        {
                            thadell->AI()->Talk(SAY_THADELL_2, player);
                        }
                        break;
                }
            }

            void JustSummoned(Creature* summoned) override
            {
                summoned->AI()->AttackStart(me);
            }

            void Reset() override
            {
                me->RestoreFaction();
            }
        };
};

/*######
## npc_maxx_a_million
######*/

enum MaxxAMillion
{
    QUEST_MARK_V_IS_ALIVE   = 10191,
    GO_DRAENEI_MACHINE      = 183771
};

class npc_maxx_a_million_escort : public CreatureScript
{
public:
    npc_maxx_a_million_escort() : CreatureScript("npc_maxx_a_million_escort") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_maxx_a_million_escortAI(creature);
    }

    struct npc_maxx_a_million_escortAI : public EscortAI
    {
        npc_maxx_a_million_escortAI(Creature* creature) : EscortAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            bTake = false;
            uiTakeTimer = 3000;
        }

        bool bTake;
        uint32 uiTakeTimer;

        void Reset() override
        {
            Initialize();
        }

        void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 7:
                case 17:
                case 29:
                    //Find Object and "work"
                    if (GetClosestGameObjectWithEntry(me, GO_DRAENEI_MACHINE, INTERACTION_DISTANCE))
                    {
                        // take the GO -> animation
                        me->HandleEmoteCommand(EMOTE_STATE_LOOT);
                        SetEscortPaused(true);
                        bTake=true;
                    }
                    break;
                case 36: //return and quest_complete
                    player->CompleteQuest(QUEST_MARK_V_IS_ALIVE);
                    break;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Player* player = GetPlayerForEscort())
                player->FailQuest(QUEST_MARK_V_IS_ALIVE);
        }

        void UpdateAI(uint32 uiDiff) override
        {
            EscortAI::UpdateAI(uiDiff);

            if (bTake)
            {
                if (uiTakeTimer < uiDiff)
                {
                    me->HandleEmoteCommand(EMOTE_STATE_NONE);
                    if (GameObject* go = GetClosestGameObjectWithEntry(me, GO_DRAENEI_MACHINE, INTERACTION_DISTANCE))
                    {
                        SetEscortPaused(false);
                        bTake=false;
                        uiTakeTimer = 3000;
                        go->Delete();
                    }
                }
                else
                    uiTakeTimer -= uiDiff;
            }
            DoMeleeAttackIfReady();
        }

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_MARK_V_IS_ALIVE)
            {
                me->SetFaction(FACTION_ESCORTEE_N_NEUTRAL_PASSIVE);
                Start(false, false, player->GetGUID());
            }
        }
    };
};

/*######
## go_captain_tyralius_prison
######*/

enum CaptainTyralius
{
    NPC_CAPTAIN_TYRALIUS    = 20787,
    SAY_FREE                = 0,
};

class go_captain_tyralius_prison : public GameObjectScript
{
    public:
        go_captain_tyralius_prison() : GameObjectScript("go_captain_tyralius_prison") { }

        struct go_captain_tyralius_prisonAI : public GameObjectAI
        {
            go_captain_tyralius_prisonAI(GameObject* go) : GameObjectAI(go) { }

            bool GossipHello(Player* player) override
            {
                me->UseDoorOrButton();
                if (Creature* tyralius = me->FindNearestCreature(NPC_CAPTAIN_TYRALIUS, 1.0f))
                {
                    player->KilledMonsterCredit(NPC_CAPTAIN_TYRALIUS);
                    tyralius->AI()->Talk(SAY_FREE);
                    tyralius->DespawnOrUnsummon(8000);
                }
                return true;
            }
        };

        GameObjectAI* GetAI(GameObject* go) const override
        {
            return new go_captain_tyralius_prisonAI(go);
        }
};

// lfm scripts 
class npc_scrapped_fel_reaver : public CreatureScript
{
public:
    npc_scrapped_fel_reaver() : CreatureScript("npc_scrapped_fel_reaver") { }

    struct npc_scrapped_fel_reaverAI : public ScriptedAI
    {
        npc_scrapped_fel_reaverAI(Creature* creature) : ScriptedAI(creature)
        {
            summonDelay0 = 2000;
            summonDelay1 = 17000;
            summonDelay2 = 32000;
            summonDelay3 = 60000;
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) override
        {
            if (spell->Id == 35282)
            {
                me->SetImmuneToAll(false);
                Talk(0);
            }
        }

        void JustAppeared()
        {
            DoCastSelf(39311);
            me->SetReactState(ReactStates::REACT_PASSIVE);
        }

        void Reset() override
        {
            me->SetImmuneToAll(true);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (summonDelay0 >= 0)
                {
                    summonDelay0 -= diff;
                }
                if (summonDelay1 >= 0)
                {
                    summonDelay1 -= diff;
                }
                if (summonDelay2 < 0)
                {
                    summonDelay2 = 1000;
                }
                if (summonDelay3 < 0)
                {
                    summonDelay3 = 1000;
                }
                if (summonDelay0 < 0)
                {
                    summonDelay0 = 30000;
                    me->SummonCreature(20287, 2547.08f, 3982.24f, 131.39f, 2.01f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
                if (summonDelay1 < 0)
                {
                    summonDelay1 = 45000;
                    me->SummonCreature(20287, 2537.7f, 3975.96f, 130.4f, 1.58f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
                if (summonDelay2 < 0)
                {
                    summonDelay2 = 45000;
                    me->SummonCreature(20287, 2506.46f, 4008.93f, 133.8f, 6.19f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
                if (summonDelay3 < 0)
                {
                    summonDelay3 = 45000;
                    me->SummonCreature(20287, 2537.3f, 4027.11f, 135.5f, 4.3f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
            }
        }

        int summonDelay0;
        int summonDelay1;
        int summonDelay2;
        int summonDelay3;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_scrapped_fel_reaverAI(creature);
    }
};

class npc_scrap_reaver_x6000 : public CreatureScript
{
public:
    npc_scrap_reaver_x6000() : CreatureScript("npc_scrap_reaver_x6000") { }

    struct npc_scrap_reaver_x6000AI : public ScriptedAI
    {
        npc_scrap_reaver_x6000AI(Creature* creature) : ScriptedAI(creature)
        {
            deathDelay = 0;
        }

        void OnCharmed(bool apply) override
        {
            if (apply)
            {
                me->SetImmuneToAll(false);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);                
                if (Unit* charmer = me->GetCharmerOrOwner())
                {
                    me->GetMotionMaster()->MoveFollow(charmer, 2.0f, M_PI * 5 / 4);
                }
            }
        }

        void JustAppeared()
        {
            me->SetImmuneToAll(true);
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {                    
                    deathDelay = 1000;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (deathDelay > 0)
            {
                deathDelay -= diff;
                if (deathDelay <= 0)
                {
                    Talk(0);
                    me->KillSelf(false);
                    me->DespawnOrUnsummon(5000, 60s);
                }
            }
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
            }
        }

        int deathDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_scrap_reaver_x6000AI(creature);
    }
};

class npc_negatron : public CreatureScript
{
public:
    npc_negatron() : CreatureScript("npc_negatron") { }

    struct npc_negatronAI : public ScriptedAI
    {
        npc_negatronAI(Creature* creature) : ScriptedAI(creature)
        {
            moveCheckDelay = 2000;
            chargeDelay = 0;
            demolishDelay = urand(8000, 12000);
            earthquakeDelay = urand(15000,20000);
            fenzyDelay = 0;
            intro = true;
            destination.m_positionX = 3087.21f;
            destination.m_positionY = 3402.86f;
            destination.m_positionZ = 105.326f;
        }

        void JustAppeared()
        {
            DoCastSelf(15742);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            std::list<Creature*> doctorList;
            me->GetCreatureListWithEntryInGrid(doctorList, 19832, 100.0f);
            for (std::list<Creature*>::iterator itr = doctorList.begin(); itr != doctorList.end(); ++itr)
            {
                if (Creature* doctor = *itr)
                {                    
                    doctor->AI()->Talk(0);
                    break;
                }
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            std::list<Creature*> reaverList;
            me->GetCreatureListWithEntryInGrid(reaverList, 19849, 10.0f);
            for (std::list<Creature*>::iterator itr = reaverList.begin(); itr != reaverList.end(); ++itr)
            {
                if (Creature* reaver = *itr)
                {
                    reaver->AI()->SetData(1, 1);
                    if (Unit* co = reaver->GetCharmerOrOwner())
                    {
                        if (Player* coPlayer = co->ToPlayer())
                        {
                            coPlayer->AreaExploredOrEventHappens(10248);
                        }
                    }
                }
            }
        }

        void Reset() override
        {
            moveCheckDelay = 2000;
            chargeDelay = 0;
            demolishDelay = urand(10000, 15000);
            earthquakeDelay = urand(15000, 20000);
            fenzyDelay = 0;
            intro = true;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                if (chargeDelay >= 0)
                {
                    chargeDelay -= diff;
                }
                if (demolishDelay >= 0)
                {
                    demolishDelay -= diff;
                }
                if (earthquakeDelay >= 0)
                {
                    earthquakeDelay -= diff;
                }
                if (fenzyDelay >= 0)
                {
                    fenzyDelay -= diff;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (intro)
                    {
                        Talk(0);
                        intro = false;
                    }
                    if (chargeDelay < 0)
                    {
                        chargeDelay = 100;
                        float victimDistance = me->GetDistance(victim);
                        if (victimDistance > 8.0f && victimDistance < 25.0f)
                        {
                            DoCastVictim(35570);
                            chargeDelay = urand(15000, 20000);
                            return;
                        }
                    }
                    if (demolishDelay < 0)
                    {
                        demolishDelay = 1000;
                        if (me->IsWithinMeleeRange(victim))
                        {
                            DoCastVictim(34625);
                            demolishDelay = urand(8000, 12000);
                            return;
                        }
                    }
                    if (earthquakeDelay < 0)
                    {
                        earthquakeDelay = 1000;
                        if (me->IsWithinMeleeRange(victim))
                        {
                            DoCastVictim(35565);
                            earthquakeDelay = urand(20000, 25000);
                            return;
                        }
                    }
                    if (fenzyDelay < 0)
                    {
                        fenzyDelay = 1000;
                        float myHealthPCT = me->GetHealthPct();
                        if (myHealthPCT < 50.0f)
                        {
                            DoCastSelf(34624);
                            fenzyDelay = urand(15000, 20000);
                            return;
                        }
                    }
                }
            }
            else
            {
                if (moveCheckDelay >= 0)
                {
                    moveCheckDelay -= diff;
                }
                if (moveCheckDelay < 0)
                {
                    moveCheckDelay = 2000;
                    if (!me->isMoving())
                    {
                        me->GetMotionMaster()->MovePoint(1, destination);
                    }
                }
            }
        }

        int moveCheckDelay;
        int chargeDelay;
        int demolishDelay;
        int earthquakeDelay;
        int fenzyDelay;
        bool intro;
        Position destination;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_negatronAI(creature);
    }
};

class npc_protectorate_defender : public CreatureScript
{
public:
    npc_protectorate_defender() : CreatureScript("npc_protectorate_defender") { }

    struct npc_protectorate_defenderAI : public ScriptedAI
    {
        npc_protectorate_defenderAI(Creature* creature) : ScriptedAI(creature)
        {
            strikeDelay = 0;
            hamstringDelay = 0;
            eventID = 0;
            eventDelay = 0;
        }

        void Reset() override
        {
            strikeDelay = 0;
            hamstringDelay = 0;
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->SetHomePosition(me->GetPosition());
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    if (Creature* saeed = me->FindNearestCreature(20985, 50.0f))
                    {
                        me->SetFacingToObject(saeed);
                    }
                }
                else if (data == 2)
                {
                    me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                }
                else if (data == 3)
                {
                    me->SetImmuneToAll(false);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->SetFaction(1807);
                    eventID = 1;
                    eventDelay = 1000;
                }
                else if (data == 4)
                {
                    eventID = 2;
                    eventDelay = 8000;
                }
                else if (data == 5)
                {
                    eventID = 3;
                    eventDelay = urand(1000, 5000);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                if (strikeDelay >= 0)
                {
                    strikeDelay -= diff;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (strikeDelay < 0)
                    {
                        strikeDelay = 1000;
                        if (me->IsWithinMeleeRange(victim))
                        {
                            DoCastVictim(11976);
                            strikeDelay = urand(5000, 8000);
                            return;
                        }
                    }
                    if (hamstringDelay < 0)
                    {
                        hamstringDelay = 1000;
                        if (me->IsWithinMeleeRange(victim))
                        {
                            DoCastVictim(31553);
                            hamstringDelay = urand(15000, 20000);
                            return;
                        }
                    }
                }
            }
            else
            {
                if (eventDelay >= 0)
                {
                    eventDelay -= diff;
                }
                if (eventDelay < 0)
                {
                    switch (eventID)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        eventDelay = 2000;
                        if (me->IsWalking())
                        {
                            me->SetWalk(false);
                        }
                        if (Creature* saeed = me->FindNearestCreature(20985, 100.0f))
                        {
                            if (saeed->IsInCombat())
                            {
                                if (Unit* saeedVictim = saeed->GetVictim())
                                {
                                    AttackStart(saeedVictim);
                                }
                            }
                            else
                            {
                                if (!me->isMoving())
                                {
                                    float followDistance = frand(3.0f, 10.0f);
                                    float followAngle = me->GetAngle(saeed->GetPosition());
                                    me->GetMotionMaster()->MoveFollow(saeed, followDistance, followAngle);
                                }
                            }
                        }
                        break;
                    }
                    case 2:
                    {
                        if (Creature* dimensius = me->FindNearestCreature(19554, 50.0f))
                        {
                            AttackStart(dimensius);
                        }
                        eventDelay = 2000;
                        break;
                    }
                    case 3:
                    {
                        me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        eventID = 4;
                        eventDelay = 20000;
                        break;
                    }
                    case 4:
                    {
                        me->DespawnOrUnsummon(urand(5000, 10000));
                        eventDelay = 20000;
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

        int strikeDelay;
        int hamstringDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_protectorate_defenderAI(creature);
    }
};

class npc_protectorate_regenerator : public CreatureScript
{
public:
    npc_protectorate_regenerator() : CreatureScript("npc_protectorate_regenerator") { }

    struct npc_protectorate_regeneratorAI : public ScriptedAI
    {
        npc_protectorate_regeneratorAI(Creature* creature) : ScriptedAI(creature)
        {
            eventID = 0;
            eventDelay = 0;
        }

        void Reset() override
        {

        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->SetHomePosition(me->GetPosition());
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    if (Creature* saeed = me->FindNearestCreature(20985, 50.0f))
                    {
                        me->SetFacingToObject(saeed);
                    }
                }
                else if (data == 2)
                {
                    me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                }
                else if (data == 3)
                {
                    me->SetImmuneToAll(false);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->SetFaction(1807);
                    eventID = 1;
                    eventDelay = 1000;
                }
                else if (data == 4)
                {
                    eventID = 2;
                    eventDelay = 8000;
                }
                else if (data == 5)
                {                    
                    eventID = 3;
                    eventDelay = urand(1000, 5000);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (me->IsNonMeleeSpellCast(false))
                {
                    return;
                }
                if (Unit* victim = me->GetVictim())
                {
                    DoCastVictim(34232);
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
                    switch (eventID)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        eventDelay = 2000;
                        if (me->IsWalking())
                        {
                            me->SetWalk(false);
                        }
                        if (Creature* saeed = me->FindNearestCreature(20985, 100.0f))
                        {
                            if (saeed->IsInCombat())
                            {
                                if (Unit* saeedVictim = saeed->GetVictim())
                                {
                                    AttackStart(saeedVictim);
                                }
                            }
                            else
                            {
                                if (!me->isMoving())
                                {
                                    float followDistance = frand(3.0f, 10.0f);
                                    float followAngle = me->GetAngle(saeed->GetPosition());
                                    me->GetMotionMaster()->MoveFollow(saeed, followDistance, followAngle);
                                }
                            }
                        }
                        break;
                    }
                    case 2:
                    {
                        if (Creature* dimensius = me->FindNearestCreature(19554, 50.0f))
                        {
                            AttackStart(dimensius);
                        }
                        eventDelay = 2000;
                        break;
                    }
                    case 3:
                    {
                        me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        eventID = 4;                        
                        eventDelay = 20000;
                        break;
                    }
                    case 4:
                    {
                        me->DespawnOrUnsummon(urand(5000, 10000));
                        eventDelay = 20000;
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

        int strikeDelay;
        int hamstringDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_protectorate_regeneratorAI(creature);
    }
};

class npc_protectorate_avenger : public CreatureScript
{
public:
    npc_protectorate_avenger() : CreatureScript("npc_protectorate_avenger") { }

    struct npc_protectorate_avengerAI : public ScriptedAI
    {
        npc_protectorate_avengerAI(Creature* creature) : ScriptedAI(creature)
        {
            glaiveDelay = 0;
            eventID = 0;
            eventDelay = 0;
        }

        void Reset() override
        {
            glaiveDelay = 0;
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            me->SetHomePosition(me->GetPosition());
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    if (Creature* saeed = me->FindNearestCreature(20985, 50.0f))
                    {
                        me->SetFacingToObject(saeed);
                    }
                }
                else if (data == 2)
                {
                    me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                }
                else if (data == 3)
                {
                    me->SetImmuneToAll(false);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->SetFaction(1807);
                    eventID = 1;
                    eventDelay = 1000;
                }
                else if (data == 4)
                {
                    eventID = 2;
                    eventDelay = 8000;
                }
                else if (data == 5)
                {
                    eventID = 3;
                    eventDelay = urand(1000, 5000);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (glaiveDelay >= 0)
                {
                    glaiveDelay -= diff;
                }
                if (Unit* victim = me->GetVictim())
                {
                    float victimDistance = me->GetDistance(victim);
                    if (victimDistance > 10.0f && victimDistance < 25.0f)
                    {
                        if (me->isMoving())
                        {
                            me->StopMoving();
                            SetCombatMovement(false);
                        }
                        if (glaiveDelay < 0)
                        {
                            glaiveDelay = 2000;
                            DoCastVictim(36500);
                            return;
                        }
                    }
                    else
                    {
                        SetCombatMovement(true);
                    }
                    DoMeleeAttackIfReady();
                }
            }
            else
            {
                if (eventDelay >= 0)
                {
                    eventDelay -= diff;
                }
                if (eventDelay < 0)
                {
                    switch (eventID)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        eventDelay = 2000;
                        if (me->IsWalking())
                        {
                            me->SetWalk(false);
                        }
                        if (Creature* saeed = me->FindNearestCreature(20985, 100.0f))
                        {
                            if (saeed->IsInCombat())
                            {
                                if (Unit* saeedVictim = saeed->GetVictim())
                                {
                                    AttackStart(saeedVictim);
                                }
                            }
                            else
                            {
                                if (!me->isMoving())
                                {
                                    float followDistance = frand(3.0f, 10.0f);
                                    float followAngle = me->GetAngle(saeed->GetPosition());
                                    me->GetMotionMaster()->MoveFollow(saeed, followDistance, followAngle);
                                }
                            }
                        }
                        break;
                    }
                    case 2:
                    {
                        if (Creature* dimensius = me->FindNearestCreature(19554, 50.0f))
                        {
                            AttackStart(dimensius);
                        }
                        eventDelay = 2000;
                        break;
                    }
                    case 3:
                    {
                        me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        eventID = 4;
                        eventDelay = 20000;
                        break;
                    }
                    case 4:
                    {
                        me->DespawnOrUnsummon(urand(5000, 10000));
                        eventDelay = 20000;
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

        int glaiveDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_protectorate_avengerAI(creature);
    }
};

class npc_captain_saeed : public CreatureScript
{
public:
    npc_captain_saeed() : CreatureScript("npc_captain_saeed") { }

    struct npc_captain_saeedAI : public ScriptedAI
    {
        npc_captain_saeedAI(Creature* creature) : ScriptedAI(creature)
        {
            cleaveDelay = 0;
            eventID = 0;
            eventDelay = 0;
        }

        void Reset() override
        {
            cleaveDelay = 0;
        }

        bool GossipHello(Player* player) override
        {
            player->RewardPlayerAndGroupAtEvent(20985, player);
            return false;
        }

        bool GossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
        {
            if (gossipListId == 0)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetImmuneToAll(false);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->SetFaction(1807);
                eventID = 1;
                eventDelay = 2000;
                CloseGossipMenuFor(player);
            }
            return true;
        }

        void JustEngagedWith(Unit* who) override
        {
            me->SetHomePosition(me->GetPosition());
            if (who->GetEntry() == 19554)
            {
                Talk(1);
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    eventID = 8;
                    eventDelay = 3000;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                if (cleaveDelay >= 0)
                {
                    cleaveDelay -= diff;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (cleaveDelay < 0)
                    {
                        cleaveDelay = 1000;
                        if (me->IsWithinMeleeRange(victim))
                        {
                            DoCastVictim(15496);
                            cleaveDelay = urand(8000, 12000);
                            return;
                        }
                    }
                }
            }
            else
            {
                if (eventDelay>=0)
                {
                    eventDelay -= diff;
                }
                if (eventDelay < 0)
                {
                    switch (eventID)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        Talk(0);
                        std::list<Creature*> defenderList;
                        me->GetCreatureListWithEntryInGrid(defenderList, 20984, 50.0f);
                        for (std::list<Creature*>::iterator itr = defenderList.begin(); itr != defenderList.end(); ++itr)
                        {
                            if (Creature* defender = *itr)
                            {
                                defender->AI()->SetData(1, 1);
                            }
                        }
                        std::list<Creature*> avengerList;
                        me->GetCreatureListWithEntryInGrid(avengerList, 21805, 50.0f);
                        for (std::list<Creature*>::iterator itr = avengerList.begin(); itr != avengerList.end(); ++itr)
                        {
                            if (Creature* avenger = *itr)
                            {
                                avenger->AI()->SetData(1, 1);
                            }
                        }
                        std::list<Creature*> regeneratorList;
                        me->GetCreatureListWithEntryInGrid(regeneratorList, 21783, 50.0f);
                        for (std::list<Creature*>::iterator itr = regeneratorList.begin(); itr != regeneratorList.end(); ++itr)
                        {
                            if (Creature* regenerator = *itr)
                            {
                                regenerator->AI()->SetData(1, 1);
                            }
                        }
                        eventID = 2;
                        eventDelay = 2000;
                        break;
                    }
                    case 2:
                    {
                        //me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        eventID = 3;
                        eventDelay = 1000;
                        break;
                    }
                    case 3:
                    {
                        std::list<Creature*> defenderList;
                        me->GetCreatureListWithEntryInGrid(defenderList, 20984, 50.0f);
                        for (std::list<Creature*>::iterator itr = defenderList.begin(); itr != defenderList.end(); ++itr)
                        {
                            if (Creature* defender = *itr)
                            {
                                defender->AI()->SetData(1, 2);
                            }
                        }
                        std::list<Creature*> avengerList;
                        me->GetCreatureListWithEntryInGrid(avengerList, 21805, 50.0f);
                        for (std::list<Creature*>::iterator itr = avengerList.begin(); itr != avengerList.end(); ++itr)
                        {
                            if (Creature* avenger = *itr)
                            {
                                avenger->AI()->SetData(1, 2);
                            }
                        }
                        std::list<Creature*> regeneratorList;
                        me->GetCreatureListWithEntryInGrid(regeneratorList, 21783, 50.0f);
                        for (std::list<Creature*>::iterator itr = regeneratorList.begin(); itr != regeneratorList.end(); ++itr)
                        {
                            if (Creature* regenerator = *itr)
                            {
                                regenerator->AI()->SetData(1, 2);
                            }
                        }
                        eventID = 4;
                        eventDelay = 2000;
                        break;
                    }
                    case 4:
                    {
                        me->SetWalk(false);
                        me->GetMotionMaster()->MovePath(20985, false);
                        std::list<Creature*> defenderList;
                        me->GetCreatureListWithEntryInGrid(defenderList, 20984, 50.0f);
                        for (std::list<Creature*>::iterator itr = defenderList.begin(); itr != defenderList.end(); ++itr)
                        {
                            if (Creature* defender = *itr)
                            {
                                defender->AI()->SetData(1, 3);
                            }
                        }
                        std::list<Creature*> avengerList;
                        me->GetCreatureListWithEntryInGrid(avengerList, 21805, 50.0f);
                        for (std::list<Creature*>::iterator itr = avengerList.begin(); itr != avengerList.end(); ++itr)
                        {
                            if (Creature* avenger = *itr)
                            {
                                avenger->AI()->SetData(1, 3);
                            }
                        }
                        std::list<Creature*> regeneratorList;
                        me->GetCreatureListWithEntryInGrid(regeneratorList, 21783, 50.0f);
                        for (std::list<Creature*>::iterator itr = regeneratorList.begin(); itr != regeneratorList.end(); ++itr)
                        {
                            if (Creature* regenerator = *itr)
                            {
                                regenerator->AI()->SetData(1, 3);
                            }
                        }
                        eventID = 5;
                        eventDelay = 1000;
                        break;
                    }
                    case 5:
                    {
                        eventDelay = 2000;
                        if (Unit* enemy = me->SelectNearestHostileUnitInAggroRange(true))
                        {
                            AttackStart(enemy);
                            return;
                        }
                        if (!me->isMoving())
                        {
                            if (Creature* dimensius = me->FindNearestCreature(19554, 30.0f))
                            {
                                eventID = 6;
                                eventDelay = 500;
                            }
                        }
                        break;
                    }
                    case 6:
                    {
                        Talk(2);
                        std::list<Creature*> defenderList;
                        me->GetCreatureListWithEntryInGrid(defenderList, 20984, 50.0f);
                        for (std::list<Creature*>::iterator itr = defenderList.begin(); itr != defenderList.end(); ++itr)
                        {
                            if (Creature* defender = *itr)
                            {
                                defender->AI()->SetData(1, 4);
                            }
                        }
                        std::list<Creature*> avengerList;
                        me->GetCreatureListWithEntryInGrid(avengerList, 21805, 50.0f);
                        for (std::list<Creature*>::iterator itr = avengerList.begin(); itr != avengerList.end(); ++itr)
                        {
                            if (Creature* avenger = *itr)
                            {
                                avenger->AI()->SetData(1, 4);
                            }
                        }
                        std::list<Creature*> regeneratorList;
                        me->GetCreatureListWithEntryInGrid(regeneratorList, 21783, 50.0f);
                        for (std::list<Creature*>::iterator itr = regeneratorList.begin(); itr != regeneratorList.end(); ++itr)
                        {
                            if (Creature* regenerator = *itr)
                            {
                                regenerator->AI()->SetData(1, 4);
                            }
                        }
                        if (Creature* dimensius = me->FindNearestCreature(19554, 30.0f))
                        {
                            dimensius->AI()->SetData(1, 1);
                            eventID = 8;
                        }
                        eventID = 7;
                        eventDelay = 8000;
                        break;
                    }
                    case 7:
                    {
                        eventDelay = 2000;
                        if (Creature* dimensius = me->FindNearestCreature(19554, 50.0f))
                        {
                            AttackStart(dimensius);
                        }
                        break;
                    }
                    case 8:
                    {
                        Talk(3);
                        me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ROAR);
                        std::list<Creature*> defenderList;
                        me->GetCreatureListWithEntryInGrid(defenderList, 20984, 50.0f);
                        for (std::list<Creature*>::iterator itr = defenderList.begin(); itr != defenderList.end(); ++itr)
                        {
                            if (Creature* defender = *itr)
                            {
                                defender->AI()->SetData(1, 5);
                            }
                        }
                        std::list<Creature*> avengerList;
                        me->GetCreatureListWithEntryInGrid(avengerList, 21805, 50.0f);
                        for (std::list<Creature*>::iterator itr = avengerList.begin(); itr != avengerList.end(); ++itr)
                        {
                            if (Creature* avenger = *itr)
                            {
                                avenger->AI()->SetData(1, 5);
                            }
                        }
                        std::list<Creature*> regeneratorList;
                        me->GetCreatureListWithEntryInGrid(regeneratorList, 21783, 50.0f);
                        for (std::list<Creature*>::iterator itr = regeneratorList.begin(); itr != regeneratorList.end(); ++itr)
                        {
                            if (Creature* regenerator = *itr)
                            {
                                regenerator->AI()->SetData(1, 5);
                            }
                        }
                        eventID = 9;                        
                        eventDelay = 20000;
                        break;
                    }
                    case 9:
                    {
                        me->DespawnOrUnsummon(urand(5000, 10000));
                        eventDelay = 20000;
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

        int cleaveDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_captain_saeedAI(creature);
    }
};

class npc_dimensius : public CreatureScript
{
public:
    npc_dimensius() : CreatureScript("npc_dimensius") { }

    struct npc_dimensiusAI : public ScriptedAI
    {
        npc_dimensiusAI(Creature* creature) : ScriptedAI(creature)
        {
            vaultDelay = 5000;
            feedDelay = 20000;
            feedingDuration = 0;
            summoned = 0;
            eventID = 0;
            eventDelay = 0;
            summonLimit = 2;
        }

        void Reset() override
        {
            vaultDelay = 5000;
            feedDelay = 20000;
            feedingDuration = 0;
            summoned = 0;
            eventID = 1;
            eventDelay = 5000;
        }

        void JustAppeared()
        {
            eventID = 1;
            eventDelay = 5000;
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* saeed = me->FindNearestCreature(20985, 50.0f))
            {
                saeed->AI()->SetData(1, 1);
            }
            std::list<Creature*> spawnsList;
            me->GetCreatureListWithEntryInGrid(spawnsList, 21780, 50.0f);
            for (std::list<Creature*>::iterator itr = spawnsList.begin(); itr != spawnsList.end(); ++itr)
            {
                if (Creature* spawn = *itr)
                {
                    spawn->DespawnOrUnsummon(500);
                }
            }            
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    eventID = 2;
                    eventDelay = 2000;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (feedDelay >= 0)
                {
                    feedDelay -= diff;
                }
                if (vaultDelay >= 0)
                {
                    vaultDelay -= diff;
                }
                if (me->IsNonMeleeSpellCast(false))
                {
                    if (me->HasAura(37450))
                    {
                        feedingDuration += diff;
                        if (feedingDuration > 5000)
                        {
                            feedingDuration = 0;
                            float summonX = 0.0f, summonY = 0.0f, summonZ = 0.0f, summonDistance = 0.0f, summonAngle = 0.0f;
                            for (uint32 summonCount = 0; summonCount < summonLimit; summonCount++)
                            {
                                summonDistance = frand(5.0f, 10.0f);
                                summonAngle = frand(0.0f, M_PI * 2);
                                me->GetNearPoint(me, summonX, summonY, summonZ, summonDistance, summonAngle);
                                if (TempSummon* ts = me->SummonCreature(21780, summonX, summonY, summonZ, me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                                {
                                    if (Unit* myVictim = me->GetVictim())
                                    {
                                        ts->AI()->AttackStart(myVictim);
                                    }
                                    else if (Unit* enemy = ts->SelectNearestHostileUnitInAggroRange(true))
                                    {
                                        ts->AI()->AttackStart(enemy);
                                    }
                                }
                            }
                            summoned += summonLimit;
                            if (summoned >= 10)
                            {
                                me->InterruptNonMeleeSpells(false);
                                me->RemoveAurasDueToSpell(37450);
                                summoned = 0;
                                feedingDuration = 0;
                                feedDelay = urand(30000, 35000);
                            }
                        }
                    }
                    return;
                }
                if (feedDelay < 0)
                {
                    feedDelay = 1000;
                    DoCastSelf(37450);
                    Talk(2);
                    feedingDuration = 5000;
                    feedDelay = urand(300000, 600000);
                    return;
                }
                feedingDuration = 0;
                if (vaultDelay < 0)
                {
                    vaultDelay = 1000;
                    std::list<Player*> players;
                    me->GetPlayerListInGrid(players, 30.0f);
                    if (players.size() > 0)
                    {
                        std::unordered_map<uint32, Player*> nearbyPlayerMap;
                        for (auto itr = players.begin(); itr != players.end(); itr++)
                        {
                            if (Player* eachPlayer = *itr)
                            {
                                if (eachPlayer->IsAlive())
                                {
                                    nearbyPlayerMap[nearbyPlayerMap.size()] = eachPlayer;
                                }
                            }
                        }
                        uint32 targetIndex = urand(0, nearbyPlayerMap.size() - 1);
                        if (Player* targetPlayer = nearbyPlayerMap[targetIndex])
                        {
                            DoCast(targetPlayer, 37412);
                            vaultDelay = urand(5000, 8000);
                            return;
                        }
                    }
                    return;
                }
                if (Unit* victim = me->GetVictim())
                {
                    DoCastVictim(37500);
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
                    switch (eventID)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        SetCombatMovement(false);
                        DoCastSelf(35939);
                        me->SetImmuneToAll(true);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    }
                    case 2:
                    {
                        me->RemoveAurasDueToSpell(35939);
                        DoCastSelf(43759);
                        eventID = 3;
                        eventDelay = 100;
                        break;
                    }
                    case 3:
                    {
                        DoCastSelf(26638);
                        eventID = 4;
                        eventDelay = 1000;
                        break;
                    }
                    case 4:
                    {
                        Talk(1);
                        eventID = 5;
                        eventDelay = 4000;
                        break;
                    }
                    case 5:
                    {
                        me->SetImmuneToAll(false);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        eventID = 6;
                        eventDelay = 1000;
                        break;
                    }
                    case 6:
                    {
                        if (Unit* enemy = me->SelectNearestHostileUnitInAggroRange(true))
                        {
                            AttackStart(enemy);
                            DoCast(enemy, 37500);
                        }
                        eventDelay = 2000;
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

        int vaultDelay;
        int feedDelay;
        int feedingDuration;
        int summoned;
        int eventID;
        int eventDelay;
        uint32 summonLimit;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_dimensiusAI(creature);
    }
};

class npc_anchorite_karja : public CreatureScript
{
public:
    npc_anchorite_karja() : CreatureScript("npc_anchorite_karja") { }

    struct npc_anchorite_karjaAI : public ScriptedAI
    {
        npc_anchorite_karjaAI(Creature* creature) : ScriptedAI(creature)
        {
            holyDelay = 2000;
            sit = true;
        }

        void Reset() override
        {
            holyDelay = 2000;
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    sit = false;
                }
            }
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
                                    DoCast(eachUnit, 13952);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
            }
            else
            {
                if (sit)
                {
                    if (me->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_SIT_HIGH_CHAIR)
                    {
                        me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_SIT_HIGH_CHAIR);
                    }
                }
            }
        }

        int holyDelay;
        bool sit;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_anchorite_karjaAI(creature);
    }
};

class npc_exarch_orelis : public CreatureScript
{
public:
    npc_exarch_orelis() : CreatureScript("npc_exarch_orelis") { }

    struct npc_exarch_orelisAI : public ScriptedAI
    {
        npc_exarch_orelisAI(Creature* creature) : ScriptedAI(creature)
        {
            shoutDelay = 500;
            rendDelay = urand(2000, 5000);
            strikeDelay = urand(5000, 8000);
        }

        void Reset() override
        {
            shoutDelay = 500;
            rendDelay = urand(2000, 5000);
            strikeDelay = urand(5000, 8000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (shoutDelay >= 0)
                {
                    shoutDelay -= diff;
                }
                if (rendDelay >= 0)
                {
                    rendDelay -= diff;
                }
                if (strikeDelay >= 0)
                {
                    strikeDelay -= diff;
                }
                if (shoutDelay < 0)
                {
                    shoutDelay = urand(5000, 8000);
                    if (!me->HasAura(13730))
                    {
                        DoCastSelf(13730);
                        return;
                    }
                }
                if (rendDelay < 0)
                {
                    rendDelay = urand(5000, 8000);
                    if (Unit* victim = me->GetVictim())
                    {
                        if (!victim->HasAura(16509, me->GetGUID()))
                        {
                            DoCast(victim, 16509);
                        }
                    }
                }
                if (strikeDelay < 0)
                {
                    strikeDelay = urand(5000, 8000);
                    DoCastVictim(29426);
                    return;
                }
                DoMeleeAttackIfReady();
            }
        }

        int shoutDelay;
        int rendDelay;
        int strikeDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_exarch_orelisAI(creature);
    }
};

class npc_kaylaan_the_lost : public CreatureScript
{
public:
    npc_kaylaan_the_lost() : CreatureScript("npc_kaylaan_the_lost") { }

    struct npc_kaylaan_the_lostAI : public ScriptedAI
    {
        npc_kaylaan_the_lostAI(Creature* creature) : ScriptedAI(creature)
        {
            eventID = 0;
            eventDelay = 0;
            burningDelay = urand(5000, 8000);
            slamDelay = urand(2000, 5000);
            holyDelay = urand(10000, 15000);
        }

        void JustAppeared()
        {
            me->SetImmuneToAll(true);
            me->SetVisible(false);
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    eventID = 1;
                    eventDelay = 8000;
                }
            }
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (damage >= me->GetHealth())
            {
                damage = me->GetHealth() - 1;
                me->ClearInCombat();
                me->SetImmuneToAll(true);
                if (Creature* adyen = me->FindNearestCreature(18537, 50.0f))
                {
                    adyen->AI()->SetData(1, 2);
                    adyen->ClearInCombat();
                    adyen->SetImmuneToAll(true);
                }
                if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                {
                    orelis->ClearInCombat();
                    orelis->SetImmuneToAll(true);
                }
                if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                {
                    karja->ClearInCombat();
                    karja->SetImmuneToAll(true);
                }
                if (Creature* socrethar = me->FindNearestCreature(20132, 100.0f))
                {
                    socrethar->AI()->SetData(1, 2);
                }
            }                
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (UpdateVictim())
            {
                if (burningDelay >= 0)
                {
                    burningDelay -= diff;
                }
                if (slamDelay >= 0)
                {
                    slamDelay -= diff;
                }
                if (holyDelay >= 0)
                {
                    holyDelay -= diff;
                }
                if (burningDelay < 0)
                {
                    DoCastVictim(37552);
                    burningDelay = urand(8000, 12000);
                }
                if (slamDelay < 0)
                {
                    DoCastVictim(37572);
                    slamDelay = urand(5000, 8000);
                }
                if (holyDelay < 0)
                {
                    holyDelay = 1000;
                    float myPCT = me->GetHealthPct();
                    if (myPCT < 50.0f)
                    {
                        DoCastSelf(37569);
                        holyDelay = urand(20000, 30000);
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
                        break;
                    }
                    case 1:
                    {
                        Talk(4);
                        eventID = 2;
                        eventDelay = 100;
                        break;
                    }
                    case 2:
                    {
                        Creature* Ishanah = me->FindNearestCreature(18538, 60.0f);
                        if (!Ishanah)
                        {
                            Ishanah = me->SummonCreature(18538, 4899.0f, 3817.0f, 208.35f, 0.6f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 300000);
                            if (Ishanah)
                            {
                                Ishanah->SetFaction(250);
                                Ishanah->SetImmuneToAll(true);                                
                                Ishanah->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                Ishanah->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                                Ishanah->SetWalk(false);
                                Ishanah->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);                                
                            }
                        }
                        if (Ishanah)
                        {
                            if (!Ishanah->isMoving())
                            {
                                float destX = 4941.0f, destY = 3835.0f, destZ = 211.5f;
                                float destinationDistance = Ishanah->GetDistance(destX, destY, destZ);
                                if (destinationDistance > 3.0f)
                                {
                                    Ishanah->SetWalk(false);
                                    Ishanah->GetMotionMaster()->MovePoint(1, destX, destY, destZ);
                                }
                                else
                                {
                                    Ishanah->SetFacingToObject(me);
                                    me->SetFacingToObject(Ishanah);
                                    eventID = 3;
                                    eventDelay = 1000;
                                }
                            }

                        }
                        break;
                    }
                    case 3:
                    {
                        Talk(5);
                        eventID = 4;
                        eventDelay = 2000;
                        break;
                    }
                    case 4:
                    {
                        me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_KNEEL);
                        eventID = 5;
                        eventDelay = 1000;
                        break;
                    }
                    case 5:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                            {                                
                                Ishanah->SetFacingToObject(socrethar);
                                socrethar->SetFacingToObject(Ishanah);
                            }
                            Ishanah->AI()->Talk(0);
                            eventID = 6;
                            eventDelay = 6000;
                        }
                        break;
                    }
                    case 6:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            Ishanah->AI()->Talk(1);
                            eventID = 7;
                            eventDelay = 9000;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->Talk(4);
                            eventID = 8;
                            eventDelay = 7000;
                        }
                        break;
                    }
                    case 8:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                            {
                                socrethar->CastSpell(Ishanah, 35598);
                                eventID = 9;
                                eventDelay = 1500;
                            }
                        }
                        break;
                    }
                    case 9:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            Ishanah->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                            eventID = 10;
                            eventDelay = 2000;
                        }
                        break;
                    }
                    case 10:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            me->RemoveAurasDueToSpell(35596);
                            Talk(6);
                            float saveX = 0.0f, saveY = 0.0f, saveZ = 0.0f;
                            Ishanah->GetNearPoint(Ishanah, saveX, saveY, saveZ, 3.0f, Ishanah->GetAngle(me));
                            me->GetMotionMaster()->MovePoint(1, saveX, saveY, saveZ);
                            eventID = 11;
                            eventDelay = 3000;
                        }
                        break;
                    }
                    case 11:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            me->SetFacingToObject(Ishanah);
                            Talk(7);
                            DoCastSelf(13874);
                            me->SetFaction(250);
                            eventID = 12;
                            eventDelay = 4000;
                        }
                        break;
                    }
                    case 12:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                            me->CastSpell(Ishanah, 35599);
                            eventID = 13;
                            eventDelay = 4000;
                        }
                        break;
                    }
                    case 13:
                    {
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            Ishanah->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                        }
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->Talk(5);
                            eventID = 14;
                            eventDelay = 3000;
                        }
                        break;
                    }
                    case 14:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->CastSpell(me, 35598);                            
                            eventID = 15;
                            eventDelay = 1500;
                        }
                        break;
                    }
                    case 15:
                    {
                        if (Creature* socrethar = me->FindNearestCreature(20132, 50.0f))
                        {
                            socrethar->AI()->SetData(1, 3);
                            me->KillSelf(false);
                            eventID = 16;
                        }
                        break;
                    }
                    case 16:
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

        int burningDelay;
        int slamDelay;
        int holyDelay;
        int eventID;
        int eventDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_kaylaan_the_lostAI(creature);
    }
};

class npc_socrethar : public CreatureScript
{
public:
    npc_socrethar() : CreatureScript("npc_socrethar") { }

    struct npc_socretharAI : public ScriptedAI
    {
        npc_socretharAI(Creature* creature) : ScriptedAI(creature)
        {
            deathblowStatus = 0;
            eventID = 0;
            eventDelay = 0;
            protectionDelay = 1000;
            backDelay = urand(5000, 8000);
            antiDelay = urand(20000, 30000);
            shadowDelay = urand(10000, 20000);
            cleaveDelay = urand(2000, 5000);
        }

        void Reset() override
        {
            protectionDelay = 1000;
            backDelay = urand(5000, 8000);
            antiDelay = urand(20000, 30000);
            shadowDelay = urand(10000, 20000);
            cleaveDelay = urand(2000, 5000);
        }

        void JustAppeared()
        {
            me->SetImmuneToAll(true);
        }

        void JustDied(Unit* /*killer*/) override
        {
            std::list<Player*> players;
            Trinity::AnyPlayerInObjectRangeCheck checker(me, 100.0f);
            Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
            Cell::VisitWorldObjects(me, searcher, 100.0f);
            for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* eachPlayer = *itr)
                {
                    eachPlayer->CastSpell(eachPlayer, 35762);
                }
            }
            if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
            {
                orelis->DespawnOrUnsummon(60000);
            }
            if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
            {
                karja->DespawnOrUnsummon(60000);
            }
            if (Creature* adyen = me->FindNearestCreature(18537, 50.0f))
            {
                adyen->DespawnOrUnsummon(60000);
            }
            if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
            {
                Ishanah->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                Ishanah->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                Ishanah->DespawnOrUnsummon(60000);
            }
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
                if (data == 1)
                {
                    deathblowStatus = 1;
                }
                else if (data == 2)
                {
                    Talk(3);
                    if (Creature* kaylaan = me->FindNearestCreature(20794, 100.0f))
                    {
                        kaylaan->AI()->SetData(1, 1);
                    }
                }
                else if (data == 3)
                {
                    eventID = 1;
                    eventDelay = 1000;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsAlive())
            {
                return;
            }
            if (protectionDelay >= 0)
            {
                protectionDelay -= diff;
            }
            if (protectionDelay < 0)
            {
                DoCastSelf(37539);
                protectionDelay = 600000;
                return;
            }
            if (UpdateVictim())
            {
                if (backDelay >= 0)
                {
                    backDelay -= diff;
                }
                if (antiDelay >= 0)
                {
                    antiDelay -= diff;
                }
                if (shadowDelay >= 0)
                {
                    shadowDelay -= diff;
                }
                if (cleaveDelay >= 0)
                {
                    cleaveDelay -= diff;
                }
                if (backDelay < 0)
                {
                    DoCastVictim(37537);
                    backDelay = urand(12000, 15000);
                    return;
                }
                if (antiDelay < 0)
                {
                    DoCastSelf(37538);
                    antiDelay = urand(20000, 30000);
                    return;
                }
                if (shadowDelay < 0)
                {
                    DoCastVictim(28448);
                    shadowDelay = urand(10000, 20000);
                    return;
                }
                if (cleaveDelay < 0)
                {
                    DoCastVictim(15496);
                    cleaveDelay = urand(8000, 12000);
                    return;
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
                        break;
                    }
                    case 1:
                    {
                        me->SetImmuneToAll(false);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->SetFaction(14);
                        if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            Ishanah->SetImmuneToAll(false);              
                            Ishanah->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            Ishanah->SetHomePosition(Ishanah->GetPosition());
                        }
                        if (Creature* adyen = me->FindNearestCreature(18537, 50.0f))
                        {
                            adyen->SetImmuneToAll(false);
                            adyen->SetHomePosition(adyen->GetPosition());
                        }
                        if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                        {
                            orelis->SetImmuneToAll(false);
                            orelis->SetHomePosition(orelis->GetPosition());
                        }
                        if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                        {
                            karja->SetImmuneToAll(false);
                            karja->SetHomePosition(karja->GetPosition());
                        }
                        eventID = 2;
                        eventDelay = 100;
                        break;
                    }
                    case 2:
                    {
                        if (Unit* enemy = me->SelectNearestHostileUnitInAggroRange(true))
                        {
                            AttackStart(enemy);                            
                        }
                        else if (Creature* Ishanah = me->FindNearestCreature(18538, 50.0f))
                        {
                            AttackStart(Ishanah);
                            Ishanah->AI()->AttackStart(me);
                        }
                        if (Creature* adyen = me->FindNearestCreature(18537, 50.0f))
                        {
                            adyen->AI()->AttackStart(me);
                        }
                        if (Creature* orelis = me->FindNearestCreature(19466, 50.0f))
                        {
                            orelis->AI()->AttackStart(me);
                        }
                        if (Creature* karja = me->FindNearestCreature(19467, 50.0f))
                        {
                            karja->AI()->AttackStart(me);
                        }
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
        int eventID;
        int eventDelay;
        int protectionDelay;
        int backDelay;
        int antiDelay;
        int shadowDelay;
        int cleaveDelay;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_socretharAI(creature);
    }
};

void AddSC_netherstorm()
{
    new npc_commander_dawnforge();
    new at_commander_dawnforge();
    new npc_phase_hunter();
    new npc_bessy();
    new npc_maxx_a_million_escort();
    new go_captain_tyralius_prison();

    // lfm scripts
    new npc_scrapped_fel_reaver();
    new npc_scrap_reaver_x6000();
    new npc_negatron();
    new npc_captain_saeed();
    new npc_protectorate_defender();
    new npc_protectorate_regenerator();
    new npc_protectorate_avenger();
    new npc_dimensius();
    new npc_anchorite_karja();
    new npc_kaylaan_the_lost();
    new npc_socrethar();
    new npc_exarch_orelis();
}
