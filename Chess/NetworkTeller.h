#pragma once
#define SOCKET_MESSAGE_SIZE 64
#include "Socket.h"

#include "MoveController.h"

#define PORT 5020

class NetworkTeller
{
	
	std::shared_ptr<network_lib::Socket> m_pSockCl;
	std::shared_ptr<network_lib::Socket> m_pSockSr;

	bool m_IsServer = false;
	bool m_IsConnection = false;

	std::shared_ptr<network_lib::Socket> f_GetCurSock() const;

	static NetworkTeller m_Instance;
	NetworkTeller();
public:
	struct net_move
	{
		chess_lib::Move move;
		chess_lib::Board::PromotionChoice prom;
	};

	void StartAsServer();
	bool StartAsClient(std::string ip);

	bool IsSocketGot();

	bool InitGameFromServer();

	void TellMove(const chess_lib::Move move, const chess_lib::Board::PromotionChoice prom = chess_lib::Board::PromotionChoice::none);
	bool IsIncome();
	bool IsSpecial();
	bool ReadSpecialMessage();
	net_move RecreateMove();

	bool IsConnection();

	void Disconnect();

	~NetworkTeller()
	{
		Disconnect();
	}

	static NetworkTeller& GetInstance()
	{
		return m_Instance;
	}
};

