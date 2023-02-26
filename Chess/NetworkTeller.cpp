#include "NetworkTeller.h"

int NetworkTeller::InitWSA(char maj_ver, char min_ver)
{
	return m_Booter.Init(maj_ver, min_ver);
}

int NetworkTeller::InitSocketsAsServer()
{
	m_IsServer = true;

	m_Sock.SetPort(PORT);

	// create a listening socket and wait for a connection
	m_Sock.Listen();
	while (!m_Sock.CanBeAccepted(m_SockCl));

	// use the accepted socket for communication and randomly decide which side to play
	srand((unsigned int)time(NULL));
	auto side_white = rand() % 2;

	// send an initialization message to the client
	m_Msg = c_InitMessage + '%';
	m_Msg += (side_white ? '0' : '1');
	getSocket_().SendToThisSocket(m_Msg);

	return side_white;
}

int NetworkTeller::InitSocketAsClient(const std::string& ip)
{
	m_SockCl = network_lib::Socket(false);
	m_IsServer = false;
	m_IP = ip;

	m_Sock.SetPort(PORT);

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

int NetworkTeller::Listen()
{
	// we're waiting for opponent move
	// we're looping, cuz we're waiting for his move
	// listen for an incoming message
	static int status;
	while(!(status = getSocket_().CanBeRecievedFromThisSocket()))
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

NetworkTeller& NetworkTeller::getInstance() noexcept
{
	return m_Instance;
}

const network_lib::Booter& NetworkTeller::getBooter() const noexcept
{
	return m_Booter;
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

NetworkTeller NetworkTeller::m_Instance;
