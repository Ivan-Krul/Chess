#pragma once
#include "Depencies.h"

namespace network_lib
{
	class Booter
	{
		WSADATA m_WSAData;

		// Singleton pattern
		static Booter s_Instance;
		Booter() = default;

	public:
		int Init(const char maj_ver = 2, const char min_ver = 2);
		WSADATA& GetWSData();

		// Singleton pattern
		static Booter& GetInstance()
		{
			return s_Instance;
		}
	};
}
