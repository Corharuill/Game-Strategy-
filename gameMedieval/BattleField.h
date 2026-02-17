#pragma once
#include "BattleEnemy.h"
#include "BattleHero.h"
#include <array>
#include <vector>
#include <windows.h>
#include <iostream>

class Field {
public:
    static constexpr int W = 50;
    static constexpr int H = 25;
    std::array<std::array<char, W>, H> map{};

    Field();

    void init();

    void draw(int px, int py, const std::vector<Enemy>& enemies, const Hero& hero);
    
};

