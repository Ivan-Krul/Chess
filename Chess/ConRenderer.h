#pragma once
#include "Board.h"

#include <string>

class ConRenderer
{
public:
	std::string Render(const chess_lib::Board& board, const bool side);

};

