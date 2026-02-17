#pragma once
#include "BattleUnit.h"

// Функция для запуска игры
void startGame();

// Функция главного меню
void showMenu();

//работа игры
int gamplay();

struct ArmyStats {
    int attack = 0;
    int protection = 0;
    int health = 0;
};

ArmyStats calcArmyStats(const Army& army);


