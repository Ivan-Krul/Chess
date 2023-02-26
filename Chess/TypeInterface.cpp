#include "TypeInterface.h"

chess_lib::GlobMove TypeInterface::FormQuire(chess_lib::Board& board)
{
	auto m1 = std::string();
	auto m2 = std::string();
	auto mc = chess_lib::MoveController::GetInstance();

	std::cout << "Check: " << mc.IsCheck(board) << '\n';
	while (true) {
		std::cout << "Move a piece in: ";
		std::cin >> m1;
		std::cout << "Move a piece to: ";
		std::cin >> m2;

		if (mc.ConfirmMove(board, chess_lib::Move{ board.ConvertToIndex(m1),board.ConvertToIndex(m2) }))
			break;  // valid move, exit loop
		else
			std::cout << "Invalid move, please try again.\n";
	}

	auto mv = chess_lib::GlobMove{ chess_lib::Move{ board.ConvertToIndex(m1),board.ConvertToIndex(m2) }, chess_lib::PromotionChoice::none };

	if (board.NeedPromotion())
	{
		std::string s;
	ty_again_promote:
		std::cout << "Choose a promotion piece in " << board.ConvertFromIndex(board.GetPreviousMove()->GetP2()) << ": ";
		std::cin >> s;
		if (s == "queen")
		{
			mc.PromoteTo(board, chess_lib::PromotionChoice::queen);
			mv.prom = chess_lib::PromotionChoice::queen;
		}
		else if (s == "bishop")
		{
			mc.PromoteTo(board, chess_lib::PromotionChoice::bishop);
			mv.prom = chess_lib::PromotionChoice::bishop;
		}
		else if (s == "knight")
		{
			mc.PromoteTo(board, chess_lib::PromotionChoice::knight);
			mv.prom = chess_lib::PromotionChoice::knight;
		}
		else if (s == "rook")
		{
			mc.PromoteTo(board, chess_lib::PromotionChoice::rook);
			mv.prom = chess_lib::PromotionChoice::rook;
		}
		else
			goto ty_again_promote;
	}
	return mv;
}

bool TypeInterface::RecreateMove(chess_lib::Board& board, const chess_lib::GlobMove mov)
{
	auto conf = false;
	auto mc = chess_lib::MoveController::GetInstance();
	conf = mc.ConfirmMove(board, mov.mv);
	if (board.NeedPromotion())
		mc.PromoteTo(board, mov.prom);
	return conf;
}

std::string TypeInterface::ConvertToString(chess_lib::GlobMove mov)
{
	std::string m1 = chess_lib::Board::ConvertFromIndex(mov.mv.GetP1());
	std::string m2 = chess_lib::Board::ConvertFromIndex(mov.mv.GetP2());
	std::string p;
	if (mov.prom == chess_lib::PromotionChoice::rook)
		p = 'r';
	if (mov.prom == chess_lib::PromotionChoice::bishop)
		p = 'b';
	if (mov.prom == chess_lib::PromotionChoice::queen)
		p = 'q';
	if (mov.prom == chess_lib::PromotionChoice::knight)
		p = 'n';
	else
		p = '0';
	return m1 + m2 + p;
}

bool TypeInterface::IsLegal(const std::string str)
{
	if (str.length() != 5)
		return false;

	char start_col = str[0];
	char start_row = str[1];
	char end_col = str[2];
	char end_row = str[3];

	if (start_col >= 'A' && start_col <= 'H')
		start_col += 'a' - 'A';
	if (end_col >= 'A' && end_col <= 'H')
		end_col += 'a' - 'A';

	if (start_col < 'a' || start_col > 'h')
		return false;
	if (start_row < '1' || start_row > '8')
		return false;
	if (end_col < 'a' || end_col > 'h')
		return false;
	if (end_row < '1' || end_row > '8')
		return false;

	return true;
}

/*bool TypeInterface::IsLegal(std::string str)
{
	auto m1 = str.substr(0, 2);
	auto m2 = str.substr(2, 2);

	auto liter = [=](std::string& move) {
		if (move[0] - 'a' < 8 && move[0] - 'a' >= 0)
			move[0] -= 'A';
	};

	liter(m1);
	liter(m2);

	if ('A' > m1[0])
		return false;
	if (m1[0] > 'H')
		return false;
	if ('A' > m2[0])
		return false;
	if (m2[0] > 'H')
		return false;
	if ('1' > m1[1])
		return false;
	if (m1[1] > '8')
		return false;
	if ('1' > m2[1])
		return false;
	if (m2[1] > '8')
		return false;
	return true;
}*/

chess_lib::GlobMove TypeInterface::ConvertFromString(std::string str)
{
	auto gm = std::shared_ptr<chess_lib::GlobMove>();
	auto m1 = str.substr(0, 2);
	auto m2 = str.substr(2, 2);
	gm = std::make_shared<chess_lib::GlobMove>(chess_lib::GlobMove{ chess_lib::Move{chess_lib::Board::ConvertToIndex(m1), chess_lib::Board::ConvertToIndex(m2)} });
	if (str[4] == 'r')
		gm->prom = chess_lib::PromotionChoice::rook;
	else if (str[4] == 'b')
		gm->prom = chess_lib::PromotionChoice::bishop;
	else if (str[4] == 'n')
		gm->prom = chess_lib::PromotionChoice::knight;
	else if (str[4] == 'q')
		gm->prom = chess_lib::PromotionChoice::queen;
	else
		gm->prom = chess_lib::PromotionChoice::none;
	return *gm;
}
