#pragma once

class Resources;
class Player;

#include <vector>
#include <string>
#include <map>

#include "additionals.h"

extern char buf[1024];

void PlayerChangeCoins(Player *player, int change);
std::string ResourcesToString(const Resources* resources);

class Unit {
public:
    virtual const int &coins() const = 0;                 // needed coins to create a unit
    virtual const int &people() const = 0;                // needed inhabitants to create a unit
    virtual const Resources* resources() const = 0;       // needed resources to create a unit
    virtual const int &dailyNeeds() const = 0;            // daily needs to support unit
    virtual const int &initialHealth() const = 0;         // initial health of unit
    virtual const int &outDamage() const = 0;             // outcoming damage of unit per turn
    virtual const int &inDamage() const = 0;              // damage to attacking
    virtual const float &moneylessDamage() const = 0;     // damage per one missed coin if unit didn't get enough daily fee
    virtual const int &towerAttack() const = 0;           // tower damage
    virtual const std::string &unitClass() const = 0;     // unit class name
    virtual const std::string &unitDesc() const = 0;      // beautiful unit description

    virtual int &unitsOfTypeCreated() = 0;        // number of units

    const std::string unitName;        // unit name
    int health;                        // health of unit
    bool thisTurnUsed;                 // is unit used this turn

    Player *owner;    // unit owner

    Unit();

    Unit(const std::string &, const int &, Player *);

    int healingCost() const;

    std::string createInfo() const;

    std::string info() const;

    bool isDead() const;

    virtual Unit *clone(Player *) = 0;
};

extern std::map<std::string, Unit *> units;        // access to unit by name


// list of available units
class StoreUnit {
public:
    StoreUnit(Unit* unit);
};


// store all the constants for each unit only once
template<typename UnitType>
class UnitCRTP : virtual public Unit {
    using Unit::Unit;
public:
    static const int _coins;

    const int &coins() const { return _coins; }

    static const int _people;

    const int &people() const { return _people; }

    static const Resources* _resources;

    const Resources* resources() const { return _resources; }

    static const int _dailyNeeds;

    const int &dailyNeeds() const { return _dailyNeeds; }

    static const int _initialHealth;

    const int &initialHealth() const { return _initialHealth; }

    static const int _outDamage;

    const int &outDamage() const { return _outDamage; }

    static const int _inDamage;

    const int &inDamage() const { return _inDamage; }

    static const float _moneylessDamage;

    const float &moneylessDamage() const { return _moneylessDamage; }

    static const int _towerAttack;

    const int &towerAttack() const { return _towerAttack; }

    static const std::string _unitClass;

    const std::string &unitClass() const { return _unitClass; }

    static const std::string _unitDesc;

    const std::string &unitDesc() const { return _unitDesc; }

    static int _unitsOfTypeCreated;

    int &unitsOfTypeCreated() { return _unitsOfTypeCreated; }

    UnitCRTP(Player *owner) :
            Unit(unitClass() + std::to_string(++unitsOfTypeCreated()), initialHealth(), owner) {}

    UnitCRTP() {}

    Unit *clone(Player *owner) {
        return new UnitType(UnitType::unitClass() + std::to_string(++unitsOfTypeCreated()), UnitType::initialHealth(), owner);
    }
};

template<typename UnitType> int UnitCRTP<UnitType>::_unitsOfTypeCreated = 0;

extern std::map<std::string, Unit *> unitList;
