#include "AiRandom.h"
#include <random>
#include <chrono>
#include "Board.h"

using namespace Match4;

AiRandom::AiRandom(Board* board) :
	AiEngine(board),
	generator_(std::chrono::system_clock::now().time_since_epoch().count()),
	distribution_(1, 5),
	player_(0)
{

}

AiRandom::~AiRandom()
{

}

void AiRandom::setDifficulty(int difficulty)
{

}

void AiRandom::setPlayer(int player)
{
	player_ = player;
}

int AiRandom::findCpuMove()
{
	int column = distribution_(generator_);

	if (board_->checkColumnIsFree(column))
		return column;
	
	// Choosen column is not free -> return first free
	for (int c = 0; c < 5; ++c)
	{
		if (board_->checkColumnIsFree(c))
		{
			return c;
		}
	}

	// No free column
	return -1;
}