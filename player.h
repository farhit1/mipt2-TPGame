#pragma once

class Unit;
class Resources;
class Action;
class Player;
enum class ActionTypeId;

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "playerCalc.h"
#include "additionals.h"

int getDailyNeeds(Unit* unit);
void makeUnused(Unit* unit);
Resources* ResourcesCopy(const Resources* resources);
extern std::vector<Action *> actions;
Action* ActionParseQuery(Action* action, Player *player, const std::vector<std::string> &x);
Action* newStartMove(Player* player);
ActionTypeId ActionId(Action* action);
void ActionProcess(Action* action);
Action* newUnitDailyNeeds(Unit* unit, float covering);
bool isUnitDead(Unit* unit);

class Player {
public:
    const std::string name;       // player's name

    std::vector<Unit *> units;    // player's units

    int population;               // number of player's population
    int coins;                    // number of player's coins
    Resources* resources;          // player's resources
    int tax;                      // coins from 1 inhabitant per move
    int tower;                    // power of tower
    static int players;           // number of players created

    virtual std::vector<std::string> getMove() const = 0;

    Player();

    Action *parseMove() const;

    void makeMove();

    void badMove() const;

    bool isDead() const;

    std::string info() const;

    // notify player about actions
    virtual void notify(std::string) const = 0;

    size_t aliveUnits() const;
};

extern std::map<std::string, Player *> players;    // access to player by name


class AutoPlayer : public Player {
    using Player::Player;
public:
    std::vector<std::string> getMove() const;

    void notify(std::string s) const;
};

class SelfPlayer : public Player {
    using Player::Player;
public:
    std::vector<std::string> getMove() const;

    void notify(std::string s) const;
};
