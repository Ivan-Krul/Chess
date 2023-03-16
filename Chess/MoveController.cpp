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

		if (!mg.CanKingBeInCheck(board, !board.GetIsWhiteMove()))
			return false;

		our_side = board.GetIsWhiteMove() ? SideType::white : SideType::black;
		our_pieces.clear();

		for (uint8_t i = 0; i < board.GetBoard().size(); i++)
			if (board.GetBoard()[i].side == our_side)
				our_pieces.push_back(i);

		static auto prop = std::vector<Move>();

		for (const auto& p : our_pieces)
		{
			prop = Proposition(board, p);
			//for (const auto& m : prop)
			//	printf("D:\t%s -> %s\n", board.ConvertFromIndex(m.GetP1()).c_str(), board.ConvertFromIndex(m.GetP2()).c_str());
			if (prop.size() != 0)
			{
				//printf("////////////////\n");
				return false;
			}
		}

		return true;
	}

	bool MoveController::IsCheck(const Board& board) const
	{
		auto mg = MoveGenerator();
		return mg.CanKingBeInCheck(board,!board.GetIsWhiteMove());
	}

	bool MoveController::IsStalemate(const Board& board) const
	{
		if (IsMate(board))
			return false;

		static auto mg = MoveGenerator();
		static auto our_pieces = std::vector<uint8_t>();
		static auto our_side = board.GetIsWhiteMove() ? SideType::white : SideType::black;

		our_side = board.GetIsWhiteMove() ? SideType::white : SideType::black;
		our_pieces.clear();

		for (uint8_t i = 0; i < board.GetBoard().size(); i++)
			if (board.GetBoard()[i].side == our_side)
				our_pieces.push_back(i);

		for (const auto& p : our_pieces)
			if (Proposition(board, p).size() != 0)
				return false;

		return true;
	}

	bool MoveController::IsDraw(const Board& board) const
	{
		if (IsMate(board))
			return false;

		static auto wpos = std::vector<uint8_t>();
		static auto bpos = std::vector<uint8_t>();

		wpos.clear();
		bpos.clear();

		for (uint8_t i = 0; i < board.GetBoard().size(); i++)
		{
			if (board.GetBoard()[i].side == SideType::white)
				wpos.push_back(i);
			else if (board.GetBoard()[i].side == SideType::black)
				bpos.push_back(i);
		}

		if (wpos.size() == 1 && wpos.size() == bpos.size())
			return true;

		static auto wpieces = std::vector<PieceType>();
		static auto bpieces = std::vector<PieceType>();

		static auto wcount = std::array<uint8_t, 7>();
		static auto bcount = std::array<uint8_t, 7>();

		wpieces.clear();
		bpieces.clear();
		wcount.fill(0);
		bcount.fill(0);

		for (const auto& p : wpos)
			wpieces.push_back(board.GetBoard()[p].type);
		for (const auto& p : bpos)
			bpieces.push_back(board.GetBoard()[p].type);

		for (const auto& t : wpieces)
			wcount[uint8_t(t)]++;
		for (const auto& t : bpieces)
			bcount[uint8_t(t)]++;

		if (wcount == std::array<uint8_t, 7>{0, 0, 0, 1, 0, 0, 1}&& bpieces.size() == 1)
			return true;
		else if (bcount == std::array<uint8_t, 7>{0, 0, 0, 1, 0, 0, 1}&& wpieces.size() == 1)
			return true;
		else if (wcount == std::array<uint8_t, 7>{0, 0, 0, 0, 1, 0, 1}&& bpieces.size() == 1)
			return true;
		else if (bcount == std::array<uint8_t, 7>{0, 0, 0, 0, 1, 0, 1}&& wpieces.size() == 1)
			return true;


		return false;
	}

	MoveController MoveController::m_Instance;
}


	MoveController MoveController::m_Instance;
}
