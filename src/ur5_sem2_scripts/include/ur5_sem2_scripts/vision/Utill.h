#include <string>
class Utill
{
public:
	static constexpr int WQ = -1;
	static constexpr int BQ = -2;
	static constexpr int WK = -3;
	static constexpr int BK = -4;

	static constexpr int WQcastleIndex = 0;
	static constexpr int WKcastleIndex = 1;
	static constexpr int BQcastleIndex = 2;
	static constexpr int BKcastleIndex = 3;



	static std::string translateIntMoveToString(int move);
};

