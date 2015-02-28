#ifndef MOB_H
#define MOB_H

#include <stdio.h>
#include <stdlib.h>

enum {
    MOB_WANDER,
};

typedef struct {
    int id;
    int state;
    int direction;
} Mob_t;

#endif
