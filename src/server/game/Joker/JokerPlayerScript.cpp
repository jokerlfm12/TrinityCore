#include "JokerPlayerScript.h"
#include "Player.h"
#include "Item.h"
#include "JokerConfig.h"

JokerPlayerScript::JokerPlayerScript() : PlayerScript("JokerPlayerScript") { }

void JokerPlayerScript::OnCreate(Player* player)
{
    if (sJokerConfig->Enable)
    {
        uint32 startLevel = 1;
        uint32 startMoney = 0;
        switch (sJokerConfig->NewPlayerExpansion)
        {
        case 0:
        {
            break;
        }
        case 1:
        {
            startLevel = 60;
            startMoney = 10000000;
            break;
        }
        case 2:
        {
            startLevel = 70;
            startMoney = 20000000;
            break;
        }
        case 3:
        {
            startLevel = 80;
            startMoney = 30000000;
            break;
        }
        default:
        {
            break;
        }
        }
        if (startLevel > 1)
        {
            player->GiveLevel(startLevel);
            player->ModifyMoney(startMoney);
            player->setCinematic(1);
            if (Item* hearthStone = player->GetItemByEntry(6948))
            {
                player->DestroyItem(hearthStone->GetBagSlot(), hearthStone->GetSlot(), true);
            }
            WorldLocation wl;
            uint32 playerRace = player->getRace();
            if (playerRace == Races::RACE_HUMAN || playerRace == Races::RACE_DWARF || playerRace == Races::RACE_NIGHTELF || playerRace == Races::RACE_GNOME || playerRace == Races::RACE_DRAENEI || playerRace == Races::RACE_WORGEN)
            {
                if (playerRace == Races::RACE_WORGEN)
                {
                    player->CastSpell(player, 95758, true);
                    player->CastSpell(player, 72792, true);
                    player->LearnSpell(68996, false);
                    player->LearnSpell(68992, false);
                }
                wl.m_mapId = 0;
                wl.m_positionX = -8826.0f;;
                wl.m_positionY = 628.6f;
                wl.m_positionZ = 94.4f;
                wl.SetOrientation(0.76f);
            }
            else if (playerRace == Races::RACE_ORC || playerRace == Races::RACE_UNDEAD_PLAYER || playerRace == Races::RACE_TAUREN || playerRace == Races::RACE_TROLL || playerRace == Races::RACE_GOBLIN || playerRace == Races::RACE_BLOODELF)
            {
                wl.m_mapId = 0;
                wl.m_positionX = 1598.0f;;
                wl.m_positionY = -4371.5f;
                wl.m_positionZ = 21.2f;
                wl.SetOrientation(0.5f);
            }
            player->RemoveAtLoginFlag(AtLoginFlags::AT_LOGIN_FIRST);
            player->WorldRelocate(wl);
            player->SaveToDB();
        }
    }
}

void AddSC_JokerPlayerScript()
{
    new JokerPlayerScript();
}
