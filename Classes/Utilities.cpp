#include "Utilities.h"

namespace Utilities {
    std::random_device rd;
    std::mt19937 gen(rd());

    int genIntValue(int from, int to) {
        std::uniform_int_distribution<int> tmpDistrib(from, to);
        return tmpDistrib(gen);
    }

}