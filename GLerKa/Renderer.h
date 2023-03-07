#pragma once
#include "Depencies.h"

#include "LoaderColor.h"
#include "LoaderModel.h"

#include "Board.h"

namespace glerka_lib
{
	class Renderer
	{
		static int m_CWidthPx;
		static int m_CHeightPx;
		static int m_WidthPx;
		static int m_HeightPx;

		static double m_CurPosX;
		static double m_CurPosY;
	public:
		Renderer(int width, int height);
		void RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index);
		void RenderBoard(const chess_lib::Board& board);
		static void HandleResize(GLFWwindow* window, int width, int height);
		static void HandleCursor(GLFWwindow* window, double width, double height);

		int GetWidth() const;
		int GetHeight() const;
		double GetCurPosX() const;
		double GetCurPosY() const;
	private:
		LoaderModel m_lm;
		LoaderColor m_lc;
	};
}
