#pragma once

#include <vector>

#include "resources.h"

class PlayerCalc {
public:
    static const int initialPopulation;
    static const int initialCoins;
    static const Resources* initialResources;
    static const int initialTax;
    static const int initialTower;
    static const int towerDefence;

    // population change depending on tax
    static float populationChange(int);
};
