#pragma once

#ifndef MATCH4_AIRANDOM_H
#define MATCH4_AIRANDOM_H

#include "AiEngine.h"
#include <random>

namespace Match4
{
	class AiRandom : public AiEngine
	{
		std::default_random_engine generator_;
		std::uniform_int_distribution<int> distribution_;
		int player_;

	public:
		AiRandom(Board* board);
		~AiRandom();

		void setDifficulty(int difficulty) override;
		void setPlayer(int player) override;
		int findCpuMove() override;
	};
}

#endif