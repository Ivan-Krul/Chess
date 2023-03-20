#include "ConfigParser.h"

namespace chess_lib
{
	/*
	
	*/
	void ConfigParser::load(const std::string& path)
	{
		std::ifstream fout(path);
		fout.close();
	}
}
