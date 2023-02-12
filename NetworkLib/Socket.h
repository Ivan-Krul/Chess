#pragma once
#include "Depencies.h"
#include "Booter.h"

#include <string>
#include <array>

#define IPV4 AF_INET
#define TCP SOCK_STREAM

#ifdef SOCKET_MESSAGE_SIZE
namespace network_lib
{
	class Socket
	{
		SOCKET m_Socket;
		SOCKADDR_IN m_SocketAddress;
		char m_Message[SOCKET_MESSAGE_SIZE];
		std::string m_IPAdress;
		int m_AF;
		int m_Type;
		int m_Protocol;
		uint16_t m_Port;

		bool m_NeedClose;
	public:
		Socket(const bool need_close = true ,const int af = IPV4, const int type = TCP, const int protocol = 0);
		const int GetAF() const;
		const int GetType() const;
		const int GetProtocol() const;
		const uint16_t GetPort() const;
		const std::string GetIPAddress() const;

		SOCKET& GetSocket();
		SOCKADDR_IN& GetSocketAddress();

		void SetPort(const uint16_t port);
		const int Connect(const std::string ip_address);
		void Listen();
		const bool CanBeAccepted(Socket& socket) const;
		const size_t MessageSize() const;
		void SelectEventForBind(HANDLE h_event_obj, long network_event = FD_READ);
		
		const bool CanBeRecievedFromThisSocket();
		const std::string GetMessage_() const;

		int SendToThisSocket(const std::string message);

		void Close();
		~Socket();
	};
}
#endif
