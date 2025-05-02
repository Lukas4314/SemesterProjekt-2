#include "BoardTransformer.hpp"
#include <array>


std::array<std::array<double, 4>, 4> BoardTransformer::getInvTFchess(double TFchess[4][4]) {
    std::array<std::array<double, 4>, 4> invTFchess = {{
        {TFchess[0][0], TFchess[1][0], TFchess[2][0], 0},
        {TFchess[0][1], TFchess[1][1], TFchess[2][1], 0},
        {TFchess[0][2], TFchess[1][2], TFchess[2][2], 0},
        {0, 0, 0, 1}
    }};

    // Calculate the translation part
    double tx = TFchess[0][3];
    double ty = TFchess[1][3];
    double tz = TFchess[2][3];
    invTFchess[0][3] = -(TFchess[0][0] * tx + TFchess[0][1] * ty + TFchess[0][2] * tz);
    invTFchess[1][3] = -(TFchess[1][0] * tx + TFchess[1][1] * ty + TFchess[1][2] * tz);
    invTFchess[2][3] = -(TFchess[2][0] * tx + TFchess[2][1] * ty + TFchess[2][2] * tz);

    return invTFchess;
}

std::array<std::array<double, 4>, 4> BoardTransformer::multiplyMatrices(double A[4][4], double B[4][4]) {
    std::array<std::array<double, 4>, 4> C = {{{0}}};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

