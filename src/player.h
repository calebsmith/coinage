#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "item.h"
#include "floor.h"

#define PLAYER_MAX_ITEM_SIZE 20

enum {
    PLAYER_UP,
    PLAYER_RIGHT,
    PLAYER_DOWN,
    PLAYER_LEFT,
};

typedef struct {
    int x, y;
    int direction;
    Item_t items[PLAYER_MAX_ITEM_SIZE];
    int item_size;
} Player_t;

void player_move(Floor_t * floor, Player_t * player, int PLAYER_DIRECTION);
bool player_check_get_item(Floor_t * floor, Player_t * player);
#endif
