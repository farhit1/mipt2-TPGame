#include "action.h"

ActionTypeId Action::id() {
    return ActionTypeId::OwnActionId;
}

Action::Action() {}

std::vector<Action *> actions;


StoreAction::StoreAction(Action *action) {
    actions.push_back(action);
}

UnitDailyNeeds::UnitDailyNeeds(Unit *unit, float covering) :
        unit(unit),
        covering(covering) {}

Action *UnitDailyNeeds::parseQuery(Player *p, const std::vector<std::string> &x) { return nullptr; }

void UnitDailyNeeds::process() {
    UnitDailyNeedsProcessing(unit, covering);
}

StartMove::StartMove(Player *player) :
        player(player) {}

Action *StartMove::parseQuery(Player *p, const std::vector<std::string> &s) { return nullptr; }

void StartMove::process() {
    StartMoveProcessing(player);
}

EndMove::EndMove(Player *player) :
        player(player) {}

Action *EndMove::parseQuery(Player *p, const std::vector<std::string> &s) {
    if (s.size() == 1 && s[0] == "end") {
        return new EndMove(p);
    }
    return nullptr;
}

void EndMove::process() {}

Action* ActionParseQuery(Action* action, Player *p, const std::vector<std::string> &x) {
    return action->parseQuery(p, x);
}

Action* newStartMove(Player* player) {
    return new StartMove(player);
}

ActionTypeId ActionId(Action* action) {
    return action->id();
}

void ActionProcess(Action* action) {
    action->process();
}

Action* newUnitDailyNeeds(Unit* unit, float covering) {
    return new UnitDailyNeeds(unit, covering);
}

ActionTypeId endMoveId() {
    return ActionTypeId::EndMoveId;
}

StoreAction endMoveActionStored(new EndMove());

ActionTypeId EndMove::id() {
    return ActionTypeId::EndMoveId;
}