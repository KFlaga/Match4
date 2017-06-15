#include "BoardHeuristicComputer.h"
#include <stdlib.h>
#include <string.h>
#include <array>
#include <numeric>

using namespace Match4;

short Match4Heuristic::findValue(Board* board, int positivePlayer)
{
	board_ = board;
	positivePlayer_ = positivePlayer;
	negativePlayer_ = positivePlayer == Board::Player_1 ? Board::Player_2 : Board::Player_1;

	std::array<int, 16> scores;
	constexpr int offsetRowScores = 0;
	constexpr int offsetColumnScores = 5;
	constexpr int offsetDiagonalScores = 10;

	// Check matches in each row
	for (int row = 0; row < Board::boardLength; ++row)
	{
		int score_1 = getScoreForLineInDirection(Point(row, 0), Point(0, 1));
		int score_2 = getScoreForLineInDirection(Point(row, 1), Point(0, 1));
		scores[row + offsetRowScores] = abs(score_1) > abs(score_2) ? score_1 : score_2;
	}

	// Check matches in each column
	for (int col = 0; col < Board::boardLength; ++col)
	{
		int score_1 = getScoreForLineInDirection(Point(0, col), Point(1, 0));
		int score_2 = getScoreForLineInDirection(Point(1, col), Point(1, 0));
		scores[col + offsetColumnScores] = abs(score_1) > abs(score_2) ? score_1 : score_2;
	}

	// Check matches in digonals
	scores[0 + offsetDiagonalScores] = getScoreForLineInDirection(Point(1, 0), Point(1, 1));
	scores[1 + offsetDiagonalScores] = getScoreForLineInDirection(Point(0, 1), Point(1, 1));
	int score_d00 = getScoreForLineInDirection(Point(0, 0), Point(1, 1));
	int score_d11 = getScoreForLineInDirection(Point(1, 1), Point(1, 1));
	scores[2 + offsetDiagonalScores] = abs(score_d00) > abs(score_d11) ? score_d00 : score_d11;

	scores[3 + offsetDiagonalScores] = getScoreForLineInDirection(Point(1, 4), Point(1, -1));
	scores[4 + offsetDiagonalScores] = getScoreForLineInDirection(Point(0, 3), Point(1, -1));
	int score_d04 = getScoreForLineInDirection(Point(0, 4), Point(1, -1));
	int score_d13 = getScoreForLineInDirection(Point(1, 3), Point(1, -1));
	scores[5 + offsetDiagonalScores] = abs(score_d04) > abs(score_d13) ? score_d04 : score_d13;

	// Sum all scores
	return std::accumulate(std::begin(scores), std::end(scores), 0);
}

int Match4Heuristic::getScoreForLineInDirection(Point point, const Point dir)
{
	int pawnCounts[3] = { 0, 0, 0 };
	for (int i = 0; i < 4; ++i)
	{
		pawnCounts[board_->getField(point)] += 1;
		point += dir;
	}
	return getScoreForLine(pawnCounts[positivePlayer_], pawnCounts[negativePlayer_]);
}

int Match4Heuristic::getScoreForLine(int positiveCount, int negativeCount)
{
	if (positiveCount == 0)
	{
		if (negativeCount == 0)
		{
			return 1;
		}
		else if (negativeCount == 4)
		{
			return -1000;
		}
		else
		{
			return -(negativeCount * negativeCount);
		}
	}
	else if (negativeCount == 0)
	{
		if (positiveCount == 4)
		{
			return 1000;
		}
		else
		{
			return (positiveCount * positiveCount) + 1;
		}
	}
	return 0;
}
