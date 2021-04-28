#include <iostream>
#include <string>
#include <SDL.h>

using namespace std;

int main(int argc, char** arbv) {
	
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

	system("pause");
	return 0;
}