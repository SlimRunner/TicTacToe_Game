#include "TicTacToe_AI.h"
#include "T3_Match.h"

#include <cstdlib>
#include <ctime>

template <class T, class ... Args> T rand_select(T, Args ...) noexcept;
template <class T> T rand_ratio_pick(T, T, float);

constexpr float operator "" _per(long double percent)
{
	return static_cast<float>(percent / 100);
}

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
	/*TEMP*/srand(static_cast<unsigned int>(time(NULL)));
}

tic::TicTacToe_AI::TicTacToe_AI(AI_Level difficulty) :
	m_myDiff(difficulty)
{
	//nothing else to do
	/*TEMP*/srand(static_cast<unsigned int>(time(NULL)));
}

tic::TicTacToe_AI::~TicTacToe_AI()
{
	//nothing to do
}

t3g::cell_loc tic::TicTacToe_AI::get_move(const t3g::T3_Match & currMatch) const
{
	/*  ALL THE CODE INSIDE THIS DEFINITION IS TEMPORARY  */
	t3g::cell_loc aiSel;
	
	if (currMatch.get_board_status() == t3g::T3_board_state::EMPTY_BOARD)
	{
		aiSel = rand_select(0, 2, 6, 8); //selects any of the corners
		aiSel = rand_ratio_pick(static_cast<int>(aiSel), 4, 80.0_per); //80% of the time stays with corners, otherwise it will select the center
	}
	else
	{
		//do recursive function here
		do
		{
			aiSel = rand() % t3g::BOARD_SIZE;
			//get offense map
			//get defense map
			//randLoc = 0;
		} while (currMatch.get_cell_state(aiSel) != t3g::T3_cell_state::NULL_STATE);
	}

	return aiSel;
}

template<class T, class ... Args>
T rand_select(T first, Args ... args) noexcept
{
	T arr[sizeof...(args) + 1] = { first, args... };

	return arr[rand() % (sizeof...(args) + 1)];
}

template <class T>
T rand_ratio_pick(T in1, T in2, float ratio)
{
	if (ratio >= 1) return in1; //one is always true
	if (ratio <= 0) return in2; //zero is always false

	int threshold = static_cast<int>((RAND_MAX + 1) * ratio);
	int output = rand();

	if (output < threshold)
	{
		return in1;
	}
	else
	{
		return in2;
	}
}