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

		bool breakpoints[6];

		while (fin)
		{

		}

		fin.close();
	}
}
