#pragma once
#include "BattleBattle.h"
#include "BattleUnit.h"
#include "Servis.h"
#include "BattleField.h"
#include <windows.h>
#include <conio.h>
#include "BattleHero.h"
#include <iostream>

// Функция для запуска игры
void startGame() {
    std::cout << "\n=== Игра запущена! ===\n";
    gamplay(); // нужно будет сюда вставить логику игры 

}

// Функция главного меню
void showMenu() {
    int choice;
    bool exitMenu = false;

    while (!exitMenu) {
        // Очистка экрана, может не работать 

 /*#ifdef_WIN*/
        system("cls");
        //#else
        system("clear");
        /*  #endif*/

        std::cout << "=========================\n";
        std::cout << "      МЕНЮ ИГРЫ        \n";
        std::cout << "=========================\n";
        std::cout << "1. Начать игру\n";
        std::cout << "2. Выход\n";
        std::cout << "=========================\n";
        std::cout << "Ваш выбор (1-2): ";

        // Проверка корректности ввода
        if (!(std::cin >> choice)) {
            std::cin.clear(); // сброс ошибочного состояния
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');; // очистка буфера
            choice = -1;
        }

        switch (choice) {
        case 1:
            startGame();
            std::cout << "\nНажмите Enter, чтобы вернуться в меню...";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cin.get();
            break;
        case 2:
            std::cout << "\nСпасибо за игру! До свидания!\n";
            exitMenu = true;
            break;
        default:
            std::cout << "\nОшибка: введите 1 или 2.\n";
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Нажмите Enter для продолжения...";
            std::cin.get();
        }
    }
}

//работа игры
int gamplay() {

    Hero hero;
    auto hInf = createInfantryArmy(3);
    auto hCav = createCavalryArmy(1);
    for (auto& u : hInf) hero.army.units.push_back(std::move(u));
    for (auto& u : hCav) hero.army.units.push_back(std::move(u));

    Field field;
    int px = 10, py = 10;

    std::vector<Enemy> enemies;
    for (int i = 0; i < 6; ++i)
        enemies.emplace_back(Field::W, Field::H, 2 + i % 3, i % 2);

    while (true) {
        field.draw(px, py, enemies, hero);

        char key = _getch();
        int ox = px, oy = py;

        switch (key) {
        case 72: case 'w': case 'W':   // ↑
            if (py > 1) py--;
            break;

        case 80: case 's': case 'S':   // ↓
            if (py < Field::H - 2) py++;
            break;

        case 75: case 'a': case 'A':   // ←
            if (px > 1) px--;
            break;

        case 77: case 'd': case 'D':   // →
            if (px < Field::W - 2) px++;
            break;

        case 'q': case 'Q':
            return 0; // выход из игры
        }


        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (it->x == px && it->y == py) {
            Battle battle(hero, *it);
                if (battle.start())
                    enemies.erase(it);
                else {
                    px = ox;
                    py = oy;
                }
                break;
            }
        }
    }
    return 0;
}

ArmyStats calcArmyStats(const Army& army) {
    ArmyStats s;
    for (const auto& u : army.units) {
        s.attack += u->attack;
        s.protection += u->protection;
        s.health += u->health;
    }
    return s;
}

