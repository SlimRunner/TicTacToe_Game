#ifndef T3_BOARD_H
#define T3_BOARD_H

namespace t3g
{
	constexpr size_t BOARD_SIZE = 9; //Total of cells in the board
	constexpr size_t BOARD_SIDES = 3; //Size of sides of board. Board is a square.

	typedef size_t cell_loc; //defines the serial number of a cell, its location.

	enum class T3_cell_state
	{
		NULL_STATE = 0,
		X_STATE = 1,
		O_STATE = 2
	};

	enum class T3_cell_condition
	{
		NORMAL = 0,
		LAST_MOVE = 1,
		WIN_FLAG = 2
	};
	inline T3_cell_condition operator | (T3_cell_condition lhs, const T3_cell_condition & rhs)
	{
		return static_cast<T3_cell_condition>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	enum class T3_board_state
	{
		EMPTY_BOARD = 0,
		NORMAL_BOARD = 1,
		TIED_BOARD = 2,
		X_WIN_BOARD = 3,
		O_WIN_BOARD = 4
	};

	///<summary>Manages a board of Tic Tac Toe</summary>
	class T3_Board
	{
	public: //---------------- P U B L I C ----------------//

		/***** EXCEPTION HANDLING *****/

		class Ex_out_of_bounds { };

		/******** CONSTRUCTORS ********/

		T3_Board() noexcept;
		~T3_Board() noexcept;

		/********* ACCESSORS **********/

		T3_cell_state get_cell_state(cell_loc x_loc, cell_loc y_loc) const;
		T3_cell_state get_cell_state(cell_loc serial) const;
		T3_cell_condition get_cell_condition(cell_loc x_loc, cell_loc y_loc) const;
		T3_cell_condition get_cell_condition(cell_loc serial) const;
		T3_board_state get_board_status() const noexcept;

		/********** MUTATORS **********/

		void clear_board() noexcept;

	protected:
		T3_board_state set_cell_state(cell_loc x_loc, cell_loc y_loc, T3_cell_state state);
		T3_board_state set_cell_state(cell_loc serial, T3_cell_state state);

		//This function is intended to be used to load a saved game, should not be used
		//as a replacement of the other overload since this one bypasses validation
		T3_board_state set_cell_state(T3_cell_state * in_board, size_t arr_size);

	private: //--------------- P R I V A T E ---------------//
		
		/****** MEMBER VARIABLES ******/

		T3_cell_state m_board[BOARD_SIZE];
		T3_cell_condition m_cond_board[BOARD_SIZE];
		T3_board_state m_state;

		/****** MEMBER FUNCTIONS ******/

		T3_board_state determine_board_state();
		T3_board_state determine_board_state(cell_loc x_loc, cell_loc y_loc);
		bool is_win_state(size_t bit_field);
	};
}


#endif // !T3_BOARD_H