#pragma once
#include <string>
#include "BattleUnit.h"


class Hero {
public:
    std::string name = "Таксиарх Афин";
    int experience = 0;
    int gold = 10;
    Army army;
};
