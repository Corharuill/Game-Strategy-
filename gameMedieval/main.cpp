#include <iostream>
#include <array>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>

// ---------- ЮНИТЫ ----------
class Unit {
public:
    int attack = 0;
    int protection = 0;
    int health = 0;
    virtual ~Unit() = default; // виртуальный деструктор для полиморфизма
};

class Infantry : public Unit {
public:
    Infantry() {
        attack = 1;
        protection = 2;
        health = 10;
    }
    Infantry(int complexity) {

        attack = 2* complexity;
        protection = 2 * complexity;
        health = 10 * complexity;
    }
};

class Cavalry : public Unit {
public:
    Cavalry() {
        attack = 3;
        protection = 3;
        health = 15;
    }
};

class Army {
public:
    std::vector<std::unique_ptr<Unit>> units;
};

// ---------- ENEMY ----------
//обьявил выше филда, т.к в филде есть константа энеми  //уровни сложности ( размеры армии) 
class Enemy {
public:
    int x{}, y{};
    std::string name;
    int experience = 0;
    int gold = 0;
    Army army;
  
    // Конструктор с генерацией случайной позиции , добавить сюда параметры размера войска 
    Enemy(int fieldWidth, int fieldHeight,int countInfanty, int counCavalry) {
        static std::mt19937 gen{ std::random_device{}() };

        std::uniform_int_distribution<> dx(1, fieldWidth - 2);
        std::uniform_int_distribution<> dy(1, fieldHeight - 2);

        x = dx(gen);
        y = dy(gen);

        name = "Rebellious peasants";
        experience = 10;
        gold = 1 + gen() % 5;
        // Можно задать имя, опыт, золото, армию по умолчанию или рандомно

        army.units.push_back(std::make_unique<Infantry>()); //размер армии должен быть в цикле который будет генерировать количество каждого юнита
    }
};

// ---------- FIELD ----------
class Field {
public:
    static constexpr int WIDTH = 50;
    static constexpr int HEIGHT = 25;

    std::array<std::array<char, WIDTH>, HEIGHT> arr{};

    Field() {
        initBorders();
    }
    //только рамка.Конструкция не очень хороша, если конструктор выдаст ошибку на пример при выдидении динамисческой памияти.избегать в конструкторе сложной логии.

    void initBorders() {
        for (auto& row : arr)
            row.fill(' ');

        for (int i = 0; i < WIDTH; i++) {
            arr[0][i] = '-';
            arr[HEIGHT - 1][i] = '+';
        }

        for (int i = 1; i < HEIGHT - 1; i++) {
            arr[i][0] = '1';
            arr[i][WIDTH - 1] = '2';
        }
    }

    void gotoXY(int x, int y) {
        COORD pos{ (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    void renderFrame(int playerX, int playerY, const std::vector<Enemy>& enemies) {
        initBorders();

        // игрок
        if (playerX > 0 && playerX < WIDTH - 1 &&
            playerY > 0 && playerY < HEIGHT - 1)
            arr[playerY][playerX] = '$';

        // враги
        for (const auto& e : enemies) {
            if (e.x > 0 && e.x < WIDTH - 1 &&
                e.y > 0 && e.y < HEIGHT - 1)
                arr[e.y][e.x] = '%';
        }

        gotoXY(0, 0);
        for (const auto& row : arr) {
            for (char c : row)
                std::cout << c;
            std::cout << '\n';
        }

        std::cout << "\nW A S D — движение | Q — выход\n";
    }
};

// ---------- HERO ----------
class Hero {
public:
    std::string name = "Player";
    int experience = 0;
    int gold = 0;
    Army army;
};

// ---------- SERVICE ----------
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = false;
    SetConsoleCursorInfo(hConsole, &info);
}
//убирает курсор  
//написать такие же функции для создания юнитов , но они будут перед тем как я буделть emplace_back
std::vector<Enemy> createEnemies(int count, int w, int h) {
    std::vector<Enemy> enemies;
    enemies.reserve(count); //создание вектора с определенным размером 

    for (int i = 0; i < count; ++i)
        enemies.emplace_back(w, h); //создание обьекта 

    return enemies;
}

// ---------- MAIN ----------
int main() {

    setlocale(LC_ALL, "Russian");
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    hideCursor();

    Field field;
    Hero hero;
    hero.army.units.push_back(std::make_unique<Infantry>()); //конструктор инфантри без параметров 

    int playerX = 10;
    int playerY = 10;

    auto enemies = createEnemies(10, Field::WIDTH, Field::HEIGHT);

    // Первоначальная отрисовка
    field.renderFrame(playerX, playerY, enemies);

    while (true) {
        int oldX = playerX;
        int oldY = playerY;
        char key = 0;

        if (_kbhit()) {
            key = _getch();
            switch (key) {
            case 72: if (playerY > 1) playerY--; break;
            case 80: if (playerY < Field::HEIGHT - 2) playerY++; break;
            case 75: if (playerX > 1) playerX--; break;
            case 77: if (playerX < Field::WIDTH - 2) playerX++; break;
            case 'q': case 'Q': return 0;
           
           
            }
        }

        // Проверка столкновений (опционально, но полезно)
        bool collision = false;
        for (const auto& e : enemies) {
            if (e.x == playerX && e.y == playerY) {
                collision = true;
                hero.experience += e.experience;
                hero.gold += e.gold;
                break;
            }
        }

        if (collision) {
            //удаление врага с поля после боя 
            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [&](const Enemy& e) {
                        return e.x == playerX && e.y == playerY;
                    }),
                enemies.end()
            );
        }

        if (oldX != playerX || oldY != playerY || collision) {
            field.renderFrame(playerX, playerY, enemies);
            std::cout << "EXP: " << hero.experience
                << " | GOLD: " << hero.gold << "\n";
        }

        Sleep(50);
    }
}

