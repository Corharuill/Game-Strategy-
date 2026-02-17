#pragma once

#include "BattleUnit.h"
#include <string>
#include <random>

class Enemy {
public:
    int x = 0, y = 0;
    std::string name = "ћ€тежные илоты";
    int experience = 0;
    int gold = 0;
    Army army;
    Enemy(int fieldWidth, int fieldHeight, int countInfantry, int countCavalry);

    
};

