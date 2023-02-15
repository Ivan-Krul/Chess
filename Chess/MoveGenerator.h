#pragma once
#include "Board.h"

namespace chess_lib
{
	class MoveGenerator
	{
		Board* m_pBoard;

		struct pos
		{
			int8_t x;
			int8_t y;

		};
		std::vector<Move> f_GenerateLineMoves(const uint8_t position, const std::vector<pos> vectors, bool iterative = true, bool only_enemy = false) const;
	public:
		MoveGenerator(Board* p_board);
		std::vector<Move> GenerateRookMove(const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateBishopMove(const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateQueenMove(const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKnightMove(const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GenerateKingMove(const uint8_t position, bool only_enemy = false) const;
		std::vector<Move> GeneratePawnMove(const uint8_t position, bool only_enemy = false) const;

		std::vector<Move> GenerateCastlings(const bool is_white_turn) const;

		bool CanKingBeInCheck(const bool is_white_turn) const;
	};
}
