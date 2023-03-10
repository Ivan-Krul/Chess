#pragma once
#include <stdint.h>
#include <vector>

namespace chess_lib
{
	enum class PieceType : uint8_t
	{
		none = 0,
		pawn,
		rook,
		knight,
		bishop,
		queen,
		king
	};

	enum class SideType : uint8_t
	{
		none = 0,
		white,
		black
	};

	struct Tile
	{
		PieceType type : 3;
		SideType side : 2;
	};

	class Move
	{
		uint8_t P1, P2;
	public:
		Move(uint8_t pos_from, uint8_t pos_to) : P1(pos_from), P2(pos_to) {}

		const uint8_t GetP1() const
		{
			return P1;
		}
		const uint8_t GetP2() const
		{
			return P2;
		}
	};

	enum class PromotionChoice
	{
		none,
		queen,
		knight,
		rook,
		bishop
	};

	struct GlobMove
	{
		Move mv;
		PromotionChoice prom;
	};

	struct pos
	{
		int8_t x : 4;
		int8_t y : 4;
	};
}

