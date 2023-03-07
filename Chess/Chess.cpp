#include <iostream>

//#include "ConRenderer.h"
#include "MoveController.h"
//#include "TypeInterface.h"
//#include "NetworkTeller.h"

#pragma comment(lib, "opengl32.lib")
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Renderer.cpp"

int main(int argc, char const* argv[])
{
	/*
	auto n_booter = network_lib::Booter::GetInstance();

	if (n_booter.Init() != 0)
	{
		std::cout << "Network service: WSA isn't startup correctly, abort\n";
		system("pause");
		return 0;
	}

	auto nt = NetworkTeller();
	bool n_is_server;

	auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();
	bool side_white = false;

	std::cout << "Chess\n";
	std::cout << "Type of connection(0 - client, 1 - server): ";
	std::cin >> n_is_server;

	if (n_is_server)
	{
		std::cout << "Waiting for connection...";
		side_white = nt.InitSocket(n_is_server);
	}
	else
	{
		std::string n_ip;
		// input a IP address
		std::cout << "IP address: ";
		std::cin >> n_ip;

		// Connect to the server using a socket and wait for an initialization message
		if ((side_white = nt.InitSocket(n_is_server, n_ip)) == SOCKET_ERROR)
		{
			std::cout << "Network service: Connection proccess has failured, abort\n";
			system("pause");
			return 0;
		}
		std::cout << "Connected\n";
	}
	std::cout << "You are playing as " << (side_white ? "white" : "black") << '\n';

	// loop
	while (!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board))
	{
		if (side_white != board.GetIsWhiteMove())
		{
			std::cout << "Waiting for further action...\n";

			// getting a quert and checking for status
			if (nt.Listen() == SOCKET_ERROR)
				break;
			else if (nt.isQuitMsg())
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

	// checking for situation of ending a game
	if (mc.IsMate(board))
		std::cout << "Mate!\n" << (!board.GetIsWhiteMove() ? "White" : "Black") << " wins\n";
	else if (mc.IsStalemate(board))
		std::cout << "Stalemate!\nDraw\n";
	else if (mc.IsDraw(board))
		std::cout << "Draw!\n";
	else
		std::cout << "Game was aborted!\n";
	*/

	/*auto mc = chess_lib::MoveController::GetInstance();
	auto r = ConRenderer();
	auto board = chess_lib::Board();

	while ((!mc.IsMate(board) && !mc.IsStalemate(board) && !mc.IsDraw(board)))
	{
		std::cout << r.Render(board, board.GetIsWhiteMove());
		TypeInterface::FormQuire(board);
	}

	if (mc.IsMate(board))
		std::cout << "Mate!\n" << (!board.GetIsWhiteMove() ? "White" : "Black") << " wins\n";
	else if (mc.IsStalemate(board))
		std::cout << "Stalemate!\nDraw\n";
	else
		std::cout << "Draw!\n";
	*/
	chess_lib::Board b;
	glerka_lib::Renderer r(640, 480);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		r.RenderBoard(b);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */

		glfwSetCursorPosCallback(window, (GLFWcursorposfun)glerka_lib::Renderer::HandleCursor);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			std::cout << "Click!\n";
		}

		glfwPollEvents();
		glfwSetFramebufferSizeCallback(window, glerka_lib::Renderer::HandleResize);
		glViewport(0, 0, r.GetWidth(), r.GetHeight());
	}

	glfwTerminate();
	return 0;
}
