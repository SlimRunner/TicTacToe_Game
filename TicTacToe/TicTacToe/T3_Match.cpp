#include "T3_Match.h"

t3g::T3_Match::T3_Match() noexcept :
	t3g::T3_Board(),
	m_turn_switch(t3g::T3_cell_state::X_STATE)
{
	//nothing else to do
}

t3g::T3_Match::T3_Match(t3g::T3_cell_state starting_symbol) :
	t3g::T3_Board()
{
	switch (starting_symbol)
	{
	case t3g::T3_cell_state::NULL_STATE:
		m_turn_switch = t3g::T3_cell_state::X_STATE;
		break;
	case t3g::T3_cell_state::X_STATE:
		m_turn_switch = starting_symbol;
		break;
	case t3g::T3_cell_state::O_STATE:
		m_turn_switch = starting_symbol;
		break;
	default:
		throw; //TODO: replace with custom exception
		break;
	}
}

t3g::T3_Match::~T3_Match() noexcept
{
	//nothing to do
}

t3g::T3_board_state t3g::T3_Match::make_move(t3g::cell_loc x_loc, t3g::cell_loc y_loc)
{
	t3g::T3_cell_state curr_cell = get_cell_state(x_loc, y_loc);

	switch (get_board_status())
	{
	case t3g::T3_board_state::EMPTY_BOARD:
	case t3g::T3_board_state::NORMAL_BOARD:
		if (curr_cell == t3g::T3_cell_state::NULL_STATE)
		{
			using ret_t3 = t3g::T3_cell_state; //temporary alias
			
			t3g::T3_board_state retVal = set_cell_state(x_loc, y_loc, m_turn_switch);
			m_turn_switch = (m_turn_switch == ret_t3::X_STATE ? ret_t3::O_STATE : ret_t3::X_STATE);
			return retVal;
		}
		else
		{
			throw Ex_invalid_move();
		}
		break;
	case t3g::T3_board_state::TIED_BOARD:
	case t3g::T3_board_state::X_WIN_BOARD:
	case t3g::T3_board_state::O_WIN_BOARD:
		return get_board_status();
		break;
	default:
		throw; //TODO: replace with custom exception
		break;
	}
}

t3g::T3_board_state t3g::T3_Match::make_move(t3g::cell_loc serial)
{
	return make_move(serial % BOARD_SIDES, serial / BOARD_SIDES);
}

void t3g::T3_Match::clear_board() noexcept
{
	m_turn_switch = t3g::T3_cell_state::X_STATE;
	T3_Board::clear_board();
}

void t3g::T3_Match::clear_board(T3_cell_state default_symbol) noexcept
{
	m_turn_switch = default_symbol;

	if (m_turn_switch != T3_cell_state::X_STATE && m_turn_switch != T3_cell_state::O_STATE)
	{
		m_turn_switch = T3_cell_state::X_STATE;
	}

	T3_Board::clear_board();
}

t3g::T3_cell_state t3g::T3_Match::get_curr_symbol() const
{
	return m_turn_switch;
}
