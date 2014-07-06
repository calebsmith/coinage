#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#include "item.h"
#include "floor.h"
#include "ui_init.h"

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
    int coins;
    int time;
} Player_t;

bool player_load_level(Floor_t * floor, Player_t * player, unsigned int level_number);
bool player_move(Floor_t * floor, Player_t * player, int PLAYER_DIRECTION);
bool player_check_get_item(Asset_t * assets, Floor_t * floor, Player_t * player);
#endif
