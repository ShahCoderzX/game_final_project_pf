// All SDL Libraries
#include "sdl.h"

// Other Needed Libraries
#include "main.h"

// Constant Created Library
#include "constant.h"

// Functions
void clear(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, TTF_Font* fontHeading, SDL_Texture* backgroundTexture);
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, Uint8 r_color, Uint8 g_color, Uint8 b_color) ;
bool checkButtonClick(int mouseX, int mouseY, SDL_Rect* button);
void renderText_Heading(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font);

int main(int argc, char* args[]){

    // SDL INIT CHECK
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDl Text --> TTF
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow(
        "Game", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        Windows_Width, Windows_Height, SDL_WINDOW_SHOWN
    );

    // Check Window
    if(!window){
        printf("Window Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        printf("Renderer Error: %s", SDL_GetError());
        return 1;
    }

    // Surface
    SDL_Texture* main_backgroundTexture = IMG_LoadTexture(renderer, "./image/mainbackground.png");
    SDL_Texture* towerGame_mainPage_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_background.jpg");
    SDL_Texture* mainBackground = main_backgroundTexture;

    if(!main_backgroundTexture || !towerGame_mainPage_backgroundTexture){
        printf("Texture Error: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("./font/font.otf", 28);
    TTF_Font* mainfont = TTF_OpenFont("./font/mainfont.ttf", 20);
    TTF_Font* fontHeading = TTF_OpenFont("./font/font.otf", 100);

    if(!font || !mainfont || !fontHeading){
        printf("Font Error: %s\n", TTF_GetError());
        SDL_DestroyTexture(main_backgroundTexture);
        SDL_DestroyTexture(towerGame_mainPage_backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // For Main Home/ First Screen
    SDL_Rect selectedPage_back_Button = {20, 200, 80, 60};
    // Home Page
    SDL_Rect smallGames_play_Button = {(Windows_Width/2)-100, (Windows_Height/2)-160, 200, 80};
    SDL_Rect largeGames_play_Button = {smallGames_play_Button.x, smallGames_play_Button.y + 100, 200, 80};
    SDL_Rect quitGame_play_Button = {smallGames_play_Button.x, largeGames_play_Button.y + 100, 200, 80};
    // Small Games Page
    SDL_Rect snakeGame_play_Button = {(Windows_Width/2)-100, (Windows_Height/2)-160, 200, 80};
    SDL_Rect tictactoeGame_play_Button = {snakeGame_play_Button.x, snakeGame_play_Button.y + 100, 200, 80};
    SDL_Rect flappybirdGame_play_Button = {tictactoeGame_play_Button.x, tictactoeGame_play_Button.y + 100, 200, 80};
    // Large Game Button
    SDL_Rect towerGame_play_Button = {(Windows_Width/2)-100, (Windows_Height/2)-80, 200, 80};
    

    // For Tower Game
    // Home Menu Buttons
    SDL_Rect continueButton = {(Windows_Width/2)-100, (Windows_Height/2)-200, 200, 80};
    SDL_Rect levelButton = {continueButton.x, continueButton.y + 90, 200, 80};
    SDL_Rect optionButton = {continueButton.x, levelButton.y + 90, 200, 80};
    SDL_Rect quitButton = {continueButton.x, optionButton.y + 90, 200, 80};
    

    // Level Menu Button
    SDL_Rect menuHomeButton = {20, 20, 100, 80};
    SDL_Rect level1Button = {(Windows_Width/2)-200, (Windows_Height/2)- 300, 100, 80};
    SDL_Rect level2Button = {level1Button.x + 120, level1Button.y, 100, 80};
    SDL_Rect level3Button = {level2Button.x + 120, level1Button.y, 100, 80};
    SDL_Rect level4Button = {level3Button.x + 120, level1Button.y, 100, 80};
    SDL_Rect level5Button = {level1Button.x, level1Button.y + 100, 100, 80};
    SDL_Rect level6Button = {level2Button.x, level1Button.y + 100, 100, 80};
    SDL_Rect level7Button = {level3Button.x, level1Button.y + 100, 100, 80};
    SDL_Rect level8Button = {level4Button.x, level1Button.y + 100, 100, 80};

    // Option Menu Button
    SDL_Rect shopButton = {(Windows_Width/2)-100, (Windows_Height/2)-200, 200, 80};
    SDL_Rect musicButton = {shopButton.x, shopButton.y + 90, 200, 80};
    SDL_Rect soundButton = {shopButton.x, musicButton.y + 90, 200, 80};
    SDL_Rect controllerButton = {shopButton.x, soundButton.y + 90, 200, 80};

    // Option -----> Other Button
    SDL_Rect option_menuHomeButton = {20, 20, 100, 80};
    // Music Menu 
    SDL_Rect option_musicButton = {(Windows_Width/2)-200, (Windows_Height/2)-200, 200, 80};
    SDL_Rect option_music_on_off_Button = {option_musicButton.x + 220, option_musicButton.y, 200, 80};
    // Sound Menu 
    SDL_Rect option_soundButton = {(Windows_Width/2)-200, (Windows_Height/2)-200, 200, 80};
    SDL_Rect option_sound_on_off_Button = {option_soundButton.x + 220, option_soundButton.y, 200, 80};
    // Controller Menu
    SDL_Rect option_controllerButton_1 = {(Windows_Width/2)-200, (Windows_Height/2)-300, 200, 80};
    SDL_Rect option_controllerButton_2 = {option_controllerButton_1.x, option_controllerButton_1.y + 90, 200, 80};
    SDL_Rect option_controllerButton_3 = {option_controllerButton_2.x, option_controllerButton_2.y + 90, 200, 80};
    SDL_Rect option_controllerButton_4 = {option_controllerButton_3.x, option_controllerButton_3.y + 90, 200, 80};
    SDL_Rect option_controllerButton_5 = {option_controllerButton_4.x, option_controllerButton_4.y + 90, 200, 80};
    SDL_Rect option_controllerButton_6 = {option_controllerButton_5.x, option_controllerButton_5.y + 90, 200, 80};
    SDL_Rect option_controllerButton_7 = {option_controllerButton_6.x, option_controllerButton_6.y + 90, 200, 80};
    SDL_Rect option_controllerButton_8 = {option_controllerButton_7.x, option_controllerButton_7.y + 90, 200, 80};

    // For Snake Game

    // Boolean
    bool running = true;
    bool gameStarted = false;

    // For Main Home/ First  Page
    bool selectedGame_page = true;
    bool selectedGame_page_HomePage_menu = true;
    bool selectedGame_page_smallGames_menu = false;
    bool selectedGame_page_largeGames_menu = false;

    // Snake Game

    // All For Tower Game
    bool towerGame = false;
    bool towerGame_val = false;
    bool towerGame_Started = false;
    bool towerGame_homemenu = false;
    bool towerGame_levelmenu = false;
    bool towerGame_optionmenu = false;
    bool towerGame_option_shopMenu = false;
    bool towerGame_option_musicMenu = false;
    bool towerGame_option_soundMenu = false;
    bool towerGame_option_controllerMenu = false;

    // ON / OFF
    bool music = false;
    bool sound = false;

    // Event
    SDL_Event event;

    while (running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } 
        else if(!gameStarted){
            if((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)){
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                
                // Selected Game Page
                if(selectedGame_page){

                    mainBackground = main_backgroundTexture;

                    if(selectedGame_page_HomePage_menu){

                        if(checkButtonClick(mouseX, mouseY, &smallGames_play_Button)){
                            selectedGame_page_smallGames_menu = true;
                            selectedGame_page_HomePage_menu = false;
                        }else if(checkButtonClick(mouseX, mouseY, &largeGames_play_Button)){
                            selectedGame_page_largeGames_menu = true;
                            selectedGame_page_HomePage_menu = false;
                        }else if(checkButtonClick(mouseX, mouseY, &quitGame_play_Button)){
                            selectedGame_page_largeGames_menu = false;
                            selectedGame_page_HomePage_menu = false;
                            selectedGame_page = false;
                            running = false;
                        }

                    }else if(selectedGame_page_smallGames_menu){
                        if(checkButtonClick(mouseX, mouseY, &snakeGame_play_Button)){
                            printf("Snake Game Play");
                        }else if(checkButtonClick(mouseX, mouseY, &tictactoeGame_play_Button)){
                            printf("Tic Tac Toe Game Play");
                        }else if(checkButtonClick(mouseX, mouseY, &flappybirdGame_play_Button)){
                            printf("Flappy Bird Game Play");
                        }else if(checkButtonClick(mouseX, mouseY, &selectedPage_back_Button)){
                            selectedGame_page_HomePage_menu = true;
                            selectedGame_page_smallGames_menu = false;
                        }
                    }else if(selectedGame_page_largeGames_menu){
                        if(checkButtonClick(mouseX, mouseY, &towerGame_play_Button)){
                            selectedGame_page = false;
                            towerGame = true;
                            towerGame_homemenu = true;
                            towerGame_val = true;
                        }else if(checkButtonClick(mouseX, mouseY, &selectedPage_back_Button)){
                            selectedGame_page_HomePage_menu = true;
                            selectedGame_page_largeGames_menu = false;
                        }
                    }
                }

                // If Snake Game is True

                // If Tower Game is True
                if(towerGame){
                    
                    mainBackground = towerGame_mainPage_backgroundTexture;
                    if(towerGame_homemenu){
                        if(checkButtonClick(mouseX, mouseY, &continueButton)){
                            printf("Continue Game");
                        }else if(checkButtonClick(mouseX, mouseY, &levelButton)){
                            if(towerGame_val){
                                towerGame_levelmenu = false;
                                towerGame_homemenu = true;
                                towerGame_val = false;
                            }else{
                                towerGame_levelmenu = true;
                                towerGame_homemenu = false;
                            }
                        }else if(checkButtonClick(mouseX, mouseY, &optionButton)){
                            if(towerGame_val == 1){
                                towerGame_optionmenu = false;
                                towerGame_homemenu = true;
                                towerGame_val = false;
                            }else{
                                towerGame_optionmenu = true;
                                towerGame_homemenu = false;
                            }
                        }else if(checkButtonClick(mouseX, mouseY, &quitButton)){
                            towerGame = false;
                            selectedGame_page = true;
                            mainBackground = main_backgroundTexture;
                        }
                    }else if(towerGame_levelmenu){
                        if(checkButtonClick(mouseX, mouseY, &menuHomeButton)){
                            towerGame_homemenu = true;
                            towerGame_levelmenu = false;
                        }else if(checkButtonClick(mouseX, mouseY, &level1Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level2Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level3Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level4Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level5Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level6Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level7Button)){
                            printf("Level");
                        }else if(checkButtonClick(mouseX, mouseY, &level8Button)){
                            printf("Level");
                        }
                    }else if(towerGame_optionmenu){
                        if(checkButtonClick(mouseX, mouseY, &menuHomeButton)){
                            towerGame_optionmenu = false;
                            towerGame_homemenu  = true;
                        }else if(checkButtonClick(mouseX, mouseY, &musicButton)){
                            towerGame_optionmenu = false;
                            towerGame_option_musicMenu = true;
                        }else if(checkButtonClick(mouseX, mouseY, &shopButton)){
                            towerGame_optionmenu = false;
                            towerGame_option_shopMenu = true;
                        }else if(checkButtonClick(mouseX, mouseY, &soundButton)){
                            towerGame_optionmenu = false;
                            towerGame_option_soundMenu = true;
                        }else if(checkButtonClick(mouseX, mouseY, &controllerButton)){
                            towerGame_optionmenu = false;
                            towerGame_option_controllerMenu = true;
                        }
                    }else if(towerGame_option_musicMenu){
                        if(checkButtonClick(mouseX, mouseY, &option_music_on_off_Button)){
                            music = !music;
                        }else if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                            towerGame_option_musicMenu = false;
                            towerGame_optionmenu = true;
                        }
                    }else if(towerGame_option_soundMenu){
                        if(checkButtonClick(mouseX, mouseY, &option_sound_on_off_Button)){
                            sound = !sound;
                        }else if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                            towerGame_option_soundMenu = false;
                            towerGame_optionmenu = true;
                        }
                    }else if(towerGame_option_controllerMenu){
                        if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                            towerGame_option_controllerMenu = false;
                            towerGame_optionmenu = true;
                        }
                    }else if(towerGame_option_shopMenu){
                        if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                            towerGame_option_shopMenu = false;
                            towerGame_optionmenu = true;
                        }
                    }
                }

            }else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
            }
        }
        else if(gameStarted){
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, mainBackground, NULL, NULL);

    // If Selected Page is true
    if(selectedGame_page){
        if(selectedGame_page_HomePage_menu){
            SDL_SetRenderDrawColor(renderer, 225, 225, 225, 1);

            SDL_RenderFillRect(renderer, &smallGames_play_Button);
            SDL_RenderFillRect(renderer, &largeGames_play_Button);
            SDL_RenderFillRect(renderer, &quitGame_play_Button);

            renderText(renderer, "Play Small Games", smallGames_play_Button.x + 10, smallGames_play_Button.y + 28, mainfont, 0, 0, 0);
            renderText(renderer, "Play Large Games", largeGames_play_Button.x + 10, largeGames_play_Button.y + 28, mainfont,0,0,0);
            renderText(renderer, "Quit Game", quitGame_play_Button.x + 50, quitGame_play_Button.y + 28, mainfont,0,0,0);
        }else if(selectedGame_page_smallGames_menu){
            SDL_SetRenderDrawColor(renderer, 225, 225, 225, 1);

            SDL_RenderFillRect(renderer, &snakeGame_play_Button);
            SDL_RenderFillRect(renderer, &tictactoeGame_play_Button);
            SDL_RenderFillRect(renderer, &flappybirdGame_play_Button);
            SDL_RenderFillRect(renderer, &selectedPage_back_Button);

            renderText(renderer, "Snake Game", snakeGame_play_Button.x + 42, snakeGame_play_Button.y + 28, mainfont, 0, 0, 0);
            renderText(renderer, "Tic Tac Toe", tictactoeGame_play_Button.x + 42, tictactoeGame_play_Button.y + 28, mainfont, 0, 0, 0);
            renderText(renderer, "Flappy Bird", flappybirdGame_play_Button.x + 46, flappybirdGame_play_Button.y + 28, mainfont, 0, 0, 0);
            renderText(renderer, "Back", selectedPage_back_Button.x + 14, selectedPage_back_Button.y + 20, mainfont, 0, 0, 0);
        }else if(selectedGame_page_largeGames_menu){
            SDL_SetRenderDrawColor(renderer, 225, 225, 225, 1);

            SDL_RenderFillRect(renderer, &towerGame_play_Button);
            SDL_RenderFillRect(renderer, &selectedPage_back_Button);

            renderText(renderer, "Deck of Dominion", towerGame_play_Button.x + 14, towerGame_play_Button.y + 28, mainfont, 0, 0, 0);
            renderText(renderer, "Back", selectedPage_back_Button.x + 14, selectedPage_back_Button.y + 20, mainfont, 0, 0, 0);
        }
    }


    // If Tower Game is True
    if(towerGame){
        if(!towerGame_Started && towerGame_homemenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            // Render Button
            SDL_RenderFillRect(renderer, &continueButton);
            SDL_RenderFillRect(renderer, &levelButton);
            SDL_RenderFillRect(renderer, &optionButton);
            SDL_RenderFillRect(renderer, &quitButton);

            // Render button text
            renderText(renderer, "Continue Game", continueButton.x + 30, continueButton.y + 24, font,225 ,225, 225);
            renderText(renderer, "Levels", levelButton.x + 60, levelButton.y + 24, font,225 ,225, 225);
            renderText(renderer, "Options", optionButton.x + 60, optionButton.y + 24, font,225 ,225, 225);
            renderText(renderer, "Quit Game", quitButton.x + 50, quitButton.y + 24, font,225 ,225, 225);
        }else if(!towerGame_Started && towerGame_levelmenu){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
            SDL_RenderFillRect(renderer, &menuHomeButton);
            SDL_RenderFillRect(renderer, &level1Button);
            SDL_RenderFillRect(renderer, &level2Button);
            SDL_RenderFillRect(renderer, &level3Button);
            SDL_RenderFillRect(renderer, &level4Button);
            SDL_RenderFillRect(renderer, &level5Button);
            SDL_RenderFillRect(renderer, &level6Button);
            SDL_RenderFillRect(renderer, &level7Button);
            SDL_RenderFillRect(renderer, &level8Button);

            // Render the button text
            renderText(renderer, "Back", menuHomeButton.x + 30, menuHomeButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "1", level1Button.x + 45, level1Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "2", level2Button.x + 45, level2Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "3", level3Button.x + 45, level3Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "4", level4Button.x + 45, level4Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "5", level5Button.x + 45, level5Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "6", level6Button.x + 45, level6Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "7", level7Button.x + 45, level7Button.y + 26, font,225 ,225, 225);
            renderText(renderer, "8", level8Button.x + 45, level8Button.y + 26, font,225 ,225, 225);
        }else if(!towerGame_Started && towerGame_optionmenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            // Render Button
            SDL_RenderFillRect(renderer, &menuHomeButton);
            SDL_RenderFillRect(renderer, &shopButton);
            SDL_RenderFillRect(renderer, &musicButton);
            SDL_RenderFillRect(renderer, &soundButton);
            SDL_RenderFillRect(renderer, &controllerButton);

            // Render Button Text
            renderText(renderer, "Back", menuHomeButton.x + 30, menuHomeButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Shop", shopButton.x + 75, shopButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Music", musicButton.x + 70, musicButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Sound", soundButton.x + 70, soundButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Controller", controllerButton.x + 40, controllerButton.y + 26, font,225 ,225, 225);
        }else if(!towerGame_Started && towerGame_option_musicMenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            // Render Button
            SDL_RenderFillRect(renderer, &option_musicButton);
            SDL_RenderFillRect(renderer, &option_music_on_off_Button);
            SDL_RenderFillRect(renderer, &option_menuHomeButton);

            // Render Button Text
            renderText_Heading(renderer, "Music", (Windows_Width/2) - 100, 100, fontHeading);
            renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Music", option_musicButton.x + 70, option_musicButton.y + 26, font,225 ,225, 225);
            renderText(renderer, (music) ? "On" : "Off", option_music_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font,225 ,225, 225);
        }else if(!towerGame_Started && towerGame_option_soundMenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            // Render Button
            SDL_RenderFillRect(renderer, &option_soundButton);
            SDL_RenderFillRect(renderer, &option_sound_on_off_Button);
            SDL_RenderFillRect(renderer, &option_menuHomeButton);

            // Render Button Text
            renderText_Heading(renderer, "Sound", (Windows_Width/2) - 100, 100, fontHeading);
            renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
            renderText(renderer, "Sound", option_soundButton.x + 70, option_soundButton.y + 26, font,225 ,225, 225);
            renderText(renderer, (sound) ? "On" : "Off", option_sound_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font,225 ,225, 225);
        }else if(!towerGame_Started && towerGame_option_controllerMenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            // Render Button
            SDL_RenderFillRect(renderer, &option_menuHomeButton);
            SDL_RenderFillRect(renderer, &option_controllerButton_1);
            SDL_RenderFillRect(renderer, &option_controllerButton_2);
            SDL_RenderFillRect(renderer, &option_controllerButton_3);
            SDL_RenderFillRect(renderer, &option_controllerButton_4);
            SDL_RenderFillRect(renderer, &option_controllerButton_5);
            SDL_RenderFillRect(renderer, &option_controllerButton_6);
            SDL_RenderFillRect(renderer, &option_controllerButton_7);
            SDL_RenderFillRect(renderer, &option_controllerButton_8);


            // Render Button Text
            renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
            renderText_Heading(renderer, "Controller", (Windows_Width/2) - 200, 100, fontHeading);

            renderText(renderer, "W", option_controllerButton_1.x + 30, option_controllerButton_1.y + 26, font,225 ,225, 225);
            renderText(renderer, "Forward", option_controllerButton_1.x + 220, option_controllerButton_1.y + 26, font,225 ,225, 225);
            renderText(renderer, "S", option_controllerButton_2.x + 30, option_controllerButton_2.y + 26, font,225 ,225, 225);
            renderText(renderer, "Backward", option_controllerButton_2.x + 220, option_controllerButton_2.y + 26, font,225 ,225, 225);
            renderText(renderer, "L", option_controllerButton_3.x + 30, option_controllerButton_3.y + 26, font,225 ,225, 225);
            renderText(renderer, "Left", option_controllerButton_3.x + 220, option_controllerButton_3.y + 26, font,225 ,225, 225);
            renderText(renderer, "R", option_controllerButton_4.x + 30, option_controllerButton_4.y + 26, font,225 ,225, 225);
            renderText(renderer, "Right", option_controllerButton_4.x + 220, option_controllerButton_4.y + 26, font,225 ,225, 225);

            renderText(renderer, "F", option_controllerButton_5.x + 30, option_controllerButton_5.y + 26, font,225 ,225, 225);
            renderText(renderer, "Attack", option_controllerButton_5.x + 220, option_controllerButton_5.y + 26, font,225 ,225, 225);
            renderText(renderer, "H", option_controllerButton_6.x + 30, option_controllerButton_6.y + 26, font,225 ,225, 225);
            renderText(renderer, "Heal", option_controllerButton_6.x + 220, option_controllerButton_6.y + 26, font,225 ,225, 225);
            renderText(renderer, "Space", option_controllerButton_7.x + 30, option_controllerButton_7.y + 26, font,225 ,225, 225);
            renderText(renderer, "Jump", option_controllerButton_7.x + 220, option_controllerButton_7.y + 26, font,225 ,225, 225);
            renderText(renderer, "Ctrl", option_controllerButton_8.x + 30, option_controllerButton_8.y + 26, font,225 ,225, 225);
            renderText(renderer, "Crouch", option_controllerButton_8.x + 220, option_controllerButton_8.y + 26, font,225 ,225, 225);

        }else if(!towerGame_Started && towerGame_option_shopMenu){
            SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
            SDL_RenderFillRect(renderer, &option_menuHomeButton);
            // Render Button Text
            renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
            renderText_Heading(renderer, "Shop", (Windows_Width/2) - 90, 100, fontHeading);
            renderText(renderer, "Shop is Empty", (Windows_Width/2) - 80, 300, font, 225, 225, 225);

        }
    }



    // SDL Render Present
    SDL_RenderPresent(renderer);
}



    clear(window, renderer, font, fontHeading, main_backgroundTexture);
    

}

// Functions
void clear(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, TTF_Font* fontHeading, SDL_Texture* backgroundTexture){
    TTF_CloseFont(font);
    TTF_CloseFont(fontHeading);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

}

// Check for button click
bool checkButtonClick(int mouseX, int mouseY, SDL_Rect* button) {
    return mouseX >= button->x && mouseX <= button->x + button->w &&
           mouseY >= button->y && mouseY <= button->y + button->h;
}

// RenderText
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font,Uint8 r_color,Uint8 g_color,Uint8 b_color) {
    SDL_Color color = { r_color, g_color, b_color }; // White color
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void renderText_Heading(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font) {
    SDL_Color color = { 0, 0, 0, 255}; // White color
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}