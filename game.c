// All SDL Libraries
#include "sdl.h"

// Other Needed Libraries
#include "main.h"

// Constant Created Library
#include "constant.h"

// Own Functions Created Library
#include "functions.h"

int main(int argc, char* args[]){

    int Windows_Width, Windows_Height;

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

    // Give The Values of Windows Resoltuion
    SDL_Rect screenDisplay;
    if(SDL_GetDisplayBounds(0, &screenDisplay) == 0){
        if(screenDisplay.w >= 1920 && screenDisplay.h >= 1080){
            Windows_Width = 1920;
            Windows_Height = 1080;
        }else if(screenDisplay.w >= 1280 && screenDisplay.h >= 720){
            Windows_Width = 1280;
            Windows_Height = 720;
        }else if(screenDisplay.w >= 1024 && screenDisplay.h <= 768){
            Windows_Width = 1024;
            Windows_Height = 768;
        }else if(screenDisplay.w >= 800 && screenDisplay.h <= 600){
            Windows_Width = 800;
            Windows_Height = 600;
        }else{
            Windows_Width = screenDisplay.w;
            Windows_Height = screenDisplay.h;
        }
    }

    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow(
        "Impact Gamer", 
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

    // Check Renderer
    if(!renderer){
        printf("Renderer Error: %s", SDL_GetError());
        return 1;
    }

    // Surface Texture
    SDL_Texture* main_backgroundTexture = IMG_LoadTexture(renderer, "./image/mainbackground.png");
    SDL_Texture* towerGame_homePage_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_background.jpg");
    SDL_Texture* towerGame_mainPage_backgroundTexture = IMG_LoadTexture(renderer, "./image/main_tower_background.jpg");
    SDL_Texture* towerGame_level1_background = IMG_LoadTexture(renderer, "./image/towerGame_level1_background.jpg");
    SDL_Texture* arrow = IMG_LoadTexture(renderer, "./image/arrow.png");
    SDL_Texture* archer_texture = IMG_LoadTexture(renderer, "./image/archer.png");
    // Main Background Texture
    SDL_Texture* mainBackground = main_backgroundTexture;

    // Check For all Background Texture
    if(!main_backgroundTexture || !towerGame_mainPage_backgroundTexture || !towerGame_homePage_backgroundTexture || !towerGame_level1_background){
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
    TTF_Font* towerGame_font = TTF_OpenFont("./font/tower_font.ttf", 30);
    TTF_Font* towerGame_fontHeading = TTF_OpenFont("./font/tower_font.ttf", 100);

    // Check All The Fonts
    if(!font || !mainfont || !fontHeading || !towerGame_font || !towerGame_fontHeading){
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
    // Tower Characters
    SDL_Rect tower_attacker = {100, Windows_Height - (tower_attacker.h + 300), 100, 300};
    // SDL_Rect archer_character = {Windows_Width-(archer_character.w + 100), Windows_Height - (archer_character.h + 300), 200, 160};
    int archerX = Windows_Width-(200 + 100);
    int archerY = Windows_Height - (160 + 300);
    Pointer archer = {(float)archerX, (float)archerY, 0, 0, false};
    Pointer tower_bomb = {(float)tower_attacker.x, (float)tower_attacker.y, 0, 0, false};
    Arrow archer_arrow = {(float)archer.x, (float)archer.y, 0, 0, false, (float)0};
    // Health Bar
    SDL_Rect tower_health_box= {100, 100, 400, 60};
    SDL_Rect tower_health_box_outline= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};
    SDL_Rect tower_health_box_innerfill= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};

    SDL_Rect archer_health_box = {Windows_Width-archer_health_box.w - 100, 100, 400, 60};
    SDL_Rect archer_health_box_outline = {archer_health_box.x, archer_health_box.y, archer_health_box.w, archer_health_box.h};
    SDL_Rect archer_health_box_innerfill = {archer_health_box.x, archer_health_box.y, archer_health_box.w, archer_health_box.h};

    // For Snake Game

    // Boolean
    bool running = true;
    bool gameStarted = false;

    // For Main Home/ First  Page
    bool selectedGame_page = true;   //! True Karo
    bool selectedGame_page_HomePage_menu = true; //! True Karo
    bool selectedGame_page_smallGames_menu = false;
    bool selectedGame_page_largeGames_menu = false;

    // Snake Game

    // All For Tower Game
    bool towerGame_start_bt = false;
    bool towerGame = false; //! False Karo
    bool towerGame_Started = false; //! False Karo
    bool towerGame_homemenu = false;
    bool towerGame_levelmenu = false;
    bool towerGame_optionmenu = false;
    bool towerGame_option_shopMenu = false;
    bool towerGame_option_musicMenu = false;
    bool towerGame_option_soundMenu = false;
    bool towerGame_option_controllerMenu = false;

    // Tower Game Levels
    bool towerGame_Started_level1 = false; //! False Karo
    bool towerGame_Started_level2 = false;
    bool towerGame_Started_level3 = false;
    bool towerGame_Started_level4 = false;
    bool towerGame_Started_level5 = false;
    bool towerGame_Started_level6 = false;
    bool towerGame_Started_level7 = false;
    bool towerGame_Started_level8 = false;

    // Health For Tower Game
    int tower_attacker_health = tower_health_box.w/4;
    int archer_health = archer_health_box.w/4;


    // ON / OFF
    bool music = false;
    bool sound = false;

    // !
    float tower_attack_timer = 0.0f;  // Timer for tracking attack delay
    const float tower_attack_delay = 1.0f;  // Delay in seconds

    // Inside your game loop:
    float deltaTime = 0.016f;  // Assume you calculate the delta time from the frame rate (e.g., 1/60 seconds)
    tower_attack_timer += deltaTime;  // Update the timer

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
                            towerGame_start_bt = true;
                        }else if(checkButtonClick(mouseX, mouseY, &selectedPage_back_Button)){
                            selectedGame_page_HomePage_menu = true;
                            selectedGame_page_largeGames_menu = false;
                        }
                    }
                }

                // If Snake Game is True

                // If Tower Game is True
                if(towerGame){
                    if(!towerGame_Started){
                        if(towerGame_start_bt){
                            mainBackground = towerGame_mainPage_backgroundTexture;
                        }else{
                            mainBackground = towerGame_homePage_backgroundTexture;
                        }
                        if(towerGame_homemenu){
                            if(checkButtonClick(mouseX, mouseY, &continueButton)){
                                printf("Continue Game");
                            }else if(checkButtonClick(mouseX, mouseY, &levelButton)){
                                    towerGame_levelmenu = true;
                                    towerGame_homemenu = false;
                            }else if(checkButtonClick(mouseX, mouseY, &optionButton)){
                                    towerGame_optionmenu = true;
                                    towerGame_homemenu = false;
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
                                towerGame_Started_level1 = true;
                                towerGame_Started = true;
                                mainBackground = towerGame_level1_background;
                                towerGame_levelmenu = false;
                                towerGame_homemenu = true;
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
                    }else if(towerGame_Started){
                        if(towerGame_Started_level1){
                            mainBackground = towerGame_level1_background;

                        }
                    }
                }

            }else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
                if(towerGame){
                    if(!towerGame_Started){
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_RETURN:
                            if(towerGame_start_bt){
                                towerGame_start_bt = false;
                                towerGame_homemenu = true;
                                mainBackground = towerGame_homePage_backgroundTexture;
                            }
                            break;
                        default:
                            break;
                        }
                    }else if(towerGame_Started){
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_LEFT:
                            if(archer.x > (Windows_Width - 200)/2){
                                archer.x -= 8;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(archer.x < Windows_Width - (200)){
                                archer.x += 8;
                            }
                            break;
                        case SDLK_UP: 
                            if (archer.y >= archerY) {  
                                archer.vy = -10; 
                                archer.active = true; 
                            }
                            break;
                        case SDLK_f: 
                            if(!archer_arrow.active){
                                archer_arrow.vx = -18;
                                archer_arrow.vy = -22;
                                archer_arrow.active = true;
                                archer_arrow.angle = 0;
                            }
                            break;
                        default:
                            break;
                        }
                    }
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
    
    if(towerGame_Started){
        if(towerGame_Started_level1){

            if (archer.active) {
                archer.y += archer.vy; 
                archer.vy += 0.5; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }
            tower_attack_timer += deltaTime;  // Update the timer
            
            if (tower_attack_timer >= tower_attack_delay) {
                if(!tower_bomb.active){
                    tower_bomb.vx += 20;
                    tower_bomb.vy += -18;
                    tower_bomb.active = true;
                }
            }
                // tower_bomb.active = true;
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx;
                tower_bomb.y += tower_bomb.vy;
                tower_bomb.vy += 0.5;
                if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer)){
                    if(checkCollisionArcher(&tower_bomb, &archer)){
                        if(archer_health >0){
                            archer_health_box.w -= 10;  
                            archer_health = archer_health_box.w/4;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = tower_attacker.x;
                    tower_bomb.y = tower_attacker.y;
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }
            }

            if(archer_arrow.active){
                archer_arrow.x += archer_arrow.vx;
                archer_arrow.y += archer_arrow.vy;
                archer_arrow.vy += 0.5;      
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy < -25) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }



                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y)){
                        if(tower_attacker_health>0){
                            tower_health_box.w -= 10;
                            tower_attacker_health = tower_health_box.w/4;
                        }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.x = archer.x;
                    archer_arrow.y = archer.y;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
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
        if(!towerGame_Started){
            if(towerGame_start_bt){
                // Render button text
                renderText(renderer, "DECK", (Windows_Width/2)-100, (Windows_Height/2)-200, towerGame_fontHeading,0 ,0, 0);
                renderText(renderer, "OF", (Windows_Width/2)-40, (Windows_Height/2) - 100 , towerGame_fontHeading,0 ,0, 0);
                renderText(renderer, "DOMINIONS", (Windows_Width/2)-240, (Windows_Height/2), towerGame_fontHeading,0 ,0, 0);
                renderText(renderer, "Press \"Enter\" to start the Game", (Windows_Width/2)-250, (Windows_Height-100), towerGame_font,255 , 255, 255);
            }else if(towerGame_homemenu){
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
            }else if(towerGame_levelmenu){
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
            }else if(towerGame_optionmenu){
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
            }else if(towerGame_option_musicMenu){
                SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
                // Render Button
                SDL_RenderFillRect(renderer, &option_musicButton);
                SDL_RenderFillRect(renderer, &option_music_on_off_Button);
                SDL_RenderFillRect(renderer, &option_menuHomeButton);

                // Render Button Text
                renderText_Heading(renderer, "Music", (Windows_Width/2) - 100, 100, fontHeading, 0, 0, 0);
                renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
                renderText(renderer, "Music", option_musicButton.x + 70, option_musicButton.y + 26, font,225 ,225, 225);
                renderText(renderer, (music) ? "On" : "Off", option_music_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font,225 ,225, 225);
            }else if(towerGame_option_soundMenu){
                SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
                // Render Button
                SDL_RenderFillRect(renderer, &option_soundButton);
                SDL_RenderFillRect(renderer, &option_sound_on_off_Button);
                SDL_RenderFillRect(renderer, &option_menuHomeButton);

                // Render Button Text
                renderText_Heading(renderer, "Sound", (Windows_Width/2) - 100, 100, fontHeading, 0, 0, 0);
                renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
                renderText(renderer, "Sound", option_soundButton.x + 70, option_soundButton.y + 26, font,225 ,225, 225);
                renderText(renderer, (sound) ? "On" : "Off", option_sound_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font,225 ,225, 225);
            }else if(towerGame_option_controllerMenu){
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
                renderText_Heading(renderer, "Controller", (Windows_Width/2) - 200, 100, fontHeading, 0, 0, 0);

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

            }else if(towerGame_option_shopMenu){
                SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
                SDL_RenderFillRect(renderer, &option_menuHomeButton);
                // Render Button Text
                renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font,225 ,225, 225);
                renderText_Heading(renderer, "Shop", (Windows_Width/2) - 90, 100, fontHeading, 0, 0, 0);
                renderText(renderer, "Shop is Empty", (Windows_Width/2) - 80, 300, font, 225, 225, 225);

            }
        }else if(towerGame_Started){
            if(towerGame_Started_level1){

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, 120, 50};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }


                SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, 40, 40};
                SDL_SetRenderDrawColor(renderer, 150, 75, 0, 1);
                SDL_RenderFillRect(renderer, &tower_bomb_rect);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
                SDL_RenderFillRect(renderer, &tower_attacker);


                SDL_Rect archer_rect = {(int)archer.x, (int)archer.y, 300, 200};
                SDL_RenderCopy(renderer,archer_texture, NULL, &archer_rect);
                // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1);
                // SDL_RenderFillRect(renderer, &archer_rect);


                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,1);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, font, 0, 0, 0);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 1);
                SDL_RenderFillRect(renderer, &tower_health_box);

                char archerHealth_val[50];
                sprintf(archerHealth_val, "Archer Health: %d %%", archer_health);
                renderText(renderer, archerHealth_val, archer_health_box.x, archer_health_box.y-40, font, 0, 0, 0);
                SDL_SetRenderDrawColor(renderer, 127,23,31, 1);
                SDL_RenderFillRect(renderer, &archer_health_box);

                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
            }
        }
    }


    if(towerGame_Started){
        SDL_Delay(16);
    }
    // SDL Render Present
    SDL_RenderPresent(renderer);
}

    clear(window, renderer, font, fontHeading, main_backgroundTexture);

}

