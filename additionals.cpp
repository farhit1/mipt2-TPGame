#include "additionals.h"

char buf[1024];

std::vector<std::string> split(const std::string &s) {
    std::vector<std::string> result;
    std::string word;
    for (char c : s)
        if (c == ' ') {
            if (!word.empty()) {
                result.push_back(word);
                word = "";
            }
        } else
            word += c;
    if (!word.empty())
        result.push_back(word);
    return result;
}

std::string lineG(int fill, int total) {
    std::string s(total / 10, ' ');
    for (size_t i = 0; i < fill / 10; ++i)
        s[i] = '=';
    s = '[' + s + ']';
    return s;
}

std::string frame(const std::string& s) {
    static const char c = '#';
    return std::string(s.length() + 4, c) + '\n' + c + ' ' + s + ' ' + c + '\n' + std::string(s.length() + 4, c);
}

int Time::_day = 1;

void Time::nextDay() {
    ++_day;
}

int Time::getDay() {
    return _day;
}