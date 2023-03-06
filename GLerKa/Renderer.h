#pragma once
#include "Depencies.h"

#include "LoaderColor.h"
#include "LoaderModel.h"

#include "Board.h"

namespace glerka_lib
{
	class Renderer
	{
	public:
		Renderer();
		void RenderUnPushedSquare(const chess_lib::Board& board, const uint8_t index);
		void RenderBoard(const chess_lib::Board& board);

	private:
		LoaderModel m_lm;
		LoaderColor m_lc;
	};
}
