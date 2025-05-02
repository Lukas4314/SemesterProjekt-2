#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <string>
#include <vector>
#include <cmath>

class BoardTransformer
{
private:
    const std::array<std::array<double, 4>, 4> TFRed = 
        {{{1.0, 0.0, 0.0, 90.0},
        {0.0, 1.0, 0.0, -30.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}}};
    const std::array<std::array<double, 4>, 4> TFGreen = 
        {{{1.0, 0.0, 0.0, 35.0},
        {0.0, 1.0, 0.0, 25.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}}};

public:
    BoardTransformer();
    ~BoardTransformer();
    static std::array<std::array<double, 4>, 4> getInvTFchess(double TFchess[4][4]);
    static std::array<std::array<double, 4>, 4> multiplyMatrices(double A[4][4], double B[4][4]);
    
};

BoardTransformer::BoardTransformer()
{
}

BoardTransformer::~BoardTransformer()
{
}
