#include "MinMaxBoard.h"
#include "T3_Match.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>
#include <random>

mmx::MinMaxBoard get_choice_map(const t3g::T3_Match &);
mmx::MinMaxScore t3_minimax(t3g::T3_Match, t3g::cell_loc, int, bool);

mmx::MinMaxBoard::MinMaxBoard()
{
	//Nothing To Do
}

mmx::MinMaxBoard::~MinMaxBoard()
{
	//Nothing To Do
}

mmx::MinMaxBoard::Proxy mmx::MinMaxBoard::operator [] (t3g::cell_loc index)
{
	return Proxy(*this, index);
}

mmx::MinMaxScore mmx::MinMaxBoard::operator () (t3g::cell_loc index)
{
	return m_arr[index];
}

mmx::MinMaxScore mmx::MinMaxBoard::operator [] (t3g::cell_loc index) const
{
	return m_arr[index];
}

void mmx::MinMaxBoard::calc_board(const t3g::T3_Match & match)
{
	//calculates the minimax for the passed match and assigns the result to itself
	*this = get_choice_map(match);
}

t3g::cell_loc mmx::MinMaxBoard::rand_cell_query(Mmb_States state, Rank_Range rank)
{
	typedef std::vector<t3g::cell_loc> cell_vector;
	typedef std::vector<Mmb_States> states_vector;
	typedef std::vector<MinMaxScore> mmscore_vector;

	//vector of enum Mmb_States
	states_vector cell_states{ Mmb_States::OCCUPIED, Mmb_States::NEUTRAL, Mmb_States::TIE_MOVE, Mmb_States::LOSE_MOVE, Mmb_States::WIN_MOVE };
	//vector of cell_loc (size_t)
	cell_vector cell_collector;
	//parallel data vector of cell_collector
	mmscore_vector score_subset;

	//go through all possible states of Mmb_States...
	for (states_vector::iterator it = cell_states.begin(); it != cell_states.end(); ++it)
	{
		//...if state contains current Mmb_State flag (it)...
		if (static_cast<bool>(state & *it))
		{
			//...iterate through all cells of board...
			for (t3g::cell_loc i = 0; i < t3g::BOARD_SIZE; ++i)
			{
				//...and if current cell (i) of board contains current flag (it)...
				if (m_arr[i].state == *it)
				{
					//...push this index (i) into the cell_collector vector
					cell_collector.push_back(i);
					score_subset.push_back(m_arr[i]);
					//rank_sorted
				}
			}
		}
	}

	//if the criteria above returns no results exit with invalid value
	if (cell_collector.empty())
		return std::numeric_limits<t3g::cell_loc>::max();

	//sorts subset of data to find the highest and lowest scores
	std::sort(score_subset.begin(), score_subset.end(), [](const MinMaxScore & lhs, const MinMaxScore & rhs)
	{
		return lhs.score < rhs.score;
	});

	int max = score_subset.back().score;
	int min = score_subset.front().score;

	cell_vector cell_range;

	//pushes indexes of values which fulfill the criterion into cell_range
	for (cell_vector::iterator it = cell_collector.begin(); it != cell_collector.end(); ++it)
	{
		if (static_cast<bool>(rank & Rank_Range::BEST_RANK) &&
			m_arr[*it].score == min && min != max)
		{
			cell_range.push_back(*it);
		}

		if (static_cast<bool>(rank & Rank_Range::WORST_RANK) &&
			m_arr[*it].score == max && min != max)
		{
			cell_range.push_back(*it);
		}

		if (static_cast<bool>(rank & Rank_Range::MID_RANK) &&
			m_arr[*it].score != min && m_arr[*it].score != max && min != max)
		{
			cell_range.push_back(*it);
		}

		if (static_cast<bool>(rank & (Rank_Range::ANY_RANK)) &&
			max == min)
		{
			cell_range.push_back(*it);
		}
	}


	//makes sure cell_range has data, otherwise return an error value
	if (!cell_range.empty())
	{
		std::default_random_engine def_gene(static_cast<unsigned int>(time(NULL)));
		std::uniform_int_distribution<size_t> uni_dist(0U, cell_range.size()-1);

		return cell_range[uni_dist(def_gene)];
	}
	else
	{
		return std::numeric_limits<t3g::cell_loc>::max();
	}
}

//Entrance function of the minimax algorithm
mmx::MinMaxBoard get_choice_map(const t3g::T3_Match & match)
{
	mmx::MinMaxBoard retVal;

	//goes through all cells of match and ranks them based on minimax
	for (size_t i = 0; i < t3g::BOARD_SIZE; i++)
	{
		//asserts that cell is not occupied
		if (match.get_cell_state(i) == t3g::T3_cell_state::NULL_STATE)
		{
			retVal[i] = t3_minimax(match, i, 0, true);
		}
		else
		{
			retVal[i] = mmx::MinMaxScore(0, mmx::Mmb_States::OCCUPIED);
		}
	}

	return retVal;
}

//finds and returns the highest of two values
inline mmx::MinMaxScore max_score(const mmx::MinMaxScore & bestVal, const mmx::MinMaxScore & newVal)
{
	if (bestVal > newVal)
		return bestVal;
	else
		return newVal;
}

//finds and returns the lowest of two values
inline mmx::MinMaxScore min_score(const mmx::MinMaxScore & bestVal, const mmx::MinMaxScore & newVal)
{
	if (bestVal < newVal)
		return bestVal;
	else
		return newVal;
}

//Recursive implemetation of minimax algorithm
mmx::MinMaxScore t3_minimax(t3g::T3_Match match, t3g::cell_loc seed_loc, int depth, bool isMax)
{
	const int SCORE_BOUND = 1000;
	const int MAX_SCORE = 10;

	//makes move and returns the state of the game
	switch (match.make_move(seed_loc))
	{
	case t3g::T3_board_state::TIED_BOARD:
		return mmx::MinMaxScore(0, mmx::Mmb_States::TIE_MOVE);
		break;
	case t3g::T3_board_state::X_WIN_BOARD:
	case t3g::T3_board_state::O_WIN_BOARD:
		if (!isMax) return mmx::MinMaxScore(MAX_SCORE - depth, mmx::Mmb_States::LOSE_MOVE);
		else return mmx::MinMaxScore(-MAX_SCORE + depth, mmx::Mmb_States::WIN_MOVE);
		break;
	default:
		break;
	}

	mmx::MinMaxScore best_score; //Stores the best value found for current tree
	mmx::MinMaxScore rec_score; //Intermediary variable that receives the return of the recursion
	//generates the Minimax first assumption (worse case scenario)
	best_score = mmx::MinMaxScore(SCORE_BOUND * (isMax ? -1 : 1), mmx::Mmb_States::TIE_MOVE);

	for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
	{
		//asserts that cell is not occupied
		if (match.get_cell_state(i) == t3g::T3_cell_state::NULL_STATE)
		{
			//recursion
			rec_score = t3_minimax(match, i, depth + 1, !isMax);

			if (isMax) //maximizer
			{
				best_score = max_score(best_score, rec_score);
			}
			else //minimizer
			{
				best_score = min_score(best_score, rec_score);
			}
		}
	}

	return best_score;
}