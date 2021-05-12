#include "Script_Priest.h"
#include "Group.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"

Script_Priest::Script_Priest(Player* pmMe) :Script_Base(pmMe)
{
    manaCheckDelay = 0;
    powerWordShieldDelay = 0;
    powerWordBarrierDelay = 0;
    fearWardDelay = 0;
    painSuppressionDelay = 0;
    penanceDelay = 0;
    innerFocusDelay = 0;
    powerInfusionDelay = 0;
    hymnOfHopeDelay = 0;
    prayerOfMendingDelay = 0;
    shadowfiendDelay = 0;
}

void Script_Priest::Update(uint32 pmDiff)
{
    Script_Base::Update(pmDiff);
    if (manaCheckDelay >= 0)
    {
        manaCheckDelay -= pmDiff;
    }
    if (powerWordShieldDelay >= 0)
    {
        powerWordShieldDelay -= pmDiff;
    }
    if (powerWordBarrierDelay >= 0)
    {
        powerWordBarrierDelay -= pmDiff;
    }
    if (fearWardDelay >= 0)
    {
        fearWardDelay -= pmDiff;
    }
    if (painSuppressionDelay >= 0)
    {
        painSuppressionDelay -= pmDiff;
    }
    if (penanceDelay >= 0)
    {
        penanceDelay -= pmDiff;
    }
    if (innerFocusDelay >= 0)
    {
        innerFocusDelay -= pmDiff;
    }
    if (powerInfusionDelay >= 0)
    {
        powerInfusionDelay -= pmDiff;
    }
    if (hymnOfHopeDelay >= 0)
    {
        hymnOfHopeDelay -= pmDiff;
    }
    if (prayerOfMendingDelay >= 0)
    {
        prayerOfMendingDelay -= pmDiff;
    }
    if (shadowfiendDelay >= 0)
    {
        shadowfiendDelay -= pmDiff;
    }
}

void Script_Priest::Reset()
{
    Script_Base::Reset();
}

bool Script_Priest::Revive(Player* pmTarget)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    if (me->IsNonMeleeSpellCast(false))
    {
        return true;
    }
    if (pmTarget)
    {
        if (!pmTarget->IsAlive())
        {
            if (!pmTarget->IsResurrectRequested())
            {
                float targetDistance = me->GetDistance(pmTarget);
                if (targetDistance < RANGE_HEAL_DISTANCE)
                {
                    if (CastSpell(pmTarget, "Resurrection"))
                    {
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        if (ogReviveTarget.IsEmpty())
        {
            if (Group* myGroup = me->GetGroup())
            {
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (Revive(member))
                        {
                            return true;
                        }
                    }
                }
            }
        }
        else
        {
            Player* targetPlayer = ObjectAccessor::FindPlayer(ogReviveTarget);
            if (Revive(targetPlayer))
            {
                return true;
            }
        }
    }

    return true;
}

bool Script_Priest::Cure(Unit* pmTarget)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    if (pmTarget)
    {
        if (pmTarget->IsAlive())
        {
            float targetDistance = me->GetDistance(pmTarget);
            if (targetDistance < RANGE_HEAL_DISTANCE)
            {
                for (uint32 type = SPELL_AURA_NONE; type < TOTAL_AURAS; ++type)
                {
                    std::list<AuraEffect*> auraList = pmTarget->GetAuraEffectsByType((AuraType)type);
                    for (auto auraIT = auraList.begin(), end = auraList.end(); auraIT != end; ++auraIT)
                    {
                        const SpellInfo* pST = (*auraIT)->GetSpellInfo();
                        if (!pST->IsPassive())
                        {
                            if (!pST->IsPositive())
                            {
                                if (pST->Dispel == DispelType::DISPEL_MAGIC)
                                {
                                    if (CastSpell(pmTarget, "Dispel Magic"))
                                    {
                                        return true;
                                    }
                                }
                                else if (pST->Dispel == DispelType::DISPEL_DISEASE)
                                {
                                    if (CastSpell(pmTarget, "Cure Disease"))
                                    {
                                        return true;
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
        if (cureDelay < 0)
        {
            cureDelay = 1000;
            if (Group* myGroup = me->GetGroup())
            {
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (Cure(member))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Script_Priest::Heal(Unit* pmTarget)
{
    if (!me)
    {
        return false;
    }
    if (!me->IsAlive())
    {
        return false;
    }
    bool healResult = false;

    if (manaCheckDelay < 0)
    {
        manaCheckDelay = 1000;
        int manaPCT = me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA);
        if (manaPCT < 20)
        {
            if (hymnOfHopeDelay < 0)
            {
                hymnOfHopeDelay = 1000;
                if (CastSpell(me, "Hymn of Hope"))
                {
                    hymnOfHopeDelay = 361000;
                    return true;
                }
            }
            if (shadowfiendDelay < 0)
            {
                shadowfiendDelay = 1000;
                if (Group* myGroup = me->GetGroup())
                {
                    for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                    {
                        if (Player* member = groupRef->GetSource())
                        {
                            if (Awareness_Base* ab = member->awarenessMap[member->activeAwarenessIndex])
                            {
                                if (ab->groupRole == GroupRole::GroupRole_Tank)
                                {
                                    if (Unit* tankTarget = member->GetSelectedUnit())
                                    {
                                        if (me->IsValidAttackTarget(tankTarget))
                                        {
                                            if (CastSpell(tankTarget, "Shadowfiend"))
                                            {
                                                shadowfiendDelay = 301000;
                                                return true;
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    switch (maxTalentTab)
    {
    case 0:
    {
        healResult = Heal_Discipline(pmTarget);
        break;
    }
    case 1:
    {
        break;
    }
    case 2:
    {
        break;
    }
    default:
    {
        break;
    }
    }

    return healResult;
}

bool Script_Priest::Heal_Discipline(Unit* pmTarget)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    if (pmTarget)
    {
        if (pmTarget->IsAlive())
        {
            float healthPCT = pmTarget->GetHealthPct();

            if (pmTarget->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (Player* targetPlayer = pmTarget->ToPlayer())
                {
                    if (Awareness_Base* ab = targetPlayer->awarenessMap[targetPlayer->activeAwarenessIndex])
                    {
                        if (ab->groupRole == GroupRole::GroupRole_Tank)
                        {
                            if (pmTarget->IsInCombat())
                            {
                                if (powerWordShieldDelay < 0)
                                {
                                    powerWordShieldDelay = 1000;
                                    if (!sNingerManager->HasAura(pmTarget, "Weakened Soul"))
                                    {
                                        if (CastSpell(pmTarget, "Power Word: Shield"))
                                        {
                                            return true;
                                        }
                                    }
                                }
                            }
                            if (prayerOfMendingDelay < 0)
                            {
                                prayerOfMendingDelay = 1000;
                                if (CastSpell(pmTarget, "Prayer of Mending", true))
                                {
                                    prayerOfMendingDelay = 11000;
                                    return true;
                                }
                            }
                            if (healthPCT < 95.0f)
                            {
                                if (CastSpell(pmTarget, "Renew", true, true))
                                {
                                    return true;
                                }
                            }
                            if (healthPCT > 70.0f && healthPCT < 90.0f)
                            {
                                if (CastSpell(pmTarget, "Heal"))
                                {
                                    return true;
                                }
                            }
                            if (healthPCT < 70.0f)
                            {
                                if (penanceDelay < 0)
                                {
                                    penanceDelay = 1000;
                                    if (CastSpell(pmTarget, "Penance"))
                                    {
                                        penanceDelay = 13000;
                                        return true;
                                    }
                                }
                            }
                            if (pmTarget->IsInCombat())
                            {
                                if (healthPCT > 40.0f && healthPCT < 70.0f)
                                {
                                    if (powerInfusionDelay < 0)
                                    {
                                        powerInfusionDelay = 1000;
                                        if (!sNingerManager->HasAura(me, "Inner Focus"))
                                        {
                                            if (CastSpell(me, "Power Infusion"))
                                            {
                                                powerInfusionDelay = 121000;
                                            }
                                        }
                                    }
                                    else if (innerFocusDelay < 0)
                                    {
                                        innerFocusDelay = 1000;
                                        if (!sNingerManager->HasAura(me, "Power Infusion"))
                                        {
                                            if (CastSpell(me, "Inner Focus"))
                                            {
                                                innerFocusDelay = 46000;
                                            }
                                        }
                                    }
                                    if (CastSpell(pmTarget, "Greater Heal"))
                                    {
                                        return true;
                                    }
                                }
                                if (healthPCT < 40.0f)
                                {
                                    if (powerWordBarrierDelay < 0)
                                    {
                                        powerWordBarrierDelay = 1000;
                                        if (!sNingerManager->HasAura(pmTarget, "Pain Suppression"))
                                        {
                                            if (CastSpell(pmTarget, "Power Word: Barrier"))
                                            {
                                                powerWordBarrierDelay = 181000;
                                                return true;
                                            }
                                        }
                                    }
                                    else if (painSuppressionDelay < 0)
                                    {
                                        painSuppressionDelay = 1000;
                                        if (!sNingerManager->HasAura(pmTarget, "Power Word: Barrier"))
                                        {
                                            if (CastSpell(pmTarget, "Pain Suppression"))
                                            {
                                                painSuppressionDelay = 181000;
                                                return true;
                                            }
                                        }
                                    }
                                }
                                if (CastSpell(pmTarget, "Flash Heal"))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            if (healthPCT < 80.0f)
            {
                if (CastSpell(pmTarget, "Renew", true, true))
                {
                    return true;
                }
                if (CastSpell(pmTarget, "Heal"))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if (Group* myGroup = me->GetGroup())
        {
            Player* mainTank = NULL;
            for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
            {
                if (Player* member = groupRef->GetSource())
                {
                    if (Awareness_Base* ab = member->awarenessMap[member->activeAwarenessIndex])
                    {
                        if (ab->groupRole == GroupRole::GroupRole_Tank)
                        {
                            mainTank = member;
                            break;
                        }
                    }
                }
            }
            if (mainTank)
            {
                if (mainTank->IsAlive())
                {
                    if (Heal_Discipline(mainTank))
                    {
                        return true;
                    }
                }
            }
            for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
            {
                if (Player* member = groupRef->GetSource())
                {
                    if (Awareness_Base* ab = member->awarenessMap[member->activeAwarenessIndex])
                    {
                        if (ab->groupRole != GroupRole::GroupRole_Tank)
                        {
                            float healthPCT = member->GetHealthPct();
                            if (healthPCT < 50.0f)
                            {
                                if (Heal_Discipline(member))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Script_Priest::Buff(Unit* pmTarget)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    if (pmTarget)
    {
        if (pmTarget->GetGUID() == me->GetGUID())
        {
            if (CastSpell(me, "Inner Fire", true))
            {
                return true;
            }
        }
        if (pmTarget->IsAlive())
        {
            float targetDistance = me->GetDistance(pmTarget);
            if (targetDistance < RANGE_DPS_DISTANCE)
            {
                if (CastSpell(pmTarget, "Power Word: Fortitude", true))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if (buffDelay < 0)
        {
            buffDelay = 2000;
            if (Group* myGroup = me->GetGroup())
            {
                for (GroupReference* groupRef = myGroup->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
                {
                    if (Player* member = groupRef->GetSource())
                    {
                        if (Buff(member))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}
