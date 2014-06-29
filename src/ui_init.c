#include "ui_init.h"

// Initialize SDL and game window
bool init_ui(Asset_t * assets)
{
    // initialize SDL, the window, and fonting
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableKeyRepeat(0, 0);
    SDL_WM_SetCaption("Coinage", NULL);
    TTF_Init();
    assets->font = NULL;
    // initialize the video buffer in the window
    assets->buffer = SDL_SetVideoMode(SCRWIDTH, SCRHEIGHT, SCRBPP, SDL_HWSURFACE);      // | SDL_FULLSCREEN);
    if (!assets->buffer) {
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
    TTF_Quit();
    SDL_Quit();
}
