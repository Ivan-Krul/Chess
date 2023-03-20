#pragma once
#include <array>
#include <memory>

#include "Tile.h"
#include "ChessConvertor.h"

namespace chess_lib
{
	class Board
	{
	public:
		struct  caslstate {
			char wq : 2;
			char wk : 2;
			char bq : 2;
			char bk : 2;
		};
	private:
		caslstate m_CastlingState;
		std::array<Tile, 64> m_Board;
		std::shared_ptr<Move> m_PreviousMove;
		bool m_IsWhiteMove = true;

		const uint8_t PromPos() const;
	public:
		Board();
		Board(const std::string& fen);

		const std::array<Tile, 64> GetBoard() const;
		const std::shared_ptr<Move> GetPreviousMove() const;
		const bool GetIsWhiteMove() const;
		const caslstate GetCastlingState() const;

		void ForcedMove(const Move move, bool need_accept = true);

		bool NeedPromotion() const;
		void ChoosePromotion(PromotionChoice type);
	};
}
