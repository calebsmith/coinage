#include "ui.h"

static int STATS_DISPLAY_X = (TILE_DISPLAY_WIDTH * TILEW) + BOARD_OFFSET_X;


void render_text(Asset_t * assets, int x, int y, char * message)
{
  SDL_Color color = {0xff, 0x99, 0x00, 0xff};
  SDL_Rect position = {x, y, 0, 0 };
  SDL_Surface *text;

  if (assets->font) {
      text = TTF_RenderText_Solid(assets->font, message, color);
      SDL_BlitSurface (text, NULL, assets->buffer, &position);
  }
}

void render_tile(Asset_t * assets, int tile, int x, int y)
{
    SDL_Rect sprite_offset, position;

    sprite_offset.x = 0;
    sprite_offset.y = tile * TILEH;
    sprite_offset.w = TILEW;
    sprite_offset.h = TILEH;
    position.x = (x * TILEW) + BOARD_OFFSET_X;
    position.y = (y * TILEH) + BOARD_OFFSET_Y;
    if (assets->spritesheet != NULL) {
        SDL_BlitSurface(assets->spritesheet, &sprite_offset, assets->buffer,
                        &position);
    }
}

void render_item_on_map(Asset_t * assets, Item_t item, int x, int y)
{
    SDL_Rect sprite_offset, position;

    sprite_offset.x = 0;
    sprite_offset.y = item.id * TILEH;
    sprite_offset.w = TILEW;
    sprite_offset.h = TILEH;
    position.x = (x * TILEW) + BOARD_OFFSET_X;
    position.y = (y * TILEH) + BOARD_OFFSET_Y;
    if (assets->items != NULL) {
        SDL_BlitSurface(assets->items, &sprite_offset, assets->buffer,
                        &position);
    }
}

void render_items(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    Stream_t stream;
    Box_t query;
    TaggedData_t point_item;
    Item_t item;
    Point_t point;
    int x_offset, y_offset;
    int map_x, map_y;

    x_offset = floor_get_x_offset(player->x, floor->width);
    y_offset = floor_get_y_offset(player->y, floor->height);
    query = (Box_t) {
        (Point_t) {x_offset * -1, y_offset * -1},
        TILE_DISPLAY_WIDTH - 1, TILE_DISPLAY_HEIGHT - 1
    };
    stream = floor_get_item_stream(floor, query);
    while(!list_stream_is_empty(&stream)) {
        point_item = **((TaggedData_t **) list_stream_get(&stream));
        item = *(Item_t *) point_item.data;
        point = (Point_t) point_item.point;
        map_x = point.x + x_offset;
        map_y = point.y + y_offset;
        if ((map_x >= 0 && map_x < TILE_DISPLAY_WIDTH) &&
            (map_y >= 0 && map_y < TILE_DISPLAY_HEIGHT)) {
            render_item_on_map(assets, item, map_x, map_y);
        }
    }
    list_destroy(stream.list);
}


void render_board(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int x, y;
    int x_offset, y_offset;
    int map_x, map_y;
    int tile;

    x_offset = floor_get_x_offset(player->x, floor->width);
    y_offset = floor_get_y_offset(player->y, floor->height);
    for (y = 0; y < TILE_DISPLAY_HEIGHT; y++) {
        for (x = 0; x < TILE_DISPLAY_WIDTH; x++) {
            map_x = x - x_offset;
            map_y = y - y_offset;
            if ((map_x >= 0 && map_x < floor->width) &&
                (map_y >= 0 && map_y < floor->height)) {
                tile = floor_get_tile(floor, map_x, map_y);
                render_tile(assets, tile, x, y);
            }
        }
    }
}

void render_player(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    int display_x, display_y;
    SDL_Rect sprite_offset, position;

    sprite_offset.y = 0;
    sprite_offset.x = player->direction * TILEW;
    sprite_offset.w = TILEW;
    sprite_offset.h = TILEH;
    display_x = player->x + floor_get_x_offset(player->x, floor->width);
    display_y = player->y + floor_get_y_offset(player->y, floor->height);
    position.x = (display_x * TILEW) + BOARD_OFFSET_X;
    position.y = (display_y * TILEH) + BOARD_OFFSET_Y;
    if (assets->player != NULL) {
        SDL_BlitSurface(assets->player, &sprite_offset, assets->buffer, &position);
    }
}

void render_inventory(Asset_t * assets, Player_t * player)
{
    SDL_Rect sprite_offset, position;

    Item_t item;
    int i;

    sprite_offset.x = 0;
    sprite_offset.w = TILEW;
    sprite_offset.h = TILEH;
    for (i = 0; i < player->item_size; i++) {
        item = player->items[i];
        position.x = ((TILE_DISPLAY_WIDTH + i) * TILEW) + BOARD_OFFSET_X;
        position.y = ((TILE_DISPLAY_HEIGHT - 1) * TILEH) + BOARD_OFFSET_Y;

        sprite_offset.y = item.id * TILEH;
        if (assets->items != NULL) {
            SDL_BlitSurface(assets->items, &sprite_offset, assets->buffer, &position);
        }
    }
}

void render(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    char buffer[50] = "";

    SDL_FillRect(assets->buffer, NULL, 0x000000);
    render_board(assets, floor, player);
    render_items(assets, floor, player);
    render_player(assets, floor, player);
    render_inventory(assets, player);
    snprintf(buffer, sizeof(buffer), "Coins: %d", player->coins);
    render_text(assets, STATS_DISPLAY_X + 15, 15, buffer);
    SDL_Flip(assets->buffer);
    return;
}

bool get_input(SDL_Event event, Floor_t * floor, Player_t * player)
{
    int tile_x, tile_y;

    if (SDL_PollEvent(&event) == 0) {
        // No event, carry on
        return true;
    }
    if (event.type == SDL_QUIT ||
        (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        // Handle quitting
        return false;
    }
    // Handle arrow keys
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            player_move(floor, player, PLAYER_UP);
            break;
        case SDLK_DOWN:
            player_move(floor, player, PLAYER_DOWN);
            break;
        case SDLK_LEFT:
            player_move(floor, player, PLAYER_LEFT);
            break;
        case SDLK_RIGHT:
            player_move(floor, player, PLAYER_RIGHT);
            break;
        default:
            break;
        }
    }
    // Handle mouse
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
            tile_x = (event.motion.x - BOARD_OFFSET_X) / TILEW;
            tile_y = (event.motion.y - BOARD_OFFSET_Y) / TILEH;
            if (tile_x >= 0 && tile_x < TILE_DISPLAY_WIDTH &&
                tile_y >= 0 && tile_y < TILE_DISPLAY_HEIGHT) {
                printf("clicked %d %d\n", tile_x, tile_y);
            }
        }
    }
    return true;
}

void logic(Floor_t * floor, Player_t * player)
{
    player_check_get_item(floor, player);
}

void render_loop(Asset_t * assets, Floor_t * floor, Player_t * player)
{
    SDL_Event event;

    while (get_input(event, floor, player)) {
        logic(floor, player);
        render(assets, floor, player);
    }
}


