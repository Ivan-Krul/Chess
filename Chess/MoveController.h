#pragma once
#include "MovePusher.h"

namespace chess_lib
{
	class MoveController
	{
		MovePusher pusher = MovePusher::GetInstance();

		std::vector<Move> f_GetGeneratedLegalMovesFromPlace(const Board& board, const uint8_t position) const;

		// Singleton pattern
		static MoveController m_Instance;
		MoveController() = default;
	public:
		std::vector<Move> Proposition(const Board& board, const uint8_t position) const;
		bool ConfirmMove(const Board& board, const Move move);

		bool NeedPromotion(const Board& board) const;
		void PromoteTo(Board& board, Board::PromotionChoice choice);

		bool IsMate(const Board& board) const;
		bool IsCheck(const Board& board) const;
		bool IsStalemate(const Board& board) const;
		bool IsDraw(const Board& board) const;

		// Singleton pattern
		static MoveController& GetInstance()
		{
			return m_Instance;
		}
	};
}
