#include "Utill.h"
#include <string>
#include <vector>
#include <rclcpp/rclcpp.hpp> // For git logger

using namespace std;



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

string Utill::translateEngineBestMove(string bestmove) {
	if (bestmove == "e1g1")
	{
		return "wK";
	}
	else if (bestmove == "e1c1")
	{
		return "wQ";
	}
	else if (bestmove == "e8g8")
	{
		return "bK";
	}
	else if (bestmove == "e8c8")
	{
		return "bQ";
	}
	else 
	{
		return bestmove;
	}
}

string Utill::translateToEngine(string move) 
{
	if (move == "wK")
	{
		return "e1g1";
	}
	else if (move == "wQ")
	{
		return "e1c1";
	}
	else if (move == "bK")
	{
		return "e8g8";
	}
	else if (move == "bQ")
	{
		return "e8c8";
	}
	else 
	{
		return move;
	}
}


string Utill::vectorStringToString(const vector<string> &moveHistory)
{
	string allMoves; // String of all moves
	for (const string &m : moveHistory)
	{ // Here we add move to all moves
		allMoves += m + " ";
	}
	return allMoves;
}