#pragma once
#include "MoveGenerator.h"

namespace chess_lib
{
	class MovePusher
	{
	public:
		const bool MovePiece(Board& board, const Move move) const;
	};
}
