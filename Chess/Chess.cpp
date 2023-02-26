#include <iostream>

#include "ConRenderer.h"
#include "MoveController.h"
#include "TypeInterface.h"
#include "NetworkTeller.h"

int main(int argc, char const* argv[])
{
	auto nt = NetworkTeller::getInstance();
	bool n_is_server;

	auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();
	bool side_white = false;

	if (nt.InitWSA() != 0)
	{
		std::cout << "Network service: WSA isn't startup correctly, abort\n";
		system("pause");
		return 0;
	}

	std::cout << "Chess\n";
	std::cout << "Type of connection(0 - client, 1 - server): ";
	std::cin >> n_is_server;

	if (n_is_server)
		side_white = nt.InitSocketsAsServer();
	else
	{
		std::string n_ip;
		// input a IP address
		std::cout << "IP address: ";
		std::cin >> n_ip;

		// Connect to the server using a socket and wait for an initialization message
		if ((side_white = nt.InitSocketAsClient(n_ip)) == SOCKET_ERROR)
		{
			std::cout << "Network service: Connection proccess has failured, abort\n";
			system("pause");
			return 0;
		}
		std::cout << "Connected\n";
	}

	std::cout << "You are playing as " << (side_white ? "white" : "black") << '\n';

	while (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board))
	{
		if (side_white != board.GetIsWhiteMove())
		{
			
			std::cout << "Waiting for further action...\n";
			
			// checking for status
			if (nt.Listen() == SOCKET_ERROR)
				break;
			if (nt.isQuitMsg())
				break;

			// checking is message correct
			if (!nt.isLegalMove())
			{
				// handle it
				std::cout << "Network service: The message, what was recieved isn't correct\n";
				continue;
			}
			
			// convert and recreate a move
			auto gm = nt.ConvertToMove();
			if (!TypeInterface::RecreateMove(board, gm))
				std::cout << "Network service: The move, what was recieved is rejected\n";

			// checking for cases of mate, stalemate or draw
			continue;
		}

		// render and make a move
		std::cout << r.Render(board, board.GetIsWhiteMove());
		auto gm = TypeInterface::FormQuire(board);
		nt.SendMove(gm);
	}
	nt.SendQuitMessage();

	WSACleanup();

	if (mc.IsMate(board))
		std::cout << "Mate!\n" << (!board.GetIsWhiteMove() ? "White" : "Black") << " wins\n";
	else if (mc.IsStalemate(board))
		std::cout << "Stalemate!\nDraw\n";
	else if (mc.IsDraw(board))
		std::cout << "Draw!\n";
	else
		std::cout << "Game was aborted!\n";

	system("pause");
	return 0;
}