#include <iostream>
#include <string>
#include <SDL.h>
#include "Gameboard.h"

using namespace std;


enum MenuChoices
{
	PLAYER_VS_PLAYER = 1,
	PLAYER_VS_COMPUTER,
	COMPUTER_VS_COMPUTER,
	HOW_TO_PLAY,
	QUIT
};

void displayHowToPlay() {
	system("cls");
	cout << "================================How to Play================================" << endl << endl;
	cout << "\tTaking turns placing either a Naught or Cross on a tile" << endl;
	cout << "\tYou choose your move by entering a number between 1 and 9 inclusive" << endl;
	cout << "\tPositions are displayed below" << endl << endl;;
	cout << "\t\t\tBoard Layout:" << endl << endl;
	cout << "\t\t\t   |   |   " << endl;
	cout << "\t\t\t 7 | 8 | 9 " << endl;
	cout << "\t\t\t---|---|---" << endl;
	cout << "\t\t\t 4 | 5 | 6 " << endl;
	cout << "\t\t\t---|---|---" << endl;
	cout << "\t\t\t 1 | 2 | 3 " << endl;
	cout << "\t\t\t   |   |   " << endl;
	system("pause");
	system("cls");
}

int main(int argc, char** arbv) {


	bool menu = true;
	PlayerType p1Type, p2Type;

	//menu loop
	while (menu) {
		int choice;
		//displays the different game modes
		cout << "============Welcome to Tic Tac Toe============" << endl;
		cout << "\tPlease choose an option:" << endl << endl;
		cout << "\t 1 - Player vs Player" << endl;
		cout << "\t 2 - Player vs Computer" << endl;
		cout << "\t 3 - Computer vs Computer" << endl;
		cout << "\t 4 - How to play" << endl;
		cout << "\t 5 - Quit" << endl << endl;
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
				menu = false;
				break;
			case PLAYER_VS_COMPUTER: //if the player chose option 2 - player vs computer
				p1Type = PlayerType::HUMAN;
				p2Type = PlayerType::COMPUTER;
				cout << "You have chosen Player vs Computer" << endl;
				menu = false;
				break;
			case COMPUTER_VS_COMPUTER://if the player chose option 3 - computer vs computer
				p1Type = PlayerType::COMPUTER;
				p2Type = PlayerType::COMPUTER;
				cout << "You have chosen Computer vs Computer" << endl;
				menu = false;
				break;
			case HOW_TO_PLAY://if the player chose option 4 - how to play
				displayHowToPlay();
				break;
			case QUIT://if the player chose option 5 - Quit
				cout << "Quitting..." << endl;
				system("pause");
				return 0;
				break;
			}
		} else {
			cout << "Please choose a valid number...try again" << endl;
			system("pause");
			system("cls");
		}
	}
	//check if SDL initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Error: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}
	//create window
	SDL_Window* window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	//if it couldn't create the window for some reason then quit
	if (window == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		system("pause");
		return 1;
	}
	//create the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if there was en error creating the renderer then quit
	if (renderer == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		system("pause");
		return 1;
	}

	Gameboard gameBoard(renderer, p1Type, p2Type);
	bool quit = false;

	//the current player
	bool pOneTurn = true;
	//game loop
	while (!quit) {
		//gameBoard.makeAllCross();
		if (pOneTurn) {
			if (gameBoard.checkWin(gameBoard.p1)) {
				cout << "Player 1 wins" << endl;
				quit = true;
				break;
			}
			else if (gameBoard.checkFullBoard()) {
				cout << "No one has won. It's a draw :(" << endl;
				quit = true;
				break;
			}
			int tile;
			//make move
			cout << "Where will you place >>";
			cin >> tile;
			//if the entered tile is out of bounds then return an error
			Move tileMove;
			//gameBoard.makeMove(tileMove,p);
		}

		//set the background color of the window
		SDL_SetRenderDrawColor(renderer, 100, 170, 170, 255);
		SDL_RenderClear(renderer);

		gameBoard.draw();

		//swap buffers
		SDL_RenderPresent(renderer);
	}
	
	
	system("pause");


	return 0;
}