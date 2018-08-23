#pragma once
#include <random>

namespace Utilities {
    extern std::random_device rd;
    extern std::mt19937 gen;
    int genIntValue(int from, int to);
}
