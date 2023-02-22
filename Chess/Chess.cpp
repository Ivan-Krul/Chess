#include <iostream>

#include "NetworkTeller.h"
#include "ConRenderer.h"
#include "MoveController.h"
#include "TypeInterface.h"

int main(int argc, char const* argv[])
{
	auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();

	while ((!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board)))
	{
		std::cout << r.Render(board, board.GetIsWhiteMove());
		TypeInterface::FormQuire(board);
	}
	
	if (mc.IsMate(board))
		std::cout << "Mate!\n" << (!board.GetIsWhiteMove() ? "White" : "Black") << " wins\n";
	else if(mc.IsStalemate(board))
		std::cout << "Stalemate!\nDraw\n";
	else
		std::cout << "Draw!\n";
	system("pause");
	return 0;
}