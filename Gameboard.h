#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <algorithm>
#include "Textbox.h"

using namespace std;

//struct for move
struct Move {
	int xCoord;
	int yCoord;
};

//whether the player is human or computer
enum class PlayerType
{
	HUMAN,
	COMPUTER
};

//a player struct that stores information about the player
struct Player {
	PlayerType playerType;
	char type;
	string name;

};

class Gameboard
{
public:
	static char BLANK;
	static char CROSS;
	static char NAUGHT;

	SDL_Renderer* renderer;
	SDL_Texture* blank, * cross, * naught, * crossTurn, * naughtTurn;

	Player p1, p2;

	Textbox p1disp, p2disp;
	char board[3][3];//3 x 3 array representing board
	int boardX = 250, boardY = 150;//X and Y offset of where there gameboard is
	int tileSize = 100;


	Gameboard(SDL_Renderer* renderer, PlayerType p1Type, PlayerType p2Type);
	~Gameboard();

	void clearBoard();
	void makeAllNaught();
	void makeAllCross();
	bool makeMove(Move move, Player p);
	bool isValidMove(Move move);
	bool checkFullBoard();
	bool checkWin(Player& p);

	void draw(SDL_Texture* texture, SDL_Window* window);

	//MINIMAX Algorithm
	int evaluate(Player& p, Player& o);
	int minimax(int depth, bool isMax, Player& p, Player& o);
	int minimax(int depth, bool isMax, Player& p, Player& o, int alpha, int beta);
	Move findBestMove(char type,Player& p, Player& o);
};

