#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <array>

#include "ConfigPath.h"

namespace glerka_lib
{
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	class LoaderColor
	{
	public:
		Color getColorBoard(bool is_white);
		Color getColorPiece(bool is_white);
		Color getSelected(bool is_white);

		void setColors(Color cboardw, Color cboardb, Color cpiecew, Color cpieceb, Color cdeltas);
		void load();
	private:
		const std::vector<Color> c_Stand = { Color{221,221,221}, Color{34,34,34}, Color{232, 167, 120}, Color{108, 57, 20}, Color{0,0,uint8_t(-63)}};

		Color m_ColBoardW;
		Color m_ColBoardB;
		Color m_ColPieceW;
		Color m_ColPieceB;
		Color m_ColDeltaSelect;
	};
}
