#include "Utill.h"
#include <string>

std::string Utill::translateIntMoveToString(int move) {
	if (move == 0) return "0";



	
	if (move == Utill::WQ) return "wQ";
	if (move == Utill::BQ) return "bQ";
	if (move == Utill::WK) return "wK";
	if (move == Utill::BK) return "bK";

	
	std::string moveString = "";
	moveString += (char)(move % 1000 / 100 + 97);
	moveString += (char)(7-move / 1000 + 49);
	moveString += (char)(move % 10 + 97);

	moveString += (char)(7-move % 100 / 10 + 49);
	return moveString;

}