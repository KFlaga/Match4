#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Board.h>
#include <AiRandom.h>
#include <AiAlfaBeta.h>
#include <BoardHeuristicComputer.h>

using namespace Match4;

namespace UnitTests
{
	TEST_CLASS(AiTests)
	{
	private:
		Board* board_;
		FourChecker* checker_;
		AiRandom* aiRandom_;

	public:
		void setup()
		{
			board_ = new Board();
			checker_ = new FourChecker(board_);

			aiRandom_ = new AiRandom(board_);
		}

		void teardown()
		{
			delete aiRandom_;

			delete checker_;
			delete board_;
		}

		TEST_METHOD(test_random)
		{
			setup();

			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);

			board_->pushPawn(1, Board::Player_1);
			board_->pushPawn(1, Board::Player_1);
			board_->pushPawn(1, Board::Player_1);
			board_->pushPawn(1, Board::Player_1);

			board_->pushPawn(2, Board::Player_1);
			board_->pushPawn(2, Board::Player_1);
			board_->pushPawn(2, Board::Player_1);
			board_->pushPawn(2, Board::Player_1);
			board_->pushPawn(2, Board::Player_1);

			board_->pushPawn(3, Board::Player_1);
			board_->pushPawn(3, Board::Player_1);
			board_->pushPawn(3, Board::Player_1);
			board_->pushPawn(3, Board::Player_1);
			board_->pushPawn(3, Board::Player_1);

			board_->pushPawn(4, Board::Player_1);
			board_->pushPawn(4, Board::Player_1);
			board_->pushPawn(4, Board::Player_1);
			board_->pushPawn(4, Board::Player_1);

			aiRandom_->setDifficulty(0);
			aiRandom_->setPlayer(Board::Player_2);

			int col1 = aiRandom_->findCpuMove();
			Assert::IsTrue(col1 == 1 || col1 == 4);
			board_->pushPawn(col1, Board::Player_2);

			int col2 = aiRandom_->findCpuMove();
			Assert::IsTrue((col2 == 1 || col2 == 4) && col1 != col2);

			teardown();
		}
	};
}