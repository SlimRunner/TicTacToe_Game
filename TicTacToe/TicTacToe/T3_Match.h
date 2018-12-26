#ifndef T3_MATCH_H
#define T3_MATCH_H

#include "T3_Board.h"

namespace t3g
{
	///<summary>Uses a board of Tic Tac Toe and adds rules to it.</summary>
	class T3_Match : public t3g::T3_Board
	{
	public:
		class Ex_invalid_move {};

		T3_Match() noexcept;
		T3_Match(t3g::T3_cell_state starting_symbol);
		~T3_Match() noexcept;

		t3g::T3_board_state make_move(t3g::cell_loc x_loc, t3g::cell_loc y_loc);
		t3g::T3_board_state make_move(t3g::cell_loc serial);
		void clear_board() noexcept;
		void clear_board(t3g::T3_cell_state default_symbol) noexcept;

		t3g::T3_cell_state get_curr_symbol() const;

	private:
		t3g::T3_cell_state m_turn_switch;
	};
}

#endif // !T3_MATCH_H
