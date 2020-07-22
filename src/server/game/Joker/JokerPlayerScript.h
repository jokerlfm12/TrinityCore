#ifndef JOKER_PLAYER_SCRIPT_H
#define JOKER_PLAYER_SCRIPT_H

#include "Common.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"

class Player;

class TC_GAME_API JokerPlayerScript : public PlayerScript
{
public:
    JokerPlayerScript();

    // Player Hooks
    void OnCreate(Player* player) override;
};

void AddSC_JokerPlayerScript();

#endif
