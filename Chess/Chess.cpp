#include <iostream>

#include "NetworkTeller.h"
#include "ConRenderer.h"
#include "MoveController.h"
#include "TypeInterface.h"

int main(int argc, char const* argv[])
{
	auto nt = NetworkTeller::GetInstance();
	auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();
	auto issr = false;
	auto side = false;
	auto ip = std::string();

	std::cout << "Init game(0 - client/1 - server):";
	std::cin >> issr;

	if (issr)
	{
		nt.StartAsServer();
		while (!nt.IsSocketGot());
		side = nt.InitGameFromServer();
		while (nt.IsConnection() && (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board)))
		{
			if (nt.IsIncome())
			{
				if (nt.IsSpecial())
					nt.ReadSpecialMessage();
				if (!nt.IsConnection())
					break;
				RecreateMove(board, nt.RecreateMove());
			}
			if (board.GetIsWhiteMove() != side)
			{
				std::cout << "Waiting...";
				std::cout << "\r\r\r\r\r\r\r\r\r\r";
				continue;
			}
			std::cout << r.Render(board, side);
			auto recr = FormQuire(board);
			nt.TellMove(recr.move, recr.prom);
		}
		nt.Disconnect();
	}
	else
	{
		std::cout << "IP address:";
		std::cin >> ip;
		nt.StartAsClient(ip);
		while (!nt.IsIncome());
		side = nt.ReadSpecialMessage();
		while (nt.IsConnection() && (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board)))
		{
			if (nt.IsIncome())
			{
				if (nt.IsSpecial())
					nt.ReadSpecialMessage();
				if (!nt.IsConnection())
					break;
				RecreateMove(board, nt.RecreateMove());
			}
			if (board.GetIsWhiteMove() != side)
			{
				std::cout << "Waiting...";
				std::cout << "\r\r\r\r\r\r\r\r\r\r";
				continue;
			}
			auto recr = FormQuire(board);
			nt.TellMove(recr.move, recr.prom);
		}
		nt.Disconnect();
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