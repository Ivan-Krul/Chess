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
		std::ifstream fin;
		fin.open(PATH_TO_CONFIG);

		if (!fin.is_open())
		{
			m_ColBoardW = c_Stand[0];
			m_ColBoardB = c_Stand[1];
			m_ColPieceW = c_Stand[2];
			m_ColPieceB = c_Stand[3];
			m_ColDeltaSelect = c_Stand[4];
			return;
		}

		nlohmann::json json_file = nlohmann::json::parse(fin);

		fin.close();

		m_ColBoardW.r = json_file["color"]["board_w"][0];
		m_ColBoardW.g = json_file["color"]["board_w"][1];
		m_ColBoardW.b = json_file["color"]["board_w"][2];
		
		m_ColBoardB.r = json_file["color"]["board_b"][0];
		m_ColBoardB.g = json_file["color"]["board_b"][1];
		m_ColBoardB.b = json_file["color"]["board_b"][2];

		m_ColPieceW.r = json_file["color"]["piece_w"][0];
		m_ColPieceW.g = json_file["color"]["piece_w"][1];
		m_ColPieceW.b = json_file["color"]["piece_w"][2];

		m_ColPieceB.r = json_file["color"]["piece_b"][0];
		m_ColPieceB.g = json_file["color"]["piece_b"][1];
		m_ColPieceB.b = json_file["color"]["piece_b"][2];

		m_ColDeltaSelect.r = json_file["color"]["delta"][0];
		m_ColDeltaSelect.g = json_file["color"]["delta"][1];
		m_ColDeltaSelect.b = json_file["color"]["delta"][2];
	}
}
