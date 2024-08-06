#include <iostream>

#define l(v) std::wcout << #v << " = " << (v) << "\n";

struct  Vec2{ int x, y; };

Vec2 U = {120, 120};     /// Верхняя левая точка координаты шахматной доски.
Vec2 D = {200, 200};     /// Нижняя правая точка координаты шахматной доски.

void check(const Vec2 M) /// Рандомный клик мауса с координатами { M.x, M.y }
{
    static std::string bx = {"abcdefgh"};
    static std::string by = {"12345678"};

    const int Cell = (D.x - U.x) / 8;

    Vec2 B = {M.x - U.x,
              M.y - U.y};

    unsigned ix = B.x /  Cell;
    unsigned iy = B.y /  Cell;

    if(ix < 8 && iy < 8)
    {   std::cout << "Cell::" << bx[ix] << "," << by[iy] << '\n';
    }
    else std::cout << "Past the board\n";
}

int main()
{
    check({121, 121});
    check({199, 199});
    check({199, 222});
}
