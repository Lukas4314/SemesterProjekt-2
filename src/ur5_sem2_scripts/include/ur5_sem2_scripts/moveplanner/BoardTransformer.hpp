#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <string>
#include <vector>
#include <cmath>

class BoardTransformer
{
private:
    cv::Mat board_image;
public:
    BoardTransformer();
    BoardTransformer(const cv::Mat& image) : board_image(image) {}
    void setBoardImage(const cv::Mat& image) { board_image = image; }
    cv::Mat getBoardImage() const { return board_image; }
    std::array<std::array<double, 4>, 4> getTFchess();

    ~BoardTransformer();
};

BoardTransformer::BoardTransformer()
{
}

BoardTransformer::~BoardTransformer()
{
}
