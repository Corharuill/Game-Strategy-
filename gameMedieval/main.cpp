#include <iostream>
#include <array>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <locale>
#include <limits>// для очистки буфера ввода


// ---------- ЮНИТЫ ----------
class Unit {
public:
    int attack = 0;
    int protection = 0;
    int health = 0;
    virtual ~Unit() = default;
};

class Infantry : public Unit {
public:
    Infantry(int complexity = 1) {
        attack = 2 * complexity;
        protection = 2 * complexity;
        health = 10 * complexity;
    }
};

class Cavalry : public Unit {
public:
    Cavalry() {
        attack = 4;
        protection = 3;
        health = 15;
    }
};

// ---------- АРМИЯ ----------
class Army {
public:
    std::vector<std::unique_ptr<Unit>> units;
};

// ---------- ФАБРИКИ ----------
std::unique_ptr<Unit> createInfantryUnit(int complexity = 1) {
    return std::make_unique<Infantry>(complexity);
}

std::unique_ptr<Unit> createCavalryUnit() {
    return std::make_unique<Cavalry>();
}

std::vector<std::unique_ptr<Unit>> createInfantryArmy(int count, int complexity = 1) {
    std::vector<std::unique_ptr<Unit>> army;
    for (int i = 0; i < count; ++i)
        army.push_back(createInfantryUnit(complexity));
    return army;
}

std::vector<std::unique_ptr<Unit>> createCavalryArmy(int count) {
    std::vector<std::unique_ptr<Unit>> army;
    for (int i = 0; i < count; ++i)
        army.push_back(createCavalryUnit());
    return army;
}

// ---------- ГЕРОЙ ----------
class Hero {
public:
    std::string name = "Таксиарх Афин";
    int experience = 0;
    int gold = 10;
    Army army;
};

// ---------- ВРАГ ----------
class Enemy {
public:
    int x = 0, y = 0;
    std::string name = "Мятежные илоты";
    int experience = 0;
    int gold = 0;
    Army army;

    Enemy(int w, int h, int inf, int cav) {
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
};

// ---------- СТАТЫ АРМИИ ----------
struct ArmyStats {
    int attack = 0;
    int protection = 0;
    int health = 0;
};

ArmyStats calcArmyStats(const Army& army) {
    ArmyStats s;
    for (const auto& u : army.units) {
        s.attack += u->attack;
        s.protection += u->protection;
        s.health += u->health;
    }
    return s;
}
// по образцу филд надо написать филд батл 

// ---------- ПОЛЕ ----------
class Field {
public:
    static constexpr int W = 50;
    static constexpr int H = 25;
    std::array<std::array<char, W>, H> map{};

    Field() { init(); }

    void init() {
        for (auto& r : map) r.fill(' ');
        for (int x = 0; x < W; x++) map[0][x] = map[H - 1][x] = '=';
        for (int y = 0; y < H; y++) map[y][0] = map[y][W - 1] = '|';
    }

    void draw(int px, int py, const std::vector<Enemy>& enemies, const Hero& hero) {
        init();
        map[py][px] = '@';
        for (auto& e : enemies) map[e.y][e.x] = 'E';

        system("cls");
        for (auto& r : map) {
            for (char c : r) std::cout << c;
            std::cout << '\n';
        }

        std::cout << "EXP: " << hero.experience
            << " | GOLD: " << hero.gold
            << " | SPACE — бой | Q — выход\n";
    }
};

// ---------- БОЙ ----------
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

    void drawUI() {
        const int W = 70;

        ArmyStats es = calcArmyStats(enemy.army);
        ArmyStats hs = calcArmyStats(hero.army);

        std::cout << std::string(W, '=') << "\n";
        std::cout << "ВРАГ: " << enemy.name << "\n";
        std::cout << "Юнитов: " << enemy.army.units.size()
            << " | ATK: " << es.attack
            << " | DEF: " << es.protection
            << " | HP: " << es.health << "\n";
        std::cout << "Награда: " << enemy.experience << " EXP, "
            << enemy.gold << " GOLD\n";
        std::cout << std::string(W, '=') << "\n\n";

        std::cout << "      БОЙ \n";
        std::cout << "  [A] Атаковать    [R] Отступить\n\n";

        std::cout << std::string(W, '=') << "\n";
        std::cout << "ГЕРОЙ: " << hero.name << "\n";
        std::cout << "Юнитов: " << hero.army.units.size()
            << " | ATK: " << hs.attack
            << " | DEF: " << hs.protection
            << " | HP: " << hs.health << "\n";
        std::cout << "Опыт: " << hero.experience
            << " | Золото: " << hero.gold << "\n";
        std::cout << std::string(W, '=') << "\n";
    }

    bool handleInput() {
        char c;
        do {
            c = _getch();
        } while (c != 'a' && c != 'A' && c != 'r' && c != 'R');

        if (c == 'r' || c == 'R') {
            system("cls");
            std::cout << "Вы отступили.\n";
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
        std::cout << "Пошаговый бой начался!\n\n";
        Sleep(800);

        int round = 1;

       

        while (!hero.army.units.empty() && !enemy.army.units.empty()) {
            Unit& h = *hero.army.units.front();
            Unit& e = *enemy.army.units.front();

            std::cout << "---- Раунд " << round++ << " ----\n";

           

            // ХОД ГЕРОЯ
            int dmg = calcDamage(h, e);
            e.health -= dmg;
            std::cout << "Герой атакует: -" << dmg
                << " HP (враг HP=" << e.health << ")\n";
            Sleep(500);

            if (e.health <= 0) {
                std::cout << "Вражеский юнит пал!\n\n";
                enemy.army.units.erase(enemy.army.units.begin());
                Sleep(600);
                continue;
            }

          
            // ХОД ВРАГА
            dmg = calcDamage(e, h);
            h.health -= dmg;
            std::cout << "Враг атакует: -" << dmg
                << " HP (герой HP=" << h.health << ")\n";
            Sleep(500);

            if (h.health <= 0) {
                std::cout << "Юнит героя погиб!\n\n";
                hero.army.units.erase(hero.army.units.begin());
                Sleep(600);
            }

            std::cout << "\n";
            Sleep(500);
        }
        system("cls");

        // РЕЗУЛЬТАТ
        if (!hero.army.units.empty()) {
            std::cout << "ПОБЕДА!\n";
            hero.experience += enemy.experience;
            hero.gold += enemy.gold;
            std::cout << "+" << enemy.experience << " EXP, +"
                << enemy.gold << " GOLD\n";
            Sleep(1200);
            return true;
        }
        else {
            std::cout << "ПОРАЖЕНИЕ...\n";
            hero.gold = max(0, hero.gold - enemy.gold / 2);
            Sleep(1200);
            return false;
        }
    }
};
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



// ---------- MAIN ----------
int main() {
    setlocale(LC_ALL, "Russian");

    showMenu();
    
  
}
