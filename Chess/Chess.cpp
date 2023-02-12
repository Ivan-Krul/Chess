#include <iostream>
#define SOCKET_MESSAGE_SIZE 64
#include "Socket.h"
#include "Socket.cpp"

#include "MoveGenerator.h"
#include "Board.h"

int main(int argc, char const* argv[])
{
/*	auto b = network_lib::Booter::GetInstance();
	b.Init();
	network_lib::Socket socket;
	socket.SetPort(5001);

	char c = 0;

	std::cout << socket.MessageSize() << '\n';

	while(c != 'c' && c != 's')
		std::cin >> c;

	if (c == 'c')
	{
		std::string ip;
		std::cin >> ip;
		if (socket.Connect(ip) != 0)
		{
			std::cout << "Oh no!\n";
			system("pause");
			exit(0);
		}
		while (!socket.CanBeRecievedFromThisSocket());
		std::cout << "Message: \"" << socket.GetMessage_() << "\"\n";

		socket.SendToThisSocket("Hello, from client!");
		std::cout << "Send!\n";

		while (!socket.CanBeRecievedFromThisSocket());
		std::cout << "Message: \"" << socket.GetMessage_() << "\"\n";
	}
	else
	{
		socket.Listen();
		network_lib::Socket client_sock(false);
		while (socket.CanBeAccepted(client_sock))
		{
			std::cout << "Connected!\n";
			client_sock.SelectEventForBind(0);

			client_sock.SendToThisSocket("Hello!, from server!");
			std::cout << "Send!\n";

			while (!client_sock.CanBeRecievedFromThisSocket());
			std::cout << "Message: \"" << client_sock.GetMessage_() << "\"\n";

			client_sock.SendToThisSocket("Hello!, from server!");
			std::cout << "Send!\n";

			break;
		}

	}*/

	auto board = chess_lib::Board();
	auto arr = board.GetBoard();
	auto i = 0;
	auto mstring = std::string();
	auto m1 = 0;
	chess_lib::MoveGenerator mg;
	while (true)
	{
		for(uint8_t i = 0; i < 64; i++)
		{
			switch (arr[i].type)
			{
				case chess_lib::PieceType::pawn:
					std::cout << char('p' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				case chess_lib::PieceType::rook:
					std::cout << char('r' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				case chess_lib::PieceType::knight:
					std::cout << char('n' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				case chess_lib::PieceType::bishop:
					std::cout << char('b' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				case chess_lib::PieceType::queen:
					std::cout << char('q' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				case chess_lib::PieceType::king:
					std::cout << char('k' + (uint8_t(arr[i].side) - 2) * ('a' - 'A'));
					break;
				default:
					if (board.GetPreviousMove()->GetP1() == i)
						std::cout << '*';
					else
						std::cout << '.';
					break;
			}
			std::cout << ' ';
			if ((i + 1) % 8 == 0)
			{
				std::cout << '\n';
			}
			
		}
		std::cout << '\n';
		std::cout << "B4.size(): " << mg.GeneratePawnMove(board,board.ConvertToIndex("B4")).size() << '\n';
		std::cout << "Move piece in coord(x,y): ";
		std::cin >> mstring;
		if (mstring == "resign")
			break;
		m1 = board.ConvertToIndex(mstring);
		std::cout << "Move piece to coord(x,y): ";
		std::cin >> mstring;
		if (!board.MovePiece(chess_lib::Move(m1, board.ConvertToIndex(mstring))))
			std::cout << "no move\n";
		arr = board.GetBoard();
		std::cout << '\n';
	}


	std::cout << "Chess!\n";
	//system("pause");
	return 0;
}