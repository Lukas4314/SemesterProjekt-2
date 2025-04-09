#include <opencv2/core.hpp>
class MoveFinder
{
public:
	MoveFinder();
	~MoveFinder();

	static int findMove(cv::Mat, cv::Mat, int);


};

