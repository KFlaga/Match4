#pragma once

#ifndef MATCH4_AIALFABETA_H
#define MATCH4_AIALFABETA_H

#include "AiEngine.h"
#include <memory>

namespace Match4
{
	class AlfaBetaMoveFinder;
	class BoardHeuristicComputer;
	class AiAlfaBeta : public AiEngine
	{
		std::unique_ptr<AlfaBetaMoveFinder> moveFinder_;
		std::shared_ptr<BoardHeuristicComputer> heuristic_;
		int maxDepth_;
		int maxTime_;
		int player_;

	public:
		AiAlfaBeta(Board* board);
		~AiAlfaBeta();

		void setDifficulty(int difficulty) override;
		void setPlayer(int player) override;
		int findCpuMove() override;
	};
}

#endif