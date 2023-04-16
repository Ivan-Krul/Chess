#include "Renderer.h"

namespace glerka_lib
{
	Renderer::Renderer(int width, int height)
	{
		m_lc.load();
		m_lm.load();
		m_CWidthPx = width;
		m_CHeightPx = height;
		m_WidthPx = width;
		m_HeightPx = height;
		m_ClickCoord = -1;
		Load();

	}

	void Renderer::RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index, const bool is_selected, const bool is_check)
	{
		const auto& tile = board.GetBoard()[index];
		const auto col_piece = m_lc.getColorPiece(board.GetBoard()[index].side == chess_lib::SideType::white);
		const auto ver_piece = m_lm.getPiece(tile.type);

		auto col_board = Color();

		if (is_selected || board.GetPreviousMove()->GetP1() == index || board.GetPreviousMove()->GetP2() == index)
			col_board = m_lc.getSelected((index % 8 + index / 8) % 2);
		else if (is_check)
		{
			col_board = m_lc.getSelected((index % 8 + index / 8) % 2);
			col_board.r = ~col_board.r;
			col_board.g = ~col_board.g;
			col_board.b = ~col_board.b;
		}
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
			glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)ver_piece.size() / 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Renderer::RenderBoard(const chess_lib::Board& board, bool is_white_side)
	{
		glLoadIdentity();
		glTranslatef(-1.0f, -1.0f, 0.0f);
		glScalef(2.0f / 8.0f, 2.0f / 8.0f, 1.0f);


		// vision
		bool is_white_vision = m_CFGNeedRotate
			? (m_CFGPlayAsBlackVision 
				? !is_white_side
				: is_white_side)
			: !m_CFGPlayAsBlackVision;
		m_IsSwapedN = !is_white_vision;

		bool is_check = false;
		if(m_CFGLightCheck)
		{
			chess_lib::MoveGenerator mg;
			is_check = mg.CanKingBeInCheck(board, !is_white_side);
		}

		// mouse
		auto conv_to_index = [=](uint8_t x, uint8_t y) { return x + y * 8; };

		std::vector<chess_lib::Move> selected;
		uint8_t x = static_cast<uint8_t>(is_white_side ? std::floor(GetCurPosX() * 8.0) : (7 - std::floor(GetCurPosX() * 8.0)));
		uint8_t y = static_cast<uint8_t>(is_white_side ? std::floor(GetCurPosY() * 8.0) : (7 - std::floor(GetCurPosY() * 8.0)));
		unsigned cur_coord = conv_to_index(x, y);
		bool hold = false;
		if (~m_ClickCoord)
		{
			auto mc = chess_lib::MoveController::GetInstance();
			selected = mc.Proposition(board, m_ClickCoord);
		}


		// render loop
		auto light_check = [=](uint8_t index) { 
			bool is_king = board.GetBoard().at(index).type == chess_lib::PieceType::king;
			return is_check && ((board.GetBoard().at(index).side == chess_lib::SideType::white) == is_white_side) && is_king;
		};
		for (uint8_t y = 0; y < 8; y++)
		{
			for (uint8_t x = 0; x < 8; x++)
			{
				hold = false;
				if (selected.size())
				{
					for (auto& s : selected)
					{
						if (!(s.GetP1() == conv_to_index(x, y) || s.GetP2() == conv_to_index(x, y)))
							continue;

						hold = true;
						break;
					}
				}
				glPushMatrix();
				glTranslatef(float(is_white_vision ? x : 7 - x), float(is_white_vision ? 7 - y : y), 0.0f);
				
				RenderUnPushedSquare(board, conv_to_index(x, y), hold, light_check(conv_to_index(x, y)));
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (m_ClickCoord == -1)
		{
			m_ClickCoord = GetCurPos(board.GetIsWhiteMove());
			if (board.GetBoard()[m_ClickCoord].side != (board.GetIsWhiteMove() ? chess_lib::SideType::white : chess_lib::SideType::black))
				m_ClickCoord = -1;
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

			m_ClickCoord = -1;
		}
	}

	void Renderer::HandlePromotion(GLFWwindow* window, chess_lib::Board& board)
	{
		auto& mc = chess_lib::MoveController::GetInstance();
		if (!mc.NeedPromotion(board))
			return;

		// render message
		glTranslatef(0.0f, 6.0f, 0.0f);
		RenderWord("promotion", { 255,0,0 }, 5.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);
		RenderWord("press key:", { 255,0,0 }, 5.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);
		RenderWord("Q - queen", { 255,0,0 }, 5.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);
		RenderWord("N - knight", { 255,0,0 }, 5.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);
		RenderWord("R - rool", { 255,0,0 }, 5.0f);
		glTranslatef(0.0f, -1.0f, 0.0f);
		RenderWord("B - bishop", { 255,0,0 }, 5.0f);

		// checking key
		if (glfwGetKey(window, 'Q'))
			mc.PromoteTo(board, chess_lib::PromotionChoice::queen);
		else if (glfwGetKey(window, 'N'))
			mc.PromoteTo(board, chess_lib::PromotionChoice::knight);
		else if (glfwGetKey(window, 'R'))
			mc.PromoteTo(board, chess_lib::PromotionChoice::rook);
		else if (glfwGetKey(window, 'B'))
			mc.PromoteTo(board, chess_lib::PromotionChoice::bishop);
	}

	void Renderer::HandleMate(GLFWwindow* window, const chess_lib::Board& board)
	{
		auto& mc = chess_lib::MoveController::GetInstance();
		if (!mc.IsMate(board))
			return;

		glTranslatef(0.0f, 6.0f, 0.0f);
		RenderWord("mate", { 255,0,0 }, 5.0f);
	}

	void Renderer::HandleDraw(GLFWwindow* window, const chess_lib::Board& board)
	{
		auto& mc = chess_lib::MoveController::GetInstance();
		if (!mc.IsDraw(board))
			return;

		glTranslatef(0.0f, 6.0f, 0.0f);
		RenderWord("draw", { 255,0,0 }, 5.0f);
	}

	void Renderer::HandleStalemate(GLFWwindow* window, const chess_lib::Board& board)
	{
		auto& mc = chess_lib::MoveController::GetInstance();
		if (!mc.IsStalemate(board))
			return;

		glTranslatef(0.0f, 6.0f, 0.0f);
		RenderWord("stalemate", { 255,0,0 }, 5.0f);
	}

	void Renderer::Load()
	{
		std::ifstream fin;

		fin.open(PATH_TO_CONFIG);

		if (!fin.is_open())
			return;

		nlohmann::json json_file = nlohmann::json::parse(fin);

		fin.close();

		m_CFGNeedRotate = json_file["need rotate after move"];
		m_CFGPlayAsBlackVision = json_file["vision as black"];
		m_CFGLightCheck = json_file["light up when check"];

		m_IsSwapedN = m_CFGPlayAsBlackVision;
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
		if (m_CFGNeedRotate)
		{
			auto x = ((!m_IsSwapedN) ? std::floor(GetCurPosX() * 8.0) : (7 - std::floor(GetCurPosX() * 8.0)));
			auto y = ((!m_IsSwapedN) ? std::floor(GetCurPosY() * 8.0) : (7 - std::floor(GetCurPosY() * 8.0)));
			return static_cast<uint8_t>(x + y * 8.0);
		}
		else
		{
			auto x = (!m_CFGPlayAsBlackVision ? std::floor(GetCurPosX() * 8.0) : (7 - std::floor(GetCurPosX() * 8.0)));
			auto y = (!m_CFGPlayAsBlackVision ? std::floor(GetCurPosY() * 8.0) : (7 - std::floor(GetCurPosY() * 8.0)));
			return static_cast<uint8_t>(x + y * 8.0);
		}
	}

	int Renderer::m_CWidthPx = 1;
	int Renderer::m_CHeightPx = 1;
	int Renderer::m_WidthPx = 1;
	int Renderer::m_HeightPx = 1;
	double Renderer::m_CurPosX = 1;
	double Renderer::m_CurPosY = 1;
}
