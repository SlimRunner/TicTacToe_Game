#include "TicTacToe_AI.h"
#include "T3_Match.h"

#include <cstdlib>
#include <ctime>

enum class tic::AI_Level
{
	AI_EASY = 0,
	AI_MEDIUM = 1,
	AI_HARD = 2
};

tic::TicTacToe_AI::TicTacToe_AI() :
	m_myDiff(AI_Level::AI_MEDIUM)
{
	//nothing else to do
	/*TEMP*/srand(time(NULL));
}

tic::TicTacToe_AI::TicTacToe_AI(AI_Level difficulty) :
	m_myDiff(difficulty)
{
	//nothing else to do
	/*TEMP*/srand(time(NULL));
}

tic::TicTacToe_AI::~TicTacToe_AI()
{
	//nothing to do
}

t3g::cell_loc tic::TicTacToe_AI::get_move(t3g::T3_Match & currMatch)
{
	/*  ALL THE CODE INSIDE THIS DEFINITION IS TEMPORARY  */

	//bool found = false;
	t3g::cell_loc randLoc;

	/*do
	{
		randLoc = rand() % t3g::BOARD_SIZE;
		if (currMatch.get_cell_state(randLoc) == t3g::T3_cell_state::NULL_STATE)
		{
			found = true;
		}
	} while (!found);*/

	do
	{
		randLoc = rand() % t3g::BOARD_SIZE;
	} while (currMatch.get_cell_state(randLoc) != t3g::T3_cell_state::NULL_STATE);

	return randLoc;
}
