#include "ui.h"

int game_loop(Asset_t * assets, Floor_t * floor, Player_t * player, unsigned int level_number);

int main()
{
    Asset_t assets;
    Floor_t floor;
    Player_t player;
    int result;

    if (!init_ui(&assets)) {
        quit_ui(&assets);
        printf("Failed to initialize a window or load required assets\n");
        return 1;
    }
    if ((result = game_loop(&assets, &floor, &player, 3)) != 0) {
        printf("Error code %d during game\n", result);
        return result;
    }
    floor_destroy(&floor);
    quit_ui(&assets);
    return 0;
}

int game_loop(Asset_t * assets, Floor_t * floor, Player_t * player, unsigned int level_number)
{
    int result;

    if (!player_load_level(floor, player, level_number)) {
        printf("Failed to load map file\n");
        return 1;
    }

    result = play_loop(assets, floor, player);
    if (result == 0) {
        // 0 means exit game without error
        return 0;
    } else {
        // otherwise load next level
        game_loop(assets, floor, player, result);
    }
    return 0;
}
