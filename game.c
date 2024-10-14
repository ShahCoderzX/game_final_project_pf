// All SDL Libraries
#include "sdl.h"

// Other Needed Libraries
#include "main.h"

// Constant Created Library
#include "constant.h"

// Functions
void clear(SDL_Window* window, SDL_Renderer* renderer);
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font) ;
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
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "./image/background.jpg");

    if(!backgroundTexture){
        printf("Texture Error: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load the font
    TTF_Font* font = TTF_OpenFont("./font/font.otf", 28);
    TTF_Font* fontHeading = TTF_OpenFont("./font/font.otf", 100);
    if(!font){
        printf("Font Error: %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

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

    // Boolean
    bool running = true;
    bool gameStarted = false;
    bool homemenu = true;
    bool levelmenu = false;
    bool optionmenu = false;
    bool option_shopMenu = false;
    bool option_musicMenu = false;
    bool option_soundMenu = false;
    bool option_controllerMenu = false;

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
                if(homemenu){
                    if(checkButtonClick(mouseX, mouseY, &continueButton)){
                        printf("Continue Game");
                    }else if(checkButtonClick(mouseX, mouseY, &levelButton)){
                        levelmenu = true;
                        homemenu = false;
                    }else if(checkButtonClick(mouseX, mouseY, &optionButton)){
                        optionmenu = true;
                        homemenu = false;
                    }else if(checkButtonClick(mouseX, mouseY, &quitButton)){
                        running = false;
                    }
                }else if(levelmenu){
                    if(checkButtonClick(mouseX, mouseY, &menuHomeButton)){
                        homemenu = true;
                        levelmenu = false;
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
                }else if(optionmenu){
                    if(checkButtonClick(mouseX, mouseY, &menuHomeButton)){
                        optionmenu = false;
                        homemenu  = true;
                    }else if(checkButtonClick(mouseX, mouseY, &musicButton)){
                        optionmenu = false;
                        option_musicMenu = true;
                    }else if(checkButtonClick(mouseX, mouseY, &shopButton)){
                        optionmenu = false;
                        option_shopMenu = true;
                    }else if(checkButtonClick(mouseX, mouseY, &soundButton)){
                        optionmenu = false;
                        option_soundMenu = true;
                    }else if(checkButtonClick(mouseX, mouseY, &controllerButton)){
                        optionmenu = false;
                        option_controllerMenu = true;
                    }
                }else if(option_musicMenu){
                    if(checkButtonClick(mouseX, mouseY, &option_music_on_off_Button)){
                        music = !music;
                    }else if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                        option_musicMenu = false;
                        optionmenu = true;
                    }
                }else if(option_soundMenu){
                    if(checkButtonClick(mouseX, mouseY, &option_sound_on_off_Button)){
                        sound = !sound;
                    }else if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                        option_soundMenu = false;
                        optionmenu = true;
                    }
                }else if(option_controllerMenu){
                    if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                        option_controllerMenu = false;
                        optionmenu = true;
                    }
                }else if(option_shopMenu){
                    if(checkButtonClick(mouseX, mouseY, &option_menuHomeButton)){
                        option_shopMenu = false;
                        optionmenu = true;
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

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    if(!gameStarted && homemenu){
        SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
        // Render Button
        SDL_RenderFillRect(renderer, &continueButton);
        SDL_RenderFillRect(renderer, &levelButton);
        SDL_RenderFillRect(renderer, &optionButton);
        SDL_RenderFillRect(renderer, &quitButton);

        // Render button text
        renderText(renderer, "Continue Game", continueButton.x + 30, continueButton.y + 24, font);
        renderText(renderer, "Levels", levelButton.x + 60, levelButton.y + 24, font);
        renderText(renderer, "Options", optionButton.x + 60, optionButton.y + 24, font);
        renderText(renderer, "Quit Game", quitButton.x + 50, quitButton.y + 24, font);
    }else if(!gameStarted && levelmenu){
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
        renderText(renderer, "Back", menuHomeButton.x + 30, menuHomeButton.y + 26, font);
        renderText(renderer, "1", level1Button.x + 45, level1Button.y + 26, font);
        renderText(renderer, "2", level2Button.x + 45, level2Button.y + 26, font);
        renderText(renderer, "3", level3Button.x + 45, level3Button.y + 26, font);
        renderText(renderer, "4", level4Button.x + 45, level4Button.y + 26, font);
        renderText(renderer, "5", level5Button.x + 45, level5Button.y + 26, font);
        renderText(renderer, "6", level6Button.x + 45, level6Button.y + 26, font);
        renderText(renderer, "7", level7Button.x + 45, level7Button.y + 26, font);
        renderText(renderer, "8", level8Button.x + 45, level8Button.y + 26, font);
    }else if(!gameStarted && optionmenu){
        SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
        // Render Button
        SDL_RenderFillRect(renderer, &menuHomeButton);
        SDL_RenderFillRect(renderer, &shopButton);
        SDL_RenderFillRect(renderer, &musicButton);
        SDL_RenderFillRect(renderer, &soundButton);
        SDL_RenderFillRect(renderer, &controllerButton);

        // Render Button Text
        renderText(renderer, "Back", menuHomeButton.x + 30, menuHomeButton.y + 26, font);
        renderText(renderer, "Shop", shopButton.x + 75, shopButton.y + 26, font);
        renderText(renderer, "Music", musicButton.x + 70, musicButton.y + 26, font);
        renderText(renderer, "Sound", soundButton.x + 70, soundButton.y + 26, font);
        renderText(renderer, "Controller", controllerButton.x + 40, controllerButton.y + 26, font);
    }else if(!gameStarted && option_musicMenu){
        SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
        // Render Button
        SDL_RenderFillRect(renderer, &option_musicButton);
        SDL_RenderFillRect(renderer, &option_music_on_off_Button);
        SDL_RenderFillRect(renderer, &option_menuHomeButton);

        // Render Button Text
        renderText_Heading(renderer, "Music", (Windows_Width/2) - 100, 100, fontHeading);
        renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font);
        renderText(renderer, "Music", option_musicButton.x + 70, option_musicButton.y + 26, font);
        renderText(renderer, (music) ? "On" : "Off", option_music_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font);
    }else if(!gameStarted && option_soundMenu){
        SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
        // Render Button
        SDL_RenderFillRect(renderer, &option_soundButton);
        SDL_RenderFillRect(renderer, &option_sound_on_off_Button);
        SDL_RenderFillRect(renderer, &option_menuHomeButton);

        // Render Button Text
        renderText_Heading(renderer, "Sound", (Windows_Width/2) - 100, 100, fontHeading);
        renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font);
        renderText(renderer, "Sound", option_soundButton.x + 70, option_soundButton.y + 26, font);
        renderText(renderer, (sound) ? "On" : "Off", option_sound_on_off_Button.x + 80, option_music_on_off_Button.y + 26, font);
    }else if(!gameStarted && option_controllerMenu){
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
        renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font);
        renderText_Heading(renderer, "Controller", (Windows_Width/2) - 200, 100, fontHeading);

        renderText(renderer, "W", option_controllerButton_1.x + 30, option_controllerButton_1.y + 26, font);
        renderText(renderer, "Forward", option_controllerButton_1.x + 220, option_controllerButton_1.y + 26, font);
        renderText(renderer, "S", option_controllerButton_2.x + 30, option_controllerButton_2.y + 26, font);
        renderText(renderer, "Backward", option_controllerButton_2.x + 220, option_controllerButton_2.y + 26, font);
        renderText(renderer, "L", option_controllerButton_3.x + 30, option_controllerButton_3.y + 26, font);
        renderText(renderer, "Left", option_controllerButton_3.x + 220, option_controllerButton_3.y + 26, font);
        renderText(renderer, "R", option_controllerButton_4.x + 30, option_controllerButton_4.y + 26, font);
        renderText(renderer, "Right", option_controllerButton_4.x + 220, option_controllerButton_4.y + 26, font);

        renderText(renderer, "F", option_controllerButton_5.x + 30, option_controllerButton_5.y + 26, font);
        renderText(renderer, "Attack", option_controllerButton_5.x + 220, option_controllerButton_5.y + 26, font);
        renderText(renderer, "H", option_controllerButton_6.x + 30, option_controllerButton_6.y + 26, font);
        renderText(renderer, "Heal", option_controllerButton_6.x + 220, option_controllerButton_6.y + 26, font);
        renderText(renderer, "Space", option_controllerButton_7.x + 30, option_controllerButton_7.y + 26, font);
        renderText(renderer, "Jump", option_controllerButton_7.x + 220, option_controllerButton_7.y + 26, font);
        renderText(renderer, "Ctrl", option_controllerButton_8.x + 30, option_controllerButton_8.y + 26, font);
        renderText(renderer, "Crouch", option_controllerButton_8.x + 220, option_controllerButton_8.y + 26, font);

    }else if(!gameStarted && option_shopMenu){
        SDL_SetRenderDrawColor(renderer,0, 0, 0, 1);
        SDL_RenderFillRect(renderer, &option_menuHomeButton);
        // Render Button Text
        renderText(renderer, "Back", option_menuHomeButton.x + 30, option_menuHomeButton.y + 26, font);
        renderText_Heading(renderer, "Shop", (Windows_Width/2) - 90, 100, fontHeading);
        renderText(renderer, "Shop is Empty", (Windows_Width/2) - 80, 300, font);

    }
    SDL_RenderPresent(renderer);
}



    clear(window, renderer);
    

}

// Functions
void clear(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// Check for button click
bool checkButtonClick(int mouseX, int mouseY, SDL_Rect* button) {
    return mouseX >= button->x && mouseX <= button->x + button->w &&
           mouseY >= button->y && mouseY <= button->y + button->h;
}

// RenderText
void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font) {
    SDL_Color color = { 255, 255, 255, 255 }; // White color
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