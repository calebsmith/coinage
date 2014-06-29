#include "floor.h"
#include "ui.h"

int main()
{
    Asset_t assets;
    Floor_t floor;
    Player_t player;

    if (!init_ui(&assets)) {
        quit_ui(&assets);
        return 1;
    }
    if (!player_load_level(&floor, &player, "data/maps/floor1.dat")) {
        return 1;
    }
    render_loop(&assets, &floor, &player);
    quit_ui(&assets);
    floor_destroy(&floor);
    return 0;
}
