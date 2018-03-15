#include <algorithm>
#include <fstream>

#include "action.h"
#include "player.h"
#include "unit.h"
#include "resources.h"
#include "additionals.h"


// unit->unit attack
class Attack : public Action {
    using Action::Action;
public:
    Player* attacking;
    std::string attackFromName;
    std::string attackToName;

    Attack(Player* attacking, const std::string& attackFromName, const std::string& attackToName) :
            attacking(attacking),
            attackFromName(attackFromName),
            attackToName(attackToName) {}

    Action *parseQuery(Player *attacking, const std::vector<std::string> &s) {
        if (s.size() != 3 || s[0] != "attack")
            return nullptr;
        return new Attack(attacking, s[1], s[2]);
    }

    void process() {
        if (units.find(attackFromName) == units.end()) {
            attacking->notify("no such unit to attack from");
            return;
        }
        if (units.find(attackToName) == units.end()) {
            attacking->notify("no such unit to attack to");
            return;
        }
        Unit *attackFrom = units[attackFromName];
        Unit *attackTo = units[attackToName];
        if (attackFrom->owner->name != attacking->name) {
            attacking->notify("this is not your unit");
            return;
        }
        if (attackTo->owner->name == attacking->name) {
            attacking->notify("you can't attack yourself");
            return;
        }
        if (attackFrom->isDead()) {
            attacking->notify("unit you attacking with is dead");
            return;
        }
        if (attackTo->isDead()) {
            attacking->notify("unit you attacking to is dead");
            return;
        }
        if (attackFrom->thisTurnUsed) {
            attacking->notify("this unit was already used this turn");
            return;
        }
        attackTo->health = std::max(0, attackTo->health - attackFrom->outDamage());
        attackFrom->health = std::max(0, attackFrom->health - attackTo->inDamage());
        attackFrom->thisTurnUsed = true;
    }

    ~Attack() = default;
};

StoreAction attackActionStored(new Attack());


// unit->tower attack
class AttackTower : public Action {
    using Action ::Action;
public:
    Player *to;
    std::string attackFromName;
    std::string attackToName;

    AttackTower(Player *to, const std::string& attackFromName, const std::string& attackToName) :
            to(to),
            attackFromName(attackFromName),
            attackToName(attackToName) {}

    Action *parseQuery(Player *attacking, const std::vector<std::string> &s) {
        if (s.size() != 3 || s[0] != "tower")
            return nullptr;
        return new AttackTower(attacking, s[1], s[2]);
    }

    void process() {
        if (players.find(attackToName) == players.end()) {
            to->notify("no such player");
            return;
        }
        if (units.find(attackFromName) == units.end()) {
            to->notify("no such unit to attack from");
            return;
        }
        Unit *attackFrom = units[attackFromName];
        Player *attackTo = players[attackToName];
        if (attackTo->name == to->name) {
            to->notify("you can't attack yourself");
            return;
        }
        if (attackFrom->owner->name != to->name) {
            to->notify("this is not your unit");
            return;
        }
        if (attackTo->aliveUnits() != 0) {
            to->notify("you can't attack tower with alive units");
            return;
        }
        if (attackFrom->isDead()) {
            to->notify("unit you attacking with is dead");
            return;
        }
        if (attackFrom->thisTurnUsed) {
            to->notify("this unit was already used this turn");
            return;
        }
        if (attackTo->isDead()) {
            to->notify("this player is already dead");
            return;
        }
        attackFrom->health = std::max(0, attackFrom->health - PlayerCalc::towerDefence);
        attackTo->tower = std::max(0, attackTo->tower - attackFrom->towerAttack());
        attackFrom->thisTurnUsed = true;
    }

    ~AttackTower() = default;
};

StoreAction attackTowerActionStored(new AttackTower());


// purchasing units
class PurchaseUnit : public Action {
    using Action::Action;
public:
    Player *player;
    std::string unitName;

    PurchaseUnit(Player *player, std::string unitName) :
            player(player),
            unitName(unitName) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 3 || s[0] != "buy" || s[1] != "unit")
            return nullptr;
        return new PurchaseUnit(p, s[2]);
    }

    void process() {
        if (unitList.find(unitName) == unitList.end()) {
            player->notify("no such unit class");
            return;
        }
        Unit* unit = unitList[unitName];
        if (unit->coins() > player->coins) {
            player->notify("not enough money");
            return;
        }
        if (!(*player->resources >= *unit->resources())) {
            player->notify("not enough resources");
            return;
        }
        if (player->population < unit->people()) {
            player->notify("not enough people");
            return;
        }
        player->population -= unit->people();
        player->coins -= unit->coins();
        *player->resources -= *unit->resources();
        player->units.push_back(unit->clone(player));
    }

    ~PurchaseUnit() = default;
};

StoreAction purchaseUnitActionStored(new PurchaseUnit());


// purchasing resources
class PurchaseResources : public Action {
    using Action::Action;
public:
    Player *player;
    std::string resourceName;

    PurchaseResources(Player *player, std::string resourceName) :
            player(player),
            resourceName(resourceName) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 3 || s[0] != "buy" || s[1] != "resource")
            return nullptr;
        return new PurchaseResources(p, s[2]);
    }

    void process() {
        size_t pos = std::find(Resources::names.begin(), Resources::names.end(), resourceName) - Resources::names.begin();
        if (pos == Resources::size) {
            player->notify("no such resource");
            return;
        }
        if (Resources::costs[pos] > player->coins) {
            player->notify("not enough money");
            return;
        }
        player->coins -= Resources::costs[pos];
        player->resources->number[pos]++;
    }

    ~PurchaseResources() = default;
};

StoreAction purchaseResourcesActionStored(new PurchaseResources());


// setting tax
class SetTax : public Action {
    using Action::Action;
public:
    Player *player;
    int newTax;

    SetTax(Player *player, int newTax) :
            player(player),
            newTax(newTax) {}

    bool isANumber(const std::string &s) {
        if (s.empty() || s == "-")
            return false;
        int i = 0;
        if (s[0] == '-')
            ++i;
        for (; i < s.length(); i++)
            if (s[i] < '0' || s[i] > '9')
                return false;
        return true;
    }

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 2 || s[0] != "tax")
            return nullptr;
        if (!isANumber(s[1]))
            return nullptr;
        return new SetTax(p, std::stoi(s[1]));
    }

    void process() {
        player->tax = newTax;
    }

    ~SetTax() = default;
};

StoreAction setTaxActionStored(new SetTax());


// print manual
class Help : public Action {
    using Action::Action;
public:
    Player *player;

    Help(Player *player) :
            player(player) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 1 || s[0] != "help")
            return nullptr;
        return new Help(p);
    }

    void process() {
        std::ifstream input("help.txt");
        for (std::string line; getline( input, line );) {
            player->notify(line);
        }
        input.close();
    }

    ~Help() = default;
};

StoreAction helpActionStored(new Help());


// player statistics
class Info : public Action {
    using Action::Action;
public:
    std::string playerName;
    Player *to;

    Info(Player *to, std::string playerName) :
            playerName(playerName),
            to(to) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() < 1 || s[0] != "info")
            return nullptr;
        if (s.size() == 1)
            return new Info(p, p->name);
        if (s.size() > 2)
            return nullptr;
        return new Info(p, s[1]);
    }

    std::string popchange(Player *player) {
        int res = PlayerCalc::populationChange(player->tax) * (player->population + 1) - player->population;
        return (res < 0 ? "-" : "+") + std::to_string(abs(res));
    }

    std::string coinschange(Player *player) {
        int newPop = PlayerCalc::populationChange(player->tax) * (player->population + 1);
        int res = newPop * player->tax;
        return (res < 0 ? "-" : "+") + std::to_string(abs(res));
    }

    std::string usedTurn(Unit* unit) {
        if (unit->thisTurnUsed)
            return "-";
        else
            return "+";
    }

    void process() {
        if (players.find(playerName) == players.end()) {
            to->notify("no such player");
            return;
        }
        Player *player = players[playerName];
        if (player->isDead()) {
            to->notify("dead");
            return;
        }
        to->notify("Money:       " + std::to_string(player->coins) + " (" + coinschange(player) + ")");
        to->notify("Population:  " + std::to_string(player->population) + " (" + popchange(player) + ")");
        to->notify("Tax:         " + std::to_string(player->tax));
        to->notify("Tower:       " + lineG(player->tower, PlayerCalc::initialTower) + " (" +
                               std::to_string(player->tower) + "/" + std::to_string(PlayerCalc::initialTower) + ")");
        to->notify("Resources:   " + player->resources->toString());
        to->notify("Alive units (" + std::to_string(player->aliveUnits()) + ")");
        for (Unit* unit : player->units)
            if (!unit->isDead())
                to->notify("  [" + usedTurn(unit) + "] " + unit->info());
    }

    ~Info() = default;
};

StoreAction infoActionStored(new Info());


// unit class info
class UnitInfo : public Action {
    using Action::Action;
public:
    Player *to;
    std::string unitName;

    UnitInfo(Player *to, std::string unitName) :
            to(to),
            unitName(unitName) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 2 || s[0] != "unit")
            return nullptr;
        return new UnitInfo(p, s[1]);
    }

    void process() {
        if (unitList.find(unitName) == unitList.end()) {
            to->notify("no such unit class");
            return;
        }
        Unit *about = unitList[unitName];
        to->notify(about->unitClass() + ":");
        to->notify(about->unitDesc());
        to->notify("Initial health       : " + std::to_string(about->initialHealth()));
        to->notify("Daily needs          : " + std::to_string(about->dailyNeeds()));
        to->notify("Out unit damage  (->): " + std::to_string(about->outDamage()));
        to->notify("Out tower damage (->): " + std::to_string(about->towerAttack()));
        to->notify("In damage        (<-): " + std::to_string(about->inDamage()));
        to->notify("To buy it you need");
        to->notify("  Money:      " + std::to_string(about->coins()));
        to->notify("  Resources:  " + about->resources()->toString());
    }

    ~UnitInfo() = default;
};

StoreAction unitInfoActionStored(new UnitInfo());


// heal unit
class Heal : public Action {
    using Action::Action;
public:
    std::string unitName;
    Player *player;

    Heal(std::string unitName, Player *player) :
            unitName(unitName),
            player(player) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 2 || s[0] != "heal")
            return nullptr;
        return new Heal(s[1], p);
    }

    void process() {
        if (units.find(unitName) == units.end()) {
            player->notify("no such unit");
            return;
        }
        Unit* unit = units[unitName];
        if (unit->healingCost() > player->coins) {
            player->notify("not enough money");
            return;
        }
        if (unit->isDead()) {
            player->notify("dead units couldn't be healed");
            return;
        }
        if (unit->owner != player) {
            player->notify("this is not your unit");
            return;
        }
        player->coins -= unit->healingCost();
        unit->health = unit->initialHealth();
    }

    ~Heal() = default;
};

StoreAction healActionStored(new Heal());


// prints list of unit classes
class Units : public Action {
    using Action::Action;
public:
    Player *player;

    Units(Player *player) :
            player(player) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 1 || s[0] != "units")
            return nullptr;
        return new Units(p);
    }

    void process() {
        player->notify("Available units (" + std::to_string(unitList.size()) + ")");
        for (auto &i : unitList)
            player->notify(i.first);
    }

    ~Units() = default;
};

StoreAction unitsActionStored(new Units());


// prints cost of resource
class ResourceCost : public Action {
    using Action::Action;
public:
    Player *player;
    std::string resourceName;

    ResourceCost(Player *player, const std::string &resourceName) :
            player(player),
            resourceName(resourceName) {}

    Action *parseQuery(Player *p, const std::vector<std::string> &s) {
        if (s.size() != 2 || s[0] != "resource")
            return nullptr;
        return new ResourceCost(p, s[1]);
    }

    void process() {
        size_t pos = std::find(Resources::names.begin(), Resources::names.end(), resourceName) - Resources::names.begin();
        if (pos == Resources::size) {
            player->notify("no such resource");
            return;
        }
        player->notify(std::to_string(Resources::costs[pos]));
    }

    ~ResourceCost() = default;
};

StoreAction resourceCostActionStored(new ResourceCost());
