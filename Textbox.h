#pragma once
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

class Textbox
{
public:
	SDL_Renderer* renderer;

	TTF_Font* font;

	string text;
	SDL_Texture* textTexture = NULL;
	SDL_Rect textRect;
	SDL_Rect boxRect;

	bool isVisible = true;

	Textbox();
	~Textbox();

	void setup(SDL_Renderer* renderer);
	void SetText(string text, SDL_Window* window, Uint32 wrapping, SDL_Color tColor = { 0,0,0,0 });
	void draw();
};

