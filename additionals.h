#pragma once

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s);

std::string lineG(int fill, int total);

std::string frame(const std::string &s);

class Time {
    static int _day;
public:
    static void nextDay();

    static int getDay();
};