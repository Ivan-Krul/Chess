#include "LoaderModel.h"

namespace glerka_lib
{
	std::vector<float> LoaderModel::getPiece(chess_lib::PieceType type)
	{
		if(size_t(type) != 0)
			return m_Models[size_t(type) - 1];
		return
			std::vector<float>();
	}
	void LoaderModel::Load()
	{
		std::ifstream fin(c_Directory);

		if (!fin.is_open())
		{
			m_Models = c_StandModels;
			return;
		}

		bool is_my_part = false;
		bool breakpoints[6] = { false, false, false, false, false, false };
		auto statement = 0;
		auto str = std::string();
		auto is_break = true;

		while (fin >> str)
		{
			if (str == "color" && is_my_part)
				break;

			if (is_my_part)
			{
				if (str == "pawn")
					statement = 0;
				else if (str == "rook")
					statement = 1;
				else if (str == "knight")
					statement = 2;
				else if (str == "bishop")
					statement = 3;
				else if (str == "queen")
					statement = 4;
				else if (str == "king")
					statement = 5;
				else if (str == "end")
					breakpoints[statement] = true;
				else
					m_Models[statement].push_back(std::stof(str));
			}

			if (str == "model")
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
	}
}
