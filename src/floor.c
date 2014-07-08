#include "floor.h"

static int MAX_TILE_LINE = MAX_WIDTH * (MAX_TILE + 2) + 1;


void floor_init(Floor_t * floor, FILE* infile)
{
    char current_line[MAX_TILE_LINE];
    int current_tile;
    char *token;
    int line_number;
    int size;
    int x, i;
    int item_id, item_size, item_x, item_y;

    if ((fscanf(infile, "%d", &floor->total_time)) != 1) {
        printf("Bad file format. No map time\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    timer_init(&floor->timer);
    floor->time_left = floor->total_time;
    if ((fscanf(infile, "%d,%d", &floor->width, &floor->height)) != 2) {
        printf("Bad file format. No map width/height\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    if ((floor->width > MAX_WIDTH || floor->width < MIN_WIDTH) ||
        (floor->height > MAX_HEIGHT || floor->height < MIN_HEIGHT)) {
        printf("Invalid width/height data.\n"
            "Must be %d < x < %d and %d < y < %d\n",
            MIN_WIDTH, MAX_WIDTH, MIN_HEIGHT, MAX_HEIGHT
        );
        exit(EXIT_STATUS_BAD_FILE);
    }
    if ((fscanf(infile, "%d,%d", &floor->player_start_x, &floor->player_start_y)) != 2) {
        printf("Bad file format. No player start position\n");
        exit(EXIT_STATUS_BAD_FILE);
    }
    if ((floor->player_start_x >= floor->width || floor->player_start_x < 0) ||
        (floor->player_start_y >= floor->height || floor->player_start_y < 0)) {
        printf("Invalid width/height data.\n"
            "Must be %d < x < %d and %d < y < %d\n",
            MIN_WIDTH, MAX_WIDTH, MIN_HEIGHT, MAX_HEIGHT
        );
        exit(EXIT_STATUS_BAD_FILE);
    }
    size = floor->width * floor->height;
    floor->tiles = calloc(size, size * sizeof(int));
    for (i = 0, line_number = 0; i < size; line_number++) {
        if (fgets(current_line, MAX_TILE_LINE, infile) == NULL) {
            printf("Not enough tile lines in floor file\n");
            exit(EXIT_STATUS_BAD_FILE);
        }
        for (token = strtok(current_line, ","), x = 0; token != NULL && x < floor->width; token = strtok(NULL, ","), x++) {
            if (sscanf(token, "%d", &current_tile) != 1) {
                // Allow first loop to have garbage token, otherwise error
                if (x > 0) {
                    printf("Bad data in floor file. Expected integer, found %s on line %d\n", token, line_number + 1);
                    exit(EXIT_STATUS_BAD_FILE);
                }
            } else {
                floor->tiles[i] = current_tile;
                i++;
            }
        }
    }
    floor->items = qtree_init((Box_t) {(Point_t) {0, 0}, floor->width, floor->height});
    floor->coins = 0;
    if ((fscanf(infile, "%d", &item_size)) != 1) {
        printf("Bad file format. No item length given\n");
        exit(EXIT_STATUS_BAD_FILE);
    } else {
        floor->item_storage = malloc(item_size * sizeof(Item_t));
        for (i = 0; i < item_size; i++) {
            if ((fscanf(infile, "%d:%d,%d", &item_id, &item_x, &item_y)) != 3) {
                printf("Bad file format\n");
                exit(EXIT_STATUS_BAD_FILE);
            } else {
                if (item_id == 0) {
                    floor->coins++;
                }
                floor->item_storage[i] = (Item_t) {item_id};
                qtree_insert(&floor->items, (Point_t) {item_x, item_y}, &floor->item_storage[i]);
            }
        }
    }
}

/*
 * Given a Floor_t* and x,y coordinates, returns the int value of the tile
 * at that location.
 *
 * Parameters: const Floor_t* floor, int x, int y
 * Side-Effects: None
 * Returns: int value of the cell (0 or 1)
 */
int floor_get_tile(const Floor_t * floor, int x, int y)
{
    int index;

    assert(x >= 0 && x < floor->width);
    assert(y >= 0 && y < floor->height);
    index = y * floor->width + x;
    return floor->tiles[index];
}

void floor_set_tile(Floor_t * floor, int x, int y, int tile)
{
    int index;

    assert(x >= 0 && x < floor->width);
    assert(y >= 0 && y < floor->height);
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
    if (floor->item_storage) {
        free(floor->item_storage);
    }
    qtree_destroy(&floor->items);
}

void floor_display(const Floor_t * floor)
{
    int tile;
    int x, y;

    printf("-------\n");
    for (y = 0; y < floor->height; y++) {
        for (x = 0; x < floor->width; x++) {
            tile = floor_get_tile(floor, x, y);
            printf("%d|", tile);
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

void floor_increment_time(Floor_t * floor)
{
    int diff;
    int time_left;

    if (floor->total_time > 0) {
        diff = timer_diff(&floor->timer);
        time_left = floor->total_time - (diff / 1000);
        floor->time_left = (time_left >= -1) ? time_left: -1;
    }
}
