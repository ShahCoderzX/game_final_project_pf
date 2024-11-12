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

    // Mix Audio Check
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }

    // All Audios
    Mix_Chunk *button_sound = Mix_LoadWAV("./audio/button_sound.mp3");
    Mix_Chunk *towerGame_backgroundMusic = Mix_LoadWAV("./audio/deckofdominions/background.mp3");
    Mix_Chunk *typing_SoundEffect = Mix_LoadWAV("./audio/deckofdominions/typing.mp3");
    
    Mix_Chunk *archerDamage_sound = Mix_LoadWAV("./audio/deckofdominions/archer_damage.mp3");
    Mix_Chunk *towerDamage_sound = Mix_LoadWAV("./audio/deckofdominions/tower_damage.mp3");
    Mix_Chunk *towerShooting_sound = Mix_LoadWAV("./audio/deckofdominions/tower_shooting.mp3");
    Mix_Chunk *archerShooting_sound = Mix_LoadWAV("./audio/deckofdominions/archer_shooting.mp3");

    Mix_Chunk *level1Background_Sound = Mix_LoadWAV("./audio/deckofdominions/level1_Background.mp3");


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
    
    // Define Tower Menu button dimensions
    const int tower_menu_button_width = (Windows_Width * 220) / 1920; 
    const int tower_menu_button_height = (Windows_Height * 74) / 1080; 

    // Select Section textures
    SDL_Texture* select_menu_playDeck_button = IMG_LoadTexture(renderer, "./image/playdeck.png");
    SDL_Texture* select_menu_quit_button = IMG_LoadTexture(renderer, "./image/quitselect.png");

    const int select_button_Width = (Windows_Width*300)/1920;
    const int select_button_Height = (Windows_Width*80)/1080;

    SDL_Rect select_menu_playDeck_button_rect = {(Windows_Width/2)-(select_button_Width)/2, (Windows_Height/2)-(select_button_Height/2)-(Windows_Height*60)/1080, select_button_Width, select_button_Height};
    SDL_Rect select_menu_quit_button_rect = {(Windows_Width/2)-(select_button_Width)/2, select_menu_playDeck_button_rect.y + select_button_Height + (Windows_Height*30)/1080, select_button_Width, select_button_Height};

    // All Tower Game Button Textures

    // Menu Page
    SDL_Texture* start_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/start.png");
    SDL_Texture* continue_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/continue.png");
    SDL_Texture* level_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level.png");
    SDL_Texture* option_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/option.png");
    SDL_Texture* control_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/controls.png");
    SDL_Texture* quit_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/quit.png");
    SDL_Texture* back_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/back.png"); 

    // Account Buttons
    SDL_Texture* account_create_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/create.png");
    SDL_Texture* account_login_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/login.png");

    // Tower Game Account Menu
    const int account_main_Width = 800;
    const int account_main_Height = 600;
    SDL_Rect account_main_rect = {(Windows_Width/2)-(account_main_Width/2), (Windows_Height/2)-(account_main_Height/2), account_main_Width, account_main_Height};

    const int account_input_Width = 600;
    const int account_input_Height = 50;
    SDL_Rect account_username = {(account_main_rect.x)+100, account_main_rect.y+220, account_input_Width, account_input_Height};
    SDL_Rect account_password = {(account_main_rect.x)+100, account_username.y+140, account_input_Width, account_input_Height};

    // Account Buttons
    SDL_Rect account_create_button1_rect = {account_main_rect.x + (account_main_rect.w/2 - tower_menu_button_width/
    2), account_main_rect.y + (account_main_rect.h - 150), tower_menu_button_width, tower_menu_button_height};
    SDL_Rect account_login_button1_rect = {account_main_rect.x + (account_main_rect.w/2 - tower_menu_button_width/
    2), account_main_rect.y + (account_main_rect.h - 150), tower_menu_button_width, tower_menu_button_height};

    SDL_Rect account_create_button_rect = {account_main_rect.x+(account_main_rect.w/2 - (tower_menu_button_width*2+20)/2), account_main_rect.y + (account_main_rect.h/2 - tower_menu_button_height/2), tower_menu_button_width, tower_menu_button_height};
    SDL_Rect account_login_button_rect = {account_create_button_rect.x+tower_menu_button_width+20 , account_main_rect.y + (account_main_rect.h/2 - tower_menu_button_height/2), tower_menu_button_width, tower_menu_button_height};

    // Typwriter Blinker for Account Input
    SDL_Rect account_username_typewriter = {account_username.x+10, account_username.y + 4, 4, 42};
    SDL_Rect account_password_typewriter = {account_password.x+10, account_password.y + 4, 4, 42};

    // Calculate the total height of all buttons and spacing
    const int total_buttons_height = (tower_menu_button_height * 4) + (Windows_Height * 16 / 1080 * 3); // 4 buttons + 3 spacings
    const int starting_y_position = (Windows_Height / 2) - (total_buttons_height / 2); // Calculate the starting y position for vertical centering
    const int center_button_horizontally = (Windows_Width / 2) - (tower_menu_button_width / 2);

    // Initialize button rectangles
    SDL_Rect continue_button_rect = {
        center_button_horizontally, 
        starting_y_position, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect level_button_rect = {
        center_button_horizontally, 
        continue_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect option_button_rect = {
        center_button_horizontally, 
        level_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect quit_button_rect = {
        center_button_horizontally,
        option_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };


    // Tower Levels Button
    SDL_Texture* level1_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level1.png");
    SDL_Texture* level2_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level2.png");
    SDL_Texture* level3_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level3.png");
    SDL_Texture* level4_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level4.png");
    SDL_Texture* level5_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level5.png");
    SDL_Texture* level6_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level6.png");
    SDL_Texture* level7_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level7.png");
    SDL_Texture* level8_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/level8.png");

    const int tower_menu_level_button_Width = (Windows_Width*110)/1920;
    const int tower_menu_level_button_Height = (Windows_Height*70)/1080;

    SDL_Rect level1_button_rect = {
        (Windows_Width*720)/1920, 
        starting_y_position, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level2_button_rect = {
        level1_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level3_button_rect = {
        level2_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level4_button_rect = {
        level3_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level5_button_rect = {
        (Windows_Width*720)/1920, 
        starting_y_position + (Windows_Height*100)/1080, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level6_button_rect = {
        level5_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position + (Windows_Height*100)/1080, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level7_button_rect = {
        level6_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position + (Windows_Height*100)/1080, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };
    SDL_Rect level8_button_rect = {
        level7_button_rect.x + (Windows_Width*130)/1920, 
        starting_y_position + (Windows_Height*100)/1080, 
        tower_menu_level_button_Width, 
        tower_menu_level_button_Height
    };

   
    SDL_Texture* shop_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/shop.png");
    SDL_Texture* sound_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound.png");
    SDL_Texture* music_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/music.png");
    SDL_Texture* on_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/on.png");
    SDL_Texture* off_button = IMG_LoadTexture(renderer, "./image/towerGame/menu/off.png");

    SDL_Rect back_button_rect = {(Windows_Width*100)/1920, // Center horizontally
    continue_button_rect.y - (tower_menu_button_height + (Windows_Height * 40) / 1080), // Adjusted position above continue button
    tower_menu_button_width,
    tower_menu_button_height};

    // Option Buttons
    SDL_Rect shop_button_rect = {
        center_button_horizontally, 
        starting_y_position, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect music_button_rect = {
        center_button_horizontally,
        shop_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect sound_button_rect = {
        center_button_horizontally, 
        music_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };

    SDL_Rect control_button_rect = {
        center_button_horizontally, 
        sound_button_rect.y + tower_menu_button_height + (Windows_Height * 16) / 1080, 
        tower_menu_button_width,
        tower_menu_button_height
    };
    
    SDL_Rect music_on_off_button_rect = {(Windows_Width*750)/1920, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect sound_on_off_button_rect = {(Windows_Width*750)/1920, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect on_button_rect = {(Windows_Width*770)/1920 + tower_menu_button_width, starting_y_position, tower_menu_button_width, tower_menu_button_height};
    SDL_Rect off_button_rect = {(Windows_Width*770)/1920 + tower_menu_button_width, starting_y_position, tower_menu_button_width, tower_menu_button_height};

    
    // Buttons For Pause Section
    SDL_Texture* resumeButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/resume.png");
    SDL_Texture* restartButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/restart.png");
    SDL_Texture* homeButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/home.png");
    SDL_Texture* pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/pause.png");
    SDL_Texture* sound_on_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound_on_pause.png");
    SDL_Texture* sound_off_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/sound_off_pause.png");
    SDL_Texture* music_on_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/music_on_pause.png");
    SDL_Texture* music_off_pauseButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/music_off_pause.png");

    // For Game Over
    SDL_Texture* retryButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/retry.png");
    // For You Win
    SDL_Texture* nextlevelButton = IMG_LoadTexture(renderer, "./image/towerGame/menu/nextlevel.png");


    // Surface Texture
    SDL_Texture* main_backgroundTexture = IMG_LoadTexture(renderer, "./image/mainbackground.png");
    SDL_Texture* towerGame_homePage_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_background.png");
    SDL_Texture* towerGame_option_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_option_background.png");
    SDL_Texture* towerGame_level_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_level_background.png");
    SDL_Texture* towerGame_shop_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_shop_background.png");
    SDL_Texture* towerGame_sound_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_sound_background.png");
    SDL_Texture* towerGame_music_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_music_background.png");
    SDL_Texture* towerGame_control_backgroundTexture = IMG_LoadTexture(renderer, "./image/tower_control_background.png");
    SDL_Texture* towerGame_mainPage_backgroundTexture = IMG_LoadTexture(renderer, "./image/main_tower_background.png");
    SDL_Texture* towerGame_level1_background = IMG_LoadTexture(renderer, "./image/towerGame_level1_background.jpg");

    SDL_Texture* arrow = IMG_LoadTexture(renderer, "./image/arrow.png");
    SDL_Texture* archer_standing_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_standing.png");
    SDL_Texture* archer_walking_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_walking.png");
    SDL_Texture* archer_shooting_texture = IMG_LoadTexture(renderer, "./image/towerGame/characters/archer/archer_shooting.png");

    // Cards
    SDL_Texture* card_archer_full_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/archer_full.png");
    SDL_Texture* card_archer_notfull_texture = IMG_LoadTexture(renderer, "./image/towerGame/cards/archer_notfull.png");

    const int cards_Width = (Windows_Width*114)/1920;
    const int cards_Height = (Windows_Height*144)/1080;
    SDL_Rect card_archer_rect= {(Windows_Width/2)-(cards_Width/2), (Windows_Height-cards_Height)-((Windows_Height*50)/1080), cards_Width, cards_Height};
    // SDL_Rect card_archer_notfull_rect= {card_archer_full_rect.x + cards_Width + ((Windows_Width*20)/1920), card_archer_full_rect.y, cards_Width, cards_Height};
    SDL_Rect cardsBackground_rect = {0, Windows_Height - ((Windows_Height*240)/1080), Windows_Width, (Windows_Height*240)/1080};
    SDL_Rect cardsBackground_outline_rect = {0, cardsBackground_rect.y, Windows_Width, 0};
    

    const int small_pause_button_Width = (Windows_Width*74)/1920;
    const int small_pause_button_Height = (Windows_Height*74)/1080;
    const int normal_pause_button_Width = (Windows_Width*200)/1920;
    const int normal_pause_button_Height = (Windows_Height*74)/1080;
    const int pause_screen_Width = (Windows_Width*500)/1920;
    const int pause_screen_Height = (Windows_Height*600)/1080;
    SDL_Rect pause_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect pause_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - pause_screen_Height) / 2, pause_screen_Width, pause_screen_Height};
    SDL_Rect pauseButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    // Game Over 
    const int gameOver_screen_Height = (Windows_Height*400)/1080;
    SDL_Rect gameOver_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect gameOver_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - gameOver_screen_Height) / 2, pause_screen_Width, gameOver_screen_Height};
    SDL_Rect gameOverButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    // You Win
    const int youWin_screen_Height = (Windows_Height*400)/1080;
    SDL_Rect youWin_screen_background = {0, 0, Windows_Width, Windows_Height};
    SDL_Rect youWin_screen = { (Windows_Width - pause_screen_Width) / 2,
    (Windows_Height - youWin_screen_Height) / 2, pause_screen_Width, youWin_screen_Height};
    SDL_Rect youWinButton_rect = {Windows_Width-((Windows_Width*20)/1920)-(small_pause_button_Width), (Windows_Height*20)/1080, small_pause_button_Width,small_pause_button_Height};

    SDL_Rect resumeButton_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  
        pause_screen.y + (Windows_Height*140)/1080,  
        normal_pause_button_Width,
        normal_pause_button_Height
    };

    // Define spacing and button dimensions
    const int buttonSpacing = (Windows_Height*20)/1080;  

    SDL_Rect restartButton_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,
        resumeButton_rect.y + normal_pause_button_Height + buttonSpacing,
        normal_pause_button_Width,
        normal_pause_button_Height
    };

    SDL_Rect homeButton_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,
        restartButton_rect.y + normal_pause_button_Height + buttonSpacing,
        normal_pause_button_Width,
        normal_pause_button_Height
    };

    SDL_Rect music_pause_button_rect = {
        pause_screen.x + (pause_screen.w - (small_pause_button_Width*2 + (Windows_Width*20)/1920)) / 2,
        homeButton_rect.y + normal_pause_button_Height + buttonSpacing,
        small_pause_button_Width,
        small_pause_button_Height
    };

    SDL_Rect sound_pause_button_rect = {
        music_pause_button_rect.x + (Windows_Width*20)/1920 + small_pause_button_Width,
        homeButton_rect.y + normal_pause_button_Height + buttonSpacing,
        small_pause_button_Width,
        small_pause_button_Height
    };

    // For Game Over 
    SDL_Rect retryButton_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  
        pause_screen.y + (Windows_Height*250)/1080,  
        normal_pause_button_Width,
        normal_pause_button_Height
    };
    // Spacing
    SDL_Rect homeButton_gameOver_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,
        retryButton_rect.y + normal_pause_button_Height + buttonSpacing,
        normal_pause_button_Width,
        normal_pause_button_Height
    };

    // For Game Over 
    SDL_Rect nextlevelButton_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,  
        pause_screen.y + (Windows_Height*250)/1080,  
        normal_pause_button_Width,
        normal_pause_button_Height
    };
    // Spacing
    SDL_Rect homeButton_youWin_rect = {
        pause_screen.x + (pause_screen.w - normal_pause_button_Width) / 2,
        retryButton_rect.y + normal_pause_button_Height + buttonSpacing,
        normal_pause_button_Width,
        normal_pause_button_Height
    };


    // RECT OF ARROW
    const int velocity_bar_Width = (Windows_Width*10)/1920;
    const int velocity_bar_Height = (Windows_Height*180)/1080;
    SDL_Rect arrow_velocity_bar_outline = {Windows_Width-(Windows_Width*60)/1920, (Windows_Height/2)-(velocity_bar_Height/2), velocity_bar_Width, velocity_bar_Height};
    SDL_Rect arrow_velocity_bar_innerfill = {Windows_Width-(Windows_Width*60)/1920, (Windows_Height/2)-(velocity_bar_Height/2), velocity_bar_Width, velocity_bar_Height};
    SDL_Rect arrow_velocity_bar = {arrow_velocity_bar_innerfill.x, arrow_velocity_bar_outline.y, velocity_bar_Width, 0};

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
    TTF_Font* poppinsFont_Head = TTF_OpenFont("./font/poppinsFont.ttf", 60);
    TTF_Font* poppinsFont_Normal = TTF_OpenFont("./font/poppinsFont.ttf", 30);
    TTF_Font* poppinsFont_Normal_Customize = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*30)/1920);
    TTF_Font* pause_poppinsFont_Normal = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*40)/1920);
    TTF_Font* poppinsFont_Small = TTF_OpenFont("./font/poppinsFont.ttf", 20);
    TTF_Font* poppinsFont_Cards = TTF_OpenFont("./font/poppinsFont.ttf", (Windows_Width*20)/1920);

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

    

    // For Tower Game
    
    // !
    const int towerWidth = (Windows_Width*100)/1920;
    const int towerHeight = (Windows_Height*300)/1080;

    // Tower Characters
    SDL_Rect tower_attacker = {(Windows_Width*100)/1920, Windows_Height - (towerHeight+(int)(Windows_Height * 0.28)), towerWidth, towerHeight};
    Pointer tower_bomb = {(float)tower_attacker.x, (float)tower_attacker.y, 0, 0, false};
    
    // SDL_Rect archer_character = {Windows_Width-(archer_character.w + 100), Windows_Height - (archer_character.h + 300), 200, 160};
    // Archer Width, Height, x & y axis for all the window resoltuion
    int archer_basic_Width = (Windows_Width * ARCHER_BASIC_FRAME_WIDTH) / 1920;
    int archer_basic_Height = (Windows_Height * ARCHER_BASIC_FRAME_HEIGHT) / 1080;
    int archer_fire_Width = (Windows_Height * ARCHER_FIRE_FRAME_WIDTH) / 1080;
    int archer_fire_Height = (Windows_Height * ARCHER_BASIC_FRAME_HEIGHT) / 1080;
    int archerX = Windows_Width - (int)((200 * ScaleX) + (100 * ScaleY));
    int archerY = Windows_Height - (int)((160 * ScaleY) + (330 * ScaleY));

    Pointer archer = {(float)archerX, (float)archerY, 0, 0, false};
    Arrow archer_arrow = {(float)archer.x, (float)archer.y, 0, 0, false, (float)0};

    // Health Bar
    const int tower_health_box_Width = (Windows_Width*1000)/1920;
    const int tower_health_box_Height = (Windows_Height*30)/1080;
    SDL_Rect tower_health_box= {(Windows_Width - tower_health_box_Width)/2, (Windows_Height * 100) / 1080, tower_health_box_Width, tower_health_box_Height};
    SDL_Rect tower_health_box_outline= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};
    SDL_Rect tower_health_box_innerfill= {tower_health_box.x,tower_health_box.y,tower_health_box.w,tower_health_box.h};

    // Characters Health Box
    const int character_health_Width = (Windows_Width*100)/1920;
    const int character_health_Height = (Windows_Height*10)/1080;
    SDL_Rect archer_health_box= {0, 0, character_health_Width, character_health_Height};
    SDL_Rect archer_health_box_outline= {archer_health_box.x,archer_health_box.y,archer_health_box.w,archer_health_box.h};
    SDL_Rect archer_health_box_innerfill= {archer_health_box.x,archer_health_box.y,archer_health_box.w,archer_health_box.h};

    // SDL_Rect archer_health_box = {Windows_Width-archer_health_box.w - (Windows_Width*100)/1920, (Windows_Height * 100) / 1080, 400, 60};
    // SDL_Rect archer_health_box_outline = {archer_health_box.x, archer_health_box.y, archer_health_box.w, archer_health_box.h};
    // SDL_Rect archer_health_box_innerfill = {archer_health_box.x, archer_health_box.y, archer_health_box.w, archer_health_box.h};


    // Boolean
    bool running = true;
    bool gameStarted = false;

    // For Main Home/ First  Page
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
    bool towerGame_Started_level6 = false;
    bool towerGame_Started_level7 = false;
    bool towerGame_Started_level8 = false;

    // Health For Tower Game
    int tower_attacker_health = 100;
    int archer_health = 100;

    // ON / OFF
    bool music = true;
    bool sound = true;

    // Paused 
    bool gamePause = false;
    bool gameOver = false;
    bool youWin = false;
    bool levelRestart = false;

    // !
    float tower_attack_timer = 0.0f;  // Timer for tracking attack delay
    const float tower_attack_delay = 1.0f;  // Delay in seconds

    // Inside your game loop:
    float deltaTime = 0.016f;  // Assume you calculate the delta time from the frame rate (e.g., 1/60 seconds)
    tower_attack_timer += deltaTime;  // Update the timer

    // Event
    SDL_Event event;

    // Variables for animation
    int currentFrame = 0;
    SDL_Rect walkClips[TOTAL_WALK_FRAMES];
    SDL_Rect standClips[TOTAL_STAND_FRAME];
    SDL_Rect fireClips[TOTAL_FIRE_FRAMES];

    // Set up frames for walking sprite sheet
    for (int i = 0; i < TOTAL_WALK_FRAMES; i++) {
        walkClips[i].x = i * ARCHER_BASIC_FRAME_WIDTH;
        walkClips[i].y = 0;
        walkClips[i].w = ARCHER_BASIC_FRAME_WIDTH;
        walkClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }
    // Set up frames for satnding sprite sheet
    for (int i = 0; i < TOTAL_WALK_FRAMES; i++) {
        standClips[i].x = i * ARCHER_BASIC_FRAME_WIDTH;
        standClips[i].y = 0;
        standClips[i].w = ARCHER_BASIC_FRAME_WIDTH;
        standClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }
    // Set up frames for walking sprite sheet
    for (int i = 0; i < TOTAL_WALK_FRAMES; i++) {
        walkClips[i].x = i * ARCHER_BASIC_FRAME_WIDTH;
        walkClips[i].y = 0;
        walkClips[i].w = ARCHER_BASIC_FRAME_WIDTH;
        walkClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }

    // Set up frames for shooting sprite sheet
    for (int i = 0; i < TOTAL_FIRE_FRAMES; i++) {
        fireClips[i].x = i * ARCHER_FIRE_FRAME_WIDTH;
        fireClips[i].y = 0;
        fireClips[i].w = ARCHER_FIRE_FRAME_WIDTH;
        fireClips[i].h = ARCHER_BASIC_FRAME_HEIGHT;
    }

    // For Archer
    bool archer_moving_left = false;
    bool archer_moving_right = false;
    bool archer_aiming = false;
    bool archer_shooting = false;
    bool archer_standing = false;

    // Money
    int money = 0;
    int archer_card_cool = 0;

    // Timer
    float money_Timer = 0.00f;
    float money_Increase_Timer = 0.01f;
    float archer_card_cool_Timer = 0.00f;
    float archer_card_Increase_cool_Timer = 0.05f;

    // For Handling & Changing Frames
    Uint32 frameDelay = 100;  // Delay between frames (in milliseconds)
    Uint32 lastFrameTime = 0;

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

                    if(checkButtonClick(mouseX, mouseY, &select_menu_playDeck_button_rect)){
                        Mix_PlayChannel(1, button_sound, 0); 
                        selectedGame_page = false;
                        towerGame = true;
                        towerGame_start_bt = true;
                    }else if(checkButtonClick(mouseX, mouseY, &select_menu_quit_button_rect)){
                        Mix_PlayChannel(1, button_sound, 0); 
                        selectedGame_page = false;
                        running = false;
                    }
                }
                
                if(towerGame){
                    if(!towerGame_Started){
                        if(towerGame_start_bt){
                            mainBackground = towerGame_mainPage_backgroundTexture;
                            if(towerGame_account){
                                if(towerGame_account_choose){
                                    if(checkButtonClick(mouseX, mouseY, &account_create_button_rect)){
                                        if(sound){
                                            Mix_PlayChannel(1, button_sound, 0); 
                                        }
                                        towerGame_account_choose = false;
                                        towerGame_account_create = true;
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
                        else if(towerGame_homemenu){
                            if(checkButtonClick(mouseX, mouseY, &continue_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Continue Game");
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
                                towerGame_Started_level1 = true;
                                towerGame_Started = true;
                                mainBackground = towerGame_level1_background;
                                towerGame_levelmenu = false;
                                towerGame_homemenu = true;
                                money = 5;
                                archer_card_cool = 100;
                                archer_standing = false;

                                Mix_HaltChannel(0);
                                if(music){
                                    Mix_PlayChannel(0, level1Background_Sound, 0);
                                }
                            }else if(checkButtonClick(mouseX, mouseY, &level2_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level3_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level4_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level5_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level6_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level7_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
                            }else if(checkButtonClick(mouseX, mouseY, &level8_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("Level");
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
                                towerGame_option_shopMenu = true;
                                mainBackground = towerGame_shop_backgroundTexture;
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
                    }else if(towerGame_Started){
                        if(towerGame_Started_level1){
                            mainBackground = towerGame_level1_background;
                            if(checkButtonClick(mouseX, mouseY, &card_archer_rect)){
                                if(archer_card_cool>=100 && money >= 5){
                                    archer_card_cool = 0;
                                    money -= 5;
                                }
                            }
                            
                        }
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
                                towerGame_Started_level1 = false;
                                towerGame_Started = false;
                                towerGame_homemenu = true;
                                gamePause = false;
                                mainBackground = towerGame_homePage_backgroundTexture;
                            }else  if(checkButtonClick(mouseX, mouseY, &restartButton_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                gamePause = false;
                            }else if(checkButtonClick(mouseX, mouseY, &music_pause_button_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                music = !music;
                                if(music){
                                    Mix_PlayChannel(0, towerGame_backgroundMusic, -1);
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
                                towerGame_Started_level1 = false;
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
                                levelRestart = true;
                            }else  if(checkButtonClick(mouseX, mouseY, &homeButton_youWin_rect)){
                                if(sound){
                                    Mix_PlayChannel(1, button_sound, 0); 
                                }
                                printf("shjdhsd");
                                towerGame_Started_level1 = false;
                                towerGame_Started = false;
                                towerGame_homemenu = true;
                                youWin = false;
                                mainBackground = towerGame_homePage_backgroundTexture;
                            }
                        }
                    }
                }
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
                                if(!towerGame_account){
                                    // towerGame_start_bt = true;
                                    towerGame_account = true;
                                    towerGame_account_choose = true;
                                    // towerGame_homemenu = true;
                                    // mainBackground = towerGame_homePage_backgroundTexture;
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
                    }else if(towerGame_Started && !gamePause &&!youWin){
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_LEFT:
                            if(archer.x > (Windows_Width - ARCHER_FIRE_FRAME_WIDTH)/2){
                                archer_moving_left = true;
                                archer_standing = false;
                                archer.x -= 16*ScaleX;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(archer.x < Windows_Width - (ARCHER_FIRE_FRAME_WIDTH)){
                                archer_moving_right = true;
                                archer_standing = false;
                                archer.x += 16*ScaleX;
                            }
                            break;
                        case SDLK_UP: 
                            if (archer.y >= archerY) {  
                                archer_standing = false;
                                archer.vy = -10*ScaleY; 
                                archer.active = true; 
                            }
                            break;
                        case SDLK_f: 
                             // Increase velocity while 'F' is held down
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
                            break;
                        default:
                            break;
                        }
                    }
                }
            }else if(event.type == SDL_KEYUP){
                if(towerGame_Started && !gamePause){
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                                archer_moving_left = false;
                                // archer_standing = true;
                                currentFrame = 0;
                            break;
                        case SDLK_RIGHT:
                                archer_moving_right = false;
                                // archer_standing = true;
                                currentFrame = 0;
                            break;
                        case SDLK_f: 
                                if(sound){
                                    Mix_PlayChannel(2, archerShooting_sound, 0); 
                                }
                                if(!archer_arrow.active){
                                    archer_arrow.x = archer.x;
                                    archer_arrow.y = archer.y;
                                    archer_arrow.active = true;
                                    archer_arrow.angle = 0;
                                }   
                                archer_shooting = true;
                                archer_aiming = false;
                                arrow_velocity_bar.h = 0;
                            break;
                        default:
                            break;
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
    
    if(towerGame_Started && !gamePause && !youWin && !gameOver){
        
        if(towerGame_Started_level1){
            
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
                    tower_bomb.vx += 20 * ScaleX;
                    tower_bomb.vy += -18 * ScaleY;
                    tower_bomb.active = true;
                }
            }
                // tower_bomb.active = true;
            if(tower_bomb.active){
                tower_bomb.x += tower_bomb.vx; 
                tower_bomb.y += tower_bomb.vy; 
                tower_bomb.vy += 0.5 * ScaleY;
                if(tower_bomb.x > Windows_Width || tower_bomb.y > Windows_Height || checkCollisionArcher(&tower_bomb, &archer)){
                    if(checkCollisionArcher(&tower_bomb, &archer)){
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



                if(archer_arrow.x > Windows_Width || archer_arrow.y > Windows_Height || checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y)){
                    if(checkCollisionTower(&archer_arrow, (int)tower_attacker.x, (int)tower_attacker.y)){
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
                // // Render button text
                // renderText(renderer, "DECK", (Windows_Width/2)-100, (Windows_Height/2)-200, towerGame_fontHeading,0 ,0, 0);
                // renderText(renderer, "OF", (Windows_Width/2)-40, (Windows_Height/2) - 100 , towerGame_fontHeading,0 ,0, 0);
                // renderText(renderer, "DOMINIONS", (Windows_Width/2)-240, (Windows_Height/2), towerGame_fontHeading,0 ,0, 0);
                // renderText(renderer, "Press \"Enter\" to start the Game", (Windows_Width/2)-250, (Windows_Height-100), towerGame_font,255 , 255, 255);
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
                SDL_RenderCopy(renderer, level6_button, NULL, &level6_button_rect);
                SDL_RenderCopy(renderer, level7_button, NULL, &level7_button_rect);
                SDL_RenderCopy(renderer, level8_button, NULL, &level8_button_rect);
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
            renderText(renderer, levelMoney, (Windows_Width*20)/1920, (Windows_Height*20)/1080, poppinsFont_Normal_Customize, 0, 0, 0);
            if(towerGame_Started_level1){
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
                    SDL_Rect archer_arrow_rect = {(int)archer_arrow.x, (int)archer_arrow.y, (Windows_Width*120)/1920, (Windows_Height*50)/1080};
                    SDL_Point center = {archer_arrow_rect.w / 2, archer_arrow_rect.h / 2};  
                    SDL_RenderCopyEx(renderer, arrow, NULL, &archer_arrow_rect, archer_arrow.angle, &center, SDL_FLIP_NONE);
                }


                SDL_Rect tower_bomb_rect= {(int)tower_bomb.x, (int)tower_bomb.y, (Windows_Width*40)/1920, (Windows_Height*40)/1080};
                SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
                SDL_RenderFillRect(renderer, &tower_bomb_rect);

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &tower_attacker);

                SDL_Rect archerBasic_rect = { (int)archer.x, (int)archer.y, archer_basic_Width, archer_basic_Height };  // Position and size to render
                SDL_Rect archerFire_rect = { (int)archer.x, (int)archer.y, archer_fire_Width, archer_fire_Height };  // Position and size to render
                if (archer_aiming) {
                    SDL_RenderCopy(renderer, archer_shooting_texture, &fireClips[currentFrame], &archerFire_rect);
                    SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
                    SDL_RenderFillRect(renderer, &arrow_velocity_bar_innerfill);
                    SDL_SetRenderDrawColor(renderer, 0,255,0, 255);
                    SDL_RenderFillRect(renderer, &arrow_velocity_bar);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &arrow_velocity_bar_outline);
                } else {
                    SDL_RenderCopy(renderer, archer_walking_texture, &walkClips[currentFrame], &archerBasic_rect);
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
            renderText(renderer, "GAME PAUSED", pause_screen.x + (Windows_Width*110)/1920, pause_screen.y + (Windows_Height*50)/1080, pause_poppinsFont_Normal, 255,255,255);
        }else if(gameOver){
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderFillRect(renderer, &gameOver_screen_background);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &gameOver_screen);
            SDL_RenderCopy(renderer, retryButton, NULL, &retryButton_rect);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButton_gameOver_rect);
            renderText(renderer, "Game Over", gameOver_screen.x + (Windows_Width*136)/1920, gameOver_screen.y + (Windows_Height*50)/1080, pause_poppinsFont_Normal, 255,0,0);
        }else if(youWin){
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderFillRect(renderer, &youWin_screen_background);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
            SDL_RenderFillRect(renderer, &youWin_screen);
            SDL_RenderCopy(renderer, nextlevelButton, NULL, &nextlevelButton_rect);
            SDL_RenderCopy(renderer, homeButton, NULL, &homeButton_youWin_rect);
            renderText(renderer, "You Win", youWin_screen.x + (Windows_Width*166)/1920, youWin_screen.y + (Windows_Height*50)/1080, pause_poppinsFont_Normal, 0,255,0);
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

