#include "BattleField.h"
Field::Field() { init(); }
void Field::init() {
    for (auto& r : map) r.fill(' ');
    for (int x = 0; x < W; x++) map[0][x] = map[H - 1][x] = '=';
    for (int y = 0; y < H; y++) map[y][0] = map[y][W - 1] = '|';
}

void  Field::draw(int px, int py, const std::vector<Enemy>& enemies, const Hero& hero) {
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
        << " | SPACE Ч бой | Q Ч выход\n";
}