#include "floor.h"
#include "player.h"

void player_move(Floor_t * floor, Player_t * player, int PLAYER_DIRECTION)
{
    int target_tile;

    switch (PLAYER_DIRECTION) {
    case PLAYER_UP:
        if (player->y > 0) {
            player->direction = PLAYER_UP;
            target_tile = floor_get_tile(floor, player->x, player->y - 1);
            if (target_tile == 0) {
                player->y = player->y - 1;
            } else if(player->y > 1 && target_tile == 2 &&
                floor_get_tile(floor, player->x, player->y - 2) == 0) {
                floor_set_tile(floor, player->x, player->y - 2, 2);
                floor_set_tile(floor, player->x, player->y - 1, 0);
                player->y = player->y - 1;
            }
        }
        break;
    case PLAYER_DOWN:
        if (player->y < floor->height - 1) {
            player->direction = PLAYER_DOWN;
            target_tile = floor_get_tile(floor, player->x, player->y + 1);
            if (target_tile == 0) {
                player->y = player->y + 1;
            } else if(player->y < floor->height - 2 && target_tile == 2 &&
                floor_get_tile(floor, player->x, player->y + 2) == 0) {
                floor_set_tile(floor, player->x, player->y + 2, 2);
                floor_set_tile(floor, player->x, player->y + 1, 0);
                player->y = player->y + 1;
            }
        }
        break;
    case PLAYER_LEFT:
        if (player->x > 0) {
            player->direction = PLAYER_LEFT;
            target_tile = floor_get_tile(floor, player->x - 1, player->y);
            if (target_tile == 0) {
                player->x = player->x - 1;
            } else if(player->x > 1 && target_tile == 2 &&
                floor_get_tile(floor, player->x - 2, player->y) == 0) {
                floor_set_tile(floor, player->x - 2, player->y, 2);
                floor_set_tile(floor, player->x - 1, player->y, 0);
                player->x = player->x - 1;
            }
        }
        break;
    case PLAYER_RIGHT:
        if (player->x < floor->width - 1) {
            player->direction = PLAYER_RIGHT;
            target_tile = floor_get_tile(floor, player->x + 1, player->y);
            if (target_tile == 0) {
                player->x = player->x + 1;
            } else if(player->x < floor->width - 2 && target_tile == 2 &&
                floor_get_tile(floor, player->x + 2, player->y) == 0) {
                floor_set_tile(floor, player->x + 2, player->y, 2);
                floor_set_tile(floor, player->x + 1, player->y, 0);
                player->x = player->x + 1;
            }
        }
        break;
    default:
        break;
    }
}

bool player_check_get_item(Floor_t * floor, Player_t * player)
{
    Point_t position;
    Item_t * item;

    if (player->item_size < PLAYER_MAX_ITEM_SIZE) {
        position = (Point_t) {player->x, player->y};
        if (qtree_pop(&(floor->items), position, (void **) &item)) {
            player->items[player->item_size] = *item;
            player->item_size++;
            return true;
        }
    }
    return false;
}
