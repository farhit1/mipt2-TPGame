#include "player.h"
#include "action.h"

std::map<std::string, Player *> players;

Player::Player() :
        name("player" + std::to_string(++players)),
        population(PlayerCalc::initialPopulation),
        coins(PlayerCalc::initialCoins),
        resources(ResourcesCopy(PlayerCalc::initialResources)),
        tax(PlayerCalc::initialTax),
        tower(PlayerCalc::initialTower) {
    ::players.insert({name, this});
}

Action *Player::parseMove() const {
    Action *newAction = nullptr;
    std::vector<std::string> move = getMove();
    for (auto i : ::actions) {
        newAction = ActionParseQuery(i, const_cast<Player *>(this), move);
        if (newAction != nullptr)
            return newAction;
    }
    return newAction;
}

void Player::makeMove() {
    Action *startMove = newStartMove(this);
    startMove->process();

    for (Unit *unit : units)
        makeUnused(unit);

    while (!isDead()) {
        Action *action = parseMove();
        if (action == nullptr) {
            badMove();
            continue;
        }
        if (ActionId(action) == ActionTypeId::EndMoveId)
            return;
        ActionProcess(action);
    }
}

void Player::badMove() const {
    notify("incorrect move");
}

bool Player::isDead() const {
    return tower == 0;
}

std::string Player::info() const {
    return "player info is called";
}

void StartMoveProcessing(Player* player) {
    player->notify(frame(player->name + ", now it's your turn (day " + std::to_string(Time::getDay()) + ")"));

    player->population = (1 + player->population) * PlayerCalc::populationChange(player->tax);
    player->coins += player->population * player->tax;
    int neededCoins = 0;
    for (Unit *i : player->units)
        neededCoins += getDailyNeeds(i);

    float covering = std::min(1.0, (double) player->coins / neededCoins);
    for (Unit *i : player->units)
        ActionProcess(newUnitDailyNeeds(i, covering));
}

void PlayerChangeCoins(Player *player, int change) {
    player->coins += change;
}

int Player::players = 0;

std::vector<std::string> AutoPlayer::getMove() const {
    // to do
    return {"end"};
}

void AutoPlayer::notify(std::string s) const {}

std::vector<std::string> SelfPlayer::getMove() const {
    std::string ret;
    std::cout << name + " > ";
    getline(std::cin, ret);
    return split(ret);
}

void SelfPlayer::notify(std::string s) const {
    std::cout << s << std::endl;
}

size_t Player::aliveUnits() const {
    int cnt = 0;
    for (Unit* unit : units)
        if (!isUnitDead(unit))
            ++cnt;
    return cnt;
}
