#include "NetworkTeller.h"

#include <ctime>

std::shared_ptr<network_lib::Socket> NetworkTeller::f_GetCurSock() const
{
	if (m_IsServer)
		return m_pSockCl;
	else
		return m_pSockSr;
}

NetworkTeller::NetworkTeller()
{
	m_IsServer = false;
	auto b = network_lib::Booter::GetInstance();
	b.Init();
}

void NetworkTeller::StartAsServer()
{
	m_IsServer = true;
	m_pSockSr = std::make_shared<network_lib::Socket>();
	m_pSockSr->SetPort(PORT);
	m_pSockSr->Listen();
	m_pSockCl = std::make_shared<network_lib::Socket>(false);
}

bool NetworkTeller::StartAsClient(std::string ip)
{
	m_IsServer = false;
	return m_pSockCl->Connect(ip);
}

bool NetworkTeller::IsSocketGot()
{
	return m_IsConnection = m_pSockSr->CanBeAccepted(*m_pSockCl);
}

bool NetworkTeller::InitGameFromServer()
{
	m_pSockCl->SelectEventForBind(0);
	srand(time(NULL));
	bool side = rand() % 2;
	f_GetCurSock()->SendToThisSocket("$InIt%" + (side + '0'));
	return side;
}

void NetworkTeller::TellMove(const chess_lib::Move move, const chess_lib::Board::PromotionChoice prom)
{
	chess_lib::Board b;
	char p;

	if(prom == chess_lib::Board::PromotionChoice::rook)
		p = 'r';
	else if (prom == chess_lib::Board::PromotionChoice::knight)
		p = 'n';
	else if (prom == chess_lib::Board::PromotionChoice::queen)
		p = 'q';
	else if (prom == chess_lib::Board::PromotionChoice::bishop)
		p = 'b';
	else 
		p = '0';

	f_GetCurSock()->SendToThisSocket(b.ConvertFromIndex(move.GetP1()) + b.ConvertFromIndex(move.GetP2()) + p);
}

bool NetworkTeller::IsIncome()
{
	return f_GetCurSock()->CanBeRecievedFromThisSocket();
}

bool NetworkTeller::IsSpecial()
{
	return f_GetCurSock()->GetMessage_()[0] == '$';
}

bool NetworkTeller::ReadSpecialMessage()
{
	if (!IsSpecial())
		return false;
	if (f_GetCurSock()->GetMessage_().substr(1) == "QuIt!")
		m_IsConnection = false;
	if (f_GetCurSock()->GetMessage_().substr(1, 5) == "InIt%")
	{
		if (*f_GetCurSock()->GetMessage_().rbegin() == '1')
			return true;
		return false;
	}
	return false;
}

NetworkTeller::net_move NetworkTeller::RecreateMove()
{
	if (IsSpecial())
		return NetworkTeller::net_move{ chess_lib::Move{0xff,0xff}, chess_lib::Board::PromotionChoice::none };
	std::shared_ptr<net_move> nm;
	std::string p1 = f_GetCurSock()->GetMessage_().substr(0, 2);
	std::string p2 = f_GetCurSock()->GetMessage_().substr(2, 2);
	char p = f_GetCurSock()->GetMessage_()[4];
	chess_lib::Board::PromotionChoice pp;
	chess_lib::Board b;

	if (p == 'r')
		pp = chess_lib::Board::PromotionChoice::rook;
	else if (p == 'n')
		pp = chess_lib::Board::PromotionChoice::knight;
	else if (p == 'b')
		pp = chess_lib::Board::PromotionChoice::bishop;
	else if (p == 'q')
		pp = chess_lib::Board::PromotionChoice::queen;
	else
		pp = chess_lib::Board::PromotionChoice::none;
		
	nm = std::make_shared<net_move>(net_move{ chess_lib::Move{b.ConvertToIndex(p1), b.ConvertToIndex(p2)},pp });
	return *nm;
}

bool NetworkTeller::IsConnection()
{
	return m_IsConnection;
}

void NetworkTeller::Disconnect()
{
	f_GetCurSock()->SendToThisSocket("$QuIt!");
}

NetworkTeller NetworkTeller::m_Instance;
