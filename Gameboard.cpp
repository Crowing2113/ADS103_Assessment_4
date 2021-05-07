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
Gameboard::Gameboard(SDL_Renderer* renderer, PlayerType p1Type, PlayerType p2Type)
{
	this->renderer = renderer;
	blank = IMG_LoadTexture(renderer, "assets/blank.png");
	cross = IMG_LoadTexture(renderer, "assets/cross.png");
	naught = IMG_LoadTexture(renderer, "assets/naught.png");
	crossTurn = IMG_LoadTexture(renderer, "assets/cross_turn.png");
	naughtTurn = IMG_LoadTexture(renderer, "assets/naught_turn.png");

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
	SDL_DestroyTexture(naughtTurn);
	SDL_DestroyTexture(crossTurn);
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

bool Gameboard::makeMove(Move move, Player p)
{
	bool moveWasMade;

	if (isValidMove(move)) {
		board[move.xCoord][move.yCoord] = p.type;
		moveWasMade = true;
	}
	else {
		cout << "[" << move.xCoord << "]" << "[" << move.yCoord << "]" << " is not a valid move" << endl;
		moveWasMade = false;
	}
	return moveWasMade;
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

bool Gameboard::checkWin(Player& p)
{
	//checking horizontal win
	for (int x = 0; x < 3; x++) {
		if (board[x][0] == p.type && board[x][1] == p.type && board[x][2] == p.type) {
			//cout << "Horizontal" << endl;
			return true;
		}
	}
	//checking vertical win
	for (int y = 0; y < 3; y++) {
		if (board[0][y] == p.type && board[1][y] == p.type && board[2][y] == p.type) {
			//cout << "Vertical" << endl;
			return true;
		}
	}

	//checking diagonal wins
	if (board[0][0] == p.type && board[1][1] == p.type && board[2][2] == p.type) {
		//cout << "Diagonal" << endl;
		return true;

	}
	if (board[0][2] == p.type && board[1][1] == p.type && board[2][0] == p.type) {
		//cout << "Diagonal" << endl;
		return true;
	}

	return false;
}

//player is maximiser
//opponent is minimiser
//these should work with AI vs AI
int Gameboard::evaluate(Player& p, Player& o)
{
	if (checkWin(p))
		return 10;
	if (checkWin(o))
		return -10;

	return 0;//no winners
}

int Gameboard::minimax(int depth, bool isMax, Player& p, Player& o)
{
	int score = evaluate(p, o);
	//return score if there wasn't a draw
	if (score == 10 || score == -10)
		return score;
	if (checkFullBoard())
		return 0;

	if (isMax) {
		int best = -1000;

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				//check if tile is empty
				if (board[x][y] == BLANK) {
					//make move
					board[x][y] = p.type;
					//call minimax
					best = max(best, minimax(depth++, !isMax, p, o));
					//Undo move
					board[x][y] = BLANK;
				}
			}
		}
		return best;
	}
	else//minimisers turn
	{
		int best = 1000;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (board[x][y] == BLANK) {
					board[x][y] = o.type;
					best = min(best, minimax(depth++, !isMax, p, o));
					board[x][y] = BLANK;
				}
			}
		}
		return best;
	}
}

int Gameboard::minimax(int depth, bool isMax, Player& p, Player& o, int alpha, int beta)
{
	return 0;
}

Move Gameboard::findBestMove(char type,Player& p,Player& o)
{
	bool isMaximiser = true;
	if (type == o.type)
		isMaximiser = false;
	int bestVal = -1000;
	if (!isMaximiser)
		bestVal = 1000;

	Move bestMove;
	bestMove.xCoord = -1;
	bestMove.yCoord = -1;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (board[x][y] == BLANK) {
				//make move
				board[x][y] = type;
				int moveVal = minimax(0, !isMaximiser, p, o);
				board[x][y] = BLANK;
				//MAXIMISER - is this move better than previously computed ones
				if (isMaximiser && moveVal > bestVal) {
					bestMove.xCoord = x;
					bestMove.yCoord = y;
					bestVal = moveVal;
				}
				//MINIMISER - is this move better than previously computed ones
				if (!isMaximiser && moveVal < bestVal) {
					bestMove.xCoord = x;
					bestMove.yCoord = y;
					bestVal = moveVal;
				}
			}
		}
	}
	return bestMove;
}

void Gameboard::draw(SDL_Texture* texture, SDL_Window* window)
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
			SDL_Rect turnRect;
			int sWidth = 0;
			int sHeight = 0;
			SDL_GetWindowSize(window, &sWidth, &sHeight);
			turnRect.x = (sWidth / 2) - (tileSize / 2);
			turnRect.y = sHeight - (sHeight / 4);
			turnRect.w = tileSize;
			turnRect.h = tileSize;

			if (board[x][y] == BLANK)
				SDL_RenderCopy(renderer, blank, NULL, &drawRect);
			if (board[x][y] == CROSS)
				SDL_RenderCopy(renderer, cross, NULL, &drawRect);
			if (board[x][y] == NAUGHT)
				SDL_RenderCopy(renderer, naught, NULL, &drawRect);

			SDL_RenderCopy(renderer, texture, NULL, &turnRect);

		}
	}
}
