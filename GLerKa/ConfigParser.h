#pragma once
#include <fstream>
#include <string>
#include <map>
#include <vector>

namespace glerka_lib
{
	const std::string PATH = "chess.cfg";

	class ConfigParser
	{
	public:
		void load();

	private:
		std::map<std::string, std::vector<std::string>> m_Variables;

	};
}
