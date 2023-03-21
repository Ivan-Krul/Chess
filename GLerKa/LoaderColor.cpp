#include "LoaderColor.h"

namespace glerka_lib
{
	Color LoaderColor::getColorBoard(bool is_white)
	{
		return is_white ? m_ColBoardW : m_ColBoardB;
	}

	Color LoaderColor::getColorPiece(bool is_white)
	{
		return is_white ? m_ColPieceW : m_ColPieceB;
	}

	Color LoaderColor::getSelected(bool is_white)
	{
		auto def_col = getColorBoard(is_white);

		short r = def_col.r;
		short g = def_col.g;
		short b = def_col.b;

		r += (int8_t)m_ColDeltaSelect.r;
		g += (int8_t)m_ColDeltaSelect.g;
		b += (int8_t)m_ColDeltaSelect.b;

		r = __min(__max(r, 0), 255);
		g = __min(__max(g, 0), 255);
		b = __min(__max(b, 0), 255);

		return Color{ (uint8_t)r, (uint8_t)g, (uint8_t)b };
	}

	void LoaderColor::setColors(Color cboardw, Color cboardb, Color cpiecew, Color cpieceb, Color cdeltas)
	{
		m_ColBoardW = cboardw;
		m_ColBoardB = cboardb;
		m_ColPieceW = cpiecew;
		m_ColPieceB = cpieceb;
		m_ColDeltaSelect = cdeltas;
	}

	void LoaderColor::load()
	{
		std::ifstream fin(c_Directory);

		if (!fin.is_open())
		{
			m_ColBoardW = c_Stand[0];
			m_ColBoardB = c_Stand[1];
			m_ColPieceW = c_Stand[2];
			m_ColPieceB = c_Stand[3];
			m_ColDeltaSelect = c_Stand[4];
			return;
		}

		bool is_my_part = false;
		bool breakpoints[] = { false, false, false, false, false };
		auto statement = 0;
		auto vec_state = 0;
		auto str = std::string();
		auto is_break = true;
		auto filler = std::array<uint8_t, 15>();

		while (fin >> str)
		{
			if (is_my_part)
			{
				if (str == "board_w")
					statement = 0;
				else if (str == "board_b")
					statement = 1;
				else if (str == "piece_w")
					statement = 2;
				else if (str == "piece_b")
					statement = 3;
				else if (str == "delta")
					statement = 4;
				else if (str == "end")
					breakpoints[statement] = true;
				else
				{
					filler[statement * 3 + vec_state % 3] = uint8_t(std::stoi(str));
					vec_state++;
				}
			}

			if (str == "color")
				is_my_part = true;

			if (!is_my_part)
				continue;

			for (auto& bp : breakpoints)
			{
				if (bp)
					continue;
				is_break = false;
				break;
			}
			if (is_break)
				break;
		}

		fin.close();

		m_ColBoardW = {			filler[0 + 0],	filler[1 + 0],	 filler[2 + 0] };
		m_ColBoardB = {			filler[0 + 3],	filler[1 + 3],	 filler[2 + 3] };
		m_ColPieceW = {			filler[0 + 6],	filler[1 + 6],	 filler[2 + 6] };
		m_ColPieceB = {			filler[0 + 9],	filler[1 + 9],	 filler[2 + 9] };
		m_ColDeltaSelect = {	filler[0 + 12],	filler[1 + 12],	 filler[2 + 12] };
	}
}
