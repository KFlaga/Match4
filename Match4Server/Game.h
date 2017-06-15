#pragma once

#ifndef MATCH4_GAME_H
#define MATCH4_GAME_H

#include "Board.h"
#include "Message.h"
#include <memory>

namespace Match4
{
	class AiEngine;
	class Server;
	class Game
	{
	private:
		Board board_;
		FourChecker checker_;
		std::unique_ptr<AiEngine> ai_1_;
		std::unique_ptr<AiEngine> ai_2_;
		int currentPlayer_;

		Server* server_;
		friend Server;

	public:
		Game(Server* server);
		~Game();

		void setGameSettings(int diffCpu_1, int diffCpu_2);
		Board& getBoard() { return board_; }
		FourChecker& getChecker() { return checker_; }
		AiEngine* getAi_1() { return ai_1_.get(); }
		AiEngine* getAi_2() { return ai_2_.get(); }
		int getCurrentPlayer() { return currentPlayer_; }
		bool isPlayerCpu(int player) const;

	protected:
		void processHoverRequest(const Message& request);
		void processSelectRequest(const Message& request);
		void processSettingsRequest(const Message& request);
		void processNextPlayerRequest(const Message& request);

	private:
		void setAi(std::unique_ptr<AiEngine>& ai, Board* board, int player, int difficulty);
		bool serveEndingMove(int column);
	};
}

#endif