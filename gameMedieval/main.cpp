#include <iostream>
#include <array>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <memory>


class Field {
public:
    static constexpr int WIDTH = 50;
    static constexpr int HEIGHT = 25;

    std::array<std::array<char, WIDTH>, HEIGHT> arr{}; //можно везде заменить цыфры на переменные 

    Field() {
        initField();
    }

    void initField() {
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

    void drawEnemy(int x, int y) {
        arr[y][x] = '%';
    }
    void drawPlayer(int x, int y) {
        arr[y][x] = '$';
    }

    void clearPlayer(int x, int y) {
        arr[y][x] = ' ';
    }

    void gotoXY(int x, int y) {
        COORD pos{ (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //вывод в консоль в конкретную кординатную позицию ( стандаотный вывод под виндоус , н оне сработает на линуксе, пос это вывод позиции )
    }

    void render() {
        gotoXY(0, 0);
        for (auto& row : arr) {
            for (char c : row)
                std::cout << c;
            std::cout << '\n';
        }
        std::cout << "\nW A S D — движение | Q — выход\n";
    }
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
    int x = 15;
    int y = 15; 

    std::string name;
    int experience = 0;
    int gold = 0;
    Army army;
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
    Enemy enemy;
    Field field;
    int x = 10, y = 10;
    char key;

    field.drawPlayer(x, y);
    field.drawEnemy(enemy.x, enemy.y);
    
    
    field.render(); //перерисовывает консоль с 0 позиции 
    
   
    while (true) {
        int oldX = x, oldY = y; 


        // Проверяем, нажата ли клавиша
        if (_kbhit()) { 
            key = _getch(); // читаем символ без ожидания Enter

            switch (key) {
            case 'w': case 'W': if (y > 1) y--; break;
            case 's': case 'S': if (y < Field::HEIGHT - 2) y++; break;
            case 'a': case 'A': if (x > 1) x--; break;
            case 'd': case 'D': if (x < Field::WIDTH - 2) x++; break;
            case 'q': case 'Q': return 0;
            }
        }

        if (oldX != x || oldY != y) {
            field.clearPlayer(oldX, oldY);
            field.drawPlayer(x, y);
            field.drawEnemy(enemy.x, enemy.y);
            field.render();

        }

        Sleep(50); // ≈20 FPS Небольшая задержка, чтобы не грузить процессор
    }
    //необходимо добавить метод размещения других персонажей 
    //написать конструктор для жэнеми что бы генерировать врагов в определенной точке
    // в цикле до вайл  создается массив врагов 
    //в игровом  цыкле пробегаю по всем умассиву энеми и сравниваю находится ли игрок на кординатах энеми и вызов функции обрабатывающая столкновение ( бой ) 
   // масси в врагов будет динамически т.к после победы враги будут удалятся из масива 
}