#pragma once

class Player;
class Unit;

#include <vector>
#include <string>

void StartMoveProcessing(Player *player);
void UnitDailyNeedsProcessing(Unit *unit, float covering);

enum class ActionTypeId { UnitDailyNeedsId, StartMoveId, EndMoveId, OwnActionId };

class Action {
public:
    virtual ActionTypeId id();      // action id
    virtual void process() = 0;     // processing action

    // ONLY FOR CONSOLE VERSION
    // trying to parse query and form action
    virtual Action *parseQuery(Player *, const std::vector<std::string> &) = 0;

    Action();
};


// list of available actions to user
extern std::vector<Action *> actions;
// storing available actions to list above
class StoreAction {
public:
    explicit StoreAction(Action *action);
};


// some base actions

class UnitDailyNeeds : public Action {
    using Action::Action;
public:
    Unit *unit;
    float covering;

    UnitDailyNeeds(Unit *unit, float covering);

    Action *parseQuery(Player *p, const std::vector<std::string> &x);

    void process();
};

class StartMove : public Action {
    using Action::Action;
public:
    Player *player;

    StartMove(Player *player);

    Action *parseQuery(Player *p, const std::vector<std::string> &s);

    void process();
};

class EndMove : public Action {
    using Action::Action;
public:
    Player *player;

    ActionTypeId id();

    EndMove(Player *player);

    Action *parseQuery(Player *p, const std::vector<std::string> &s);

    void process();
};
