#include "floor.h"
#include "player.h"

void player_move(Floor_t * floor, Player_t * player, int PLAYER_DIRECTION)
{
    Tile_t target_tile;

    switch (PLAYER_DIRECTION) {
    case PLAYER_UP:
        if (player->y > 0) {
            target_tile = floor_get_tile(floor, player->x, player->y - 1);
            if (!target_tile.solid_down) {
                player->y = player->y - 1;
            }
        }
        break;
    case PLAYER_DOWN:
        if (player->y < floor->height - 1) {
            target_tile = floor_get_tile(floor, player->x, player->y + 1);
            if (!target_tile.solid_up) {
                player->y = player->y + 1;
            }
        }
        break;
    case PLAYER_LEFT:
        if (player->x > 0) {
            target_tile = floor_get_tile(floor, player->x - 1, player->y);
            if (!target_tile.solid_right) {
                player->x = player->x - 1;
            }
        }
        break;
    case PLAYER_RIGHT:
        if (player->x < floor->width - 1) {
            target_tile = floor_get_tile(floor, player->x + 1, player->y);
            if (!target_tile.solid_left) {
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
