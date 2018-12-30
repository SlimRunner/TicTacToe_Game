# TicTacToe_Game
A series of classes that manage a match of Tic Tac Toe. The implementation of the game is completely separated from it's current command-line interface.

## Instructions - Visual Studio
1. Download the folder at the root of the repository named 'TicTacToe'
1. Open the sln file
1. Compile to make sure it works.

## Description of Classes
* **T3_Board:** This class takes care of correctly assigning symbols (`X`, `O`, or `empty`) to the corresponding cells. It also has bound checks. It does not enforce rules of any kind (e.g. you can override an `X` with an `O`)
* **T3_Match:** This class specializes (inherits) T3_Board. This class enforces correct assignment of symbols, disallows overriding, and automatically takes care of symbol selection (alternates symbol after every assignemnt).
* **TicTacToe_cli:** This is a command-line interface that displays the match current status and also prompts the user for input. It organizes input and output with menus.
* **TicTacToe_AI:** Takes care of determining automatically an available spot to insert a symbol in a T3_Match object. Implementation of smart decision is pending, currently it chooses spot at random.
