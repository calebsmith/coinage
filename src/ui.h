#ifndef UI_H
#define UI_H
#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

#include "floor.h"
#include "item.h"
#include "player.h"
#include "ui_init.h"

#define TILEW 64
#define TILEH 64

#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 32

#define TILE_DISPLAY_WIDTH 10
#define TILE_DISPLAY_HEIGHT 10
#define TILE_DISPLAY_MID_X (TILE_DISPLAY_WIDTH / 2)
#define TILE_DISPLAY_MID_Y (TILE_DISPLAY_HEIGHT / 2)

void render_tile(Asset_t * assets, Tile_t tile, int x, int y);
void render_board(Asset_t * assets, Floor_t * floor, Player_t * player);
void render_player(Asset_t * assets, Floor_t * floor, Player_t * player);
void render_items(Asset_t * assets, Floor_t * floor, Player_t * player);
void render(Asset_t * assets, Floor_t * floor, Player_t * player);

void logic(Floor_t * floor, Player_t * player);

bool get_input(SDL_Event event, Floor_t * floor, Player_t * player);

int render_loop();

#endif
