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
}
