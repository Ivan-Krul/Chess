#pragma once
#include <iostream>
#include "MoveController.h"

class TypeInterface
{
public:
	static chess_lib::GlobMove FormQuire(chess_lib::Board& board);
	static chess_lib::GlobMove ConvertFromString(std::string str);
	static std::string ConvertToString(chess_lib::GlobMove mov);
	static bool IsLegal(const std::string str);
	static bool RecreateMove(chess_lib::Board& board, const chess_lib::GlobMove mov);
};
