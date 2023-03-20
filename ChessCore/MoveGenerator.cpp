#include "MoveGenerator.h"

namespace chess_lib
{
	std::vector<Move> MoveGenerator::f_GenerateLineMoves(const Board& p_board, const uint8_t position, const std::vector<pos> vectors, bool iterative, bool only_enemy) const
	{
		auto& arr = p_board.GetBoard();
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
					if (!only_enemy)
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

		auto own_side = (is_white_move ? 1 : -1);

		for (int8_t i = -1; i < 2; i += 2)
		{
			if (!ChessConvertor::IsInBoard(pos{ int8_t(position % 8 + i) , int8_t(position / 8 + own_side) }))
				continue;
			moves.push_back(constr((position + i) + own_side * 8));
		}
		return moves;
	}

	void MoveGenerator::f_GenerateStraightForwardMove(std::vector<Move>& moves, const Board& board, const uint8_t position) const
	{
		auto& arr = board.GetBoard();
		auto is_white_move = board.GetIsWhiteMove();

		auto constr = [=](uint8_t ind) { return Move{ position, uint8_t(ind) }; };
		auto opposite_delta = (is_white_move ? -8 : 8);

		if (arr[position + opposite_delta].side == SideType::none)
		{
			moves.push_back(constr(position + opposite_delta));
			if (arr[position + opposite_delta * 2].side == SideType::none
				&& position / 8 == (is_white_move ? 6 : 1))
				moves.push_back(constr(position + opposite_delta * 2));
		}
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

	std::vector<Move> MoveGenerator::GenerateKingMove(const Board& p_board, const uint8_t position, bool only_enemy, bool only_moves) const
	{
		auto moves = f_GenerateLineMoves(p_board, position, { {1,1},{-1,1},{-1,-1},{1,-1},{1,0},{0,1},{-1,0},{0,-1} }, false, only_enemy);
		if (only_moves)
		{
			auto castling = GenerateCastlings(p_board, p_board.GetIsWhiteMove());
			for (auto& c : castling)
				moves.push_back(c);
		}
		return moves;
	}

	std::vector<Move> MoveGenerator::GeneratePawnMove(const Board& p_board, const uint8_t position, bool only_enemy) const
	{
		auto is_white_move = p_board.GetIsWhiteMove();
		auto& arr = p_board.GetBoard();
		auto moves = std::vector<Move>();
		auto prev_move = p_board.GetPreviousMove();

		f_GenerateStraightForwardMove(moves, p_board, position);

		auto opposite_piece = (is_white_move ? SideType::black : SideType::white);
		auto forward = is_white_move ? -1 : 1;

		auto constr = [=](uint8_t ind) { return Move{ position, uint8_t(ind) }; };

		// Attack
		for (int8_t i = -1; i < 2; i += 2)
		{
			if (!ChessConvertor::IsInBoard(pos{ (int8_t)(position % 8 + i), (int8_t)(position / 8 + forward) }))
				continue;
			if (arr[(position + i) + forward * 8].side == opposite_piece)
				moves.push_back(constr((position + i) + forward * 8));
		}

		// En Passant
		for (int8_t i = -1; i < 2; i += 2)
		{
			if (!ChessConvertor::IsInBoard(pos{ (int8_t)(position % 8 + i), (int8_t)(position / 8) }))
				continue;
				
			if (arr[position + i].side == opposite_piece
				&& prev_move->GetP1() % 8 == position % 8 + i
				&& prev_move->GetP1() / 8 == (is_white_move ? 1 : 6)
				&& arr[position + i].type == PieceType::pawn
				)
				moves.push_back(constr(position + i + (is_white_move ? -8 : 8)));
		}

		return moves;
	}

	std::vector<Move> MoveGenerator::GenerateCastlings(const Board& p_board, const bool is_white_turn) const
	{
		if (CanKingBeInCheck(p_board, is_white_turn))
			return std::vector<Move>();

		auto caslstates = p_board.GetCastlingState();
		auto moves = std::vector<Move>();
		auto state = false;
		auto points = 0;
		auto x = int8_t(0);

		const auto get_king_pos = uint8_t(is_white_turn ? 60 : 4);
		const auto get_king_tile = p_board.GetBoard()[get_king_pos];

		for (uint8_t t = 0; t < 2; t++)
		{
			const auto delta = t == 0 ? 1 : -1;
			// getting state
			if (!t)
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

			// checking for opportunity
			if (!state)
				continue;

			// counting a reasons for castling
			// first checking king's movement
			// second checking for empty space
			points = 0;
			for (int8_t i = delta; abs(i) < 3; i += delta)
			{
				auto cpy = p_board;
				cpy.ForcedMove(Move{ get_king_pos, uint8_t(get_king_pos + i) });
				if (!CanKingBeInCheck(cpy, !is_white_turn))
					points++;
			} // at result, if all conditions is perfect, [points] would be equal 2
			for (int8_t i = delta; abs(i) < 3 + t; i += delta)
			{
				if (p_board.GetBoard()[get_king_pos + i].side == SideType::none)
					points++;
			} // at result, if all conditions is perfect, [points] would be equal 4 - 5

			// at last, checking for having rook
			if (p_board.GetBoard()[get_king_pos + delta * 3 - t].side == get_king_tile.side && p_board.GetBoard()[get_king_pos + delta * 3 - t].type == PieceType::rook)
				points++;

			// making decision of castling move
			if (points == 5 + t)
				moves.push_back(Move{ get_king_pos, uint8_t(get_king_pos + delta * 2) });
		}

		return moves;
	}

	bool MoveGenerator::CanKingBeInCheck(const Board& p_board, const bool is_white_turn) const
	{
		auto& arr = p_board.GetBoard();
		auto king_index = uint8_t(0);
		auto side = [=](bool is_white) {return is_white ? SideType::white : SideType::black; };

		for (king_index; king_index < arr.size(); king_index++)
			if (!(arr[king_index].type != PieceType::king || arr[king_index].side == side(is_white_turn)))
				break;

		auto enemy_list_rook = GenerateRookMove(p_board, king_index, true);
		auto enemy_list_bishop = GenerateBishopMove(p_board, king_index, true);
		auto enemy_list_knight = GenerateKnightMove(p_board, king_index, true);
		auto enemy_list_pawn = f_GeneratePawnAttackMoves(p_board, king_index, true);
		auto enemy_list_king = GenerateKingMove(p_board, king_index, true, false);
		auto enemy_list_queen = GenerateQueenMove(p_board, king_index, true);

		for (auto move : enemy_list_knight)
			if (arr[move.GetP2()].type == PieceType::knight && arr[move.GetP2()].side == side(is_white_turn))
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
