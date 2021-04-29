#include "Gameboard.h"

char Gameboard::BLANK = ' ';
char Gameboard::CROSS = 'X';
char Gameboard::NAUGHT = 'O';

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="renderer">- reference to the renderer</param>
	/// <param name="p1Type">- PlayerType of player 1</param>
	/// <param name="p2Type">- PlayerType of player 2</param>
Gameboard::Gameboard(SDL_Renderer* renderer,PlayerType p1Type,PlayerType p2Type)
{
	this->renderer = renderer;
	blank = IMG_LoadTexture(renderer,"assets/blank.png");
	cross = IMG_LoadTexture(renderer, "assets/cross.png");
	naught = IMG_LoadTexture(renderer, "assets/naught.png");

	p1.playerType = p1Type;
	p2.playerType = p2Type;
	p1.type = CROSS;
	p2.type = NAUGHT;
	clearBoard();
}

Gameboard::~Gameboard()
{
	SDL_DestroyTexture(blank);
	SDL_DestroyTexture(cross);
	SDL_DestroyTexture(naught);
}

void Gameboard::clearBoard()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			board[x][y] = BLANK;
}

void Gameboard::makeAllNaught() {
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			board[x][y] = NAUGHT;
}

void Gameboard::makeAllCross()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			board[x][y] = CROSS;
}

void Gameboard::makeMove(Move move,Player p)
{
	if (isValidMove(move)) {
		board[move.xCoord][move.yCoord] = p.type;
	}
	else {
		cout << "[" << move.xCoord << "]" << "[" << move.yCoord << "]" << " is not a valid move" << endl;
	}
}

bool Gameboard::isValidMove(Move move)
{
	if (board[move.xCoord][move.yCoord] != BLANK) {
		return false;
	}
	return true;
}

bool Gameboard::checkFullBoard()
{
	for (int x = 0; x < 3; x++) 
		for (int y = 0; y < 3; y++) 
			if (board[x][y] == BLANK) 
				return false;
	
	return true;
}

bool Gameboard::checkWin(Player p)
{
	//checking horizontal win
	for (int x = 0; x < 3; x++) {
		if (board[x][0] == p.type && board[x][1] == p.type && board[x][2] == p.type) {
			return true;
		}

		//checking vertical win
		for (int y = 0; y < 3; y++)
			if (board[0][y] == p.type && board[1][y] == p.type && board[2][y] == p.type) {
				return true;
			}
				

		//checking diagonal wins
		if (board[0][0] == p.type && board[1][1] == p.type && board[2][2] == p.type) 
			return true;
		if (board[0][2] == p.type && board[1][1] == p.type && board[0][2] == p.type)
			return true;
		
		return false;
	}
}

void Gameboard::draw()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			SDL_Rect drawRect;
			drawRect.x = boardX + (x * tileSize);
			drawRect.y = boardY + (y * tileSize);
			drawRect.w = tileSize;
			drawRect.h = tileSize;

			if (board[x][y] == BLANK)
				SDL_RenderCopy(renderer, blank, NULL, &drawRect);
			if (board[x][y] == CROSS)
				SDL_RenderCopy(renderer, cross, NULL, &drawRect);
			if (board[x][y] == NAUGHT)
				SDL_RenderCopy(renderer, naught, NULL, &drawRect);
		}
	}
}
