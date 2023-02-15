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
	public:
		const bool MovePiece(Board& board, Move move);
	};
}
