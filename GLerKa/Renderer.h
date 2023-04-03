#pragma once
#include "Depencies.h"

#include "LoaderColor.h"
#include "LoaderModel.h"

#include "MoveController.h"

#include "SegmentLineRender.h"

#include <fstream>
#include <chrono>
#include <thread>

namespace glerka_lib
{
	class Renderer
	{
	public:
		Renderer(int width, int height);
		void RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index, const bool is_selected, const bool is_check);
		void RenderBoard(const chess_lib::Board& board, bool is_white_side = true);
		static void HandleResize(GLFWwindow* window, int width, int height);
		static void HandleCursor(GLFWwindow* window, double width, double height);
		void HandleClick(GLFWwindow* window, const chess_lib::Board& board);
		void HandlePromotion(GLFWwindow* window, chess_lib::Board& board);
		void HandleMate(GLFWwindow* window, const chess_lib::Board& board);
		void HandleDraw(GLFWwindow* window, const chess_lib::Board& board);
		void HandleStalemate(GLFWwindow* window, const chess_lib::Board& board);
		void Load();

		int GetWidth() const;
		int GetHeight() const;
		double GetCurPosX() const;
		double GetCurPosY() const;
	private:
		static int m_CWidthPx;
		static int m_CHeightPx;
		static int m_WidthPx;
		static int m_HeightPx;

		static double m_CurPosX;
		static double m_CurPosY;

		bool m_CFGNeedRotate = true;
		bool m_CFGPlayAsBlackVision = false;
		bool m_CFGLightCheck = true;

		bool m_IsSwapedN = false;

		int8_t m_ClickCoord;

		LoaderModel m_lm;
		LoaderColor m_lc;

		uint8_t GetCurPos(const bool is_white);
	};
}
