#pragma once
#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <string>
#include <vector>
#include <cmath>

class BoardTransformer
{
private:
    

public:
    BoardTransformer();
    ~BoardTransformer();
    static std::array<std::array<double, 4>, 4> getInverse(std::array<std::array<double, 4>, 4>);
    static std::array<std::array<double, 4>, 4> multiplyMatrices(std::array<std::array<double, 4>, 4>, std::array<std::array<double, 4>, 4>);
    
};

BoardTransformer::BoardTransformer()
{
}

BoardTransformer::~BoardTransformer()
{
}
