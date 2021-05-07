#include <iostream>
#include <fstream>;
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include "Gameboard.h"
#include "Textbox.h"
using namespace std;

static int compWins;
static Player* oPlayer = nullptr;
static Player* cPlayer = nullptr;
PlayerType p1Type, p2Type;

enum MenuChoices
{
	PLAYER_VS_PLAYER = 1,
	PLAYER_VS_COMPUTER,
	COMPUTER_VS_COMPUTER,
	HOW_TO_PLAY,
	SCORES,
	QUIT
};

//DO THIS STILL
void displayHowToPlay() {
	/*TESTING SDL TEXT*/
	//create window
	SDL_Window* window = SDL_CreateWindow("How to Play", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	//if it couldn't create the window for some reason then quit
	if (window == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		system("pause");
		return;
	}
	//create the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if there was en error creating the renderer then quit
	if (renderer == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		system("pause");
		return;
	}
	bool display = true;

	Textbox textbox;
	textbox.setup(renderer);
	
	textbox.SetText("================================How to Play================================\n\n\tTaking turns placing either a Naught or Cross on a tile\n\tYou choose your move by entering a number between 1 and 9 inclusive\n\tPositions are displayed below\n\tBelow the board is the turn indicator\n\t\t\tBoard Layout:\n\n\t\t\t   |   |   \n\t\t\t 7 | 8 | 9 \n\t\t\t---|---|---\n\t\t\t 4 | 5 | 6 \n\t\t\t---|---|---\n\t\t\t 1 | 2 | 3 \n\t\t\t   |   |   \n\t\t\t     X\n", window);
	//textbox.SetText("yes", window);
	textbox.isVisible = true;
	
	textbox.draw();

	while (display) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			//int curr = SDL_PollEvent(&e);
			//cout << "poll event" << curr << endl;
			//if the type of event was a key press then check what key was pressed
			if (e.type == SDL_KEYDOWN) {
				//if enter key was pressed then clear the board
				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
					SDL_DestroyWindow(window);
					system("cls");
					display = false;
				}
			}
		}
		SDL_RenderPresent(renderer);
		/*ENDING SDL TEXT*/



	/*
		system("cls");
		cout << "================================How to Play================================" << endl << endl;
		cout << "\tTaking turns placing either a Naught or Cross on a tile" << endl;
		cout << "\tYou choose your move by entering a number between 1 and 9 inclusive" << endl;
		cout << "\tPositions are displayed below" << endl;
		cout << "\tBelow the board is the turn indicator" << endl;
		cout << "\t\t\tBoard Layout:" << endl << endl;
		cout << "\t\t\t   |   |   " << endl;
		cout << "\t\t\t 7 | 8 | 9 " << endl;
		cout << "\t\t\t---|---|---" << endl;
		cout << "\t\t\t 4 | 5 | 6 " << endl;
		cout << "\t\t\t---|---|---" << endl;
		cout << "\t\t\t 1 | 2 | 3 " << endl;
		cout << "\t\t\t   |   |   " << endl << endl;
		cout << "\t\t\t     X" << endl;
		//system("pause");
		//system("cls");
		*/
	}
}

void gameLoop() {

	//create window
	SDL_Window* window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	//if it couldn't create the window for some reason then quit
	if (window == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		system("pause");
		return;
	}
	//create the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if there was en error creating the renderer then quit
	if (renderer == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		system("pause");
		return;
	}

	Gameboard gameBoard(renderer, p1Type, p2Type);
	bool gameQuit = false;
	gameBoard.p1.name = "Joe";
	gameBoard.p2.name = "Fred";

	//bool pOneTurn = true;
	bool moved = false;
	cPlayer = &gameBoard.p1;//the current player
	oPlayer = &gameBoard.p2;//the opponent player
	//game loop
	while (!gameQuit) {
		if (oPlayer == nullptr || cPlayer == nullptr) {
			cout << "oPlayer or cPlayer is not set" << endl;
			return;
		}
		int tile;
		Move tileMove = Move();

		SDL_Event e;
		//if the current playerType is to COMPUTER then ai will move
		if (cPlayer->playerType == PlayerType::COMPUTER) {
			Move aiMove = gameBoard.findBestMove(cPlayer->type, *cPlayer, *oPlayer);
			if (aiMove.xCoord != -1 && aiMove.yCoord != -1) {
				moved = gameBoard.makeMove(aiMove, *cPlayer);
			}

		}
		//else the playerTytpe is set to HUMAN so it is the human turn
		else {
			//go through the queue of events until there are none left
			while (SDL_PollEvent(&e) != 0) {
				//int curr = SDL_PollEvent(&e);
				//cout << "poll event" << curr << endl;
				//if the type of event was a key press then check what key was pressed
				if (e.type == SDL_KEYDOWN) {
					//if enter key was pressed then clear the board
					if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
						gameBoard.clearBoard();
						break;
					}
					//if escape was pressed then quit
					if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						SDL_DestroyWindow(window);
						cout << "Quitting....";
						system("pause");
						return;
					}
					//switch statement to set the tileMove variable to a specific Move coord
					switch (e.key.keysym.scancode)
					{
					case SDL_SCANCODE_KP_1:
						//set move to [0,2]
						tileMove.xCoord = 0;
						tileMove.yCoord = 2;
						break;
					case SDL_SCANCODE_KP_2:
						//set move to [1,2]
						tileMove.xCoord = 1;
						tileMove.yCoord = 2;
						break;
					case SDL_SCANCODE_KP_3:
						//set move to [2,2]
						tileMove.xCoord = 2;
						tileMove.yCoord = 2;
						break;
					case SDL_SCANCODE_KP_4:
						//set move to [0,1]
						tileMove.xCoord = 0;
						tileMove.yCoord = 1;
						break;
					case SDL_SCANCODE_KP_5:
						//set move to [1,1]
						tileMove.xCoord = 1;
						tileMove.yCoord = 1;
						break;
					case SDL_SCANCODE_KP_6:
						//set move to [2,1]
						tileMove.xCoord = 2;
						tileMove.yCoord = 1;
						break;
					case SDL_SCANCODE_KP_7:
						//set move to [0,0]
						tileMove.xCoord = 0;
						tileMove.yCoord = 0;
						break;
					case SDL_SCANCODE_KP_8:
						//set move to [1,0]
						tileMove.xCoord = 1;
						tileMove.yCoord = 0;
						break;
					case SDL_SCANCODE_KP_9:
						//set move to [2,0]
						tileMove.xCoord = 2;
						tileMove.yCoord = 0;
						break;
					default:
						tileMove.xCoord = -1;
						tileMove.yCoord = -1;
						break;
					}
					moved = gameBoard.makeMove(tileMove, *cPlayer);
					

				}
			}
		}
		//set the background color of the window
		SDL_SetRenderDrawColor(renderer, 100, 170, 170, 255);
		SDL_RenderClear(renderer);
		SDL_Texture* tempTex;
		if (cPlayer->type == gameBoard.CROSS) {
			tempTex = gameBoard.crossTurn;
		}
		else {
			tempTex = gameBoard.naughtTurn;
		}

		gameBoard.draw(tempTex, window);

		//swap buffers
		SDL_RenderPresent(renderer);
		Sleep(200);

		if (moved) {
			if (gameBoard.checkWin(*cPlayer)) {
				if (cPlayer->playerType == PlayerType::COMPUTER)
					compWins++;
				cout << "Player " << cPlayer->name << " wins" << endl;
				gameQuit = true;
				break;
			}
			else if (gameBoard.checkFullBoard()) {
				cout << "No one has won. It's a draw :(" << endl;
				gameQuit = true;
				break;
			}
			if (cPlayer->type == gameBoard.p1.type) {
				cPlayer = &gameBoard.p2;
				oPlayer = &gameBoard.p1;
			}
			else {
				cPlayer = &gameBoard.p1;
				oPlayer = &gameBoard.p2;
			}
			moved = false;
		}


	}
	ofstream oFile;
	oFile.open("comScore.txt");
	oFile << compWins;
	oFile.close();
	SDL_DestroyWindow(window);
	system("pause");
	system("cls");

}

int main(int argc, char** arbv) {

	//TODO: save to file comp wins v player and comp wins v comp
	//		keep all players in a list read from file
	//			when reading from file read first to string then int
	//		save player in a file that reads as "NAME  NUMBER" 
	//								stats such as name and wins
	//		ask player for name before game start
	//		if that player already exists then let them know else create a new player

	ifstream inFile;
	inFile.open("comScore.txt");
	inFile >> compWins;
	//check if SDL initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}
	if (TTF_Init() != 0) {
		cout << "SDL TTF Init Error: " << TTF_GetError() << endl;
		system("pause");
		return 1;
	}
	bool quit = false;
	//menu loop
	while (!quit) {
		int choice;
		bool play = false;

		//displays the different game modes
		cout << "============Welcome to Tic Tac Toe============" << endl;
		cout << "\tPlease choose an option:" << endl << endl;
		cout << "\t 1 - Player vs Player" << endl;
		cout << "\t 2 - Player vs Computer" << endl;
		cout << "\t 3 - Computer vs Computer" << endl;
		cout << "\t 4 - How to play" << endl;
		cout << "\t 5 - Scores" << endl;
		cout << "\t 6 - Quit" << endl << endl;
		cout << "\t\tChoice > ";
		cin >> choice;
		//checking if what was input was a number
		//if it wasn't then show an error
		if (cin.fail()) {
			cout << "Not a valid choice" << endl;
			//clears the error flag so that we can get more input
			cin.clear();
			cin.ignore(256, '\n');
			system("pause");
			system("cls");
		}
		else if (choice <= QUIT && choice >= PLAYER_VS_PLAYER) {//if it was a number then check if it was valid number

			switch (choice)
			{
			case PLAYER_VS_PLAYER: //if the player chose option 1 - player vs player
				p1Type = PlayerType::HUMAN;
				p2Type = PlayerType::HUMAN;
				cout << "You have chosen Player vs Player" << endl;
				play = true;
				break;
			case PLAYER_VS_COMPUTER: //if the player chose option 2 - player vs computer
				p1Type = PlayerType::HUMAN;
				p2Type = PlayerType::COMPUTER;
				cout << "You have chosen Player vs Computer" << endl;
				play = true;
				break;
			case COMPUTER_VS_COMPUTER://if the player chose option 3 - computer vs computer
				p1Type = PlayerType::COMPUTER;
				p2Type = PlayerType::COMPUTER;
				cout << "You have chosen Computer vs Computer" << endl;
				play = true;
				break;
			case HOW_TO_PLAY://if the player chose option 4 - how to play
				displayHowToPlay();
				break;
			case SCORES:
				//displayScores();
				system("cls");
				break;
			case QUIT://if the player chose option 5 - Quit
				cout << "Quitting..." << endl;
				quit = true;
				break;
			}
		} else {
			cout << "Please choose a valid number...try again" << endl;
			system("pause");
			system("cls");
		}
		if (play) {
			gameLoop();
			play = false;
		}
	}

	system("pause");
	return 0;
}