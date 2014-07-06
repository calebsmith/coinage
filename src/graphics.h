#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

#include "floor.h"
#include "item.h"
#include "player.h"
#include "timer.h"
#include "ui.h"


void render_text(Asset_t * assets, int x, int y, char * message);
void render_tile(Asset_t * assets, int tile, int x, int y);
void render_board(Asset_t * assets, Floor_t * floor, Player_t * player);
void render_player(Asset_t * assets, Floor_t * floor, Player_t * player);
void render_items(Asset_t * assets, Floor_t * floor, Player_t * player);
void render_inventory(Asset_t * assets, Player_t * player);
void render_stats(Asset_t * assets, Floor_t * floor, Player_t * player);
void render(Asset_t * assets, Floor_t * floor, Player_t * player);

#endif
