#include <iostream>
#include "TicTacToe_cli.h"

int main()
{
	//t3g::T3_Match myBoard;

	//std::cout << static_cast<int>(myBoard.make_move(0, 0)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(1, 1)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(2, 0)) << std::endl; //to get winner swap this line with the one above
	//std::cout << static_cast<int>(myBoard.make_move(2, 1)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(2, 2)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(1, 0)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(0, 1)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(0, 2)) << std::endl;
	//std::cout << static_cast<int>(myBoard.make_move(1, 2)) << std::endl << std::endl;

	//for (size_t y = 0; y < t3g::BOARD_SIDES; ++y)
	//{
	//	for (size_t x = 0; x < t3g::BOARD_SIDES; ++x)
	//		std::cout << static_cast<int>(myBoard.get_cell_condition(x,y)) << "\t";
	//	std::cout << "\n";
	//}

	//std::cout << std::endl;

	//for (size_t y = 0; y < t3g::BOARD_SIDES; ++y)
	//{
	//	for (size_t x = 0; x < t3g::BOARD_SIDES; ++x)
	//		std::cout << static_cast<int>(myBoard.get_cell_state(x, y)) << "\t";
	//	std::cout << "\n";
	//}

	tic::TicTacToe_cli thisMatch;

	thisMatch.runGame();
	return 0;
}