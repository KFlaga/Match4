#include "Game.h"
#include "Server.h"
#include "AiRandom.h"
#include "AiAlfaBeta.h"

using namespace Match4;

Game::Game(Server* server) :
	server_(server),
	board_(),
	checker_(&board_),
	ai_1_(nullptr),
	ai_2_(nullptr)
{

}

Game::~Game()
{

}

void Game::setGameSettings(int diffCpu_1, int diffCpu_2)
{
	setAi(ai_1_, &board_, Board::Player_1, diffCpu_1);
	setAi(ai_2_, &board_, Board::Player_2, diffCpu_2);
}

void Game::setAi(std::unique_ptr<AiEngine>& ai, Board* board, int player, int difficulty)
{
	ai.release();
	if (difficulty >= 0)
	{
		AiEngine* newAi = difficulty == 0 ? (AiEngine*)new AiRandom(board) : (AiEngine*)new AiAlfaBeta(board);
		newAi->setDifficulty(difficulty);
		newAi->setPlayer(player);
		ai.reset(newAi);
	}
}

bool Game::isPlayerCpu(int player) const
{
	return player == Board::Player_1 ? (bool)ai_1_ : (bool)ai_2_;
}

void Game::processHoverRequest(const Message& request)
{
	int column = request.columnData.column;

	if (column >= 0 
		&& column < board_.boardLength &&
		board_.checkColumnIsFree(column))
	{
		server_->pushResponse(Message{ MessageType::RespGoodMove, column, 0, 0 });
	}
	else
	{
		server_->pushResponse(Message{ MessageType::RespBadMove, column, 0, 0 });
	}
}

void Game::processSelectRequest(const Message& request)
{
	int column = request.columnData.column;
	bool isFree = board_.checkColumnIsFree(column);
	server_->logMessage(std::string("Select column: ") + 
		std::to_string(column) + ". IsFree = " + std::to_string(isFree));
	
	if (false == (column >= 0
		&& column < board_.boardLength &&
		board_.checkColumnIsFree(column)))
	{
		server_->pushResponse(Message{ MessageType::RespBadMove, column, 0, 0 });
		return;
	}
	server_->pushResponse(Message{ MessageType::RespGoodMove, column, 0, 0 });

	// Update board and check win/end
	board_.pushPawn(column, currentPlayer_);
	if (false == serveEndingMove(column))
		server_->pushInternal(Message{ MessageType::ReqNextPlayer, currentPlayer_ ^ 3, 0, 0 });
}

bool Game::serveEndingMove(int column)
{
	auto res = checker_.checkIsWinnerAfterAddingPawn(
		currentPlayer_, board_.getTopPawnRow(column), column);

	if (res.isMatch)
	{
		server_->pushResponse(Message{ MessageType::RespWinMove, currentPlayer_, 0, 0 });
		server_->pushResponse(Message{ MessageType::RespField, res.startPoint.row, res.startPoint.col, 0 });
		server_->pushResponse(Message{ MessageType::RespField, res.endPoint.row, res.endPoint.col, 0 });
		return true;
	}

	if (board_.isFull())
	{
		server_->pushResponse(Message{ MessageType::RespDraw, 0, 0, 0 });
		return true;
	}
	return false;
}

void Game::processSettingsRequest(const Message& request)
{
	int diff_1 = request.data[0];
	int diff_2 = request.data[1];
	setGameSettings(diff_1, diff_2);
	currentPlayer_ = Board::Player_1;

	server_->pushResponse(Message{ MessageType::RespConfirm, 0, 0, 0 });
	server_->pushInternal(Message{ MessageType::ReqNextPlayer, currentPlayer_, 0, 0 });
}

void Game::processNextPlayerRequest(const Message& request)
{
	currentPlayer_ = request.data[0];
	if (!isPlayerCpu(currentPlayer_))
	{
		server_->pushResponse(Message{ MessageType::RespNextPlayer_Human, currentPlayer_, 0, 0 });
		return;
	}

	server_->pushResponse(Message{ MessageType::RespNextPlayer_Cpu, currentPlayer_, 0, 0 });
	auto& ai = currentPlayer_ == Board::Player_1 ? ai_1_ : ai_2_;
	int column = ai->findCpuMove();
	server_->pushInternal(Message{ MessageType::ReqSelect, column, 0, 0 });
}