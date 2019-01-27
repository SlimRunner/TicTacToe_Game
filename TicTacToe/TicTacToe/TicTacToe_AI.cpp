#include "TicTacToe_AI.h"
#include "MinMaxBoard.h"
#include "T3_Match.h"

#include <limits>
#include <cassert>
#include <cstdlib>
#include <ctime>
//#include <random> //SUGGESTION: use instead of ctime and cstdlib

/*PROTOTYPES*/

template <class T, class ... Args> T rand_select(T, Args ...) noexcept;
template <class T> T rand_ratio_pick(T, T, float);

/*User-defined literal for percentage*/
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

//Default Constructor
tic::TicTacToe_AI::TicTacToe_AI() :
	m_myDiff(AI_Level::AI_MEDIUM)
{
	srand(static_cast<unsigned int>(time(NULL)));
}

//Constructor
tic::TicTacToe_AI::TicTacToe_AI(AI_Level difficulty) :
	m_myDiff(difficulty)
{
	srand(static_cast<unsigned int>(time(NULL)));
}

tic::TicTacToe_AI::~TicTacToe_AI()
{
	//nothing to do
}

t3g::cell_loc tic::TicTacToe_AI::get_move(const t3g::T3_Match & currMatch) const
{
	t3g::cell_loc edge_start = rand_select(1, 3, 5, 7);
	t3g::cell_loc corner_start = rand_select(0, 2, 6, 8);
	const t3g::cell_loc CENTER_START = 4U;
	t3g::cell_loc aiSel;
	
	if (currMatch.get_board_status() == t3g::T3_board_state::EMPTY_BOARD)
	{
		//Selects an arbitrary starting move based on difficulty
		switch (m_myDiff)
		{
		case tic::AI_Level::AI_EASY:
			//75% selects center, 20% selects any edge, 5% selects any corner
			aiSel = rand_ratio_pick(CENTER_START, rand_ratio_pick(edge_start, corner_start, 80.0_per), 75.0_per);

			break;
		case tic::AI_Level::AI_MEDIUM:
			//40% selects any edge, 30% selects any corner, 30% selects center
			aiSel = rand_ratio_pick(edge_start, rand_ratio_pick(corner_start, CENTER_START, 50.0_per), 40.0_per);

			break;
		case tic::AI_Level::AI_HARD:
			//80% selects any corner, 10% selects any edge, 10% selects center
			aiSel = rand_ratio_pick(corner_start, rand_ratio_pick(edge_start, CENTER_START, 50.0_per), 80.0_per);

			break;
		default:
			assert(false); //not a valid difficulty
			break;
		}
	}
	else
	{
		mmx::MinMaxBoard choice_map;
		choice_map.calc_board(currMatch);

		switch (m_myDiff)
		{
		case tic::AI_Level::AI_EASY:
			//do
			//{
			//	aiSel = rand() % t3g::BOARD_SIZE;
			//} while (currMatch.get_cell_state(aiSel) != t3g::T3_cell_state::NULL_STATE);
			//break;

			//choose any rank of any type of move
			aiSel = choice_map.rand_cell_query(mmx::Mmb_States::LOSE_MOVE | mmx::Mmb_States::TIE_MOVE | mmx::Mmb_States::WIN_MOVE, mmx::Rank_Range::ANY);

			break;
		case tic::AI_Level::AI_MEDIUM:
			//do
			//{
			//	aiSel = rand() % t3g::BOARD_SIZE;
			//} while (currMatch.get_cell_state(aiSel) != t3g::T3_cell_state::NULL_STATE);
			//break;

			//80% of the time choose among any winning or tying move at random, and 20% of the time choose among losing moves or tying moves at random
			//BUG BUG: out of bounds happen becuase sometimes the algorithm will try NOT to choose a losing move but that is the only move available.
			aiSel = rand_ratio_pick(choice_map.rand_cell_query(mmx::Mmb_States::TIE_MOVE | mmx::Mmb_States::WIN_MOVE, mmx::Rank_Range::ANY),
				choice_map.rand_cell_query(mmx::Mmb_States::LOSE_MOVE | mmx::Mmb_States::TIE_MOVE, mmx::Rank_Range::HIGHEST | mmx::Rank_Range::ANY_MIDDLE),
				80.0_per);

			break;
		case tic::AI_Level::AI_HARD:
			//choose the highest ranked winning move
			aiSel = choice_map.rand_cell_query(mmx::Mmb_States::WIN_MOVE, mmx::Rank_Range::LOWEST);
			//if there is none
			if (aiSel > t3g::BOARD_SIZE)
			{
				//choose the highest ranked tying move
				aiSel = choice_map.rand_cell_query(mmx::Mmb_States::TIE_MOVE, mmx::Rank_Range::HIGHEST);
			}

			break;
		default:
			assert(false); //not a valid difficulty
			break;
		}
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