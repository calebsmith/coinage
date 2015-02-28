#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdlib.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_ttf.h"

#include "floor.h"
#include "item.h"
#include "mob.h"
#include "player.h"
#include "timer.h"
#include "ui.h"


void render(Asset_t * assets, Floor_t * floor, Player_t * player);

#endif
