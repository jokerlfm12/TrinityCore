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
SDName: Nagrand
SD%Complete: 90
SDComment: Quest support: 9868, 9874, 10085. TextId's unknown for altruis_the_sufferer and greatmother_geyah (npc_text)
SDCategory: Nagrand
EndScriptData */

/* ContentData
npc_maghar_captive
npc_creditmarker_visit_with_ancestors
EndContentData */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"
#include "TemporarySummon.h"

/*######
## npc_maghar_captive
#####*/

enum MagharCaptive
{
    SAY_MAG_START               = 0,
    SAY_MAG_NO_ESCAPE           = 0,
    SAY_MAG_MORE                = 1,
    SAY_MAG_MORE_REPLY          = 0,
    SAY_MAG_LIGHTNING           = 2,
    SAY_MAG_SHOCK               = 3,
    SAY_MAG_COMPLETE            = 4,

    SPELL_CHAIN_LIGHTNING       = 16006,
    SPELL_EARTHBIND_TOTEM       = 15786,
    SPELL_FROST_SHOCK           = 12548,
    SPELL_HEALING_WAVE          = 12491,

    QUEST_TOTEM_KARDASH_H       = 9868,

    NPC_MURK_RAIDER             = 18203,
    NPC_MURK_BRUTE              = 18211,
    NPC_MURK_SCAVENGER          = 18207,
    NPC_MURK_PUTRIFIER          = 18202
};

static float m_afAmbushA[]= {-1568.805786f, 8533.873047f, 1.958f};
static float m_afAmbushB[]= {-1491.554321f, 8506.483398f, 1.248f};

class npc_maghar_captive : public CreatureScript
{
public:
    npc_maghar_captive() : CreatureScript("npc_maghar_captive") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_maghar_captiveAI(creature);
    }

    struct npc_maghar_captiveAI : public EscortAI
    {
        npc_maghar_captiveAI(Creature* creature) : EscortAI(creature) { Reset(); }

        uint32 ChainLightningTimer;
        uint32 HealTimer;
        uint32 FrostShockTimer;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_TOTEM_KARDASH_H)
            {
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetFaction(FACTION_ESCORTEE_H_NEUTRAL_ACTIVE);
                Start(true, false, player->GetGUID(), quest);
                Talk(SAY_MAG_START);

                me->SummonCreature(NPC_MURK_RAIDER, m_afAmbushA[0] + 2.5f, m_afAmbushA[1] - 2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushA[0] - 2.5f, m_afAmbushA[1] + 2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(NPC_MURK_BRUTE, m_afAmbushA[0], m_afAmbushA[1], m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
            }
        }

        void Reset() override
        {
            ChainLightningTimer = 1000;
            HealTimer = 0;
            FrostShockTimer = 6000;
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            DoCast(me, SPELL_EARTHBIND_TOTEM, false);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (!HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            if (Player* player = GetPlayerForEscort())
            {
                if (player->GetQuestStatus(QUEST_TOTEM_KARDASH_H) != QUEST_STATUS_COMPLETE)
                    player->FailQuest(QUEST_TOTEM_KARDASH_H);
            }
        }

        void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
        {
            switch (waypointId)
            {
                case 7:
                    Talk(SAY_MAG_MORE);

                    if (Creature* temp = me->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0], m_afAmbushB[1], m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                        temp->AI()->Talk(SAY_MAG_MORE_REPLY);

                    me->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0]-2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]+2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 16:
                    Talk(SAY_MAG_COMPLETE);

                    if (Player* player = GetPlayerForEscort())
                        player->GroupEventHappens(QUEST_TOTEM_KARDASH_H, me);

                    SetRun();
                    break;
            }
        }

        void JustSummoned(Creature* summoned) override
        {
            if (summoned->GetEntry() == NPC_MURK_BRUTE)
                summoned->AI()->Talk(SAY_MAG_NO_ESCAPE);

            if (summoned->IsTotem())
                return;

            summoned->SetWalk(false);
            summoned->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
            summoned->AI()->AttackStart(me);

        }

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_CHAIN_LIGHTNING)
            {
                if (rand32() % 10)
                    return;

                Talk(SAY_MAG_LIGHTNING);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            EscortAI::UpdateAI(diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (ChainLightningTimer <= diff)
            {
                DoCastVictim(SPELL_CHAIN_LIGHTNING);
                ChainLightningTimer = urand(7000, 14000);
            }
            else
                ChainLightningTimer -= diff;

            if (HealthBelowPct(30))
            {
                if (HealTimer <= diff)
                {
                    DoCast(me, SPELL_HEALING_WAVE);
                    HealTimer = 5000;
                }
                else
                    HealTimer -= diff;
            }

            if (FrostShockTimer <= diff)
            {
                DoCastVictim(SPELL_FROST_SHOCK);
                FrostShockTimer = urand(7500, 15000);
            }
            else
                FrostShockTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_creditmarker_visist_with_ancestors
######*/

class npc_creditmarker_visit_with_ancestors : public CreatureScript
{
public:
    npc_creditmarker_visit_with_ancestors() : CreatureScript("npc_creditmarker_visit_with_ancestors") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_creditmarker_visit_with_ancestorsAI(creature);
    }

    struct npc_creditmarker_visit_with_ancestorsAI : public ScriptedAI
    {
        npc_creditmarker_visit_with_ancestorsAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }

        void JustEngagedWith(Unit* /*who*/) override { }

        void MoveInLineOfSight(Unit* who) override

        {
            if (!who)
                return;

            Player* player = who->ToPlayer();
            if (player && player->GetQuestStatus(10085) == QUEST_STATUS_INCOMPLETE)
            {
                uint32 creditMarkerId = me->GetEntry();
                if (creditMarkerId >= 18840 && creditMarkerId <= 18843)
                {
                    // 18840: Sunspring, 18841: Laughing, 18842: Garadar, 18843: Bleeding
                    if (!player->GetReqKillOrCastCurrentCount(10085, creditMarkerId))
                        player->KilledMonsterCredit(creditMarkerId, me->GetGUID());
                }
            }
        }
    };
};

/*######
## go_corkis_prison and npc_corki
######*/

enum CorkiData
{
  // first quest
  QUEST_HELP                                    = 9923,
  NPC_CORKI                                     = 18445,
  NPC_CORKI_CREDIT_1                            = 18369,
  GO_CORKIS_PRISON                              = 182349,
  CORKI_SAY_THANKS                              = 0,
  // 2nd quest
  QUEST_CORKIS_GONE_MISSING_AGAIN               = 9924,
  NPC_CORKI_2                                   = 20812,
  GO_CORKIS_PRISON_2                            = 182350,
  CORKI_SAY_PROMISE                             = 0,
  // 3rd quest
  QUEST_CHOWAR_THE_PILLAGER                     = 9955,
  NPC_CORKI_3                                   = 18369,
  NPC_CORKI_CREDIT_3                            = 18444,
  GO_CORKIS_PRISON_3                            = 182521,
  CORKI_SAY_LAST                                = 0
};

class go_corkis_prison : public GameObjectScript
{
    public:
      go_corkis_prison() : GameObjectScript("go_corkis_prison") { }

      struct go_corkis_prisonAI : public GameObjectAI
      {
          go_corkis_prisonAI(GameObject* go) : GameObjectAI(go) { }

          bool GossipHello(Player* player) override
          {
              me->SetGoState(GO_STATE_READY);
              if (me->GetEntry() == GO_CORKIS_PRISON)
              {
                  if (Creature* corki = me->FindNearestCreature(NPC_CORKI, 25, true))
                  {
                      corki->AI()->SetData(1, player->GetGUID().GetCounter());
                      corki->GetMotionMaster()->MovePoint(1, me->GetPositionX() + 5, me->GetPositionY(), me->GetPositionZ());
                      if (player)
                      {
                          player->KilledMonsterCredit(NPC_CORKI_CREDIT_1);
                      }                          
                  }
              }

              if (me->GetEntry() == GO_CORKIS_PRISON_2)
              {
                  if (Creature* corki = me->FindNearestCreature(NPC_CORKI_2, 25, true))
                  {
                      corki->AI()->SetData(1, player->GetGUID().GetCounter());
                      corki->GetMotionMaster()->MovePoint(1, me->GetPositionX() - 5, me->GetPositionY(), me->GetPositionZ());
                      if (player)
                      {
                          player->KilledMonsterCredit(NPC_CORKI_2);
                      }                          
                  }
              }

              if (me->GetEntry() == GO_CORKIS_PRISON_3)
              {
                  if (Creature* corki = me->FindNearestCreature(NPC_CORKI_3, 25, true))
                  {
                      corki->AI()->SetData(1, player->GetGUID().GetCounter());
                      corki->GetMotionMaster()->MovePoint(1, me->GetPositionX() + 4, me->GetPositionY(), me->GetPositionZ());
                      if (player)
                      {
                          player->KilledMonsterCredit(NPC_CORKI_CREDIT_3);
                      }                          
                  }
              }
              return true;
          }
      };

      GameObjectAI* GetAI(GameObject* go) const override
      {
          return new go_corkis_prisonAI(go);
      }
};

class npc_corki : public CreatureScript
{
public:
  npc_corki() : CreatureScript("npc_corki") { }

  CreatureAI* GetAI(Creature* creature) const override
  {
      return new npc_corkiAI(creature);
  }

  struct npc_corkiAI : public ScriptedAI
  {
      npc_corkiAI(Creature* creature) : ScriptedAI(creature)
      {
          Initialize();
      }

      void Initialize()
      {
          Say_Timer = 5000;
          ReleasedFromCage = false;
          ogBenefactor.Clear();
      }

      uint32 Say_Timer;
      bool ReleasedFromCage;

      void Reset() override
      {
          Initialize();
      }

      void SetData(uint32 type, uint32 data) override
      {
          if (type == 1)
          {
              ogBenefactor = ObjectGuid(HighGuid::Player, data);
          }
      }

      void UpdateAI(uint32 diff) override
      {
          if (ReleasedFromCage)
          {
              if (Say_Timer <= diff)
              {
                  me->DespawnOrUnsummon();
                  ReleasedFromCage = false;
              }
              else
                  Say_Timer -= diff;
          }
      }

      void MovementInform(uint32 type, uint32 id) override
      {
          if (type == POINT_MOTION_TYPE && id == 1)
          {
              Say_Timer = 5000;
              ReleasedFromCage = true;
              if (!ogBenefactor.IsEmpty())
              {
                  if (Player* benefactor = ObjectAccessor::GetPlayer(*me, ogBenefactor))
                  {
                      if (me->GetEntry() == NPC_CORKI)
                      {
                          Talk(CORKI_SAY_THANKS, benefactor);
                      }                          
                      if (me->GetEntry() == NPC_CORKI_2)
                      {
                          Talk(CORKI_SAY_PROMISE, benefactor);
                      }                          
                      if (me->GetEntry() == NPC_CORKI_3)
                      {
                          Talk(CORKI_SAY_LAST, benefactor);
                      }                          
                  }
              }
          }
      };

      ObjectGuid ogBenefactor;
  };
};

/*#####
## npc_kurenai_captive
#####*/

enum KurenaiCaptive
{
    SAY_KUR_START                   = 0,
    SAY_KUR_NO_ESCAPE               = 1,
    SAY_KUR_MORE                    = 2,
    SAY_KUR_MORE_TWO                = 3,
    SAY_KUR_LIGHTNING               = 4,
    SAY_KUR_SHOCK                   = 5,
    SAY_KUR_COMPLETE                = 6,

    SPELL_KUR_CHAIN_LIGHTNING       = 16006,
    SPELL_KUR_EARTHBIND_TOTEM       = 15786,
    SPELL_KUR_FROST_SHOCK           = 12548,
    SPELL_KUR_HEALING_WAVE          = 12491,

    QUEST_TOTEM_KARDASH_A           = 9879,

    NPC_KUR_MURK_RAIDER             = 18203,
    NPC_KUR_MURK_BRUTE              = 18211,
    NPC_KUR_MURK_SCAVENGER          = 18207,
    NPC_KUR_MURK_PUTRIFIER          = 18202,
};

static float kurenaiAmbushA[]= {-1568.805786f, 8533.873047f, 1.958f};
static float kurenaiAmbushB[]= {-1491.554321f, 8506.483398f, 1.248f};

class npc_kurenai_captive : public CreatureScript
{
public:
    npc_kurenai_captive() : CreatureScript("npc_kurenai_captive") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_kurenai_captiveAI(creature);
    }

    struct npc_kurenai_captiveAI : public EscortAI
    {
        npc_kurenai_captiveAI(Creature* creature) : EscortAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            ChainLightningTimer = 1000;
            HealTimer = 0;
            FrostShockTimer = 6000;
        }

        uint32 ChainLightningTimer;
        uint32 HealTimer;
        uint32 FrostShockTimer;

        void QuestAccept(Player* player, const Quest* quest) override
        {
            if (quest->GetQuestId() == QUEST_TOTEM_KARDASH_A)
            {
                me->SetStandState(UNIT_STAND_STATE_STAND);
                me->SetFaction(FACTION_ESCORTEE_A_NEUTRAL_ACTIVE);
                Start(true, false, player->GetGUID(), quest);
                Talk(SAY_KUR_START);

                me->SummonCreature(NPC_KUR_MURK_RAIDER, kurenaiAmbushA[0] + 2.5f, kurenaiAmbushA[1] - 2.5f, kurenaiAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(NPC_KUR_MURK_BRUTE, kurenaiAmbushA[0] - 2.5f, kurenaiAmbushA[1] + 2.5f, kurenaiAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                me->SummonCreature(NPC_KUR_MURK_SCAVENGER, kurenaiAmbushA[0], kurenaiAmbushA[1], kurenaiAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
            }
        }

        void Reset() override
        {
            Initialize();
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            DoCast(me, SPELL_KUR_EARTHBIND_TOTEM, false);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (!HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            if (Player* player = GetPlayerForEscort())
            {
                if (player->GetQuestStatus(QUEST_TOTEM_KARDASH_A) != QUEST_STATUS_COMPLETE)
                    player->FailQuest(QUEST_TOTEM_KARDASH_A);
            }
        }

        void WaypointReached(uint32 waypointId, uint32 /*pathId*/) override
        {
            switch (waypointId)
            {
                case 3:
                {
                    Talk(SAY_KUR_MORE);

                    if (me->SummonCreature(NPC_KUR_MURK_PUTRIFIER, kurenaiAmbushB[0], kurenaiAmbushB[1], kurenaiAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                        Talk(SAY_KUR_MORE_TWO);

                    me->SummonCreature(NPC_KUR_MURK_PUTRIFIER, kurenaiAmbushB[0]-2.5f, kurenaiAmbushB[1]-2.5f, kurenaiAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_KUR_MURK_SCAVENGER, kurenaiAmbushB[0]+2.5f, kurenaiAmbushB[1]+2.5f, kurenaiAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_KUR_MURK_SCAVENGER, kurenaiAmbushB[0]+2.5f, kurenaiAmbushB[1]-2.5f, kurenaiAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                }
                case 7:
                {
                    Talk(SAY_KUR_COMPLETE);

                    if (Player* player = GetPlayerForEscort())
                        player->GroupEventHappens(QUEST_TOTEM_KARDASH_A, me);

                    SetRun();
                    break;
                }
            }
        }

        void JustSummoned(Creature* summoned) override
        {
            if (summoned->GetEntry() == NPC_KUR_MURK_BRUTE)
                Talk(SAY_KUR_NO_ESCAPE);

            // This function is for when we summoned enemies to fight - so that does NOT mean we should make our totem count in this!
            if (summoned->IsTotem())
                return;

            summoned->SetWalk(false);
            summoned->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
            summoned->AI()->AttackStart(me);
        }

        void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) override
        {
            if (spell->Id == SPELL_KUR_CHAIN_LIGHTNING)
            {
                if (rand32() % 30)
                    return;

                Talk(SAY_KUR_LIGHTNING);
            }

            if (spell->Id == SPELL_KUR_FROST_SHOCK)
            {
                if (rand32() % 30)
                    return;

                Talk(SAY_KUR_SHOCK);
            }
        }

        void UpdateAI(uint32 diff) override
        {
            EscortAI::UpdateAI(diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (ChainLightningTimer <= diff)
            {
                DoCastVictim(SPELL_KUR_CHAIN_LIGHTNING);
                ChainLightningTimer = urand(7000, 14000);
            } else ChainLightningTimer -= diff;

            if (HealthBelowPct(30))
            {
                if (HealTimer <= diff)
                {
                    DoCast(me, SPELL_KUR_HEALING_WAVE);
                    HealTimer = 5000;
                } else HealTimer -= diff;
            }

            if (FrostShockTimer <= diff)
            {
                DoCastVictim(SPELL_KUR_FROST_SHOCK);
                FrostShockTimer = urand(7500, 15000);
            } else FrostShockTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## go_warmaul_prison
######*/

enum FindingTheSurvivorsData
{
    QUEST_FINDING_THE_SURVIVORS                     = 9948,
    NPC_MAGHAR_PRISONER                             = 18428,

    SAY_FREE                                        = 0,
};

class go_warmaul_prison : public GameObjectScript
{
    public:
        go_warmaul_prison() : GameObjectScript("go_warmaul_prison") { }

        struct go_warmaul_prisonAI : public GameObjectAI
        {
            go_warmaul_prisonAI(GameObject* go) : GameObjectAI(go) { }

            bool GossipHello(Player* player) override
            {
                me->UseDoorOrButton();
                if (player->GetQuestStatus(QUEST_FINDING_THE_SURVIVORS) != QUEST_STATUS_INCOMPLETE)
                    return false;

                if (Creature* prisoner = me->FindNearestCreature(NPC_MAGHAR_PRISONER, 5.0f))
                {
                    player->KilledMonsterCredit(NPC_MAGHAR_PRISONER);

                    prisoner->AI()->Talk(SAY_FREE, player);
                    prisoner->DespawnOrUnsummon(6000);
                }
                return true;
            }
        };

        GameObjectAI* GetAI(GameObject* go) const override
        {
            return new go_warmaul_prisonAI(go);
        }
};

// lfm scripts 
class npc_gankly_rottenfist : public CreatureScript
{
public:
    npc_gankly_rottenfist() : CreatureScript("npc_gankly_rottenfist") { }

    struct npc_gankly_rottenfistAI : public ScriptedAI
    {
        npc_gankly_rottenfistAI(Creature* creature) : ScriptedAI(creature)
        {
            destination.m_positionX = -1451.46f;
            destination.m_positionY = 6352.27f;
            destination.m_positionZ = 37.2739f;
            poisonDelay = urand(5000, 10000);
            shadowDelay = urand(10000, 15000);
            sinisterDelay = urand(3000, 6000);
            stepDelay = 2000;
            stepIndex = 0;
        }

        void IsSummonedBy(Unit* /*summoner*/)
        {
            DoCastSelf(32199);
            //me->GetMotionMaster()->MovePath(6018297, false);
            me->SetImmuneToAll(true);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetWalk(false);
            stepIndex = 1;
            stepDelay = 7000;
        }

        void JustEngagedWith(Unit* who) override
        {
            if (who->GetEntry() == 18294)
            {
                Talk(0);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            std::list<Creature*> kristenList;
            me->GetCreatureListWithEntryInGrid(kristenList, 18294, 20.0f);
            for (Creature* kristen : kristenList)
            {
                if (kristen->IsAlive())
                {
                    kristen->AI()->SetData(1, 2);
                }
                break;
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    me->SetHomePosition(me->GetPosition());
                    me->SetImmuneToPC(true);
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    stepIndex = 2;
                    stepDelay = 2000;
                }
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE && id == 0)
            {
                stepIndex = 4;
                stepDelay = 500;
            }
        };

        void UpdateAI(uint32 diff) override
        {
            if (UpdateVictim())
            {
                if (poisonDelay >= 0)
                {
                    poisonDelay -= diff;
                }
                if (shadowDelay >= 0)
                {
                    shadowDelay -= diff;
                }
                if (sinisterDelay >= 0)
                {
                    sinisterDelay -= diff;
                }
                if (poisonDelay < 0)
                {
                    poisonDelay = urand(5000, 10000);
                    DoCastVictim(30981);
                }
                if (shadowDelay < 0)
                {
                    shadowDelay = urand(10000, 15000);
                    DoCastVictim(41176);
                }
                if (sinisterDelay < 0)
                {
                    sinisterDelay = urand(3000, 6000);
                    DoCastVictim(15581);
                }
                DoMeleeAttackIfReady();
            }
            if (stepDelay >= 0)
            {
                stepDelay -= diff;
            }
            if (stepDelay < 0)
            {
                stepDelay = 1000;
                switch (stepIndex)
                {
                case 0:
                {
                    break;
                }
                case 1:
                {
                    std::list<Creature*> kristenList;
                    me->GetCreatureListWithEntryInGrid(kristenList, 18294, 20.0f);
                    for (Creature* kristen : kristenList)
                    {
                        if (kristen->IsAlive())
                        {
                            me->SetImmuneToAll(false);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            kristen->SetImmuneToAll(false);
                            AttackStart(kristen);
                            stepDelay = 5000;
                        }
                        break;
                    }
                    break;
                }
                case 2:
                {
                    Talk(1);
                    stepIndex = 3;
                    stepDelay = 3000;
                    break;
                }
                case 3:
                {
                    if (!me->isMoving())
                    {
                        me->SetWalk(true);
                        me->GetMotionMaster()->MovePoint(0, destination);
                    }
                    break;
                }
                case 4:
                {
                    std::list<Creature*> haroldList;
                    me->GetCreatureListWithEntryInGrid(haroldList, 18218, 20.0f);
                    for (Creature* harold : haroldList)
                    {
                        me->SetFacingToObject(harold);
                        stepIndex = 5;
                        stepDelay = 500;
                        break;
                    }
                    break;
                }
                case 5:
                {
                    Talk(2);
                    stepIndex = 6;
                    stepDelay = 5000;
                    break;
                }
                case 6:
                {
                    std::list<Creature*> haroldList;
                    me->GetCreatureListWithEntryInGrid(haroldList, 18218, 20.0f);
                    for (Creature* harold : haroldList)
                    {
                        harold->AI()->SetData(1, 3);
                        stepIndex = 7;
                        stepDelay = 5000;
                        break;
                    }
                    break;
                }
                case 7:
                {
                    std::list<Creature*> fitzList;
                    me->GetCreatureListWithEntryInGrid(fitzList, 18200, 30.0f);
                    for (Creature* fitz : fitzList)
                    {
                        fitz->AI()->SetData(1, 3);
                        stepIndex = 8;
                        stepDelay = 200;
                        break;
                    }
                    break;
                }
                case 8:
                {
                    std::list<Creature*> hemetList;
                    me->GetCreatureListWithEntryInGrid(hemetList, 18180, 30.0f);
                    for (Creature* hemet : hemetList)
                    {
                        hemet->AI()->SetData(1, 3);
                        stepIndex = 9;
                        stepDelay = 300;
                        break;
                    }
                    break;
                }
                case 9:
                {
                    me->KillSelf();
                    stepIndex = 10;
                    stepDelay = 500;
                    break;
                }
                case 10:
                {
                    stepDelay = 60000;
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }

        int poisonDelay;
        int shadowDelay;
        int sinisterDelay;
        int stepDelay;
        int stepIndex;
        Position destination;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_gankly_rottenfistAI(creature);
    }
};

void AddSC_nagrand()
{
    new npc_maghar_captive();
    new npc_creditmarker_visit_with_ancestors();
    new npc_corki();
    new go_corkis_prison();
    new npc_kurenai_captive();
    new go_warmaul_prison();

    // lfm scripts
    new npc_gankly_rottenfist();
}
