#pragma once
#include "Board.h"

namespace chess_lib
{
	class MoveGenerator
	{
		std::vector<Move> f_GenerateLineMoves(Board& p_board, const uint8_t position, const std::vector<pos> vectors, bool iterative = true, bool only_enemy = false) const;
	public:
		std::vector<Move> GenerateRookMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateBishopMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateQueenMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKnightMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKingMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GeneratePawnMove(Board& p_board, const uint8_t position, bool only_enemy = false) const;

		std::vector<Move> GenerateCastlings(Board& p_board, const bool is_white_turn) const;

		bool CanKingBeInCheck(Board& p_board, const bool is_white_turn) const;
	};
}
