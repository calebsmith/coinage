/*
 * The "floor" constitutes a grid of "tiles" each with a given tile type.
 *
 * floor_init() should be called to initialize a floor
 * floor_destroy() *MUST* be called when any floor is no longer needed.
 */
#ifndef FLOOR_H
#define FLOOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "qtree.h"
#include "item.h"
#include "timer.h"

#define TILE_DISPLAY_WIDTH 11
#define TILE_DISPLAY_HEIGHT 11
#define TILE_DISPLAY_MID_X (TILE_DISPLAY_WIDTH / 2)
#define TILE_DISPLAY_MID_Y (TILE_DISPLAY_HEIGHT / 2)

enum {
    TILEFLAG_SOLID,
    TILEFLAG_PUSHABLE,
    TILEFLAG_BREAKABLE,
    TILEFLAG_FILLABLE,
    TILEFLAG_IMMOVABLE,
    TILEFLAG_SLIPPERY,
    TILEFLAG_FIRE,
    TILEFLAG_WATER,
    TILEFLAG_ICE,
    TILEFLAG_PORTAL,
};

#define TILEFLAG_SOLID_FLAG 1 << TILEFLAG_SOLID
#define TILEFLAG_PUSHABLE_FLAG 1 << TILEFLAG_PUSHABLE
#define TILEFLAG_BREAKABLE_FLAG 1 << TILEFLAG_BREAKABLE
#define TILEFLAG_FILLABLE_FLAG 1 << TILEFLAG_FILLABLE
#define TILEFLAG_IMMOVABLE_FLAG 1 << TILEFLAG_IMMOVABLE
#define TILEFLAG_SLIPPERY_FLAG 1 << TILEFLAG_SLIPPERY
#define TILEFLAG_FIRE_FLAG 1 << TILEFLAG_FIRE
#define TILEFLAG_WATER_FLAG 1 << TILEFLAG_WATER
#define TILEFLAG_ICE_FLAG 1 << TILEFLAG_ICE
#define TILEFLAG_PORTAL_FLAG 1 << TILEFLAG_PORTAL

#define tile_has_flag(tile, flag) (!! (TILE_FLAG_LOOKUP[(tile)] & (1 << (flag))))

#define EXIT_STATUS_BAD_FILE 1
#define MIN_WIDTH 3
#define MIN_HEIGHT 3
#define MAX_WIDTH 50
#define MAX_HEIGHT 50
#define MAX_TILE 99


// A bit vector that stores the properties of each tile id
static const int TILE_FLAG_LOOKUP [] = {
    0,
    0 | TILEFLAG_SOLID_FLAG,
    0 | TILEFLAG_SOLID_FLAG | TILEFLAG_PUSHABLE_FLAG,
    0 | TILEFLAG_SOLID_FLAG | TILEFLAG_BREAKABLE_FLAG,
    0 | TILEFLAG_FILLABLE_FLAG | TILEFLAG_WATER_FLAG,
    0 | TILEFLAG_IMMOVABLE_FLAG | TILEFLAG_SLIPPERY_FLAG | TILEFLAG_ICE_FLAG,
    0 | TILEFLAG_FIRE_FLAG,
    0 | TILEFLAG_PORTAL_FLAG,
};

typedef struct {
    unsigned int level_number;
    int width, height;
    int total_time;
    int time_left;
    int player_start_x, player_start_y;
    int *tiles;
    int coins;
    Timer_t timer;
    int *item_storage;
    QTree_t items;
} Floor_t;

void floor_init(Floor_t * floor, FILE* infile);
void floor_display(const Floor_t * floor);
int floor_get_tile(const Floor_t * floor, int x, int y);
void floor_set_tile(Floor_t * floor, int x, int y, int tile);
int floor_get_x_offset(int x, int floor_width);
int floor_get_y_offset(int y, int floor_height);
Stream_t floor_get_item_stream(Floor_t * floor, Box_t query);
void floor_increment_time(Floor_t * floor);
void floor_destroy(Floor_t * floor);

#endif
