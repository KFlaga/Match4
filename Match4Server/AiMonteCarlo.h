#pragma once

#ifndef MATCH4_AIMONTECARLO_H
#define MATCH4_AIMONTECARLO_H

#include "AiEngine.h"

namespace Match4
{
	class AiMonteCarlo : public AiEngine
	{
	public:
		AiMonteCarlo(Board* board);
		~AiMonteCarlo();

		void setDifficulty(int difficulty) override;
		void setPlayer(int player) override;
		int findCpuMove() override;
	};
}

#endif