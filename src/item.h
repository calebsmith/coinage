#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>

enum {
    ITEM_COIN,
    ITEM_SWORD,
    ITEM_SKATE,
    ITEM_BOOT,
};

#define MAX_ITEM ITEM_BOOT + 1


static const int ITEM_FLAG_LOOKUP [] = {
    1 << ITEM_COIN,
    1 << ITEM_SWORD,
    1 << ITEM_SKATE,
    1 << ITEM_BOOT,
};

#define item_flag_lookup(item) (ITEM_FLAG_LOOKUP[(item)])

#endif
