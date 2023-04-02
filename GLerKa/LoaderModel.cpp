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
	void LoaderModel::load()
	{
		std::ifstream fin(c_Directory);

		if (!fin.is_open())
		{
			m_Models = c_StandModels;
			return;
		}

		nlohmann::json json_file = nlohmann::json::parse(fin);

		m_Models.fill(std::vector<float>());
		for (auto& vert : json_file["model"]["pawn"])
			m_Models[0].push_back(vert);
		for (auto& vert : json_file["model"]["rook"])
			m_Models[1].push_back(vert);
		for (auto& vert : json_file["model"]["knight"])
			m_Models[2].push_back(vert);
		for (auto& vert : json_file["model"]["bishop"])
			m_Models[3].push_back(vert);
		for (auto& vert : json_file["model"]["queen"])
			m_Models[4].push_back(vert);
		for (auto& vert : json_file["model"]["king"])
			m_Models[5].push_back(vert);

		fin.close();
	}
}
