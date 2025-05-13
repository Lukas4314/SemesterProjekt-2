#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class HueDifferenceProcessor {
private:
    cv::VideoCapture cap;
    cv::Mat lastFrame, lastHue;
    const std::string windowName = "Webcam Feed";

public:
    HueDifferenceProcessor();  // Constructor declaration
    ~HueDifferenceProcessor();  // Destructor declaration

    cv::Mat captureFrame();  // Function declarations
    cv::Mat extractHueChannel(const cv::Mat& frame);
    cv::Mat computeHueDifference(const cv::Mat& hue1, const cv::Mat& hue2);
    cv::Mat resizeToWidth(const cv::Mat& img, int newWidth);
    void processFrames();
};