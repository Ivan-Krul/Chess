#pragma once
#include <fstream>
#include <vector>
#include <array>
#include <string>

#include "Tile.h"

#include "nlohmann/json.hpp"

namespace glerka_lib
{
	class LoaderModel
	{ 
	public:
		void load();
		std::vector<float> getPiece(chess_lib::PieceType type);
	private:
		const std::string c_Directory = "./chess.dat";

		const std::array<std::vector<float>, 6> c_StandModels = 
		{
			std::vector<float>{ .5f,.8f,.2f,.2f,.8f,.2f },
			{ .35f,.2f,.65f,.2f,.35f,.8f,.65f,.8f },
			{ .1f,.7f,.5f,.9f,.9f,.5f,.5f,.6f,.2f,.2f,.9f,.5f,.9f,.2f },
			{ .5f,.9f,.3f,.7f,.7f,.7f,.5f,.5f,.5f,.5f,.8f,.2f,.2f,.2f },
			{ .5f,.9f,.3f,.8f,.7f,.8f,.5f,.5f,.5f,.6f,.8f,.2f,.2f,.2f },
			{ .8f,.2f,.2f,.2f,.5f,.7f,.5f,.7f,.3f,.7f,.3f,.9f,.7f,.7f,.7f,.9f }
		};

		std::array<std::vector<float>, 6> m_Models;
	};
}
