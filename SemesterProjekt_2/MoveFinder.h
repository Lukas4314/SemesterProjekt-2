#include <opencv2/core.hpp>
#pragma once
class MoveFinder
{
public:
	MoveFinder();
	~MoveFinder();

	static int findMove(cv::Mat, cv::Mat, int);


};

