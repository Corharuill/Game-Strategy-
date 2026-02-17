#pragma once
#include "BattleHero.h"
#include "BattleEnemy.h"
#include "Servis.h"

class Battle {
public:
    Battle(Hero& h, Enemy& e) : hero(h), enemy(e) {}

    bool start() {
        system("cls");
        drawUI();
        return handleInput();
    }

private:
    Hero& hero;
    Enemy& enemy;

    void drawUI();
    bool handleInput();
    int calcDamage(const Unit& attacker, const Unit& defender);

    bool resolve();
};

