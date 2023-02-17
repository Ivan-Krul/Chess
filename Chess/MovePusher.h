#pragma once
#include "MoveGenerator.h"

namespace chess_lib
{
	class MovePusher
	{
		Board* m_pBoard;
		std::shared_ptr<Move> m_pMove;

		bool f_EnPassantMove();
		bool f_CastlingMove();

		// Singleton pattern
		static MovePusher s_Instance;
		MovePusher() = default;

	public:
		const bool MovePiece(Board& board, Move move, Board::PromotionChoice prom = Board::PromotionChoice::none);

		// Singleton pattern
		static MovePusher& GetInstance()
		{
			return s_Instance;
		}
	};
}
