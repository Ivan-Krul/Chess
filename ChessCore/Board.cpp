#include "Board.h"

namespace chess_lib
{
	const uint8_t Board::PromPos() const
	{
		for (uint8_t y = 0; y < 8; y += 7)
			for (uint8_t x = 0; x < 8; x++)
				if (m_Board[x + y * 8].type == PieceType::pawn)
					return x + y * 8;
		return 0xff;
	}

	Board::Board()
	{
		m_Board.fill({ PieceType::none, SideType::none });
		m_Board[0] = { PieceType::rook, SideType::black };
		m_Board[1] = { PieceType::knight, SideType::black };
		m_Board[2] = { PieceType::bishop, SideType::black };
		m_Board[3] = { PieceType::queen, SideType::black };
		m_Board[4] = { PieceType::king, SideType::black };
		m_Board[5] = { PieceType::bishop, SideType::black };
		m_Board[6] = { PieceType::knight, SideType::black };
		m_Board[7] = { PieceType::rook, SideType::black };
		auto pawn = Tile{ PieceType::pawn, SideType::black };
		memset((&*m_Board.begin()) + 8, *(int*)&pawn, 8);
		pawn.side = SideType::white;
		memset((&*m_Board.rbegin()) - 15, *(int*)&pawn, 8);
		m_Board[56] = { PieceType::rook, SideType::white };
		m_Board[57] = { PieceType::knight, SideType::white };
		m_Board[58] = { PieceType::bishop, SideType::white };
		m_Board[59] = { PieceType::queen, SideType::white };
		m_Board[60] = { PieceType::king, SideType::white };
		m_Board[61] = { PieceType::bishop, SideType::white };
		m_Board[62] = { PieceType::knight, SideType::white };
		m_Board[63] = { PieceType::rook, SideType::white };

		m_CastlingState = { 1,1,1,1 };

		m_PreviousMove = std::make_shared<Move>(0xff, 0xff);
	}

	const std::array<Tile, 64> Board::GetBoard() const
	{
		return m_Board;
	}

	const std::shared_ptr<Move> Board::GetPreviousMove() const
	{
		return m_PreviousMove;
	}

	const bool Board::GetIsWhiteMove() const
	{
		return m_IsWhiteMove;
	}

	const Board::caslstate Board::GetCastlingState() const
	{
		return m_CastlingState;
	}

	const uint8_t Board::ConvertToIndex(std::string position)
	{
		if (position.size() < 2)
			return 0xff;
		auto x = 0;
		if (position[0] - 'a' < 8 && position[0] >= 'a')
			x = position[0] - 'a';
		else if (position[0] - 'A' < 8 && position[0] >= 'A')
			x = position[0] - 'A';
		else
			return 0xff;
		auto y = position[1] - '1';
		if (x >= 0 && x < 8 && y >= 0 && y < 8)
			return x + (8 - y - 1) * 8;
		else
			return 0xff;
	}

	const std::string Board::ConvertFromIndex(uint8_t index)
	{
		auto x = index % 8;
		auto y = index / 8;
		auto str = std::string("  ");
		str[0] = 'A' + x;
		str[1] = '1' + (7 - y);
		return str;
	}

	void Board::ForcedMove(const Move move, bool need_accept)
	{
		if (m_Board[move.GetP1()].type == PieceType::king)
		{
			if (move.GetP1() / 8 == 0)
			{
				m_CastlingState.bk = 0;
				m_CastlingState.bq = 0;
			}
			else
			{
				m_CastlingState.wk = 0;
				m_CastlingState.wq = 0;
			}
		}
		if (m_Board[move.GetP1()].type == PieceType::rook)
		{
			if (move.GetP1() / 8 == 0)
			{
				if (move.GetP1() % 8 == 0)
					m_CastlingState.bq = 0;
				else
					m_CastlingState.bk = 0;
			}
			else
			{
				if (move.GetP1() % 8 == 0)
					m_CastlingState.wq = 0;
				else
					m_CastlingState.wk = 0;
			}
		}
		m_PreviousMove = std::make_shared<Move>(move);
		m_Board[move.GetP2()] = m_Board[move.GetP1()];
		m_Board[move.GetP1()] = { PieceType::none, SideType::none };
		if(need_accept && !NeedPromotion())
			m_IsWhiteMove = !m_IsWhiteMove;
	}
	bool Board::NeedPromotion() const
	{
		for (uint8_t y = 0; y < 8; y += 7)
			for (uint8_t x = 0; x < 8; x++)
				if (m_Board[x + y * 8].type == PieceType::pawn)
					return true;
		return false;
	}
	void Board::ChoosePromotion(PromotionChoice type)
	{
		switch (type)
		{
			case chess_lib::PromotionChoice::queen:
				m_Board[PromPos()].type = PieceType::queen;
				m_IsWhiteMove = !m_IsWhiteMove;
				break;
			case chess_lib::PromotionChoice::knight:
				m_Board[PromPos()].type = PieceType::knight;
				m_IsWhiteMove = !m_IsWhiteMove;
				break;
			case chess_lib::PromotionChoice::rook:
				m_Board[PromPos()].type = PieceType::rook;
				m_IsWhiteMove = !m_IsWhiteMove;
				break;
			case chess_lib::PromotionChoice::bishop:
				m_Board[PromPos()].type = PieceType::bishop;
				m_IsWhiteMove = !m_IsWhiteMove;
				break;
		}
		
	}
}
