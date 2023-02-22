#pragma once
#include <iostream>
#include "NetworkTeller.h"

class TypeInterface
{
public:
	inline static NetworkTeller::net_move FormQuire(chess_lib::Board& board)
	{
		auto m1 = std::string();
		auto m2 = std::string();
		auto mc = chess_lib::MoveController::GetInstance();
		auto mv = std::shared_ptr<NetworkTeller::net_move>();
		std::cout << "Check: " << mc.IsCheck(board) << '\n';
	ty_again_move:
		std::cout << "Move a piece in: ";
		std::cin >> m1;
		std::cout << "Move a piece to: ";
		std::cin >> m2;
		if (!mc.ConfirmMove(board, chess_lib::Move{ board.ConvertToIndex(m1),board.ConvertToIndex(m2) }))
		{
			std::cout << "no move\n";
			goto ty_again_move;
		}
		mv = std::make_shared<NetworkTeller::net_move>(NetworkTeller::net_move{ chess_lib::Move{ board.ConvertToIndex(m1),board.ConvertToIndex(m2) }, chess_lib::Board::PromotionChoice::none });
		if (board.NeedPromotion())
		{
			std::string s;
		ty_again_promote:
			std::cout << "Choose a promotion piece in " << board.ConvertFromIndex(board.GetPreviousMove()->GetP2()) << ": ";
			std::cin >> s;
			if (s == "queen")
			{
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::queen);
				mv->prom = chess_lib::Board::PromotionChoice::queen;
			}
			else if (s == "bishop")
			{
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::bishop);
				mv->prom = chess_lib::Board::PromotionChoice::bishop;
			}
			else if (s == "knight")
			{
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::knight);
				mv->prom = chess_lib::Board::PromotionChoice::knight;
			}
			else if (s == "rook")
			{
				mc.PromoteTo(board, chess_lib::Board::PromotionChoice::rook);
				mv->prom = chess_lib::Board::PromotionChoice::rook;
			}
			else
				goto ty_again_promote;
		}
		return *mv;
	}

	inline static bool RecreateMove(chess_lib::Board& board, NetworkTeller::net_move mv)
	{
		auto conf = false;
		auto mc = chess_lib::MoveController::GetInstance();
		conf = mc.ConfirmMove(board, mv.move);
		if (board.NeedPromotion())
			mc.PromoteTo(board, mv.prom);
		return conf;
	}
};
