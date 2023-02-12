#pragma once
#include "Board.h"

namespace chess_lib
{
	class MoveGenerator
	{
		struct pos
		{
			int8_t x;
			int8_t y;

		};
		std::vector<Move> f_GenerateLineMoves(const Board& board, const uint8_t position, const std::vector<pos> vectors, bool iterative = true, bool only_enemy = false);
	public:
		std::vector<Move> GenerateRookMove(const Board& board, const uint8_t position, bool only_enemy = false);
		std::vector<Move> GenerateBishopMove(const Board& board, const uint8_t position, bool only_enemy = false);
		std::vector<Move> GenerateQueenMove(const Board& board, const uint8_t position, bool only_enemy = false);
		std::vector<Move> GenerateKnightMove(const Board& board, const uint8_t position, bool only_enemy = false);
		std::vector<Move> GenerateKingMove(const Board& board, const uint8_t position, bool only_enemy = false);
		std::vector<Move> GeneratePawnMove(const Board& board, const uint8_t position, bool only_enemy = false);

		bool CanKingBeInCheck(const Board& board);
	};
}
