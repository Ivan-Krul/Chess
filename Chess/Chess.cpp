#include <iostream>

//#include "ConRenderer.h"
#include "MoveController.h"
//#include "TypeInterface.h"
//#include "NetworkTeller.h"

#include "Renderer.h"
#include "Renderer.cpp"

#include <cassert>

int main(int argc, char const* argv[])
{
	chess_lib::Board b("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ");
	glerka_lib::Renderer r(640, 480);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Oops... We have trouble to initialize glfw...\n";
		std::cin.get();
		return -1;
	}
	std::cout << "D: initialize is done\n";

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Chess", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Oops... We have trouble to create a window...\n";
		std::cin.get();
		return -1;
	}
	std::cout << "D: creating window is done\n";

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		r.RenderBoard(b, b.GetIsWhiteMove());
		r.HandleClick(window, b);
		r.HandlePromotion(window, b);
		r.HandleMate(window, b);
		r.HandleStalemate(window, b);
		r.HandleDraw(window, b);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */

		glfwSetCursorPosCallback(window, (GLFWcursorposfun)glerka_lib::Renderer::HandleCursor);
		glfwPollEvents();
		glfwSetFramebufferSizeCallback(window, glerka_lib::Renderer::HandleResize);
		glViewport(0, 0, r.GetWidth(), r.GetHeight());
	}

	glfwTerminate();
	return 0;
}
