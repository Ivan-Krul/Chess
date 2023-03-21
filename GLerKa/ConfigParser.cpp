#include "ConfigParser.h"

namespace glerka_lib
{
	void ConfigParser::load()
	{
		const auto END_TOKEN = "end";
		const auto DIVIDE_TOKEN = '/';
		const auto STRING_TOKEN = '"';

		auto fin = std::ifstream(PATH);
		auto field = std::string();
		auto directory = std::string();

		auto string_tok = false;

		m_Variables.clear();

		while (fin >> field)
		{
			if (field == END_TOKEN)
			{
				if (directory.size() == 0)
					break;
				for (auto i = 0; i < directory.size() - 1; i++)
				{
					directory.erase(directory.size() - i - 1);
					if (directory[directory.size() - i - 1] != DIVIDE_TOKEN)
						continue;
					directory.erase(directory.size() - i - 1);
					break;
				}
			}
			else if (field[0] == STRING_TOKEN || *field.rbegin() == STRING_TOKEN)
			{

			}
			else if (field[0] >= '0' && field[0] <= '9')
			{
				
			}
			else
			{
				
			}
		}


		fin.close();
	}
}
