#ifndef MINMAXBOARD_H
#define MINMAXBOARD_H

#include "T3_Board.h"

namespace t3g
{
	class T3_Match; //forward declaration
}

namespace mmx
{
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
	inline bool operator > (const MinMaxScore & lhs, const MinMaxScore & rhs) { return rhs < lhs; }
	inline bool operator <= (const MinMaxScore & lhs, const MinMaxScore & rhs) { return !(lhs > rhs); }
	inline bool operator >= (const MinMaxScore & lhs, const MinMaxScore & rhs) { return !(lhs < rhs); }

	class MinMaxBoard
	{
	public:
		MinMaxBoard();
		~MinMaxBoard();

		/*Class used as intermediary for subscript assignment*/
		class Proxy
		{
		public:
			Proxy(MinMaxBoard &ref, t3g::cell_loc index) :
				m_ref(ref), m_index(index) { }

			MinMaxScore & operator = (MinMaxScore rhs)
			{
				m_ref.m_arr[m_index] = rhs;
				return m_ref.m_arr[m_index];
			}

		private:
			MinMaxBoard & m_ref;
			t3g::cell_loc m_index;
		};

		/*Allows assignment to an array member*/
		Proxy operator [] (t3g::cell_loc index);

		/*Allows read only access to the value of the index*/
		MinMaxScore operator () (t3g::cell_loc index);

		/*Allows read only access to the value of the index*/
		MinMaxScore operator [] (t3g::cell_loc index) const;

		void get(t3g::T3_Match match);

	private:
		MinMaxScore m_arr[t3g::BOARD_SIZE]{}; //static array initialized to default value (Mbm_States::NEUTRAL)
	};
}
#endif // !MINMAXBOARD_H

