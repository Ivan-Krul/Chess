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
		m_ClickCoord = -1;
	}

	void Renderer::RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index, const bool is_selected)
	{
		const auto& tile = board.GetBoard()[index];
		const auto col_piece = m_lc.getColorPiece(board.GetBoard()[index].side == chess_lib::SideType::white);
		const auto ver_piece = m_lm.getPiece(tile.type);

		auto col_board = Color();

		if (is_selected || board.GetPreviousMove()->GetP1() == index || board.GetPreviousMove()->GetP2() == index)
			col_board = m_lc.getSelected((index % 8 + index / 8) % 2);
		else
			col_board = m_lc.getColorBoard((index % 8 + index / 8) % 2);


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

	void Renderer::RenderBoard(const chess_lib::Board& board, const bool is_white_side)
	{
		glLoadIdentity();
		glTranslatef(-1.0f, -1.0f, 0.0f);
		glScalef(2.0f / 8.0f, 2.0f / 8.0f, 1.0f);

		m_IsSwapedN = is_white_side;

		auto selected = std::vector<chess_lib::Move>();
		auto conv_to_index = [=](uint8_t x, uint8_t y) {return x + y * 8;  };
		unsigned cur_coord = conv_to_index((is_white_side ? std::floor(GetCurPosX() * 8.0) : (7 - std::floor(GetCurPosX() * 8.0))), (is_white_side ? std::floor(GetCurPosY() * 8.0) : (7 - std::floor(GetCurPosY() * 8.0))));
		//auto norm_predict = [=](uint8_t x, uint8_t y) { return (is_white_side ? (x + (7 - y) * 8) : ((7 - x) + y * 8)); };


		auto hold = false;
		if ((~m_ClickCoord))
		{
			auto mc = chess_lib::MoveController::GetInstance();
			selected = mc.Proposition(board, m_ClickCoord);
		}
		
		for (uint8_t y = 0; y < 8; y++)
		{
			for (uint8_t x = 0; x < 8; x++)
			{
				hold = false;
				if (selected.size())
				{
					for (auto& s : selected)
					{
						if (s.GetP1() == conv_to_index(x, y) || s.GetP2() == conv_to_index(x, y))
						{
							hold = true;
							break;
						}
					}
				}
				glPushMatrix();
				glTranslatef(is_white_side ? x : 7 - x, is_white_side ? 7 - y : y, 0.0f);
				RenderUnPushedSquare(board, conv_to_index(x, y), hold);
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

	void Renderer::HandleClick(GLFWwindow* window, const chess_lib::Board& board)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
			return;
		printf("%c: ", board.GetIsWhiteMove() ? 'w' : 'b');
		printf("click");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (m_ClickCoord == -1)
		{
			m_ClickCoord = GetCurPos(board.GetIsWhiteMove());
			if (board.GetBoard()[m_ClickCoord].side != (board.GetIsWhiteMove() ? chess_lib::SideType::white : chess_lib::SideType::black))
				m_ClickCoord = -1;
			printf(" {%d}\n", m_ClickCoord);
		}
		else
		{
			// we have a coordinates of click, so, we're making move
			uint8_t click_coord = GetCurPos(board.GetIsWhiteMove());
			auto selected = std::vector<chess_lib::Move>();
			auto mc = chess_lib::MoveController::GetInstance();

			if (board.GetBoard()[click_coord].side == (board.GetIsWhiteMove() ? chess_lib::SideType::white : chess_lib::SideType::black))
			{
				m_ClickCoord = click_coord;
				printf(" {%d}\n", m_ClickCoord);
				return;
			}

			selected = mc.Proposition(board, m_ClickCoord);

			for (auto& m : selected)
			{
				if (m.GetP2() == click_coord)
				{
					mc.ConfirmMove(board, chess_lib::Move{ static_cast<uint8_t>(m_ClickCoord), click_coord });
					break;
				}
			}

			printf(" {%d, %d}\n", m_ClickCoord, click_coord);

			m_ClickCoord = -1;
		}
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

	uint8_t Renderer::GetCurPos(const bool is_white)
	{
		auto x = ((is_white && m_IsSwapedN) ? std::floor(GetCurPosX() * 8.0) : (7 - std::floor(GetCurPosX() * 8.0)));
		auto y = ((is_white && m_IsSwapedN) ? std::floor(GetCurPosY() * 8.0) : (7 - std::floor(GetCurPosY() * 8.0)));
		return x + y * 8;
	}

	int Renderer::m_CWidthPx = 1;
	int Renderer::m_CHeightPx = 1;
	int Renderer::m_WidthPx = 1;
	int Renderer::m_HeightPx = 1;
	double Renderer::m_CurPosX = 1;
	double Renderer::m_CurPosY = 1;
}
