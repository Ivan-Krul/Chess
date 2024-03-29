#include "MovePusher.h"

namespace chess_lib
{
	bool MovePusher::f_EnPassantMove()
	{
		auto& arr = m_pBoard->GetBoard();
		auto is_white_turn = m_pBoard->GetIsWhiteMove();

		auto front = is_white_turn ? -8 : 8;

		if (arr[m_pMove->GetP1()].type != PieceType::pawn)
			return false;
		else if (arr[m_pMove->GetP2() - front].side != (is_white_turn ? SideType::black : SideType::white))
			return false;
		else if (abs(abs(m_pMove->GetP2() - m_pMove->GetP1()) - 8) != 1)
			return false;
		else if (arr[m_pMove->GetP2() - front].type != PieceType::pawn)
			return false;
		else if (arr[m_pMove->GetP2() - front].side != (is_white_turn ? SideType::black : SideType::white))
			return false;
		else if (arr[m_pMove->GetP2()].side != SideType::none)
			return false;

		auto dx = m_pMove->GetP2() - m_pMove->GetP1();
		auto cleaner = Move{ m_pMove->GetP2(), uint8_t(m_pMove->GetP1() + dx - front) };
		
		m_pBoard->ForcedMove(cleaner,false);
		m_pBoard->ForcedMove(*m_pMove);

		return true;
	}

	bool MovePusher::f_CastlingMove()
	{
		auto moves = std::vector<Move>();
		{
			auto mg = MoveGenerator();
			moves = mg.GenerateCastlings(*m_pBoard, m_pBoard->GetIsWhiteMove());
		}
		if (moves.size() == 0)
			return false;
		for (auto& move : moves)
		{
			if (!(move.GetP1() == m_pMove->GetP1() && move.GetP2() == m_pMove->GetP2()))
				continue;
			// Castling
			auto dx = move.GetP1() - move.GetP2();
			auto rx = dx > 0 ? 0 : 7;
			auto y = move.GetP1() / 8;
			auto rmove = Move{ uint8_t(rx + y * 8), uint8_t(move.GetP1() - dx / 2) };

			m_pBoard->ForcedMove(rmove, false);
			m_pBoard->ForcedMove(*m_pMove);
			return true;
		}

		return false;
	}

	const bool MovePusher::MovePiece(Board& board, Move move, PromotionChoice prom)
	{
		m_pBoard = &board;
		m_pMove = std::make_shared<Move>(move);
		auto& arr = board.GetBoard();
		auto is_white_turn = board.GetIsWhiteMove();
		auto mg = MoveGenerator();

		if (!(move.GetP1() < 64 && move.GetP2() < 64))
			return false;
		if (arr[move.GetP1()].side != SideType::white && is_white_turn && !board.NeedPromotion())
			return false;
		if (arr[move.GetP1()].side != SideType::black && !is_white_turn && !board.NeedPromotion())
			return false;
		if (arr[move.GetP1()].side == SideType::none && !board.NeedPromotion())
			return false;
		if (move.GetP1() == move.GetP2() && !board.NeedPromotion())
			return false;

		if (f_EnPassantMove())
			return true;
		else if (f_CastlingMove())
			return true;
		else if (board.NeedPromotion())
		{
			board.ChoosePromotion(prom);
			return true;
		}
		// else
		
		board.ForcedMove(move);
		return true;
	}

	MovePusher MovePusher::s_Instance;
}
