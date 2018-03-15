#include "unit.h"
#include "resources.h"

class Soldier : public UnitCRTP<Soldier> {
    using UnitCRTP<Soldier>::UnitCRTP;
};

template<> const int UnitCRTP<Soldier>::_coins = 150;
template<> const int UnitCRTP<Soldier>::_people = 1;
template<> const Resources* UnitCRTP<Soldier>::_resources = new Resources(std::vector<int>({0, 0}));
template<> const int UnitCRTP<Soldier>::_dailyNeeds = 10;
template<> const int UnitCRTP<Soldier>::_initialHealth = 100;
template<> const int UnitCRTP<Soldier>::_outDamage = 40;
template<> const int UnitCRTP<Soldier>::_inDamage = 30;
template<> const float UnitCRTP<Soldier>::_moneylessDamage = 0.20;
template<> const int UnitCRTP<Soldier>::_towerAttack = 10;
template<> const std::string UnitCRTP<Soldier>::_unitClass = "soldier";
template<> const std::string UnitCRTP<Soldier>::_unitDesc = "Just a cannon fodder";
StoreUnit soldierUnitStored(new Soldier());



class Horseman : public UnitCRTP<Horseman> {
    using UnitCRTP<Horseman>::UnitCRTP;
};

template<> const int UnitCRTP<Horseman>::_coins = 350;
template<> const int UnitCRTP<Horseman>::_people = 1;
template<> const Resources* UnitCRTP<Horseman>::_resources = new Resources(std::vector<int>({1, 0}));
template<> const int UnitCRTP<Horseman>::_dailyNeeds = 15;
template<> const int UnitCRTP<Horseman>::_initialHealth = 150;
template<> const int UnitCRTP<Horseman>::_outDamage = 50;
template<> const int UnitCRTP<Horseman>::_inDamage = 25;
template<> const float UnitCRTP<Horseman>::_moneylessDamage = 0.25;
template<> const int UnitCRTP<Horseman>::_towerAttack = 20;
template<> const std::string UnitCRTP<Horseman>::_unitClass = "horseman";
template<> const std::string UnitCRTP<Horseman>::_unitDesc = "Man on a horse";
StoreUnit horsemanUnitStored(new Horseman());


class Archer : public UnitCRTP<Archer> {
    using UnitCRTP<Archer>::UnitCRTP;
};

template<> const int UnitCRTP<Archer>::_coins = 250;
template<> const int UnitCRTP<Archer>::_people = 1;
template<> const Resources* UnitCRTP<Archer>::_resources = new Resources(std::vector<int>({0, 0}));
template<> const int UnitCRTP<Archer>::_dailyNeeds = 10;
template<> const int UnitCRTP<Archer>::_initialHealth = 100;
template<> const int UnitCRTP<Archer>::_outDamage = 70;
template<> const int UnitCRTP<Archer>::_inDamage = 25;
template<> const float UnitCRTP<Archer>::_moneylessDamage = 0.20;
template<> const int UnitCRTP<Archer>::_towerAttack = 40;
template<> const std::string UnitCRTP<Archer>::_unitClass = "archer";
template<> const std::string UnitCRTP<Archer>::_unitDesc = "Man with a bow";
StoreUnit archerUnitStored(new Archer());
