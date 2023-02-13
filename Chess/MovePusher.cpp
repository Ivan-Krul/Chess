#include "MovePusher.h"

namespace chess_lib
{
	const bool MovePusher::MovePiece(Board& board, const Move move) const
	{
		auto& arr = board.GetBoard();
		auto is_white_turn = board.GetIsWhiteMove();

		if (!(move.GetP1() < 64 && move.GetP2() < 64))
			return false;
		if (arr[move.GetP1()].side != SideType::white && is_white_turn)
			return false;
		if (arr[move.GetP1()].side != SideType::black && !is_white_turn)
			return false;
		if (arr[move.GetP1()].side == SideType::none)
			return false;
		if (move.GetP1() == move.GetP2())
			return false;

		{
			auto cpy = board;
			cpy.ForcedMove(move);
			auto mg = MoveGenerator(&cpy);
			if (mg.CanKingBeInCheck(!board.GetIsWhiteMove()))
				return false;
		}

		board.ForcedMove(move);
		return true;
	}
}
