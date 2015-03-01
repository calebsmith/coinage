#ifndef MOB_H
#define MOB_H

#include <stdio.h>
#include <stdlib.h>

enum {
    MOB_FOLLOW,
    MOB_SENTRY,
    MOB_STATIONARY,
    MOB_MAXAI,
};

typedef struct {
    int id;
    int ai;
    int direction;
} Mob_t;

#endif
