#include "ConRenderer.h"

std::string ConRenderer::Render(const chess_lib::Board& board, const bool side)
{
	auto arr = board.GetBoard();
	auto str = std::string();
	for (uint8_t i = 0; i < 64; i++)
	{
		uint8_t a = side ? i : 63-i;
		switch (arr[a].type)
		{
			case chess_lib::PieceType::pawn:
				str += char('p' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			case chess_lib::PieceType::rook:
				str += char('r' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			case chess_lib::PieceType::knight:
				str += char('n' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			case chess_lib::PieceType::bishop:
				str += char('b' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			case chess_lib::PieceType::queen:
				str += char('q' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			case chess_lib::PieceType::king:
				str += char('k' + (uint8_t(arr[a].side) - 2) * ('a' - 'A'));
				break;
			default:
				if (board.GetPreviousMove()->GetP1() == a)
					str += '*';
				else
					str += '.';
				break;
		}
		str += ' ';
		if (a % 8 == (side ? 7 : 0))
		{
			str += '\n';
		}
	}
	str += '\n';
	return str;
}
