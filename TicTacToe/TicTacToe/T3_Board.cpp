#include "T3_Board.h"

#define WIN_MASK(bField, mask) ((bField & mask) == mask)

static size_t get_xy_index(size_t x_loc, size_t y_loc, size_t width)
{
	return x_loc + y_loc * width;
}

/*Default constructor*/
t3g::T3_Board::T3_Board() noexcept :
	m_board { T3_cell_state::NULL_STATE },
	m_cond_board { T3_cell_condition::NORMAL },
	m_state(t3g::T3_board_state::EMPTY_BOARD)
{
	//nothing else to do
}

//Destructor
t3g::T3_Board::~T3_Board() noexcept
{
	//nothing to do
}

//Sets the value of an individual cell in the board and returns the state of the board
t3g::T3_board_state t3g::T3_Board::set_cell_state(cell_loc x_loc, cell_loc y_loc, T3_cell_state state)
{
	cell_loc index;

	if (x_loc > BOARD_SIDES || y_loc > BOARD_SIDES)
	{
		throw Ex_out_of_bounds();
	}
	
	index = get_xy_index(x_loc, y_loc, BOARD_SIDES);
	m_board[index] = state; //sets current state

	return determine_board_state(x_loc, y_loc);
}

t3g::T3_board_state t3g::T3_Board::set_cell_state(cell_loc serial, T3_cell_state state)
{
	return set_cell_state(serial % BOARD_SIDES, serial / BOARD_SIDES, state);
}

//Clears board and sets the whole board at once from an array
t3g::T3_board_state t3g::T3_Board::set_cell_state(T3_cell_state * in_board, size_t arr_size)
{
	if (arr_size == t3g::BOARD_SIZE)
	{
		clear_board();
		for (size_t i = 0; i < arr_size; ++i)
			m_board[i] = in_board[i];
	}
	else
	{
		throw Ex_out_of_bounds();
	}

	return determine_board_state();
}

//Gets current state of a single cell
t3g::T3_cell_state t3g::T3_Board::get_cell_state(cell_loc x_loc, cell_loc y_loc) const
{
	if (x_loc > BOARD_SIDES || y_loc > BOARD_SIDES)
	{
		throw Ex_out_of_bounds();
	}

	cell_loc index = get_xy_index(x_loc, y_loc, BOARD_SIDES);
	return m_board[index];
}

t3g::T3_cell_state t3g::T3_Board::get_cell_state(cell_loc serial) const
{
	return get_cell_state(serial % BOARD_SIDES, serial / BOARD_SIDES);
}

//Gets current condition of a single cell
t3g::T3_cell_condition t3g::T3_Board::get_cell_condition(cell_loc x_loc, cell_loc y_loc) const
{
	if (x_loc > BOARD_SIDES || y_loc > BOARD_SIDES)
	{
		throw Ex_out_of_bounds();
	}

	cell_loc index = get_xy_index(x_loc, y_loc, BOARD_SIDES);
	return m_cond_board[index];
}

t3g::T3_cell_condition t3g::T3_Board::get_cell_condition(cell_loc serial) const
{
	return get_cell_condition(serial % BOARD_SIDES, serial / BOARD_SIDES);
}

//Gets the board state without chaging any internal values
t3g::T3_board_state t3g::T3_Board::get_board_status() const noexcept
{
	return m_state;
}

//Clears the board to it's initial state
void t3g::T3_Board::clear_board() noexcept
{
	for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
	{
		m_board[i] = t3g::T3_cell_state::NULL_STATE;
		m_cond_board[i] = t3g::T3_cell_condition::NORMAL;
	}
	m_state = t3g::T3_board_state::EMPTY_BOARD;
}

//Determines the state of the board and updates the condition array accordingly
t3g::T3_board_state t3g::T3_Board::determine_board_state()
{
	enum Normal_states
	{
		EMPTY_BOARD = 0,
		FULL_BOARD = 0x1ff
	};

	size_t x_bitfield = 0, o_bitfield = 0;

	for (cell_loc i = 0; i < BOARD_SIZE; ++i)
	{
		switch (m_board[i])
		{
		case t3g::T3_cell_state::NULL_STATE:
			break;
		case t3g::T3_cell_state::X_STATE:
			x_bitfield |= 1 << i;
			break;
		case t3g::T3_cell_state::O_STATE:
			o_bitfield |= 1 << i;
			break;
		default:
			break;
		}
	}


	if (is_win_state(x_bitfield))
	{
		//x has a win pattern
		m_state = T3_board_state::X_WIN_BOARD;
	}
	else if (is_win_state(o_bitfield))
	{
		//o has a win pattern
		m_state = T3_board_state::O_WIN_BOARD;
	}
	else
	{
		//no win pattern found
		switch (x_bitfield | o_bitfield)
		{
		case EMPTY_BOARD:
			m_state = T3_board_state::EMPTY_BOARD;
			break;
		case FULL_BOARD:
			m_state = T3_board_state::TIED_BOARD;
			break;
		default:
			m_state = T3_board_state::NORMAL_BOARD;
			break;
		}
	}

	return m_state;
}

//Determines the state of the board, updates the condition array accordingly, and sets the last changed item
t3g::T3_board_state t3g::T3_Board::determine_board_state(cell_loc x_loc, cell_loc y_loc)
{
	T3_board_state retVal;

	for (int i = 0; i < BOARD_SIZE; i++)
		m_cond_board[i] = t3g::T3_cell_condition::NORMAL;

	retVal = determine_board_state();
	cell_loc index = get_xy_index(x_loc, y_loc, BOARD_SIDES);
	m_cond_board[index] = m_cond_board[index] | t3g::T3_cell_condition::LAST_MOVE; //sets last move

	return retVal;
}

//Returns true if a win state has been reached
bool t3g::T3_Board::is_win_state(size_t bit_field)
{
	enum Win_states
	{
		HORZ_UPP = 0x7,
		HORZ_MID = 0x38,
		HORZ_LOW = 0x1c0,
		VERT_UPP = 0x49,
		VERT_MID = 0x92,
		VERT_LOW = 0x124,
		DIAG_0T8 = 0x111, //NW to SE
		DIAG_2T6 = 0x54   //NE to SW
	};

	const size_t ARR_LEN = 8;
	bool retVal = false;
	Win_states ws_array[] = { HORZ_UPP, HORZ_MID, HORZ_LOW, VERT_UPP, VERT_MID, VERT_LOW, DIAG_0T8, DIAG_2T6 };

	for (size_t i = 0; i < ARR_LEN; ++i)
	{
		if WIN_MASK(bit_field, ws_array[i])
		{
			switch (ws_array[i])
			{
			case HORZ_UPP: m_cond_board[0] = m_cond_board[1] = m_cond_board[2] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case HORZ_MID: m_cond_board[3] = m_cond_board[4] = m_cond_board[5] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case HORZ_LOW: m_cond_board[6] = m_cond_board[7] = m_cond_board[8] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case VERT_UPP: m_cond_board[0] = m_cond_board[3] = m_cond_board[6] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case VERT_MID: m_cond_board[1] = m_cond_board[4] = m_cond_board[7] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case VERT_LOW: m_cond_board[2] = m_cond_board[5] = m_cond_board[8] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case DIAG_0T8: m_cond_board[0] = m_cond_board[4] = m_cond_board[8] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			case DIAG_2T6: m_cond_board[2] = m_cond_board[4] = m_cond_board[6] = t3g::T3_cell_condition::WIN_FLAG;
				break;
			default:
				break;
			}

			retVal = true;
		}
	}
	return retVal;
}