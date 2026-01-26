#include <iostream>
#include <array>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <locale>

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
        attack = 2 * complexity;
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

// ---------- UNITS ----------
// Создание отдельного юнита пехоты
std::unique_ptr<Unit> createInfantryUnit(int complexity = 1) {
    return std::make_unique<Infantry>(complexity);
}

// Создание отдельного юнита кавалерии
std::unique_ptr<Unit> createCavalryUnit() {
    return std::make_unique<Cavalry>();
}

// Создание армии пехоты (вектор юнитов)
std::vector<std::unique_ptr<Unit>> createInfantryArmy(int count, int complexity = 1) {
    std::vector<std::unique_ptr<Unit>> army;
    army.reserve(count);
    for (int i = 0; i < count; ++i) {
        army.push_back(createInfantryUnit(complexity));
    }
    return army;
}
// добавить проверку на дебаг, добавить список армий и сколько юнитов в каждой 
// Создание армии кавалерии (вектор юнитов)
std::vector<std::unique_ptr<Unit>> createCavalryArmy(int count) {
    std::vector<std::unique_ptr<Unit>> army;
    army.reserve(count);
    for (int i = 0; i < count; ++i) {
        army.push_back(createCavalryUnit());
    }
    return army;
}

// ---------- ENEMY ----------
class Enemy {
public:
    int x{}, y{};
    std::string name;
    int experience = 0;
    int gold = 0;
    Army army;

    // Конструктор с генерацией случайной позиции и армии
    Enemy(int fieldWidth, int fieldHeight, int countInfantry, int countCavalry) {
        static std::mt19937 gen{ std::random_device{}() };

        std::uniform_int_distribution<> dx(1, fieldWidth - 2);
        std::uniform_int_distribution<> dy(1, fieldHeight - 2);

        x = dx(gen);
        y = dy(gen);

        name = "The rebel helots";
        experience = 5 * countInfantry + 8 * countCavalry; // Опыт пропорционален силе армии
        gold = 2 * countInfantry + 5 * countCavalry + gen() % 3;

        // Формирование армии с использованием фабрик
        auto infantryArmy = createInfantryArmy(countInfantry, 1);
        auto cavalryArmy = createCavalryArmy(countCavalry);

        // Перемещение юнитов в армию врага
        for (auto& unit : infantryArmy) {
            army.units.push_back(std::move(unit));
        }
        for (auto& unit : cavalryArmy) {
            army.units.push_back(std::move(unit));
        }
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

    void initBorders() {
        for (auto& row : arr)
            row.fill(' ');

        // Верхняя и нижняя границы
        for (int i = 0; i < WIDTH; i++) {
            arr[0][i] = '=';
            arr[HEIGHT - 1][i] = '=';
        }

        // Боковые границы
        for (int i = 1; i < HEIGHT - 1; i++) {
            arr[i][0] = '|';
            arr[i][WIDTH - 1] = '|';
        }

        // Углы 
        arr[0][0] = '+';
        arr[0][WIDTH - 1] = '+';
        arr[HEIGHT - 1][0] = '+';
        arr[HEIGHT - 1][WIDTH - 1] = '+';
    }

    void gotoXY(int x, int y) {
        COORD pos{ (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }

    void renderFrame(int playerX, int playerY, const std::vector<Enemy>& enemies, int heroExp, int heroGold) {
        initBorders();

        // Игрок
        if (playerX > 0 && playerX < WIDTH - 1 &&
            playerY > 0 && playerY < HEIGHT - 1)
            arr[playerY][playerX] = '@'; //игрок

        // Враги
        for (const auto& e : enemies) {
            if (e.x > 0 && e.x < WIDTH - 1 &&
                e.y > 0 && e.y < HEIGHT - 1) //враги
                arr[e.y][e.x] = 'E'; 
        }

        // Отрисовка поля
        gotoXY(0, 0);
        for (const auto& row : arr) {
            for (char c : row)
                std::cout << c;
            std::cout << '\n';
        }

        // Статусная строка
        std::cout << "WASD/ARROWS - движение | SPACE - battle | Q - выход";
        gotoXY(0, HEIGHT + 1);
        std::cout << "Опыт : " << heroExp << " | Драхмы : " << heroGold
            << " | Отряд : " << countArmyUnits(enemies) << " enemies nearby";
        gotoXY(playerX, playerY); // Возврат курсора к игроку для плавности
    }

private:
    // Вспомогательная функция для подсчета общего числа врагов на поле
    int countArmyUnits(const std::vector<Enemy>& enemies) {
        int total = 0;
        for (const auto& e : enemies) {
            total += e.army.units.size();
        }
        return total;
    }
};

// ---------- HERO ----------
class Hero {
public:
    std::string name = "Таксиарх Афинской филы";
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

// Создание врагов с случайным составом армии
std::vector<Enemy> createEnemies(int count, int w, int h) {
    std::vector<Enemy> enemies;
    enemies.reserve(count);

    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_int_distribution<> infDist(1, 4);   // 1-4 пехотинца
    std::uniform_int_distribution<> cavDist(0, 2);   // 0-2 кавалериста

    for (int i = 0; i < count; ++i) {
        int infantryCount = infDist(gen);
        int cavalryCount = cavDist(gen);
        enemies.emplace_back(w, h, infantryCount, cavalryCount);
    }

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

    // Создание армии героя с использованием фабрик
    auto heroInfantry = createInfantryArmy(3, 1); // 3 пехотинца базового уровня
    auto heroCavalry = createCavalryArmy(1);      // 1 кавалерист

    // Перемещение юнитов в армию героя
    for (auto& unit : heroInfantry) {
        hero.army.units.push_back(std::move(unit));
    }
    for (auto& unit : heroCavalry) {
        hero.army.units.push_back(std::move(unit));
    }

    int playerX = 10;
    int playerY = 10;

    auto enemies = createEnemies(8, Field::WIDTH, Field::HEIGHT); // 8 врагов вместо 10 для лучшей видимости

    // Первоначальная отрисовка
    field.renderFrame(playerX, playerY, enemies, hero.experience, hero.gold);

    while (true) {
        int oldX = playerX;
        int oldY = playerY;
        char key = 0;

        if (_kbhit()) {
            key = _getch();
            // Поддержка стрелок и WASD
            switch (key) {
            case 72: case 'w': case 'W': if (playerY > 1) playerY--; break; // Вверх
            case 80: case 's': case 'S': if (playerY < Field::HEIGHT - 2) playerY++; break; // Вниз
            case 75: case 'a': case 'A': if (playerX > 1) playerX--; break; // Влево
            case 77: case 'd': case 'D': if (playerX < Field::WIDTH - 2) playerX++; break; // Вправо
            case ' ': // Пробел для боя при встрече с врагом
            {
                bool inBattle = false;
                for (const auto& e : enemies) {
                    if (e.x == playerX && e.y == playerY) {
                        inBattle = true;
                        hero.experience += e.experience;
                        hero.gold += e.gold;
                        break;
                    }
                }
                if (inBattle) {
                    enemies.erase(
                        std::remove_if(enemies.begin(), enemies.end(),
                            [&](const Enemy& e) {
                                return e.x == playerX && e.y == playerY;
                            }),
                        enemies.end()
                    );
                }
            }
            break;
            case 'q': case 'Q':
                field.gotoXY(0, Field::HEIGHT + 3);
                std::cout << "Прощай, таксиарх, полис тебя не забудет! \n";
                return 0;
            }
        }

        // Проверка автоматического боя при входе на клетку врага
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
            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [&](const Enemy& e) {
                        return e.x == playerX && e.y == playerY;
                    }),
                enemies.end()
            );
        }

        // Перерисовка только при изменении состояния
        if (oldX != playerX || oldY != playerY || collision) {
            field.renderFrame(playerX, playerY, enemies, hero.experience, hero.gold);
        }

        Sleep(40); // Плавное движение
    }
}