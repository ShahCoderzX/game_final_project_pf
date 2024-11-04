#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Libraries
#include "main.h"
#include "sdl.h"

typedef struct 
{
    float x,y;
    float vx,vy;
    bool active;
}Pointer;
typedef struct 
{
    float x,y;
    float vx,vy;
    bool active;
    float angle;
}Arrow;

// Create Account Function
typedef struct {
    char username[20];
    char password[20];
    int money;
} Account;

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

// ! Tower Game Function
//  Check if arrow hits the tower
bool checkCollisionTower(Arrow *arrow, int towerX, int towerY);
// Check if tower bomb hits the archer
bool checkCollisionArcher(Pointer *towerbomb, Pointer *archer);

// Account Creation
int createAccount(char* username, char* password);
// UserName Exists or not
int usernameExists(const char* username);
// Login Account
int login(char* username, char* password);

// Set Up Sprite Sheet frames
void setupSpriteFrames(SDL_Rect* clips, int totalFrames, int frameWidth, int frameHeight);

//! Tower Game Function declaration --------------------->

#endif