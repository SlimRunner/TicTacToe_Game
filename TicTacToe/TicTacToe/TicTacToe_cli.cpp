#include "TicTacToe_cli.h"
#include "TicTacToe_AI.h"
#include <iostream>
#include <string>

#define ENUM_TO_INT(inEnum) static_cast<char>(inEnum)

static const char ASTERISK_SEPARATOR[65] = "****************************************************************";

static char prompt_getChar(const char *, bool);

tic::TicTacToe_cli::TicTacToe_cli() :
	m_thisMatch(t3g::T3_cell_state::X_STATE),
	m_ai_diff(AI_Level::AI_MEDIUM)
{
	m_starting_symbol = m_thisMatch.get_curr_symbol();
}

tic::TicTacToe_cli::~TicTacToe_cli()
{
	//nothing to do
}

void tic::TicTacToe_cli::runGame()
{
	std::cout << "TicTacToe in command-line. Have fun!\n";

	do { } while (runMainMenu() != OptMainMenu::MNU_EXIT);
}

tic::OptMainMenu tic::TicTacToe_cli::runMainMenu()
{
	OptMainMenu userSel;

	std::cout << "\n" << ASTERISK_SEPARATOR;
	std::cout << "\nMAIN MENU\n\n"; //cascade pass the menu name as argument to the next menu

	std::cout << "Play a match\t(" << ENUM_TO_INT(OptMainMenu::MNU_PLAY) << ")\n";
	std::cout << "Settings\t(" << ENUM_TO_INT(OptMainMenu::MNU_SETTINGS) << ")\n";
	std::cout << "Exit Game\t(" << ENUM_TO_INT(OptMainMenu::MNU_EXIT) << ")\n";
	userSel = static_cast<OptMainMenu>(prompt_getChar("\nSelect an option: ", true));

	switch (userSel)
	{
	case tic::OptMainMenu::MNU_PLAY:
		do {} while (runPlayMenu() != OptPlayMatch::SUB_EXIT);
		break;
	case tic::OptMainMenu::MNU_SETTINGS:
		do {} while (runSettings() != OptSettings::SUB_EXIT);
		break;
	case tic::OptMainMenu::MNU_EXIT:
		break;
	default:
		std::cout << "\nThat is not a valid command.\n\a";
		break;
	}
	return userSel;
}

tic::OptPlayMatch tic::TicTacToe_cli::runPlayMenu()
{
	OptPlayMatch userSel;

	std::cout << "\n" << ASTERISK_SEPARATOR;
	std::cout << "\nMAIN MENU > PLAY MATCH\n\n"; //cascade pass the menu name as argument to the next menu

	std::cout << "1v1 match\t(" << ENUM_TO_INT(OptPlayMatch::SUB_1V1) << ")\n";
	std::cout << "Match vs AI\t(" << ENUM_TO_INT(OptPlayMatch::SUB_AI) << ")\n";
	/*std::cout << "1v1 rev match\t(" << ENUM_TO_INT(OptPlayMatch::SUB_REV_1V1) << ")\n";
	std::cout << "Rev Match vs AI\t(" << ENUM_TO_INT(OptPlayMatch::SUB_REV_AI) << ")\n";*/
	std::cout << "Go back\t\t(" << ENUM_TO_INT(OptPlayMatch::SUB_EXIT) << ")\n";
	userSel = static_cast<OptPlayMatch>(prompt_getChar("\nSelect an option: ", true));

	switch (userSel)
	{
	case tic::OptPlayMatch::SUB_1V1:
		runMatch(false);
		break;
	case tic::OptPlayMatch::SUB_AI:
		runMatch(true);
		break;
	/*case tic::OptPlayMatch::SUB_REV_1V1:
		break;
	case tic::OptPlayMatch::SUB_REV_AI:
		break;*/
	case tic::OptPlayMatch::SUB_EXIT:
		break;
	default:
		std::cout << "\nThat is not a valid command.\n\a";
		break;
	}

	return userSel;
}

tic::OptSettings tic::TicTacToe_cli::runSettings()
{
	OptSettings userSel;

	std::cout << "\n" << ASTERISK_SEPARATOR;
	std::cout << "\nMAIN MENU > SETTINGS\n\n"; //cascade pass the menu name as argument to the next menu

	std::cout << "Default symbol\t(" << ENUM_TO_INT(OptSettings::SUB_DEF_SYMBOL) << ") Current symbol is " << getSymbol(m_starting_symbol) << "\n";
	/*std::cout << "AI difficulty\t\t(" << ENUM_TO_INT(OptSettings::SUB_AI_DIFFICULTY) << ")\n";*/
	std::cout << "Go back\t\t(" << ENUM_TO_INT(OptSettings::SUB_EXIT) << ")\n";
	userSel = static_cast<OptSettings>(prompt_getChar("\nSelect an option: ", true));

	switch (userSel)
	{
	case tic::OptSettings::SUB_DEF_SYMBOL:
		if (m_starting_symbol == t3g::T3_cell_state::X_STATE)
		{
			m_starting_symbol = t3g::T3_cell_state::O_STATE;
		}
		else
		{
			m_starting_symbol = t3g::T3_cell_state::X_STATE;
		}

		m_thisMatch.clear_board(m_starting_symbol);

		break;
		/*case tic::OptSettings::SUB_AI_DIFFICULTY:
			break;*/
	case tic::OptSettings::SUB_EXIT:
		break;
	default:
		std::cout << "\nThat is not a valid command.\n\a";
		break;
	}

	return userSel;
}

t3g::T3_cell_state tic::TicTacToe_cli::chooseSym_prompt(const char * message)
{
	char userSel;

	std::cout << getSymbol(m_starting_symbol) << " goes first\n";
	do
	{
		userSel = prompt_getChar(message, true);

		switch (userSel)
		{
		case 'X':
			return t3g::T3_cell_state::X_STATE;
			//break;
		case 'O':
			return t3g::T3_cell_state::O_STATE;
			//break;
		default:
			std::cout << "That's not a valid symbol";
			break;
		}
	} while (true);
}

void tic::TicTacToe_cli::runMatch(bool ai_match)
{
	const char SOFT_SEPARATOR[33] = "--------------------------------";
	size_t turnCount = 1;

	//lambda expression to print board
	auto printBoard = [&]()
	{
		for (int y = 0; y < t3g::BOARD_SIDES; ++y)
		{
			for (int x = 0; x < t3g::BOARD_SIDES; ++x)
			{
				std::cout << getSymbol(m_thisMatch.get_cell_state(x, y)) << "\t";
			}
			std::cout << "\n\n";
		}
	};

	std::cout << "\n" << ASTERISK_SEPARATOR;
	std::cout << "\nMAIN MENU > PLAY MATCH > 1v1\n\n"; //cascade pass the menu name as argument to the next menu
	
	std::cout << "To input a symbol use the numeric keyboard\n\n";
	
	TicTacToe_AI thisAI(m_ai_diff);
	t3g::T3_cell_state userSymbol = t3g::T3_cell_state::NULL_STATE;

	if (ai_match)
	{
		std::cout << SOFT_SEPARATOR << "\n";
		userSymbol = chooseSym_prompt("Select your symbol [X/O]: ");
	}

	do
	{
		std::cout << SOFT_SEPARATOR << "\n";
		std::cout << "Current board - Turn " << turnCount << "\n";
		std::cout << getSymbol(m_thisMatch.get_curr_symbol()) << " is playing" << "\n\n";
		
		printBoard();

		char symLoc; /*location to insert next symbol*/
		t3g::cell_loc serialLoc;
		

		try
		{
			if (!ai_match || userSymbol == m_thisMatch.get_curr_symbol())
			{
				symLoc = prompt_getChar("Make move: ", false);
				while (m_thisMatch.get_cell_condition(serialLoc = getSerialFromKeypad(symLoc)) != t3g::T3_cell_condition::NORMAL)
				{
					std::cout << "This cell is occupied already\n";
					symLoc = prompt_getChar("Make move: ", false);
				}
			}
			else
			{
				serialLoc = thisAI.get_move(m_thisMatch);
			}

			m_thisMatch.make_move(serialLoc);
			turnCount++;
		}
		catch (const t3g::T3_Board::Ex_out_of_bounds&)
		{
			std::cout << "Not a valid location\n";
		}
		catch (const t3g::T3_Match::Ex_invalid_move&)
		{
			std::cout << "An invalid move was made\n";
		}

	} while (m_thisMatch.get_board_status() == t3g::T3_board_state::EMPTY_BOARD ||
		m_thisMatch.get_board_status() == t3g::T3_board_state::NORMAL_BOARD);

	std::cout << SOFT_SEPARATOR << "\n";
	std::cout << "Final board\n\n";
	printBoard();

	/*This switch could be sent to it's own function*/
	switch (m_thisMatch.get_board_status())
	{
	case t3g::T3_board_state::TIED_BOARD:
		std::cout << "DRAW\n\n";
		break;
	case t3g::T3_board_state::X_WIN_BOARD:
		std::cout << "X WINS!!\n\n";
		break;
	case t3g::T3_board_state::O_WIN_BOARD:
		std::cout << "O WINS!!\n\n";
		break;
	default:
		std::cout << "This should have not happened!!\n\n";
		break;
	}

	std::cout << "press any key to continue...";
	std::cin.get(); //pause
	m_thisMatch.clear_board(m_starting_symbol);
}

char tic::TicTacToe_cli::getSymbol(t3g::T3_cell_state thisCell) const
{
	char retVal;
	switch (thisCell)
	{
	case t3g::T3_cell_state::NULL_STATE:
		retVal = '-';
		break;
	case t3g::T3_cell_state::X_STATE:
		retVal = 'X';
		break;
	case t3g::T3_cell_state::O_STATE:
		retVal = 'O';
		break;
	default:
		retVal = '\0';
		break;
	}

	return retVal;
}

t3g::cell_loc tic::TicTacToe_cli::getSerialFromKeypad(char inCharacter) const
{
	t3g::cell_loc retVal;

	switch (inCharacter)
	{
	case '7': //upper left
		retVal = 0;
		break;
	case '8': //upper middle
		retVal = 1;
		break;
	case '9': //upper right
		retVal = 2;
		break;
	case '4': //mid left
		retVal = 3;
		break;
	case '5': //mid middle
		retVal = 4;
		break;
	case '6': //mid right
		retVal = 5;
		break;
	case '1': //lower left
		retVal = 6;
		break;
	case '2': //lower middle
		retVal = 7;
		break;
	case '3': //lower right
		retVal = 8;
		break;
	default:
		retVal = UINT_MAX;
		break;
	}

	return retVal;
}

///<summary>Shows a prompt to the user, accepts a char input, and then returns the character to the caller.</summary>
///<param name = "message">String literal that will be shown before requesting the input.</param>
///<param name = "makeUpper">Determines if the output charater is converted upper case.</param>
///<returns>Returns the character value entered by user.</returns>
static char prompt_getChar(const char * message, bool makeUpper)
{
	std::string inStr;
	bool firstLoop = true;

	while ((std::cout << message) //return is irrelevant, it's here to loop the message
		&& !(std::getline(std::cin, inStr)) //return is null when input is not valid
		|| (inStr.length() != 1))
	{
		std::cout << "Not a valid character input." << std::endl;
		if (std::cin.eof()) std::cin.clear(); //clears error if user enters ^Z(EOF)
	}

	if (!makeUpper)
		return inStr.at(0);
	else
		return toupper(inStr.at(0));
}