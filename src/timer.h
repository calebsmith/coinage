#ifndef TIMER_H
#define TIMER_H
#include <stdbool.h>

#include "SDL/SDL_timer.h"

#define FPS 60
#define FPS_WAIT 1000 / FPS

typedef struct {
    int start;
    int now;
    bool run;
} Timer_t;

void timer_init(Timer_t * timer);
void timer_reset(Timer_t * timer);
void timer_pause(Timer_t * timer);
void timer_play(Timer_t * timer);
void timer_wait_fps(Timer_t * timer);
bool timer_tick(Timer_t* timer, int target);
int timer_get_now(Timer_t * timer);
int timer_diff(Timer_t * timer);

#endif
