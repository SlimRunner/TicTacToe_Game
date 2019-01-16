#ifndef TICTACTOE_AI_H
#define TICTACTOE_AI_H

//Forward declarations in the t3g namespace
namespace t3g
{
	class T3_Match; //forward declaration
	typedef unsigned int cell_loc; //re-declaration
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

		t3g::cell_loc get_move(const t3g::T3_Match & currMatch) const;

	private:
		AI_Level m_myDiff;
	};
}
#endif // !TICTACTOE_AI_H
