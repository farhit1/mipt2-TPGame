#include "unit.h"

#include <iostream>

std::map<std::string, Unit *> unitList;
std::map<std::string, Unit *> units;

StoreUnit::StoreUnit(Unit* unit) {
    unitList.insert({unit->unitClass(), unit});
}

int Unit::healingCost() const {
    return coins() * (initialHealth() - health) / initialHealth();
}

std::string Unit::createInfo() const {
    sprintf(buf, "To create %s you need %d coins, %d people and other resources: %s", unitClass().c_str(),
            coins(), people(), ResourcesToString(resources()).c_str());
    return std::string(buf);
}

std::string Unit::info() const {
    sprintf(buf, "%s (%s) | health: %s (%d/%d, healing costs %d)", unitClass().c_str(), unitName.c_str(),
            lineG(health, initialHealth()).c_str(), health, initialHealth(), healingCost());
    return std::string(buf);
}

bool Unit::isDead() const {
    return health == 0;
}

Unit::Unit() = default;

Unit::Unit(const std::string &unitName, const int &health, Player *owner) :
        unitName(unitName),
        health(health),
        thisTurnUsed(true),
        owner(owner) {
    units.insert({unitName, this});
}

void UnitDailyNeedsProcessing(Unit *unit, float covering) {
    int cover = unit->dailyNeeds() * covering;
    PlayerChangeCoins(unit->owner, -cover);
    unit->health -= (unit->dailyNeeds() - cover) * unit->moneylessDamage();
}

void makeUnused(Unit* unit) {
    unit->thisTurnUsed = false;
}

int getDailyNeeds(Unit* unit) {
    return unit->dailyNeeds();
}

bool isUnitDead(Unit* unit) {
    return unit->isDead();
}
