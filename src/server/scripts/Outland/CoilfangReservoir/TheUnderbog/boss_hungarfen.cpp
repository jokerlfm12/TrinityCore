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
SDName: Boss_Hungarfen
SD%Complete: 95
SDComment: Need confirmation if spell data are same in both modes. Summons should have faster rate in heroic
SDCategory: Coilfang Resevoir, Underbog
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_underbog.h"

// lfm scripts
#include "GridNotifiers.h"

enum Spells
{
    SPELL_FOUL_SPORES           = 31673,
    SPELL_ACID_GEYSER           = 38739,

    SPELL_SPORE_CLOUD           = 34168,
    SPELL_PUTRID_MUSHROOM       = 31690,
    SPELL_GROW                  = 31698
};

class boss_hungarfen : public CreatureScript
{
public:
    boss_hungarfen() : CreatureScript("boss_hungarfen") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetTheUnderbogAI<boss_hungarfenAI>(creature);
    }

    struct boss_hungarfenAI : public ScriptedAI
    {
        boss_hungarfenAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            Root = false;
            Mushroom_Timer = 5000;                              // 1 mushroom after 5s, then one per 10s. This should be different in heroic mode
            AcidGeyser_Timer = 10000;
        }

        bool Root;
        uint32 Mushroom_Timer;
        uint32 AcidGeyser_Timer;

        void Reset() override
        {
            Initialize();
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (!HealthAbovePct(20))
            {
                if (!Root)
                {
                    DoCast(me, SPELL_FOUL_SPORES);
                    Root = true;
                }
            }

            if (Mushroom_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    me->SummonCreature(17990, target->GetPositionX() + (rand32() % 8), target->GetPositionY() + (rand32() % 8), target->GetPositionZ(), float(rand32() % 5), TEMPSUMMON_TIMED_DESPAWN, 22000);
                else
                    me->SummonCreature(17990, me->GetPositionX() + (rand32() % 8), me->GetPositionY() + (rand32() % 8), me->GetPositionZ(), float(rand32() % 5), TEMPSUMMON_TIMED_DESPAWN, 22000);

                Mushroom_Timer = 10000;
            } else Mushroom_Timer -= diff;

            if (AcidGeyser_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_ACID_GEYSER);
                AcidGeyser_Timer = 10000 + rand32() % 7500;
            } else AcidGeyser_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

class npc_underbog_mushroom : public CreatureScript
{
public:
    npc_underbog_mushroom() : CreatureScript("npc_underbog_mushroom") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetTheUnderbogAI<npc_underbog_mushroomAI>(creature);
    }

    struct npc_underbog_mushroomAI : public ScriptedAI
    {
        npc_underbog_mushroomAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            Stop = false;
            Grow_Timer = 0;
            Shrink_Timer = 20000;
        }

        bool Stop;
        uint32 Grow_Timer;
        uint32 Shrink_Timer;

        void Reset() override
        {
            Initialize();

            DoCast(me, SPELL_PUTRID_MUSHROOM, true);
            DoCast(me, SPELL_SPORE_CLOUD, true);
        }

        void MoveInLineOfSight(Unit* /*who*/) override { }


        void AttackStart(Unit* /*who*/) override { }

        void JustEngagedWith(Unit* /*who*/) override { }

        void UpdateAI(uint32 diff) override
        {
            if (Stop)
                return;

            if (Grow_Timer <= diff)
            {
                DoCast(me, SPELL_GROW);
                Grow_Timer = 3000;
            } else Grow_Timer -= diff;

            if (Shrink_Timer <= diff)
            {
                me->RemoveAurasDueToSpell(SPELL_GROW);
                Stop = true;
            } else Shrink_Timer -= diff;
        }
    };

};

void AddSC_boss_hungarfen()
{
    new boss_hungarfen();
    new npc_underbog_mushroom();
}

// lfm muselek scripts 
class boss_swamplord_muselek : public CreatureScript
{
public:
    boss_swamplord_muselek() : CreatureScript("boss_swamplord_muselek") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetTheUnderbogAI<boss_swamplord_muselekAI>(creature);
    }

    struct boss_swamplord_muselekAI : public ScriptedAI
    {
        boss_swamplord_muselekAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Initialize()
        {
            talkDelay = urand(5000, 10000);
            shootDelay = 0;
            trapDelay = 5000;
            multiDelay = 500;
            setDelay = 0;
            moveDelay = 3000000;
            markDelay = 100;
            knockDelay = 500;
            commandDelay = 5000;
            passiveDelay = 10000;
            aimDelay = 0;
            me->SetWalk(true);
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            ogAimTarget.Clear();
            if (!IsCombatMovementAllowed())
            {
                SetCombatMovement(true);
            }
        }

        void Reset() override
        {
            Initialize();
        }

        void JustEngagedWith(Unit* who) override
        {
            Talk(0);
            me->SetWalk(false);
            if (Creature* claw = me->FindNearestCreature(17827, 100.0f, true))
            {
                claw->AI()->SetData(1, 2);
                claw->AI()->AttackStart(who);
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(3);
            if (Creature* claw = me->FindNearestCreature(17827, 100.0f, true))
            {
                claw->AI()->SetData(1, 1);
                claw->AI()->SetData(1, 4);
            }
        }

        void KilledUnit(Unit* /*victim*/) override
        {
            Talk(2);
        }

        void UpdateAI(uint32 diff) override
        {
            if (UpdateVictim())
            {
                if (me->IsWalking())
                {
                    me->SetWalk(false);
                }
                if (talkDelay >= 0)
                {
                    talkDelay -= diff;
                }
                if (shootDelay >= 0)
                {
                    shootDelay -= diff;
                }
                if (trapDelay >= 0)
                {
                    trapDelay -= diff;
                }
                if (multiDelay >= 0)
                {
                    multiDelay -= diff;
                }
                if (moveDelay >= 0)
                {
                    moveDelay -= diff;
                }
                if (markDelay >= 0)
                {
                    markDelay -= diff;
                }
                if (knockDelay >= 0)
                {
                    knockDelay -= diff;
                }
                if (commandDelay >= 0)
                {
                    commandDelay -= diff;
                }
                if (talkDelay < 0)
                {
                    talkDelay = urand(5000, 10000);
                    Talk(1);
                }
                if (setDelay > 0)
                {
                    setDelay -= diff;
                    if (setDelay <= 0)
                    {
                        if (!ogAimTarget.IsEmpty())
                        {
                            if (Player* targetPlayer = ObjectAccessor::GetPlayer(*me, ogAimTarget))
                            {
                                float destX = 0.0f, destY = 0.0f, destZ = 0.0f;
                                targetPlayer->GetNearPoint(targetPlayer, destX, destY, destZ, 12.0f, targetPlayer->GetAngle(me->GetPosition()));                                
                                me->GetMotionMaster()->MovePoint(0, destX, destY, destZ);
                                moveDelay = 2000;
                            }
                        }
                    }
                }
                if (moveDelay < 0)
                {
                    moveDelay = 3000000;
                    if (!ogAimTarget.IsEmpty())
                    {
                        if (Player* targetPlayer = ObjectAccessor::GetPlayer(*me, ogAimTarget))
                        {
                            me->SetFacingToObject(targetPlayer);
                            DoCast(targetPlayer, 48871);
                            aimDelay = 6000;
                        }
                    }
                    return;
                }
                if (aimDelay > 0)
                {
                    aimDelay -= diff;
                    if (aimDelay <= 0)
                    {
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        Creature* claw = me->FindNearestCreature(17827, 100.0f, true);
                        if (claw)
                        {
                            claw->AI()->SetData(1, 4);
                        }
                        if (!ogAimTarget.IsEmpty())
                        {
                            if (Player* targetPlayer = ObjectAccessor::GetPlayer(*me, ogAimTarget))
                            {
                                AttackStart(targetPlayer);
                                if (claw)
                                {
                                    claw->AI()->AttackStart(targetPlayer);
                                }
                            }
                        }
                    }
                    else if (!me->IsNonMeleeSpellCast(false))
                    {
                        aimDelay = 1;
                    }
                    return;
                }
                if (Unit* victim = me->GetVictim())
                {
                    if (commandDelay < 0)
                    {
                        commandDelay = 5000;
                        if (Creature* claw = me->FindNearestCreature(17827, 100.0f, true))
                        {
                            if (!claw->IsInCombat())
                            {
                                claw->AI()->AttackStart(victim);
                            }
                        }
                    }
                    float victimDistance = me->GetDistance(victim);
                    if (victimDistance > 30.0f)
                    {
                        if (!IsCombatMovementAllowed())
                        {
                            SetCombatMovement(true);
                        }
                        if (!me->isMoving())
                        {
                            me->GetMotionMaster()->MoveChase(victim, 25.0f, me->GetAngle(victim->GetPosition()));
                        }
                    }
                    else if (victimDistance > 6.0f)
                    {
                        if (me->GetSheath() != SheathState::SHEATH_STATE_RANGED)
                        {
                            me->SetSheath(SheathState::SHEATH_STATE_RANGED);
                        }
                        me->StopMoving();
                        if (IsCombatMovementAllowed())
                        {
                            SetCombatMovement(false);
                        }
                        if (shootDelay < 0)
                        {
                            shootDelay = 1000;
                            DoCastVictim(22907);
                            return;
                        }
                        if (markDelay < 0)
                        {
                            markDelay = 30000;
                            DoCastVictim(31615);
                            return;
                        }
                        if (multiDelay < 0)
                        {
                            multiDelay = 10000;
                            DoCastVictim(34974);
                            return;
                        }
                    }
                    else
                    {
                        if (me->GetSheath() != SheathState::SHEATH_STATE_MELEE)
                        {
                            me->SetSheath(SheathState::SHEATH_STATE_MELEE);
                        }
                        if (trapDelay < 0)
                        {
                            trapDelay = 20000;
                            //DoCastVictim(31946);
                            DoCastVictim(55040);
                            me->AttackStop();
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->StopMoving();
                            me->GetMotionMaster()->Clear();
                            if (Creature* claw = me->FindNearestCreature(17827, 100.0f, true))
                            {
                                claw->AI()->SetData(1, 3);                                
                            }
                            ogAimTarget = victim->GetGUID();
                            setDelay = 4000;
                            return;
                        }
                        if (knockDelay < 0)
                        {
                            me->StopMoving();
                            if (IsCombatMovementAllowed())
                            {
                                SetCombatMovement(false);
                            }
                            knockDelay = 20000;
                            DoCastVictim(18813);
                            return;
                        }
                        if (!IsCombatMovementAllowed())
                        {
                            SetCombatMovement(true);
                        }
                    }
                    if (me->GetReactState() == ReactStates::REACT_PASSIVE)
                    {
                        if (passiveDelay >= 0)
                        {
                            passiveDelay -= diff;
                        }
                        if (passiveDelay < 0)
                        {
                            passiveDelay = 10000;
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        }
                    }
                    else
                    {
                        DoMeleeAttackIfReady();
                    }
                }
            }
        }

        int talkDelay;
        int shootDelay;
        int trapDelay;
        int multiDelay;
        int setDelay;
        int moveDelay;
        int aimDelay;
        int markDelay;
        int knockDelay;
        int commandDelay;
        int passiveDelay;
        ObjectGuid ogAimTarget;
    };
};

class boss_claw : public CreatureScript
{
public:
    boss_claw() : CreatureScript("boss_claw") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetTheUnderbogAI<boss_clawAI>(creature);
    }

    struct boss_clawAI : public ScriptedAI
    {
        boss_clawAI(Creature* creature) : ScriptedAI(creature)
        {
            freed = false;
            eventIndex = 0;
            Initialize();
        }

        void Initialize()
        {
            chargeDelay = 0;
            roarDelay = 2000;
            fenzyDelay = 10000;
            maulDelay = 3000;
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            me->SetImmuneToAll(false);
            eventDelay = 5000;
            commandDelay = 5000;
            passiveDelay = 10000;
            me->SetWalk(true);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            if (!IsCombatMovementAllowed())
            {
                SetCombatMovement(true);
            }
        }

        void Reset() override
        {
            Initialize();
        }

        void JustEngagedWith(Unit* who) override
        {
            me->SetWalk(false);
            if (Creature* musel = me->FindNearestCreature(17826, 100.0f, true))
            {
                if (!musel->IsInCombat())
                {
                    musel->AI()->AttackStart(who);
                }
            }
        }

        void JustDied(Unit* /*killer*/) override
        {

        }

        void KilledUnit(Unit* /*victim*/) override
        {

        }

        bool GossipHello(Player* player) override
        {
            player->RewardPlayerAndGroupAtEvent(17894, player);
            return false;
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    freed = true;
                    eventIndex = 0;
                }
                else if (data == 2)
                {
                    freed = false;
                    eventIndex = 0;
                }
                else if (data == 3)
                {
                    me->AttackStop();
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->StopMoving();
                    passiveDelay = 10000;
                }
                else if (data == 4)
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    passiveDelay = 0;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (chargeDelay >= 0)
            {
                chargeDelay -= diff;
            }
            if (roarDelay >= 0)
            {
                roarDelay -= diff;
            }
            if (fenzyDelay >= 0)
            {
                fenzyDelay -= diff;
            }
            if (maulDelay >= 0)
            {
                maulDelay -= diff;
            }
            if (eventDelay >= 0)
            {
                eventDelay -= diff;
            }
            if (commandDelay >= 0)
            {
                commandDelay -= diff;
            }
            if (me->GetReactState() == ReactStates::REACT_AGGRESSIVE)
            {
                if (UpdateVictim())
                {
                    if (me->IsWalking())
                    {
                        me->SetWalk(false);
                    }
                    if (chargeDelay < 0)
                    {
                        chargeDelay = urand(8000, 12000);
                        Player* targetPlayer = nullptr;
                        Trinity::AnyPlayerInObjectRangeCheck checker(me, 40.0f);
                        Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, targetPlayer, checker);
                        Cell::VisitWorldObjects(me, searcher, 40.0f);
                        if (targetPlayer)
                        {
                            DoCastVictim(39435);
                            return;
                        }
                    }
                    if (Unit* victim = me->GetVictim())
                    {
                        if (commandDelay < 0)
                        {
                            commandDelay = 5000;
                            if (Creature* musel = me->FindNearestCreature(17826, 100.0f, true))
                            {
                                if (!musel->IsInCombat())
                                {
                                    musel->AI()->AttackStart(victim);
                                }
                            }
                        }
                        if (roarDelay < 0)
                        {
                            roarDelay = 25000;
                            DoCastVictim(31429);
                            return;
                        }
                        if (fenzyDelay < 0)
                        {
                            fenzyDelay = urand(30000, 40000);
                            DoCastSelf(34971);
                            return;
                        }
                        if (maulDelay < 0)
                        {
                            maulDelay = urand(5000, 8000);
                            DoCastVictim(34298);
                            return;
                        }
                        DoMeleeAttackIfReady();
                    }
                }
            }
            else
            {
                if (passiveDelay >= 0)
                {
                    passiveDelay -= diff;
                }
                if (passiveDelay < 0)
                {
                    passiveDelay = 10000;
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                }
            }
            if (eventDelay < 0)
            {
                switch (eventIndex)
                {
                case 0:
                {
                    if (freed)
                    {
                        float healthPCT = me->GetHealthPct();
                        if (healthPCT < 20.0f)
                        {
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->ClearInCombat();
                            me->SetImmuneToAll(true);
                            me->SetFaction(1660);
                            me->RemoveAllAuras();
                            me->SetWalk(false);
                            me->GetMotionMaster()->MovePoint(0, 290.532f, -125.352f, 29.6971f);
                            eventIndex = 1;
                            eventDelay = 5000;
                        }
                    }
                    break;
                }
                case 1:
                {
                    if (me->isMoving())
                    {
                        eventDelay = 1000;
                    }
                    else
                    {
                        me->SetFacingTo(1.82491f);
                        eventIndex = 2;
                        eventDelay = 500;
                    }
                    break;
                }
                case 2:
                {
                    me->SetDisplayId(17309);
                    eventIndex = 3;
                    eventDelay = 1000;
                    break;
                }
                case 3:
                {
                    me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_SIT);
                    eventIndex = 4;
                    eventDelay = 1000;
                    break;
                }
                case 4:
                {
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    eventIndex = 5;
                    eventDelay = 5000;
                    break;
                }
                case 5:
                {
                    eventDelay = 5000;
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }

        int chargeDelay;
        int roarDelay;
        int fenzyDelay;
        int maulDelay;
        bool freed;
        int eventIndex;
        int eventDelay;
        int commandDelay;
        int passiveDelay;
    };
};

void AddSC_boss_swamplord_muselek()
{
    new boss_swamplord_muselek();
    new boss_claw();
}
