#include "BoardTransformer.hpp"


#include <array>

std::array<std::array<double, 4>, 4> BoardTransformer::getTFchess()
{
    // Initialize the transformation matrix
    std::array<std::array<double, 4>, 4> TFchess = {{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }};
    

    return TFchess;
}
