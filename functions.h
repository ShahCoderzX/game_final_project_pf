#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Libraries
#include "main.h"
#include "sdl.h"

//! All Functions --------------------->
// Clear Function
void clear(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, TTF_Font* fontHeading, SDL_Texture* backgroundTexture);

// Check Button Click
bool checkButtonClick(int mouseX, int mouseY, SDL_Rect* button);

// Render Text Function
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, Uint8 r_color, Uint8 g_color, Uint8 b_color) ;

// Render Text Heading
void renderText_Heading(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, Uint8 r_color, Uint8 g_color, Uint8 b_color);


//! ---------------------x---------------------x

//! Tower Game Function declaration --------------------->

#endif