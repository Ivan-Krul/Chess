#pragma once
#include <array>
#include <string>
#include <memory>

#include "Tile.h"

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

		const std::array<Tile, 64> GetBoard() const;
		const std::shared_ptr<Move> GetPreviousMove() const;
		const bool GetIsWhiteMove() const;
		const caslstate GetCastlingState() const;

		static const uint8_t ConvertToIndex(std::string position);
		static const std::string ConvertFromIndex(uint8_t index);
		static const char ConvertToChar(const Tile tile);
		static const Tile ConvertFromTile(const char ch);

		void ForcedMove(const Move move, bool need_accept = true);

		bool NeedPromotion() const;
		void ChoosePromotion(PromotionChoice type);
	};
}
