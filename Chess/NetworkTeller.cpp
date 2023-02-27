#include "NetworkTeller.h"

int NetworkTeller::InitSocket(const bool is_server, const std::string ip)
{
	m_IsServer = is_server;
	m_IP = ip;
	m_SockCl = network_lib::Socket(false);
	m_Sock.SetPort(PORT);
	srand((unsigned int)time(NULL));
	auto side_white = rand() % 2;

	if (m_IsServer)
	{
		// create a listening socket and wait for a connection
		m_Sock.Listen();
		while (!m_Sock.CanBeAccepted(m_SockCl));
		
		// send an initialization message to the client
		m_Msg = c_InitMessage + '%';
		m_Msg += (side_white ? '0' : '1');
		getSocket_().SendToThisSocket(m_Msg);
	}
	else
	{
		// connect to the server using a socket and wait for an initialization message
		if (getSocket_().Connect(m_IP) == SOCKET_ERROR)
			return -1;

		// getting statement of turn
		while (!(getSocket_().CanBeRecievedFromThisSocket() > 0));
		m_Msg = getSocket_().GetMessage_();

		if (m_Msg.substr(0, c_InitMessage.size()) != c_InitMessage)
			return -1;

		return m_Msg[6] != '0';
	}
	return side_white;
}

int NetworkTeller::Listen()
{
	// we're waiting for opponent move
	// we're looping, cuz we're waiting for his move
	// listen for an incoming message
	int status;
	while (!(status = getSocket_().CanBeRecievedFromThisSocket()));
	return status;
}

void NetworkTeller::SendQuitMessage()
{
	getSocket_().SendToThisSocket(c_QuitMessage);
}

bool NetworkTeller::isQuitMsg()
{
	return getSocket_().GetMessage_() == c_QuitMessage;
}

bool NetworkTeller::isLegalMove()
{
	return TypeInterface::IsLegal(getSocket_().GetMessage_());
}

const chess_lib::GlobMove NetworkTeller::ConvertToMove()
{
	return TypeInterface::ConvertFromString(getSocket_().GetMessage_());
}

void NetworkTeller::SendMove(const chess_lib::GlobMove& move)
{
	getSocket_().SendToThisSocket(TypeInterface::ConvertToString(move));
}

const std::string NetworkTeller::getIPAddress() const noexcept
{
	return m_IP;
}

network_lib::Socket& NetworkTeller::getSocket_() noexcept
{
	if (m_IsServer)
		return m_SockCl;
	else
		return m_Sock;
}
