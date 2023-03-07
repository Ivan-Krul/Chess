#include "Renderer.h"

namespace glerka_lib
{
	Renderer::Renderer(int width, int height)
	{
		m_lc.Load();
		m_lm.Load();
		m_CWidthPx = width;
		m_CHeightPx = height;
		m_WidthPx = width;
		m_HeightPx = height;
	}

	void Renderer::RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index)
	{
		const auto& tile = board.GetBoard()[index];
		auto col_board = m_lc.getColorBoard((index % 8 + index / 8) % 2);

		if (board.GetPreviousMove()->GetP1() == index || board.GetPreviousMove()->GetP2() == index)
			col_board = m_lc.getSelected((index % 8 + index / 8) % 2);

		const auto col_piece = m_lc.getColorPiece(board.GetBoard()[index].side != chess_lib::SideType::white);
		const auto ver_piece = m_lm.getPiece(tile.type);

		glBegin(GL_TRIANGLE_FAN);
			glColor3ub(col_board.r, col_board.g, col_board.b);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);
		glEnd();

		glVertexPointer(2, GL_FLOAT, 0, ver_piece.data());
		glEnableClientState(GL_VERTEX_ARRAY);
			glColor3ub(col_piece.r, col_piece.g, col_piece.b);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, ver_piece.size() / 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Renderer::RenderBoard(const chess_lib::Board& board)
	{
		glLoadIdentity();
		glTranslatef(-1.0f, -1.0f, 0.0f);
		glScalef(2.0f / 8.0f, 2.0f / 8.0f, 1.0f);
		for (uint8_t x = 0; x < 8; x++)
		{
			for (uint8_t y = 0; y < 8; y++)
			{
				glPushMatrix();
					glTranslatef(x, y, 0.0f);
					RenderUnPushedSquare(board, x + y * 8);
				glPopMatrix();
			}
		}
	}

	void Renderer::HandleResize(GLFWwindow* window, int width, int height)
	{
		m_WidthPx = width;
		m_HeightPx = height;
	}

	void Renderer::HandleCursor(GLFWwindow* window, double width, double height)
	{
		m_CurPosX = width;
		m_CurPosY = height;
	}

	int Renderer::GetWidth() const
	{
		return m_WidthPx;
	}

	int Renderer::GetHeight() const
	{
		return m_HeightPx;
	}

	double Renderer::GetCurPosX() const
	{
		return m_CurPosX / (double)m_WidthPx;
	}

	double Renderer::GetCurPosY() const
	{
		return m_CurPosY / (double)m_HeightPx;
	}

	int Renderer::m_CWidthPx = 1;
	int Renderer::m_CHeightPx = 1;
	int Renderer::m_WidthPx = 1;
	int Renderer::m_HeightPx = 1;
	double Renderer::m_CurPosX = 1;
	double Renderer::m_CurPosY = 1;
}
