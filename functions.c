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
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        return; // Exit if the surface could not be created
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return; // Exit if the texture could not be created
    }

    // Define a rectangle for the position of the text
    SDL_Rect dstrect = {x, y, surface->w, surface->h};

    // Render the texture
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    // Clean up
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

// ! Tower Game
// Check if arrow hits the tower
bool checkCollisionTower(Arrow *arrow, int towerX, int towerY) {
    SDL_Rect arrowRect = {(int)arrow->x, (int)arrow->y, 120, 50};
    SDL_Rect towerRect = {towerX, towerY, 100, 300};

    return SDL_HasIntersection(&arrowRect, &towerRect);
}
// Check if Tower Bomb hits the Archer
bool checkCollisionArcher(Pointer *towerbomb, Pointer *archer) {
    SDL_Rect arrowRect = {(int)towerbomb->x, (int)towerbomb->y, 200, 50};
    SDL_Rect towerRect = {((int)archer->x), (int)archer->y, 100, 40};

    return SDL_HasIntersection(&arrowRect, &towerRect);
}

// UserName Exist or not 
int usernameExists(const char* username) {
    FILE *file = fopen("accounts.txt", "r");
    Account account;

    if (file == NULL) {
        return 0; 
    }

    while (fscanf(file, "%s %s %f", account.username, account.password, &account.money) != EOF) {
        if (strcmp(account.username, username) == 0) {
            fclose(file);
            return 1; // Username already exists
        }
    }

    fclose(file);
    return 0; // Username does not exist
}
// Create Account
int createAccount(char* username, char* password) {
    Account newAccount;
    FILE *file = fopen("accounts.txt", "a");

    if (file == NULL) {
        return 0;
    }

    strcpy(newAccount.username,username);
    strcpy(newAccount.password,password);
    newAccount.money = 100;

    // fprintf(file, "%s %s %d\n", newAccount.username, newAccount.password, newAccount.money);
    fwrite(&newAccount, sizeof(Account), 1, file);
    fclose(file);

    return 1;
}
// Login Account
int login(char* username, char* password) {
    Account account;
    int found = 0, account_money = 0;
    FILE *file = fopen("accounts.txt", "r");

    if (file == NULL) {
        return 0;
    }

    // Read account details from text file using fscanf
    // fread(&account, sizeof(Account), 1, file)
    // fscanf(file, "%s %s %d", account.username, account.password, &account.money) != EOF
    while (fread(&account, sizeof(Account), 1, file)) {
        if (strcmp(account.username, username) == 0 && strcmp(account.password, password) == 0) {
            found = 1;
            account_money = account.money;
            break;
        }
    }

    if (!found) {
        return 0;
    }

    fclose(file);
    return account_money;
}

// Set up sprite sheet frames
void setupSpriteFrames(SDL_Rect* clips, int totalFrames, int frameWidth, int frameHeight) {
    for (int i = 0; i < totalFrames; i++) {
        clips[i].x = i * frameWidth;
        clips[i].y = 0;
        clips[i].w = frameWidth;
        clips[i].h = frameHeight;
    }
}

//! ---------------------x---------------------x