#pragma once
#include <iostream>

#include "Socket.h"

#include "TypeInterface.h"

#define PORT 5020

class NetworkTeller
{
public:
	int InitSocket(const bool is_server, const std::string ip = "");

	int Listen();
	void SendQuitMessage();
	bool isQuitMsg();
	bool isLegalMove();

	const chess_lib::GlobMove ConvertToMove();
	void SendMove(const chess_lib::GlobMove& move);

	// getters
	const std::string getIPAddress() const noexcept;

private:
	const std::string c_QuitMessage = "$QuIt";
	const std::string c_InitMessage = "$InIt";
	
	network_lib::Socket m_Sock;
	network_lib::Socket m_SockCl;

	std::string m_IP;
	std::string m_Msg;

	bool m_IsServer = false;

	// private functions
	network_lib::Socket& getSocket_() noexcept;
};

