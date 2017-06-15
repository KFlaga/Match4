#include "Board.h"
#include <string.h>

using namespace Match4;

FourChecker::FourChecker(Board* board) :
	board_(board),
	dirs_{ Point{ -1,0 }, { -1,1 }, { 0,1 }, { 1,1 }, { 1,0 }, { 1,-1 }, { 0,-1 }, { -1,-1 } }
{ }

FourChecker::CheckResult FourChecker::checkIsWinnerAfterAddingPawn(int player, int row, int column)
{
	Point pawn{ row, column };
	countInDirs_.fill(0);

	countNumberOfAdjacentPawns(pawn, player);
	return checkForFourInLine(pawn);
}

void FourChecker::countNumberOfAdjacentPawns(const Point pawn, const int player)
{
	for (int i = 0; i < 8; ++i)
	{
		Point p = pawn + dirs_[i];
		while (isWithinBounds(p) && haveSamePlayer(p, pawn))
		{
			countInDirs_[i] += 1;
			p += dirs_[i];
		}
	}
}

inline bool FourChecker::isWithinBounds(const Point p)
{
	return !(p.row < 0 || p.row >= Board::boardLength ||
		p.col < 0 || p.col >= Board::boardLength);
}

inline bool FourChecker::haveSamePlayer(const Point p1, const Point p2)
{
	return board_->getField(p1) == board_->getField(p2);
}

FourChecker::CheckResult FourChecker::checkForFourInLine(const Point pawn)
{
	// We have match4 if count in dir + opposite dir (dir+4) is 3
	for (int i = 0; i < 4; ++i)
	{
		if (countInDirs_[i] + countInDirs_[i + 4] >= 3)
		{
			// Find start/end point -> move count number in match direction
			return CheckResult{
				true,
				pawn - dirs_[i] * countInDirs_[i],
				pawn - dirs_[i + 4] * countInDirs_[i + 4]
			};
		}
	}
	return CheckResult{ false };
}
