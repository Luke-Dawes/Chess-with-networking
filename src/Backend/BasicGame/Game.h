#include "MoveGen.h"
#include <vector>

struct Game {
    Position currentPosition = startPosition();

    std::vector<uint64_t> hasList = {};

};