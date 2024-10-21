// Libraries
#include "main.h"
#include "sdl.h"
#include "functions.h"

//! All Common Fucntions --------------------->

// Clear Fucntion --------------------->
void clear(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, TTF_Font* fontHeading, SDL_Texture* backgroundTexture){
    TTF_CloseFont(font);
    TTF_CloseFont(fontHeading);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

}
// ---------------------x---------------------x

// Check for button click --------------------->
bool checkButtonClick(int mouseX, int mouseY, SDL_Rect* button) {
    return mouseX >= button->x && mouseX <= button->x + button->w &&
           mouseY >= button->y && mouseY <= button->y + button->h;
}
// ---------------------x---------------------

// RenderText --------------------->
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font,Uint8 r_color,Uint8 g_color,Uint8 b_color) {
    SDL_Color color = { r_color, g_color, b_color, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
// ---------------------x---------------------x

// Render Text Heading --------------------->
void renderText_Heading(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, Uint8 r_color,Uint8 g_color,Uint8 b_color) {
    SDL_Color color = { r_color, g_color, b_color, 255}; 
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
// ---------------------x---------------------x

//! ---------------------x---------------------x