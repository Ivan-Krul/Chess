#include "MoveGenerator.h"

namespace chess_lib
{
	std::vector<Move> MoveGenerator::f_GenerateLineMoves(const Board& p_board, const uint8_t position, const std::vector<pos> vectors, bool iterative, bool only_enemy) const
	{
		auto &arr = p_board.GetBoard();
		auto moves = std::vector<Move>();
		auto cur_pos = pos{ int8_t(position % 8), int8_t(position / 8) };

		for (uint8_t i = 0; i < vectors.size(); i++)
		{
			cur_pos = pos{ int8_t(position % 8), int8_t(position / 8) };
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

	std::vector<Move> MoveGenerator::f_GeneratePawnAttackMoves(const Board& board, const uint8_t position, bool only_enemy) const
	{
		auto is_white_move = board.GetIsWhiteMove();
		auto& arr = board.GetBoard();
		auto moves = std::vector<Move>();

		auto constr = [=](uint8_t ind) { return Move{ position, uint8_t(ind) }; };
		auto to_ind = [=](pos dot) { return dot.x + dot.y * 8; };
		auto to_pos = [=](uint8_t ind) { return pos{ int8_t(ind % 8), int8_t(ind / 8) }; };

		auto posit = to_pos(position);

		auto is_in_map = [=](pos dot) { return unsigned(dot.x) < 8 && unsigned(dot.y) < 8; };

		auto opposite_delta_side = [=](int8_t x) {return is_white_move ? -x : x; };

		auto opposite_piece = [=]() {return is_white_move ? SideType::black : SideType::white; };
		auto delta_pos = [=](int8_t x, int8_t y) {return pos{ int8_t(posit.x + x) , int8_t(posit.y + y) }; };

		auto del_pos = delta_pos(0, opposite_delta_side(1));

		for (int8_t i = -1; i < 2; i += 2)
		{
			del_pos = delta_pos(i, opposite_delta_side(1));
			if (!is_in_map(del_pos))
				continue;
			if (arr[to_ind(del_pos)].side == opposite_piece())
				moves.push_back(constr(to_ind(del_pos)));
		}
		return moves;
	}

	std::vector<Move> MoveGenerator::GenerateRookMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		return f_GenerateLineMoves(p_board, position, { {1,0},{0,1},{-1,0},{0,-1} }, true, only_enemy);
	}

	std::vector<Move> MoveGenerator::GenerateBishopMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		return f_GenerateLineMoves(p_board, position, { {1,1},{-1,1},{-1,-1},{1,-1} }, true, only_enemy);
	}

	std::vector<Move> MoveGenerator::GenerateQueenMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		return f_GenerateLineMoves(p_board, position, { {1,1},{-1,1},{-1,-1},{1,-1},{1,0},{0,1},{-1,0},{0,-1} }, true, only_enemy);
	}

	std::vector<Move> MoveGenerator::GenerateKnightMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		return f_GenerateLineMoves(p_board, position, { {2,1},{2,-1},{-1,2},{1,2},{-2,1},{-2,-1},{-1,-2},{1,-2} }, false, only_enemy);
	}

	std::vector<Move> MoveGenerator::GenerateKingMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		return f_GenerateLineMoves(p_board, position, { {1,1},{-1,1},{-1,-1},{1,-1},{1,0},{0,1},{-1,0},{0,-1} }, false, only_enemy);
	}

	std::vector<Move> MoveGenerator::GeneratePawnMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		auto is_white_move = p_board.GetIsWhiteMove();
		auto &arr = p_board.GetBoard();
		auto moves = std::vector<Move>();
		auto prev_move = p_board.GetPreviousMove();

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

	std::vector<Move> MoveGenerator::GenerateCastlings(const Board& p_board, const bool is_white_turn) const
	{
		if(CanKingBeInCheck(p_board, is_white_turn))
			return std::vector<Move>();

		auto caslstates = p_board.GetCastlingState();
		auto moves = std::vector<Move>();
		auto state = false;
		auto points = 0;
		auto x = int8_t(0);

		auto get_king_pos =  uint8_t(is_white_turn ? 60 : 4);

		for (uint8_t t = 0; t != 2; t++)
		{
			if (t == 0)
			{
				if (is_white_turn)
					state = caslstates.wk;
				else
					state = caslstates.bk;
			}
			else
			{
				if (is_white_turn)
					state = caslstates.wq;
				else
					state = caslstates.bq;
			}
			if (!state)
				continue;
			points = 0;
			for (x = (t == 0 ? 1 : -1); abs(x) < 3; x += (t == 0 ? 1 : -1))
			{
				
				auto cpy = p_board;
				cpy.ForcedMove(Move{ get_king_pos, uint8_t(get_king_pos + x) });
				auto mg = MoveGenerator();
				if (!mg.CanKingBeInCheck(p_board, is_white_turn))
					points++;
				if (abs(x + (t == 0 ? 1 : -1)) == 3)
					break;
			}
			for (int8_t f = (t == 0 ? 1 : -1); abs(f) < 3 + t; f += (t == 0 ? 1 : -1))
				points += (p_board.GetBoard()[get_king_pos + f].side == SideType::none);
			if (points == 4 + t)
				moves.push_back(Move{ get_king_pos, uint8_t(get_king_pos + x) });
		}

		return moves;
	}

	bool MoveGenerator::CanKingBeInCheck(const Board& p_board, const bool is_white_turn) const
	{
		auto &arr = p_board.GetBoard();
		auto king_index = uint8_t(0);
		auto side = [=](bool is_white) {return is_white ? SideType::white : SideType::black; };

		for (king_index; king_index < arr.size(); king_index++)
			if (!(arr[king_index].type != PieceType::king || arr[king_index].side == side(is_white_turn)))
				break;

		auto enemy_list_rook = GenerateRookMove(p_board, king_index, true);
		auto enemy_list_bishop = GenerateBishopMove(p_board, king_index, true);
		auto enemy_list_knight = GenerateKnightMove(p_board, king_index, true);
		auto enemy_list_pawn = f_GeneratePawnAttackMoves(p_board, king_index, true);
		auto enemy_list_king = GenerateKingMove(p_board, king_index, true);
		auto enemy_list_queen = GenerateQueenMove(p_board, king_index, true);

		for (auto move : enemy_list_knight)
			if (arr[move.GetP2()].type == PieceType::knight && arr[move.GetP2()].side != side(is_white_turn))
				return true;
		for (auto move : enemy_list_rook)
			if (arr[move.GetP2()].type == PieceType::rook && arr[move.GetP2()].side == side(is_white_turn))
				return true;
		for (auto move : enemy_list_bishop)
			if (arr[move.GetP2()].type == PieceType::bishop && arr[move.GetP2()].side == side(is_white_turn))
				return true;
		for (auto move : enemy_list_pawn)
			if (arr[move.GetP2()].type == PieceType::pawn && arr[move.GetP2()].side == side(is_white_turn))
				return true;
		for (auto move : enemy_list_king)
			if (arr[move.GetP2()].type == PieceType::king && arr[move.GetP2()].side == side(is_white_turn))
				return true;
		for (auto move : enemy_list_queen)
			if (arr[move.GetP2()].type == PieceType::queen && arr[move.GetP2()].side == side(is_white_turn))
				return true;

		return false;
	}
}
