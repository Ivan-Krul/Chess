#include "MoveController.h"

namespace chess_lib
{
	std::vector<Move> MoveController::f_GetGeneratedLegalMovesFromPlace(const Board& board, const uint8_t position) const
	{
		static auto mg = MoveGenerator();
		static auto stdmoves = std::vector<Move>();
		static auto stdmind = uint8_t();
		static auto conclmoves = std::vector<Move>();

		conclmoves.clear();

		switch (board.GetBoard()[position].type)
		{
			case PieceType::pawn:
				stdmoves = mg.GeneratePawnMove(const_cast<Board&>(board), position);
				break;
			case PieceType::rook:
				stdmoves = mg.GenerateRookMove(const_cast<Board&>(board), position);
				break;
			case PieceType::knight:
				stdmoves = mg.GenerateKnightMove(const_cast<Board&>(board), position);
				break;
			case PieceType::bishop:
				stdmoves = mg.GenerateBishopMove(const_cast<Board&>(board), position);
				break;
			case PieceType::queen:
				stdmoves = mg.GenerateQueenMove(const_cast<Board&>(board), position);
				break;
			case PieceType::king:
				stdmoves = mg.GenerateKingMove(const_cast<Board&>(board), position);
				break;
		}

		
		for (stdmind = 0; stdmind < stdmoves.size(); stdmind++)
		{
			auto cpy = board;
			cpy.ForcedMove(stdmoves[stdmind]);
			if (!mg.CanKingBeInCheck(cpy, !board.GetIsWhiteMove()))
				conclmoves.push_back(stdmoves[stdmind]);
		}
		return conclmoves;
	}

	std::vector<Move> MoveController::Proposition(const Board& board, const uint8_t position) const
	{
		return f_GetGeneratedLegalMovesFromPlace(board, position);
	}

	bool MoveController::ConfirmMove(const Board& board, const Move move)
	{
		static auto stdmoves = f_GetGeneratedLegalMovesFromPlace(board, move.GetP1());
		stdmoves = f_GetGeneratedLegalMovesFromPlace(board, move.GetP1());

		for (const auto& m : stdmoves)
		{
			if(m.GetP2() == move.GetP2())
				return pusher.MovePiece(const_cast<Board&>(board), move);
		}
		return false;
	}

	bool MoveController::NeedPromotion(const Board& board) const
	{
		return board.NeedPromotion();
	}

	void MoveController::PromoteTo(Board& board, Board::PromotionChoice choice)
	{
		board.ChoosePromotion(choice);
	}

	bool MoveController::IsMate(const Board& board) const
	{
		static auto mg = MoveGenerator();
		static auto our_pieces = std::vector<uint8_t>();
		static auto our_side = board.GetIsWhiteMove() ? SideType::white : SideType::black;

		for (uint8_t i = 0; i < board.GetBoard().size(); i++)
			if (board.GetBoard()[i].side == our_side)
				our_pieces.push_back(i);

		for (const auto& p : our_pieces)
			if (Proposition(board, p).size() != 0)
				return false;

		return true;
	}

	MoveController MoveController::m_Instance;
}
