#include <iostream>

#include "Socket.h"
#include "ConRenderer.h"
#include "MoveController.h"
#include "TypeInterface.h"

#define PORT 5020

int main(int argc, char const* argv[])
{
	auto n_booter = network_lib::Booter::GetInstance();

	if (n_booter.Init() != 0)
	{
		std::cout << "Network service: WSA isn't startup correctly, abort\n";
		system("pause");
		return 0;
	}

	auto		 			n_sock = network_lib::Socket();
	auto		    		n_sockcl = network_lib::Socket(false);
	network_lib::Socket*	n_use_sock;
	auto		    		n_msg = std::string();
	auto					n_ip = std::string();
	bool		    		n_is_server = false;

	auto			mc = chess_lib::MoveController::GetInstance();
	auto			r = ConRenderer();
	auto			board = chess_lib::Board();
	bool			side_white = false;

	n_sock.SetPort(PORT);

	std::cout << "Chess\n";
	std::cout << "Type of connection(0 - client, 1 - server): ";
	std::cin >> n_is_server;

	if (n_is_server)
	{
		// Create a listening socket and wait for a connection
		n_sock.Listen();
		std::cout << "Waiting for connection...\n";
		while (!n_sock.CanBeAccepted(n_sockcl));

		// Use the accepted socket for communication and randomly decide which side to play
		n_use_sock = &n_sockcl;
		srand((unsigned int)time(NULL));
		side_white = rand() % 2;
		
		// Send an initialization message to the client
		n_msg = "$InIt%";
		n_msg += (side_white ? '0' : '1');
		std::cout << side_white << "vs" << n_msg[6] << "\n";
		n_use_sock->SendToThisSocket(n_msg);
	}
	else
	{
		// input a IP address
		std::cout << "IP address: ";
		std::cin >> n_ip;

		// Connect to the server using a socket and wait for an initialization message
		if (n_sock.Connect(n_ip) == SOCKET_ERROR)
		{
			std::cout << "Network service: Connection proccess has failured, abort\n";
			system("pause");
			return 0;
		}
		n_use_sock = &n_sock;
		std::cout << "Connected\n";

		// Getting statement of turn
		while (!(n_use_sock->CanBeRecievedFromThisSocket() > 0));
		n_msg = n_use_sock->GetMessage_();
		side_white = n_msg[6] != '0';
	}

	std::cout << "You are playing as " << (side_white ? "white" : "black") << '\n';

	while (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board))
	{
		if (side_white != board.GetIsWhiteMove())
		{
			// we're waiting for opponent move
			// we're looping, cuz we're waiting for his move
			// listen for an incoming message
			std::cout << "Waiting for further action...\n";
			int n_status;
			while (!(n_status = n_use_sock->CanBeRecievedFromThisSocket()));
			n_msg = n_use_sock->GetMessage_();

			// checking for status
			if (n_status == SOCKET_ERROR)
				break;

			// is it special message?
			if (n_msg == "$QuIt")
				break;

			// checking is message correct
			if (!TypeInterface::IsLegal(n_msg))
			{
				// handle it
				std::cout << "Network service: The message, what was recieved isn't correct\n";
				continue;
			}
			
			// convert and recreate a move
			auto gm = TypeInterface::ConvertFromString(n_msg);
			if (!TypeInterface::RecreateMove(board, gm))
			{
				// handle it
				std::cout << "Network service: The move, what was recieved is rejected\n";
			}

			// checking for cases of mate, stalemate or draw
			continue;
		}

		// render and make a move
		std::cout << r.Render(board, board.GetIsWhiteMove());
		auto gm = TypeInterface::FormQuire(board);

		// prepare a move and send it for opponent
		n_msg = TypeInterface::ConvertToString(gm);
		n_use_sock->SendToThisSocket(n_msg);
	}
	
	n_use_sock->SendToThisSocket("$QuIt");
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