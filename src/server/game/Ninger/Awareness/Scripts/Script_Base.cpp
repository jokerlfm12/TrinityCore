#include "Script_Base.h"
#include "Bag.h"
#include "Spell.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "CreatureAI.h"
#include "MotionMaster.h"
#include "SpellMgr.h"
#include "SpellHistory.h"
#include "Pet.h"

NingerMovement::NingerMovement(Player* pmMe)
{
    me = pmMe;
    ogChaseTarget = ObjectGuid::Empty;
    activeMovementType = NingerMovementType::NingerMovementType_None;
    chaseDistanceMin = MELEE_MIN_DISTANCE;
    chaseDistanceMax = MELEE_MAX_DISTANCE;
}

void NingerMovement::ResetMovement()
{
    ogChaseTarget = ObjectGuid::Empty;
    activeMovementType = NingerMovementType::NingerMovementType_None;
    chaseDistanceMin = MELEE_MIN_DISTANCE;
    chaseDistanceMax = MELEE_MAX_DISTANCE;
    if (me)
    {
        me->StopMoving();
        me->GetMotionMaster()->Initialize();
    }
}

bool NingerMovement::Chase(Unit* pmChaseTarget, float pmChaseDistanceMin, float pmChaseDistanceMax, uint32 pmLimitDelay)
{
    if (!me)
    {
        return false;
    }
    if (!me->IsAlive())
    {
        return false;
    }
    if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
    {
        return false;
    }
    if (me->HasUnitState(UnitState::UNIT_STATE_NOT_MOVE))
    {
        return false;
    }
    if (me->IsNonMeleeSpellCast(false))
    {
        return false;
    }
    if (!pmChaseTarget)
    {
        return false;
    }
    if (me->GetMapId() != pmChaseTarget->GetMapId())
    {
        return false;
    }
    float unitTargetDistance = me->GetDistance(pmChaseTarget);
    if (unitTargetDistance > VISIBILITY_DISTANCE_LARGE)
    {
        return false;
    }
    if (pmChaseTarget->GetTypeId() == TypeID::TYPEID_PLAYER)
    {
        if (Player* targetPlayer = pmChaseTarget->ToPlayer())
        {
            if (targetPlayer->IsBeingTeleported())
            {
                return false;
            }
        }
    }
    chaseDistanceMin = pmChaseDistanceMin;
    chaseDistanceMax = pmChaseDistanceMax;
    if (activeMovementType == NingerMovementType::NingerMovementType_Chase)
    {
        if (ogChaseTarget == pmChaseTarget->GetGUID())
        {
            return true;
        }
    }
    ResetMovement();
    ogChaseTarget = pmChaseTarget->GetGUID();
    activeMovementType = NingerMovementType::NingerMovementType_Chase;
    if (me->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
    {
        me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
    }
    bool distanceValid = false;
    if (unitTargetDistance < chaseDistanceMax + MIN_DISTANCE_GAP)
    {
        if (chaseDistanceMin > MELEE_MIN_DISTANCE)
        {
            if (unitTargetDistance > chaseDistanceMin)
            {
                distanceValid = true;
            }
            else
            {
                distanceValid = false;
            }
        }
        else
        {
            distanceValid = true;
        }
    }
    if (distanceValid)
    {
        if (me->IsWithinLOSInMap(pmChaseTarget))
        {
            if (!me->isInFront(pmChaseTarget, M_PI / 4))
            {
                me->SetFacingToObject(pmChaseTarget);
            }
        }
    }
    else
    {
        float distanceInRange = frand(chaseDistanceMin, chaseDistanceMax);
        float nearX = 0, nearY = 0, nearZ = 0;
        float dynamicAngle = M_PI / 16;
        if (distanceInRange < INTERACTION_DISTANCE)
        {
            dynamicAngle = M_PI / 8;
        }
        float chaseAngle = pmChaseTarget->GetAngle(me);
        chaseAngle = frand(chaseAngle - dynamicAngle, chaseAngle + dynamicAngle);
        pmChaseTarget->GetNearPoint(me, nearX, nearY, nearZ, distanceInRange, chaseAngle);
        MoveTargetPosition(nearX, nearY, nearZ);
    }
    return true;
}

void NingerMovement::MovePoint(Position pmTargetPosition, uint32 pmLimitDelay)
{
    MovePoint(pmTargetPosition.m_positionX, pmTargetPosition.m_positionY, pmTargetPosition.m_positionZ, pmLimitDelay);
}

void NingerMovement::MovePoint(float pmX, float pmY, float pmZ, uint32 pmLimitDelay)
{
    if (!me)
    {
        return;
    }
    if (!me->IsAlive())
    {
        return;
    }
    if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
    {
        return;
    }
    if (me->HasUnitState(UnitState::UNIT_STATE_NOT_MOVE))
    {
        return;
    }
    if (me->IsNonMeleeSpellCast(false))
    {
        return;
    }
    if (me->IsBeingTeleported())
    {
        ResetMovement();
        return;
    }
    float distance = me->GetDistance(pmX, pmY, pmZ);
    if (distance >= CONTACT_DISTANCE && distance <= VISIBILITY_DISTANCE_LARGE)
    {
        activeMovementType = NingerMovementType::NingerMovementType_Point;
        MoveTargetPosition(pmX, pmY, pmZ);
    }
}

void NingerMovement::MoveTargetPosition()
{
    if (me)
    {
        if (me->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
        {
            me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
        }
        if (me->IsWalking())
        {
            me->SetWalk(false);
        }
        me->GetMotionMaster()->MovePoint(0, positionTarget);
    }
}

void NingerMovement::MoveTargetPosition(float pmX, float pmY, float pmZ)
{
    positionTarget.m_positionX = pmX;
    positionTarget.m_positionY = pmY;
    positionTarget.m_positionZ = pmZ;
    MoveTargetPosition();
}

void NingerMovement::Update(uint32 pmDiff)
{
    if (!me)
    {
        return;
    }
    if (!me->IsAlive())
    {
        return;
    }
    if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
    {
        return;
    }
    if (me->HasUnitState(UnitState::UNIT_STATE_NOT_MOVE))
    {
        return;
    }
    if (me->IsNonMeleeSpellCast(false))
    {
        return;
    }
    if (me->IsBeingTeleported())
    {
        ResetMovement();
        return;
    }
    switch (activeMovementType)
    {
    case NingerMovementType::NingerMovementType_None:
    {
        break;
    }
    case NingerMovementType::NingerMovementType_Point:
    {
        float distance = me->GetExactDist(positionTarget);
        if (distance > VISIBILITY_DISTANCE_LARGE || distance < CONTACT_DISTANCE)
        {
            ResetMovement();
        }
        else
        {
            if (!me->isMoving())
            {
                MoveTargetPosition();
            }
        }
        break;
    }
    case NingerMovementType::NingerMovementType_Chase:
    {
        if (Unit* chaseTarget = ObjectAccessor::GetUnit(*me, ogChaseTarget))
        {
            if (me->GetMapId() != chaseTarget->GetMapId())
            {
                ResetMovement();
                break;
            }
            if (chaseTarget->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (Player* targetPlayer = chaseTarget->ToPlayer())
                {
                    if (!targetPlayer->IsInWorld())
                    {
                        ResetMovement();
                        break;
                    }
                    else if (targetPlayer->IsBeingTeleported())
                    {
                        ResetMovement();
                        break;
                    }
                }
            }
            float unitTargetDistance = me->GetDistance(chaseTarget);
            if (unitTargetDistance > VISIBILITY_DISTANCE_LARGE)
            {
                ResetMovement();
                break;
            }
            bool ok = false;
            if (unitTargetDistance >= chaseDistanceMin && unitTargetDistance <= chaseDistanceMax + MIN_DISTANCE_GAP)
            {
                if (me->IsWithinLOSInMap(chaseTarget))
                {
                    if (me->isMoving())
                    {
                        me->StopMoving();
                    }
                    if (!me->isInFront(chaseTarget, M_PI / 4))
                    {
                        me->SetFacingToObject(chaseTarget);
                    }
                    ok = true;
                }
            }
            if (!ok)
            {
                if (me->isMoving())
                {
                    ok = true;
                }
            }
            if (!ok)
            {
                if (me->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
                {
                    me->SetStandState(UnitStandStateType::UNIT_STAND_STATE_STAND);
                }
                float distanceInRange = frand(chaseDistanceMin, chaseDistanceMax);
                float nearX = 0, nearY = 0, nearZ = 0;
                float dynamicAngle = M_PI / 16;
                if (distanceInRange < INTERACTION_DISTANCE)
                {
                    dynamicAngle = M_PI / 8;
                }
                float chaseAngle = chaseTarget->GetAngle(me);
                chaseAngle = frand(chaseAngle - dynamicAngle, chaseAngle + dynamicAngle);
                chaseTarget->GetNearPoint(me, nearX, nearY, nearZ, distanceInRange, chaseAngle);
                MoveTargetPosition(nearX, nearY, nearZ);
            }
        }
        else
        {
            ResetMovement();
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

Script_Base::Script_Base(Player* pmMe)
{
    me = pmMe;
    rm = new NingerMovement(me);
    spellIDMap.clear();
    maxTalentTab = 0;
    buffDelay = 0;
    healDelay = 0;
    cureDelay = 0;
    potionDelay = 0;
    aoeCheckDelay = 0;
    chaseDistanceMin = MELEE_MIN_DISTANCE;
    chaseDistanceMax = MELEE_MAX_DISTANCE;

    rti = -1;
    ogReviveTarget.Clear();
}

void Script_Base::Initialize()
{
    spellIDMap.clear();
    for (PlayerSpellMap::iterator it = me->GetSpellMap().begin(); it != me->GetSpellMap().end(); it++)
    {
        const SpellInfo* pS = sSpellMgr->GetSpellInfo(it->first);
        if (pS)
        {
            if (!pS->IsPassive())
            {
                std::string checkNameStr = std::string(pS->SpellName);
                if (spellIDMap.find(checkNameStr) == spellIDMap.end())
                {
                    spellIDMap[checkNameStr] = it->first;
                }
            }
        }
    }
}

void Script_Base::Reset()
{
    rti = -1;
    if (me)
    {
        maxTalentTab = me->GetMaxTalentCountTab();
    }
    if (rm)
    {
        rm->ResetMovement();
    }
    ClearTarget();
    buffDelay = 0;
    healDelay = 0;
    cureDelay = 0;
    potionDelay = 0;
    aoeCheckDelay = 0;
}

bool Script_Base::Revive(Player* pmTarget)
{
    return false;
}

void Script_Base::Update(uint32 pmDiff)
{
    if (buffDelay >= 0)
    {
        buffDelay -= pmDiff;
    }
    if (healDelay >= 0)
    {
        healDelay -= pmDiff;
    }
    if (cureDelay >= 0)
    {
        cureDelay -= pmDiff;
    }
    if (potionDelay >= 0)
    {
        potionDelay -= pmDiff;
    }
    if (aoeCheckDelay >= 0)
    {
        aoeCheckDelay -= pmDiff;
    }
    rm->Update(pmDiff);
    return;
}

bool Script_Base::DPS(Unit* pmTarget, bool pmChase, bool pmAOE, bool pmMark, float pmChaseDistanceMin, float pmChaseDistanceMax)
{
    return false;
}

bool Script_Base::Tank(Unit* pmTarget, bool pmChase, bool pmAOE)
{
    return false;
}

bool Script_Base::Heal(Unit* pmTarget)
{
    return false;
}

bool Script_Base::Cure(Unit* pmTarget)
{
    return false;
}

bool Script_Base::Buff(Unit* pmTarget)
{
    return false;
}

bool Script_Base::Petting(bool pmSummon)
{
    return false;
}

bool Script_Base::Assist(Unit* pmTarget)
{
    return false;
}

Item* Script_Base::GetItemInInventory(uint32 pmEntry)
{
    if (!me)
    {
        return NULL;
    }
    for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
    {
        Item* pItem = me->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (pItem)
        {
            if (pItem->GetEntry() == pmEntry)
            {
                return pItem;
            }
        }
    }

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
    {
        if (Bag* pBag = (Bag*)me->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        {
            for (uint32 j = 0; j < pBag->GetBagSize(); j++)
            {
                Item* pItem = me->GetItemByPos(i, j);
                if (pItem)
                {
                    if (pItem->GetEntry() == pmEntry)
                    {
                        return pItem;
                    }
                }
            }
        }
    }

    return NULL;
}

bool Script_Base::UseItem(Item* pmItem, Unit* pmTarget)
{
    if (!me)
    {
        return false;
    }
    if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
    {
        return false;
    }

    if (me->IsNonMeleeSpellCast(true))
    {
        return false;
    }

    if (const ItemTemplate* proto = pmItem->GetTemplate())
    {
        me->SetSelection(pmTarget->GetGUID());
        me->SetTarget(pmTarget->GetGUID());
        SpellCastTargets targets;
        targets.Update(pmTarget);
        me->CastItemUseSpell(pmItem, targets, 1, 0);
        return true;
    }

    return false;
}

bool Script_Base::UseItem(Item* pmItem, Item* pmTarget)
{
    if (!me)
    {
        return false;
    }
    if (me->CanUseItem(pmItem) != EQUIP_ERR_OK)
    {
        return false;
    }
    if (me->IsNonMeleeSpellCast(true))
    {
        return false;
    }
    if (const ItemTemplate* proto = pmItem->GetTemplate())
    {
        SpellCastTargets targets;
        targets.SetItemTarget(pmTarget);
        me->CastItemUseSpell(pmItem, targets, 1, 0);
        return true;
    }

    return false;
}

bool Script_Base::Follow(Unit* pmTarget, float pmDistance)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    else if (me->HasAuraType(SPELL_AURA_MOD_PACIFY))
    {
        return false;
    }
    else if (me->HasUnitState(UnitState::UNIT_STATE_NOT_MOVE))
    {
        return false;
    }
    else if (me->IsNonMeleeSpellCast(false))
    {
        return false;
    }
    return rm->Chase(pmTarget, 0.0f, pmDistance);
}

bool Script_Base::Chase(Unit* pmTarget, float pmMinDistance, float pmMaxDistance)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    else if (rm->Chase(pmTarget, pmMinDistance, pmMaxDistance))
    {
        ChooseTarget(pmTarget);
        return true;
    }
    return false;
}

uint32 Script_Base::FindSpellID(std::string pmSpellName)
{
    if (spellIDMap.find(pmSpellName) != spellIDMap.end())
    {
        return spellIDMap[pmSpellName];
    }
    //for (std::unordered_map<std::string, uint32>::iterator sIT = spellIDMap.begin(); sIT != spellIDMap.end(); sIT++)
    //{
    //    if (sIT->first == pmSpellName)
    //    {
    //        if (const SpellInfo* pS = sSpellMgr->GetSpellInfo(sIT->second))
    //        {
    //            if (!pS->IsPassive())
    //            {
    //                return sIT->second;
    //            }
    //        }
    //    }
    //}

    return 0;
}

bool Script_Base::SpellValid(uint32 pmSpellID)
{
    if (pmSpellID == 0)
    {
        return false;
    }
    if (!me)
    {
        return false;
    }
    if (me->GetSpellHistory()->HasCooldown(pmSpellID))
    {
        return false;
    }
    return true;
}

bool Script_Base::CastSpell(Unit* pmTarget, std::string pmSpellName, bool pmCheckAura, bool pmOnlyMyAura, bool pmClearShapeShift)
{
    if (!me)
    {
        return false;
    }
    if (pmClearShapeShift)
    {
        ClearShapeshift();
    }
    uint32 spellID = FindSpellID(pmSpellName);
    if (!SpellValid(spellID))
    {
        return false;
    }
    const SpellInfo* pS = sSpellMgr->GetSpellInfo(spellID);
    if (!pS)
    {
        return false;
    }
    if (pmTarget)
    {
        if (!me->IsWithinLOSInMap(pmTarget))
        {
            return false;
        }
        if (pmTarget->IsImmunedToSpell(pS, me))
        {
            return false;
        }
        if (pmCheckAura)
        {
            if (pmOnlyMyAura)
            {
                if (sNingerManager->HasAura(pmTarget, pmSpellName, me))
                {
                    return false;
                }
            }
            else
            {
                if (sNingerManager->HasAura(pmTarget, pmSpellName))
                {
                    return false;
                }
            }
        }
        if (!me->isInFront(pmTarget, M_PI / 4))
        {
            me->SetFacingToObject(pmTarget);
        }
        if (me->GetTarget() != pmTarget->GetGUID())
        {
            ChooseTarget(pmTarget);
        }
    }
    for (size_t i = 0; i < MAX_SPELL_REAGENTS; i++)
    {
        if (pS->Reagent[i] > 0)
        {
            if (!me->HasItemCount(pS->Reagent[i], pS->ReagentCount[i]))
            {
                me->StoreNewItemInBestSlots(pS->Reagent[i], pS->ReagentCount[i] * 10);
            }
        }
    }
    if (me->GetStandState() != UnitStandStateType::UNIT_STAND_STATE_STAND)
    {
        me->SetStandState(UNIT_STAND_STATE_STAND);
    }
    SpellCastResult scr = me->CastSpell(pmTarget, pS);
    if (scr == SpellCastResult::SPELL_CAST_OK)
    {
        return true;
    }

    return false;
}

void Script_Base::ClearShapeshift()
{
    if (!me)
    {
        return;
    }
    uint32 spellID = 0;
    switch (me->GetShapeshiftForm())
    {
    case ShapeshiftForm::FORM_NONE:
    {
        break;
    }
    case ShapeshiftForm::FORM_CAT:
    {
        spellID = FindSpellID("Cat Form");
        break;
    }
    case ShapeshiftForm::FORM_DIREBEAR:
    {
        spellID = FindSpellID("Dire Bear Form");
        break;
    }
    case ShapeshiftForm::FORM_BEAR:
    {
        spellID = FindSpellID("Bear Form");
        break;
    }
    case ShapeshiftForm::FORM_MOONKIN:
    {
        spellID = FindSpellID("Moonkin Form");
        break;
    }
    default:
    {
        break;
    }
    }
    CancelAura(spellID);
}

bool Script_Base::CancelAura(std::string pmSpellName)
{
    if (!me)
    {
        return false;
    }
    std::set<uint32> spellIDSet = sNingerManager->spellNameEntryMap[pmSpellName];
    for (std::set<uint32>::iterator it = spellIDSet.begin(); it != spellIDSet.end(); it++)
    {
        if (me->HasAura((*it)))
        {
            CancelAura((*it));
            return true;
        }
    }

    return false;
}

void Script_Base::CancelAura(uint32 pmSpellID)
{
    if (pmSpellID == 0)
    {
        return;
    }
    if (!me)
    {
        return;
    }
    const SpellInfo* pS = sSpellMgr->GetSpellInfo(pmSpellID);
    if (!pS)
    {
        return;
    }
    if (pS->Attributes & SPELL_ATTR0_CANT_CANCEL)
    {
        return;
    }
    // channeled spell case (it currently cast then)
    if (pS->IsChanneled())
    {
        if (Spell* curSpell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            if (curSpell->m_spellInfo->Id == pmSpellID)
                me->InterruptSpell(CURRENT_CHANNELED_SPELL);
        return;
    }

    // non channeled case:
    // don't allow remove non positive spells
    // don't allow cancelling passive auras (some of them are visible)
    if (!pS->IsPositive() || pS->IsPassive())
        return;

    // maybe should only remove one buff when there are multiple?
    me->RemoveOwnedAura(pmSpellID, ObjectGuid::Empty, 0, AuraRemoveFlags::ByCancel);

    // If spell being removed is a resource tracker, see if player was tracking both (herbs / minerals) and remove the other
    if (sWorld->getBoolConfig(CONFIG_ALLOW_TRACK_BOTH_RESOURCES) && pS->HasAura(SPELL_AURA_TRACK_RESOURCES))
    {
        Unit::AuraEffectList const& auraEffects = me->GetAuraEffectsByType(SPELL_AURA_TRACK_RESOURCES);
        if (!auraEffects.empty())
        {
            // Build list of spell IDs to cancel. Trying to cancel the aura while iterating
            //  over AuraEffectList caused "incompatible iterator" errors on second pass
            std::list<uint32> spellIDs;

            for (Unit::AuraEffectList::const_iterator auraEffect = auraEffects.begin(); auraEffect != auraEffects.end(); ++auraEffect)
                spellIDs.push_back((*auraEffect)->GetId());

            // Remove all auras related to resource tracking (only Herbs and Minerals in 3.3.5a)
            for (std::list<uint32>::iterator it = spellIDs.begin(); it != spellIDs.end(); ++it)
                me->RemoveOwnedAura(*it, ObjectGuid::Empty, 0, AuraRemoveFlags::ByCancel);
        }
    }
}

bool Script_Base::Eat(bool pmForce)
{
    if (!me)
    {
        return false;
    }
    else if (!me->IsAlive())
    {
        return false;
    }
    else if (me->IsInCombat())
    {
        return false;
    }
    bool result = pmForce;
    if (!result)
    {
        if (me->GetHealthPct() < 40.0f)
        {
            result = true;
        }
    }
    if (result)
    {
        uint32 foodEntry = 0;
        uint32 myLevel = me->getLevel();
        if (myLevel >= 85)
        {
            foodEntry = 58261;
        }
        else if (myLevel >= 80)
        {
            foodEntry = 58260;
        }
        else if (myLevel >= 75)
        {
            foodEntry = 35950;
        }
        else if (myLevel >= 65)
        {
            foodEntry = 33449;
        }
        else if (myLevel >= 55)
        {
            foodEntry = 27855;
        }
        else if (myLevel >= 45)
        {
            foodEntry = 8950;
        }
        else if (myLevel >= 35)
        {
            foodEntry = 4601;
        }
        else if (myLevel >= 25)
        {
            foodEntry = 4544;
        }
        else if (myLevel >= 15)
        {
            foodEntry = 4542;
        }
        else if (myLevel >= 5)
        {
            foodEntry = 4541;
        }
        else
        {
            foodEntry = 4540;
        }
        if (result)
        {
            if (!me->HasItemCount(foodEntry, 1))
            {
                me->StoreNewItemInBestSlots(foodEntry, 20);
            }
            me->CombatStop(true);
            me->StopMoving();
            me->GetMotionMaster()->Initialize();
            ClearTarget();

            Item* pFood = GetItemInInventory(foodEntry);
            if (pFood && !pFood->IsInTrade())
            {
                if (UseItem(pFood, me))
                {
                    rm->ResetMovement();
                }
            }
        }
    }
    else
    {
        if (me->GetPowerType() == Powers::POWER_MANA)
        {
            if (me->GetPower(Powers::POWER_MANA) * 100 / me->GetMaxPower(Powers::POWER_MANA) < 40.0f)
            {
                result = true;
            }
        }
    }
    return result;
}

bool Script_Base::Drink()
{
    if (!me)
    {
        return false;
    }
    if (!me->IsAlive())
    {
        return false;
    }
    if (me->IsInCombat())
    {
        return false;
    }
    uint32 drinkEntry = 0;
    uint32 myLevel = me->getLevel();
    if (myLevel >= 85)
    {
        drinkEntry = 58257;
    }
    else if (myLevel >= 80)
    {
        drinkEntry = 58256;
    }
    else if (myLevel >= 75)
    {
        drinkEntry = 33445;
    }
    else if (myLevel >= 70)
    {
        drinkEntry = 33444;
    }
    else if (myLevel >= 60)
    {
        drinkEntry = 28399;
    }
    else if (myLevel >= 45)
    {
        drinkEntry = 8766;
    }
    else if (myLevel >= 35)
    {
        drinkEntry = 1645;
    }
    else if (myLevel >= 25)
    {
        drinkEntry = 1708;
    }
    else if (myLevel >= 15)
    {
        drinkEntry = 1205;
    }
    else if (myLevel >= 5)
    {
        drinkEntry = 1179;
    }
    else
    {
        drinkEntry = 159;
    }

    if (!me->HasItemCount(drinkEntry, 1))
    {
        me->StoreNewItemInBestSlots(drinkEntry, 20);
    }
    me->CombatStop(true);
    me->GetMotionMaster()->Initialize();
    me->StopMoving();
    ClearTarget();
    Item* pDrink = GetItemInInventory(drinkEntry);
    if (pDrink && !pDrink->IsInTrade())
    {
        if (UseItem(pDrink, me))
        {
            rm->ResetMovement();
            return true;
        }
    }

    return false;
}

void Script_Base::PetAttack(Unit* pmTarget)
{
    if (me)
    {
        if (Pet* myPet = me->GetPet())
        {
            if (myPet->IsAlive())
            {
                if (CreatureAI* cai = myPet->AI())
                {
                    cai->AttackStart(pmTarget);
                }
            }
        }
    }
}

void Script_Base::PetStop()
{
    if (me)
    {
        if (Pet* myPet = me->GetPet())
        {
            myPet->AttackStop();
            if (CharmInfo* pci = myPet->GetCharmInfo())
            {
                if (pci->IsCommandAttack())
                {
                    pci->SetIsCommandAttack(false);
                }
                if (!pci->IsCommandFollow())
                {
                    pci->SetIsCommandFollow(true);
                }
            }
        }
    }
}

bool Script_Base::UseHealingPotion()
{
    bool result = false;

    if (potionDelay > 61000)
    {
        if (!me)
        {
            return false;
        }
        if (!me->IsInCombat())
        {
            return false;
        }
        uint32 itemEntry = 0;
        uint32 myLevel = me->getLevel();
        if (myLevel >= 45)
        {
            itemEntry = 13446;
        }
        else if (myLevel >= 35)
        {
            itemEntry = 3928;
        }
        else if (myLevel >= 21)
        {
            itemEntry = 1710;
        }
        else if (myLevel >= 12)
        {
            itemEntry = 929;
        }
        else
        {
            itemEntry = 118;
        }
        if (!me->HasItemCount(itemEntry, 1))
        {
            me->StoreNewItemInBestSlots(itemEntry, 20);
        }
        Item* pItem = GetItemInInventory(itemEntry);
        if (pItem && !pItem->IsInTrade())
        {
            if (UseItem(pItem, me))
            {
                potionDelay = 0;
                result = true;
            }
        }

    }

    return result;
}

bool Script_Base::UseManaPotion()
{
    bool result = false;

    if (potionDelay < 0)
    {
        potionDelay = 2000;
        if (!me)
        {
            return false;
        }
        if (!me->IsInCombat())
        {
            return false;
        }
        uint32 itemEntry = 0;
        uint32 myLevel = me->getLevel();
        if (myLevel >= 49)
        {
            itemEntry = 13444;
        }
        else if (myLevel >= 41)
        {
            itemEntry = 13443;
        }
        else if (myLevel >= 31)
        {
            itemEntry = 6149;
        }
        else if (myLevel >= 22)
        {
            itemEntry = 3827;
        }
        else if (myLevel >= 14)
        {
            itemEntry = 3385;
        }
        else
        {
            itemEntry = 2455;
        }
        if (!me->HasItemCount(itemEntry, 1))
        {
            me->StoreNewItemInBestSlots(itemEntry, 20);
        }
        Item* pItem = GetItemInInventory(itemEntry);
        if (pItem && !pItem->IsInTrade())
        {
            if (UseItem(pItem, me))
            {
                potionDelay = 120000;
                result = true;
            }
        }

    }

    return result;
}

void Script_Base::ChooseTarget(Unit* pmTarget)
{
    if (pmTarget)
    {
        if (me)
        {
            me->SetSelection(pmTarget->GetGUID());
            me->SetTarget(pmTarget->GetGUID());
        }
    }
}

void Script_Base::ClearTarget()
{
    if (me)
    {
        me->SetSelection(ObjectGuid::Empty);
        me->SetTarget(ObjectGuid::Empty);
    }
}
