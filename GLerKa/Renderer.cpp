#include "Renderer.h"

namespace glerka_lib
{
	Renderer::Renderer()
	{
		m_lc.Load();
		m_lm.Load();
		float scale = 1.0f;
		glScalef(4.0f * scale, 4.0f * scale, 0.0f);
	}

	void Renderer::RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index)
	{
		const auto& tile = board.GetBoard()[index];
		auto col_board = m_lc.getColorBoard(index % 2);

		if (board.GetPreviousMove()->GetP1() == index || board.GetPreviousMove()->GetP2() == index)
			col_board = m_lc.getSelected(index % 2);

		const auto col_piece = m_lc.getColorPiece(index % 2);
		const auto ver_piece = m_lm.getPiece(tile.type);

		glBegin(GL_TRIANGLE_FAN);
			glColor3ub(col_board.r, col_board.g, col_board.b);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);
		glEnd();

		glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, ver_piece.data());
			glColor3ub(col_piece.r, col_piece.g, col_piece.b);
			glDrawElements(GL_TRIANGLE_STRIP, ver_piece.size(), GL_FLOAT, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Renderer::RenderBoard(const chess_lib::Board& board)
	{
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
}
