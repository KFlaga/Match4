#pragma once

#include "Board.h"

namespace Match4
{
	/*
		Serves to compute heuristic value of given board layout for given player
	*/
	class BoardHeuristicComputer
	{
	public:
		virtual short findValue(Board* board, int positivePlayer) = 0;
	};

	/*
		Add points for each possible match4 to do.
		Add more points for each possible match4 with player's pawns in line.
		Removes points for each possible match4 with enemy's pawns in line.
	*/
	class Match4Heuristic : public BoardHeuristicComputer
	{
		Board* board_;
		int positivePlayer_;
		int negativePlayer_;
		int* pawnCounts_;

	public:
		short findValue(Board* board, int positivePlayer) override;

	private:
		int getScoreForLineInDirection(Point point, const Point dir);
		int getScoreForLine(int positiveCount, int negativeCount);
	};
}