#include <iostream>

#include "MoveController.h"

#include "Renderer.h"
#include "Renderer.cpp"

int main(int argc, char const* argv[])
{
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
