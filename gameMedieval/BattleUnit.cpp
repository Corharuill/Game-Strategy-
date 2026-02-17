#include "BattleUnit.h"

// ---------- Infantry ----------
Infantry::Infantry() {
    attack = 1;
    protection = 2;
    health = 10;
}

Infantry::Infantry(int complexity) {
    attack = 2 * complexity;
    protection = 2 * complexity;
    health = 10 * complexity;
}

// ---------- Cavalry ----------
Cavalry::Cavalry() {
    attack = 3;
    protection = 3;
    health = 15;
}

// ---------- Factories ----------
std::unique_ptr<Unit> createInfantryUnit(int complexity) {
    return std::make_unique<Infantry>(complexity);
}

std::unique_ptr<Unit> createCavalryUnit() {
    return std::make_unique<Cavalry>();
}

std::vector<std::unique_ptr<Unit>> createInfantryArmy(int count, int complexity) {
    std::vector<std::unique_ptr<Unit>> army;
    army.reserve(count);
    for (int i = 0; i < count; ++i)
        army.push_back(createInfantryUnit(complexity));
    return army;
}

std::vector<std::unique_ptr<Unit>> createCavalryArmy(int count) {
    std::vector<std::unique_ptr<Unit>> army;
    army.reserve(count);
    for (int i = 0; i < count; ++i)
        army.push_back(createCavalryUnit());
    return army;
}