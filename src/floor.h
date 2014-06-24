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

#define TILE_DISPLAY_WIDTH 10
#define TILE_DISPLAY_HEIGHT 10
#define TILE_DISPLAY_MID_X (TILE_DISPLAY_WIDTH / 2)
#define TILE_DISPLAY_MID_Y (TILE_DISPLAY_HEIGHT / 2)

#define TILEFLAG_SOLID 1
#define TILEFLAG_PUSHABLE 2

typedef struct {
    int width, height;
    int *tiles;
    QTree_t items;
} Floor_t;

void floor_init(Floor_t * floor);
void floor_display(const Floor_t * floor);
int floor_get_tile(const Floor_t * floor, int x, int y);
void floor_set_tile(Floor_t * floor, int x, int y, int tile);
int floor_get_x_offset(int x, int floor_width);
int floor_get_y_offset(int y, int floor_height);
Stream_t floor_get_item_stream(Floor_t * floor, Box_t query);
void floor_destroy(Floor_t * floor);

#endif
