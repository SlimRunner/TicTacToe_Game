#include "TicTacToe_AI.h"
#include "T3_Match.h"

#include <cstdlib>
#include <ctime>

#include <iostream>

typedef size_t cell_loc;

/*SUGESTION: make an object that stores an Mmb_State and a score*/

enum class Mmb_States
{
	OCCUPIED = 0,
	NEUTRAL = 1,
	TIE_MOVE = 2,
	LOSE_MOVE = 3,
	WIN_MOVE = 4
};

struct MinMaxScore
{
	int score;
	Mmb_States state;

	MinMaxScore() :
		score(0), state(Mmb_States::NEUTRAL) { }
	MinMaxScore(int newScore, Mmb_States newState) :
		score(newScore), state(newState) { }
};

inline bool operator < (const MinMaxScore & lhs, const MinMaxScore & rhs)
{
	return lhs.score < rhs.score;
}
inline bool operator> (const MinMaxScore & lhs, const MinMaxScore & rhs) { return rhs < lhs; }
inline bool operator<=(const MinMaxScore & lhs, const MinMaxScore & rhs) { return !(lhs > rhs); }
inline bool operator>=(const MinMaxScore & lhs, const MinMaxScore & rhs) { return !(lhs < rhs); }

class MinMaxBoard
{
public:
	MinMaxBoard() { }
	~MinMaxBoard() { }

	/*Class used as intermediary for subscript assignment*/
	class Proxy
	{
	public:
		Proxy(MinMaxBoard &ref, cell_loc index) :
			m_ref(ref), m_index(index) { }

		MinMaxScore & operator = (MinMaxScore rhs)
		{
			m_ref.m_arr[m_index] = rhs;
			return m_ref.m_arr[m_index];
		}

	private:
		MinMaxBoard & m_ref;
		cell_loc m_index;
	};

	/*Allows assignment to an array member*/
	Proxy operator [] (cell_loc index)
	{
		return Proxy(*this, index);
	}

	/*Allows read only access to the value of the index*/
	MinMaxScore operator () (cell_loc index)
	{
		return m_arr[index];
	}

	/*Allows read only access to the value of the index*/
	MinMaxScore operator [] (cell_loc index) const
	{
		return m_arr[index];
	}

private:
	MinMaxScore m_arr[t3g::BOARD_SIZE] {}; //static array initialized to default value (Mbm_States::NEUTRAL)
};

/*PROTOTYPES*/

template <class T, class ... Args> T rand_select(T, Args ...) noexcept;
template <class T> T rand_ratio_pick(T, T, float);
MinMaxBoard get_choice_map(const t3g::T3_Match &);
MinMaxScore t3_minimax(t3g::T3_Match, cell_loc, int, bool);

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
	/*  ALL THE CODE INSIDE THIS DEFINITION IS TEMPORARY  */
	t3g::cell_loc badStart = rand_select(1, 3, 5, 7);
	t3g::cell_loc aiSel;
	
	if (false)//currMatch.get_board_status() == t3g::T3_board_state::EMPTY_BOARD)
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
		//temporary loop to choose random cell
		do
		{
			aiSel = rand() % t3g::BOARD_SIZE;
		} while (currMatch.get_cell_state(aiSel) != t3g::T3_cell_state::NULL_STATE);
		
		//int choice_map[t3g::BOARD_SIZE];
		MinMaxBoard choice_map;


		choice_map = get_choice_map(currMatch);
		/*cell value description
		occupied	= -2 (can't be chosen)
		bad choice	= -1 (leads to a loss)
		neutral		=  0 (may lead to a draw)
		win choice	=  1 (leads to a victory)
		*/

		for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
		{
			std::cout << (i%t3g::BOARD_SIDES ? "\t" : "\n") <<static_cast<int>(choice_map(i).state);
		}
		std::cout << std::endl;
		for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
		{
			std::cout << (i%t3g::BOARD_SIDES ? "\t" : "\n") << choice_map(i).score;
		}

		std::cout << std::endl;

		switch (m_myDiff)
		{
		case tic::AI_Level::AI_EASY:
			/*Select index which contains 1 rarely unless there is none, in that case select index with 0*/
			/*select often index with -1*/
			break;
		case tic::AI_Level::AI_MEDIUM:
			/*Select sometimes index which contains 1 unless there is none, in that case select index with 0*/
			/*select sometimes index with -1*/
			break;
		case tic::AI_Level::AI_HARD:
			/*Select always index which contains 1 unless there is none, in that case select index with 0*/
			break;
		default:
			break;
		}
	}

	return aiSel;
}

MinMaxBoard get_choice_map(const t3g::T3_Match & match)
{
	MinMaxBoard retVal;

	for (size_t i = 0; i < t3g::BOARD_SIZE; i++)
	{
		if (match.get_cell_state(i) == t3g::T3_cell_state::NULL_STATE)
		{
			retVal[i] = t3_minimax(match, i, 0, true);
		}
		else
		{
			retVal[i] = MinMaxScore(0, Mmb_States::OCCUPIED);
		}
	}

	return retVal;
}

inline MinMaxScore max_score(const MinMaxScore & bestVal, const MinMaxScore & newVal)
{
	if (bestVal > newVal)
		return bestVal;
	else
		return newVal;
}

inline MinMaxScore min_score(const MinMaxScore & bestVal, const MinMaxScore & newVal)
{
	if (bestVal < newVal)
		return bestVal;
	else
		return newVal;
}

MinMaxScore t3_minimax(t3g::T3_Match match, cell_loc seed_loc, int depth, bool isMax)
{
	const int SCORE_BOUND = 1000;
	const int MAX_SCORE = 10;

	//makes move and returns the state of the game
	switch (match.make_move(seed_loc))
	{
	case t3g::T3_board_state::TIED_BOARD:
		return MinMaxScore(0, Mmb_States::TIE_MOVE);
		break;
	case t3g::T3_board_state::X_WIN_BOARD:
	case t3g::T3_board_state::O_WIN_BOARD:
		if (isMax)
		{
			return MinMaxScore(MAX_SCORE - depth, Mmb_States::WIN_MOVE);
		}
		else
		{
			return MinMaxScore(-MAX_SCORE + depth, Mmb_States::LOSE_MOVE);
		}
		break;
	default:
		break;
	}

	MinMaxScore best_score, rec_score;
	best_score = MinMaxScore(SCORE_BOUND * (isMax ? -1 : 1), Mmb_States::TIE_MOVE);

	for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
	{
		if (match.get_cell_state(i) == t3g::T3_cell_state::NULL_STATE)
		{
			rec_score = t3_minimax(match, i, depth + 1, !isMax);

			if (isMax) //maximizer
			{
				//if (rec_score > best_score) best_score = rec_score;
				best_score = max_score(best_score, rec_score);
				//if (best_score.score - depth - 1 == -MAX_SCORE && best_score.score != 0) return best_score;
			}
			else //minimizer
			{
				//if (rec_score < best_score) best_score = rec_score;
				best_score = min_score(best_score, rec_score);
				//if (best_score.score + depth + 1 == MAX_SCORE && best_score.score != 0) return best_score;
			}
		}
	}

	//if (best_score.state != Mmb_States::TIE_MOVE)
	//	best_score.state = Mmb_States::NEUTRAL;
	return best_score;
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