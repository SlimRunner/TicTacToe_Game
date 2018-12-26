#ifndef TICTACTOE_CLI_H
#define TICTACTOE_CLI_H

#include "T3_Match.h"

namespace tic
{
	enum class OptMainMenu : char
	{
		MNU_PLAY = '1',
		MNU_SETTINGS = '2',
		MNU_EXIT = '0'
	};

	enum class OptPlayMatch : char
	{
		SUB_1V1 = '1',
		SUB_AI = '2',
		SUB_REV_1V1 = '3',
		SUB_REV_AI = '4',
		SUB_EXIT = '0'
	};

	class TicTacToe_cli
	{
	public:
		TicTacToe_cli();
		~TicTacToe_cli();

		void runGame();
		
	private:
		t3g::T3_Match m_thisMatch;

		OptMainMenu runMainMenu();
		OptPlayMatch runPlayMenu();

		void runMatch();
		char getSymbol(t3g::T3_cell_state) const;
		t3g::cell_loc getSerialFromKeypad(char) const;
	};
}

#endif // !TICTACTOE_CLI_H
