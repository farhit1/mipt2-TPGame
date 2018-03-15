#pragma once

#include <iostream>
#include <vector>
#include <map>

class Player;

#include "additionals.h"
#include "player.h"

int playersAlive(const std::vector<Player *> &players);

// game launcher
void launch(const int selfPlayers, const int autoPlayers = 0);
