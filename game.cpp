#include "game.h"

int playersAlive(const std::vector<Player *> &players) {
    int cnt = 0;
    for (Player *player : players)
        if (!player->isDead())
            ++cnt;
    return cnt;
}

// game launcher
void launch(const int selfPlayers, const int autoPlayers) {
    std::vector<Player*> p;
    for (size_t _ = 0; _ < selfPlayers; ++_)
        p.push_back(new SelfPlayer());
    for (size_t _ = 0; _ < autoPlayers; ++_)
        p.push_back(new AutoPlayer());

    Player* last;
    while (true) {
        for (Player* i : p)
            if (!i->isDead()) {
                last = i;
                i->makeMove();
            }
        if (playersAlive(p) == 1) {
            last->notify(frame(last->name + ", you won in " + std::to_string(Time::getDay()) + " days"));
            break;
        }
        Time::nextDay();
    }
}
