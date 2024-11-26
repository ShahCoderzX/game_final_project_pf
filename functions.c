// Libraries
#include "main.h"
#include "sdl.h"
#include "functions.h"
#include "constant.h"

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
bool checkCollisionTower(Arrow *arrow, int towerX, int towerY, int arrowWidth, int arrowHeight, int towerWidth, int towerHeight) {
    SDL_Rect arrowRect = {(int)arrow->x, (int)arrow->y, arrowWidth, arrowHeight};
    SDL_Rect towerRect = {towerX, towerY, towerWidth, towerHeight};

    return SDL_HasIntersection(&arrowRect, &towerRect);
}
// For Knight
bool checkCollisionCharacter(Character *arrow, int towerX, int towerY, int arrowWidth, int arrowHeight, int towerWidth, int towerHeight){
    SDL_Rect arrowRect = {(int)arrow->x, (int)arrow->y, arrowWidth, arrowHeight};
    SDL_Rect towerRect = {towerX, towerY, towerWidth, towerHeight};

    return SDL_HasIntersection(&arrowRect, &towerRect);
};
// Check if Tower Bomb hits the Archer
bool checkCollisionArcher(Pointer *towerbomb, Character *archer, int archerWidth, int archerHeight, int towerWidth, int towerHeight) {
    SDL_Rect arrowRect = {(int)towerbomb->x, (int)towerbomb->y, archerWidth, archerHeight};
    SDL_Rect towerRect = {((int)archer->x), (int)archer->y, towerWidth, towerHeight};

    return SDL_HasIntersection(&arrowRect, &towerRect);
}
// Check if Enemy hits the Archer
bool checkCollisionEnemy(Character *towerbomb, Character *archer, int archerWidth, int archerHeight, int towerWidth, int towerHeight) {
    SDL_Rect arrowRect = {(int)towerbomb->x, (int)towerbomb->y, archerWidth, archerHeight};
    SDL_Rect towerRect = {((int)archer->x), (int)archer->y, towerWidth, towerHeight};

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


// Function to change the variable to default
 void default_game(SDL_Rect* tower_attacker, SDL_Rect* hot_air_balloon_rect, Pointer* tower_bomb, Pointer* tower_bomb_2,Pointer* tower_bomb_3, Pointer* tower_bomb_1_5, Pointer* tower_bomb_4, Pointer* hot_air_balloon_bomb, Arrow* archer_arrow, bool* towerGame_Started_level1, bool* towerGame_Started_level2, bool* towerGame_Started_level3, bool* towerGame_Started_level4, bool* towerGame_Started_level5, bool* all_levels_Started, bool* towerGame_Started, SDL_Texture* mainBackground,bool* towerGame_levelmenu, bool*  towerGame_homemenu, int* money, int* archer_health, int* knight_health, int* mille_health, int* soldier_health, int* tower_attacker_health, int* hot_air_balloon_health, int* wolf_health, int* archer_card_cool, int* knight_card_cool, int* mille_card_cool,  int* soldier_card_cool, bool* midPause, bool* midPause_dialog_starting, bool* midPause_archer_moving, bool* frame_Video, bool* loading_screen, bool* archer_standing, int* archerY, Character* archer, Character* knight, Character* mille, Character* soldier, Character* wolf, SDL_Rect* archer_health_box, SDL_Rect* knight_health_box, SDL_Rect* soldier_health_box, SDL_Rect* mille_health_box, SDL_Rect* tower_health_box, SDL_Rect* hot_air_ballon_health_box, SDL_Rect* wolf_health_box, bool* wolf_running, bool* wolf_attacking, SDL_Rect* mille_rect, SDL_Rect* soldier_rect, SDL_Texture* towerGame_level1_background, SDL_Texture* towerGame_level2_background, SDL_Texture* towerGame_level3_background, SDL_Texture* towerGame_level4_background, SDL_Texture* towerGame_level5_background, int level3_towerWidth, int level3_towerHeight, int level4_towerWidth, int level4_towerHeight, int level5_towerWidth, int level5_towerHeight, int knight_basic_Width, int knight_basic_Height, int archer_basic_Width, int mille_basic_Width, int mille_basic_Height, int soldier_basic_Width, int soldier_basic_Height, int* milleY,float ScaleX, float ScaleY, int Windows_Width, int Windows_Height, int towerWidth, int towerHeight, int level , bool* level1_ending_scene, int* archerclicked, int* knightclicked, int* milleclicked, int* soldierclicked){
    *level1_ending_scene = false;
    *archerclicked = 1;
    *knightclicked = 0;
    *milleclicked = 0;
    *soldierclicked = 0;
    *frame_Video = true;
    *loading_screen = true;
    *towerGame_Started_level1 = false;
    *towerGame_Started_level2 = false;
    *towerGame_Started_level3 = false;
    *towerGame_Started_level4 = false;
    *towerGame_Started_level5 = false;
    *all_levels_Started = true;
    *towerGame_Started = true;
    *midPause = true;
    *midPause_dialog_starting = false;
    *midPause_archer_moving = true;
    *towerGame_levelmenu = false;
    *towerGame_homemenu = true;
    *money = 5;
    *archer_card_cool = 100;
    *knight_card_cool = 100;
    *mille_card_cool = 100;
    *soldier_card_cool = 100;
    *archer_standing = false;
    *wolf_running = false;
    *wolf_attacking = false;
    *hot_air_balloon_health = 100;
    *archer_health = 100;
    *knight_health = 100;
    *mille_health = 100;
    *soldier_health = 100;
    *tower_attacker_health = 100;
    archer_health_box->w =(Windows_Width*100)/1920;
    knight_health_box->w =(Windows_Width*100)/1920;
    mille_health_box->w = (Windows_Width*100)/1920;
    soldier_health_box->w = (Windows_Width*100)/1920;
    tower_health_box->w =(Windows_Width*1000)/1920;
    tower_attacker->w = towerWidth;
    tower_attacker->h = towerHeight;
    tower_bomb_2->active = false;
    tower_bomb->active = false;
    tower_bomb_3->active = false;
    tower_bomb_4->active = false;
    tower_bomb_1_5->active = false;
    knight->spawn = false;
    soldier->spawn = false;
    mille->spawn = false;
    wolf->spawn = false;
    tower_bomb->active = false;
    archer_arrow->active = false;
    if(level == 1){
        tower_attacker->x = (Windows_Width*100)/1920;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.26));
        tower_attacker->w = towerWidth;
        tower_attacker->h = towerHeight;                 
        *towerGame_Started_level1 = true;
        *archerY = Windows_Height - (int)((160 * ScaleY) + (330 * ScaleY));
    }else if(level == 2){
        tower_attacker->x = (Windows_Width*100)/1920;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.26));
        tower_attacker->w = towerWidth;
        tower_attacker->h = towerHeight;
        *towerGame_Started_level2 = true;
        *archerY = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        archer->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->y = Windows_Height - (int)((180 * ScaleY) + (280 * ScaleY));
        knight->x = Windows_Width - (knight_basic_Width + (Windows_Width*100)/1920);
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.23));
        hot_air_balloon_rect->x = (Windows_Width*300)/1920;
        hot_air_balloon_rect->y = (Windows_Height*400)/1080;
        hot_air_ballon_health_box->w =(Windows_Width*100)/1920;
        archer_health_box->w =(Windows_Width*100)/1920;
        knight_health_box->w =(Windows_Width*100)/1920;
        tower_health_box->w =(Windows_Width*1000)/1920;
        knight->spawn = false;
        tower_bomb->active = false;
        archer_arrow->active = false;
        archer->x = (float)(Windows_Width+(archer_basic_Width));
 

    }else if(level == 3){
        tower_attacker->x = (Windows_Width*100)/1920;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.26));
        tower_attacker->w = towerWidth;
        tower_attacker->h = towerHeight;
        *towerGame_Started_level3 = true;
        *archerY = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        archer->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->x = Windows_Width - (knight_basic_Width + (Windows_Width*100)/1920);
        mille->y = Windows_Height - (int)((110 * ScaleY) + (280 * ScaleY));
        *milleY = mille->y;
        mille->x = Windows_Width - (mille_basic_Width + (Windows_Width*100)/1920);
        mille_rect->w = (Windows_Width * (MILLE_BASIC_WIDTH/1.5))/1920;
        mille_rect->h = (Windows_Height *(MILLE_BASIC_HEIGHT/1.5))/1080;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.23));
        tower_attacker->w = level3_towerWidth;
        tower_attacker->h = level3_towerHeight;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.39));
        tower_bomb->x = (float)tower_attacker->x + (Windows_Width*50)/1920; 
        tower_bomb->y = (float)tower_attacker->y + (Windows_Height*200)/1080; 
        tower_bomb_2->x = (float)tower_attacker->x + (Windows_Width*50)/1920; 
        tower_bomb_2->y = (float)tower_attacker->y + (Windows_Height*350)/1080; 
        tower_bomb_2->active = false;
        wolf->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.08));
        wolf->x = (float) (- (WOLF_RUN_WIDTH + (Windows_Width*40)/1920));
        archer->x = (float)(Windows_Width+(archer_basic_Width));
    }else if(level == 4){
        *towerGame_Started_level4 = true;                
        *archerY = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        archer->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->x = Windows_Width - (knight_basic_Width + (Windows_Width*100)/1920);
        mille->y = Windows_Height - (int)((110 * ScaleY) + (280 * ScaleY));
        *milleY = mille->y;
        soldier->x = Windows_Width - (soldier_basic_Width + (Windows_Width*100)/1920);
        soldier->y = Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY));
        mille->x = Windows_Width - (mille_basic_Width + (Windows_Width*100)/1920);
        mille_rect->w = (Windows_Width * (MILLE_BASIC_WIDTH/1.5))/1920;
        mille_rect->h = (Windows_Height *(MILLE_BASIC_HEIGHT/1.5))/1080;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.23));
        tower_attacker->w = level4_towerWidth;
        tower_attacker->h = level4_towerHeight;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.39));
        tower_bomb->x = (float)tower_attacker->x + (Windows_Width*50)/1920; 
        tower_bomb->y = (float)tower_attacker->y + (Windows_Height*200)/1080; 
        tower_bomb_2->x = (float)tower_attacker->x + (Windows_Width*80)/1920; 
        tower_bomb_2->y = (float)tower_attacker->y + (Windows_Height*150)/1080; 
        tower_bomb_3->x = (float)tower_attacker->x + (Windows_Width*80)/1920; 
        tower_bomb_3->y = (float)tower_attacker->y + (Windows_Height*270)/1080; 
        tower_bomb_2->active = false;
        tower_bomb_3->active = false;
        archer->x = (float)(Windows_Width+(archer_basic_Width));
    }else if(level == 5){
        *towerGame_Started_level5 = true;
        *archerY = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        archer->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
        knight->x = Windows_Width - (knight_basic_Width + (Windows_Width*100)/1920);
        mille->y = Windows_Height - (int)((110 * ScaleY) + (280 * ScaleY));
        *milleY = mille->y;
        soldier->x = Windows_Width - (soldier_basic_Width + (Windows_Width*100)/1920);
        soldier->y = Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY));
        mille->x = Windows_Width - (mille_basic_Width + (Windows_Width*100)/1920);
        mille_rect->w = (Windows_Width * (MILLE_BASIC_WIDTH/1.5))/1920;
        mille_rect->h = (Windows_Height *(MILLE_BASIC_HEIGHT/1.5))/1080;
        hot_air_balloon_rect->x = (Windows_Width*200)/1920;
        hot_air_balloon_rect->y = (Windows_Height*100)/1080;
        hot_air_ballon_health_box->w =(Windows_Width*100)/1920;
        hot_air_balloon_bomb->active = false;
        wolf->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.08));
        wolf->x = (float) (- (WOLF_RUN_WIDTH + (Windows_Width*40)/1920));
        tower_attacker->w = level5_towerWidth;
        tower_attacker->h = level5_towerHeight;
        tower_attacker->y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.52));
        tower_attacker->x = 0;
        tower_bomb_1_5->x = (float)tower_attacker->x + (Windows_Width*600)/1920; 
        tower_bomb_1_5->y = (float)tower_attacker->y + (Windows_Height*200)/1080; 
        tower_bomb_2->x = (float)tower_attacker->x + (Windows_Width*500)/1920; 
        tower_bomb_2->y = (float)tower_attacker->y + (Windows_Height*350)/1080; 
        tower_bomb_3->x = (float)tower_attacker->x + (Windows_Width*500)/1920; 
        tower_bomb_3->y = (float)tower_attacker->y + (Windows_Height*150)/1080; 
        tower_bomb_4->x = (float)tower_attacker->x + (Windows_Width*300)/1920; 
        tower_bomb_4->y = (float)tower_attacker->y + (Windows_Height*300)/1080; 
        archer->x = (float)(Windows_Width+(archer_basic_Width));
    }
 }
//! ---------------------x---------------------x