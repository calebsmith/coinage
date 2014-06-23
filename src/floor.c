#include "floor.h"

void floor_init(Floor_t * floor)
{
    int size;
    int i;
    unsigned int tile_id;

    floor->width = 19;
    floor->height = 20;
    size = floor->width * floor->height;
    floor->tiles = malloc(size * sizeof(Tile_t));
    for (i = 0; i < size; i++) {
        tile_id = 0;
        if (i == 3 * 19 + 4) {
            tile_id = 1;
        }
        if (i == 5 * 19 + 8) {
            tile_id = 1;
        }
        if (i == 6 * 19 + 12) {
            tile_id = 1;
        }
        if (i == 9 * 19 + 3) {
            tile_id = 1;
        }
        if (i == 12 * 19 + 6) {
            tile_id = 1;
        }
        if (i == 18 * 19 + 1) {
            tile_id = 1;
        }
        if (i == 18) {
            tile_id = 1;
        }
        floor->tiles[i].id = tile_id;
        if (tile_id == 1) {
            floor->tiles[i].solid_up = true;
            floor->tiles[i].solid_down = true;
            floor->tiles[i].solid_left = true;
            floor->tiles[i].solid_right = true;
        }
    }
    floor->items = qtree_init((Box_t) {(Point_t) {0, 0}, floor->width, floor->height});
}

/*
 * Given a Floor_t* and x,y coordinates, returns the Tile_t value of the tile
 * at that location.
 *
 * Parameters: const Floor_t* floor, int x, int y
 * Side-Effects: None
 * Returns: int value of the cell (0 or 1)
 */
Tile_t floor_get_tile(const Floor_t * floor, int x, int y)
{
    int index;

    assert(x >= 0 && x < floor->width);
    assert(y >= 0 && y < floor->height);
    index = y * floor->width + x;
    return floor->tiles[index];
}

void floor_set_tile(Floor_t * floor, int x, int y, Tile_t tile)
{
    int index;

    assert(x >= 0 && x < floor->width);
    assert(y >= 0 && y < floor->width);
    index = y * floor->width + x;
    floor->tiles[index] = tile;
}

/*
 * Given a Floor_t pointer, free the memory allocated for that floor.
 *
 * Parameters: Floor_t* floor
 * Side-Effects: frees memory for the given floor.
 */
void floor_destroy(Floor_t * floor)
{
    free(floor->tiles);
}

void floor_display(const Floor_t * floor)
{
    Tile_t tile;
    int x, y;

    printf("-------\n");
    for (y = 0; y < floor->height; y++) {
        for (x = 0; x < floor->width; x++) {
            tile = floor_get_tile(floor, x, y);
            printf("%d|", tile.id);
        }
        printf("|\n");
        printf("-------\n");
    }
}

int floor_get_x_offset(int x, int floor_width)
{
    if (x < TILE_DISPLAY_MID_X) {
        return 0;
    } else if (x < floor_width - TILE_DISPLAY_MID_X) {
        return TILE_DISPLAY_MID_X - x;
    } else {
        return TILE_DISPLAY_WIDTH - floor_width;
    }
}

int floor_get_y_offset(int y, int floor_height)
{
    if (y < TILE_DISPLAY_MID_Y) {
        return 0;
    } else if (y < floor_height - TILE_DISPLAY_MID_Y) {
        return TILE_DISPLAY_MID_Y - y;
    } else {
        return TILE_DISPLAY_HEIGHT - floor_height;
    }
}

Stream_t floor_get_item_stream(Floor_t * floor, Box_t query)
{
    LList_t list;
    Stream_t stream;

    list_init(&list, NULL);
    list_qtree_query_range(&(floor->items), query, &list);
    list_stream_init(&stream, &list);
    return stream;
}
