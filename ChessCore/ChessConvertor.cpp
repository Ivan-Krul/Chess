#include "ChessConvertor.h"

namespace chess_lib
{
	const uint8_t ChessConvertor::ConvertToIndex(std::string position)
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

	const std::string ChessConvertor::ConvertFromIndex(uint8_t index)
	{
		auto x = index % 8;
		auto y = index / 8;
		auto str = std::string("  ");
		str[0] = 'A' + x;
		str[1] = '1' + (7 - y);
		return str;
	}

	const char ChessConvertor::ConvertToChar(const Tile tile)
	{
		if (tile.side == SideType::none || tile.type == PieceType::none)
			return ' ';

		auto incr_cap = (tile.side == SideType::black) * 32;
		static const auto atlas = " PRNBQK";

		return atlas[static_cast<char>(tile.type)] + incr_cap;
	}

	const Tile ChessConvertor::ConvertFromChar(const char ch)
	{
		if (ch == ' ')
			return Tile{ PieceType::none, SideType::none };

		auto caps_char = ch - ('a' - 'A');
		static const auto atlas = " PRNBQK";

		for (auto i = 0; i < 7; i++)
		{
			if (caps_char == atlas[i] || ch == atlas[i])
			{
				auto side = SideType();
				if (ch == atlas[i])
					side = SideType::white;
				else
					side = SideType::black;
				return Tile{ PieceType(i), side };
			}
				
		}

		return Tile{ PieceType::none, SideType::none };
	}
	bool ChessConvertor::IsInBoard(pos position)
	{
		return 0 <= position.x && position.x < 8 && 0 <= position.y && position.y < 8;
	}
}
