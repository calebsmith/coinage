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
#include "timer.h"
#include "ui_init.h"
#include "graphics.h"

#define TILEW 64
#define TILEH 64

#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 32

#define TICK_FREQ 500

#define MAX_LEVEL 7

int check_level_complete(Floor_t * floor, Player_t * player);
int logic(Timer_t * tick_timer, Asset_t * assets, Floor_t * floor, Player_t * player);
int get_input(SDL_Event event, Asset_t * assets, Floor_t * floor, Player_t * player);
int play_loop(Asset_t * assets, Floor_t * floor, Player_t * player);

#endif
