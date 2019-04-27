#ifndef TICTACTOE_AI_H
#define TICTACTOE_AI_H

//Forward declarations in the t3g namespace
namespace t3g
{
	class T3_Match; //forward declaration
	typedef size_t cell_loc; //re-declaration
}

namespace tic
{
	enum class AI_Level
	{
		AI_EASY = 0,
		AI_MEDIUM = 1,
		AI_HARD = 2
	};

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
