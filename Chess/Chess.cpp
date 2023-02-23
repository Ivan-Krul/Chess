#include <iostream>

#include "Socket.h"
#include "ConRenderer.h"
#include "MoveController.h"
#include "TypeInterface.h"

#define PORT 5020
#define IPAD "127.0.0.1"

int main(int argc, char const* argv[])
{
	/*	
	auto b = network_lib::Booter::GetInstance();
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

	auto n_booter = network_lib::Booter::GetInstance();
	n_booter.Init();

	auto n_sock = network_lib::Socket();
	auto n_sockcl = network_lib::Socket(false);
	auto n_is_server = false;
	network_lib::Socket* n_use_sock;
	auto n_connected = false;
	auto n_msg = std::string();

	auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();
	auto side_white = false;

	n_sock.SetPort(PORT);

	

	std::cout << "Type of connection(0 - client, 1 - server): ";
	std::cin >> n_is_server;

	if (n_is_server)
	{
		n_sock.Listen();
		std::cout << "Waiting";
		while (!n_sock.CanBeAccepted(n_sockcl))
		{
			std::cout << '.';
			std::cout << '\b';
			std::cout << ' ';
			std::cout << '\b';
		}
		std::cout << '\n';

		
		n_use_sock = &n_sockcl;
		n_connected = true;
		side_white = rand() % 2;

		n_use_sock->SelectEventForBind(0);
		n_msg = "$InIt%";
		n_msg += (side_white ? '0' : '1');
		n_use_sock->SendToThisSocket(n_msg);
	}
	else
	{
		//auto ip = std::string();
		//std::cout << "IP address: ";
		//std::cin >> ip;

		n_sock.Connect(IPAD);

		n_use_sock = &n_sock;
		n_connected = true;

		while (!n_use_sock->CanBeRecievedFromThisSocket());
		n_msg = n_use_sock->GetMessage_();
		side_white = n_msg[7] - '0';
	}

	std::shared_ptr<chess_lib::GlobMove> gm;
	
	std::cout << r.Render(board, board.GetIsWhiteMove()) << '\n';

	while ((!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board)))
	{
		if (n_use_sock->CanBeRecievedFromThisSocket())
		{
			std::cout << n_use_sock->GetMessage_() << '\n';
			if (n_use_sock->GetMessage_() == "$QuIt")
				break;
			if (n_use_sock->GetMessage_()[0] == '$')
				goto skip;
			if (!TypeInterface::IsLegal(n_use_sock->GetMessage_()))
			{
				std::cout << "bad query!\n";
				continue;
			}
			gm = std::make_shared<chess_lib::GlobMove>(TypeInterface::ConvertFromString(n_use_sock->GetMessage_()));
			TypeInterface::RecreateMove(board, *gm);
			
		}
		skip:
		if (side_white != board.GetIsWhiteMove())
			continue;
		std::cout << r.Render(board, board.GetIsWhiteMove());
		gm = std::make_shared<chess_lib::GlobMove>(TypeInterface::FormQuire(board));
		n_msg = TypeInterface::ConvertToString(*gm);
		n_use_sock->SendToThisSocket(n_msg);
		
	}

	n_use_sock->SendToThisSocket("$QuIt");
	
	if (mc.IsMate(board))
		std::cout << "Mate!\n" << (!board.GetIsWhiteMove() ? "White" : "Black") << " wins\n";
	else if(mc.IsStalemate(board))
		std::cout << "Stalemate!\nDraw\n";
	else
		std::cout << "Draw!\n";
	system("pause");
	return 0;
}