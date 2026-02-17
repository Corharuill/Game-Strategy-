#include "BattleEnemy.h"

Enemy::Enemy(int w, int h, int inf, int cav) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<> dx(1, w - 2);
    std::uniform_int_distribution<> dy(1, h - 2);

    x = dx(gen);
    y = dy(gen);

    experience = inf * 5 + cav * 8;
    gold = inf * 2 + cav * 5;

    auto infA = createInfantryArmy(inf);
    auto cavA = createCavalryArmy(cav);

    for (auto& u : infA) army.units.push_back(std::move(u));
    for (auto& u : cavA) army.units.push_back(std::move(u));
}