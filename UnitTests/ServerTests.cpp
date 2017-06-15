#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Server.h>
#include <chrono>
#include <Game.h>

using namespace Match4;

namespace UnitTests
{		
	TEST_CLASS(ServerTests)
	{
	private:
		Server* server_;

	public:
		void setup()
		{
			server_ = new Server();
		}

		void teardown()
		{
			delete server_;
		}

		TEST_METHOD(test_newServer)
		{
			auto handle = createServer();
			Assert::IsTrue(handle != 0, L"Server creation failed");
		}

		TEST_METHOD(test_deleteServer)
		{
			auto handle = createServer();
			Assert::IsTrue(handle != 0, L"Server creation failed");
			try
			{
				destroyServer(handle);
			}
			catch (...)
			{
				Assert::IsTrue(false, L"Server deletion failed");
			}
		}

		bool waitForResponse(Message* response, int msTimeout, std::wstring messageInfo = L"")
		{
			auto timeStart = std::chrono::system_clock::now();
			auto timeout = std::chrono::milliseconds(msTimeout);
			bool timedOut = false;

			while (!server_->haveResponesPending())
			{
				auto now = std::chrono::system_clock::now();
				auto duration = now - timeStart;
				if (duration > timeout)
				{
					timedOut = true;
					Assert::IsTrue(false, 
						(std::wstring(L"Wait for message " + messageInfo + L" timed-out")).c_str());
				}
			}

			if (timedOut)
				return false;

			*response = server_->getNextResponse();	
			return true;
		}

		void testMessageType(const Message& response, MessageType expected)
		{
			Assert::IsTrue(response.type == expected,
				(std::wstring(L"Bad response type: ") + std::to_wstring(response.type) +
					L". Expected: " + std::to_wstring(expected)).c_str());
		}

		void testColumn(const Message& response, int expected)
		{
			Assert::IsTrue(response.columnData.column == expected,
				(std::wstring(L"Bad column data: ") + std::to_wstring(response.type) +
					L". Expected: " + std::to_wstring(expected)).c_str());
		}

		void testMessageData(const Message& response, int expected[3])
		{
			Assert::IsTrue(std::memcmp(response.data, expected, sizeof(int)*3) == 0,
				(std::wstring(L"Bad column data: { ") + 
					std::to_wstring(response.data[0]) + L", " +
					std::to_wstring(response.data[1]) + L", " +
					std::to_wstring(response.data[2]) + L" }" +
					L". Expected: { " +
					std::to_wstring(expected[0]) + L", " +
					std::to_wstring(expected[1]) + L", " +
					std::to_wstring(expected[2]) + L" }").c_str());
		}

		TEST_METHOD(test_messageHoverOk)
		{
			setup();

			server_->pushRequest(Message{ MessageType::ReqHover, 1, 0, 0 });
			Message response;
			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespGoodMove);
				testColumn(response, 1);
			}

			teardown();
		}

		TEST_METHOD(test_messageHoverOverflow)
		{
			setup();

			server_->pushRequest(Message{ MessageType::ReqHover, 6, 0, 0 });
			Message response;
			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespBadMove);
				testColumn(response, 6);
			}

			teardown();
		}


		TEST_METHOD(test_messageHoverBad)
		{
			setup();

			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);

			server_->pushRequest(Message{ MessageType::ReqHover, 1, 0, 0 });
			Message response;
			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespBadMove);
				testColumn(response, 1);
			}

			teardown();
		}

		TEST_METHOD(test_messageSettingsHumanVsCpu0)
		{
			setup();

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, -1, 0, 0 });
			Message response;
			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespConfirm);

				Assert::IsFalse(server_->getGame()->isPlayerCpu(Board::Player_1),
					L"Player 1 is not Human");
				Assert::IsTrue(server_->getGame()->isPlayerCpu(Board::Player_2),
					L"Player 2 is not Cpu");
			}

			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespNextPlayer_Human);
				int expectedData[3] = { Board::Player_1, 0, 0 };
				testMessageData(response, expectedData);
			}

			teardown();
		}

		TEST_METHOD(test_messageSelectBad)
		{
			setup();
			Message response;

			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, -1, -1, 0 });
			waitForResponse(&response, 100);
			waitForResponse(&response, 100);

			server_->pushRequest(Message{ MessageType::ReqSelect, 1, 0, 0 });
			if (waitForResponse(&response, 100))
			{
				testMessageType(response, MessageType::RespBadMove);
				testColumn(response, 1);
			}

			teardown();
		}

		TEST_METHOD(test_messageSelectOkHumanVsHuman)
		{
			setup();
			Message response;

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, -1, -1, 0 });
			waitForResponse(&response, 100);
			waitForResponse(&response, 100);

			server_->pushRequest(Message{ MessageType::ReqSelect, 1, 0, 0 });
			if (waitForResponse(&response, 100, L"RespGoodMove"))
			{
				testMessageType(response, MessageType::RespGoodMove);
				testColumn(response, 1);
			}

			if (waitForResponse(&response, 100, L"RespNextPlayer_Human"))
			{
				testMessageType(response, MessageType::RespNextPlayer_Human);

				int expectedData[3] = { Board::Player_2, 0, 0 };
				testMessageData(response, expectedData);
			}

			teardown();
		}

		TEST_METHOD(test_messageSelectWinHumanVsHuman)
		{
			setup();
			Message response;

			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);
			server_->getGame()->getBoard().pushPawn(1, Board::Player_1);

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, -1, -1, 0 });
			waitForResponse(&response, 100);
			waitForResponse(&response, 100);

			server_->pushRequest(Message{ MessageType::ReqSelect, 1, 0, 0 });
			if (waitForResponse(&response, 100, L"RespGoodMove"))
			{
				testMessageType(response, MessageType::RespGoodMove);
				testColumn(response, 1);
			}

			if (waitForResponse(&response, 100, L"RespWinMove"))
			{
				testMessageType(response, MessageType::RespWinMove);

				int expectedData[3] = { Board::Player_1, 0, 0 };
				testMessageData(response, expectedData);
			}

			if (waitForResponse(&response, 10, L"RespField_start"))
			{
				testMessageType(response, MessageType::RespField);
			}

			if (waitForResponse(&response, 10, L"RespField_end"))
			{
				testMessageType(response, MessageType::RespField);
			}

			teardown();
		}

		TEST_METHOD(test_messageSelectOkHumanVsCpu0)
		{
			setup();
			Message response;

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, -1, 0, 0 });
			waitForResponse(&response, 100);
			waitForResponse(&response, 100);

			server_->pushRequest(Message{ MessageType::ReqSelect, 1, 0, 0 });
			if (waitForResponse(&response, 100, L"RespGoodMove"))
			{
				testMessageType(response, MessageType::RespGoodMove);
				testColumn(response, 1);
			}

			if (waitForResponse(&response, 100, L"RespNextPlayer_Cpu"))
			{
				testMessageType(response, MessageType::RespNextPlayer_Cpu);

				int expectedData[3] = { Board::Player_2, 0, 0 };
				testMessageData(response, expectedData);
			}

			if (waitForResponse(&response, 100, L"RespGoodMove"))
			{
				testMessageType(response, MessageType::RespGoodMove);
			}

			if (waitForResponse(&response, 100, L"RespNextPlayer_Human"))
			{
				testMessageType(response, MessageType::RespNextPlayer_Human);

				int expectedData[3] = { Board::Player_1, 0, 0 };
				testMessageData(response, expectedData);
			}

			teardown();
		}

		TEST_METHOD(test_playCpu0vsCpu0)
		{
			setup();
			Message response;

			server_->pushRequest(Message{ MessageType::ReqSetGameSettings, 0, 0, 0 });
			waitForResponse(&response, 100);
			waitForResponse(&response, 100);

			bool running = true;
			while (running)
			{
				while (server_->haveResponesPending())
				{
					Message resp = server_->getNextResponse();
					if (resp.type == MessageType::RespDraw ||
						resp.type == MessageType::RespWinMove)
						running = false;
				}
			}

			teardown();
		}
	};
}