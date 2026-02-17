#include "BattleBattle.h"
#include "BattleEnemy.h"
#include "BattleHero.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include "Servis.h"


    void Battle::drawUI() {
        const int W = 70;

        ArmyStats es = calcArmyStats(enemy.army);
        ArmyStats hs = calcArmyStats(hero.army);

        std::cout << std::string(W, '=') << "\n";
        std::cout << "ÂÐÀÃ: " << enemy.name << "\n";
        std::cout << "Þíèòîâ: " << enemy.army.units.size()
            << " | ATK: " << es.attack
            << " | DEF: " << es.protection
            << " | HP: " << es.health << "\n";
        std::cout << "Íàãðàäà: " << enemy.experience << " EXP, "
            << enemy.gold << " GOLD\n";
        std::cout << std::string(W, '=') << "\n\n";

        std::cout << "      ÁÎÉ \n";
        std::cout << "  [A] Àòàêîâàòü    [R] Îòñòóïèòü\n\n";

        std::cout << std::string(W, '=') << "\n";
        std::cout << "ÃÅÐÎÉ: " << hero.name << "\n";
        std::cout << "Þíèòîâ: " << hero.army.units.size()
            << " | ATK: " << hs.attack
            << " | DEF: " << hs.protection
            << " | HP: " << hs.health << "\n";
        std::cout << "Îïûò: " << hero.experience
            << " | Çîëîòî: " << hero.gold << "\n";
        std::cout << std::string(W, '=') << "\n";
    }

    bool handleInput() {
        char c;
        do {
            c = _getch();
        } while (c != 'a' && c != 'A' && c != 'r' && c != 'R');

        if (c == 'r' || c == 'R') {
            system("cls");
            std::cout << "Âû îòñòóïèëè.\n";
            Sleep(800);
            return false;
        }

        return resolve();
    }
    int calcDamage(const Unit& attacker, const Unit& defender) {
        int dmg = attacker.attack - defender.protection / 2;
        return max(1, dmg);
    };

    bool resolve() {
        system("cls");
        std::cout << "Ïîøàãîâûé áîé íà÷àëñÿ!\n\n";
        Sleep(800);

        int round = 1;



        while (!hero.army.units.empty() && !enemy.army.units.empty()) {
            Unit& h = *hero.army.units.front();
            Unit& e = *enemy.army.units.front();

            std::cout << "---- Ðàóíä " << round++ << " ----\n";



            // ÕÎÄ ÃÅÐÎß
            int dmg = calcDamage(h, e);
            e.health -= dmg;
            std::cout << "Ãåðîé àòàêóåò: -" << dmg
                << " HP (âðàã HP=" << e.health << ")\n";
            Sleep(500);

            if (e.health <= 0) {
                std::cout << "Âðàæåñêèé þíèò ïàë!\n\n";
                enemy.army.units.erase(enemy.army.units.begin());
                Sleep(600);
                continue;
            }


            // ÕÎÄ ÂÐÀÃÀ
            dmg = calcDamage(e, h);
            h.health -= dmg;
            std::cout << "Âðàã àòàêóåò: -" << dmg
                << " HP (ãåðîé HP=" << h.health << ")\n";
            Sleep(500);

            if (h.health <= 0) {
                std::cout << "Þíèò ãåðîÿ ïîãèá!\n\n";
                hero.army.units.erase(hero.army.units.begin());
                Sleep(600);
            }

            std::cout << "\n";
            Sleep(500);
        }
        system("cls");

        // ÐÅÇÓËÜÒÀÒ
        if (!hero.army.units.empty()) {
            std::cout << "ÏÎÁÅÄÀ!\n";
            hero.experience += enemy.experience;
            hero.gold += enemy.gold;
            std::cout << "+" << enemy.experience << " EXP, +"
                << enemy.gold << " GOLD\n";
            Sleep(1200);
            return true;
        }
        else {
            std::cout << "ÏÎÐÀÆÅÍÈÅ...\n";
            hero.gold = max(0, hero.gold - enemy.gold / 2);
            Sleep(1200);
            return false;
        }
    }
};