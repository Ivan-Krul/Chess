#include <iostream>
#define SOCKET_MESSAGE_SIZE 64
#include "Socket.h"
#include "Socket.cpp"

#include "MoveController.h"

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
	auto m1 = std::string();
	auto m2 = std::string();
	auto mc = chess_lib::MoveController::GetInstance();

	while (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board))
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
		std::cout << "Check: " << mc.IsCheck(board) << '\n';
		
		std::cout << "Move a piece in: ";
		std::cin >> m1;
		std::cout << "Move a piece to: ";
		std::cin >> m2;

		if (!mc.ConfirmMove(board, chess_lib::Move{board.ConvertToIndex(m1),board.ConvertToIndex(m2) }))
			std::cout << "no move\n";

		if (board.NeedPromotion())
		{
			std::string s;
		ty_again:
			std::cout << "Choose a promotion piece in " << board.ConvertFromIndex(board.GetPreviousMove()->GetP2()) << ": ";
			std::cin >> s;
			if (s == "queen")
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::queen);
			else if (s == "bishop")
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::bishop);
			else if (s == "knight")
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::knight);
			else if (s == "rook")
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::rook);
			else
				goto ty_again;
		}

		arr = board.GetBoard();		

		std::cout << '\n';
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