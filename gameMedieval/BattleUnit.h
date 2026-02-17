#pragma once
#include <memory>
#include <vector>

// ---------- ﬁÕ»“€ ----------
class Unit {
public:
    int attack = 0;
    int protection = 0;
    int health = 0;
    virtual ~Unit() = default;
};

class Infantry : public Unit {
public:
    Infantry();
    Infantry(int complexity);
};

class Cavalry : public Unit {
public:
    Cavalry();
};

class Army {
public:
    std::vector<std::unique_ptr<Unit>> units;
};


std::unique_ptr<Unit> createInfantryUnit(int complexity = 1);
std::unique_ptr<Unit> createCavalryUnit();
std::vector<std::unique_ptr<Unit>> createInfantryArmy(int count, int complexity = 1);
std::vector<std::unique_ptr<Unit>> createCavalryArmy(int count);