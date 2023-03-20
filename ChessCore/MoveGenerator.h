#pragma once
#include "Board.h"

namespace chess_lib
{
	class MoveGenerator
	{
		std::vector<Move> f_GenerateLineMoves(const Board& p_board, const uint8_t position, const std::vector<pos> vectors, bool iterative = true, bool only_enemy = false) const;
		std::vector<Move> f_GeneratePawnAttackMoves(const Board& board, const uint8_t position, bool only_enemy = false) const;

		void f_GenerateStraightForwardMove(std::vector<Move>& moves, const Board& board, const uint8_t position) const;
	public:
		std::vector<Move> GenerateRookMove(const Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateBishopMove(const Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateQueenMove(const Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKnightMove(const Board& p_board, const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKingMove(const Board& p_board, const uint8_t position, bool only_enemy = false, bool only_moves = true) const;
		std::vector<Move> GeneratePawnMove(const Board& p_board, const uint8_t position, bool only_enemy = false) const;

		std::vector<Move> GenerateCastlings(const Board& p_board, const bool is_white_turn) const;

		bool CanKingBeInCheck(const Board& p_board, const bool is_white_turn) const;
	};
}
