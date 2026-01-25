#include <iostream>
#include <array>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>
#include <random>

class Field {
public:
    static constexpr int WIDTH = 50;
    static constexpr int HEIGHT = 25;

    std::array<std::array<char, WIDTH>, HEIGHT> arr{}; //можно везде заменить цыфры на переменные 

    Field() {
        initBorders(); // только рамка
    }

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

    // Новый метод: полностью перерисовать поле с игроком и врагами
    void renderFrame(int playerX, int playerY, const std::vector<Enemy>& enemies) {
        // Сначала восстанавливаем рамку
        initBorders();

        // Рисуем игрока
        if (playerX >= 1 && playerX < WIDTH - 1 && playerY >= 1 && playerY < HEIGHT - 1)
            arr[playerY][playerX] = '$';

        // Рисуем всех врагов
        for (const auto& enemy : enemies) {
            if (enemy.x >= 1 && enemy.x < WIDTH - 1 && enemy.y >= 1 && enemy.y < HEIGHT - 1)
                arr[enemy.y][enemy.x] = '%';
        }

        // Выводим на экран
        gotoXY(0, 0);
        for (const auto& row : arr) {
            for (char c : row)
                std::cout << c;
            std::cout << '\n';
        }
        std::cout << "\nW A S D — движение | Q — выход\n";
    }

    void gotoXY(int x, int y) {
        COORD pos{ (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }
};
//Field() {
//    initField();
//}
//    
//void initField() {
//    for (auto& row : arr)
//        row.fill(' ');

//    for (int i = 0; i < WIDTH; i++) {
//        arr[0][i] = '-';
//        arr[HEIGHT - 1][i] = '+';
//    }

//    for (int i = 1; i < HEIGHT - 1; i++) {
//        arr[i][0] = '1';
//        arr[i][WIDTH - 1] = '2';
//    }
//}

//void drawEnemy(int x, int y) {
//    arr[y][x] = '%';
//}
//void drawPlayer(int x, int y) {
//    arr[y][x] = '$';
//}

//void clearPlayer(int x, int y) {
//    arr[y][x] = ' ';
//}

//void gotoXY(int x, int y) {
//    COORD pos{ (SHORT)x, (SHORT)y };
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //вывод в консоль в конкретную кординатную позицию ( стандаотный вывод под виндоус , н оне сработает на линуксе, пос это вывод позиции )
//}

//void render() {
//    gotoXY(0, 0);
//    for (auto& row : arr) {
//        for (char c : row)
//            std::cout << c;
//        std::cout << '\n';
//    }
//    std::cout << "\nW A S D — движение | Q — выход\n";
//}

};

// ---------- ЮНИТЫ ----------
class Unit {
public:
    int attack = 0;
    int protection = 0;
    int health = 0;
    virtual ~Unit() = default;// виртуальный деструктор для полиморфизма
};

class Infantry : public Unit {
public:
    Infantry() { attack = 1; protection = 2; health = 10; }
};

class Cavalry : public Unit {
public:
    Cavalry() { attack = 3; protection = 3; health = 15; }
};

class Army {
public:
    std::vector<std::unique_ptr<Unit>> units;
};

// ---------- ENEMY ----------

class Enemy {
public:
    int x, y;
    std::string name;
    int experience = 0;
    int gold = 0;
    Army army;

    // Конструктор с генерацией случайной позиции
    Enemy(int fieldWidth, int fieldHeight) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(1, fieldWidth - 2);
        std::uniform_int_distribution<> disY(1, fieldHeight - 2);

        x = disX(gen);
        y = disY(gen);

        // Можно задать имя, опыт, золото, армию по умолчанию или рандомно
            name = "Rebellious peasants";
        experience = 10;
        gold = 1 + (gen() % 5); // от 1 до 5


        army.units.push_back(std::make_unique<Infantry>(10));

    }
};

// ---------- HERO ----------
class Hero {
public:
    std::string name;
    int experience = 0;
    int gold = 0;
    Army army;
};

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
} //убирает курсор  

// ---------- MAIN ----------

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    hideCursor();
    Field field;

    int playerX = 10, playerY = 10;

    // Создаём 10 врагов
    std::vector<Enemy> enemies = createEnemies(10, Field::WIDTH, Field::HEIGHT);

    // Первоначальная отрисовка
    field.renderFrame(playerX, playerY, enemies);

    char key;
    while (true) {
        int oldX = playerX, oldY = playerY;

        if (_kbhit()) {
            key = _getch();
            switch (key) {
            case 'w': case 'W': if (playerY > 1) playerY--; break;
            case 's': case 'S': if (playerY < Field::HEIGHT - 2) playerY++; break;
            case 'a': case 'A': if (playerX > 1) playerX--; break;
            case 'd': case 'D': if (playerX < Field::WIDTH - 2) playerX++; break;
            case 'q': case 'Q': return 0;
            }
        }

        // Проверка столкновений (опционально, но полезно)
        bool collision = false;
        for (const auto& enemy : enemies) {
            if (playerX == enemy.x && playerY == enemy.y) {
                collision = true;
                // Здесь можно вызвать бой: fight(hero, enemy);
                std::cout << "\n Отряд вашего таксиарха столкнулися с " << enemy.name << "!\n";
                Sleep(1000); // пауза для демонстрации
                break;
            }
        }

        if (collision) {
            //удаление врага с поля после боя 
            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [&](const Enemy& e) { return e.x == playerX && e.y == playerY; }),
                enemies.end()
            );
        }

        // Перерисовываю, изменения  что-то изменилось
        if (oldX != playerX || oldY != playerY || collision) {
            field.renderFrame(playerX, playerY, enemies);
        }

        Sleep(50);
    }
}

//int main() {
//    std::ios::sync_with_stdio(false);
//    std::cin.tie(nullptr);
//
//    hideCursor();
//    std::vector<Enemy> createEnemies(int count, int fieldWidth, int fieldHeight) {
//        std::vector<Enemy> enemies;
//        enemies.reserve(count);
//        for (int i = 0; i < count; ++i) {
//            enemies.emplace_back(fieldWidth, fieldHeight);
//        }
//        return enemies;
//    };
//
//    Field field;
//    int x = 10, y = 10;
//    char key;
//
//    field.drawPlayer(x, y);
// 
//    
//    field.render(); //перерисовывает консоль с 0 позиции 
//    
//   
//    while (true) {
//        int oldX = x, oldY = y; 
//
//
//        // Проверяем, нажата ли клавиша
//        if (_kbhit()) { 
//            key = _getch(); // читаем символ без ожидания Enter
//
//            switch (key) {
//            case 'w': case 'W': if (y > 1) y--; break;
//            case 's': case 'S': if (y < Field::HEIGHT - 2) y++; break;
//            case 'a': case 'A': if (x > 1) x--; break;
//            case 'd': case 'D': if (x < Field::WIDTH - 2) x++; break;
//            case 'q': case 'Q': return 0;
//            }
//        }
//
//        if (oldX != x || oldY != y) {
//            field.clearPlayer(oldX, oldY);
//            field.drawPlayer(x, y);
//            field.drawEnemy(enemy.x, enemy.y);
//            field.render();
//
//        }
//
//        Sleep(50); // ≈20 FPS Небольшая задержка, чтобы не грузить процессор
//    }
// 
    //необходимо добавить метод размещения других персонажей 
    //написать конструктор для жэнеми что бы генерировать врагов в определенной точке
    // в цикле до вайл  создается массив врагов 
    //в игровом  цыкле пробегаю по всем умассиву энеми и сравниваю находится ли игрок на кординатах энеми и вызов функции обрабатывающая столкновение ( бой ) 
   // масси в врагов будет динамически т.к после победы враги будут удалятся из масива 
}