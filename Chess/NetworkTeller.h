#pragma once
#include <iostream>

#include "Socket.h"

#include "TypeInterface.h"

#define PORT 5020

class NetworkTeller
{
public:
	int InitWSA(char maj_ver = 2, char min_ver = 2);
	int InitSocketsAsServer();
	int InitSocketAsClient(const std::string& ip);

	int Listen();
	void SendQuitMessage();
	bool isQuitMsg();
	bool isLegalMove();

	const chess_lib::GlobMove ConvertToMove();
	void SendMove(const chess_lib::GlobMove& move);

	// getters
	static NetworkTeller& getInstance() noexcept;
	const network_lib::Booter& getBooter() const noexcept;
	const std::string getIPAddress() const noexcept;

private:
	const std::string c_QuitMessage = "$QuIt";
	const std::string c_InitMessage = "$InIt";

	network_lib::Booter m_Booter = network_lib::Booter::GetInstance();
	
	network_lib::Socket m_Sock;
	network_lib::Socket m_SockCl;

	std::string m_IP;
	std::string m_Msg;

	bool m_IsServer = false;

	// private functions
	network_lib::Socket& getSocket_() noexcept;

	// singleton pattern
	NetworkTeller() = default;
	static NetworkTeller m_Instance;
};

