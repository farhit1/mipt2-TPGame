#include "resources.h"

std::vector<std::string> Resources::names;
std::vector<int> Resources::costs;
int Resources::size = 0;

void Resources::add(std::string name, int cost) {
    names.push_back(name);
    costs.push_back(cost);
    ++size;
}

Resources::Resources() :
        number(std::vector<int>(size, 0)) {}

Resources::Resources(const std::vector<int> &number) :
        number(number) {}

bool Resources::operator>=(const Resources &other) const {
    for (size_t i = 0; i < size; i++)
        if (number[i] < other.number[i])
            return false;
    return true;
}

void Resources::operator-=(const Resources &other) {
    for (size_t i = 0; i < size; i++)
        number[i] -= other.number[i];
}

void Resources::operator+=(const Resources &other) {
    for (size_t i = 0; i < size; i++)
        number[i] += other.number[i];
}

int Resources::cost() const {
    int cost = 0;
    for (size_t i = 0; i < size; i++)
        cost += costs[i] * number[i];
    return cost;
}

ResourceAdder::ResourceAdder(const std::string &name, int cost) {
    Resources::add(name, cost);
}

Resources* ResourcesCopy(const Resources *resources) {
    return new Resources(resources->number);
}

std::string Resources::toString() const {
    std::string res;
    for (size_t i = 0; i < size; i++) {
        res += names[i] + ": " + std::to_string(number[i]);
        if (i != size - 1)
            res += ", ";
    }
    return res;
}

std::string ResourcesToString(const Resources* resources) {
    return resources->toString();
}