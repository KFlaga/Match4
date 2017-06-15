#pragma once

#ifndef MATCH4_AIENGINE_H
#define MATCH4_AIENGINE_H

namespace Match4
{
	class Board;

	class AiEngine
	{
	protected:
		Board* board_;

	public:
		AiEngine(Board* board) :
			board_(board)
		{ }
		virtual ~AiEngine() = default;

		virtual void setDifficulty(int difficulty) = 0;
		virtual void setPlayer(int player) = 0;

		// Returns column to push pawn to or -1 if theres no free column
		virtual int findCpuMove() = 0;
	};
}

#endif