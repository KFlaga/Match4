#include "AiAlfaBeta.h"
#include "BoardHeuristicComputer.h"
#include <chrono>
#include <vector>
#include <algorithm>

using namespace Match4;

namespace Match4
{
	struct Node
	{
		typedef __int32 int32;

		union
		{
			struct
			{
				Board board;
				short score;
				short pruned;
				short padding;
			};
			struct
			{
				short board[5];
				short score;
				short pruned;
				short padding;
			} raw;
		};

		Node() :
			raw{ 0, 0, 0, 0, 0, 0, 1, 0 }
		{ }

		Node(Board board) :
			board{ board },
			score{ 0 },
			pruned{ 0 },
			padding{ 0 }
		{ }

		Node(Board board, short score) :
			board{ board },
			score{ score },
			pruned{ 0 },
			padding{ 0 }
		{ }

		Node(short* board) :
			raw{ board[0], board[1], board[2], board[3], board[4], 0, 0, 0 }
		{ }

		Node(short* board, short score) :
			raw{ board[0], board[1], board[2], board[3], board[4], score, 0, 0 }
		{ }
	};

	class NodeTree
	{
		std::vector<Node> tree_;

	public:
		static int getTreeSize(int depth)
		{
			int size = 1;
			int cur = 1;
			for (int i = 0; i < depth; ++i)
			{
				cur *= 5;
				size += cur;
			}
			return size;
		}

	public:
		NodeTree(int size)
		{
			tree_.resize(size);
		}

		inline Node get(const int pos) const
		{
			return tree_[pos];
		}

		inline void set(const int pos, const Node node)
		{
			tree_[pos] = node;
		}

		inline Node& operator[](int pos)
		{
			return tree_[pos];
		}

		inline Node operator[](int pos) const
		{
			return tree_[pos];
		}

		void reset()
		{
			for (Node node : tree_)
			{
				node.pruned = 1;
			}
		}

		static constexpr int child(int parent, int n)
		{
			return parent * 5 + n + 1;
		}

		static constexpr int parent(int child)
		{
			return (child - 1) / 5;
		}
	};

	class AlfaBetaMoveFinder
	{
		NodeTree tree_;
		std::shared_ptr<BoardHeuristicComputer> heuristic_;

		std::chrono::milliseconds maxTime_;
		decltype(std::chrono::system_clock::now()) startTime_;
		int maxLevel_;

		int currentLevelPlayer_;
		int positivePlayer_;

	public:
		AlfaBetaMoveFinder(int maxDepth, std::shared_ptr<BoardHeuristicComputer> heuristic, int msTimeout = 2000) :
			tree_(NodeTree::getTreeSize(maxDepth)),
			heuristic_(heuristic),
			maxLevel_(maxDepth),
			maxTime_(msTimeout)
		{ }

		void createChildren(const Board previousMove, const int parentPosition)
		{
			for (int c = 0; c < Board::boardLength; ++c)
			{
				if (previousMove.checkColumnIsFree(c))
				{
					Board b(previousMove);
					b.pushPawn(c, currentLevelPlayer_);
					short score = heuristic_->findValue(&b, positivePlayer_);
					tree_[NodeTree::child(parentPosition, c)] = Node{ b, score };
				}
			}
		}

		void createLevel(const int firstParentNode, const int lastParentNode)
		{
			for (int p = firstParentNode; p < lastParentNode; ++p)
			{
				if (!tree_[firstParentNode].pruned)
				{
					createChildren(tree_[firstParentNode].board, p);
				}
			}
		}

		void prune()
		{

		}

		void backPropagate()
		{

		}

		int decideMove()
		{
			// Choose max point move
			int move = 0;
			int score = -100000;
			for (int i = 1; i <= 5; ++i)
			{
				move = score > tree_[i].score ? move : (i - 1);
			}
			return move;
		}

		bool isTimedOut()
		{
			auto now = std::chrono::system_clock::now();
			return now - startTime_ < maxTime_;
		}

		int alphabeta(int parent, int depth, int α, int β, int movePlayer, int maximizingPlayer)
		{
			if (depth == 0)// or node is a terminal node)
			{
				return 0;// the heuristic value of node;
			}
			if (movePlayer == maximizingPlayer)
			{
				int v = -10000;  //-∞
				for (int child = NodeTree::child(parent, 0), i = 0; i < 5; ++i, ++child)
				{
					v = std::max(v, alphabeta(child, depth - 1, α, β, movePlayer ^ 3, maximizingPlayer));
					α = std::max(α, v);
					if (β <= α)
						break; //(*β cut - off *)
				}
				return v;
			}
			else
			{
				int v = 10000;//+∞
				for (int child = NodeTree::child(parent, 0), i = 0; i < 5; ++i, ++child)
				{
					v = std::min(v, alphabeta(child, depth - 1, α, β, movePlayer ^ 3, maximizingPlayer));
					β = std::min(β, v);
					if (β <= α)
						break; //(*α cut - off *)
				}
				return v;
			}
		}

	public:
		int findMove(Board board, const int player)
		{
			int firstParent = 0;
			int lastParent = 1;
			int curLevel = 0;
			currentLevelPlayer_ = player;
			positivePlayer_ = player;
			tree_[0] = Node{ board };

			while (false == isTimedOut() && curLevel < maxLevel_)
			{
				createLevel(firstParent, lastParent);
				prune();

				firstParent = lastParent;
				lastParent = NodeTree::child(lastParent, 0);
				currentLevelPlayer_ ^= 3;
				curLevel += 1;
			}
			backPropagate();
			return decideMove();
		}
	};
}

AiAlfaBeta::AiAlfaBeta(Board* board) :
	AiEngine(board),
	heuristic_(new Match4Heuristic()),
	moveFinder_()
{
	maxTime_ = 2000;//ms
	maxDepth_ = 4;
}

AiAlfaBeta::~AiAlfaBeta()
{

}

void AiAlfaBeta::setDifficulty(int difficulty)
{
	maxDepth_ = difficulty * 2;
	maxDepth_ = maxDepth_ > 8 ? 8 : maxDepth_;

	moveFinder_.reset(new AlfaBetaMoveFinder(maxDepth_, heuristic_, maxTime_));
}

void AiAlfaBeta::setPlayer(int player)
{
	player_ = player;
}

int AiAlfaBeta::findCpuMove()
{
	int move = moveFinder_->findMove(*board_, player_);
	return move;
}