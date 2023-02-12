#include "MoveGenerator.h"

namespace chess_lib
{
	std::vector<Move> MoveGenerator::f_GenerateLineMoves(const Board& board, const uint8_t position, const std::vector<pos> vectors, bool iterative, bool only_enemy)
	{
		auto arr = board.GetBoard();
		auto moves = std::vector<Move>();
		auto cur_pos = pos{ char(position % 8), char(position / 8) };

		for (uint8_t i = 0; i < vectors.size(); i++)
		{
			cur_pos = pos{ char(position % 8), char(position / 8) };
			cur_pos.x += vectors[i].x;
			cur_pos.y += vectors[i].y;

			while (cur_pos.x >= 0 && cur_pos.x < 8 && cur_pos.y >= 0 && cur_pos.y < 8)
			{
				if (arr[cur_pos.x + cur_pos.y * 8].side == SideType::none)
				{
					if(!only_enemy)
						moves.push_back(Move{ position, uint8_t(cur_pos.x + cur_pos.y * 8) });
					cur_pos.x += vectors[i].x;
					cur_pos.y += vectors[i].y;
				}
				else if (arr[cur_pos.x + cur_pos.y * 8].side == arr[position].side)
					break;
				else
				{
					moves.push_back(Move{ position, uint8_t(cur_pos.x + cur_pos.y * 8) });
					break;
				}
				if (!iterative)
					break;

			}

		}
		return moves;
	}
	std::vector<Move> MoveGenerator::GenerateRookMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		return f_GenerateLineMoves(board,position, { {1,0},{0,1},{-1,0},{0,-1} }, only_enemy);
	}
	std::vector<Move> MoveGenerator::GenerateBishopMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		return f_GenerateLineMoves(board, position, { {1,1},{-1,1},{-1,-1},{1,-1} }, only_enemy);
	}
	std::vector<Move> MoveGenerator::GenerateQueenMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		return f_GenerateLineMoves(board, position, { {1,1},{-1,1},{-1,-1},{1,-1},{1,0},{0,1},{-1,0},{0,-1} }, only_enemy);
	}
	std::vector<Move> MoveGenerator::GenerateKnightMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		return f_GenerateLineMoves(board, position, { {2,1},{2,-1},{-1,2},{1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} }, false, only_enemy);
	}
	std::vector<Move> MoveGenerator::GenerateKingMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		return f_GenerateLineMoves(board, position, { {1,1},{-1,1},{-1,-1},{1,-1},{1,0},{0,1},{-1,0},{0,-1} }, false, only_enemy);
	}
	std::vector<Move> MoveGenerator::GeneratePawnMove(const Board& board, const uint8_t position, bool only_enemy)
	{
		auto is_white_move = !board.GetIsWhiteMove();
		auto arr = board.GetBoard();
		auto moves = std::vector<Move>();
		auto prev_move = board.GetPreviousMove();

		auto constr = [=](int ind) { return Move{ position, ind }; };
		auto pos_upper = [=]() {return is_white_move ? -8 : 8; };
		auto pos_double_move = [=]() {return is_white_move ? -16 : 16; };
		auto no_pawn_upper = [=]() { return arr[pos_upper() + position].side == SideType::none; };
		auto no_pawn_double_upper = [=]() { return no_pawn_upper() && arr[pos_double_move() + position].side == SideType::none && position / 8 == (is_white_move ? 1 : 6); };

		if (no_pawn_upper())
			moves.push_back(constr(pos_upper() + position));
		if (no_pawn_double_upper())
			moves.push_back(constr(pos_double_move() + position));
		


		return moves;
	}
	bool MoveGenerator::CanKingBeInCheck(const Board& board)
	{
		auto is_white_move = !board.GetIsWhiteMove();
		auto arr = board.GetBoard();
		auto king_index = uint8_t(0);

		for (king_index; king_index < arr.size() && arr[king_index].type != PieceType::king && (bool)((int)arr[king_index].side - 1) == is_white_move; king_index++);

		auto enemy_list_rook = GenerateRookMove(board, king_index, true);
		auto enemy_list_bishop = GenerateBishopMove(board, king_index, true);
		auto enemy_list_knight = GenerateKnightMove(board, king_index, true);

		for (auto move : enemy_list_knight)
			if (arr[move.GetP2()].type == PieceType::knight && (bool)((int)arr[move.GetP2()].side) == is_white_move)
				return true;
		for (auto move : enemy_list_rook)
			if ((arr[move.GetP2()].type == PieceType::rook || arr[move.GetP2()].type == PieceType::queen) && (bool)((int)arr[move.GetP2()].side) == is_white_move)
				return true;
		for (auto move : enemy_list_bishop)
			if ((arr[move.GetP2()].type == PieceType::bishop || arr[move.GetP2()].type == PieceType::queen) && (bool)((int)arr[move.GetP2()].side) == is_white_move)
				return true;

		return false;
	}
}
