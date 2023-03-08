#pragma once
#pragma comment(lib, "opengl32.lib")

#include "Board.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

class WinInterface
{
public:
	static chess_lib::GlobMove FormQuire(chess_lib::Board& board);
	static chess_lib::GlobMove ConvertFromString(std::string str);
	static std::string ConvertToString(chess_lib::GlobMove mov);
	static bool IsLegal(const std::string str);
	static bool RecreateMove(chess_lib::Board& board, const chess_lib::GlobMove mov);
};

