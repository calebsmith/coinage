#include "ui_init.h"

// Initialize SDL and game window
bool init_ui(Asset_t * assets)
{
    assets->font = NULL;
    assets->grunt_sound = NULL;
    assets->win_sound = NULL;
    assets->coin_sound = NULL;
    // initialize SDL, the window, sound system, and fonting
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableKeyRepeat(0, 0);
    SDL_WM_SetCaption("Coinage", NULL);
    Mix_Init(0);
    TTF_Init();
    // initialize the video buffer in the window
    assets->buffer = SDL_SetVideoMode(SCRWIDTH, SCRHEIGHT, SCRBPP, SDL_HWSURFACE);
    if (!assets->buffer) {
        return false;
    }
    // Initialize Audio
    if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 2048) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    // Load sound files
    assets->grunt_sound = Mix_LoadWAV("data/sounds/grunt.wav");
    if (assets->grunt_sound == NULL) {
        printf( "Failed to load grunt sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    assets->coin_sound = Mix_LoadWAV("data/sounds/coin.wav");
    if (assets->coin_sound == NULL) {
        printf("Failed to load grunt sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    assets->win_sound = Mix_LoadWAV("data/sounds/win.wav");
    if (assets->win_sound == NULL) {
        printf( "Failed to load grunt sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    // load tile graphics
    assets->spritesheet = load_image("data/images/forest.jpg");
    if (!assets->spritesheet) {
        return false;
    }
    // load player graphics
    assets->player = load_image("data/images/player.png");
    if (!assets->player) {
        return false;
    }
    // load item graphics
    assets->items = load_image("data/images/items.png");
    if (!assets->items) {
        return false;
    }
    // load fonts
    assets->font = TTF_OpenFont("data/fonts/NotCourierSans.otf", 20);
    if (!assets->font) {
        return false;
    }
    return true;
}

SDL_Surface *load_image(const char *filename)
{
    SDL_Surface *image = NULL;
    SDL_Surface *unopt = NULL;

    unopt = IMG_Load(filename);
    if (unopt == NULL) {
        fprintf(stderr, "File not found while trying to load %s\n", filename);
        return NULL;
    }
    image = SDL_DisplayFormatAlpha(unopt);
    SDL_FreeSurface(unopt);
    return image;
}

void quit_ui(Asset_t * assets)
{
    if (assets->grunt_sound) {
        Mix_FreeChunk(assets->grunt_sound);
    }
    if (assets->coin_sound) {
        Mix_FreeChunk(assets->coin_sound);
    }
    if (assets->win_sound) {
        Mix_FreeChunk(assets->win_sound);
    }
    if (assets->buffer) {
        SDL_FreeSurface(assets->buffer);
    }
    if (assets->spritesheet) {
        SDL_FreeSurface(assets->spritesheet);
    }
    if (assets->player) {
        SDL_FreeSurface(assets->player);
    }
    if (assets->font) {
        TTF_CloseFont(assets->font);
    }
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
