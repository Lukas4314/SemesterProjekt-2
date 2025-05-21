#include "ImageFinder.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

ImageFinder::ImageFinder() {}

void ImageFinder::testStart()
{
    // Open webcam once
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        throw std::runtime_error("Error: Could not open the cam.");
    }

    // Load reference images
    cv::Mat greenCircle = cv::imread("greenCircle.png", cv::IMREAD_COLOR);
    cv::Mat redCircle = cv::imread("redCircle.png", cv::IMREAD_COLOR);
    cv::Mat mask = cv::imread("circleMask.png", cv::IMREAD_GRAYSCALE);
    float imageScale = 0.5;
    cv::resize(greenCircle, greenCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
    cv::resize(redCircle, redCircle, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);
    cv::resize(mask, mask, cv::Size(), imageScale, imageScale, cv::INTER_LINEAR);

    if (greenCircle.empty() || redCircle.empty())
    {
        throw std::runtime_error("Error: Could not load the reference images.");
    }

    cv::Point2i greenBallPoint(0, 0);

    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // Capture frame
        if (frame.empty())
            break;

        // Find green circle in the frame
        findImageInImage(greenCircle, frame, greenBallPoint, mask, "test", bgrMode);

        cv::rectangle(frame, cv::Rect(greenBallPoint.x, greenBallPoint.y, greenCircle.cols, greenCircle.rows), cv::Scalar(0, 255, 0), 2);

        // Show the original frame
        cv::imshow("Frame", frame);

        // Exit if 'q' is pressed
        if (cv::waitKey() == 'q')
            break;
    }

    cap.release(); // Release webcam resource
}

void ImageFinder::findImageInImage(cv::Mat image, cv::Mat frame, cv::Point2i &point, cv::Mat mask, std::string name, int mode)
{
    if (frame.empty() || image.empty())
        return;

    // Match the template using the hue channel with a mask
    cv::Mat result;

    if (!mask.empty() && mask.size() == image.size())
    {
        if (mode == ImageFinder::bgrMode)
        {
            int templateMode = cv::TM_SQDIFF_NORMED;

            cv::matchTemplate(frame, image, result, templateMode, mask);
        }
        else if (mode == ImageFinder::hsvMode)
        {
            int templateMode = cv::TM_SQDIFF_NORMED;

            cv::Mat hsvImage;
            cv::Mat hsvFrame;
            cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
            cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

            std::vector<cv::Mat> hsvImageChannels;
            std::vector<cv::Mat> hsvFrameChannels;

            cv::split(hsvImage, hsvImageChannels);
            cv::split(hsvFrame, hsvFrameChannels);

            double hueExponent = 1.0;
            double saturationExponent = 1.0;
            double valueExponent = 1.0;

            adjustHSVChannels(hsvImageChannels, hueExponent, saturationExponent, valueExponent);
            adjustHSVChannels(hsvFrameChannels, hueExponent, saturationExponent, valueExponent);

            cv::Mat finalImage = (hsvImageChannels[0] + hsvImageChannels[1] + hsvImageChannels[2]) / 3;
            cv::Mat finalFrame = (hsvFrameChannels[0] + hsvFrameChannels[1] + hsvFrameChannels[2]) / 3;

            finalFrame.convertTo(finalFrame, CV_8U);
            finalFrame.convertTo(finalFrame, CV_8U);

            cv::matchTemplate(finalFrame, finalImage, result, templateMode, mask);
        }

        else if (mode == ImageFinder::hsvMode2)
        {
            int templateMode = cv::TM_CCORR_NORMED;

            cv::Vec3b pixel = image.at<cv::Vec3b>(0, 0);       // Get the first pixel (row=0, col=0)
            cv::Scalar bgrColor(pixel[0], pixel[1], pixel[2]); // Convert to Scalar (B, G, R

            // Convert input image to HSV
            cv::Mat hsvImage;
            cv::cvtColor(frame, hsvImage, cv::COLOR_BGR2HSV);

            // Convert input color to HSV
            cv::Mat colorMat(1, 1, CV_8UC3, bgrColor);
            cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2HSV);
            cv::resize(colorMat, colorMat, cv::Size(frame.cols, frame.rows), cv::INTER_NEAREST);

            std::vector<cv::Mat> hsvColorChannels;
            cv::split(colorMat, hsvColorChannels);

            std::vector<cv::Mat> hsvChannels;
            cv::split(hsvImage, hsvChannels);

            double hueExponent = 1;
            double saturationExponent = 1;
            double valueExponent = 1;

            adjustHSVChannels(hsvChannels, hueExponent, saturationExponent, valueExponent);
            adjustHSVChannels(hsvColorChannels, hueExponent, saturationExponent, valueExponent);

            // Compute absolute differences
            cv::Mat diffH, diffS, diffV;
            cv::absdiff(hsvChannels[0], hsvColorChannels[0], diffH);
            cv::absdiff(hsvChannels[1], hsvColorChannels[1], diffS);
            cv::absdiff(hsvChannels[2], hsvColorChannels[2], diffV);

            // Normalize to 8-bit for better visualization
            cv::normalize(diffH, diffH, 0, 255, cv::NORM_MINMAX);
            cv::normalize(diffS, diffS, 0, 255, cv::NORM_MINMAX);
            cv::normalize(diffV, diffV, 0, 255, cv::NORM_MINMAX);

            cv::Mat summedHSVImage = (diffH + diffS + diffV) / 3;

            cv::Mat invertedMask = 255 - mask;

            cv::matchTemplate(summedHSVImage, mask, result, templateMode, mask);

            cv::Mat resultClone = result.clone();
            cv::normalize(resultClone, resultClone, 0, 255, cv::NORM_MINMAX);
            resultClone.convertTo(resultClone, CV_8U);
            cv::imshow("result", resultClone);
        }

        else
        {
            std::cout << "No mask" << std::endl;
        }

        // Find the best match location
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

        cv::normalize(result, result, 0, 255, cv::NORM_MINMAX, CV_8U);
        // cv::imshow(name, 255-result);

        // Store best match coordinates
        point = minLoc;
    }
}

void ImageFinder::adjustHSVChannels(std::vector<cv::Mat> &hsvChannels,
                                    double hueExponent,
                                    double saturationExponent,
                                    double valueExponent)
{
    // Apply exponentiation
    cv::pow(hsvChannels[0], hueExponent, hsvChannels[0]);        // Hue
    cv::pow(hsvChannels[1], saturationExponent, hsvChannels[1]); // Saturation
    cv::pow(hsvChannels[2], valueExponent, hsvChannels[2]);      // Value
}

void ImageFinder::rotatePoint(cv::Point2i &point, cv::Point2i center, double angle)
{

    double s = sin(angle * CV_PI / 180);
    double c = cos(angle * CV_PI / 180);
    // Translate point back to origin
    point.x -= center.x;
    point.y -= center.y;
    // Rotate point
    int xnew = (int)round(point.x * c - point.y * s);
    int ynew = (int)round(point.x * s + point.y * c);
    // Translate point back
    point.x = xnew + center.x;
    point.y = ynew + center.y;
}
ImageFinder::~ImageFinder() {}

void ImageFinder::showHSVChannelDifferences(const cv::Mat &image, const cv::Scalar &bgrColor, const std::string &name)
{
    if (image.empty())
    {
        std::cerr << "Error: Image is empty!" << std::endl;
        return;
    }

    // Convert input image to HSV
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    // Convert input color to HSV
    cv::Mat colorMat(1, 1, CV_8UC3, bgrColor);
    cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2HSV);
    cv::resize(colorMat, colorMat, cv::Size(image.cols, image.rows), cv::INTER_NEAREST);

    std::vector<cv::Mat> hsvColorChannels;
    cv::split(colorMat, hsvColorChannels);

    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);

    float hueExponent = 1;
    float saturationExponent = 1;
    float valueExponent = 1;

    adjustHSVChannels(hsvChannels, hueExponent, saturationExponent, valueExponent);
    adjustHSVChannels(hsvColorChannels, hueExponent, saturationExponent, valueExponent);

    // Compute absolute differences
    cv::Mat diffH, diffS, diffV;
    cv::absdiff(hsvChannels[0], hsvColorChannels[0], diffH);
    cv::absdiff(hsvChannels[1], hsvColorChannels[1], diffS);
    cv::absdiff(hsvChannels[2], hsvColorChannels[2], diffV);

    // Normalize to 8-bit for better visualization
    cv::normalize(diffH, diffH, 0, 255, cv::NORM_MINMAX);
    cv::normalize(diffS, diffS, 0, 255, cv::NORM_MINMAX);
    cv::normalize(diffV, diffV, 0, 255, cv::NORM_MINMAX);

    // Show the results

    cv::Size size = cv::Size(1440 / 3, 400);

    cv::resize(diffH, diffH, size, cv::INTER_LINEAR);
    cv::resize(diffS, diffS, size, cv::INTER_LINEAR);
    cv::resize(diffV, diffV, size, cv::INTER_LINEAR);
    cv::resize(hsvChannels[2], hsvChannels[2], size, cv::INTER_LINEAR);
    cv::resize(hsvChannels[1], hsvChannels[1], size, cv::INTER_LINEAR);

    cv::Mat summedHSVImage = (diffH + diffS + diffV) / 3;

    cv::imshow("Summed HSV" + name, summedHSVImage);
    cv::imshow("Hue Difference" + name, diffH);
    cv::imshow("Saturation Difference" + name, diffS);
    cv::imshow("Value Difference" + name, diffV);
    cv::imshow("Value Channel" + name, hsvChannels[2]);
    cv::imshow("Saturation Channel" + name, hsvChannels[1]);

    cv::moveWindow("Hue Difference", 0, 0);
    cv::moveWindow("Saturation Difference", 480, 0);
    cv::moveWindow("Value Difference", 480 * 2, 0);
    cv::moveWindow("Value Channel", 480 * 2, 400);
    cv::moveWindow("Saturation Channel", 480, 400);
    cv::moveWindow("Summed HSV", 0, 400);
}

void ImageFinder::showHSVImageDifferences(const cv::Mat hsvImage, const cv::Mat hsvImage2, std::string name)
{
    if (hsvImage.empty() || hsvImage2.empty())
    {
        std::cerr << "Error: one of the images are empty!" << std::endl;
        return;
    }

    cv::cvtColor(hsvImage, hsvImage, cv::COLOR_BGR2HSV);
    cv::cvtColor(hsvImage2, hsvImage2, cv::COLOR_BGR2HSV);

    // Compute absolute differences
    cv::Mat diffImage;
    cv::absdiff(hsvImage, hsvImage2, diffImage);

    std::vector<cv::Mat> hsvChannels;
    cv::split(diffImage, hsvChannels);

    // Normalize to 8-bit for better visualization
    // cv::normalize(diffH, diffH, 0, 255, cv::NORM_MINMAX);
    // cv::normalize(diffS, diffS, 0, 255, cv::NORM_MINMAX);
    // cv::normalize(diffV, diffV, 0, 255, cv::NORM_MINMAX);

    cv::imshow("Summed HSV " + name, diffImage);
    cv::imshow("Hue Difference " + name, hsvChannels[0]);
    cv::imshow("Saturation Difference " + name, hsvChannels[1]);
    cv::imshow("Value Difference " + name, hsvChannels[2]);

    cv::moveWindow("Hue Difference " + name, 0, 0);
    cv::moveWindow("Saturation Difference " + name, 480, 0);
    cv::moveWindow("Value Difference " + name, 480 * 2, 0);
    cv::moveWindow("Summed HSV " + name, 0, 400);
}

void ImageFinder::hueHeatmapWithParams(cv::Mat inputImage, cv::Mat &outputImage, int minSat, int maxSat, int minVal, int maxVal)
{
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsvImage, channels);
    cv::Mat hue = channels[0];        // Extract Hue channel
    cv::Mat saturation = channels[1]; // Extract Saturation channel
    cv::Mat value = channels[2];      // Extract Value channel

    // Create mask for low/high saturation or value
    cv::Mat mask;
    cv::Mat lowSat = saturation < minSat;
    cv::Mat highSat = saturation > maxSat;
    cv::Mat lowVal = value < minVal;
    cv::Mat highVal = value > maxVal;

    mask = lowSat | highSat | lowVal | highVal; // Combine all conditions

    // Convert Hue channel to a heatmap
    cv::Mat heatmap;
    cv::applyColorMap(hue, heatmap, cv::COLORMAP_JET);

    for (int y = 0; y < heatmap.rows; y++)
    {
        for (int x = 0; x < heatmap.cols; x++)
        {
            if (mask.at<uchar>(y, x) > 0)
            {
                heatmap.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    outputImage = heatmap;
}