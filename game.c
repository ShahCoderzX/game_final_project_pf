// All SDL Libraries
#include "sdl.h"

// Other Needed Libraries
#include "main.h"

// Constant Created Library
#include "constant.h"
// Own Functions Created Library
#include "functions.h"

// Function to play video (plays once and stops after the video ends)
int playVideo(SDL_Window* window, SDL_Renderer* renderer, const char* audioPath, const char* framePath, int frameCount, int skipFrames, int targetFPS, int finalcut) {
    Mix_Chunk* framesound = Mix_LoadWAV(audioPath); 
    if(!(strcmp(audioPath, "none") == 0)){
        if (!framesound) return 0;  // Return false if music fails to load
    }
    Mix_PlayChannel(0, framesound, 0);


    SDL_Texture* frameTexture = NULL;
    SDL_Event event;
    int frameIndex = 0;
    int faltoIndex = frameIndex;
    Uint32 frameStart, frameDuration = 1000 / targetFPS;

    char fullframePath[256];
    Uint32 lastFrameTime = SDL_GetTicks();
    Uint32 lastAudioTime = 0;

    // Main loop for rendering frames and playing music
    while (1) {
        frameStart = SDL_GetTicks();

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && !(strcmp(audioPath, "none") == 0)) {
                Mix_HaltChannel(0);
                Mix_FreeChunk(framesound);
                SDL_DestroyTexture(frameTexture);
                return 0;
            }
        }

        // Construct the image path dynamically for each frame
        snprintf(fullframePath, sizeof(fullframePath), framePath, frameIndex + 1);

        // Load the current frame texture
        SDL_DestroyTexture(frameTexture); // Free the previous frame texture
        frameTexture = IMG_LoadTexture(renderer, fullframePath);
        if (!frameTexture) {
            printf("IMG_LoadTexture failed for %s: %s\n", fullframePath, IMG_GetError());
            break;
        }

        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the current frame
        SDL_RenderCopy(renderer, frameTexture, NULL, NULL);

        // Present the rendered frame
        SDL_RenderPresent(renderer);

        // Calculate the time it took to render the frame
        int frameTime = SDL_GetTicks() - frameStart;


        if (frameTime < frameDuration) {
            SDL_Delay(frameDuration - frameTime); // Delay to match frame rate
        }

        if (++frameIndex % skipFrames == 0) {
            frameIndex = (frameIndex + 1) % frameCount;
        }


         // Skip frames as necessary
        // Increment the frameIndex, making sure it stays within bounds
        if (frameIndex >= frameCount-finalcut) {
            // Once the last frame is reached, stop the video
            break;
        }

    }
    // Stop the music and cleanup after video ends
    Mix_HaltChannel(0);
    Mix_FreeChunk(framesound);
    SDL_DestroyTexture(frameTexture);
    return 0; // Return false when video ends
}

int playVideoCredit(SDL_Window* window, SDL_Renderer* renderer, const char* audioPath, const char* framePath, 
              int frameCount, int skipFrames, int targetFPS, int finalcut, int fm) {
    Mix_Chunk* framesound = NULL;

    // Load audio if specified
    if (strcmp(audioPath, "none") != 0) {
        framesound = Mix_LoadWAV(audioPath);
        if (!framesound) {
            printf("Failed to load audio: %s\n", Mix_GetError());
            return 0; // Return false if audio loading fails
        }
        Mix_PlayChannel(0, framesound, 0); // Play audio on channel 0
    }

    SDL_Texture* frameTexture = NULL;
    SDL_Event event;
    int frameIndex = 0; // Start from the first frame
    Uint32 frameStart, frameDuration = 1000 / targetFPS; // Calculate duration per frame in milliseconds

    char fullframePath[256];

    // Main loop for rendering frames
    while (1) {
        frameStart = SDL_GetTicks();

        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                // Stop audio and clean up on RETURN key press
                if (framesound) {
                    Mix_HaltChannel(0);
                    Mix_FreeChunk(framesound);
                }
                SDL_DestroyTexture(frameTexture);
                return 0; // Exit the function
            }
        }

        // Construct the image path for the current frame
        snprintf(fullframePath, sizeof(fullframePath), framePath, frameIndex + 1);

        // Load the current frame texture
        SDL_DestroyTexture(frameTexture); // Free previous texture to avoid memory leaks
        frameTexture = IMG_LoadTexture(renderer, fullframePath);
        if (!frameTexture) {
            printf("Failed to load frame %s: %s\n", fullframePath, IMG_GetError());
            break; // Exit loop if texture loading fails
        }

        // Clear the renderer and render the current frame
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frameTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Calculate frame rendering time
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDuration) {
            SDL_Delay(frameDuration - frameTime); // Delay to maintain target FPS
        }

        // Increment frame index, ensuring it doesn't exceed the valid range
        frameIndex++;
        if(fm == 1){
            if (frameIndex >= 577) {
                break; // Exit once the last frame is reached
            }
        }else if(fm == 2){
            if (frameIndex >= 0175) {
                break; // Exit once the last frame is reached
            }
        }
    }

    // Clean up resources after playback
    if (framesound) {
        Mix_HaltChannel(0);
        Mix_FreeChunk(framesound);
    }
    SDL_DestroyTexture(frameTexture);

    return 1; // Return success
}


int main(int argc, char* args[]){

    int level = 1;

    /* ==================================================
                ! User Screen Width and Height
    =====================================================*/
    int Windows_Width, Windows_Height;
    /* ==================================================
                ! x-----------x----------x
    =====================================================*/


    /* ==================================================
            ! Check SDl_Init and Other things
    =====================================================*/
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

    // Mix Audio Check
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    /* ==================================================
                ! x-----------x----------x
    =====================================================*/


    /* ==================================================
                    ! Declare All Audios
    =====================================================*/
    // All Audios
    Mix_Chunk *button_sound = Mix_LoadWAV("./audio/button_sound.mp3");
    Mix_Chunk *towerGame_backgroundMusic = Mix_LoadWAV("./audio/deckofdominions/background.mp3");
    Mix_Chunk *typing_SoundEffect = Mix_LoadWAV("./audio/deckofdominions/typing.mp3");
    
    Mix_Chunk *archerDamage_sound = Mix_LoadWAV("./audio/deckofdominions/archer_damage.mp3");
    Mix_Chunk *towerDamage_sound = Mix_LoadWAV("./audio/deckofdominions/tower_damage.mp3");
    Mix_Chunk *towerShooting_sound = Mix_LoadWAV("./audio/deckofdominions/tower_shooting.mp3");
    Mix_Chunk *archerShooting_sound = Mix_LoadWAV("./audio/deckofdominions/archer_shooting.mp3");

    Mix_Chunk *level1Background_Sound = Mix_LoadWAV("./audio/deckofdominions/level1_Background.mp3");

     /* ==================================================
                ! x-----------x----------x
    =====================================================*/


    /* ==================================================
    ! Window Resolution check and create scaleX and scaleY
    =====================================================*/
    // Give The Values of Windows Resoltuion
    SDL_Rect screenDisplay;
    if(SDL_GetDisplayBounds(0, &screenDisplay) == 0){
        // if(screenDisplay.w >= 1920 && screenDisplay.h >= 1080){
        //     Windows_Width = 1920;
        //     Windows_Height = 1080;
        // }else if(screenDisplay.w >= 1280 && screenDisplay.h >= 720){
        //     Windows_Width = 1280;
        //     Windows_Height = 720;
        // }else if(screenDisplay.w >= 1024 && screenDisplay.h <= 768){
        //     Windows_Width = 1024;
        //     Windows_Height = 768;
        // }else if(screenDisplay.w >= 800 && screenDisplay.h <= 600){
        //     Windows_Width = 800;
        //     Windows_Height = 600;
        // }else{
        //     Windows_Width = screenDisplay.w;
        //     Windows_Height = screenDisplay.h;
        // }
        Windows_Width = screenDisplay.w;
        Windows_Height = screenDisplay.h;
    }

    // Calculate scaling factors based on the original resolution
    float ScaleX = (float)Windows_Width / 1920;
    float ScaleY = (float)Windows_Height / 1080;

    /* ==================================================
                ! x-----------x----------x
    =====================================================*/


    /* ==================================================
    ! Create SDL_Window and SDL_renderer and Check Both
    =====================================================*/
    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow(
        "Impact Gamer", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        Windows_Width, Windows_Height, SDL_WINDOW_SHOWN
    );

    // Set the window to fullscreen mode
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); 

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

    /* ==================================================
                ! x-----------x----------x
    =====================================================*/

    /* ==================================================
                        ! All Textures
    =====================================================*/

    // Main Screen Button Images
    SDL_Texture* select_menu_playDeck_button = IMG_LoadTexture(renderer, "./image/playdeck.png");
    SDL_Texture* select_menu_quit_button = IMG_LoadTexture(renderer, "./image/quitselect.png");

    // Account Buttons Images
    SDL_Texture* account_create_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/create.png");
    SDL_Texture* account_login_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/login.png");

    // All Background Images For Main Game
    // ? Main Tower Background
    SDL_Texture* main_backgroundTexture = IMG_LoadTexture(renderer, "./image/mainbackground.png");
    // ? Tower Game All Menus Section 
    SDL_Texture* towerGame_homePage_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_background.png");
    SDL_Texture* towerGame_option_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_option_background.png");
    SDL_Texture* towerGame_level_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_level_background.png");
    SDL_Texture* towerGame_shop_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_shop_background.png");
    SDL_Texture* towerGame_sound_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_sound_background.png");
    SDL_Texture* towerGame_music_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_music_background.png");
    SDL_Texture* towerGame_control_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_control_background.png");
    // ? Press Enter background Images
    SDL_Texture* towerGame_mainPage_backgroundTexture = IMG_LoadTexture(renderer, "./image/main_tower_background.png");
    //? All levels Background Images
    SDL_Texture* towerGame_level1_background = IMG_LoadTexture(renderer, "./image/level1_background.jpg");
    SDL_Texture* towerGame_level2_background = IMG_LoadTexture(renderer, "./image/level2_background.jpg");
    SDL_Texture* towerGame_level3_background = IMG_LoadTexture(renderer, "./image/level3_background.jpg");
    SDL_Texture* towerGame_level4_background = IMG_LoadTexture(renderer, "./image/level4_background.jpeg");
    SDL_Texture* towerGame_level5_background = IMG_LoadTexture(renderer, "./image/level5_background.png");

    // TowerGame Menu Page Buttons Images
    SDL_Texture* start_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/start.png");
    SDL_Texture* continue_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/continue.png");
    SDL_Texture* level_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level.png");
    SDL_Texture* option_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/option.png");
    SDL_Texture* control_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/controls.png");
    SDL_Texture* quit_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/quit.png");
    SDL_Texture* back_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/back.png"); 

    // TowerGame Level Page Buttons Images
    SDL_Texture* level1_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level1.png");
    SDL_Texture* level2_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level2.png");
    SDL_Texture* level3_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level3.png");
    SDL_Texture* level4_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level4.png");
    SDL_Texture* level5_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level5.png");

    // TowerGame Option Page Button Images
    SDL_Texture* shop_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/shop.png");
    SDL_Texture* sound_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound.png");
    SDL_Texture* music_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/music.png");
    SDL_Texture* on_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/on.png");
    SDL_Texture* off_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/off.png");

    // TowerGame PauseSection Buttons Images
    SDL_Texture* resumeButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/resume.png");
    SDL_Texture* restartButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/restart.png");
    SDL_Texture* homeButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/home.png");
    SDL_Texture* pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/pause.png");
    SDL_Texture* sound_on_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound_on_pause.png");
    SDL_Texture* sound_off_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound_off_pause.png");
    SDL_Texture* music_on_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/music_on_pause.png");
    SDL_Texture* music_off_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/music_off_pause.png");

    // Game Over Section Buttons Images
    SDL_Texture* retryButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/retry.png");

    // You Win Section Buttons Images
    SDL_Texture* nextlevelButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/nextlevel.png");

    // Arrow Images
    SDL_Texture* arrow = IMG_LoadTexture(renderer, "./image/arrow.png");
    // Archer Image
    SDL_Texture* archer_standing_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_standing.png");
    SDL_Texture* archer_walking_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_walking.png");
    SDL_Texture* archer_shooting_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_shooting.png");

    // Tower Image
    // ? Level1
    SDL_Texture* level1_tower_fullhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level1/fullhealth.png");
    SDL_Texture* level1_tower_halfhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level1/halfhealth.png");
    SDL_Texture* level1_tower_zerohealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level1/zerohealth.png");

    // ? Level2
    SDL_Texture* level2_tower_fullhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level2/fullhealth.png");
    SDL_Texture* level2_tower_halfhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level2/halfhealth.png");
    SDL_Texture* level2_tower_zerohealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level2/zerohealth.png");

    // ? Level3
    SDL_Texture* level3_tower_fullhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level3/fullhealth.png");
    SDL_Texture* level3_tower_halfhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level3/halfhealth.png");
    SDL_Texture* level3_tower_zerohealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level3/zerohealth.png");

    // ? Level 4
    SDL_Texture* level4_tower_fullhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level4/fullhealth.png");
    SDL_Texture* level4_tower_halfhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level4/halfhealth.png");
    SDL_Texture* level4_tower_zerohealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level4/zerohealth.png");

    // ? Level 5
    SDL_Texture* level5_tower_fullhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level5/fullhealth.png");
    SDL_Texture* level5_tower_halfhealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level5/halfhealth.png");
    SDL_Texture* level5_tower_zerohealth_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/level5/zerohealth.png");

    // Connon ball Image
    SDL_Texture* cannon_ball_texture=IMG_LoadTexture(renderer, "./image/towerGame/towers/cannon_ball.png");

    // Hot Air Balloon Image 
    SDL_Texture* hot_air_bombing_balloon_texture = IMG_LoadTexture(renderer, "./image/towerGame/towers/hairballon/hotairballoonBombing.png");
    SDL_Texture* hot_air_balloon_texture = IMG_LoadTexture(renderer, "./image/towerGame/towers/hairballon/hotairballoon.png");
    SDL_Texture* hot_air_balloon_bomb_texture = IMG_LoadTexture(renderer, "./image/towerGame/towers/hairballon/bomb.png");
    SDL_Texture* hot_air_balloon_bomb_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/towers/hairballon/bomb_spritesheet.png");

    // Cards
    //? Archer
    SDL_Texture* card_archer_full_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/archer_full.png");
    SDL_Texture* card_archer_notfull_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/archer_notfull.png");
    //? Knight
    SDL_Texture* card_knight_full_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/knight_full.png");
    SDL_Texture* card_knight_notfull_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/knight_notfull.png");
    //? Mille
    SDL_Texture* card_mille_full_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/mille_full.png");
    SDL_Texture* card_mille_notfull_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/mille_notfull.png");
    //? Soldier
    SDL_Texture* card_soldier_full_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/soldier_full.png");
    SDL_Texture* card_soldier_notfull_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/soldier_notfull.png");

    // Knight Character
    SDL_Texture* knight_running_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/characters/knight/run.png");
    SDL_Texture* knight_running_attacking_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/characters/knight/run_attack.png");
    SDL_Texture* knight_standing_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/characters/knight/standing.png");

    // Wolf Character
    SDL_Texture* wolf_running_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/characters/wolf/running.png");
    SDL_Texture* wolf_attacking_spritesheet = IMG_LoadTexture(renderer, "./image/towerGame/characters/wolf/attacking.png");

    // Mille 
    SDL_Texture* mille_standing_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/mille/standing.png");
    SDL_Texture* mille_standing_spritesheet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/mille/mille_standing.png");
    SDL_Texture* mille_walking_spritesheet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/mille/mille_walking.png");
    SDL_Texture* mille_attacking_spritesheet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/mille/mille_attacking.png");
    SDL_Texture* mille_laser_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/mille/laser.png");

    // Soldier 
    SDL_Texture* soldier_walking_spritesheet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/soldier/walking.png");
    SDL_Texture* soldier_attacking_spritesheet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/soldier/shooting.png");
    SDL_Texture* soldier_bullet_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/soldier/bullet.png");

    // Dialog Images
    // ? For Level1 Starting
    SDL_Texture* level1_archer_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level1/archer_dialog1.png");
    SDL_Texture* level1_king_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level1/king_dialog1.png");
    // ? For Level1 Ending
    SDL_Texture* level1_ending_archer_dialog1 = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level1/ending_archer_dialog1.png");
    SDL_Texture* level1_ending_knight_dialog1 = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level1/ending_knight_dialog1.png");

    // ? For Level2 Starting
    SDL_Texture* level2_king_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/king_dialog1.png");
    SDL_Texture* level2_king_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/king_dialog2.png");
    SDL_Texture* level2_archer_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/archer_dialog1.png");
    SDL_Texture* level2_archer_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/archer_dialog2.png");
    SDL_Texture* level2_mille_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/mille_dialog1.png");

    // ? For Level2 Ending
    SDL_Texture* level2_ending_archer_dialog3_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/archer_dialog3.png");
    SDL_Texture* level2_ending_mille_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level2/mille_dialog2.png");

    // ? For Level3 Starting
    SDL_Texture* level3_knight_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level3/knight_dialog1.png");
    SDL_Texture* level3_archer_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level3/archer_dialog1.png");
    SDL_Texture* level3_mille_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level3/mille_dialog1.png");
    SDL_Texture* level3_archer_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level3/archer_dialog2.png");

    // ? For Level4 Starting
    SDL_Texture* level4_archer_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level4/archer_dialog1.png");
    SDL_Texture* level4_soldier_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level4/soldier_dialog1.png");
    SDL_Texture* level4_knight_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level4/knight_dialog1.png");
    SDL_Texture* level4_mille_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level4/mille_dialog1.png");

    // ? For Level5 Starting
    SDL_Texture* level5_king_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level5/knig_dialog1.png");
    SDL_Texture* level5_archer_dialog1_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level5/archer_dialog1.png");
    SDL_Texture* level5_king_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level5/king_dialog2.png");
    SDL_Texture* level5_archer_dialog2_texture = IMG_LoadTexture(renderer, "./image/towerGame/dialog/level5/archer_dialog2.png");

    /* ==================================================
            ! x-----------------x--------------x
    =====================================================*/


    /* ==================================================
            ! Buttons Width And Height Defines
    =====================================================*/
    // Main Page Buttons Width and Height
    const int select_button_Width = (Windows_Width*300)/1920;
    const int select_button_Height = (Windows_Width*80)/1080;

    // Tower Game Pages Buttons Width and Height
    const int tower_menu_button_width = (Windows_Width * 220) / 1920; 
    const int tower_menu_button_height = (Windows_Height * 74) / 1080; 

    // Account Menu Buttons Width And Height
    const int account_main_Width = 800;
    const int account_main_Height = 600;

    // Buttons for Front Pages of Twoer Game
    //? Calculate the total height of all buttons and spacing
    const int total_buttons_height = (tower_menu_button_height * 4) + (Windows_Height * 16 / 1080 * 3); // 4 buttons + 3 spacings
    const int starting_y_position = (Windows_Height / 2) - (total_buttons_height / 2); // Calculate the starting y position for vertical centering
    const int center_button_horizontally = (Windows_Width / 2) - (tower_menu_button_width / 2);

    // Tower Game Level Page Buttons Width and Height
    const int tower_menu_level_button_Width = (Windows_Width*110)/1920;
    const int tower_menu_level_button_Height = (Windows_Height*70)/1080;

    // Tower Game Menus Buttons Width and Height
    const int small_pause_button_Width = (Windows_Width*74)/1920;
    const int small_pause_button_Height = (Windows_Height*74)/1080;
    const int normal_pause_button_Width = (Windows_Width*200)/1920;
    const int normal_pause_button_Height = (Windows_Height*74)/1080;

    /* ==================================================
            ! x----------------x--------------x
    =====================================================*/
    
    /* ==================================================
            ! Others Width And Height Defines
    =====================================================*/
    // Cards Width and Height
    const int cards_Width = (Windows_Width*114)/1920;
    const int cards_Height = (Windows_Height*144)/1080;

    // Game Over Screen Height
    const int gameOver_screen_Height = (Windows_Height*400)/1080;

    // You Win Screen Height
    const int youWin_screen_Height = (Windows_Height*400)/1080;

    // Pause Screen Width and Height
    const int pause_screen_Width = (Windows_Width*500)/1920;
    const int pause_screen_Height = (Windows_Height*600)/1080;

    // Account Input Box Width And Height
    const int account_input_Width = 600;
    const int account_input_Height = 50;
    
    // Dialog Width and Height
    const int dialog_Width = (Windows_Width*356)/1920;
    const int dialog_Height = (Windows_Height*136)/1080;

    // Velocity Bar for arrow Width and Height
    const int velocity_bar_Width = (Windows_Width*10)/1920;
    const int velocity_bar_Height = (Windows_Height*180)/1080;
    
    // Tower Width and Height
    const int towerWidth = (Windows_Width*161)/1920;
    const int towerHeight = (Windows_Height*300)/1080;
    const int level3_towerWidth = (Windows_Width*139)/1920;
    const int level3_towerHeight = (Windows_Height*477)/1080;
    const int level4_towerWidth = (Windows_Width*360)/1920;
    const int level4_towerHeight = (Windows_Height*477)/1080;
    const int level5_towerWidth = (Windows_Width*722)/1920;
    const int level5_towerHeight = (Windows_Height*612)/1080;

    // Archer Width and height
    int archer_basic_Width = (Windows_Width * ARCHER_BASIC_FRAME_WIDTH) / 1920;
    int archer_basic_Height = (Windows_Height * ARCHER_BASIC_FRAME_HEIGHT) / 1080;
    int archer_fire_Width = (Windows_Height * ARCHER_FIRE_FRAME_WIDTH) / 1080;
    int archer_fire_Height = (Windows_Height * ARCHER_BASIC_FRAME_HEIGHT) / 1080;
    int archerX = Windows_Width - (int)((200 * ScaleX) + (100 * ScaleY));
    int archerY = Windows_Height - (int)((160 * ScaleY) + (330 * ScaleY));

    // Archer Arrow
    const int archer_arrow_basic_Width = (Windows_Width*120)/1920;
    const int archer_arrow_basic_Height = (Windows_Height*50)/1080;

    // Knight Width and Height
    int knight_basic_Width = ((Windows_Width * 170) / 1920);
    int knight_basic_Height = ((Windows_Height * 204) / 1080);

    // Wolf Width and Height
    int wolf_basic_Width = (Windows_Width*180)/1920;
    int wolf_basic_Height = (Windows_Height*135)/1080;

    // Mille Width and Height
    int mille_basic_Width = (Windows_Width*90)/1920;
    int mille_basic_Height = (Windows_Height*140)/1080;

    // Soldier Width and Height
    int soldier_basic_Width = (Windows_Width * SOLDIER_BASIC_WIDTH)/1920;
    int soldier_basic_Height = (Windows_Height * SOLDIER_BASIC_HEIGHT)/1080;
    // Soldier Bullet Width and Height
    int soldier_bullet_Width = (Windows_Width*100)/1920;
    int soldier_bullet_Height = (Windows_Height*14)/1080;

    // Tower Health box Width and Height
    const int tower_health_box_Width = (Windows_Width*1000)/1920;
    const int tower_health_box_Height = (Windows_Height*30)/1080;

    // Character Health Box Width and Height
    const int character_health_Width = (Windows_Width*100)/1920;
    const int character_health_Height = (Windows_Height*10)/1080;

    // Define spacing and button dimensions
    const int buttonSpacing = (Windows_Height*20)/1080;  

     /* ==================================================
            ! x---------------x---------------x
    =====================================================*/


    /* ==================================================
                    ! SDL _ RECT
    =====================================================*/

    /* ==================================================
                    ? Main Page
    =====================================================*/

    // Main Page Buttons Rect
    SDL_Rect select_menu_playDeck_button_rect = {(Windows_Width/2)-(select_button_Width)/2, (Windows_Height/2)-(select_button_Height/2)-(Windows_Height*60)/1080, select_button_Width, select_button_Height};
    SDL_Rect select_menu_quit_button_rect = {(Windows_Width/2)-(select_button_Width)/2, select_menu_playDeck_button_rect.y + select_button_Height + (Windows_Height*30)/1080, select_button_Width, select_button_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
                    ? Account Page
    =====================================================*/

    // Account Menu Rect
    SDL_Rect account_main_rect = {(Windows_Width/2)-(account_main_Width/2), (Windows_Height/2)-(account_main_Height/2), account_main_Width, account_main_Height};

    // Account Input Box Rect
    SDL_Rect account_username = {(account_main_rect.x)+100, account_main_rect.y+220, account_input_Width, account_input_Height};
    SDL_Rect account_password = {(account_main_rect.x)+100, account_username.y+140, account_input_Width, account_input_Height};

    // Account Buttons Rect
    //? Buttons For Create Account
    SDL_Rect account_create_button1_rect = {account_main_rect.x + (account_main_rect.w/2 - tower_menu_button_width/
    2), account_main_rect.y + (account_main_rect.h - 150), tower_menu_button_width, tower_menu_button_height};
    SDL_Rect account_login_button1_rect = {account_main_rect.x + (account_main_rect.w/2 - tower_menu_button_width/
    2), account_main_rect.y + (account_main_rect.h - 150), tower_menu_button_width, tower_menu_button_height};
    //? Buttons for login Account
    SDL_Rect account_create_button_rect = {account_main_rect.x+(account_main_rect.w/2 - (tower_menu_button_width*2+20)/2), account_main_rect.y + (account_main_rect.h/2 - tower_menu_button_height/2), tower_menu_button_width, tower_menu_button_height};
    SDL_Rect account_login_button_rect = {account_create_button_rect.x+tower_menu_button_width+20 , account_main_rect.y + (account_main_rect.h/2 - tower_menu_button_height/2), tower_menu_button_width, tower_menu_button_height};

    // Typwriter Blinker for Account Input
    SDL_Rect account_username_typewriter = {account_username.x+10, account_username.y + 4, 4, 42};
    SDL_Rect account_password_typewriter = {account_password.x+10, account_password.y + 4, 4, 42};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
                    ? Dialog
    =====================================================*/

    // Level1 Dialog 
    // ? Starting Dialog
    SDL_Rect level1_archer_dialog1_rect = {((Windows_Width-dialog_Width)-((Windows_Width*100)/1920)), ((Windows_Height-dialog_Height)-((Windows_Height*500)/1920)), dialog_Width, dialog_Height};
    SDL_Rect level1_king_dialog1_rect = {(Windows_Width*100)/1920, ((Windows_Height-dialog_Height)-((Windows_Height*500)/1920)), dialog_Width, dialog_Height};
       
    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Main Page Button Rect
    =====================================================*/
    SDL_Rect continue_button_rect = {center_button_horizontally, starting_y_position, tower_menu_button_width, tower_menu_button_height};

    SDL_Rect level_button_rect = {center_button_horizontally, continue_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};

    SDL_Rect option_button_rect = {center_button_horizontally, level_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};

    SDL_Rect quit_button_rect = {center_button_horizontally,option_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Level Page Button Rect
    =====================================================*/

    SDL_Rect level1_button_rect = {(Windows_Width*720)/1920, starting_y_position, tower_menu_level_button_Width, tower_menu_level_button_Height};
    SDL_Rect level2_button_rect = {level1_button_rect.x + (Windows_Width*130)/1920, starting_y_position, tower_menu_level_button_Width, tower_menu_level_button_Height};
    SDL_Rect level3_button_rect = {level2_button_rect.x + (Windows_Width*130)/1920, starting_y_position, tower_menu_level_button_Width, tower_menu_level_button_Height};
    SDL_Rect level4_button_rect = {level3_button_rect.x + (Windows_Width*130)/1920, starting_y_position, tower_menu_level_button_Width, tower_menu_level_button_Height};
    SDL_Rect level5_button_rect = {(Windows_Width*720)/1920, starting_y_position + (Windows_Height*100)/1080, tower_menu_level_button_Width, tower_menu_level_button_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Back Button Rect
    =====================================================*/

    SDL_Rect back_button_rect = {(Windows_Width*100)/1920, continue_button_rect.y - (tower_menu_button_height + (Windows_Height * 40) / 1080), tower_menu_button_width,tower_menu_button_height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Option Page Button Rect
    =====================================================*/
    // Option Buttons
    SDL_Rect shop_button_rect = {center_button_horizontally, starting_y_position, tower_menu_button_width,tower_menu_button_height};

    SDL_Rect music_button_rect = {center_button_horizontally,shop_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};

    SDL_Rect sound_button_rect = {center_button_horizontally, music_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};

    SDL_Rect control_button_rect = {center_button_horizontally, sound_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, tower_menu_button_width,tower_menu_button_height};
    
    SDL_Rect music_on_off_button_rect = {(Windows_Width*750)/1920, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect sound_on_off_button_rect = {(Windows_Width*750)/1920, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect on_button_rect = {(Windows_Width*770)/1920 + tower_menu_button_width, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect off_button_rect = {(Windows_Width*770)/1920 + tower_menu_button_width, starting_y_position, tower_menu_button_width, tower_menu_button_height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Characters Card Rect
    =====================================================*/
    // Archer Card
    SDL_Rect card_archer_rect= {(Windows_Width/2)-(cards_Width/2), (Windows_Height-cards_Height)-(archer_arrow_basic_Height), cards_Width, cards_Height};
    // Knight Card 
    SDL_Rect card_knight_rect= {(Windows_Width*700)/1920, (Windows_Height-cards_Height)-(archer_arrow_basic_Height), cards_Width, cards_Height};
    // Mille Card Rect
    SDL_Rect card_mille_rect= {(Windows_Width*500)/1920, (Windows_Height-cards_Height)-(archer_arrow_basic_Height), cards_Width, cards_Height};
    // Soldier Card Rect
    SDL_Rect card_soldier_rect= {(Windows_Width*1100)/1920, (Windows_Height-cards_Height)-(archer_arrow_basic_Height), cards_Width, cards_Height};
    // Background Screen of Cards
    SDL_Rect cardsBackground_rect = {0, Windows_Height - ((Windows_Height*240)/1080), Windows_Width, (Windows_Height*240)/1080};
    SDL_Rect cardsBackground_outline_rect = {0, cardsBackground_rect.y, Windows_Width, 0};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game Pause Screen Rect
    =====================================================*/
    
    // Pause Screen Background
    SDL_Rect pause_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect pause_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - pause_screen_Height) / 2, pause_screen_Width, pause_screen_Height};
    // Pause Button
    SDL_Rect pauseButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/


    /* ==================================================
            ? Tower Game Game Over Screen Rect
    =====================================================*/
    
    // Game Over Screen Background
    SDL_Rect gameOver_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect gameOver_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - gameOver_screen_Height) / 2, pause_screen_Width, gameOver_screen_Height};
    // Game Over Button
    SDL_Rect gameOverButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    SDL_Rect retryButton_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  pause_screen.y + (Windows_Height*250)/1080,  normal_pause_button_Width,normal_pause_button_Height};

    SDL_Rect homeButton_gameOver_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,retryButton_rect.y + normal_pause_button_Height + buttonSpacing,normal_pause_button_Width,normal_pause_button_Height};

    SDL_Rect nextlevelButton_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  pause_screen.y + (Windows_Height*250)/1080,  normal_pause_button_Width,normal_pause_button_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ? Tower Game You Win Screen Rect
    =====================================================*/

    // You Win Screen Background
    SDL_Rect youWin_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect youWin_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - youWin_screen_Height) / 2, pause_screen_Width, youWin_screen_Height};
    // You Win Button Rect
    SDL_Rect youWinButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    SDL_Rect homeButton_youWin_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,retryButton_rect.y + normal_pause_button_Height + buttonSpacing,normal_pause_button_Width,normal_pause_button_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/


    /* ==================================================
            ? Tower Game Pause Screen Buttons Rect
    =====================================================*/

    SDL_Rect resumeButton_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  pause_screen.y + (Windows_Height*140)/1080,  normal_pause_button_Width,normal_pause_button_Height};

    SDL_Rect restartButton_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,resumeButton_rect.y + normal_pause_button_Height + buttonSpacing,normal_pause_button_Width,normal_pause_button_Height};

    SDL_Rect homeButton_rect = {pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,restartButton_rect.y + normal_pause_button_Height + buttonSpacing,normal_pause_button_Width,normal_pause_button_Height};

    SDL_Rect music_pause_button_rect = {pause_screen.x + (pause_screen.w - (small_pause_button_Width*2 + (Windows_Width*20)/1920)) / 2,homeButton_rect.y + normal_pause_button_Height + buttonSpacing,small_pause_button_Width,small_pause_button_Height
    };

    SDL_Rect sound_pause_button_rect = {music_pause_button_rect.x + (Windows_Width*20)/1920 + small_pause_button_Width,homeButton_rect.y + normal_pause_button_Height + buttonSpacing,small_pause_button_Width,small_pause_button_Height
    };

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

     /* ==================================================
            ? Arrow Velocity Bar Rect
    =====================================================*/

    SDL_Rect arrow_velocity_bar_outline = {Windows_Width-(Windows_Width*60)/1920, (Windows_Height/2)-(velocity_bar_Height/2), velocity_bar_Width, velocity_bar_Height};
    SDL_Rect arrow_velocity_bar_innerfill = {Windows_Width-(Windows_Width*60)/1920, (Windows_Height/2)-(velocity_bar_Height/2), velocity_bar_Width, velocity_bar_Height};
    SDL_Rect arrow_velocity_bar = {arrow_velocity_bar_innerfill.x, arrow_velocity_bar_outline.y, velocity_bar_Width, 0};
    
    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
                ? Tower Character Rect
    =====================================================*/
    // Tower Attacker
    SDL_Rect tower_attacker = {(Windows_Width*100)/1920, Windows_Height - (towerHeight+(int)(Windows_Height * 0.26)), towerWidth, towerHeight};
    // Tower Bomb
    Pointer tower_bomb = {(float)tower_attacker.x, (float)tower_attacker.y, 0, 0, false};
    // Tower Bomb 2
    Pointer tower_bomb_1_5 = {(float)tower_attacker.x + (Windows_Width*50)/1920, (float) tower_attacker.y + (Windows_Height*350)/1080, 0, 0, false};
    Pointer tower_bomb_2 = {(float)tower_attacker.x + (Windows_Width*50)/1920, (float) tower_attacker.y + (Windows_Height*350)/1080, 0, 0, false};
    Pointer tower_bomb_3 = {(float)tower_attacker.x + (Windows_Width*50)/1920, (float) tower_attacker.y + (Windows_Height*350)/1080, 0, 0, false};
    Pointer tower_bomb_4 = {(float)tower_attacker.x + (Windows_Width*50)/1920, (float) tower_attacker.y + (Windows_Height*350)/1080, 0, 0, false};

    // Mille Character
    SDL_Rect mille_level2_rect = {(Windows_Width*20)/1920, Windows_Height - (towerHeight+(int)(Windows_Height * 0.06)), mille_basic_Width, mille_basic_Height};
    SDL_Rect mille_rect = {0, 0, mille_basic_Width, mille_basic_Height};

    // Hot Air Balloon
    const int hot_air_ballon_Width = (Windows_Width*192)/1920;
    const int hot_air_ballon_Height = (Windows_Height*256)/1080;
    SDL_Rect hot_air_balloon_rect = {(Windows_Width*300)/1920, (Windows_Height*400)/1080, hot_air_ballon_Width, hot_air_ballon_Height};
    Pointer hot_air_balloon_bomb = {(float)hot_air_balloon_rect.x, (float)hot_air_balloon_rect.y, 0, 0, false};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
                    ? Health Box Rect
    =====================================================*/

    // Tower Health Box
    SDL_Rect tower_health_box= {(Windows_Width - tower_health_box_Width)/2, (Windows_Height * 100) / 1080, tower_health_box_Width, tower_health_box_Height};
    SDL_Rect tower_health_box_outline= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};
    SDL_Rect tower_health_box_innerfill= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};

    // Hot Air Balloon
    SDL_Rect hot_air_ballon_health_box= {hot_air_balloon_rect.x + (Windows_Width*20)/1920, hot_air_balloon_rect.y - (Windows_Height*20)/1080, character_health_Width, character_health_Height};
    SDL_Rect hot_air_ballon_health_box_outline= {hot_air_ballon_health_box.x,hot_air_ballon_health_box.y,hot_air_ballon_health_box.w,hot_air_ballon_health_box.h};
    SDL_Rect hot_air_ballon_health_box_innerfill= {hot_air_ballon_health_box.x,hot_air_ballon_health_box.y,hot_air_ballon_health_box.w,hot_air_ballon_health_box.h};

    // Characters Health Box
    SDL_Rect archer_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect archer_health_box_outline= {archer_health_box.x,archer_health_box.y,archer_health_box.w,archer_health_box.h};
    SDL_Rect archer_health_box_innerfill= {archer_health_box.x,archer_health_box.y,archer_health_box.w,archer_health_box.h};

    SDL_Rect knight_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect knight_health_box_outline= {knight_health_box.x,knight_health_box.y,knight_health_box.w,knight_health_box.h};
    SDL_Rect knight_health_box_innerfill= {knight_health_box.x,knight_health_box.y,knight_health_box.w,knight_health_box.h};

    SDL_Rect mille_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect mille_health_box_outline= {mille_health_box.x,mille_health_box.y,mille_health_box.w,mille_health_box.h};
    SDL_Rect mille_health_box_innerfill= {mille_health_box.x,mille_health_box.y,mille_health_box.w,mille_health_box.h};

    SDL_Rect soldier_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect soldier_health_box_outline= {soldier_health_box.x,soldier_health_box.y,soldier_health_box.w,soldier_health_box.h};
    SDL_Rect soldier_health_box_innerfill= {soldier_health_box.x,soldier_health_box.y,soldier_health_box.w,soldier_health_box.h};

    SDL_Rect wolf_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect wolf_health_box_outline= {wolf_health_box.x,wolf_health_box.y,wolf_health_box.w,wolf_health_box.h};
    SDL_Rect wolf_health_box_innerfill= {wolf_health_box.x,wolf_health_box.y,wolf_health_box.w,wolf_health_box.h};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
                    ? Characters Rect
    =====================================================*/

    // Archer Character Rect
    Character archer = {(float)(Windows_Width+(archer_basic_Width)), (float)archerY, 0, 0, false};

    // Knight Character Rect 
    Character knight = {(float)(Windows_Width+(KNIGHT_RUN_FRAME_WIDTH)), (float)archerY, 0, 0, false};

    // Mille Character Rect 
    Character mille = {(float)(Windows_Width+(MILLE_BASIC_WIDTH)), (float)archerY, 0, 0, false};

    // Soldier Character Rect 
    Character soldier = {(float)(Windows_Width+(SOLDIER_BASIC_WIDTH)), (float)archerY, 0, 0, false};
    SDL_Rect soldier_rect = {(int)soldier.x, (int)soldier.y, soldier_basic_Width, soldier_basic_Height};

    // WOLF Character Rect 
    Character wolf = {(float)(-(WOLF_RUN_WIDTH + (Windows_Width*40)/1920)), (float)archerY, 0, 0, false};

    // Archer Arrow Rect
    Arrow archer_arrow = {(float)archer.x, (float)archer.y, 0, 0, false, (float)0};
    Arrow mille_laser = {(float)mille.x, (float)mille.y, 0, 0, false, (float)0};
    Arrow soldier_bullet = {(float)soldier.x, (float)soldier.y, 0, 0, false, (float)0};
    const int mille_laser_Width = (Windows_Width*(166/1.5))/1920;
    const int mille_laser_Height = (Windows_Height*(40/1.5))/1080;
    SDL_Rect mille_laser_rect = {(int)mille_laser.x, (int)mille_laser.y, mille_laser_Width, mille_laser_Height};

    /* ==================================================
            ? x----------------x---------------x
    =====================================================*/

    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/


    /* ==================================================
                ! Check All the Textures
    =====================================================*/
    if (!towerGame_level1_background || !towerGame_level2_background || !towerGame_level3_background || !towerGame_level4_background ||!main_backgroundTexture || !towerGame_mainPage_backgroundTexture || !towerGame_homePage_backgroundTexture || !towerGame_level1_background ) {
        printf("Texture Error: %s", SDL_GetError());
    }
    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/
    
    /* ==================================================
                        ! All Fonts
    =====================================================*/
    TTF_Font* font = TTF_OpenFont("./font/font.otf", 28);
    TTF_Font* mainfont = TTF_OpenFont("./font/mainfont.ttf", 20);
    TTF_Font* fontHeading = TTF_OpenFont("./font/font.otf", 100);
    TTF_Font* towerGame_font = TTF_OpenFont("./font/tower_font.ttf", 30);
    TTF_Font* towerGame_fontHeading = TTF_OpenFont("./font/tower_font.ttf", 100);
    TTF_Font* poppinsFont_Head = TTF_OpenFont("./font/poppinsFont.ttf", 60);
    TTF_Font* poppinsFont_Normal = TTF_OpenFont("./font/poppinsFont.ttf", 30);
    TTF_Font* poppinsFont_Normal_Customize = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*30)/1920);
    TTF_Font* pause_poppinsFont_Normal = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*40)/1920);
    TTF_Font* poppinsFont_Small = TTF_OpenFont("./font/poppinsFont.ttf", 20);
    TTF_Font* poppinsFont_Cards = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*20)/1920);
    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/

    /* ==================================================
                    ! Check All Fonts
    =====================================================*/
     if(!font || !mainfont || !fontHeading || !towerGame_font || !towerGame_fontHeading){
        printf("Font Error: %s\n", TTF_GetError());
        SDL_DestroyTexture(main_backgroundTexture);
        SDL_DestroyTexture(towerGame_mainPage_backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/
    
    /* ==================================================
                ! Main Background Texture
    =====================================================*/
    // Main Background Texture
    SDL_Texture* mainBackground = main_backgroundTexture;
    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/
    

    /* ==================================================
                        ! Boolean
    =====================================================*/

    // Click Functionlaity
    int archerclicked =1,knightclicked=0, milleclicked = 0, soldierclicked = 0;
    // Archer Dynamic Attacking
    float archerlastvelX =0.00,archerlastvelY =0.00;

    // Game Running 
    bool running = true;

    // Game Started
    bool gameStarted = false;

    // For Main Home/ First Page
    bool selectedGame_page = true; //! True Karo  

    // For Account 
    char username[20] = "";
    char password[20] = "";
    bool username_active = false;
    bool password_active = false;
    bool account_problem = false;
    bool account_true = false;
    char account_problem_str[50] = ""; 
    char account_true_str[50] = "";

    // All For Tower Game
    bool towerGame_start_bt = false;
    bool towerGame_account = false;
    bool towerGame_account_choose = false;
    bool towerGame_account_create = false;
    bool towerGame_account_login = false;
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

    // For All Levels
    bool all_levels_Started = false; //! False Karo

    bool midPause = false; //! False Karo
    bool midPause_archer_moving = false; //! false karo
    bool midPause_dialog_starting = false;//! False Karo 

    // Dialog
    // ? Starting
    bool dialog1_start = false;
    bool dialog2_start = false;
    bool dialog3_start = false;
    bool dialog4_start = false;
    bool dialog5_start = false;

    // ? Ending
    bool dialog1_end = false;
    bool dialog2_end = false;

    // Level 1 ending scene
    bool level1_ending_scene = false;
    bool level2_ending_scene = false;
    bool knight_level1_ending_dialog = false;
    bool archer_level1_ending_dialog = false;
    bool knight_moving_level1_ending = false;
    bool knight_running_ending_scene = false;
    bool knight_standing_ending_scene = false;
    bool knight_running_attacking_ending_scene = false;

    bool archer_showing_ending_scene = false;

    bool frame_Video = false; //! false
    bool level1_frame1 = false;//! false karo
    bool level1_frame2 = false;//! false karo

    bool loading_screen = false;
    // archer.y = Windows_Height - (int)((160 * ScaleY) + (280 * ScaleY));
    // tower_attacker.y = Windows_Height - (towerHeight+(int)(Windows_Height * 0.23));
    bool introGame_frame = false;

    // Health For Tower Game
    int tower_attacker_health = 100;
    int archer_health = 100;
    int knight_health = 100;
    int hot_air_balloon_health = 100;
    int mille_health = 100;
    int soldier_health = 100;
    int wolf_health = 100;

    // ON / OFF
    bool music = true; //! True karo
    bool sound = true;//! True karo

    // Paused 
    bool gamePause = false;
    bool gameOver = false;
    bool youWin = false;
    bool levelRestart = false;

    // For Archer
    bool archer_moving_left = false;
    bool archer_moving_right = false;
    bool archer_aiming = false;
    bool archer_shooting = false;
    bool archer_standing = false;

    // For Knight
    bool knight_moving_left = false;
    bool knight_moving_right = false;
    bool knight_shooting = false;
    bool knight_standing = false;
    bool knight_attack = false;

    // Hot Air Balloon
    bool hot_air_balloon_bomb_exploide = false;
    bool hot_air_balloon_throw_bomb = false;

    // For Wolf
    bool wolf_running = false;
    bool wolf_attacking = false;

    // For Cannon Ball
    bool tower_bomb_2_active = false;

    // For Mille
    bool mille_attacking = false;
    bool mille_standing = false;
    bool mille_moving_left = false;
    bool mille_moving_right = false;
    bool mille_shooting = false;

    // For Soldier
    bool soldier_attacking = false;
    bool soldier_standing = false;
    bool soldier_moving_left = false;
    bool soldier_moving_right = false;
    bool soldier_shooting = false;

    bool final_vid = false;
    bool credits_vid = false;
    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/

    // Characters Code
    // 1. Archer ----> 1234
    // 2. Knight ----> 2468
    // 3. Bomber ----> 1357
    // 4. Soldier ----> 9876
    // 5. Witch ----> 5432

    // array for character
    // int character_arr[4] = {0, 0, 0, 0};
    // // !
    // int num = 5432;
    // // Check if the number already exists in the array
    // for (int i = 0; i < 4; i++) {
    //     if (character_arr[i] == num) {
    //         return 0;  // Return 0 if the number is already in the array
    //     }
    // }

    // // Check if there's space (a zero value) to add the number
    // for (int i = 0; i < 4; i++) {
    //     if (character_arr[i] == 0) {
    //         character_arr[i] = num;  // Add the number to the array at the first empty spot
    //         return 1;  // Successfully added the number
    //     }
    // }

    // return 0;  // Return 0 if no space is available (no zero found)
    // !

    

    /* ==================================================
                    ! Character Animation
    =====================================================*/
    
    // !Variable For Animation
    // Archer
    SDL_Rect ArcherwalkClips[TOTAL_WALK_FRAMES];
    SDL_Rect ArcherstandClips[TOTAL_STAND_FRAME];
    SDL_Rect ArcherfireClips[TOTAL_FIRE_FRAMES];

    // Knight
    SDL_Rect KnightRunnigClips[KNIGHT_TOTAL_RUN_FRAME];
    SDL_Rect KnightRunnig_AttackingClips[KNIGHT_TOTAL_RUN_ATTACK_FRAME];

    // WOLF
    SDL_Rect WolfRunningClips[WOLF_TOTAL_RUN_FRAMES];
    SDL_Rect WolfAttackingClips[WOLF_TOTAL_ATTACK_FRAMES];

    // Hot Air Balloon Bomb
    SDL_Rect Hot_Air_Balloon_Bomb_Exploiding[HOT_AIR_BALLOON_BOMB_TOTALFRAMES];

    // Mille
    SDL_Rect MilleRunningClips[MILLE_RUNNING_TOTAL_FRAME];
    SDL_Rect MilleAttackingClips[MILLE_ATTACKING_TOTAL_FRAME];
    SDL_Rect MilleStandingClips[MILLE_STANDING_TOTAL_FRAME];

    // Mille
    SDL_Rect SoldierRunningClips[SOLDIER_RUNNING_TOTAL_FRAME];
    SDL_Rect SoldierAttackingClips[SOLDIER_ATTACKING_TOTAL_FRAME];

    //! Set up frames for running sprite sheet
    // For Archer Character
    // ? Archer Walk Frames 
    for (int i = 0; i < TOTAL_WALK_FRAMES; i++) {
        ArcherwalkClips[i].x = i * ARCHER_BASIC_FRAME_WIDTH;
        ArcherwalkClips[i].y = 0;
        ArcherwalkClips[i].w = ARCHER_BASIC_FRAME_WIDTH;
        ArcherwalkClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }

    // ? Archer Fire Frames
    for (int i = 0; i < TOTAL_FIRE_FRAMES; i++) {
        ArcherfireClips[i].x = i * ARCHER_FIRE_FRAME_WIDTH;
        ArcherfireClips[i].y = 0;
        ArcherfireClips[i].w = ARCHER_FIRE_FRAME_WIDTH;
        ArcherfireClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }

    // For Knight Character
    // ? Knight Run Frame
    for (int i = 0; i < KNIGHT_TOTAL_RUN_FRAME; i++) {
        KnightRunnigClips[i].x = i * KNIGHT_RUN_FRAME_WIDTH;
        KnightRunnigClips[i].y = 0;
        KnightRunnigClips[i].w = KNIGHT_RUN_FRAME_WIDTH;
        KnightRunnigClips[i].h = KNIGHT_RUN_FRAME_HEIGHT;
    }
    // ? Knight Run and Attack Frame
    for (int i = 0; i < KNIGHT_TOTAL_RUN_ATTACK_FRAME; i++) {
        KnightRunnig_AttackingClips[i].x = i * KNIGHT_RUN_ATTACK_FRAME_WIDTH;
        KnightRunnig_AttackingClips[i].y = 0;
        KnightRunnig_AttackingClips[i].w = KNIGHT_RUN_ATTACK_FRAME_WIDTH;
        KnightRunnig_AttackingClips[i].h = KNIGHT_RUN_ATTACK_FRAME_HEIGHT;
    }

    // For Hot Air Balloon Bomb
    for (int i = 0; i < HOT_AIR_BALLOON_BOMB_TOTALFRAMES; i++) {
        Hot_Air_Balloon_Bomb_Exploiding[i].x = i * HOT_AIR_BALLOON_BOMB_WIDTH;
        Hot_Air_Balloon_Bomb_Exploiding[i].y = 0;
        Hot_Air_Balloon_Bomb_Exploiding[i].w = HOT_AIR_BALLOON_BOMB_WIDTH;
        Hot_Air_Balloon_Bomb_Exploiding[i].h = HOT_AIR_BALLOON_BOMB_HEIGHT;
    }

    // For WOLF
    // ? WOLF RUN FRAME
    for (int i = 0; i < WOLF_TOTAL_RUN_FRAMES; i++) {
        WolfRunningClips[i].x = i * WOLF_RUN_WIDTH;
        WolfRunningClips[i].y = 0;
        WolfRunningClips[i].w = WOLF_RUN_WIDTH;
        WolfRunningClips[i].h = WOLF_RUN_HEIGHT;
    }
    // ? WOLF ATTACK FRAME
    for (int i = 0; i < WOLF_TOTAL_ATTACK_FRAMES; i++) {
        WolfAttackingClips[i].x = i * WOLF_ATTACK_WIDTH;
        WolfAttackingClips[i].y = 0;
        WolfAttackingClips[i].w = WOLF_ATTACK_WIDTH;
        WolfAttackingClips[i].h = WOLF_ATTACK_HEIGHT;
    }

    // For MILLE
    // ? MILLE RUN FRAME
    for (int i = 0; i < MILLE_RUNNING_TOTAL_FRAME; i++) {
        MilleRunningClips[i].x = i * MILLE_BASIC_WIDTH;
        MilleRunningClips[i].y = 0;
        MilleRunningClips[i].w = MILLE_BASIC_WIDTH;
        MilleRunningClips[i].h = MILLE_BASIC_HEIGHT;
    }
    // ? MILLE ATTACKING FRAME
    for (int i = 0; i < MILLE_ATTACKING_TOTAL_FRAME; i++) {
        MilleAttackingClips[i].x = i * MILLE_BASIC_WIDTH;
        MilleAttackingClips[i].y = 0;
        MilleAttackingClips[i].w = MILLE_BASIC_WIDTH;
        MilleAttackingClips[i].h = MILLE_BASIC_HEIGHT;
    }
    // ? MILLE ATTACKING FRAME
    for (int i = 0; i < MILLE_STANDING_TOTAL_FRAME; i++) {
        MilleStandingClips[i].x = i * MILLE_BASIC_WIDTH;
        MilleStandingClips[i].y = 0;
        MilleStandingClips[i].w = MILLE_BASIC_WIDTH;
        MilleStandingClips[i].h = MILLE_BASIC_HEIGHT;
    }

    // For SOLDIER
    // ? SOLDIER RUN FRAME
    for (int i = 0; i < SOLDIER_RUNNING_TOTAL_FRAME; i++) {
        SoldierRunningClips[i].x = i * SOLDIER_BASIC_WIDTH;
        SoldierRunningClips[i].y = 0;
        SoldierRunningClips[i].w = SOLDIER_BASIC_WIDTH;
        SoldierRunningClips[i].h = SOLDIER_BASIC_HEIGHT;
    }
    // ? SOLDIER ATTACKING FRAME
    for (int i = 0; i < SOLDIER_ATTACKING_TOTAL_FRAME; i++) {
        SoldierAttackingClips[i].x = i * SOLDIER_BASIC_WIDTH;
        SoldierAttackingClips[i].y = 0;
        SoldierAttackingClips[i].w = SOLDIER_BASIC_WIDTH;
        SoldierAttackingClips[i].h = SOLDIER_BASIC_HEIGHT;
    }


    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/

    
    /* ==================================================
                        ! Timer and Other
    =====================================================*/

    //! Event
    SDL_Event event;

    // ! Current frame
    int currentFrame = 0;
    int currentFrame_Enemy =0;

    int milleY = 0;
    int currentFrame_Knight = 0;
    int currentFrame_Mille = 0;
    int currentFrame_Soldier = 0;
    Uint32 lastFrameTime_Knight = 0;
    Uint32 lastFrameTime_Mille = 0;
    Uint32 lastFrameTime_Soldier = 0;
    
    //  ! Money and Card Cool
    int money = 0;
    int archer_card_cool = 0;
    int knight_card_cool = 0;
    int mille_card_cool = 0;
    int soldier_card_cool = 0;

    // ! Timer 
    // Tower Timer
    float tower_attack_timer = 0.0f;  
    const float tower_attack_delay = 1.0f;  
    // Hot Air Balloon Timer
    float hot_air_balloon_bomb_attack_timer = 0.0f;  
    const float hot_air_balloon_bomb_attack_delay = 6.0f;  

    // Knight Shooting 
     float knight_shooting_timer = 0.0f;  
    const float knight_shooting_delay = 2.0f;  

    // WOLF Timer
    float wolf_attack_timer = 0.00f;  
    const float wolf_attack_delay = 5.00f;  
    float wolf_spawn_timer = 0.00f;  
    const float wolf_spawn_delay = 6.00f;  

    // Inside your game loop:
    float deltaTime = 0.016f;  
    // tower_attack_timer += deltaTime; 
    // hot_air_balloon_bomb_attack_timer += deltaTime;

    // Timer For Money
    float money_Timer = 0.00f;
    float money_Increase_Timer = 0.01f;

    // Timer For Cannon Ball 2
    float tower_bomb_2_Timer = 0.00f;
    float tower_bomb_2_attack_delay = 5.00f;

     // Timer For Cannon Ball 1 for 5 levek
    float tower_bomb_1_5_Timer = 0.00f;
    float tower_bomb_1_5_attack_delay = 1.00f;


    // Timer For Cannon Ball 3
    float tower_bomb_3_Timer = 0.00f;
    float tower_bomb_3_attack_delay = 8.00f;

    // Timer For Cannon Ball 4
    float tower_bomb_4_Timer = 0.00f;
    float tower_bomb_4_attack_delay = 10.00f;

    // Timer For Card Cool
    float archer_card_cool_Timer = 0.00f;
    float archer_card_Increase_cool_Timer = 0.05f;
    float knight_card_cool_Timer = 0.00f;
    float knight_card_Increase_cool_Timer = 0.05f;
    float mille_card_cool_Timer = 0.00f;
    float mille_card_Increase_cool_Timer = 0.05f;
    float soldier_card_cool_Timer = 0.00f;
    float soldier_card_Increase_cool_Timer = 0.05f;

    // For Handling & Changing Frames
    Uint32 frameDelay = 100;  // Delay between frames (in milliseconds)
    Uint32 lastFrameTime = 0;
    Uint32 lastFrameTime_Enemy = 0;

    /* ==================================================
            ! x----------------x---------------x
    =====================================================*/


    /* ==================================================
                ! Game Running Main Loop
    =====================================================*/
    while (running) {

        /* ==================================================
                            ! Event 
        =====================================================*/

        while (SDL_PollEvent(&event)) {

            // ! Check event for SDL_QUIT
            if (event.type == SDL_QUIT) {
                running = false;
            } 

            // ! If game is not Started
            else if(!gameStarted){

                /* ==================================================
                                    Mouse Event
                =====================================================*/

                if((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)){
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    
                    // Selected Game Page
                    if(selectedGame_page){

                        mainBackground = main_backgroundTexture;

                        // If Selected Game Buttons is Clicked
                        if(checkButtonClick(mouseX, mouseY, &select_menu_playDeck_button_rect)){
                            Mix_PlayChannel(1, button_sound, 0); 
                            selectedGame_page = false;
                            towerGame = true;
                            towerGame_start_bt = true;
                            frame_Video = true;
                            introGame_frame = true;
                        }else if(checkButtonClick(mouseX, mouseY, &select_menu_quit_button_rect)){
                            Mix_PlayChannel(1, button_sound, 0); 
                            selectedGame_page = false;
                            running = false;
                        }
                    }
                    
                    //! If Tower is Game is Open
                    if(towerGame){

                        // If Tower Game Started and Mid pause and Mid Dialog is true
                        if(towerGame && towerGame_Started && midPause && midPause_dialog_starting){
                            // ? Level 1 Dialog
                            if(towerGame_Started_level1){
                                // Level 1 Starting Dialog Scene
                                if(!level1_ending_scene){
                                    if(dialog1_start){
                                        dialog2_start = true;
                                        dialog1_start = false;
                                    }else if(dialog2_start){
                                        midPause = false;
                                        midPause_dialog_starting = false;
                                        if(music){
                                            Mix_PlayChannel(0, level1Background_Sound, 0);
                                        }
                                    }
                                // Level1 Ending Dialog Scene
                                }else{
                                    if(knight_level1_ending_dialog){
                                        knight_level1_ending_dialog = false;
                                        midPause_dialog_starting = false;
                                        knight_moving_level1_ending = true;
                                        knight_running_ending_scene = true;
                                    }
                                    else if(archer_level1_ending_dialog){
                                        bool knight_level1_ending_dialog = false;
                                        bool archer_level1_ending_dialog = false;
                                        bool knight_moving_level1_ending = false;
                                        bool knight_running_ending_scene = false;
                                        bool knight_standing_ending_scene = false;
                                        bool knight_running_attacking_ending_scene = false;
                                        midPause_dialog_starting = false;
                                        midPause = false;
                                        youWin = true;
                                    }
                                }
                            }else if(towerGame_Started_level2){
                                // For Level 2

                                    // Dialog
                                if(midPause_dialog_starting){
                                    if(dialog1_start){
                                        dialog2_start = true;
                                        dialog1_start = false;
                                    }else if(dialog2_start){
                                        dialog3_start = true;
                                        dialog2_start = false;
                                    }else if(dialog3_start){
                                        dialog4_start = true;
                                        dialog3_start = false;
                                    }else if(dialog4_start){
                                        dialog5_start = true;
                                        dialog4_start = false;
                                    }else if(dialog5_start){
                                        dialog5_start = false;
                                        midPause = false;
                                    }
                                }
                            }else if(towerGame_Started_level3){
                                // For Level 3

                                    // Dialog
                                if(midPause_dialog_starting){
                                    if(dialog1_start){
                                        dialog2_start = true;
                                        dialog1_start = false;
                                    }else if(dialog2_start){
                                        dialog3_start = true;
                                        dialog2_start = false;
                                    }else if(dialog3_start){
                                        dialog4_start = true;
                                        dialog3_start = false;
                                    }else if(dialog4_start){
                                        dialog4_start = false;
                                        midPause = false;
                                    }
                                }
                            }else if(towerGame_Started_level4){
                                // For Level 4

                                    // Dialog
                                if(midPause_dialog_starting){
                                    if(dialog1_start){
                                        dialog2_start = true;
                                        dialog1_start = false;
                                    }else if(dialog2_start){
                                        dialog3_start = true;
                                        dialog2_start = false;
                                    }else if(dialog3_start){
                                        dialog4_start = true;
                                        dialog3_start = false;
                                    }else if(dialog4_start){
                                        dialog4_start = false;
                                        midPause = false;
                                    }
                                }
                            }else if(towerGame_Started_level5){
                                // For Level 5

                                    // Dialog
                                if(midPause_dialog_starting){
                                    if(dialog1_start){
                                        dialog2_start = true;
                                        dialog1_start = false;
                                    }else if(dialog2_start){
                                        dialog3_start = true;
                                        dialog2_start = false;
                                    }else if(dialog3_start){
                                        dialog4_start = true;
                                        dialog3_start = false;
                                    }else if(dialog4_start){
                                        dialog4_start = false;
                                        midPause = false;
                                    }
                                }
                            }
                        }

                        // ! If Tower Game Not Started
                        if(!towerGame_Started){
                            // Tower game -----> Enter Key is Pressed
                            if(towerGame_start_bt){
                                mainBackground = towerGame_mainPage_backgroundTexture;
                                // Tower account
                                if(towerGame_account){
                                    // Tower game Acoount Option

                                    if(towerGame_account_choose){

                                        // ? Account Create
                                        if(checkButtonClick(mouseX, mouseY, &account_create_button_rect)){
                                            if(sound){
                                                Mix_PlayChannel(1, button_sound, 0); 
                                            }
                                            towerGame_account_choose = false;
                                            towerGame_account_create = true;

                                        // ? Account Login
                                        }else if(checkButtonClick(mouseX, mouseY, &account_login_button_rect)){
                                            if(sound){
                                                Mix_PlayChannel(1, button_sound, 0); 
                                            }
                                            towerGame_account_choose = false;
                                            towerGame_account_login = true;
                                        }
                                    }else if(towerGame_account_create){
                                        if(checkButtonClick(mouseX, mouseY, &account_username)){
                                            password_active = false;
                                            username_active = true;
                                        }else if(checkButtonClick(mouseX, mouseY, &account_password)){
                                            username_active = false;
                                            password_active = true;
                                        }if(checkButtonClick(mouseX, mouseY, &account_create_button1_rect)){
                                            if(sound){
                                                Mix_PlayChannel(1, button_sound, 0); 
                                            }
                                            if(strlen(username)>4 && strlen(password)>4){
                                                if(usernameExists(username)){
                                                    account_true = false;
                                                    account_true_str[0] = '\0';
                                                    account_problem = true;
                                                    account_problem_str[0] = '\0';
                                                    strcat(account_problem_str, "Username Already Exists");
                                                }else{
                                                    if(createAccount(username, password)){
                                                        account_problem = false;
                                                        account_problem_str[0] = '\0';
                                                        username[0] = '\0';
                                                        password[0] = '\0';
                                                        account_username_typewriter.x = account_username.x + 10;
                                                        account_password_typewriter.x = account_password.x + 10;
                                                        account_true = true;
                                                        strcpy(account_true_str, "Your Account Created Successfully");
                                                    }else{
                                                        account_true = false;
                                                        account_true_str[0] = '\0';
                                                        account_problem = true;
                                                        account_problem_str[0] = '\0';
                                                        strcat(account_problem_str, "Error in Creating Account. Try Again!");
                                                    }
                                                }
                                            }else{
                                                account_problem = true;
                                                account_problem_str[0] = '\0';
                                                strcat(account_problem_str, "Minimum Length of username and passowrd is 5");
                                            }
                                        }
                                    }else if(towerGame_account_login){
                                        if(checkButtonClick(mouseX, mouseY, &account_username)){
                                            password_active = false;
                                            username_active = true;
                                        }else if(checkButtonClick(mouseX, mouseY, &account_password)){
                                            username_active = false;
                                            password_active = true;
                                        }if(checkButtonClick(mouseX, mouseY, &account_login_button1_rect)){
                                            if(sound){
                                                Mix_PlayChannel(1, button_sound, 0); 
                                            }
                                            if(strlen(username)>4 && strlen(password)>4){
                                                int result = login(username, password);
                                                if(!result){
                                                    account_true = false;
                                                    account_true_str[0] = '\0';
                                                    account_problem = true;
                                                    account_problem_str[0] = '\0';
                                                    strcat(account_problem_str, "Invalid username or password!");
                                                }else{
                                                    account_problem = false;
                                                    account_problem_str[0] = '\0';
                                                    account_true = false;
                                                    account_true_str[0] = '\0';
                                                    username[0] = '\0';
                                                    password[0] = '\0'; 
                                                    account_username_typewriter.x = account_username.x + 10;
                                                    account_password_typewriter.x = account_password.x + 10;

                                                    username_active = false;
                                                    password_active = false;
                                                    towerGame_start_bt = false;
                                                    towerGame_account = false;
                                                    towerGame_account_choose = false;
                                                    towerGame_account_create = false;
                                                    towerGame_homemenu = true;
                                                    mainBackground = towerGame_homePage_backgroundTexture;
                                                    towerGame_account_login = false;
                                                    music = true;
                                                    Mix_PlayChannel(0, towerGame_backgroundMusic, -1);
                                                    printf("\nYour left Money is: %d", result);
                                                }
                                            }else{
                                                account_problem = true;
                                                account_problem_str[0] = '\0';
                                                strcat(account_problem_str, "Minimum Length of username and passowrd is 5");
                                            }
                                        }
                                    }
                                }
                            }

                            //! If Tower Game Home Menu is True
                            else if(towerGame_homemenu){
                                if(checkButtonClick(mouseX, mouseY, &continue_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    if(level == 1){
                                        mainBackground = towerGame_level1_background;
                                    }else if(level ==2){
                                        mainBackground = towerGame_level2_background;
                                    }else if(level ==3){
                                        mainBackground = towerGame_level3_background;
                                    }else if(level ==4){
                                        mainBackground = towerGame_level4_background;
                                    }else if(level == 5){
                                        mainBackground = towerGame_level5_background;
                                    }
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level, &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else if(checkButtonClick(mouseX, mouseY, &level_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_levelmenu = true;
                                    towerGame_homemenu = false;
                                    mainBackground = towerGame_level_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &option_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = true;
                                    towerGame_homemenu = false;
                                    mainBackground = towerGame_option_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &quit_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame = false;
                                    running = false;
                                }
                            }else if(towerGame_levelmenu){
                                if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_homemenu = true;
                                    towerGame_levelmenu = false;
                                    mainBackground = towerGame_homePage_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &level1_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    mainBackground = towerGame_level1_background;
                                    level = 1;
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    
                                     default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level, &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                    
                                }else if(checkButtonClick(mouseX, mouseY, &level2_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    mainBackground = towerGame_level2_background;
                                    level =2 ;
                                     default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level,  &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else if(checkButtonClick(mouseX, mouseY, &level3_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    mainBackground = towerGame_level3_background;
                                    level = 3;
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level,  &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else if(checkButtonClick(mouseX, mouseY, &level4_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    mainBackground = towerGame_level4_background;
                                    level = 4;
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level,  &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else if(checkButtonClick(mouseX, mouseY, &level5_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    mainBackground = towerGame_level5_background;
                                    level = 5;
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level,  &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }
                            }else if(towerGame_optionmenu){
                                if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = false;
                                    towerGame_homemenu  = true;
                                    mainBackground = towerGame_homePage_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &music_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = false;
                                    towerGame_option_musicMenu = true;
                                    mainBackground = towerGame_music_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &shop_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = false;
                                    frame_Video = true;
                                    credits_vid = true;
                                }else if(checkButtonClick(mouseX, mouseY, &sound_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = false;
                                    towerGame_option_soundMenu = true;
                                    mainBackground = towerGame_sound_backgroundTexture;
                                }else if(checkButtonClick(mouseX, mouseY, &control_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_optionmenu = false;
                                    towerGame_option_controllerMenu = true;
                                    mainBackground = towerGame_control_backgroundTexture;
                                }
                            }else if(towerGame_option_musicMenu){
                                if(checkButtonClick(mouseX, mouseY, &music_on_off_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    music = !music;
                                    if(music){
                                        Mix_PlayChannel(0, towerGame_backgroundMusic, -1);
                                    }else{
                                        Mix_HaltChannel(0);
                                    }
                                }else if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_option_musicMenu = false;
                                    towerGame_optionmenu = true;
                                    mainBackground = towerGame_option_backgroundTexture;
                                }
                            }else if(towerGame_option_soundMenu){
                                if(checkButtonClick(mouseX, mouseY, &sound_on_off_button_rect)){
                                    sound = !sound;
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                }else if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_option_soundMenu = false;
                                    towerGame_optionmenu = true;
                                    mainBackground = towerGame_option_backgroundTexture;
                                }
                            }else if(towerGame_option_controllerMenu){
                                if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_option_controllerMenu = false;
                                    towerGame_optionmenu = true;
                                    mainBackground = towerGame_option_backgroundTexture;
                                }
                            }else if(towerGame_option_shopMenu){
                                if(checkButtonClick(mouseX, mouseY, &back_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_option_shopMenu = false;
                                    towerGame_optionmenu = true;
                                    mainBackground = towerGame_option_backgroundTexture;
                                }
                            }
                        }else if(towerGame_Started && !midPause){
                            // ! Check click on Card or not

                            if((towerGame_Started_level1 || towerGame_Started_level2 || towerGame_Started_level3 || towerGame_Started_level4 || towerGame_Started_level5 && !gamePause)){
                                if(towerGame_Started_level1){
                                    mainBackground = towerGame_level1_background;
                                }else if(towerGame_Started_level2){
                                    mainBackground = towerGame_level2_background;

                                    // Card Controller
                                    if( mouseX >= archer.x && mouseX <= archer.x + archer_basic_Width&& mouseY >= archer.y && mouseY <= archer.y + archer_basic_Height){
                                        archerclicked =1;
                                        knightclicked =0;
                                        soldierclicked = 0;
                                        milleclicked = 0;
                                    }
                                    if( mouseX >= knight.x && mouseX <= knight.x + knight_basic_Width&&mouseY >= knight.y && mouseY <= knight.y + knight_basic_Height){
                                        archerclicked =0;
                                        knightclicked =1;
                                        soldierclicked = 0;
                                        milleclicked = 0;
                                    }
                                }else if(towerGame_Started_level3){
                                    mainBackground = towerGame_level3_background;

                                    // Card Controller
                                    if( mouseX >= archer.x && mouseX <= archer.x + archer_basic_Width&& mouseY >= archer.y && mouseY <= archer.y + archer_basic_Height){
                                        archerclicked =1;
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        knightclicked =0;
                                    }
                                    if( mouseX >= knight.x && mouseX <= knight.x + knight_basic_Width&&mouseY >= knight.y && mouseY <= knight.y + knight_basic_Height){
                                        knightclicked =1;
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        archerclicked =0;
                                    }
                                    if(mouseX >= mille.x && mouseX <= mille.x + mille_rect.w&&mouseY >= mille.y && mouseY <= mille.y + mille_rect.h){
                                        knightclicked =0;
                                        archerclicked =0;
                                        soldierclicked = 0;
                                        milleclicked = 1;
                                    }
                                }else if(towerGame_Started_level4){
                                    mainBackground = towerGame_level4_background;

                                    // Card Controller
                                    if( mouseX >= archer.x && mouseX <= archer.x + archer_basic_Width&& mouseY >= archer.y && mouseY <= archer.y + archer_basic_Height){
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        knightclicked =0;
                                        archerclicked =1;
                                    }
                                    if( mouseX >= knight.x && mouseX <= knight.x + knight_basic_Width&&mouseY >= knight.y && mouseY <= knight.y + knight_basic_Height){
                                        knightclicked =1;
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        archerclicked =0;
                                    }
                                    if(mouseX >= mille.x && mouseX <= mille.x + mille_rect.w&&mouseY >= mille.y && mouseY <= mille.y + mille_rect.h){
                                        knightclicked =0;
                                        archerclicked =0;
                                        soldierclicked = 0;
                                        milleclicked = 1;
                                    }
                                    if(mouseX >= soldier.x && mouseX <= soldier.x + soldier_rect.w&&mouseY >= soldier.y && mouseY <= soldier.y + soldier_rect.h){
                                        knightclicked =0;
                                        archerclicked =0;
                                        milleclicked = 0;
                                        soldierclicked = 1;
                                    }
                                }else if(towerGame_Started_level5){
                                    mainBackground = towerGame_level5_background;

                                    // Card Controller
                                    if( mouseX >= archer.x && mouseX <= archer.x + archer_basic_Width&& mouseY >= archer.y && mouseY <= archer.y + archer_basic_Height){
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        knightclicked =0;
                                        archerclicked =1;
                                    }
                                    if( mouseX >= knight.x && mouseX <= knight.x + knight_basic_Width&&mouseY >= knight.y && mouseY <= knight.y + knight_basic_Height){
                                        knightclicked =1;
                                        milleclicked = 0;
                                        soldierclicked = 0;
                                        archerclicked =0;
                                    }
                                    if(mouseX >= mille.x && mouseX <= mille.x + mille_rect.w&&mouseY >= mille.y && mouseY <= mille.y + mille_rect.h){
                                        knightclicked =0;
                                        archerclicked =0;
                                        soldierclicked = 0;
                                        milleclicked = 1;
                                    }
                                    if(mouseX >= soldier.x && mouseX <= soldier.x + soldier_rect.w&&mouseY >= soldier.y && mouseY <= soldier.y + soldier_rect.h){
                                        knightclicked =0;
                                        archerclicked =0;
                                        milleclicked = 0;
                                        soldierclicked = 1;
                                    }
                                }
                                
                                if(checkButtonClick(mouseX, mouseY, &card_archer_rect)){
                                    if(archer_card_cool>=100 && money >= 5){
                                        archer_card_cool = 0;
                                        money -= 5;
                                    }
                                }else if(checkButtonClick(mouseX, mouseY, &card_knight_rect)){
                                    if(knight_card_cool>=100 && money >= 5){
                                        knight.spawn = true;
                                        knight_card_cool = 0;
                                        money -= 5;
                                    }
                                }else if(checkButtonClick(mouseX, mouseY, &card_mille_rect)){
                                    if(mille_card_cool>=100 && money >= 5){
                                        mille.spawn = true;
                                        mille_card_cool = 0;
                                        money -= 5;
                                    }
                                }else if(checkButtonClick(mouseX, mouseY, &card_soldier_rect)){
                                    if(soldier_card_cool>=100 && money >= 5){
                                        soldier.spawn = true;
                                        soldier_card_cool = 0;
                                        money -= 5;
                                    }
                                }
                                
                            }

                            // ! Game Pause || Game Over || You Win
                            if(!gamePause){
                                if(checkButtonClick(mouseX, mouseY, &pauseButton_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    gamePause = true;
                                }
                            }else if(gamePause){
                                if(checkButtonClick(mouseX, mouseY, &resumeButton_rect)){
                                    
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    gamePause = false;
                                }else  if(checkButtonClick(mouseX, mouseY, &homeButton_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_Started_level1 = !towerGame_Started_level1;
                                    towerGame_Started_level2 = !towerGame_Started_level2;
                                    towerGame_Started_level3 = !towerGame_Started_level3;
                                    towerGame_Started_level4 = !towerGame_Started_level4;
                                    towerGame_Started_level5 = !towerGame_Started_level5;
                                    towerGame_Started = false;
                                    towerGame_homemenu = true;
                                    gamePause = false;
                                    mainBackground = towerGame_homePage_backgroundTexture;
                                    if(music){
                                        Mix_PlayChannel(0, towerGame_backgroundMusic, 0);
                                    }
                                }else  if(checkButtonClick(mouseX, mouseY, &restartButton_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    gamePause = false;
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    if(level == 1){
                                        mainBackground = towerGame_level1_background;
                                    }else if(level ==2){
                                        mainBackground = towerGame_level2_background;
                                    }else if(level ==3){
                                        mainBackground = towerGame_level3_background;
                                    }else if(level ==4){
                                        mainBackground = towerGame_level4_background;
                                    }else if(level == 5){
                                        mainBackground = towerGame_level5_background;
                                    }
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level, &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else if(checkButtonClick(mouseX, mouseY, &music_pause_button_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    music = !music;
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, -1);
                                    }else{
                                        Mix_HaltChannel(0);
                                    }
                                }else  if(checkButtonClick(mouseX, mouseY, &sound_pause_button_rect)){
                                    sound = !sound;
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                }
                            }
                            if(gameOver){
                                if(checkButtonClick(mouseX, mouseY, &retryButton_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    levelRestart = true;
                                }else  if(checkButtonClick(mouseX, mouseY, &homeButton_gameOver_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, towerGame_backgroundMusic, 0);
                                    }
                                    towerGame_Started_level1 = !towerGame_Started_level1;
                                    towerGame_Started_level2 = !towerGame_Started_level2;
                                    towerGame_Started_level3 = !towerGame_Started_level3;
                                    towerGame_Started_level4 = !towerGame_Started_level4;
                                    towerGame_Started_level5 = !towerGame_Started_level5;
                                    towerGame_Started = false;
                                    towerGame_homemenu = true;
                                    gameOver = false;
                                    mainBackground = towerGame_homePage_backgroundTexture;
                                }
                            }else if(youWin){
                                if(checkButtonClick(mouseX, mouseY, &nextlevelButton_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_Started_level1 = !towerGame_Started_level1;
                                    towerGame_Started_level2 = !towerGame_Started_level2;
                                    towerGame_Started_level3 = !towerGame_Started_level3;
                                    towerGame_Started_level4 = !towerGame_Started_level4;
                                    towerGame_Started_level5 = !towerGame_Started_level5;
                                    youWin = false;
                                    Mix_HaltChannel(0);
                                    if(music){
                                        Mix_PlayChannel(0, level1Background_Sound, 0);
                                    }
                                    if(level == 1){
                                        mainBackground = towerGame_level2_background;
                                        level = 2;
                                    }else if(level ==2){
                                        mainBackground = towerGame_level3_background;
                                        level = 3;
                                    }else if(level ==3){
                                        mainBackground = towerGame_level4_background;
                                        level = 4;
                                    }else if(level ==4){
                                        mainBackground = towerGame_level5_background;
                                        level = 5;
                                    }else if(level == 5){
                                        mainBackground = towerGame_level1_background;
                                        level = 1;
                                    }
                                    default_game(&tower_attacker, &hot_air_balloon_rect, &tower_bomb, &tower_bomb_2,&tower_bomb_3, &tower_bomb_1_5, &tower_bomb_4, &hot_air_balloon_bomb, &archer_arrow, &towerGame_Started_level1, &towerGame_Started_level2, &towerGame_Started_level3, &towerGame_Started_level4, &towerGame_Started_level5, &all_levels_Started, &towerGame_Started, mainBackground,&towerGame_levelmenu, &towerGame_homemenu, &money, &archer_health, &knight_health, &mille_health, &soldier_health, &tower_attacker_health, &hot_air_balloon_health, &wolf_health, &archer_card_cool, &knight_card_cool, &mille_card_cool, &soldier_card_cool, &midPause, &midPause_dialog_starting, &midPause_archer_moving,&frame_Video, &loading_screen, &archer_standing, &archerY, &archer, &knight, &mille, &soldier, &wolf, &archer_health_box, &knight_health_box, &soldier_health_box, &mille_health_box, &tower_health_box,&hot_air_ballon_health_box, &wolf_health_box, &wolf_running, &wolf_attacking, &mille_rect, &soldier_rect, towerGame_level1_background, towerGame_level2_background, towerGame_level3_background, towerGame_level4_background, towerGame_level5_background, level3_towerWidth, level3_towerHeight, level4_towerWidth, level4_towerHeight, level5_towerWidth, level5_towerHeight, knight_basic_Width, knight_basic_Height, archer_basic_Width, mille_basic_Width, mille_basic_Height, soldier_basic_Width, soldier_basic_Height, &milleY, ScaleX, ScaleY, Windows_Width, Windows_Height, towerWidth, towerHeight, level, &level1_ending_scene, &archerclicked, &knightclicked, &milleclicked, &soldierclicked);
                                }else  if(checkButtonClick(mouseX, mouseY, &homeButton_youWin_rect)){
                                    if(sound){
                                        Mix_PlayChannel(1, button_sound, 0); 
                                    }
                                    towerGame_Started_level1 = !towerGame_Started_level1;
                                    towerGame_Started_level2 = !towerGame_Started_level2;
                                    towerGame_Started_level3 = !towerGame_Started_level3;
                                    towerGame_Started_level4 = !towerGame_Started_level4;
                                    towerGame_Started_level5 = !towerGame_Started_level5;
                                    towerGame_Started = false;
                                    towerGame_homemenu = true;
                                    youWin = false;
                                    mainBackground = towerGame_homePage_backgroundTexture;    
                                    if(music){
                                        Mix_PlayChannel(0, towerGame_backgroundMusic, 0);
                                    }                            
                                }
                            }
                        }
                    }

                /* ==================================================
                        x--------------x-------------x
                =====================================================*/

                /* ==================================================
                            SDL_TEXTINPUT---> EVENT
                =====================================================*/
                }else if (event.type == SDL_TEXTINPUT) {
                    if(username_active){
                        if (strlen(username) < sizeof(username) - 1) {
                            account_username_typewriter.x +=  20;
                            strcat(username, event.text.text);
                            Mix_PlayChannel(2, typing_SoundEffect, 0);
                    }
                    }else if(password_active){
                        if (strlen(password) < sizeof(password) - 1) {
                            account_password_typewriter.x +=  20;
                            strcat(password, event.text.text);
                            Mix_PlayChannel(2, typing_SoundEffect, 0);
                        }
                    }    
                /* ==================================================
                        x--------------x-------------x
                =====================================================*/

                /* ==================================================
                            SDL_KEYDOWN---> EVENT
                =====================================================*/
                }else if(event.type == SDL_KEYDOWN){
                    // ! Running False
                    // if(event.key.keysym.sym == SDLK_ESCAPE){
                    //     running = false;
                    // }
                    // ! Tower Game is Open
                    if(towerGame){
                        // ! Tower Game Not Started
                        if(!towerGame_Started){
                            switch (event.key.keysym.sym)
                            {
                            case SDLK_RETURN:
                                if(towerGame_start_bt){
                                    if(!towerGame_account){
                                        towerGame_account = true;
                                        towerGame_account_choose = true;
                                    }
                                }
                                break;
                            case SDLK_BACKSPACE:
                                if(username_active && strlen(username) > 0){
                                    username[strlen(username) - 1] = '\0';
                                    account_username_typewriter.x -=  20;
                                    Mix_PlayChannel(2, typing_SoundEffect, 0);
                                }else if(password_active && strlen(password) > 0){
                                    password[strlen(password)-1] = '\0';
                                    account_password_typewriter.x -=  20;
                                    Mix_PlayChannel(2, typing_SoundEffect, 0);
                                }
                                break;
                            case SDLK_TAB:
                                if(!towerGame_account_choose && (towerGame_account_create || towerGame_account_login)){
                                    towerGame_account_choose = true;
                                    towerGame_account_create = false;
                                    towerGame_account_login = false;
                                    username_active = false;
                                    password_active = false;
                                    account_problem = false;
                                    account_true = false;
                                    account_username_typewriter.x = account_username.x + 10;
                                    account_password_typewriter.x = account_password.x + 10;
                                    account_true_str[0] = '\0';
                                    account_problem_str[0] = '\0';
                                    username[0] = '\0';
                                    password[0] = '\0';
                                }
                            default:
                                break;
                            }
                        // ! Moving Archer and other charcters depend upon conditions
                        }else if(towerGame_Started && !gamePause &&!youWin && !gameOver && !midPause){
                            switch (event.key.keysym.sym)
                            {
                            case SDLK_w:
                                if(archerclicked){
                                    if(archer.x > (Windows_Width - ARCHER_FIRE_FRAME_WIDTH)/2){
                                        archer_moving_left = true;
                                        archer_standing = false;
                                        archer.x -= 16*ScaleX;
                                    }
                                }
                                if(knight.spawn&&knightclicked){
                                    if(knight.x > KNIGHT_RUN_FRAME_WIDTH + tower_attacker.x){
                                        knight_moving_left = true;
                                        knight_standing = false;
                                        knight.x -= 16*ScaleX;
                                    }
                                }
                                if(mille.spawn&&milleclicked){
                                    if(mille.x > (Windows_Width - mille_basic_Width)/2){
                                        mille_moving_left = true;
                                        mille_standing = false;
                                        mille.x -= 16*ScaleX;
                                    }
                                }
                                if(soldier.spawn&&soldierclicked){
                                    if(soldier.x > (Windows_Width - soldier_basic_Width)/2){
                                        soldier_moving_left = true;
                                        soldier_standing = false;
                                        soldier.x -= 16*ScaleX;
                                    }
                                }
                                break;
                            case SDLK_s:
                                if(archerclicked){
                                    if(archer.x < Windows_Width - (ARCHER_FIRE_FRAME_WIDTH)){
                                        archer_moving_right = true;
                                        archer_standing = false;
                                        archer.x += 16*ScaleX;
                                    }
                                }
                                if(knight.spawn&&knightclicked){
                                    if(knight.x < Windows_Width - (KNIGHT_RUN_FRAME_WIDTH)){
                                        knight_moving_right = true;
                                        knight_standing = false;
                                        knight.x += 16*ScaleX;
                                    }
                                }   
                                if(mille.spawn&&milleclicked){
                                    if(mille.x < Windows_Width - (mille_basic_Width)){
                                        mille_moving_right = true;
                                        mille_standing = false;
                                        mille.x += 16*ScaleX;
                                    }
                                }   
                                if(soldier.spawn&&soldierclicked){
                                    if(soldier.x < Windows_Width - (soldier_basic_Width)){
                                        soldier_moving_right = true;
                                        soldier_standing = false;
                                        soldier.x += 16*ScaleX;
                                    }
                                }  
                                break;
                            case SDLK_SPACE: 
                                if(archerclicked){
                                    if (archer.y >= archerY) {  
                                        archer_standing = false;
                                        archer.vy = -10*ScaleY; 
                                        archer.active = true; 
                                    }
                                }
                                if(knight.spawn&&knightclicked){
                                    if(knight.y >= archerY){
                                        knight_standing = false;
                                        knight.vy = -10*ScaleY; 
                                        knight.active = true; 
                                    }
                                }   
                                if(mille.spawn&&milleclicked){
                                    if(mille.y >= milleY){
                                        mille_standing = true;
                                        mille.vy = -10*ScaleY; 
                                        mille.active = true;
                                    }
                                }   
                                if(soldier.spawn&&soldierclicked){
                                    if(soldier.y >=  Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY))){
                                        soldier_standing = true;
                                        soldier.vy = -10*ScaleY; 
                                        soldier.active = true;
                                    }
                                }  
                                break;
                            case SDLK_f: 
                                // Increase velocity while 'F' is held down
                                if(archerclicked){
                                    if (!archer_arrow.active) {
                                        archer_aiming = true; 
                                        if(archer_standing){
                                            currentFrame = 0;
                                            archer_standing = false;
                                        }
                                        if(arrow_velocity_bar.h >= velocity_bar_Height){
                                            arrow_velocity_bar.h = velocity_bar_Height;
                                        }else if(arrow_velocity_bar.h <= velocity_bar_Height){
                                            arrow_velocity_bar.h += (Windows_Height*30)/1080;
                                        }
                                        
                                        if (archer_arrow.vx <= MAX_ARROW_SPEED_X*ScaleX) {
                                            archer_arrow.vx = MAX_ARROW_SPEED_X*ScaleX;  
                                        }else if(archer_arrow.vx >= MAX_ARROW_SPEED_X*ScaleX){
                                            archer_arrow.vx -= 3 * ScaleX;  // Increase velocity each frame
                                        }
                                        if(archer_arrow.vy <= MAX_ARROW_SPEED_Y*ScaleY){
                                            archer_arrow.vy = MAX_ARROW_SPEED_Y*ScaleY;
                                        }else if(archer_arrow.vx >= MAX_ARROW_SPEED_Y*ScaleY){
                                            archer_arrow.vy -= 3.6 * ScaleY;  // Increase velocity each frame
                                        }
                                    }
                                }
                            if (knight.spawn&&knightclicked) {
                                knight_shooting = true; 
                                if(knight_shooting_timer>=knight_shooting_delay){
                                    knight_attack = true;
                                    knight_shooting_timer = 0.0f;
                                }
                            }

                            if (mille.spawn&&milleclicked) {
                                if(!mille.active && !mille_laser.active){
                                    mille_shooting = true; 
                                    mille.y = Windows_Height - (int)((126 * ScaleY) + (280 * ScaleY));
                                    mille_laser.active = true;
                                }
                            }
                            if (soldier.spawn&&soldierclicked) {
                                if(!soldier.active && !soldier_bullet.active){
                                    soldier_shooting = true; 
                                    soldier_bullet.active = true;
                                }
                            }
                                break;
                            default:
                                break;
                            }
                        }
                    }

                /* ==================================================
                        x--------------x-------------x
                =====================================================*/

                /* ==================================================
                            SDL_KEYUP---> EVENT
                =====================================================*/

                }else if(event.type == SDL_KEYUP){
                    //! Archer and other Chaarcter Moving And Stopping depend Upon Condition
                    if(towerGame_Started && !gamePause && !gameOver && !youWin && !midPause){
                        switch(event.key.keysym.sym){
                            case SDLK_w:
                                    if(archerclicked){
                                        archer_moving_left = false;
                                        currentFrame = 0;
                                    }
                                    if(knight.spawn&&knightclicked){
                                        knight_moving_left = false;
                                        knight_standing = true;
                                        currentFrame_Knight = 0;
                                    }
                                    if(mille.spawn&&milleclicked){
                                        mille_moving_left = false;
                                        currentFrame_Mille = 0;
                                    }
                                    if(soldier.spawn&&soldierclicked){
                                        soldier_moving_left = false;
                                        currentFrame_Soldier = 0;
                                    }
                                break;
                            case SDLK_s:
                                    if(archerclicked){
                                        archer_moving_right = false;
                                        // archer_standing = true;
                                         currentFrame = 0;
                                    }
                                    if(knight.spawn&&knightclicked){
                                        knight_moving_right = false;
                                        knight_standing = true;
                                        currentFrame_Knight = 0;
                                    }
                                    if(mille.spawn&&milleclicked){
                                        mille_moving_right = false;
                                        currentFrame_Mille = 0;
                                    }
                                    if(soldier.spawn&&soldierclicked){
                                        soldier_moving_right = false;
                                        currentFrame_Soldier = 0;
                                    }
                                break;
                            case SDLK_f: 
                                    if(sound){
                                        Mix_PlayChannel(2, archerShooting_sound, 0); 
                                    }
                                    if(archerclicked){
                                        if(!archer_arrow.active){
                                            archer_arrow.x = archer.x;
                                            archer_arrow.y = archer.y;
                                            archer_arrow.active = true;
                                            archer_arrow.angle = 0;
                                        }   
                                        archer_shooting = true;
                                        archer_aiming = false;
                                        arrow_velocity_bar.h = 0;
                                    }
                                    if(knight.spawn&&knightclicked){
                                        knight_shooting = false;
                                        knight_standing = true;
                                        currentFrame_Knight = 0;
                                    }
                                    if(mille.spawn&&milleclicked){
                                        mille_shooting = false;
                                        mille.y = Windows_Height - (int)((110 * ScaleY) + (280 * ScaleY));
                                        currentFrame_Mille = 0;
                                    }
                                    if(soldier.spawn&&soldierclicked){
                                        soldier_shooting = false;
                                        currentFrame_Soldier = 0;
                                    }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }

            /* ==================================================
                        x--------------x-------------x
            =====================================================*/

            // If game Started and if we press "ESC" key then the over all program terminate
            // else if(gameStarted){
            //         if (event.type == SDL_KEYDOWN) {
            //             switch (event.key.keysym.sym) {
            //                 case SDLK_ESCAPE:
            //                     running = false;
            //                     break;
            //                 default:
            //                     break;
            //             }
            //     }
            // }
        }

        /* ==================================================
                ! x--------------x-------------x
        =====================================================*/
        
    /* ==================================================
                ! Tower Game Not Started
    =====================================================*/
    if(!towerGame_Started){
        // Play Intro Video
        if(frame_Video){
            if(introGame_frame){
                if (!playVideo(window, renderer, "audio/introgame.mp3", "image/frames/intro/frame_%04d.png", 721, 1, 30, 1)) {
                        printf("Video playback ended or failed.\n");
                }
                introGame_frame = false;
            }
            if(credits_vid){
                     if (!playVideoCredit(window, renderer, "audio/deckofdominions/frames/credits.mp3", "image/frames/credits/frame_%04d.png", 0577, 1, 30, 2, 1)) {
                        printf("Video playback ended or failed.\n");
                    }
                    towerGame_homemenu = true;
                    mainBackground = towerGame_homePage_backgroundTexture;
                    credits_vid = false;
            }
            frame_Video = false;
        }
    }

    /* ==================================================
                ! x--------------x-------------x
    =====================================================*/

    /* ==================================================
                ! Tower Game Not Started
    =====================================================*/
    if(towerGame_Started && !gamePause && !youWin && !gameOver){
        // If level1 starting 
        //  End That Day Archer Got his revenge From the Cruel King
        if(!level1_ending_scene){
            // Frame Video Level1
            if(frame_Video){
                if(towerGame_Started_level1){
                    if(loading_screen){
                        if (!playVideo(window, renderer, "none", "image/frames/loading_frame/frame_%04d.png", 0300, 1, 30, 2)) {
                            printf("Video playback ended or failed.\n");
                        }
                        loading_screen = false;
                        level1_frame1 = true;
                    }
                    if(level1_frame1){
                        if (!playVideo(window, renderer, "audio/deckofdominions/frames/intro1.mp3", "image/frames/level1/starting/frame1/frame_%04d.jpeg", 1749, 1, 15, 1)) {
                            printf("Video playback ended or failed.\n");
                        }
                        level1_frame1 = false;
                        level1_frame2 = true;
                    }
                    if(level1_frame2){
                        if (!playVideo(window, renderer, "audio/deckofdominions/frames/intro2.mp3", "image/frames/level1/starting/frame2/frame_%04d.jpeg", 2018, 1, 15, 2)) {
                            printf("Video playback ended or failed.\n");
                        }
                        level1_frame2 = false;
                        loading_screen = true;
                    }
                }
                if(loading_screen){
                    if (!playVideo(window, renderer, "none", "image/frames/loading_frame/frame_%04d.png", 0300, 1, 30, 2)) {
                        printf("Video playback ended or failed.\n");
                    }
                    loading_screen = false;
                    Mix_HaltChannel(0);
                    if(music){
                        Mix_PlayChannel(0, level1Background_Sound, 0);
                    }
                }
                if(final_vid){
                    towerGame_Started_level1 = !towerGame_Started_level1;
                    towerGame_Started_level2 = !towerGame_Started_level2;
                    towerGame_Started_level3 = !towerGame_Started_level3;
                    towerGame_Started_level4 = !towerGame_Started_level4;
                    towerGame_Started_level5 = !towerGame_Started_level5;
                    towerGame_Started = false;
                    // towerGame_homemenu = true;
                    mainBackground = towerGame_homePage_backgroundTexture;
                    if (!playVideoCredit(window, renderer, "audio/deckofdominions/frames/final.mp3", "image/frames/final_frame/frame_%04d.png", 0175, 1, 30, 1, 2)) {
                        printf("Video playback ended or failed.\n");
                    }
                    final_vid = false;
                    credits_vid = true;
                }
                if(credits_vid){
                    towerGame_Started = false;
                     if (!playVideoCredit(window, renderer, "audio/deckofdominions/frames/credits.mp3", "image/frames/credits/frame_%04d.png", 0577, 1, 30, 2, 1)) {
                        printf("Video playback ended or failed.\n");
                    }
                    towerGame_homemenu = true;
                    credits_vid = false;
                }
                frame_Video = false;

            // MidPause For Dialog and Other things
            }else if (midPause && !frame_Video) {
                if(midPause_archer_moving){
                    if(!midPause_dialog_starting){

                        if (archer.x > archerX) {
                            archer.x -= 8 * ScaleX; // Move left
                            archer_moving_left = true;

                            // Update animation frames
                            Uint32 currentTime = SDL_GetTicks();
                            if (currentTime > lastFrameTime + frameDelay) {
                                if (archer_aiming || archer_moving_left || archer_moving_right || archer_shooting || archer_standing) {
                                    currentFrame++;

                                    if (archer_moving_left || archer_moving_right) {
                                        // Loop walking animation
                                        if (currentFrame >= TOTAL_WALK_FRAMES) {
                                            currentFrame = 0;
                                        }
                                    }
                                }
                                lastFrameTime = currentTime;
                            }
                            
                        } else {
                            midPause_dialog_starting = true;
                            dialog1_start = true;
                            archer_moving_left = false;
                            currentFrame = 0;
                        }
                    }
                }
            }

        // Level1 Ending Scene 
        }else if(level1_ending_scene){
            if(midPause && !knight_level1_ending_dialog && !archer_level1_ending_dialog &&  knight_moving_level1_ending){
                // if (knight.x > tower_attacker.x) {
                if(knight.x > (Windows_Width*600)/1920){
                    knight.x -= 8 * ScaleX; // Move left

                            // Update animation frames
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime > lastFrameTime + frameDelay) {
                        if (knight_running_ending_scene) {
                            currentFrame++;

                            // Loop walking animation
                            if (currentFrame >= KNIGHT_TOTAL_RUN_FRAME) {
                                 currentFrame = 0;
                            }
                        }
                            lastFrameTime = currentTime;
                    }
                }else if(knight.x >= (tower_attacker.x + tower_attacker.w)){
                    knight_running_ending_scene = false;
                    knight.x -= 4*ScaleX;
                    knight.y -= 1.5*ScaleY;
                }else if(knight.x <= (tower_attacker.x + tower_attacker.w)){
                    knight.y+=1.5;
                    knight_running_attacking_ending_scene = true;
                    knight.x -= 8 * ScaleX; // Move left

                            // Update animation frames
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime > lastFrameTime + frameDelay) {
                        if (knight_running_attacking_ending_scene) {
                            currentFrame++;
                        if(sound){
                            Mix_PlayChannel(4, towerDamage_sound, 0); 
                        }
                            // Loop walking animation
                            if (currentFrame >= KNIGHT_TOTAL_RUN_ATTACK_FRAME) {
                                currentFrame = 0;
                            }
                        }
                            lastFrameTime = currentTime;
                    }
                    if(knight.x <= (tower_attacker.x - 20)/1920){
                        knight.y = archerY;
                        knight_running_attacking_ending_scene = false;
                        knight_running_ending_scene = false;
                        knight_moving_level1_ending = false;
                        knight_standing_ending_scene = true;
                        tower_attacker_health = 0;
                        tower_health_box.w = 0;
                        midPause_dialog_starting = true;
                        archer_level1_ending_dialog = true;
                        currentFrame = 0;
                    }
                }
                       
            }
        }

        // Tower Game Level 1
        if(towerGame_Started_level1 && !midPause && !frame_Video){
            // Handle frame updates for animation
            Uint32 currentTime = SDL_GetTicks();
            if ((archer_moving_left || archer_moving_right || archer_aiming || archer_shooting || archer_standing) && currentTime > lastFrameTime + frameDelay) {
                if((archer_aiming && currentFrame <= TOTAL_FIRE_FRAMES - 4)||(archer_moving_left || archer_moving_right)||(archer_shooting && (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<= TOTAL_FIRE_FRAMES)) || (archer_standing)){
                    currentFrame++;
                }
                if(archer_shooting){
                    if (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<=TOTAL_FIRE_FRAMES) {
                        currentFrame = 0; 
                        archer_shooting = false;
                        // archer_standing = true;
                    }
                }
                else if(archer_moving_left || archer_moving_right){
                    // Handle walking animation frames
                    if (currentFrame >= TOTAL_WALK_FRAMES) {
                        currentFrame = 0;
                    }
                }else if(archer_standing){
                    if(currentFrame >= TOTAL_STAND_FRAME){
                        currentFrame = 0;
                    }
                }
                lastFrameTime = currentTime;
            }

            if (archer.active) {
                archer.y +=  archer.vy; 
                archer.vy += 0.5*ScaleY; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }
            tower_attack_timer += deltaTime;  // Update the timer
            
            if (tower_attack_timer >= tower_attack_delay) {
                if(!tower_bomb.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    // tower_bomb.vx += 20 * ScaleX;
                    // tower_bomb.vy += -18 * ScaleY;
                    // tower_bomb.active = true;
                    tower_bomb.vx += archer.x/80;
                    tower_bomb.vy += -18 * ScaleY;
                    tower_bomb.active = true;
                }
            }
            // tower_bomb.active = true;
            
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx; 
                tower_bomb.y += tower_bomb.vy; 
                tower_bomb.vy += 0.5 * ScaleY;
                if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health<=10){
                            level1_ending_scene = true;
                            knight_level1_ending_dialog = true;
                            midPause = true;
                            midPause_dialog_starting = true;
                            archer_showing_ending_scene = true;
                            // !
                            tower_bomb.vx = 0;
                            tower_bomb.vy = 0;
                            tower_bomb.x = tower_attacker.x;
                            tower_bomb.y = tower_attacker.y;
                            tower_attack_timer = 0.0f;
                            tower_bomb.active = false;
                            // !
                            archer_arrow.vx = 0;
                            archer_arrow.vy = 0;
                            archer_arrow.active = false;  
                            archer_arrow.angle = 0;  
                        }else if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        // if(archer_health<=0){
                        //     archer_health_box.w = 0;
                        //     gameOver = true;
                        // }
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
                archer_arrow.vy += 0.5*ScaleY;       
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy <= -25*ScaleY) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20*ScaleY) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15*ScaleY) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10*ScaleY) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5*ScaleY) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0*ScaleY) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5*ScaleY) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10*ScaleY) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15*ScaleY) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20*ScaleY) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }



                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height, towerWidth, towerHeight)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health<=10){
                            level1_ending_scene = true;
                            knight_level1_ending_dialog = true;
                            midPause = true;
                            midPause_dialog_starting = true;
                            archer_showing_ending_scene = true;
                            // !
                            tower_bomb.vx = 0;
                            tower_bomb.vy = 0;
                            tower_bomb.x = tower_attacker.x;
                            tower_bomb.y = tower_attacker.y;
                            tower_attack_timer = 0.0f;
                            tower_bomb.active = false;
                            // !
                            archer_arrow.vx = 0;
                            archer_arrow.vy = 0;
                            archer_arrow.active = false;  
                            archer_arrow.angle = 0;  
                        }else if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*100)/1920;
                            tower_attacker_health-=10;
                        }
                        // if(tower_attacker_health<=0){
                        //     tower_health_box.w = 0;
                        //     youWin = true;
                        // }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
                }
            }
        }else if(towerGame_Started_level2 && !midPause && !frame_Video){
            
            // Handle frame updates for animation
            Uint32 currentTime = SDL_GetTicks();
            if(archerclicked){
                if ((archer_moving_left || archer_moving_right || archer_aiming || archer_shooting || archer_standing) && currentTime > lastFrameTime + frameDelay) {
                    if((archer_aiming && currentFrame <= TOTAL_FIRE_FRAMES - 4)||(archer_moving_left || archer_moving_right)||(archer_shooting && (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<= TOTAL_FIRE_FRAMES)) || (archer_standing)){
                        currentFrame++;
                    }
                    if(archer_shooting){
                        if (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<=TOTAL_FIRE_FRAMES) {
                            currentFrame = 0; 
                            archer_shooting = false;
                            // archer_standing = true;
                        }
                    }
                    else if(archer_moving_left || archer_moving_right){
                        // Handle walking animation frames
                        if (currentFrame >= TOTAL_WALK_FRAMES) {
                            currentFrame = 0;
                        }
                    }else if(archer_standing){
                        if(currentFrame >= TOTAL_STAND_FRAME){
                            currentFrame = 0;
                        }
                    }
                    lastFrameTime = currentTime;
                }
            }

            if(knight.spawn&&knightclicked){
                // Handle frame updates for animation
                Uint32 currentTime = SDL_GetTicks();
                knight_shooting_timer += deltaTime;
                if ((knight_moving_left || knight_moving_right || knight_shooting) && currentTime > lastFrameTime_Knight + frameDelay) {
                    if(knight_moving_left || knight_moving_right||knight_shooting){
                        currentFrame_Knight++;
                    }
                    if(knight_shooting){
                        if (currentFrame_Knight >=  KNIGHT_TOTAL_RUN_ATTACK_FRAME) {
                            currentFrame_Knight = 0; 
                            archer_shooting = false;
                            // archer_standing = true;
                        }
                    }
                    else if(knight_moving_left || knight_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Knight >= KNIGHT_TOTAL_RUN_FRAME) {
                            currentFrame_Knight = 0;
                        }
                    }
                    lastFrameTime_Knight = currentTime;
                }
            }

            if (archer.active) {
                archer.y +=  archer.vy; 
                archer.vy += 0.5*ScaleY; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }

            if (knight.active) {
                knight.y +=  knight.vy; 
                knight.vy += 0.5*ScaleY; 
                

                if (knight.y >= archerY) {
                    knight.y = archerY; 
                    knight.vy = 0; 
                    knight.active = false; 
                }
            }

            // Tower Attacker Timer Update
            tower_attack_timer += deltaTime;  // Update the timer

            // Hot Air Balloon Bomb Timer Update
            hot_air_balloon_bomb_attack_timer += deltaTime;  // Update the timer
            
            if (tower_attack_timer >= tower_attack_delay) {
                if(!tower_bomb.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    // tower_bomb.vx += 20 * ScaleX;
                    if(knight.spawn){
                        tower_bomb.vx += knight.x/80;
                    }else{
                        tower_bomb.vx += archer.x/80;
                    }
                    tower_bomb.vy += -18 * ScaleY;
                    tower_bomb.active = true;
                }
            }
            
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx; 
                tower_bomb.y += tower_bomb.vy; 
                tower_bomb.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = tower_attacker.x;
                    tower_bomb.y = tower_attacker.y;
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
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

            if(hot_air_balloon_health>0){

                if (hot_air_balloon_bomb_attack_timer >= hot_air_balloon_bomb_attack_delay) {
                    if(!hot_air_balloon_bomb.active){
                        if(sound){
                            Mix_PlayChannel(3, towerShooting_sound, 0); 
                        }
                        // tower_bomb.vx += 20 * ScaleX;
                        hot_air_balloon_bomb.vx += archer.x/80;
                        hot_air_balloon_bomb.vy += -12 * ScaleY;
                        hot_air_balloon_bomb.active = true;
                        hot_air_balloon_throw_bomb = true;
                    }
                }

                if(hot_air_balloon_bomb.active){
                    hot_air_balloon_bomb.x += hot_air_balloon_bomb.vx; 
                    hot_air_balloon_bomb.y += hot_air_balloon_bomb.vy; 
                    hot_air_balloon_bomb.vy += 0.5 * ScaleY;
                    if(hot_air_balloon_bomb.x > Windows_Width || hot_air_balloon_bomb.y > Windows_Height || checkCollisionArcher(&hot_air_balloon_bomb, &archer, archer_basic_Width, archer_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                        if(checkCollisionArcher(&hot_air_balloon_bomb, &archer, archer_basic_Width, archer_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                            hot_air_balloon_bomb_exploide = true;
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(archer_health > 0){
                                archer_health_box.w -= (Windows_Width*5)/1920;  
                                archer_health -= 5;
                            }
                            if(archer_health<=0){
                                archer_health_box.w = 0;
                                gameOver = true;
                            }
                        }
                        hot_air_balloon_bomb.vx = 0;
                        hot_air_balloon_bomb.vy = 0;
                        hot_air_balloon_bomb.x = hot_air_balloon_rect.x;
                        hot_air_balloon_bomb.y = hot_air_balloon_rect.y;
                        hot_air_balloon_bomb_attack_timer = 0.0f;
                        hot_air_balloon_bomb.active = false;
                        hot_air_balloon_bomb_exploide = false;
                        hot_air_balloon_throw_bomb = false;
                    }
                }
            }

            if(archer_arrow.active){
                archer_arrow.x += archer_arrow.vx;
                archer_arrow.y += archer_arrow.vy;
                archer_arrow.vy += 0.5*ScaleY;     
                archerlastvelX =  (archer_arrow.vx !=0)? archer_arrow.vx:archerlastvelX;
                archerlastvelY =  (archer_arrow.vy !=0)? archer_arrow.vy:archerlastvelY;  
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy <= -25*ScaleY) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20*ScaleY) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15*ScaleY) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10*ScaleY) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5*ScaleY) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0*ScaleY) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5*ScaleY) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10*ScaleY) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15*ScaleY) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20*ScaleY) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }


                
                if(hot_air_balloon_health>0){
                    if(checkCollisionTower(&archer_arrow, (int)hot_air_balloon_rect.x, (int)hot_air_balloon_rect.y, archer_arrow_basic_Width, archer_arrow_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                         if(hot_air_balloon_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            hot_air_ballon_health_box.w -= (Windows_Width*5)/1920;
                            hot_air_balloon_health-=5;
                        }
                        if(hot_air_balloon_health<=0){
                            hot_air_ballon_health_box.w = 0;
                        }
                        archer_arrow.vx = 0;
                        archer_arrow.vy = 0;
                        archer_arrow.active = false;  
                        archer_arrow.angle = 0;  
                    }
                }

                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height, towerWidth, towerHeight)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
                }
            }
            
            if(knight_attack){
                if(checkCollisionCharacter(&knight, (int)tower_attacker.x, (int)tower_attacker.y, knight_basic_Width, knight_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                            knight_attack = false;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            tower_attacker_health = 0;
                            youWin = true;
                            knight_shooting = false;  
                            knight_standing = true;
                        }
                    }
                }
        }else if(towerGame_Started_level3 && !midPause && !frame_Video){
            
            // Handle frame updates for animation
            Uint32 currentTime = SDL_GetTicks();
            if(archerclicked){
                if ((archer_moving_left || archer_moving_right || archer_aiming || archer_shooting || archer_standing) && currentTime > lastFrameTime + frameDelay) {
                if((archer_aiming && currentFrame <= TOTAL_FIRE_FRAMES - 4)||(archer_moving_left || archer_moving_right)||(archer_shooting && (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<= TOTAL_FIRE_FRAMES)) || (archer_standing)){
                    currentFrame++;
                }
                if(archer_shooting){
                    if (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<=TOTAL_FIRE_FRAMES) {
                        currentFrame = 0; 
                        archer_shooting = false;
                        // archer_standing = true;
                    }
                }
                else if(archer_moving_left || archer_moving_right){
                    // Handle walking animation frames
                    if (currentFrame >= TOTAL_WALK_FRAMES) {
                        currentFrame = 0;
                    }
                }else if(archer_standing){
                    if(currentFrame >= TOTAL_STAND_FRAME){
                        currentFrame = 0;
                    }
                }
                lastFrameTime = currentTime;
            }
            }

            if(knight.spawn&&knightclicked){
                knight_shooting_timer += deltaTime;
                Uint32 currentTime = SDL_GetTicks();
                if ((knight_moving_left || knight_moving_right || knight_shooting || knight_standing) && currentTime > lastFrameTime_Knight + frameDelay) {
                    if(knight_moving_left || knight_moving_right||knight_shooting || knight_standing){
                        currentFrame_Knight++;
                    }
                    if(knight_shooting){
                        if (currentFrame_Knight >=  KNIGHT_TOTAL_RUN_ATTACK_FRAME) {
                            currentFrame_Knight = 0; 
                            archer_shooting = false;
                            // archer_standing = true;
                        }
                    }
                    else if(knight_moving_left || knight_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Knight >= KNIGHT_TOTAL_RUN_FRAME) {
                            currentFrame_Knight = 0;
                        }
                    }
                    lastFrameTime_Knight = currentTime;
                }
            }

            if(mille.spawn&&milleclicked){
             Uint32 currentTime = SDL_GetTicks();
                if ((mille_moving_left || mille_moving_right || mille_shooting || mille_standing) && currentTime > lastFrameTime_Mille + frameDelay) {
                    if(mille_moving_left || mille_moving_right||(mille_shooting) || mille_standing){
                        currentFrame_Mille++;
                    }

                    if(mille_shooting){
                        if (currentFrame_Mille >=  MILLE_ATTACKING_TOTAL_FRAME) {
                            currentFrame_Mille = 0; 
                            mille_shooting = false;
                        }
                    }
                    else if(mille_moving_left || mille_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Mille >= MILLE_RUNNING_TOTAL_FRAME) {
                            currentFrame_Mille = 0;
                        }
                    }
                    lastFrameTime_Mille = currentTime;
                }
                if(mille.active) {
                mille.y +=  mille.vy; 
                mille.vy += 0.5*ScaleY; 
                

                if (mille.y >= milleY) {
                    mille.y = milleY; 
                    mille.vy = 0; 
                    mille.active = false; 
                }
                }
            }

            if(mille_laser.active){
                mille_laser.x -= 18*ScaleX;

                if(mille_laser.x > Windows_Width || mille_laser.y > Windows_Height || checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level3_towerWidth, level3_towerHeight) || checkCollisionTower(&mille_laser, (int)wolf.x, (int)wolf.y, mille_laser_Width, mille_laser_Width,wolf_basic_Width, wolf_basic_Height)){
                    if(checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level3_towerWidth, level3_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*30)/1920;
                            tower_attacker_health-=3;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }
                    if(checkCollisionTower(&mille_laser, (int)wolf.x, (int)wolf.y, mille_laser_Width, mille_laser_Width,wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                        }
                        if(wolf_health<=0){
                            wolf_health_box.w = character_health_Width;
                            wolf_health = 100;
                            wolf.spawn = false;
                            wolf_attacking = false;
                            wolf_running = true;
                            wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                            currentFrame_Enemy = 0;
                        }
                    }
                    mille_laser.vx = 0;
                    mille_laser.x = mille.x;
                    mille_laser.active = false;
                }
            }

            if (archer.active) {
                archer.y +=  archer.vy; 
                archer.vy += 0.5*ScaleY; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }

            if (knight.active) {
                knight.y +=  knight.vy; 
                knight.vy += 0.5*ScaleY; 
                

                if (knight.y >= archerY) {
                    knight.y = archerY; 
                    knight.vy = 0; 
                    knight.active = false; 
                }
            }

            // Tower Attacker Timer Update
            tower_attack_timer += deltaTime;  // Update the timer

            // Tower Attacker 2
            tower_bomb_2_Timer += deltaTime;
            
            if (tower_attack_timer >= tower_attack_delay) {
                if(!tower_bomb.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    // tower_bomb.vx += 20 * ScaleX;
                    if(knight.spawn){
                        tower_bomb.vx += knight.x/80;
                    }else if(mille.spawn){
                        tower_bomb.vx += mille.x/100;
                    }else{
                        tower_bomb.vx += archer.x/80;
                    }
                    tower_bomb.vy += -18 * ScaleY;
                    tower_bomb.active = true;
                }
            }

            if (tower_bomb_2_Timer >= tower_bomb_2_attack_delay) {
                if(!tower_bomb_2.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_2.vx += knight.x/80;
                    }else if(mille.spawn){
                        tower_bomb_2.vx += mille.x/80;
                    }else{
                        tower_bomb_2.vx += archer.x/80;
                    }
                    tower_bomb_2.vy += -18 * ScaleY;
                    tower_bomb_2.active = true;
                }
            }

            if(tower_bomb_2.active){
                tower_bomb_2.x += tower_bomb_2.vx; 
                tower_bomb_2.y += tower_bomb_2.vy; 
                tower_bomb_2.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_2, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*500)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(mille.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_2, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*20)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                            mille.spawn = false;
                            currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*500)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*350)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }
            }
            
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx; 
                tower_bomb.y += tower_bomb.vy; 
                tower_bomb.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(mille.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &mille, mille_basic_Width, mille_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                            mille.spawn = false;
                            currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }
            }

            if(!wolf.spawn){
                // Tower Attacker Timer Update
                wolf_spawn_timer += deltaTime;  // Update the timer
                if (wolf_spawn_timer >= wolf_spawn_delay) {
                    wolf.spawn = true;
                    wolf_health = 100;
                    wolf_health_box.w =character_health_Width;
                    wolf_running = true;
                    wolf_attacking = false;
                    wolf_spawn_timer = 0.0f;
                }
            }

            if(wolf.spawn){
                // Handle frame updates for animation
                Uint32 currentTime = SDL_GetTicks();
                    if(checkCollisionEnemy(&wolf, &archer, archer_basic_Width, archer_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                        wolf_running = false;
                        wolf_attacking = true;
                        if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                            currentFrame_Enemy++;
                            if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                currentFrame_Enemy = 0; 
                            }
                            lastFrameTime_Enemy = currentTime;
                        }
                        if(wolf_attack_timer>=wolf_attack_delay){
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(archer_health > 0){
                                archer_health_box.w -= (Windows_Width*5)/1920;  
                                archer_health -= 5;
                            }
                            wolf_attack_timer = 0.0f;
                        }else{
                            wolf_attack_timer += deltaTime;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    else if(knight.spawn&&checkCollisionEnemy(&wolf, &knight, knight_basic_Width, knight_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                            wolf_running = false;
                            wolf_attacking = true;
                            if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                                currentFrame_Enemy++;
                                if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                    currentFrame_Enemy = 0; 
                                }
                                lastFrameTime_Enemy = currentTime;
                            }
                            if(wolf_attack_timer>=wolf_attack_delay){
                                if(sound){
                                    Mix_PlayChannel(4, archerDamage_sound, 0); 
                                }
                                if(knight_health > 0){
                                    knight_health_box.w -= (Windows_Width*5)/1920;  
                                    knight_health -= 5;
                                }
                                wolf_attack_timer = 0.0f;
                            }else{
                                wolf_attack_timer += deltaTime;
                            }
                            if(knight_health<=0){
                                knight_health_box.w = 0;
                                knight.spawn = false;
                                currentFrame_Knight = 0;
                            }
                        
                    }else if(mille.spawn && checkCollisionEnemy(&wolf, &mille, mille_basic_Width, mille_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                            wolf_running = false;
                            wolf_attacking = true;
                            if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                                currentFrame_Enemy++;
                                if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                    currentFrame_Enemy = 0; 
                                }
                                lastFrameTime_Enemy = currentTime;
                            }
                            if(wolf_attack_timer>=wolf_attack_delay){
                                if(sound){
                                    Mix_PlayChannel(4, archerDamage_sound, 0); 
                                }
                                if(mille_health > 0){
                                    mille_health_box.w -= (Windows_Width*5)/1920;  
                                    mille_health -= 5;
                                }
                                wolf_attack_timer = 0.0f;
                            }else{
                                wolf_attack_timer += deltaTime;
                            }
                            if(mille_health<=0){
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                                wolf_health_box.w = character_health_Width;
                                wolf_health = 100;
                                wolf.spawn = false;
                                wolf_attacking = false;
                                wolf_running = true;
                                wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                                currentFrame_Enemy = 0;
                            }
                        
                    }
                    else{
                        if (wolf_running && currentTime > lastFrameTime_Enemy + frameDelay) {
                            currentFrame_Enemy++;
                            if (currentFrame_Enemy >=  WOLF_TOTAL_RUN_FRAMES) {
                                currentFrame_Enemy = 0; 
                            }

                            lastFrameTime_Enemy = currentTime;
                        }
                        wolf.x += 8*ScaleX; 
                        wolf_attacking = false;
                        wolf_running = true;
                        
                    
                    if(wolf.x >= wolf_basic_Width+Windows_Width){
                        wolf.spawn = false;
                        wolf_attacking = false;
                        wolf_running = true;
                        wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                        currentFrame_Enemy = 0;
                    }
                }
            }
            

            if(archer_arrow.active){
                archer_arrow.x += archer_arrow.vx;
                archer_arrow.y += archer_arrow.vy;
                archer_arrow.vy += 0.5*ScaleY;     
                archerlastvelX =  (archer_arrow.vx !=0)? archer_arrow.vx:archerlastvelX;
                archerlastvelY =  (archer_arrow.vy !=0)? archer_arrow.vy:archerlastvelY;  
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy <= -25*ScaleY) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20*ScaleY) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15*ScaleY) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10*ScaleY) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5*ScaleY) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0*ScaleY) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5*ScaleY) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10*ScaleY) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15*ScaleY) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20*ScaleY) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }

                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height,level3_towerWidth, level3_towerHeight) || checkCollisionTower(&archer_arrow, (int)wolf.x, (int)wolf.y, archer_arrow_basic_Width, archer_arrow_basic_Height,wolf_basic_Width, wolf_basic_Height)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height,level3_towerWidth, level3_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }else if(checkCollisionTower(&archer_arrow, (int)wolf.x, (int)wolf.y, archer_arrow_basic_Width, archer_arrow_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*50)/1920;
                            wolf_health-=5;
                        }
                        if(wolf_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
                }
            }
            if(knight_attack){
                if(checkCollisionCharacter(&knight, (int)tower_attacker.x, (int)tower_attacker.y, knight_basic_Width, knight_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                            knight_attack = false;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            tower_attacker_health = 0;
                            youWin = true;
                            knight_shooting = false;  
                            knight_standing = true;
                        }
                    }else if(checkCollisionCharacter(&knight, (int)wolf.x, (int)wolf.y, knight_basic_Width, knight_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                            knight_attack = false;
                        }
                        if(wolf_health<=0){
                            wolf_health_box.w = character_health_Width;
                            wolf_health = 100;
                            wolf.spawn = false;
                            wolf_attacking = false;
                            wolf_running = true;
                            wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                            currentFrame_Enemy = 0;
                        }
                    }
                }
        
        }else if(towerGame_Started_level4 && !midPause && !frame_Video){
           // Handle frame updates for animation
            Uint32 currentTime = SDL_GetTicks();
            if(archerclicked){
                if ((archer_moving_left || archer_moving_right || archer_aiming || archer_shooting || archer_standing) && currentTime > lastFrameTime + frameDelay) {
                if((archer_aiming && currentFrame <= TOTAL_FIRE_FRAMES - 4)||(archer_moving_left || archer_moving_right)||(archer_shooting && (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<= TOTAL_FIRE_FRAMES)) || (archer_standing)){
                    currentFrame++;
                }
                if(archer_shooting){
                    if (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<=TOTAL_FIRE_FRAMES) {
                        currentFrame = 0; 
                        archer_shooting = false;
                        // archer_standing = true;
                    }
                }
                else if(archer_moving_left || archer_moving_right){
                    // Handle walking animation frames
                    if (currentFrame >= TOTAL_WALK_FRAMES) {
                        currentFrame = 0;
                    }
                }else if(archer_standing){
                    if(currentFrame >= TOTAL_STAND_FRAME){
                        currentFrame = 0;
                    }
                }
                lastFrameTime = currentTime;
            }
            }

            if(knight.spawn&&knightclicked){
                Uint32 currentTime = SDL_GetTicks();
                knight_shooting_timer += deltaTime;
                if ((knight_moving_left || knight_moving_right || knight_shooting || knight_standing) && currentTime > lastFrameTime_Knight + frameDelay) {
                    if(knight_moving_left || knight_moving_right||knight_shooting || knight_standing){
                        currentFrame_Knight++;
                    }
                    if(knight_shooting){
                        if (currentFrame_Knight >=  KNIGHT_TOTAL_RUN_ATTACK_FRAME) {
                            currentFrame_Knight = 0; 
                            archer_shooting = false;
                            // archer_standing = true;
                        }
                    }
                    else if(knight_moving_left || knight_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Knight >= KNIGHT_TOTAL_RUN_FRAME) {
                            currentFrame_Knight = 0;
                        }
                    }
                    lastFrameTime_Knight = currentTime;
                }
            }

            if(mille.spawn&&milleclicked){
             Uint32 currentTime = SDL_GetTicks();
                if ((mille_moving_left || mille_moving_right || mille_shooting || mille_standing) && currentTime > lastFrameTime_Mille + frameDelay) {
                    if(mille_moving_left || mille_moving_right||mille_shooting || mille_standing){
                        currentFrame_Mille++;
                    }

                    if(mille_shooting){
                        if (currentFrame_Mille >=  MILLE_ATTACKING_TOTAL_FRAME) {
                            currentFrame_Mille = 0; 
                            mille_shooting = false;
                        }
                    }
                    else if(mille_moving_left || mille_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Mille >= MILLE_RUNNING_TOTAL_FRAME) {
                            currentFrame_Mille = 0;
                        }
                    }
                    lastFrameTime_Mille = currentTime;
                }
                if(mille.active) {
                mille.y +=  mille.vy; 
                mille.vy += 0.5*ScaleY; 
                

                if (mille.y >= milleY) {
                    mille.y = milleY; 
                    mille.vy = 0; 
                    mille.active = false; 
                }
                }
            }

            if(mille_laser.active){
                mille_laser.x -= 18*ScaleX;

                if(mille_laser.x > Windows_Width || mille_laser.y > Windows_Height || checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level4_towerWidth, level4_towerHeight)){
                    if(checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level4_towerWidth, level4_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*20)/1920;
                            tower_attacker_health-=2;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }
                    if(checkCollisionTower(&mille_laser, (int)wolf.x, (int)wolf.y, mille_laser_Width, mille_laser_Width,wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                        }
                        if(wolf_health<=0){
                            wolf_health_box.w = character_health_Width;
                            wolf_health = 100;
                            wolf.spawn = false;
                            wolf_attacking = false;
                            wolf_running = true;
                            wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                            currentFrame_Enemy = 0;
                        }
                    }
                    mille_laser.vx = 0;
                    mille_laser.x = mille.x;
                    mille_laser.active = false;
                }
            }

             if(soldier.spawn&&soldierclicked){
             Uint32 currentTime = SDL_GetTicks();
                if ((soldier_moving_left || soldier_moving_right || soldier_shooting || soldier_standing) && currentTime > lastFrameTime_Soldier + frameDelay) {
                    if(soldier_moving_left || soldier_moving_right||soldier_shooting || soldier_standing){
                        currentFrame_Soldier++;
                    }

                    if(soldier_shooting){
                        if (currentFrame_Soldier >= SOLDIER_ATTACKING_TOTAL_FRAME) {
                            currentFrame_Soldier = 0; 
                            soldier_shooting = false;
                        }
                    }
                    else if(soldier_moving_left || soldier_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Soldier >= SOLDIER_RUNNING_TOTAL_FRAME) {
                            currentFrame_Soldier = 0;
                        }
                    }
                    lastFrameTime_Soldier = currentTime;
                }
                if(soldier.active) {
                soldier.y +=  soldier.vy; 
                soldier.vy += 0.5*ScaleY; 
                

                if (soldier.y >= Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY))) {
                    soldier.y = Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY)); 
                    soldier.vy = 0; 
                    soldier.active = false; 
                }
                }
            }

            if(soldier_bullet.active){
                    soldier_bullet.x -= 18*ScaleX;

                    if(soldier_bullet.x > Windows_Width || soldier_bullet.y > Windows_Height || checkCollisionTower(&soldier_bullet, (int)tower_attacker.x, (int)tower_attacker.y, soldier_bullet_Width, soldier_bullet_Height,level4_towerWidth, level4_towerHeight)){
                        if(checkCollisionTower(&soldier_bullet, (int)tower_attacker.x, (int)tower_attacker.y, soldier_basic_Width, soldier_bullet_Height,level4_towerWidth, level4_towerHeight)){
                            if(tower_attacker_health>0){
                                if(sound){
                                    Mix_PlayChannel(4, towerDamage_sound, 0); 
                                }
                                tower_health_box.w -= (Windows_Width*30)/1920;
                                tower_attacker_health-=3;
                            }
                            if(tower_attacker_health<=0){
                                tower_health_box.w = 0;
                                youWin = true;
                            }
                        }
                        soldier_bullet.vx = 0;
                        soldier_bullet.x = soldier.x;
                        soldier_bullet.active = false;
                    }
                }

            if (archer.active) {
                archer.y +=  archer.vy; 
                archer.vy += 0.5*ScaleY; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }

            if (knight.active) {
                knight.y +=  knight.vy; 
                knight.vy += 0.5*ScaleY; 
                

                if (knight.y >= archerY) {
                    knight.y = archerY; 
                    knight.vy = 0; 
                    knight.active = false; 
                }
            }

            // Tower Attacker Timer Update
            tower_attack_timer += deltaTime;  // Update the timer

            // Tower Attacker 2
            tower_bomb_2_Timer += deltaTime;

            // Tower Attacker 3
            tower_bomb_3_Timer += deltaTime;
            
            if (tower_attack_timer >= tower_attack_delay) {
                if(!tower_bomb.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    // tower_bomb.vx += 20 * ScaleX;
                    if(knight.spawn){
                        tower_bomb.vx += knight.x/80;
                    }else if(mille.spawn){
                        tower_bomb.vx += mille.x/100;
                    }else if(soldier.spawn){
                        tower_bomb.vx += soldier.x/80;
                    }else{
                        tower_bomb.vx += archer.x/80;
                    }
                    tower_bomb.vy += -18 * ScaleY;
                    tower_bomb.active = true;
                }
            }

            if (tower_bomb_2_Timer >= tower_bomb_2_attack_delay) {
                if(!tower_bomb_2.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_2.vx += knight.x/80;
                    }else if(mille.spawn){
                        tower_bomb_2.vx += mille.x/100;
                    }else if(soldier.spawn){
                        tower_bomb_2.vx += soldier.x/80;
                    }else{
                        tower_bomb_2.vx += archer.x/80;
                    }
                    tower_bomb_2.vy += -18 * ScaleY;
                    tower_bomb_2.active = true;
                }
            }

            if (tower_bomb_3_Timer >= tower_bomb_3_attack_delay) {
                if(!tower_bomb_3.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_3.vx += knight.x/80;
                    }else if(mille.spawn){
                        tower_bomb_3.vx += mille.x/80;
                    }else if(soldier.spawn){
                        tower_bomb_3.vx += soldier.x/80;
                    }else{
                        tower_bomb_3.vx += archer.x/80;
                    }
                    tower_bomb_3.vy += -18 * ScaleY;
                    tower_bomb_3.active = true;
                }
            }

            if(tower_bomb_3.active){
                tower_bomb_3.x += tower_bomb_3.vx; 
                tower_bomb_3.y += tower_bomb_3.vy; 
                tower_bomb_3.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*270)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(mille.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                            mille.spawn = false;
                            currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*270)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(soldier.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*10)/1920;  
                            soldier_health -= 10;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                            soldier.spawn = false;
                            currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*270)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_3, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*270)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }
            }
            if(tower_bomb_2.active){
                tower_bomb_2.x += tower_bomb_2.vx; 
                tower_bomb_2.y += tower_bomb_2.vy; 
                tower_bomb_2.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(mille.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                            mille.spawn = false;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(soldier.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*10)/1920;  
                            soldier_health -= 10;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                            soldier.spawn = false;
                            currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*80)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }
            }
            
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx; 
                tower_bomb.y += tower_bomb.vy; 
                tower_bomb.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                            currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(mille.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                            mille.spawn = false;
                            currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(soldier.spawn && (tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*10)/1920;  
                            soldier_health -= 10;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                            soldier.spawn = false;
                            currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }else if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb.vx = 0;
                    tower_bomb.vy = 0;
                    tower_bomb.x = (float)tower_attacker.x + (Windows_Width*50)/1920; 
                    tower_bomb.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_attack_timer = 0.0f;
                    tower_bomb.active = false;
                }
            }

            if(archer_arrow.active){
                archer_arrow.x += archer_arrow.vx;
                archer_arrow.y += archer_arrow.vy;
                archer_arrow.vy += 0.5*ScaleY;     
                archerlastvelX =  (archer_arrow.vx !=0)? archer_arrow.vx:archerlastvelX;
                archerlastvelY =  (archer_arrow.vy !=0)? archer_arrow.vy:archerlastvelY;  
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy <= -25*ScaleY) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20*ScaleY) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15*ScaleY) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10*ScaleY) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5*ScaleY) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0*ScaleY) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5*ScaleY) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10*ScaleY) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15*ScaleY) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20*ScaleY) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }

                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height,level4_towerWidth, level4_towerHeight)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height,level4_towerWidth, level4_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            youWin = true;
                        }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
                }
            }
            if(knight_attack){
                if(checkCollisionCharacter(&knight, (int)tower_attacker.x, (int)tower_attacker.y, knight_basic_Width, knight_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                            knight_attack = false;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            tower_attacker_health = 0;
                            youWin = true;
                            knight_shooting = false;  
                            knight_standing = true;
                        }
                    }
                }
        
        }else if(towerGame_Started_level5 && !midPause && !frame_Video){
           // Handle frame updates for animation
            Uint32 currentTime = SDL_GetTicks();
            if(archerclicked){
                if ((archer_moving_left || archer_moving_right || archer_aiming || archer_shooting || archer_standing) && currentTime > lastFrameTime + frameDelay) {
                if((archer_aiming && currentFrame <= TOTAL_FIRE_FRAMES - 4)||(archer_moving_left || archer_moving_right)||(archer_shooting && (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<= TOTAL_FIRE_FRAMES)) || (archer_standing)){
                    currentFrame++;
                }
                if(archer_shooting){
                    if (currentFrame >= TOTAL_FIRE_FRAMES-4 && currentFrame<=TOTAL_FIRE_FRAMES) {
                        currentFrame = 0; 
                        archer_shooting = false;
                        // archer_standing = true;
                    }
                }
                else if(archer_moving_left || archer_moving_right){
                    // Handle walking animation frames
                    if (currentFrame >= TOTAL_WALK_FRAMES) {
                        currentFrame = 0;
                    }
                }else if(archer_standing){
                    if(currentFrame >= TOTAL_STAND_FRAME){
                        currentFrame = 0;
                    }
                }
                lastFrameTime = currentTime;
            }
            }

            if(knight.spawn&&knightclicked){
                knight_shooting_timer += deltaTime;
                if ((knight_moving_left || knight_moving_right || knight_shooting || knight_standing) && currentTime > lastFrameTime_Knight + frameDelay) {
                    if(knight_moving_left || knight_moving_right||knight_shooting || knight_standing){
                        currentFrame_Knight++;
                    }
                    if(knight_shooting){
                        if (currentFrame_Knight >=  KNIGHT_TOTAL_RUN_ATTACK_FRAME) {
                            currentFrame_Knight = 0; 
                            archer_shooting = false;
                            // archer_standing = true;
                        }
                    }
                    else if(knight_moving_left || knight_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Knight >= KNIGHT_TOTAL_RUN_FRAME) {
                            currentFrame_Knight = 0;
                        }
                    }
                    
                    lastFrameTime_Knight = currentTime;
                }
            }

            if(mille.spawn&&milleclicked){
             Uint32 currentTime = SDL_GetTicks();
                if ((mille_moving_left || mille_moving_right || mille_shooting || mille_standing) && currentTime > lastFrameTime_Mille + frameDelay) {
                    if(mille_moving_left || mille_moving_right||(mille_shooting) || mille_standing){
                        currentFrame_Mille++;
                    }

                    if(mille_shooting){
                        if (currentFrame_Mille >=  MILLE_ATTACKING_TOTAL_FRAME) {
                            currentFrame_Mille = 0; 
                            mille_shooting = false;
                        }
                    }
                    else if(mille_moving_left || mille_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Mille >= MILLE_RUNNING_TOTAL_FRAME) {
                            currentFrame_Mille = 0;
                        }
                    }
                    lastFrameTime_Mille = currentTime;
                }
                if(mille.active) {
                mille.y +=  mille.vy; 
                mille.vy += 0.5*ScaleY; 
                

                if (mille.y >= milleY) {
                    mille.y = milleY; 
                    mille.vy = 0; 
                    mille.active = false; 
                }
                }
            }

            if(mille_laser.active){
                mille_laser.x -= 18*ScaleX;

                if(mille_laser.x > Windows_Width || mille_laser.y > Windows_Height || checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level5_towerWidth, level5_towerHeight) || checkCollisionTower(&mille_laser, (int)wolf.x, (int)wolf.y, mille_laser_Width, mille_laser_Width,wolf_basic_Width, wolf_basic_Height)){
                    if(checkCollisionTower(&mille_laser, (int)tower_attacker.x, (int)tower_attacker.y, mille_laser_Width, mille_laser_Height,level5_towerWidth, level5_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*20)/1920;
                            tower_attacker_health-=2;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            // youWin = true;
                            frame_Video = true;
                            final_vid = true;
                        }
                    }
                    if(checkCollisionTower(&mille_laser, (int)wolf.x, (int)wolf.y, mille_laser_Width, mille_laser_Width,wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                        }
                        if(wolf_health<=0){
                            wolf_health_box.w = character_health_Width;
                            wolf_health = 100;
                            wolf.spawn = false;
                            wolf_attacking = false;
                            wolf_running = true;
                            wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                            currentFrame_Enemy = 0;
                        }
                    }
                    mille_laser.vx = 0;
                    mille_laser.x = mille.x;
                    mille_laser.active = false;
                }
            }

             if(soldier.spawn&&soldierclicked){
                Uint32 currentTime = SDL_GetTicks();
                if ((soldier_moving_left || soldier_moving_right || soldier_shooting || soldier_standing) && currentTime > lastFrameTime_Soldier + frameDelay) {
                    if(soldier_moving_left || soldier_moving_right||soldier_shooting || soldier_standing){
                        currentFrame_Soldier++;
                    }

                    if(soldier_shooting){
                        if (currentFrame_Soldier >= SOLDIER_ATTACKING_TOTAL_FRAME) {
                            currentFrame_Soldier = 0; 
                            soldier_shooting = false;
                        }
                    }
                    else if(soldier_moving_left || soldier_moving_right){
                        // Handle walking animation frames
                        if (currentFrame_Soldier >= SOLDIER_RUNNING_TOTAL_FRAME) {
                            currentFrame_Soldier = 0;
                        }
                    }
                    lastFrameTime_Soldier = currentTime;
                }
                if(soldier.active) {
                soldier.y +=  soldier.vy; 
                soldier.vy += 0.5*ScaleY; 
                

                if (soldier.y >= Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY))) {
                    soldier.y = Windows_Height - (int)((150 * ScaleY) + (280 * ScaleY)); 
                    soldier.vy = 0; 
                    soldier.active = false; 
                }
                }
            }

                if(soldier_bullet.active){
                    soldier_bullet.x -= 18*ScaleX;

                    if(soldier_bullet.x > Windows_Width || soldier_bullet.y > Windows_Height || checkCollisionTower(&soldier_bullet, (int)tower_attacker.x, (int)tower_attacker.y, soldier_bullet_Width, soldier_bullet_Height,level5_towerWidth, level5_towerHeight) || checkCollisionTower(&soldier_bullet, (int)wolf.x, (int)wolf.y, soldier_bullet_Width, soldier_bullet_Height,wolf_basic_Width, wolf_basic_Height)){
                        if(checkCollisionTower(&soldier_bullet, (int)tower_attacker.x, (int)tower_attacker.y, soldier_basic_Width, soldier_bullet_Height,level5_towerWidth, level5_towerHeight)){
                            if(tower_attacker_health>0){
                                if(sound){
                                    Mix_PlayChannel(4, towerDamage_sound, 0); 
                                }
                                tower_health_box.w -= (Windows_Width*20)/1920;
                                tower_attacker_health-=2;
                            }
                            if(tower_attacker_health<=0){
                                tower_health_box.w = 0;
                                frame_Video = true;
                                final_vid = true;
                            }
                        }else if(checkCollisionTower(&soldier_bullet, (int)wolf.x, (int)wolf.y, soldier_bullet_Width, soldier_bullet_Height,wolf_basic_Width, wolf_basic_Height)){
                           if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                            }
                            if(wolf_health<=0){
                                wolf_health_box.w = character_health_Width;
                                wolf_health = 100;
                                wolf.spawn = false;
                                wolf_attacking = false;
                                wolf_running = true;
                                wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                                currentFrame_Enemy = 0;
                            }
                        }
                        soldier_bullet.vx = 0;
                        soldier_bullet.x = soldier.x;
                        soldier_bullet.active = false;
                    }
                }

            if (archer.active) {
                archer.y +=  archer.vy; 
                archer.vy += 0.5*ScaleY; 
                

                if (archer.y >= archerY) {
                    archer.y = archerY; 
                    archer.vy = 0; 
                    archer.active = false; 
                }
            }

            if (knight.active) {
                knight.y +=  knight.vy; 
                knight.vy += 0.5*ScaleY; 
                

                if (knight.y >= archerY) {
                    knight.y = archerY; 
                    knight.vy = 0; 
                    knight.active = false; 
                }
            }

            // Tower Attacker Timer Update
            tower_bomb_1_5_Timer += deltaTime;  // Update the timer

            // Tower Attacker 2
            tower_bomb_2_Timer += deltaTime;

            // Tower Attacker 3
            tower_bomb_3_Timer += deltaTime;

            // Tower Attacker 4
            tower_bomb_4_Timer += deltaTime;
            
            // Hot Air Balloon Bomb
            hot_air_balloon_bomb_attack_timer += deltaTime;

            if(tower_attacker_health>0){

                if (hot_air_balloon_bomb_attack_timer >= hot_air_balloon_bomb_attack_delay) {
                    if(!hot_air_balloon_bomb.active){
                        if(sound){
                            Mix_PlayChannel(3, towerShooting_sound, 0); 
                        }
                        // tower_bomb.vx += 20 * ScaleX;
                        if(knight.spawn){
                            hot_air_balloon_bomb.vx += knight.x/80;
                        }else if(mille.spawn){
                            hot_air_balloon_bomb.vx += mille.x/80;
                        }else if(soldier.spawn){
                             hot_air_balloon_bomb.vx += soldier.x/80;
                        }else{
                            hot_air_balloon_bomb.vx += archer.x/80;
                        }
                        hot_air_balloon_bomb.vy += -12 * ScaleY;
                        hot_air_balloon_bomb.active = true;
                        hot_air_balloon_throw_bomb = true;
                    }
                }

                if(hot_air_balloon_bomb.active){
                    hot_air_balloon_bomb.x += hot_air_balloon_bomb.vx; 
                    hot_air_balloon_bomb.y += hot_air_balloon_bomb.vy; 
                    hot_air_balloon_bomb.vy += 0.5 * ScaleY;
                    
                    if(hot_air_balloon_bomb.x > Windows_Width || hot_air_balloon_bomb.y > Windows_Height || checkCollisionArcher(&hot_air_balloon_bomb, &knight, knight_basic_Width, knight_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                        if(checkCollisionArcher(&hot_air_balloon_bomb, &knight, knight_basic_Width, knight_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                            hot_air_balloon_bomb_exploide = true;
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(knight_health > 0){
                                knight_health_box.w -= (Windows_Width*5)/1920;  
                                knight_health -= 5;
                            }
                            if(knight_health<=0){
                                knight_health_box.w = 0;
                                knight.spawn = false;
                                currentFrame_Knight = 0;
                            }
                        }
                        hot_air_balloon_bomb.vx = 0;
                        hot_air_balloon_bomb.vy = 0;
                        hot_air_balloon_bomb.x = hot_air_balloon_rect.x;
                        hot_air_balloon_bomb.y = hot_air_balloon_rect.y;
                        hot_air_balloon_bomb_attack_timer = 0.0f;
                        hot_air_balloon_bomb.active = false;
                        hot_air_balloon_bomb_exploide = false;
                        hot_air_balloon_throw_bomb = false;
                    }else if(hot_air_balloon_bomb.x > Windows_Width || hot_air_balloon_bomb.y > Windows_Height || checkCollisionArcher(&hot_air_balloon_bomb, &mille, mille_basic_Width, mille_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                        if(checkCollisionArcher(&hot_air_balloon_bomb, &mille, mille_basic_Width, mille_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                            hot_air_balloon_bomb_exploide = true;
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(mille_health > 0){
                                mille_health_box.w -= (Windows_Width*5)/1920;  
                                mille_health -= 5;
                            }
                            if(mille_health<=0){
                                mille_health_box.w = 0;
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                            }
                        }
                        hot_air_balloon_bomb.vx = 0;
                        hot_air_balloon_bomb.vy = 0;
                        hot_air_balloon_bomb.x = hot_air_balloon_rect.x;
                        hot_air_balloon_bomb.y = hot_air_balloon_rect.y;
                        hot_air_balloon_bomb_attack_timer = 0.0f;
                        hot_air_balloon_bomb.active = false;
                        hot_air_balloon_bomb_exploide = false;
                        hot_air_balloon_throw_bomb = false;
                    }else if(hot_air_balloon_bomb.x > Windows_Width || hot_air_balloon_bomb.y > Windows_Height || checkCollisionArcher(&hot_air_balloon_bomb, &soldier, soldier_basic_Width, soldier_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                        if(checkCollisionArcher(&hot_air_balloon_bomb, &soldier, soldier_basic_Width, soldier_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                            hot_air_balloon_bomb_exploide = true;
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(soldier_health > 0){
                                soldier_health_box.w -= (Windows_Width*5)/1920;  
                                soldier_health -= 5;
                            }
                            if(soldier_health<=0){
                                soldier_health_box.w = 0;
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                            }
                        }
                        hot_air_balloon_bomb.vx = 0;
                        hot_air_balloon_bomb.vy = 0;
                        hot_air_balloon_bomb.x = hot_air_balloon_rect.x;
                        hot_air_balloon_bomb.y = hot_air_balloon_rect.y;
                        hot_air_balloon_bomb_attack_timer = 0.0f;
                        hot_air_balloon_bomb.active = false;
                        hot_air_balloon_bomb_exploide = false;
                        hot_air_balloon_throw_bomb = false;
                    }else if(hot_air_balloon_bomb.x > Windows_Width || hot_air_balloon_bomb.y > Windows_Height || checkCollisionArcher(&hot_air_balloon_bomb, &archer, archer_basic_Width, archer_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                        if(checkCollisionArcher(&hot_air_balloon_bomb, &archer, archer_basic_Width, archer_basic_Height, hot_air_ballon_Width, hot_air_ballon_Height)){
                            hot_air_balloon_bomb_exploide = true;
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(archer_health > 0){
                                archer_health_box.w -= (Windows_Width*5)/1920;  
                                archer_health -= 5;
                            }
                            if(archer_health<=0){
                                archer_health_box.w = 0;
                                gameOver = true;
                            }
                        }
                        hot_air_balloon_bomb.vx = 0;
                        hot_air_balloon_bomb.vy = 0;
                        hot_air_balloon_bomb.x = hot_air_balloon_rect.x;
                        hot_air_balloon_bomb.y = hot_air_balloon_rect.y;
                        hot_air_balloon_bomb_attack_timer = 0.0f;
                        hot_air_balloon_bomb.active = false;
                        hot_air_balloon_bomb_exploide = false;
                        hot_air_balloon_throw_bomb = false;
                    }
                }
            }
            
            if (tower_bomb_1_5_Timer >= tower_bomb_1_5_attack_delay) {
                if(!tower_bomb_1_5.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_1_5.vx += knight.x/120;
                    }else if(mille.spawn){
                         tower_bomb_1_5.vx += mille.x/120;
                    }else if(soldier.spawn){
                         tower_bomb_1_5.vx += soldier.x/120;
                    }else{
                        tower_bomb_1_5.vx += archer.x/120;
                    }
                    tower_bomb_1_5.vy += -16 * ScaleY;
                    tower_bomb_1_5.active = true;
                }
            }

            if (tower_bomb_2_Timer >= tower_bomb_2_attack_delay) {
                if(!tower_bomb_2.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_2.vx += knight.x/100;
                    }else if(mille.spawn){
                         tower_bomb_2.vx += mille.x/120;
                    }else if(soldier.spawn){
                         tower_bomb_2.vx += soldier.x/120;
                    }else{
                        tower_bomb_2.vx += archer.x/100;
                    }
                    tower_bomb_2.vy += -16 * ScaleY;
                    tower_bomb_2.active = true;
                }
            }

            if (tower_bomb_3_Timer >= tower_bomb_3_attack_delay) {
                if(!tower_bomb_3.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_3.vx += knight.x/100;
                    }else if(mille.spawn){
                         tower_bomb_3.vx += mille.x/120;
                    }else if(soldier.spawn){
                         tower_bomb_3.vx += soldier.x/120;
                    }else{
                        tower_bomb_3.vx += archer.x/100;
                    }
                    tower_bomb_3.vy += -16 * ScaleY;
                    tower_bomb_3.active = true;
                }
            }
            if (tower_bomb_4_Timer >= tower_bomb_4_attack_delay) {
                if(!tower_bomb_4.active){
                    if(sound){
                        Mix_PlayChannel(3, towerShooting_sound, 0); 
                    }
                    if(knight.spawn){
                        tower_bomb_4.vx += knight.x/90;
                    }else if(mille.spawn){
                         tower_bomb_4.vx += mille.x/120;
                    }else if(soldier.spawn){
                         tower_bomb_4.vx += soldier.x/120;
                    }else{
                        tower_bomb_4.vx += archer.x/90;
                    }
                    tower_bomb_4.vy += -16 * ScaleY;
                    tower_bomb_4.active = true;
                }
            }

            if(tower_bomb_3.active){
                tower_bomb_3.x += tower_bomb_3.vx; 
                tower_bomb_3.y += tower_bomb_3.vy; 
                tower_bomb_3.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                                currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(mille.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*10)/1920;  
                            mille_health -= 10;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(soldier.spawn && (tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*5)/1920;  
                            soldier_health -= 5;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }else if(tower_bomb_3.x > Windows_Width || tower_bomb_3.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_3, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_3.vx = 0;
                    tower_bomb_3.vy = 0;
                    tower_bomb_3.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_3.y = (float)tower_attacker.y + (Windows_Height*150)/1080; 
                    tower_bomb_3_Timer = 0.0f;
                    tower_bomb_3.active = false;
                }
            }
            if(tower_bomb_2.active){
                tower_bomb_2.x += tower_bomb_2.vx; 
                tower_bomb_2.y += tower_bomb_2.vy; 
                tower_bomb_2.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                                currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*350)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(mille.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*5)/1920;  
                            mille_health -= 5;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*350)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(soldier.spawn && (tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_3, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_2, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*5)/1920;  
                            soldier_health -= 5;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*350)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }else if(tower_bomb_2.x > Windows_Width || tower_bomb_2.y > Windows_Height || checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_2, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_2.vx = 0;
                    tower_bomb_2.vy = 0;
                    tower_bomb_2.x = (float)tower_attacker.x + (Windows_Width*500)/1920; 
                    tower_bomb_2.y = (float)tower_attacker.y + (Windows_Height*350)/1080; 
                    tower_bomb_2_Timer = 0.0f;
                    tower_bomb_2.active = false;
                }
            }

            if(tower_bomb_4.active){
                tower_bomb_4.x += tower_bomb_4.vx; 
                tower_bomb_4.y += tower_bomb_4.vy; 
                tower_bomb_4.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_4.x > Windows_Width || tower_bomb_4.y > Windows_Height || checkCollisionArcher(&tower_bomb_4, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_4, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                                currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_4.vx = 0;
                    tower_bomb_4.vy = 0;
                    tower_bomb_4.x = (float)tower_attacker.x + (Windows_Width*300)/1920; 
                    tower_bomb_4.y = (float)tower_attacker.y + (Windows_Height*300)/1080; 
                    tower_bomb_4_Timer = 0.0f;
                    tower_bomb_4.active = false;
                }else if(mille.spawn && (tower_bomb_4.x > Windows_Width || tower_bomb_4.y > Windows_Height || checkCollisionArcher(&tower_bomb_4, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_4, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*5)/1920;  
                            mille_health -= 5;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_4.vx = 0;
                    tower_bomb_4.vy = 0;
                    tower_bomb_4.x = (float)tower_attacker.x + (Windows_Width*300)/1920; 
                    tower_bomb_4.y = (float)tower_attacker.y + (Windows_Height*300)/1080; 
                    tower_bomb_4_Timer = 0.0f;
                    tower_bomb_4.active = false;
                }else if(soldier.spawn && (tower_bomb_4.x > Windows_Width || tower_bomb_4.y > Windows_Height || checkCollisionArcher(&tower_bomb_4, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_4, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*5)/1920;  
                            soldier_health -= 5;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_4.vx = 0;
                    tower_bomb_4.vy = 0;
                    tower_bomb_4.x = (float)tower_attacker.x + (Windows_Width*300)/1920; 
                    tower_bomb_4.y = (float)tower_attacker.y + (Windows_Height*300)/1080; 
                    tower_bomb_4_Timer = 0.0f;
                    tower_bomb_4.active = false;
                }else if(tower_bomb_4.x > Windows_Width || tower_bomb_4.y > Windows_Height || checkCollisionArcher(&tower_bomb_4, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_4, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_4.vx = 0;
                    tower_bomb_4.vy = 0;
                    tower_bomb_4.x = (float)tower_attacker.x + (Windows_Width*300)/1920; 
                    tower_bomb_4.y = (float)tower_attacker.y + (Windows_Height*300)/1080; 
                    tower_bomb_4_Timer = 0.0f;
                    tower_bomb_4.active = false;
                }
            }
            
            if(tower_bomb_1_5.active){
                tower_bomb_1_5.x += tower_bomb_1_5.vx; 
                tower_bomb_1_5.y += tower_bomb_1_5.vy; 
                tower_bomb_1_5.vy += 0.5 * ScaleY;
                if(knight.spawn && (tower_bomb_1_5.x > Windows_Width || tower_bomb_1_5.y > Windows_Height || checkCollisionArcher(&tower_bomb_1_5, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_1_5, &knight, knight_basic_Width, knight_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(knight_health > 0){
                            knight_health_box.w -= (Windows_Width*5)/1920;  
                            knight_health -= 5;
                        }
                        if(knight_health<=0){
                            knight_health_box.w = 0;
                            knight.spawn = false;
                                currentFrame_Knight = 0;
                        }
                    }
                    tower_bomb_1_5.vx = 0;
                    tower_bomb_1_5.vy = 0;
                    tower_bomb_1_5.x = (float)tower_attacker.x + (Windows_Width*600)/1920; 
                    tower_bomb_1_5.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_bomb_1_5_Timer = 0.0f;
                    tower_bomb_1_5.active = false;
                }else if(mille.spawn && (tower_bomb_1_5.x > Windows_Width || tower_bomb_1_5.y > Windows_Height || checkCollisionArcher(&tower_bomb_1_5, &mille, mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_1_5, &mille,mille_basic_Width + (Windows_Width*40)/1920, mille_basic_Height + (Windows_Height*40)/1080, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(mille_health > 0){
                            mille_health_box.w -= (Windows_Width*5)/1920;  
                            mille_health -= 5;
                        }
                        if(mille_health<=0){
                            mille_health_box.w = 0;
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                        }
                    }
                    tower_bomb_1_5.vx = 0;
                    tower_bomb_1_5.vy = 0;
                    tower_bomb_1_5.x = (float)tower_attacker.x + (Windows_Width*600)/1920; 
                    tower_bomb_1_5.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_bomb_1_5_Timer = 0.0f;
                    tower_bomb_1_5.active = false;
                }else if(soldier.spawn && (tower_bomb_1_5.x > Windows_Width || tower_bomb_1_5.y > Windows_Height || checkCollisionArcher(&tower_bomb_1_5, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080))){
                    if(checkCollisionArcher(&tower_bomb_1_5, &soldier, soldier_basic_Width, soldier_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(soldier_health > 0){
                            soldier_health_box.w -= (Windows_Width*5)/1920;  
                            soldier_health -= 5;
                        }
                        if(soldier_health<=0){
                            soldier_health_box.w = 0;
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                        }
                    }
                    tower_bomb_1_5.vx = 0;
                    tower_bomb_1_5.vy = 0;
                    tower_bomb_1_5.x = (float)tower_attacker.x + (Windows_Width*600)/1920; 
                    tower_bomb_1_5.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_bomb_1_5_Timer = 0.0f;
                    tower_bomb_1_5.active = false;
                }else if(tower_bomb_1_5.x > Windows_Width || tower_bomb_1_5.y > Windows_Height || checkCollisionArcher(&tower_bomb_1_5, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                    if(checkCollisionArcher(&tower_bomb_1_5, &archer, archer_basic_Width, archer_basic_Height, (Windows_Width*20)/1920, (Windows_Height*20)/1080)){
                        if(sound){
                            Mix_PlayChannel(4, archerDamage_sound, 0); 
                        }
                        if(archer_health > 0){
                            archer_health_box.w -= (Windows_Width*5)/1920;  
                            archer_health -= 5;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    tower_bomb_1_5.vx = 0;
                    tower_bomb_1_5.vy = 0;
                    tower_bomb_1_5.x = (float)tower_attacker.x + (Windows_Width*600)/1920; 
                    tower_bomb_1_5.y = (float)tower_attacker.y + (Windows_Height*200)/1080; 
                    tower_bomb_1_5_Timer = 0.0f;
                    tower_bomb_1_5.active = false;
                }
            }

            if(!wolf.spawn){
                // Tower Attacker Timer Update
                wolf_spawn_timer += deltaTime;  // Update the timer
                if (wolf_spawn_timer >= wolf_spawn_delay) {
                    wolf.spawn = true;
                    wolf_health = 100;
                    wolf_health_box.w =character_health_Width;
                    wolf_running = true;
                    wolf_attacking = false;
                    wolf_spawn_timer = 0.0f;
                }
            }

            if(wolf.spawn){
                // Handle frame updates for animation
                Uint32 currentTime = SDL_GetTicks();
                    if(checkCollisionEnemy(&wolf, &archer, archer_basic_Width, archer_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                        wolf_running = false;
                        wolf_attacking = true;
                        if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                            currentFrame_Enemy++;
                            if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                currentFrame_Enemy = 0; 
                            }
                            lastFrameTime_Enemy = currentTime;
                        }
                        if(wolf_attack_timer>=wolf_attack_delay){
                            if(sound){
                                Mix_PlayChannel(4, archerDamage_sound, 0); 
                            }
                            if(archer_health > 0){
                                archer_health_box.w -= (Windows_Width*5)/1920;  
                                archer_health -= 5;
                            }
                            wolf_attack_timer = 0.0f;
                        }else{
                            wolf_attack_timer += deltaTime;
                        }
                        if(archer_health<=0){
                            archer_health_box.w = 0;
                            gameOver = true;
                        }
                    }
                    else if(knight.spawn&&checkCollisionEnemy(&wolf, &knight, knight_basic_Width, knight_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                            wolf_running = false;
                            wolf_attacking = true;
                            if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                                currentFrame_Enemy++;
                                if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                    currentFrame_Enemy = 0; 
                                }
                                lastFrameTime_Enemy = currentTime;
                            }
                            if(wolf_attack_timer>=wolf_attack_delay){
                                if(sound){
                                    Mix_PlayChannel(4, archerDamage_sound, 0); 
                                }
                                if(knight_health > 0){
                                    knight_health_box.w -= (Windows_Width*5)/1920;  
                                    knight_health -= 5;
                                }
                                wolf_attack_timer = 0.0f;
                            }else{
                                wolf_attack_timer += deltaTime;
                            }
                            if(knight_health<=0){
                                knight_health_box.w = 0;
                                knight.spawn = false;
                                currentFrame_Knight = 0;
                            }
                        
                    }else if(mille.spawn && checkCollisionEnemy(&wolf, &mille, mille_basic_Width, mille_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                            wolf_running = false;
                            wolf_attacking = true;
                            if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                                currentFrame_Enemy++;
                                if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                    currentFrame_Enemy = 0; 
                                }
                                lastFrameTime_Enemy = currentTime;
                            }
                            if(wolf_attack_timer>=wolf_attack_delay){
                                if(sound){
                                    Mix_PlayChannel(4, archerDamage_sound, 0); 
                                }
                                if(mille_health > 0){
                                    mille_health_box.w -= (Windows_Width*5)/1920;  
                                    mille_health -= 5;
                                }
                                wolf_attack_timer = 0.0f;
                            }else{
                                wolf_attack_timer += deltaTime;
                            }
                            if(mille_health<=0){
                                mille.spawn = false;
                                currentFrame_Mille = 0;
                                wolf_health_box.w = character_health_Width;
                                wolf_health = 100;
                                wolf.spawn = false;
                                wolf_attacking = false;
                                wolf_running = true;
                                wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                                currentFrame_Enemy = 0;
                            }
                        
                    }else if(soldier.spawn && checkCollisionEnemy(&wolf, &soldier, soldier_basic_Width, soldier_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                            wolf_running = false;
                            wolf_attacking = true;
                            if (wolf_attacking && currentTime > lastFrameTime_Enemy + frameDelay) {
                                currentFrame_Enemy++;
                                if (currentFrame_Enemy >=  WOLF_TOTAL_ATTACK_FRAMES) {
                                    currentFrame_Enemy = 0; 
                                }
                                lastFrameTime_Enemy = currentTime;
                            }
                            if(wolf_attack_timer>=wolf_attack_delay){
                                if(sound){
                                    Mix_PlayChannel(4, archerDamage_sound, 0); 
                                }
                                if(soldier_health > 0){
                                    soldier_health_box.w -= (Windows_Width*5)/1920;  
                                    soldier_health -= 5;
                                }
                                wolf_attack_timer = 0.0f;
                            }else{
                                wolf_attack_timer += deltaTime;
                            }
                            if(soldier_health<=0){
                                soldier.spawn = false;
                                currentFrame_Soldier = 0;
                                wolf_health_box.w = character_health_Width;
                                wolf_health = 100;
                                wolf.spawn = false;
                                wolf_attacking = false;
                                wolf_running = true;
                                wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                                currentFrame_Enemy = 0;
                            }
                        
                    }
                    else{
                        if (wolf_running && currentTime > lastFrameTime_Enemy + frameDelay) {
                            currentFrame_Enemy++;
                            if (currentFrame_Enemy >=  WOLF_TOTAL_RUN_FRAMES) {
                                currentFrame_Enemy = 0; 
                            }

                            lastFrameTime_Enemy = currentTime;
                        }
                        wolf.x += 8*ScaleX; 
                        wolf_attacking = false;
                        wolf_running = true;
                        
                    
                    if(wolf.x >= wolf_basic_Width+Windows_Width){
                        wolf.spawn = false;
                        wolf_attacking = false;
                        wolf_running = true;
                        wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                        currentFrame_Enemy = 0;
                    }
                }
            }
            

            if(archer_arrow.active){
                archer_arrow.x += archer_arrow.vx;
                archer_arrow.y += archer_arrow.vy;
                archer_arrow.vy += 0.5*ScaleY;     
                archerlastvelX =  (archer_arrow.vx !=0)? archer_arrow.vx:archerlastvelX;
                archerlastvelY =  (archer_arrow.vy !=0)? archer_arrow.vy:archerlastvelY;  
            
            // Adjust the angle of the arrow based on its vertical velocity (vy)
                if (archer_arrow.vy <= -25*ScaleY) {
                    archer_arrow.angle = 35;    // Very steep upwards
                } else if (archer_arrow.vy < -20*ScaleY) {
                    archer_arrow.angle = 30;    // Steep upwards
                } else if (archer_arrow.vy < -15*ScaleY) {
                    archer_arrow.angle = 25;    // Upwards
                } else if (archer_arrow.vy < -10*ScaleY) {
                    archer_arrow.angle = 20;    // Slightly upwards
                } else if (archer_arrow.vy < -5*ScaleY) {
                    archer_arrow.angle = 15;    // Almost horizontal upwards
                } else if (archer_arrow.vy < 0*ScaleY) {
                    archer_arrow.angle = 0;     // Horizontal
                } else if (archer_arrow.vy < 5*ScaleY) {
                    archer_arrow.angle = -5;    // Slightly downwards
                } else if (archer_arrow.vy < 10*ScaleY) {
                    archer_arrow.angle = -15;   // More downwards
                } else if (archer_arrow.vy < 15*ScaleY) {
                    archer_arrow.angle = -25;   // Further downwards
                } else if (archer_arrow.vy < 20*ScaleY) {
                    archer_arrow.angle = -35;   // Almost vertical downwards
                } else {
                    archer_arrow.angle = -45;   // Steep downwards
                }

                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height,level5_towerWidth, level5_towerHeight)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y, archer_arrow_basic_Width, archer_arrow_basic_Height, level5_towerWidth, level5_towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            frame_Video = true;
                            final_vid = true;
                        }
                    }
                    archer_arrow.vx = 0;
                    archer_arrow.vy = 0;
                    archer_arrow.active = false;  
                    archer_arrow.angle = 0;  
                }
            }
        
        if(knight_attack){
                if(checkCollisionCharacter(&knight, (int)tower_attacker.x, (int)tower_attacker.y, knight_basic_Width, knight_basic_Height, towerWidth, towerHeight)){
                        if(tower_attacker_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            tower_health_box.w -= (Windows_Width*50)/1920;
                            tower_attacker_health-=5;
                            knight_attack = false;
                        }
                        if(tower_attacker_health<=0){
                            tower_health_box.w = 0;
                            tower_attacker_health = 0;
                            knight_shooting = false;  
                            knight_standing = true;
                            frame_Video = true;
                            final_vid = true;
                        }
                    }else if(checkCollisionCharacter(&knight, (int)wolf.x, (int)wolf.y, knight_basic_Width, knight_basic_Height, wolf_basic_Width, wolf_basic_Height)){
                        if(wolf_health>0){
                            if(sound){
                                Mix_PlayChannel(4, towerDamage_sound, 0); 
                            }
                            wolf_health_box.w -= (Windows_Width*20)/1920;
                            wolf_health-=20;
                            knight_attack = false;
                        }
                        if(wolf_health<=0){
                            wolf_health_box.w = character_health_Width;
                            wolf_health = 100;
                            wolf.spawn = false;
                            wolf_attacking = false;
                            wolf_running = true;
                            wolf.x = (float) (-WOLF_RUN_WIDTH + (Windows_Width*40)/1920);
                            currentFrame_Enemy = 0;
                        }
                    }
                }
        }
        
    }

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, mainBackground, NULL, NULL);

    // If Selected Page is true
    if(selectedGame_page){
        SDL_RenderCopy(renderer, select_menu_playDeck_button, NULL, &select_menu_playDeck_button_rect);
        SDL_RenderCopy(renderer, select_menu_quit_button, NULL, &select_menu_quit_button_rect);
    }


    // If Tower Game is True
    if(towerGame){
        if(!towerGame_Started){
            if(towerGame_start_bt){
               
                if(towerGame_account){
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 240);
                        SDL_RenderFillRect(renderer, &account_main_rect);
                    if(towerGame_account_choose){
                        renderText(renderer, "GAME ACCOUNT", account_main_rect.x + 160, account_main_rect.y + 50, poppinsFont_Head, 225, 225, 225);
                        SDL_RenderCopy(renderer, account_create_button, NULL, &account_create_button_rect);
                        SDL_RenderCopy(renderer, account_login_button, NULL, &account_login_button_rect);
                    }else if(towerGame_account_create){
                        renderText(renderer, "Create Your Account", account_main_rect.x + 80, account_main_rect.y + 50, poppinsFont_Head, 225, 225, 225);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                        renderText(renderer, "Enter Your Username", account_username.x, account_username.y - 60, poppinsFont_Normal, 255,255,255);
                        SDL_RenderFillRect(renderer, &account_username);
                        if(strlen(username) > 0){
                            renderText(renderer, username, account_username.x+10, account_username.y+4, poppinsFont_Normal, 0,0,0);
                        }

                        renderText(renderer, "Enter Your Password", account_password.x, account_password.y - 60, poppinsFont_Normal, 255,255,255);
                        SDL_RenderFillRect(renderer, &account_password);
                        if(strlen(password) > 0){
                            renderText(renderer, password, account_password.x+10, account_password.y+4, poppinsFont_Normal, 0,0,0);
                        }
                        SDL_RenderCopy(renderer, account_create_button, NULL, &account_create_button1_rect);
                        if(account_problem){
                            renderText(renderer, account_problem_str, account_main_rect.x+100, account_login_button1_rect.h + account_login_button1_rect.y+4, poppinsFont_Small, 255,0,0);
                        }
                        if(account_true){
                            renderText(renderer, account_true_str, account_main_rect.x+100, account_login_button1_rect.h + account_login_button1_rect.y+4, poppinsFont_Small, 0,255,0);
                        }
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        if(username_active){
                            SDL_RenderFillRect(renderer, &account_username_typewriter);
                        }
                        if(password_active){
                            SDL_RenderFillRect(renderer, &account_password_typewriter);
                        }
                    }else if(towerGame_account_login){
                        renderText(renderer, "Login Your Account", account_main_rect.x + 80, account_main_rect.y + 50, poppinsFont_Head, 225, 225, 225);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        
                        renderText(renderer, "Enter Your Username", account_username.x, account_username.y - 60, poppinsFont_Normal, 255,255,255);
                        SDL_RenderFillRect(renderer, &account_username);
                        if(strlen(username) > 0){
                            renderText(renderer, username, account_username.x+10, account_username.y+4, poppinsFont_Normal, 0,0,0);
                        }

                        renderText(renderer, "Enter Your Password", account_password.x, account_password.y - 60, poppinsFont_Normal, 255,255,255);
                        SDL_RenderFillRect(renderer, &account_password);
                        if(strlen(password) > 0){
                            renderText(renderer, password, account_password.x+10, account_password.y+4, poppinsFont_Normal, 0,0,0);
                        }
                        SDL_RenderCopy(renderer, account_login_button, NULL, &account_login_button1_rect);
                        if(account_problem){
                            renderText(renderer, account_problem_str, account_main_rect.x+100, account_login_button1_rect.h + account_login_button1_rect.y+4, poppinsFont_Small, 255,0,0);
                        }
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        if(username_active){
                            SDL_RenderFillRect(renderer, &account_username_typewriter);
                        }
                        if(password_active){
                            SDL_RenderFillRect(renderer, &account_password_typewriter);
                        }
                    }
                }
            }else if(towerGame_homemenu){
                SDL_RenderCopy(renderer, continue_button, NULL, &continue_button_rect);
                SDL_RenderCopy(renderer, level_button, NULL, &level_button_rect);
                SDL_RenderCopy(renderer, option_button, NULL, &option_button_rect);
                SDL_RenderCopy(renderer, quit_button, NULL, &quit_button_rect);
            }else if(towerGame_levelmenu){
                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);
                SDL_RenderCopy(renderer, level1_button, NULL, &level1_button_rect);
                SDL_RenderCopy(renderer, level2_button, NULL, &level2_button_rect);
                SDL_RenderCopy(renderer, level3_button, NULL, &level3_button_rect);
                SDL_RenderCopy(renderer, level4_button, NULL, &level4_button_rect);
                SDL_RenderCopy(renderer, level5_button, NULL, &level5_button_rect);
            }else if(towerGame_optionmenu){

                SDL_RenderCopy(renderer, shop_button, NULL, &shop_button_rect);
                SDL_RenderCopy(renderer, music_button, NULL, &music_button_rect);
                SDL_RenderCopy(renderer, sound_button, NULL, &sound_button_rect);
                SDL_RenderCopy(renderer, control_button, NULL, &control_button_rect);
                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);

            }else if(towerGame_option_musicMenu){
                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);
                SDL_RenderCopy(renderer, music_button, NULL, &music_on_off_button_rect);
                if(music){
                    SDL_RenderCopy(renderer, on_button, NULL, &on_button_rect);
                }else{
                    SDL_RenderCopy(renderer, off_button, NULL, &off_button_rect);
                }
            }else if(towerGame_option_soundMenu){
                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);
                SDL_RenderCopy(renderer, sound_button, NULL, &sound_on_off_button_rect);
                if(sound){
                    SDL_RenderCopy(renderer, on_button, NULL, &on_button_rect);
                }else{
                    SDL_RenderCopy(renderer, off_button, NULL, &off_button_rect);
                }
            }else if(towerGame_option_controllerMenu){
                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);
            }else if(towerGame_option_shopMenu){

                SDL_RenderCopy(renderer, back_button, NULL, &back_button_rect);
            }
        }else if(towerGame_Started){
            char levelMoney[50];
            sprintf(levelMoney, "MONEY: %d$", money);
            renderText(renderer, levelMoney, (Windows_Width*20)/1920, (Windows_Height*20)/1080, poppinsFont_Normal_Customize, 255, 255, 255);
            if(towerGame_Started_level1){
                if(!gamePause && !gameOver && !youWin && !midPause){
                    if(archer_card_cool >= 100){
                        archer_card_cool = 100;

                    }else{
                        archer_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(archer_card_cool_Timer >= 1.50f){
                            archer_card_cool_Timer = 0.00f;
                            archer_card_cool++;
                        }
                    }
                    if(money >= 10){
                        money = 10;
                    }else{
                        money_Timer += money_Increase_Timer;
                        if(money_Timer >= 1.50f){
                            money_Timer = 0.00f;
                            money++;
                        }
                    }
                }

                char archer_card_cool_Text[4];
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &cardsBackground_outline_rect);
                SDL_SetRenderDrawColor(renderer, 0,0,0,200);
                SDL_RenderFillRect(renderer, &cardsBackground_rect);
                if(archer_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_archer_full_texture, NULL, &card_archer_rect);
                }else{
                    SDL_RenderCopy(renderer, card_archer_notfull_texture, NULL, &card_archer_rect);
                }
                sprintf(archer_card_cool_Text, "%d%%", archer_card_cool);
                renderText(renderer, "5$", card_archer_rect.x+(Windows_Width*12)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, archer_card_cool_Text, card_archer_rect.x + card_archer_rect.w - (Windows_Width*60)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, archer_arrow_basic_Width, archer_arrow_basic_Height};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }


                if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb.active){
                        SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_rect);
                    }else if(youWin){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                    }

                    
                }

                if(tower_attacker_health>50){
                    SDL_RenderCopy(renderer, level1_tower_fullhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=50 && tower_attacker_health>0){
                    SDL_RenderCopy(renderer, level1_tower_halfhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=0){
                    SDL_RenderCopy(renderer, level1_tower_zerohealth_texture, NULL, &tower_attacker);
                }

                
                if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                    SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                    if (archer_aiming) {
                        SDL_RenderCopy(renderer, archer_shooting_texture, &ArcherfireClips[currentFrame], &archerFire_rect);
                        SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                    } else {
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    }
                }
                
              
                archer_health_box.x = (int)(archer.x+(Windows_Width*20)/1920);
                archer_health_box.y = (int)(archer.y+((Windows_Height*-20)/1080));
                archer_health_box_innerfill.x = archer_health_box.x;
                archer_health_box_innerfill.y = archer_health_box.y;
                archer_health_box_outline.x = archer_health_box.x;
                archer_health_box_outline.y = archer_health_box.y;
                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, poppinsFont_Normal_Customize, 0, 0, 0);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                SDL_RenderFillRect(renderer, &tower_health_box);
                SDL_RenderFillRect(renderer, &archer_health_box);

                char archerHealth_val[50];
                SDL_SetRenderDrawColor(renderer, 127,23,31, 255);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                // SDL_RenderDrawRect(renderer, &archer_health_box_outline); 

                // Pause Button
                SDL_RenderCopy(renderer, pauseButton, NULL, &pauseButton_rect);

                if(midPause){
                    if(!level1_ending_scene){
                        SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(dialog1_start){
                            SDL_RenderCopy(renderer, level1_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                        }else if(dialog2_start){
                             SDL_RenderCopy(renderer, level1_king_dialog1_texture, NULL, &level1_king_dialog1_rect);
                        }
                    }
                }else{
                        SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[0], &archerBasic_rect);
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        if(knight_running_ending_scene){
                            SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[currentFrame], &knightBasic_rect);
                        }else if(knight_running_attacking_ending_scene){
                            SDL_RenderCopy(renderer, knight_running_attacking_spritesheet, &KnightRunnig_AttackingClips[currentFrame], &knightBasic_rect);
                        }else if(knight_standing_ending_scene){
                            SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                        }else{
                            SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[0], &knightBasic_rect);
                        }
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(knight_level1_ending_dialog){
                            SDL_RenderCopy(renderer, level1_ending_knight_dialog1, NULL, &level1_archer_dialog1_rect);
                        }else if(archer_level1_ending_dialog){
                            SDL_RenderCopy(renderer, level1_ending_archer_dialog1, NULL, &level1_archer_dialog1_rect);
                        }
                            
                    }
                }
            }
            }else if(towerGame_Started_level2){
                if(!gamePause && !gameOver && !youWin){
                    if(archer_card_cool >= 100){
                        archer_card_cool = 100;

                    }else{
                        archer_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(archer_card_cool_Timer >= 1.50f){
                            archer_card_cool_Timer = 0.00f;
                            archer_card_cool++;
                        }
                    }

                    if(knight_card_cool >= 100){
                        knight_card_cool = 100;

                    }else{
                        knight_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(knight_card_cool_Timer >= 1.50f){
                            knight_card_cool_Timer = 0.00f;
                            knight_card_cool++;
                        }
                    }
                    if(money >= 10){
                        money = 10;
                    }else{
                        money_Timer += money_Increase_Timer;
                        if(money_Timer >= 1.50f){
                            money_Timer = 0.00f;
                            money++;
                        }
                    }
                }

                char archer_card_cool_Text[4];
                char knight_card_cool_Text[4];
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &cardsBackground_outline_rect);
                SDL_SetRenderDrawColor(renderer, 0,0,0,200);
                SDL_RenderFillRect(renderer, &cardsBackground_rect);
                SDL_RenderCopy(renderer, mille_standing_texture, NULL, &mille_level2_rect);
                if(archer_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_archer_full_texture, NULL, &card_archer_rect);
                }else{
                    SDL_RenderCopy(renderer, card_archer_notfull_texture, NULL, &card_archer_rect);
                }
                if(knight_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_knight_full_texture, NULL, &card_knight_rect);
                }else{
                    SDL_RenderCopy(renderer, card_knight_notfull_texture, NULL, &card_knight_rect);
                }
                sprintf(archer_card_cool_Text, "%d%%", archer_card_cool);
                renderText(renderer, "5$", card_archer_rect.x+(Windows_Width*12)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, archer_card_cool_Text, card_archer_rect.x + card_archer_rect.w - (Windows_Width*60)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(knight_card_cool_Text, "%d%%", knight_card_cool);
                renderText(renderer, "5$", card_knight_rect.x+(Windows_Width*12)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, knight_card_cool_Text, card_knight_rect.x + card_knight_rect.w - (Windows_Width*60)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, archer_arrow_basic_Width, archer_arrow_basic_Height};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }

                if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb.active){
                        SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_rect);
                    }
                }


                if(tower_attacker_health>50){
                    SDL_RenderCopy(renderer, level2_tower_fullhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=50 && tower_attacker_health>0){
                    SDL_RenderCopy(renderer, level2_tower_halfhealth_texture, NULL, &tower_attacker);
                }else{
                    SDL_RenderCopy(renderer, level2_tower_zerohealth_texture, NULL, &tower_attacker);
                }

                if(hot_air_balloon_health>0 && hot_air_ballon_health_box.w > 0){
                    if(hot_air_balloon_throw_bomb){
                        SDL_RenderCopy(renderer, hot_air_bombing_balloon_texture, NULL, &hot_air_balloon_rect);
                    }else{
                        SDL_RenderCopy(renderer, hot_air_balloon_texture, NULL, &hot_air_balloon_rect);
                    }
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &hot_air_ballon_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &hot_air_ballon_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &hot_air_ballon_health_box_outline);
                }


                // Hot Air Balloon
                if(hot_air_balloon_health>0 && hot_air_ballon_health_box.w > 0){
                    if(hot_air_balloon_bomb.active){
                        SDL_Rect hot_air_balloon_bomb_rect= {(int)hot_air_balloon_bomb.x, (int)hot_air_balloon_bomb.y, (Windows_Width*88)/1920, (Windows_Height*80)/1080};
                        if(!hot_air_balloon_bomb_exploide){
                            SDL_RenderCopy(renderer, hot_air_balloon_bomb_texture, NULL, &hot_air_balloon_bomb_rect);
                        }else{
                            SDL_RenderCopy(renderer, hot_air_balloon_bomb_spritesheet, &Hot_Air_Balloon_Bomb_Exploiding[2], &hot_air_balloon_bomb_rect);
                        }
                    }
                }
                
                if(!midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                    SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                    if (archer_aiming) {
                        SDL_RenderCopy(renderer, archer_shooting_texture, &ArcherfireClips[currentFrame], &archerFire_rect);
                        SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                    } else {
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    }
                }
                
                if(knight.spawn){
                    // Knight Health
                    knight_health_box.x = (int)(knight.x+(Windows_Width*20)/1920);
                    knight_health_box.y = (int)(knight.y+((Windows_Height*-20)/1080));
                    knight_health_box_innerfill.x = knight_health_box.x;
                    knight_health_box_innerfill.y = knight_health_box.y;
                    knight_health_box_outline.x = knight_health_box.x;
                    knight_health_box_outline.y = knight_health_box.y;

                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &knight_health_box_innerfill);
                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &knight_health_box);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &knight_health_box_outline);

                    if(knight_moving_left || knight_moving_right){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[currentFrame_Knight], &knightBasic_rect);
                    }else if(knight_shooting){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_attacking_spritesheet, &KnightRunnig_AttackingClips[currentFrame_Knight], &knightBasic_rect);
                    }else{
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                    }
                }

                // Archer All Health
                archer_health_box.x = (int)(archer.x+(Windows_Width*20)/1920);
                archer_health_box.y = (int)(archer.y+((Windows_Height*-20)/1080));
                archer_health_box_innerfill.x = archer_health_box.x;
                archer_health_box_innerfill.y = archer_health_box.y;
                archer_health_box_outline.x = archer_health_box.x;
                archer_health_box_outline.y = archer_health_box.y;

                
                // Hot Air Balloon Health
                hot_air_ballon_health_box.x = (int)(hot_air_balloon_rect.x+(Windows_Width*50)/1920);
                hot_air_ballon_health_box.y = (int)(hot_air_balloon_rect.y+((Windows_Height*-20)/1080));
                hot_air_ballon_health_box_innerfill.x = hot_air_ballon_health_box.x;
                hot_air_ballon_health_box_innerfill.y = hot_air_ballon_health_box.y;
                hot_air_ballon_health_box_outline.x = hot_air_ballon_health_box.x;
                hot_air_ballon_health_box_outline.y = hot_air_ballon_health_box.y;

                

                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, poppinsFont_Normal_Customize, 0, 0, 0);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                SDL_RenderFillRect(renderer, &tower_health_box);
                SDL_RenderFillRect(renderer, &archer_health_box);

                char archerHealth_val[50];
                SDL_SetRenderDrawColor(renderer, 127,23,31, 255);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                // SDL_RenderDrawRect(renderer, &archer_health_box_outline); 

                // Pause Button
                SDL_RenderCopy(renderer, pauseButton, NULL, &pauseButton_rect);
                if(midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                    SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(!level2_ending_scene){
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level2_king_dialog1_texture, NULL, &level1_king_dialog1_rect);
                            }else if(dialog2_start){
                                SDL_RenderCopy(renderer, level2_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog3_start){
                                SDL_RenderCopy(renderer, level2_king_dialog2_texture, NULL, &level1_king_dialog1_rect);
                            }else if(dialog4_start){
                                SDL_RenderCopy(renderer, level2_mille_dialog1_texture, NULL, &level1_king_dialog1_rect);
                            }else if(dialog5_start){
                                SDL_RenderCopy(renderer, level2_archer_dialog2_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }else{
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level1_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }
                            
                    }
                }
            }else if(towerGame_Started_level3){
                if(!gamePause && !gameOver && !youWin){
                    if(archer_card_cool >= 100){
                        archer_card_cool = 100;

                    }else{
                        archer_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(archer_card_cool_Timer >= 1.50f){
                            archer_card_cool_Timer = 0.00f;
                            archer_card_cool++;
                        }
                    }

                    if(knight_card_cool >= 100){
                        knight_card_cool = 100;

                    }else{
                        knight_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(knight_card_cool_Timer >= 1.50f){
                            knight_card_cool_Timer = 0.00f;
                            knight_card_cool++;
                        }
                    }
                    if(mille_card_cool >= 100){
                        mille_card_cool = 100;

                    }else{
                        mille_card_cool_Timer += mille_card_Increase_cool_Timer;
                        if(mille_card_cool_Timer >= 1.50f){
                            mille_card_cool_Timer = 0.00f;
                            mille_card_cool++;
                        }
                    }
                    if(money >= 10){
                        money = 10;
                    }else{
                        money_Timer += money_Increase_Timer;
                        if(money_Timer >= 1.50f){
                            money_Timer = 0.00f;
                            money++;
                        }
                    }
                }

                char archer_card_cool_Text[4];
                char knight_card_cool_Text[4];
                char mille_card_cool_Text[4];
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &cardsBackground_outline_rect);
                SDL_SetRenderDrawColor(renderer, 0,0,0,200);
                SDL_RenderFillRect(renderer, &cardsBackground_rect);


                 // Mille All Health
                mille_health_box.x = (int)(mille.x+(Windows_Width*20)/1920);
                mille_health_box.y = (int)(mille.y+((Windows_Height*-20)/1080));
                mille_health_box_innerfill.x = mille_health_box.x;
                mille_health_box_innerfill.y = mille_health_box.y;
                mille_health_box_outline.x = mille_health_box.x;
                mille_health_box_outline.y = mille_health_box.y;
                 // Wolf All Health
                wolf_health_box.x = (int)(wolf.x+(Windows_Width*20)/1920);
                wolf_health_box.y = (int)(wolf.y+((Windows_Height*-20)/1080));
                wolf_health_box_innerfill.x = wolf_health_box.x;
                wolf_health_box_innerfill.y = wolf_health_box.y;
                wolf_health_box_outline.x = wolf_health_box.x;
                wolf_health_box_outline.y = wolf_health_box.y;

                if(mille_laser.active){
                    mille_laser_rect.x = (int)mille_laser.x;
                    mille_laser_rect.y = (int)mille_laser.y;
                    SDL_RenderCopy(renderer, mille_laser_texture, NULL, &mille_laser_rect);
                }

                if(mille.spawn){
                     // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &mille_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &mille_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &mille_health_box_outline); 
                    mille_rect.x = (int)mille.x; 
                    mille_rect.y = (int)mille.y; 
                    if(!milleclicked){
                        SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                    }else if(milleclicked){
                        if(mille_shooting){
                            SDL_RenderCopy(renderer, mille_attacking_spritesheet_texture, &MilleAttackingClips[currentFrame_Mille], &mille_rect);
                        }else if(mille_moving_left || mille_moving_right){
                            SDL_RenderCopy(renderer, mille_walking_spritesheet_texture, &MilleRunningClips[currentFrame_Mille], &mille_rect);
                        }else{
                            SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                        }
                    
                    }
                }
                if(archer_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_archer_full_texture, NULL, &card_archer_rect);
                }else{
                    SDL_RenderCopy(renderer, card_archer_notfull_texture, NULL, &card_archer_rect);
                }
                if(knight_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_knight_full_texture, NULL, &card_knight_rect);
                }else{
                    SDL_RenderCopy(renderer, card_knight_notfull_texture, NULL, &card_knight_rect);
                }
                if(mille_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_mille_full_texture, NULL, &card_mille_rect);
                }else{
                    SDL_RenderCopy(renderer, card_mille_notfull_texture, NULL, &card_mille_rect);
                }
                sprintf(archer_card_cool_Text, "%d%%", archer_card_cool);
                renderText(renderer, "5$", card_archer_rect.x+(Windows_Width*12)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, archer_card_cool_Text, card_archer_rect.x + card_archer_rect.w - (Windows_Width*60)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(knight_card_cool_Text, "%d%%", knight_card_cool);
                renderText(renderer, "5$", card_knight_rect.x+(Windows_Width*12)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, knight_card_cool_Text, card_knight_rect.x + card_knight_rect.w - (Windows_Width*60)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(mille_card_cool_Text, "%d%%", mille_card_cool);
                renderText(renderer, "5$", card_mille_rect.x+(Windows_Width*12)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, mille_card_cool_Text, card_mille_rect.x + card_mille_rect.w - (Windows_Width*60)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, archer_arrow_basic_Width, archer_arrow_basic_Height};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }

                if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb.active){
                        SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_rect);
                    }
                    if(tower_bomb_2.active){
                        SDL_Rect tower_bomb_2_rect= {(int)tower_bomb_2.x, (int)tower_bomb_2.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_2_rect);
                    }
                }


                if(tower_attacker_health>50){
                    SDL_RenderCopy(renderer, level3_tower_fullhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=50 && tower_attacker_health>0){
                    SDL_RenderCopy(renderer, level3_tower_halfhealth_texture, NULL, &tower_attacker);
                }else{
                    SDL_RenderCopy(renderer, level3_tower_zerohealth_texture, NULL, &tower_attacker);
                }

                if(wolf.spawn){
                    // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &wolf_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &wolf_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &wolf_health_box_outline); 
                    if(wolf_running){
                        SDL_Rect wolfBasic_rect = { (int)wolf.x, (int)wolf.y, wolf_basic_Width, wolf_basic_Height };
                        SDL_RenderCopy(renderer, wolf_running_spritesheet, &WolfRunningClips[currentFrame_Enemy], &wolfBasic_rect);
                    }else if(wolf_attacking){
                        SDL_Rect wolfAttack_rect = { (int)wolf.x, (int)wolf.y, wolf_basic_Width, wolf_basic_Height };
                        SDL_RenderCopy(renderer, wolf_attacking_spritesheet, &WolfAttackingClips[currentFrame_Enemy], &wolfAttack_rect);

                    }
                }
                
                if(!midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                    SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                    if (archer_aiming) {
                        SDL_RenderCopy(renderer, archer_shooting_texture, &ArcherfireClips[currentFrame], &archerFire_rect);
                        SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                    } else {
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    }
                }
                
                if(knight.spawn){
                    // Knight Health
                    knight_health_box.x = (int)(knight.x+(Windows_Width*20)/1920);
                    knight_health_box.y = (int)(knight.y+((Windows_Height*-20)/1080));
                    knight_health_box_innerfill.x = knight_health_box.x;
                    knight_health_box_innerfill.y = knight_health_box.y;
                    knight_health_box_outline.x = knight_health_box.x;
                    knight_health_box_outline.y = knight_health_box.y;

                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &knight_health_box_innerfill);
                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &knight_health_box);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &knight_health_box_outline);

                    if(knight_moving_left || knight_moving_right){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[currentFrame_Knight], &knightBasic_rect);
                    }else if(knight_shooting){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_attacking_spritesheet, &KnightRunnig_AttackingClips[currentFrame_Knight], &knightBasic_rect);
                    }else{
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                    }
                }

                // Archer All Health
                archer_health_box.x = (int)(archer.x+(Windows_Width*20)/1920);
                archer_health_box.y = (int)(archer.y+((Windows_Height*-20)/1080));
                archer_health_box_innerfill.x = archer_health_box.x;
                archer_health_box_innerfill.y = archer_health_box.y;
                archer_health_box_outline.x = archer_health_box.x;
                archer_health_box_outline.y = archer_health_box.y;
                
                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, poppinsFont_Normal_Customize, 0, 0, 0);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                SDL_RenderFillRect(renderer, &tower_health_box);
                SDL_RenderFillRect(renderer, &archer_health_box);

                char archerHealth_val[50];
                SDL_SetRenderDrawColor(renderer, 127,23,31, 255);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                // SDL_RenderDrawRect(renderer, &archer_health_box_outline); 

                // Pause Button
                SDL_RenderCopy(renderer, pauseButton, NULL, &pauseButton_rect);
                if(midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                    SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(!level2_ending_scene){
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level3_knight_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog2_start){
                                SDL_RenderCopy(renderer, level3_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog3_start){
                                SDL_RenderCopy(renderer, level3_mille_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog4_start){
                                SDL_RenderCopy(renderer, level3_archer_dialog2_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }else{
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level1_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }
                            
                    }
                }
            }else if(towerGame_Started_level4){
                 if(!gamePause && !gameOver && !youWin){
                    if(archer_card_cool >= 100){
                        archer_card_cool = 100;

                    }else{
                        archer_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(archer_card_cool_Timer >= 1.50f){
                            archer_card_cool_Timer = 0.00f;
                            archer_card_cool++;
                        }
                    }

                    if(knight_card_cool >= 100){
                        knight_card_cool = 100;

                    }else{
                        knight_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(knight_card_cool_Timer >= 1.50f){
                            knight_card_cool_Timer = 0.00f;
                            knight_card_cool++;
                        }
                    }
                    if(mille_card_cool >= 100){
                        mille_card_cool = 100;

                    }else{
                        mille_card_cool_Timer += mille_card_Increase_cool_Timer;
                        if(mille_card_cool_Timer >= 1.50f){
                            mille_card_cool_Timer = 0.00f;
                            mille_card_cool++;
                        }
                    }
                    if(soldier_card_cool >= 100){
                        soldier_card_cool = 100;

                    }else{
                        soldier_card_cool_Timer += soldier_card_Increase_cool_Timer;
                        if(soldier_card_cool_Timer >= 1.50f){
                            soldier_card_cool_Timer = 0.00f;
                            soldier_card_cool++;
                        }
                    }
                    if(money >= 10){
                        money = 10;
                    }else{
                        money_Timer += money_Increase_Timer;
                        if(money_Timer >= 1.50f){
                            money_Timer = 0.00f;
                            money++;
                        }
                    }
                }

                char archer_card_cool_Text[4];
                char knight_card_cool_Text[4];
                char mille_card_cool_Text[4];
                char soldier_card_cool_Text[4];
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &cardsBackground_outline_rect);
                SDL_SetRenderDrawColor(renderer, 0,0,0,200);
                SDL_RenderFillRect(renderer, &cardsBackground_rect);


                 // Mille All Health
                mille_health_box.x = (int)(mille.x+(Windows_Width*20)/1920);
                mille_health_box.y = (int)(mille.y+((Windows_Height*-20)/1080));
                mille_health_box_innerfill.x = mille_health_box.x;
                mille_health_box_innerfill.y = mille_health_box.y;
                mille_health_box_outline.x = mille_health_box.x;
                mille_health_box_outline.y = mille_health_box.y;
                 // Soldier All Health
                soldier_health_box.x = (int)(soldier.x+(Windows_Width*20)/1920);
                soldier_health_box.y = (int)(soldier.y+((Windows_Height*-20)/1080));
                soldier_health_box_innerfill.x = soldier_health_box.x;
                soldier_health_box_innerfill.y = soldier_health_box.y;
                soldier_health_box_outline.x = soldier_health_box.x;
                soldier_health_box_outline.y = soldier_health_box.y;

                if(mille_laser.active){
                    mille_laser_rect.x = (int)mille_laser.x;
                    mille_laser_rect.y = (int)mille_laser.y;
                    SDL_RenderCopy(renderer, mille_laser_texture, NULL, &mille_laser_rect);
                }

                if(soldier_bullet.active){
                    SDL_Rect soldier_bullet_rect = {(int)soldier_bullet.x,(int)soldier_bullet.y + (Windows_Width*120)/1920,soldier_bullet_Width, soldier_bullet_Height};
                    SDL_RenderCopy(renderer, soldier_bullet_texture, NULL, &soldier_bullet_rect);
                }

                if(mille.spawn){
                     // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &mille_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &mille_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &mille_health_box_outline); 
                    mille_rect.x = (int)mille.x; 
                    mille_rect.y = (int)mille.y; 
                    if(!milleclicked){
                        SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                    }else if(milleclicked){
                        if(mille_shooting){
                            SDL_RenderCopy(renderer, mille_attacking_spritesheet_texture, &MilleAttackingClips[currentFrame_Mille], &mille_rect);
                        }else if(mille_moving_left || mille_moving_right){
                            SDL_RenderCopy(renderer, mille_walking_spritesheet_texture, &MilleRunningClips[currentFrame_Mille], &mille_rect);
                        }else{
                            SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                        }
                    
                    }
                }

                if(soldier.spawn){
                     // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &soldier_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &soldier_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &soldier_health_box_outline); 

                    soldier_rect.x = (int)soldier.x; 
                    soldier_rect.y = (int)soldier.y; 

                    if(!soldierclicked){
                        SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[3], &soldier_rect);
                    }else if(soldierclicked){
                        if(soldier_shooting){
                            SDL_RenderCopy(renderer, soldier_attacking_spritesheet_texture, &SoldierAttackingClips[1], &soldier_rect);
                        }else if(soldier_moving_left || soldier_moving_right){
                            SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[currentFrame_Soldier], &soldier_rect);
                        }else{
                            SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[3], &soldier_rect);
                        }
                    
                    }
                }

                if(archer_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_archer_full_texture, NULL, &card_archer_rect);
                }else{
                    SDL_RenderCopy(renderer, card_archer_notfull_texture, NULL, &card_archer_rect);
                }
                if(knight_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_knight_full_texture, NULL, &card_knight_rect);
                }else{
                    SDL_RenderCopy(renderer, card_knight_notfull_texture, NULL, &card_knight_rect);
                }
                if(mille_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_mille_full_texture, NULL, &card_mille_rect);
                }else{
                    SDL_RenderCopy(renderer, card_mille_notfull_texture, NULL, &card_mille_rect);
                }
                if(soldier_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_soldier_full_texture, NULL, &card_soldier_rect);
                }else{
                    SDL_RenderCopy(renderer, card_soldier_notfull_texture, NULL, &card_soldier_rect);
                }
                sprintf(archer_card_cool_Text, "%d%%", archer_card_cool);
                renderText(renderer, "5$", card_archer_rect.x+(Windows_Width*12)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, archer_card_cool_Text, card_archer_rect.x + card_archer_rect.w - (Windows_Width*60)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(knight_card_cool_Text, "%d%%", knight_card_cool);
                renderText(renderer, "5$", card_knight_rect.x+(Windows_Width*12)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, knight_card_cool_Text, card_knight_rect.x + card_knight_rect.w - (Windows_Width*60)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(mille_card_cool_Text, "%d%%", mille_card_cool);
                renderText(renderer, "5$", card_mille_rect.x+(Windows_Width*12)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, mille_card_cool_Text, card_mille_rect.x + card_mille_rect.w - (Windows_Width*60)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                
                sprintf(soldier_card_cool_Text, "%d%%", soldier_card_cool);
                renderText(renderer, "5$", card_soldier_rect.x+(Windows_Width*12)/1920, card_soldier_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, soldier_card_cool_Text, card_soldier_rect.x + card_soldier_rect.w - (Windows_Width*60)/1920, card_soldier_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, archer_arrow_basic_Width, archer_arrow_basic_Height};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }

                if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb.active){
                        SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_rect);
                    }
                }


                if(tower_attacker_health>50){
                    SDL_RenderCopy(renderer, level4_tower_fullhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=50 && tower_attacker_health>0){
                    SDL_RenderCopy(renderer, level4_tower_halfhealth_texture, NULL, &tower_attacker);
                }else{
                    SDL_RenderCopy(renderer, level4_tower_zerohealth_texture, NULL, &tower_attacker);
                }

                 if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb_2.active){
                        SDL_Rect tower_bomb_2_rect= {(int)tower_bomb_2.x, (int)tower_bomb_2.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_2_rect);
                    }

                    if(tower_bomb_3.active){
                        SDL_Rect tower_bomb_3_rect= {(int)tower_bomb_3.x, (int)tower_bomb_3.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_3_rect);
                    }
                }
                
                if(!midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                    SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                    if (archer_aiming) {
                        SDL_RenderCopy(renderer, archer_shooting_texture, &ArcherfireClips[currentFrame], &archerFire_rect);
                        SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                    } else {
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    }
                }
                
                if(knight.spawn){
                    // Knight Health
                    knight_health_box.x = (int)(knight.x+(Windows_Width*20)/1920);
                    knight_health_box.y = (int)(knight.y+((Windows_Height*-20)/1080));
                    knight_health_box_innerfill.x = knight_health_box.x;
                    knight_health_box_innerfill.y = knight_health_box.y;
                    knight_health_box_outline.x = knight_health_box.x;
                    knight_health_box_outline.y = knight_health_box.y;

                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &knight_health_box_innerfill);
                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &knight_health_box);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &knight_health_box_outline);

                    if(knight_moving_left || knight_moving_right){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[currentFrame_Knight], &knightBasic_rect);
                    }else if(knight_shooting){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_attacking_spritesheet, &KnightRunnig_AttackingClips[currentFrame_Knight], &knightBasic_rect);
                    }else{
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                    }
                }

                // Archer All Health
                archer_health_box.x = (int)(archer.x+(Windows_Width*20)/1920);
                archer_health_box.y = (int)(archer.y+((Windows_Height*-20)/1080));
                archer_health_box_innerfill.x = archer_health_box.x;
                archer_health_box_innerfill.y = archer_health_box.y;
                archer_health_box_outline.x = archer_health_box.x;
                archer_health_box_outline.y = archer_health_box.y;
                
                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, poppinsFont_Normal_Customize, 255, 255, 255);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                SDL_RenderFillRect(renderer, &tower_health_box);
                SDL_RenderFillRect(renderer, &archer_health_box);

                char archerHealth_val[50];
                SDL_SetRenderDrawColor(renderer, 127,23,31, 255);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                // SDL_RenderDrawRect(renderer, &archer_health_box_outline); 

                // Pause Button
                SDL_RenderCopy(renderer, pauseButton, NULL, &pauseButton_rect);
                if(midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                    SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(!level2_ending_scene){
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level4_archer_dialog1_texture, NULL, &
                                level1_archer_dialog1_rect);
                            }else if(dialog2_start){
                                SDL_RenderCopy(renderer, level4_soldier_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog3_start){
                                SDL_RenderCopy(renderer, level4_knight_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog4_start){
                                SDL_RenderCopy(renderer, level4_mille_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }else{
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level1_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }
                            
                    }
                }
            }else if(towerGame_Started_level5){
                 if(!gamePause && !gameOver && !youWin){
                    if(archer_card_cool >= 100){
                        archer_card_cool = 100;

                    }else{
                        archer_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(archer_card_cool_Timer >= 1.50f){
                            archer_card_cool_Timer = 0.00f;
                            archer_card_cool++;
                        }
                    }

                    if(knight_card_cool >= 100){
                        knight_card_cool = 100;

                    }else{
                        knight_card_cool_Timer += archer_card_Increase_cool_Timer;
                        if(knight_card_cool_Timer >= 1.50f){
                            knight_card_cool_Timer = 0.00f;
                            knight_card_cool++;
                        }
                    }
                    if(mille_card_cool >= 100){
                        mille_card_cool = 100;

                    }else{
                        mille_card_cool_Timer += mille_card_Increase_cool_Timer;
                        if(mille_card_cool_Timer >= 1.50f){
                            mille_card_cool_Timer = 0.00f;
                            mille_card_cool++;
                        }
                    }
                    if(soldier_card_cool >= 100){
                        soldier_card_cool = 100;

                    }else{
                        soldier_card_cool_Timer += soldier_card_Increase_cool_Timer;
                        if(soldier_card_cool_Timer >= 1.50f){
                            soldier_card_cool_Timer = 0.00f;
                            soldier_card_cool++;
                        }
                    }
                    if(money >= 10){
                        money = 10;
                    }else{
                        money_Timer += money_Increase_Timer;
                        if(money_Timer >= 1.50f){
                            money_Timer = 0.00f;
                            money++;
                        }
                    }
                }

                char archer_card_cool_Text[4];
                char knight_card_cool_Text[4];
                char mille_card_cool_Text[4];
                char soldier_card_cool_Text[4];
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                SDL_RenderDrawRect(renderer, &cardsBackground_outline_rect);
                SDL_SetRenderDrawColor(renderer, 0,0,0,200);
                SDL_RenderFillRect(renderer, &cardsBackground_rect);


                 // Mille All Health
                mille_health_box.x = (int)(mille.x+(Windows_Width*20)/1920);
                mille_health_box.y = (int)(mille.y+((Windows_Height*-20)/1080));
                mille_health_box_innerfill.x = mille_health_box.x;
                mille_health_box_innerfill.y = mille_health_box.y;
                mille_health_box_outline.x = mille_health_box.x;
                mille_health_box_outline.y = mille_health_box.y;
                 // Soldier All Health
                soldier_health_box.x = (int)(soldier.x+(Windows_Width*20)/1920);
                soldier_health_box.y = (int)(soldier.y+((Windows_Height*-20)/1080));
                soldier_health_box_innerfill.x = soldier_health_box.x;
                soldier_health_box_innerfill.y = soldier_health_box.y;
                soldier_health_box_outline.x = soldier_health_box.x;
                soldier_health_box_outline.y = soldier_health_box.y;
                 // Wolf All Health
                wolf_health_box.x = (int)(wolf.x+(Windows_Width*20)/1920);
                wolf_health_box.y = (int)(wolf.y+((Windows_Height*-20)/1080));
                wolf_health_box_innerfill.x = wolf_health_box.x;
                wolf_health_box_innerfill.y = wolf_health_box.y;
                wolf_health_box_outline.x = wolf_health_box.x;
                wolf_health_box_outline.y = wolf_health_box.y;

                if(mille_laser.active){
                    mille_laser_rect.x = (int)mille_laser.x;
                    mille_laser_rect.y = (int)mille_laser.y;
                    SDL_RenderCopy(renderer, mille_laser_texture, NULL, &mille_laser_rect);
                }

                if(soldier_bullet.active){
                    SDL_Rect soldier_bullet_rect = {(int)soldier_bullet.x,(int)soldier_bullet.y + (Windows_Width*120)/1920,soldier_bullet_Width, soldier_bullet_Height};
                    SDL_RenderCopy(renderer, soldier_bullet_texture, NULL, &soldier_bullet_rect);
                }

                if(mille.spawn){
                     // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &mille_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &mille_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &mille_health_box_outline); 
                    mille_rect.x = (int)mille.x; 
                    mille_rect.y = (int)mille.y; 
                    if(!milleclicked){
                        SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                    }else if(milleclicked){
                        if(mille_shooting){
                            SDL_RenderCopy(renderer, mille_attacking_spritesheet_texture, &MilleAttackingClips[currentFrame_Mille], &mille_rect);
                        }else if(mille_moving_left || mille_moving_right){
                            SDL_RenderCopy(renderer, mille_walking_spritesheet_texture, &MilleRunningClips[currentFrame_Mille], &mille_rect);
                        }else{
                            SDL_RenderCopy(renderer, mille_standing_spritesheet_texture, &MilleStandingClips[0], &mille_rect);
                        }
                    
                    }
                }

                if(soldier.spawn){
                     // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &soldier_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &soldier_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &soldier_health_box_outline); 

                    soldier_rect.x = (int)soldier.x; 
                    soldier_rect.y = (int)soldier.y; 

                    if(!soldierclicked){
                            SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[3], &soldier_rect);
                    }else if(soldierclicked){
                        if(soldier_shooting){
                            SDL_RenderCopy(renderer, soldier_attacking_spritesheet_texture, &SoldierAttackingClips[1], &soldier_rect);
                        }else if(soldier_moving_left || soldier_moving_right){
                            SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[currentFrame_Soldier], &soldier_rect);
                        }else{
                            SDL_RenderCopy(renderer, soldier_walking_spritesheet_texture, &SoldierRunningClips[3], &soldier_rect);
                        }
                    
                    }
                }
                        

                if(archer_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_archer_full_texture, NULL, &card_archer_rect);
                }else{
                    SDL_RenderCopy(renderer, card_archer_notfull_texture, NULL, &card_archer_rect);
                }
                if(knight_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_knight_full_texture, NULL, &card_knight_rect);
                }else{
                    SDL_RenderCopy(renderer, card_knight_notfull_texture, NULL, &card_knight_rect);
                }
                if(mille_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_mille_full_texture, NULL, &card_mille_rect);
                }else{
                    SDL_RenderCopy(renderer, card_mille_notfull_texture, NULL, &card_mille_rect);
                }
                if(soldier_card_cool >= 100){
                    SDL_RenderCopy(renderer, card_soldier_full_texture, NULL, &card_soldier_rect);
                }else{
                    SDL_RenderCopy(renderer, card_soldier_notfull_texture, NULL, &card_soldier_rect);
                }
                sprintf(archer_card_cool_Text, "%d%%", archer_card_cool);
                renderText(renderer, "5$", card_archer_rect.x+(Windows_Width*12)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, archer_card_cool_Text, card_archer_rect.x + card_archer_rect.w - (Windows_Width*60)/1920, card_archer_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(knight_card_cool_Text, "%d%%", knight_card_cool);
                renderText(renderer, "5$", card_knight_rect.x+(Windows_Width*12)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, knight_card_cool_Text, card_knight_rect.x + card_knight_rect.w - (Windows_Width*60)/1920, card_knight_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                sprintf(mille_card_cool_Text, "%d%%", mille_card_cool);
                renderText(renderer, "5$", card_mille_rect.x+(Windows_Width*12)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, mille_card_cool_Text, card_mille_rect.x + card_mille_rect.w - (Windows_Width*60)/1920, card_mille_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                
                sprintf(soldier_card_cool_Text, "%d%%", soldier_card_cool);
                renderText(renderer, "5$", card_soldier_rect.x+(Windows_Width*12)/1920, card_soldier_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);
                renderText(renderer, soldier_card_cool_Text, card_soldier_rect.x + card_soldier_rect.w - (Windows_Width*60)/1920, card_soldier_rect.y+(Windows_Height*8)/1080, poppinsFont_Cards, 255, 255, 255);

                if(archer_arrow.active){
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, archer_arrow_basic_Width, archer_arrow_basic_Height};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                        SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                    }
                
                


                if(tower_attacker_health>50){
                    if(hot_air_balloon_throw_bomb){
                        SDL_RenderCopy(renderer, hot_air_bombing_balloon_texture, NULL, &hot_air_balloon_rect);
                    }else{
                        SDL_RenderCopy(renderer, hot_air_balloon_texture, NULL, &hot_air_balloon_rect);
                    }
                    if(hot_air_balloon_bomb.active){
                    SDL_Rect hot_air_balloon_bomb_rect= {(int)hot_air_balloon_bomb.x, (int)hot_air_balloon_bomb.y, (Windows_Width*88)/1920, (Windows_Height*80)/1080};
                    if(!hot_air_balloon_bomb_exploide){
                        SDL_RenderCopy(renderer, hot_air_balloon_bomb_texture, NULL, &hot_air_balloon_bomb_rect);
                    }else{
                        SDL_RenderCopy(renderer, hot_air_balloon_bomb_spritesheet, &Hot_Air_Balloon_Bomb_Exploiding[2], &hot_air_balloon_bomb_rect);
                    }
                }
                    SDL_RenderCopy(renderer, level5_tower_fullhealth_texture, NULL, &tower_attacker);
                }else if(tower_attacker_health<=50 && tower_attacker_health>0){
                    if(hot_air_balloon_throw_bomb){
                        SDL_RenderCopy(renderer, hot_air_bombing_balloon_texture, NULL, &hot_air_balloon_rect);
                    }else{
                        SDL_RenderCopy(renderer, hot_air_balloon_texture, NULL, &hot_air_balloon_rect);
                    }
                    if(hot_air_balloon_bomb.active){
                    SDL_Rect hot_air_balloon_bomb_rect= {(int)hot_air_balloon_bomb.x, (int)hot_air_balloon_bomb.y, (Windows_Width*88)/1920, (Windows_Height*80)/1080};
                    if(!hot_air_balloon_bomb_exploide){
                        SDL_RenderCopy(renderer, hot_air_balloon_bomb_texture, NULL, &hot_air_balloon_bomb_rect);
                    }else{
                        SDL_RenderCopy(renderer, hot_air_balloon_bomb_spritesheet, &Hot_Air_Balloon_Bomb_Exploiding[2], &hot_air_balloon_bomb_rect);
                    }
                }
                    SDL_RenderCopy(renderer, level5_tower_halfhealth_texture, NULL, &tower_attacker);
                }else{
                    SDL_RenderCopy(renderer, level5_tower_zerohealth_texture, NULL, &tower_attacker);
                }


                 if((!midPause || archer_showing_ending_scene) && !knight_moving_level1_ending){
                    if(tower_bomb_1_5.active){
                        SDL_Rect tower_bomb_1_5_rect= {(int)tower_bomb_1_5.x, (int)tower_bomb_1_5.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_1_5_rect);
                    }
                    if(tower_bomb_2.active){
                        SDL_Rect tower_bomb_2_rect= {(int)tower_bomb_2.x, (int)tower_bomb_2.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_2_rect);
                    }

                    if(tower_bomb_3.active){
                        SDL_Rect tower_bomb_3_rect= {(int)tower_bomb_3.x, (int)tower_bomb_3.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_3_rect);
                    }
                    if(tower_bomb_4.active){
                        SDL_Rect tower_bomb_4_rect= {(int)tower_bomb_4.x, (int)tower_bomb_4.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                        SDL_RenderCopy(renderer, cannon_ball_texture ,NULL, &tower_bomb_4_rect);
                    }
                }
            

                if(wolf.spawn){
                    // Health
                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &wolf_health_box_innerfill);

                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &wolf_health_box);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &wolf_health_box_outline); 
                    if(wolf_running){
                        SDL_Rect wolfBasic_rect = { (int)wolf.x, (int)wolf.y, wolf_basic_Width, wolf_basic_Height };
                        SDL_RenderCopy(renderer, wolf_running_spritesheet, &WolfRunningClips[currentFrame_Enemy], &wolfBasic_rect);
                    }else if(wolf_attacking){
                        SDL_Rect wolfAttack_rect = { (int)wolf.x, (int)wolf.y, wolf_basic_Width, wolf_basic_Height };
                        SDL_RenderCopy(renderer, wolf_attacking_spritesheet, &WolfAttackingClips[currentFrame_Enemy], &wolfAttack_rect);

                    }
                }
                
                if(!midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                    SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                    if (archer_aiming) {
                        SDL_RenderCopy(renderer, archer_shooting_texture, &ArcherfireClips[currentFrame], &archerFire_rect);
                        SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                        SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                        SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                    } else {
                        SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    }
                }
                
                if(knight.spawn){
                    // Knight Health
                    knight_health_box.x = (int)(knight.x+(Windows_Width*20)/1920);
                    knight_health_box.y = (int)(knight.y+((Windows_Height*-20)/1080));
                    knight_health_box_innerfill.x = knight_health_box.x;
                    knight_health_box_innerfill.y = knight_health_box.y;
                    knight_health_box_outline.x = knight_health_box.x;
                    knight_health_box_outline.y = knight_health_box.y;

                    SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                    SDL_RenderFillRect(renderer, &knight_health_box_innerfill);
                    SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                    SDL_RenderFillRect(renderer, &knight_health_box);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                    SDL_RenderDrawRect(renderer, &knight_health_box_outline);

                    if(knight_moving_left || knight_moving_right){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_spritesheet, &KnightRunnigClips[currentFrame_Knight], &knightBasic_rect);
                    }else if(knight_shooting){
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_running_attacking_spritesheet, &KnightRunnig_AttackingClips[currentFrame_Knight], &knightBasic_rect);
                    }else{
                        SDL_Rect knightBasic_rect = { (int)knight.x, (int)knight.y, knight_basic_Width, knight_basic_Height };
                        SDL_RenderCopy(renderer, knight_standing_spritesheet,NULL, &knightBasic_rect);
                    }
                }

                // Archer All Health
                archer_health_box.x = (int)(archer.x+(Windows_Width*20)/1920);
                archer_health_box.y = (int)(archer.y+((Windows_Height*-20)/1080));
                archer_health_box_innerfill.x = archer_health_box.x;
                archer_health_box_innerfill.y = archer_health_box.y;
                archer_health_box_outline.x = archer_health_box.x;
                archer_health_box_outline.y = archer_health_box.y;
                
                // Health
                SDL_SetRenderDrawColor(renderer, 240, 236, 235,255);
                SDL_RenderFillRect(renderer, &tower_health_box_innerfill);
                SDL_RenderFillRect(renderer, &archer_health_box_innerfill);

                char towerHealth_val[50];
                sprintf(towerHealth_val, "Tower Health: %d %%", tower_attacker_health);
                renderText(renderer, towerHealth_val, tower_health_box.x, tower_health_box.y-40, poppinsFont_Normal_Customize, 255, 255, 255);
                SDL_SetRenderDrawColor(renderer, 182,33,45, 255);
                SDL_RenderFillRect(renderer, &tower_health_box);
                SDL_RenderFillRect(renderer, &archer_health_box);

                char archerHealth_val[50];
                SDL_SetRenderDrawColor(renderer, 127,23,31, 255);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                SDL_RenderDrawRect(renderer, &tower_health_box_outline); 
                SDL_RenderDrawRect(renderer, &archer_health_box_outline); 
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
                // SDL_RenderDrawRect(renderer, &archer_health_box_outline); 

                // Pause Button
                SDL_RenderCopy(renderer, pauseButton, NULL, &pauseButton_rect);
                if(midPause){
                    SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };
                    SDL_RenderCopy(renderer, archer_walking_texture, &ArcherwalkClips[currentFrame], &archerBasic_rect);
                    if(midPause_dialog_starting){
                        SDL_Rect dialog_background_rect = {0, 0, Windows_Width, Windows_Height};
                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                        SDL_RenderFillRect(renderer, &dialog_background_rect);
                        if(!level2_ending_scene){
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level5_king_dialog1_texture, NULL, &level1_king_dialog1_rect);
                            }else if(dialog2_start){
                                SDL_RenderCopy(renderer, level5_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }else if(dialog3_start){
                                SDL_RenderCopy(renderer, level5_king_dialog2_texture, NULL, &level1_king_dialog1_rect);
                            }else if(dialog4_start){
                                SDL_RenderCopy(renderer, level5_archer_dialog2_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }else{
                            if(dialog1_start){
                                SDL_RenderCopy(renderer, level1_archer_dialog1_texture, NULL, &level1_archer_dialog1_rect);
                            }
                        }
                            
                    }
                }
            }
            
        }
        if(gamePause){
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderFillRect(renderer, &pause_screen_background);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &pause_screen);
            SDL_RenderCopy(renderer, resumeButton, NULL, &resumeButton_rect);
            SDL_RenderCopy(renderer, restartButton, NULL, &restartButton_rect);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButton_rect);
            if(music){
                SDL_RenderCopy(renderer, music_on_pauseButton, NULL, &music_pause_button_rect);
            }else{
                SDL_RenderCopy(renderer, music_off_pauseButton, NULL, &music_pause_button_rect);
            }
            if(sound){
                SDL_RenderCopy(renderer, sound_on_pauseButton, NULL, &sound_pause_button_rect);
            }else{
                SDL_RenderCopy(renderer, sound_off_pauseButton, NULL, &sound_pause_button_rect);
            }
            renderText(renderer, "GAME PAUSED", pause_screen.x + (Windows_Width*110)/1920, pause_screen.y + archer_arrow_basic_Height, pause_poppinsFont_Normal, 255,255,255);
        }else if(gameOver){
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderFillRect(renderer, &gameOver_screen_background);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &gameOver_screen);
            SDL_RenderCopy(renderer, retryButton, NULL, &retryButton_rect);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButton_gameOver_rect);
            renderText(renderer, "Game Over", gameOver_screen.x + (Windows_Width*136)/1920, gameOver_screen.y + archer_arrow_basic_Height, pause_poppinsFont_Normal, 255,0,0);
        }else if(youWin){
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderFillRect(renderer, &youWin_screen_background);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &youWin_screen);
            SDL_RenderCopy(renderer, nextlevelButton, NULL, &nextlevelButton_rect);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButton_youWin_rect);
            renderText(renderer, "You Win", youWin_screen.x + (Windows_Width*166)/1920, youWin_screen.y + archer_arrow_basic_Height, pause_poppinsFont_Normal, 0,255,0);
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

