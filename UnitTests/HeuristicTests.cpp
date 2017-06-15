#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Board.h>
#include <BoardHeuristicComputer.h>

using namespace Match4;

namespace UnitTests
{
	TEST_CLASS(HeuristicTests)
	{
	private:
		Board* board_;
		Match4Heuristic* heuristicM4_;

	public:
		void setup()
		{
			board_ = new Board();
			heuristicM4_ = new Match4Heuristic();
		}

		void teardown()
		{
			delete heuristicM4_;
			delete board_;
		}

		TEST_METHOD(test_empty)
		{
			setup();

			short score = heuristicM4_->findValue(board_, Board::Player_1);
			Assert::IsTrue(score == 16, (
				std::wstring(L"Score = ") + ToString<int>((int)score) + L". Expected score = 16").c_str());

			teardown();
		}

		TEST_METHOD(test_3inColumn)
		{
			setup();

			board_->setField(2, 1, Board::Player_1);
			board_->setField(2, 2, Board::Player_1);
			board_->setField(2, 4, Board::Player_1);
			short score = heuristicM4_->findValue(board_, Board::Player_1);

			Assert::IsTrue(score == 32, (
				std::wstring(L"Score = ") + ToString<int>((int)score) + L". Expected score = 32").c_str());

			teardown();
		}

		TEST_METHOD(test_3inColumnEnemy)
		{
			setup();

			board_->setField(2, 1, Board::Player_2);
			board_->setField(2, 2, Board::Player_2);
			board_->setField(2, 4, Board::Player_2);
			short score = heuristicM4_->findValue(board_, Board::Player_1);

			Assert::IsTrue(score == -8, (
				std::wstring(L"Score = ") + ToString<int>((int)score) + L". Expected score = -8").c_str());

			teardown();
		}

		TEST_METHOD(test_blockedLine)
		{
			setup();

			board_->setField(2, 1, Board::Player_1);
			board_->setField(2, 2, Board::Player_1);
			board_->setField(2, 3, Board::Player_2);
			board_->setField(2, 4, Board::Player_1);
			board_->setField(3, 1, Board::Player_2);
			short score = heuristicM4_->findValue(board_, Board::Player_1);

			Assert::IsTrue(score == 10, (
				std::wstring(L"Score = ") + ToString<int>((int)score) + L". Expected score = 10").c_str());

			teardown();
		}
	};
}