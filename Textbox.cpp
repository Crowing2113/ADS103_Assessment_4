#include "Textbox.h"

Textbox::Textbox()
{
	font = TTF_OpenFont("assets/Arcade.ttf", 16);
}

Textbox::~Textbox()
{
	if (textTexture != NULL) {
		SDL_DestroyTexture(textTexture);
		TTF_CloseFont(font);
	}
}

void Textbox::setup(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void Textbox::SetText(string text,SDL_Window* window)
{
	//if the entered text is not the same as the currently set text and the text is not an empty string
	if (this->text != text && text != "") {
		if (textTexture != NULL)
			SDL_DestroyTexture(textTexture);

		this->text = text;

		//build text texture using font and text string
		SDL_Color textColor = {0,0,0,0};
		//create a surface out of the text
		SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
		//convert the surface to a texture
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		//delete the surface properly
		SDL_FreeSurface(textSurface);
		//get the width and height of the texture
		SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
		//calculate the center of the screen by first getting the height and width of the screen
		int wHeight, wWidth;
		SDL_GetWindowSize(window,&wWidth,&wHeight);
		textRect.x = wWidth - wWidth/2;
		textRect.y = wHeight/2;

		//calculate the boxRectangle
		boxRect.x = textRect.x - 4;
		boxRect.y = textRect.y - 2;
		boxRect.w = textRect.w +2 ;
		boxRect.h = textRect.h+2;

	}
}

void Textbox::draw()
{
	if (isVisible && textTexture != NULL && text != "") {
		//draw box
		SDL_SetRenderDrawColor(renderer, 217, 43, 90, 255);
		SDL_RenderFillRect(renderer, &boxRect);

		//draw the border
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &boxRect);

		//draw the text to the the screen
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	}
}