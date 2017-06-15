#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Board.h>

using namespace Match4;

namespace UnitTests
{
	TEST_CLASS(BoardTests)
	{
	private:
		Board* board_;
		FourChecker* checker_;

	public:
		void setup()
		{
			board_ = new Board();
			checker_ = new FourChecker(board_);
		}

		void teardown()
		{
			delete board_;
			delete checker_;
		}

		TEST_METHOD(test_pushPawn)
		{
			setup();

			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_2);

			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Player_1, L"Bottom is not P1");
			Assert::IsTrue(board_->getField(Board::boardBottom - 1, 0) == Board::Player_2, L"Bottom-1 is not P2");
			Assert::IsTrue(board_->getField(Board::boardBottom - 2, 0) == Board::Empty, L"Bottom-2 is not E");

			teardown();
		}

		TEST_METHOD(test_overFlowColumn)
		{
			setup();

			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_2);
			board_->pushPawn(0, Board::Player_2);
			board_->pushPawn(0, Board::Player_2);

			Assert::IsTrue(board_->getField(0, 0) == Board::Player_1);

			teardown();
		}

		TEST_METHOD(test_popPawn)
		{
			setup();

			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->popPawn(0);

			Assert::IsTrue(board_->getField(Board::boardBottom - 1, 0) == Board::Empty);

			teardown();
		}

		TEST_METHOD(test_popEmptyColumn)
		{
			setup();

			board_->popPawn(0);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Empty);

			board_->pushPawn(0, Board::Player_1);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Player_1);

			board_->popPawn(0);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Empty);

			board_->popPawn(0);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Empty);

			teardown();
		}

		TEST_METHOD(test_popPush)
		{
			setup();

			board_->popPawn(0);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Empty);

			board_->pushPawn(0, Board::Player_1);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Player_1);

			board_->popPawn(0);
			Assert::IsTrue(board_->getField(Board::boardBottom, 0) == Board::Empty);

			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->pushPawn(0, Board::Player_1);
			board_->popPawn(0);
			Assert::IsTrue(board_->getField(0, 0) == Board::Empty);
			Assert::IsTrue(board_->getField(1, 0) == Board::Player_1);

			teardown();
		}

		TEST_METHOD(test_match4_column)
		{
			setup();

			board_->setField(0, 0, Board::Player_1);
			board_->setField(1, 0, Board::Player_1);
			board_->setField(2, 0, Board::Player_1);
			board_->setField(3, 0, Board::Player_1);

			Assert::IsTrue(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 0, 0).isMatch);

			teardown();
		}

		TEST_METHOD(test_match4_row)
		{
			setup();

			board_->setField(2, 0, Board::Player_1);
			board_->setField(2, 1, Board::Player_1);
			board_->setField(2, 2, Board::Player_1);
			board_->setField(2, 3, Board::Player_1);

			Assert::IsTrue(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 2, 0).isMatch);

			teardown();
		}

		TEST_METHOD(test_match4_diag)
		{
			setup();

			board_->setField(0, 0, Board::Player_1);
			board_->setField(1, 1, Board::Player_1);
			board_->setField(2, 2, Board::Player_1);
			board_->setField(3, 3, Board::Player_1);

			Assert::IsTrue(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 3, 3).isMatch);

			teardown();
		}

		TEST_METHOD(test_match4_noMatch_columnWithSpace)
		{
			setup();

			board_->setField(0, 0, Board::Player_1);
			board_->setField(1, 0, Board::Player_1);
			board_->setField(2, 0, Board::Player_1);
			board_->setField(4, 0, Board::Player_1);

			Assert::IsFalse(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 0, 0).isMatch);

			teardown();
		}

		TEST_METHOD(test_match4_noMatch_mixed)
		{
			setup();

			board_->setField(2, 2, Board::Player_1);

			board_->setField(3, 3, Board::Player_1);
			board_->setField(1, 1, Board::Player_1);

			board_->setField(2, 3, Board::Player_1);
			board_->setField(2, 1, Board::Player_1);

			board_->setField(0, 2, Board::Player_1);
			board_->setField(3, 2, Board::Player_1);

			board_->setField(3, 1, Board::Player_1);
			board_->setField(4, 0, Board::Player_1);

			Assert::IsFalse(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 2, 2).isMatch);

			teardown();
		}

		TEST_METHOD(test_match4_mixed)
		{
			setup();

			board_->setField(2, 2, Board::Player_1);

			board_->setField(3, 3, Board::Player_1);
			board_->setField(1, 1, Board::Player_1);

			board_->setField(2, 3, Board::Player_1);
			board_->setField(2, 1, Board::Player_1);

			board_->setField(0, 2, Board::Player_1);
			board_->setField(3, 2, Board::Player_1);

			board_->setField(3, 1, Board::Player_1);
			board_->setField(4, 0, Board::Player_1);
			board_->setField(1, 3, Board::Player_1);
			board_->setField(0, 4, Board::Player_1);

			Assert::IsTrue(checker_->checkIsWinnerAfterAddingPawn(Board::Player_1, 2, 2).isMatch);

			teardown();
		}
	};
}