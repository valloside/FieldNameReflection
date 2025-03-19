#include <iostream>
#include <format>
#include <cassert>

#include "reflection/GetFieldName.hpp"

using namespace valloside::rflct;

struct Empty
{};

struct Vec3
{
    int x;
    int y;
    int z;
};

struct foo3
{
    int  y[2];
    int  m;
    Vec3 c;
    int  a[2][6];
};

int main()
{
    constexpr auto count0 = FieldCounter::count<Empty>();
    static_assert(count0 == 0);

    constexpr auto count3 = FieldCounter::count<Vec3>();
    static_assert(count3 == 3);

    constexpr auto count4 = FieldCounter::count<foo3>();
    static_assert(count4 == 4);

    constexpr auto noName = FieldNames::get<Empty>();
    static_assert(noName.empty());

    constexpr auto xyz = FieldNames::get<Vec3>();
    static_assert(xyz[0] == "x" && xyz[1] == "y" && xyz[2] == "z");
    std::cout << std::format("{}{}{}\n", xyz[0], xyz[1], xyz[2]);

    constexpr auto ymca = FieldNames::get<foo3>();
    static_assert(ymca[0] == "y" && ymca[1] == "m" && ymca[2] == "c" && ymca[3] == "a");
    std::cout << std::format("{}{}{}{}\n", ymca[0], ymca[1], ymca[2], ymca[3]);
}