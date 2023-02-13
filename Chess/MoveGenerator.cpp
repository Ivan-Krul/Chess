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
		auto is_white_move = board.GetIsWhiteMove();
		auto arr = board.GetBoard();
		auto moves = std::vector<Move>();
		auto prev_move = board.GetPreviousMove();

		auto to_pos = [=](uint8_t ind) { return pos{ int8_t(ind % 8), int8_t(ind / 8) }; };
		auto constr = [=](uint8_t ind) { return Move{ position, uint8_t(ind) }; };
		auto to_ind = [=](pos dot) { return dot.x + dot.y * 8; };
		auto opposite_delta_side = [=](int8_t x) {return is_white_move ? -x : x; };

		auto posit = to_pos(position);
		
		auto is_in_map = [=](pos dot) { return unsigned(dot.x) < 8 && unsigned(dot.y) < 8; };
		auto opposite_side = [=](uint8_t x) {return is_white_move ? 7 - x : x; }; // 6y : 1y, 4y : 3y

		{
			auto cond_is_busy = [=](pos dot) {return is_in_map(dot) && arr[to_ind(dot)].side == SideType::none; };

			if (cond_is_busy(to_pos(position + opposite_delta_side(8))))
			{
				moves.push_back(constr(position + opposite_delta_side(8)));
				if (cond_is_busy(to_pos(position + opposite_delta_side(16))) && posit.y == opposite_side(1))
					moves.push_back(constr(position + opposite_delta_side(16)));
			}
		}

		auto opposite_piece = [=]() {return is_white_move ? SideType::black : SideType::white; };
		auto delta_pos = [=](int8_t x, int8_t y) {return pos{ int8_t(posit.x + x) , int8_t(posit.y + y) }; };

		{
			auto del_pos = delta_pos(0, opposite_delta_side(1));

			for (int8_t i = -1; i < 2; i += 2)
			{
				del_pos = delta_pos(i, opposite_delta_side(1));
				if (!is_in_map(del_pos))
					continue;
				if (arr[to_ind(del_pos)].side == opposite_piece())
					moves.push_back(constr(to_ind(del_pos)));
			}

			for (int8_t i = -1; i < 2; i += 2)
			{
				del_pos = delta_pos(i, 0);
				if (!is_in_map(del_pos))
					continue;
				if (arr[to_ind(del_pos)].side == opposite_piece()
					&& to_pos(prev_move->GetP1()).y == opposite_side(6)
					&& arr[to_ind(del_pos)].type == PieceType::pawn
					&& to_pos(prev_move->GetP1()).x == del_pos.x
					)
					moves.push_back(constr(to_ind(del_pos) + opposite_delta_side(8)));
			}
		}
		
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
