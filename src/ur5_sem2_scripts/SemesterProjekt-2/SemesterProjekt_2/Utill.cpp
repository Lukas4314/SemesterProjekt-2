#include "Utill.h"
#include <string>

std::string Utill::translateIntMoveToString(int move) {
	std::string moveString = "";
	moveString += (char)(move / 1000 + 97);
	moveString += (char)(move % 1000 / 100 + 49);
	moveString += (char)(move % 100 / 10 + 97);
	moveString += (char)(move % 10 + 49);
	return moveString;

}