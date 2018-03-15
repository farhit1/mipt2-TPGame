#pragma once

#include <vector>
#include <string>

class Resources {
public:
    static std::vector<std::string> names;
    static std::vector<int> costs;
    static int size;

    std::vector<int> number;

    static void add(std::string name, int cost);

    Resources();

    Resources(const std::vector<int> &number);

    bool operator>=(const Resources &other) const;

    void operator-=(const Resources &other);

    void operator+=(const Resources &other);

    int cost() const;

    std::string toString() const;
};

class ResourceAdder {
public:
    ResourceAdder(const std::string &name, int cost);
};
