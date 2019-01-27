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
		OCCUPIED = 1,
		NEUTRAL = 2,
		TIE_MOVE = 4,
		LOSE_MOVE = 8,
		WIN_MOVE = 16
	};
	inline Mmb_States operator | (Mmb_States lhs, const Mmb_States & rhs)
		{ return static_cast<Mmb_States>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
	inline Mmb_States operator & (Mmb_States lhs, const Mmb_States & rhs)
		{ return static_cast<Mmb_States>(static_cast<int>(lhs) & static_cast<int>(rhs)); }

	enum class Rank_Range
	{
		LOWEST = 1,
		ANY_MIDDLE = 2, //any number that neither highest or lowest
		HIGHEST = 4,
		ANY = LOWEST | ANY_MIDDLE | HIGHEST
	};
	inline Rank_Range operator | (Rank_Range lhs, const Rank_Range & rhs)
		{ return static_cast<Rank_Range>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
	inline Rank_Range operator & (Rank_Range lhs, const Rank_Range & rhs)
		{ return static_cast<Rank_Range>(static_cast<int>(lhs) & static_cast<int>(rhs)); }

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

		void calc_board(t3g::T3_Match match);
		t3g::cell_loc rand_cell_query(Mmb_States state, Rank_Range rank);

	private:
		//SUGGESTION: use vector instead since it's already being included in the .cpp
		MinMaxScore m_arr[t3g::BOARD_SIZE]{}; //static array initialized to default value (Mbm_States::NEUTRAL)
	};
}
#endif // !MINMAXBOARD_H
