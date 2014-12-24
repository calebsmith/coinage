#ifndef INIT_H
#define INIT_H
#include <stdio.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_timer.h"
#include "SDL/SDL_mixer.h"

#include "floor.h"

#define SCRWIDTH 1024
#define SCRHEIGHT 768
#define SCRBPP 32

typedef struct {
    SDL_Surface *buffer;
    SDL_Surface *spritesheet;
    SDL_Surface *items;
    SDL_Surface *mobs;
    SDL_Surface *player;
    Mix_Chunk *grunt_sound;
    Mix_Chunk *coin_sound;
    Mix_Chunk *win_sound;
    Mix_Chunk *death_sound;
    TTF_Font *font;
} Asset_t;

//initialize SDL and game window
bool init_ui(Asset_t * assets);
SDL_Surface *load_image(const char *filename);
void quit_ui(Asset_t * assets);

#endif
