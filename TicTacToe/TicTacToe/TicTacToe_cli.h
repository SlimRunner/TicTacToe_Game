#ifndef TICTACTOE_CLI_H
#define TICTACTOE_CLI_H

#include "T3_Match.h"
#include "TicTacToe_AI.h"

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

	enum class OptSettings : char
	{
		SUB_DEF_SYMBOL = '1',
		SUB_AI_DIFFICULTY = '2',
		SUB_EXIT = '0'
	};

	class TicTacToe_cli
	{
	public:
		TicTacToe_cli();
		TicTacToe_cli(t3g::T3_cell_state starting_symbol);
		TicTacToe_cli(tic::AI_Level ai_difficulty);
		TicTacToe_cli(tic::AI_Level ai_difficulty, t3g::T3_cell_state starting_symbol);
		~TicTacToe_cli();

		void runGame();
		
	private:
		t3g::T3_Match m_thisMatch;

		t3g::T3_cell_state m_starting_symbol;
		tic::AI_Level m_ai_diff;

		OptMainMenu runMainMenu();
		OptPlayMatch runPlayMenu();
		OptSettings runSettings();
		//shows a quick prompt to set the difficulty of the AI
		void setDifficulty_prompt();

		//shows a quick prompt to retrieve the symbol the player wants to use when playing against the AI
		t3g::T3_cell_state chooseSym_prompt(const char * message);
		
		void printBoard();
		void printResult();
		void runMatch(bool ai_match);
		char getSymbol(t3g::T3_cell_state) const;
		const char * getDiffName(tic::AI_Level) const;
		t3g::cell_loc getSerialFromKeypad(char) const;
	};
}

#endif // !TICTACTOE_CLI_H
