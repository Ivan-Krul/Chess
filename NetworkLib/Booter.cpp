#include "Booter.h"

namespace network_lib
{
	int Booter::Init(const char maj_ver, const char min_ver)
	{
		return WSAStartup(MAKEWORD(maj_ver, min_ver), &m_WSAData);
	}

	WSADATA& Booter::GetWSData()
	{
		return m_WSAData;
	}

	// Singleton pattern
	Booter Booter::s_Instance;
}
