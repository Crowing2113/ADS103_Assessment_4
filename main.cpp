#include <iostream>
#include <fstream>;
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include <vector>
#include <chrono>
#include "Gameboard.h"
#include "Textbox.h"
using namespace std;
using namespace std::chrono;

static int compVsPlayer, compVsComp;
static Player* oPlayer = nullptr;
static Player* cPlayer = nullptr;
PlayerType p1Type, p2Type;
SDL_Texture* zero, * one, * two, * three, * four, * five, * six, * seven, * eight, * nine,* ten;
SDL_Texture* boardLayoutTex;

enum MenuChoices
{
	PLAYER_VS_PLAYER = 1,
	PLAYER_VS_COMPUTER,
	COMPUTER_VS_COMPUTER,
	HOW_TO_PLAY,
	SCORES,
	QUIT
};

//Setting the timer texture in a switch function
SDL_Texture* SetTimerTexture(int timer) {
	switch (timer) {
	case 0:
		return zero;
	case 1:
		return one;
	case 2:
		return two;
	case 3:
		return three;
	case 4:
		return  four;
	case 5:
		return five;
	case 6:
		return six;
	case 7:
		return seven;
	case 8:
		return eight;
	case 9:
		return nine;
	case 10:
		return ten;
	}
}
//Display a list of player names
// ask the players to "log in" as one of them or create a new one
void displayPlayerList(string *pName) {
	system("cls");//clear the screen before showing this
	ifstream playersFI("playerList.txt"); // gettign the file that contains the player list
	vector<string> playerList; //a string vector to hold the players in
	if (playersFI.is_open()) {//check if the playerList.txt exists
		while (!playersFI.eof()) { //go through the file until you reach the end
			//store the name in a temp string variable and then add it to the vector
			string temp;
			playersFI >> temp;
			if(temp != "")
				playerList.push_back(temp);
		}
		//should sort the vector in alphabetical order
		sort(playerList.begin(), playerList.end());
		for (string p : playerList) {//go through and display them all
			cout << p << endl;
		}
		cout << "Pick a name or create a new one." << endl;
	}
	else {//if the file doesn't exist then ask player to create a new "Profile"
		cout << "Create a new profile. Enter your name" << endl;
	}
	playersFI.close();
	string name;
	cout << ">";
	cin >> name;
	bool nameFound = false;
	for (int i = 0; i < playerList.size(); i++) {
		if (name == playerList[i]) {
			cout << "you have chosen " << name << endl;
			nameFound = true;
			break;
		}
	}

	if (!nameFound) {
		playerList.push_back(name);
		cout << name << " added to playerList" << endl;
	}
	ofstream playerFO("playerList.txt");
	for (string p : playerList) {
		playerFO << p << endl;
	}
	playerFO.close();
	*pName = name;
	system("pause");
	system("cls");
}
//checks if the players are human , if they are call the displayPlayerList function otherwise set the name to AI
void GetPlayerNames(string *p1name, string *p2name, PlayerType p1type, PlayerType p2type) {
	if (p1type == PlayerType::HUMAN)
		displayPlayerList(p1name);
	else
		*p1name = "AI_1";

	if (p2type == PlayerType::HUMAN)
		displayPlayerList(p2name);
	else
		*p2name = "AI_2";
}
//Display the How to Play screen
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
	//if there was an error creating the renderer then quit
	if (renderer == NULL) {
		cout << "Error: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		system("pause");
		return;
	}
	bool display = true;

	Textbox textbox;
	textbox.setup(renderer);
	//easier to layout what it will look like
	string testT;
	testT = "================================How to Play================================";
	testT += " Take turns placing either a Naught or Cross on a tile                     ";
	testT += " Be the first to make 3 in a line of your own syumbol to win               ";
	testT += " You choose your move by entering a number between 1 and 9 inclusive       ";
	testT += " Press 'R' at anytime during the game to restart                           ";
	testT += " Hit Escape to quit and return to the main menu                            ";
	testT += " Below the board is the turn indicator                                     ";
	testT += " Board Layout";
	//setting the above text
	textbox.SetText(testT, window, 700);
	textbox.isVisible = true;

	textbox.draw();
	//loading the texture for the board layout
	boardLayoutTex = IMG_LoadTexture(renderer, "assets/boardLayout.png");
	SDL_Rect drawRect;
	int sWidth = 0;
	int sHeight = 0;
	SDL_GetWindowSize(window, &sWidth, &sHeight);

	drawRect.x = (sWidth / 2) - 150;
	drawRect.y = sHeight/2 - 75;
	drawRect.w = 300;
	drawRect.h = 300;

	SDL_RenderCopy(renderer, boardLayoutTex, NULL, &drawRect);
	while (display) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
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
	}
}
//Display the scores
void displayScores(int compVsPlayer, int compVsComp) {
	//create window
	SDL_Window* window = SDL_CreateWindow("Scores", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
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
	bool displaying = true;
	Textbox cvc;
	cvc.setup(renderer);
	cvc.isVisible = true;
	string text = "Computer wins vs Player: " + to_string(compVsPlayer);
	text += " Computer wins vs Computer: " + to_string(compVsComp);
	cvc.SetText(text, window, 350);
	SDL_Event e;
	while (displaying) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					displaying = false;
					SDL_DestroyWindow(window);
					return;
				}
			}
		}

		cvc.draw();
		SDL_RenderPresent(renderer);
	}
	//OPTIONAL:
	// 	display player names and their scores
}

//The Game Loop
void gameLoop(string p1Name, string p2Name) {
	system("cls");
	//create window
	SDL_Window* window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
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

	//set all the number textures
	zero = IMG_LoadTexture(renderer, "assets/0.png");
	one = IMG_LoadTexture(renderer, "assets/1.png");
	two = IMG_LoadTexture(renderer, "assets/2.png");
	three = IMG_LoadTexture(renderer, "assets/3.png");
	four = IMG_LoadTexture(renderer, "assets/4.png");
	five = IMG_LoadTexture(renderer, "assets/5.png");
	six = IMG_LoadTexture(renderer, "assets/6.png");
	seven = IMG_LoadTexture(renderer, "assets/7.png");
	eight = IMG_LoadTexture(renderer, "assets/8.png");
	nine = IMG_LoadTexture(renderer, "assets/9.png");
	ten = IMG_LoadTexture(renderer, "assets/10.png");
	

	Gameboard gameBoard(renderer, p1Type, p2Type);
	bool gameQuit = false;
	gameBoard.p1.name = p1Name;
	gameBoard.p2.name = p2Name;

	//bool pOneTurn = true;
	bool moved = false;
	cPlayer = &gameBoard.p1;//the current player
	oPlayer = &gameBoard.p2;//the opponent player
	//get the current time as a start point
	steady_clock::time_point startTime = steady_clock::now();
	//game loop
	while (!gameQuit) {
		if (oPlayer == nullptr || cPlayer == nullptr) {
			cout << "oPlayer or cPlayer is not set" << endl;
			return;
		}
		int tile;
		Move tileMove = Move();

		SDL_Event e;
		//create and set the countdown timer to be subtracted from later
		int countdownTimer = 10;
		bool restarted = false;
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
					case SDL_SCANCODE_R:
						gameBoard.clearBoard();
						cPlayer = &gameBoard.p1;
						oPlayer = &gameBoard.p2;
						startTime = steady_clock::now();
						restarted = true;
						break;
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
						//if the key entered is not a valid move then set the tileMove to -1,-1
						tileMove.xCoord = -1;
						tileMove.yCoord = -1;
						break;
					}
					if(!restarted)
						moved = gameBoard.makeMove(tileMove, *cPlayer);
				}
			}
		}
		SDL_Texture* timerTexture;
		//get the current time
		steady_clock::time_point curTime = steady_clock::now();
		//get the difference between the curTime and startTime
		duration<float> timeElapsed = curTime - startTime;
		//subtract timeElapsed from countdownTimer
		countdownTimer -= timeElapsed.count();
		timerTexture = SetTimerTexture(countdownTimer);
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

		//draw player names to the screen above the gameboard


		gameBoard.draw(tempTex, window);
		//draw turn symbol
		SDL_Rect turnRect;
		int sWidth = 0;
		int sHeight = 0;
		SDL_GetWindowSize(window, &sWidth, &sHeight);

		turnRect.x = (sWidth / 2) - (gameBoard.tileSize / 2);
		turnRect.y = sHeight - (sHeight / 3);
		turnRect.w = gameBoard.tileSize;
		turnRect.h = gameBoard.tileSize;

		SDL_RenderCopy(renderer, tempTex, NULL, &turnRect);
		//draw timer underneath gameboard and turn symbol
		
		SDL_Rect timeRect;
		timeRect.x = turnRect.x;
		timeRect.y = turnRect.y + gameBoard.tileSize;
		timeRect.w = gameBoard.tileSize;
		timeRect.h = gameBoard.tileSize;

		SDL_RenderCopy(renderer, timerTexture, NULL, &timeRect);


		//swap buffers
		SDL_RenderPresent(renderer);
		//sleep so that the computer player takes time to make a move
		Sleep(200);

		if (moved) {
			//check if the current player has won
			if (gameBoard.checkWin(*cPlayer)) {
				//check if the current player is human or computer
				if (cPlayer->playerType == PlayerType::COMPUTER)
					//check if the opposing player is human
					if (oPlayer->playerType == PlayerType::HUMAN) 
						compVsPlayer++;//increase the comp versus player tally
					else
						compVsComp++;//increase the comp vs comp tally (this should always be 0
				cout << "Player " << cPlayer->name << " has won!" << endl;
				gameQuit = true;
				break;
			}
			else if (gameBoard.checkFullBoard()) { //check if the board is full
				cout << "No one has won. It's a draw :(" << endl;
				gameQuit = true;
				break;
			}
			//swap the current player and the opposing player
			if (cPlayer->type == gameBoard.p1.type) {
				cPlayer = &gameBoard.p2;
				oPlayer = &gameBoard.p1;
			}
			else {
				cPlayer = &gameBoard.p1;
				oPlayer = &gameBoard.p2;
			}
			moved = false;
			//reset startTime
			startTime = steady_clock::now();
		}
		if (countdownTimer <= 0) {
			//check if the opposing player is a computer
			if (oPlayer->playerType == PlayerType::COMPUTER)
				//check if the current player is human
				if (cPlayer->playerType == PlayerType::HUMAN)
					compVsPlayer++;//increase the comp versus player tally
				else
					compVsComp++;//increase the comp vs comp tally (this will always be 0 due to the minimax algorithm)
			cout << cPlayer->name << " has taken too long and lost." << endl;
			cout << oPlayer->name << " wins the game." << endl;
			gameQuit = true;
		}


	}
	ofstream oFile;
	oFile.open("comScore.txt",fstream::trunc);
	oFile << compVsPlayer<<endl;
	oFile << compVsComp<<endl;
	oFile.close();
	SDL_DestroyWindow(window);
	system("pause");
	system("cls");

}
//Main
int main(int argc, char** arbv) {

	//TODO:
	//1.			when reading from file read first to string then int
	//		save player in a file that reads as "NAME  NUMBER" 
	//								stats such as name and wins
	string p1Name, p2Name;
	ifstream inFile;

	inFile.open("comScore.txt");
	if (inFile.is_open()) {
		inFile >> compVsPlayer;
		inFile >> compVsComp;
	}
	else {
		compVsPlayer = 0;
		compVsComp = 0;
	}
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
				//get the players to enter their names
				
				cout << "You have chosen Player vs Player" << endl;
				play = true;
				break;
			case PLAYER_VS_COMPUTER: //if the player chose option 2 - player vs computer
				p1Type = PlayerType::HUMAN;
				p2Type = PlayerType::COMPUTER;
				//get player to enter their name
				cout << "You have chosen Player vs Computer" << endl;
				play = true;
				break;
			case COMPUTER_VS_COMPUTER://if the player chose option 3 - computer vs computer
				p1Type = PlayerType::COMPUTER;
				p2Type = PlayerType::COMPUTER;
				//names will be kept as default
				cout << "You have chosen Computer vs Computer" << endl;
				play = true;
				break;
			case HOW_TO_PLAY://if the player chose option 4 - how to play
				displayHowToPlay();
				break;
			case SCORES:
				displayScores(compVsPlayer,compVsComp);
				system("cls");
				break;
			case QUIT://if the player chose option 5 - Quit
				cout << "Quitting..." << endl;
				quit = true;
				break;
			}
		}
		else {
			cout << "Please choose a valid number...try again" << endl;
			system("pause");
			system("cls");
		}
		if (play) {
			GetPlayerNames(&p1Name, &p2Name, p1Type, p2Type);
			gameLoop(p1Name,p2Name);
			play = false;
		}
	}

	system("pause");
	return 0;
}