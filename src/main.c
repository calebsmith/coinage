#include "ui.h"

int main()
{
    Asset_t assets;
    Floor_t floor;
    Player_t player;

    if (!init_ui(&assets)) {
        quit_ui(&assets);
        printf("Failed to initialize a window or load required assets\n");
        return 1;
    }
    if (!player_load_level(&floor, &player, "data/maps/floor1.dat")) {
        printf("Failed to load map file\n");
        return 1;
    }
    render_loop(&assets, &floor, &player);
    floor_destroy(&floor);
    quit_ui(&assets);
    return 0;
}
