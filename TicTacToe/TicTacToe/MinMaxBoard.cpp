#include "MinMaxBoard.h"
#include "T3_Match.h"

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

void mmx::MinMaxBoard::get(t3g::T3_Match match)
{
	*this = get_choice_map(match);
}

mmx::MinMaxBoard get_choice_map(const t3g::T3_Match & match)
{
	mmx::MinMaxBoard retVal;

	for (size_t i = 0; i < t3g::BOARD_SIZE; i++)
	{
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

inline mmx::MinMaxScore max_score(const mmx::MinMaxScore & bestVal, const mmx::MinMaxScore & newVal)
{
	if (bestVal > newVal)
		return bestVal;
	else
		return newVal;
}

inline mmx::MinMaxScore min_score(const mmx::MinMaxScore & bestVal, const mmx::MinMaxScore & newVal)
{
	if (bestVal < newVal)
		return bestVal;
	else
		return newVal;
}

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

	mmx::MinMaxScore best_score, rec_score;
	best_score = mmx::MinMaxScore(SCORE_BOUND * (isMax ? -1 : 1), mmx::Mmb_States::TIE_MOVE);

	for (size_t i = 0; i < t3g::BOARD_SIZE; ++i)
	{
		if (match.get_cell_state(i) == t3g::T3_cell_state::NULL_STATE)
		{
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