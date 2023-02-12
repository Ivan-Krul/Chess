#include "Socket.h"

#ifdef SOCKET_MESSAGE_SIZE
namespace network_lib
{
	Socket::Socket(const bool need_close, const int af, const int type, const int protocol)
	{
		m_Port = 0;
		m_AF = af;
		m_Type = type;
		m_Protocol = protocol;
		m_NeedClose = need_close;

		memset(m_Message, 0, SOCKET_MESSAGE_SIZE);

		if (need_close)
		{
			m_Socket = socket(af, type, protocol);

			memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
			m_SocketAddress.sin_family = af;
		}
	}

	const int Socket::GetAF() const
	{
		return m_AF;
	}

	const int Socket::GetType() const
	{
		return m_Type;
	}

	const int Socket::GetProtocol() const
	{
		return m_Protocol;
	}

	const uint16_t Socket::GetPort() const
	{
		return m_Port;
	}

	const std::string Socket::GetIPAddress() const
	{
		return m_IPAdress;
	}

	SOCKET& Socket::GetSocket()
	{
		return m_Socket;
	}

	SOCKADDR_IN& Socket::GetSocketAddress()
	{
		return m_SocketAddress;
	}

	void Socket::SetPort(const uint16_t port)
	{
		m_Port = port;
		m_SocketAddress.sin_port = m_Port;
	}

	const int Socket::Connect(const std::string ip_address)
	{
#pragma warning(suppress : 4996)
		m_SocketAddress.sin_addr.S_un.S_addr = inet_addr(ip_address.c_str());
		return connect(m_Socket, (sockaddr*)&m_SocketAddress, sizeof(m_SocketAddress));
	}

	void Socket::Listen()
	{
		bind(m_Socket, (sockaddr*)&m_SocketAddress, sizeof(m_SocketAddress)); // trying to bind
		listen(m_Socket, 100); // listening latest messages, then we're got a socket
	}

	const bool Socket::CanBeAccepted(Socket& socket) const
	{
		static int size = sizeof(m_SocketAddress);
		return (socket.GetSocket() = accept(m_Socket, (sockaddr*)&socket.GetSocketAddress(), &size));
	}

	const size_t Socket::MessageSize() const
	{
		return SOCKET_MESSAGE_SIZE;
	}

	void Socket::SelectEventForBind(HANDLE h_event_obj, long network_event)
	{
		WSAEventSelect(m_Socket, h_event_obj, network_event);
	}

	const bool Socket::CanBeRecievedFromThisSocket()
	{
		return recv(m_Socket, m_Message, SOCKET_MESSAGE_SIZE, 0);
	}

	const std::string Socket::GetMessage_() const
	{
		return m_Message;
	}

	int Socket::SendToThisSocket(const std::string message)
	{
		memcpy(m_Message, message.c_str(), message.size());
		return send(m_Socket, m_Message, SOCKET_MESSAGE_SIZE, 0);
	}

	void Socket::Close()
	{
		closesocket(m_Socket);
	}

	Socket::~Socket()
	{
		if(m_NeedClose)
			Close();

	}

}
#endif
