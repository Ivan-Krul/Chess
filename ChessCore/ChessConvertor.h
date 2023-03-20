#pragma once
#include "Tile.h"

#include <string>

namespace chess_lib
{
	class ChessConvertor
	{
	public:
		static const uint8_t ConvertToIndex(std::string position);
		static const std::string ConvertFromIndex(uint8_t index);
		static const char ConvertToChar(const Tile tile);
		static const Tile ConvertFromChar(const char ch);

	private:
		ChessConvertor() = default;
	};
}
