#ifndef TICTACTOE_AI_H
#define TICTACTOE_AI_H

namespace t3g
{
	class T3_Match;
	typedef unsigned int cell_loc;
}

namespace tic
{
	enum class AI_Level;

	class TicTacToe_AI
	{
	public:
		TicTacToe_AI();
		TicTacToe_AI(AI_Level difficulty);
		~TicTacToe_AI();

		/*Does this work?*/
		t3g::cell_loc get_move(t3g::T3_Match & currMatch);

	private:
		AI_Level m_myDiff;
	};
}
#endif // !TICTACTOE_AI_H
