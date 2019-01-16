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
	t3g::cell_loc badStart = rand_select(1, 3, 5, 7);
	t3g::cell_loc aiSel;
	
	if (currMatch.get_board_status() == t3g::T3_board_state::EMPTY_BOARD)
	{
		aiSel = rand_select(0, 2, 6, 8); //pre-selects any of the corners

		switch (m_myDiff)
		{
		case tic::AI_Level::AI_EASY:
			//20% of the time stays with corners, otherwise it will select the center
			aiSel = rand_ratio_pick(static_cast<int>(aiSel), 4, 20.0_per);
			//30% of the time stays with good start, otherwise it will select at random among bad choices
			aiSel = rand_ratio_pick(static_cast<int>(aiSel), static_cast<int>(badStart), 30.0_per);

			break;
		case tic::AI_Level::AI_MEDIUM:
			//20% of the time stays with corners, otherwise it will select the center
			aiSel = rand_ratio_pick(static_cast<int>(aiSel), 4, 20.0_per);
			//95% of the time stays with good start, otherwise it will select at random among bad choices
			aiSel = rand_ratio_pick(static_cast<int>(aiSel), static_cast<int>(badStart), 95.0_per);
			break;
		case tic::AI_Level::AI_HARD:
			//90% of the time stays with corners, otherwise it will select the center
			aiSel = rand_ratio_pick(static_cast<int>(aiSel), 4, 90.0_per);
			break;
		default:
			break;
		}
	}
	else
	{
		//do recursive function here
		do
		{
			aiSel = rand() % t3g::BOARD_SIZE;
			/*selMap = get_minimax_map(currMatch)*/
			switch (m_myDiff)
			{
			case tic::AI_Level::AI_EASY:
				/*Select index which contains 1 rarely unless there is none, in that case select index with 0*/
				/*often select index with -1*/
				break;
			case tic::AI_Level::AI_MEDIUM:
				/*Select index which contains 1 sometimes unless there is none, in that case select index with 0*/
				/*sometimes select index with -1*/
				break;
			case tic::AI_Level::AI_HARD:
				/*Select index which contains 1 always unless there is none, in that case select index with 0*/
				break;
			default:
				break;
			}
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