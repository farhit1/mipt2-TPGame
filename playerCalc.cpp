#include "playerCalc.h"

const int PlayerCalc::initialPopulation = 100;
const int PlayerCalc::initialCoins = 1000;
const Resources* PlayerCalc::initialResources = new Resources(std::vector<int>({0, 0}));
const int PlayerCalc::initialTax = 2;
const int PlayerCalc::initialTower = 500;
const int PlayerCalc::towerDefence = 70;

float PlayerCalc::populationChange(int tax) {
    return std::max(0.0, 1.4 - tax * 0.15);
}
