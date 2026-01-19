#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>


class Field
{
public:
     int WIDHT = 100; // ширина 
     int HEIGHT = 100; // высота

    std::array<std::array<char, 50>, 50> arr;
    int x$ = 1, y$ = 1;
    Field()
    {

   //конструктор 
         for (auto& row : arr) {
              row.fill(' ');
    }  //заполнение пробелами
        for (int i = 0; i < 50; i++)
        {
            arr[0][i] = '-';
            arr[49][i] = '+';
        }
             for (int i = 1; i < 49; i++)
            {
                 arr[i][0] = '1';
                 arr[i][49] = '2';


            }
    }
  

    void drawField(int x, int y)
    {
        // Восстанавливаем фон и рамку
        for (auto& row : arr) {
            row.fill(' ');
        }
        for (int i = 0; i < 50; i++) {
            arr[0][i] = '-';
            arr[49][i] = '+';
        }
        for (int i = 1; i < 49; i++) {
            arr[i][0] = '1';
            arr[i][49] = '2';
        }

        // Ставим '$' на новые координаты
        if (x >= 1 && x <= 48 && y >= 1 && y <= 48) {
            arr[y][x] = '$';
        }

        // Очистка экрана
        system("cls");

        // Вывод поля
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50; j++) {
                std::cout << arr[i][j];
            }
            std::cout << '\n';
        }
        std::cout << "\nУправление: W, A, S, D. Нажмите 'q' для выхода.\n";
    }
    //рисует поле
    //void drawField(int x, int y)
    //{
    //    system("cls");
    //    
    //    arr[x$][y$] = ' ';
    //    x$ = x; // потому что определено в классе 
    //    y$ = y; 
    //    arr[x][y] = '$';

    //    for (int i = 0; i < 50; i ++)
    //    {
    //        for (int j = 0; j < 50; j++)
    //        {
    //            std::cout << arr[i][j];

    //        }
    //         std::cout << "\n";
    //    }
        //for (int x = 0; x <= WIDHT; x++)
        //{
        //    for (int y = 0; y <= HEIGHT; y++)
        //    {
        //        //  левый бок
        //        if (y == 0)
        //        {
        //            std::cout << "#";
        //        }
        //        //  верхняя часть
        //        else if (x == 0)
        //        {
        //            std::cout << "1";
        //        }
        //        //  нижняя часть
        //        else if (x == HEIGHT)
        //        {
        //            std::cout << "2";
        //        }
        //        // пробелы
        //        else
        //        {
        //            std::cout << " ";
        //        }

        //    }
        //    // правая часть
        //    std::cout << "@" << std::endl;
        //}
    }

};

class Unit {
public:
    int attack = 0;
    int protection = 0;
    int health = 0;

    virtual ~Unit() = default; // виртуальный деструктор для полиморфизма
};

class infantry : public Unit
{
public:
    int attack = 1;
    int protection = 2;
    int health = 10;
};

class cavalry : public Unit
{
public:
    int attack = 3;
    int protection = 3;
    int health = 15;
};

class Army {
private:
    
public:
    std::vector<std::unique_ptr<infantry>> infantryUnits;
    std::vector<std::unique_ptr<cavalry>> cavalryUnits;
};

class Hero {
private:
    std::string name;
   
public:
    int experience = 0; 
    int gold;
    std::string skills;
    Army army; 

    void printStats() {
        std::cout << "Hero [HP: " << experience << ", Gold: " << gold << "]";
    }
    
};


int main()
{
    Field field;
    int x = 10, y = 10;
    char key;

    while (true) {
        field.drawField(x, y);

        // Проверяем, нажата ли клавиша
        if (_kbhit()) {
            key = _getch(); // читаем символ без ожидания Enter

            switch (key) {
            case 'w': case 'W':
                if (y > 1) y--;
                break;
            case 's': case 'S':
                if (y < 48) y++;
                break;
            case 'a': case 'A':
                if (x > 1) x--;
                break;
            case 'd': case 'D':
                if (x < 48) x++;
                break;
            case 'q': case 'Q':
                return 0;
            default:
                break;
            }
        }

        // Небольшая задержка, чтобы не грузить процессор
        Sleep(50); // 50 мс ≈ 20 FPS
    }

    return 0;
}

//int main()
//{
//    Field field;
//    int x = 10, y = 10; // стартовые координаты $
//    char command;
//
//    while (true) {
//        field.drawField(x, y);
//        std::cout << "\nУправление: w (вверх), a (влево), s (вниз), d (вправо). Введите команду (q — выход): ";
//        std::cin >> command;
//
//        switch (command) {
//        case 'w':
//            if (y > 1) y--; // не заходим на верхнюю рамку (y=0)
//            break;
//        case 's':
//            if (y < 48) y++; // не заходим на нижнюю рамку (y=49)
//            break;
//        case 'a':
//            if (x > 1) x--; // не заходим на левую рамку (x=0)
//            break;
//        case 'd':
//            if (x < 48) x++; // не заходим на правую рамку (x=49)
//            break;
//        case 'q':
//            return 0;
//        default:
//            // игнорируем неверные команды
//            break;
//        }
//    }
//
//    return 0;
//}
