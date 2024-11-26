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
    bool spawn;
    bool runactive;
}Character;
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
bool checkCollisionTower(Arrow *arrow, int towerX, int towerY, int arrowWidth, int arrowHeight, int towerWidth, int towerHeight);
// Check if tower bomb hits the archer
bool checkCollisionArcher(Pointer *towerbomb, Character *archer, int archerWidth, int archerHeight, int towerWidth, int towerHeight) ;
// Check if enemy hits the archer
bool checkCollisionEnemy(Character *towerbomb, Character *archer, int archerWidth, int archerHeight, int towerWidth, int towerHeight) ;

// Account Creation
int createAccount(char* username, char* password);
// UserName Exists or not
int usernameExists(const char* username);
// Login Account
int login(char* username, char* password);

// Set Up Sprite Sheet frames
void setupSpriteFrames(SDL_Rect* clips, int totalFrames, int frameWidth, int frameHeight);


// For Change All the varibles to default
 void default_game(SDL_Rect* tower_attacker, SDL_Rect* hot_air_balloon_rect, Pointer* tower_bomb, Pointer* tower_bomb_2,Pointer* tower_bomb_3, Pointer* tower_bomb_1_5, Pointer* tower_bomb_4, Pointer* hot_air_balloon_bomb, Arrow* archer_arrow, bool* towerGame_Started_level1, bool* towerGame_Started_level2, bool* towerGame_Started_level3, bool* towerGame_Started_level4, bool* towerGame_Started_level5, bool* all_levels_Started, bool* towerGame_Started, SDL_Texture* mainBackground,bool* towerGame_levelmenu, bool*  towerGame_homemenu, int* money, int* archer_health, int* knight_health, int* mille_health, int* soldier_health, int* tower_attacker_health, int* hot_air_balloon_health, int* wolf_health, int* archer_card_cool, int* knight_card_cool, int* mille_card_cool,  int* soldier_card_cool, bool* midPause, bool* midPause_dialog_starting, bool* midPause_archer_moving, bool* frame_Video, bool* loading_screen, bool* archer_standing, int* archerY, Character* archer, Character* knight, Character* mille, Character* soldier, Character* wolf, SDL_Rect* archer_health_box, SDL_Rect* knight_health_box, SDL_Rect* soldier_health_box, SDL_Rect* mille_health_box, SDL_Rect* tower_health_box, SDL_Rect* hot_air_ballon_health_box, SDL_Rect* wolf_health_box, bool* wolf_running, bool* wolf_attacking, SDL_Rect* mille_rect, SDL_Rect* soldier_rect, SDL_Texture* towerGame_level1_background, SDL_Texture* towerGame_level2_background, SDL_Texture* towerGame_level3_background, SDL_Texture* towerGame_level4_background, SDL_Texture* towerGame_level5_background, int level3_towerWidth, int level3_towerHeight, int level4_towerWidth, int level4_towerHeight, int level5_towerWidth, int level5_towerHeight, int knight_basic_Width, int knight_basic_Height, int archer_basic_Width, int mille_basic_Width, int mille_basic_Height, int soldier_basic_Width, int soldier_basic_Height, int* milleY,float ScaleX, float ScaleY, int Windows_Width, int Windows_Height, int towerWidth, int towerHeight, int level, bool* level1_ending_scene, int* archerclicked, int* knightclicked, int* milleclicked, int* soldierclicked);

//! Tower Game Function declaration --------------------->

#endif