#pragma once

#ifndef MATCH4_BOARD_H
#define MATCH4_BOARD_H

#include <array>

namespace Match4
{
	struct Point
	{
	public:
		int row;
		int col;

		constexpr Point(int row = 0, int col = 0) :
			row(row), col(col) { }

		Point operator+(const Point& f) const
		{
			return Point{ row + f.row, col + f.col };
		}

		Point operator-(const Point& f) const
		{
			return Point{ row - f.row, col - f.col };
		}

		Point operator*(int scalar) const
		{
			return Point{ row * scalar, col * scalar };
		}

		void operator+=(const Point& f)
		{
			row += f.row;
			col += f.col;
		}

		void operator-=(const Point& f)
		{
			row -= f.row;
			col -= f.col;
		}
	};

	class Board
	{
	public:
		static constexpr int boardBottom = 4;
		static constexpr int boardLength = 5;
		static constexpr int boardSize = 25;

		enum Player
		{
			Empty = 0,
			Player_1,
			Player_2
		};

	private:
		short columns_[boardLength]; // Each short is one column, each 2 bits, staring from 0 is player color

	public:
		Board() :
			columns_{0,0,0,0,0}
		{
		}

		inline int getField(const int row, const int col) const
		{
			return (((int)columns_[col]) >> 2 * row) & 3;
		}

		inline int getField(const Point p) const
		{
			return (((int)columns_[p.col]) >> 2 * p.row) & 3;
		}

		inline void setField(const int row, const int col, const int val)
		{
			columns_[col] &= ~(3 << 2 * row);
			columns_[col] |= (val << 2 * row);
		}

		inline void setField(const Point p, const int val)
		{
			short mask = ~(3 << 2 * p.row);
			columns_[p.col] = columns_[p.col] & mask;
			columns_[p.col] = columns_[p.col] | (val << 2 * p.row);
		}

		inline bool checkColumnIsFree(const int column) const
		{
			return getField(0, column) == Player::Empty;
		}

		void pushPawn(const int column, const int player)
		{
			for (int i = boardBottom; i >= 0; --i)
			{
				if (getField(i, column) == Player::Empty)
				{
					setField(i, column, player);
					return;
				}
			}
		}

		void popPawn(const int column)
		{
			for (int i = 0; i < boardLength; ++i)
			{
				if (getField(i, column) != Player::Empty)
				{
					setField(i, column, Player::Empty);
					return;
				}
			}
		}

		int getTopPawnRow(const int column) const
		{
			for (int i = 0; i < boardLength; ++i)
			{
				if (getField(i, column) != Player::Empty)
				{
					return i;
				}
			}
			return -1;
		}

		bool isFull() const
		{
			for (int i = 0; i < boardLength; ++i)
			{
				if (getField(0, i) == Player::Empty)
				{
					return false;
				}
			}
			return true;
		}
	};

	class FourChecker
	{
		Board* board_;

		std::array<int, 8> countInDirs_;
		std::array<const Point, 8> dirs_;

	public:
		struct CheckResult
		{
			CheckResult(bool match = false, const Point start = Point{}, const Point end = Point{}) :
				isMatch(match),
				startPoint(start),
				endPoint(end)
			{ }

			bool isMatch;
			Point startPoint;
			Point endPoint;
		};

	public:
		FourChecker(Board* board);
		CheckResult checkIsWinnerAfterAddingPawn(int player, int row, int column);

	private:
		void countNumberOfAdjacentPawns(const Point pawn, const int player);
		bool isWithinBounds(const Point p);
		bool haveSamePlayer(const Point p1, const Point p2);
		CheckResult checkForFourInLine(const Point pawn);
	};
}

#endif