#include "timer.h"


void timer_init(Timer_t * timer)
{
    timer_reset(timer);
    timer->run = true;
}

void timer_reset(Timer_t * timer)
{
    timer->start = SDL_GetTicks();
    timer->now = timer->start;
}

void timer_pause(Timer_t * timer)
{
    timer->run = false;
}

void timer_play(Timer_t * timer)
{
    timer->run = true;
}

int timer_get_now(Timer_t * timer)
{
    if (timer->run) {
        timer->now = SDL_GetTicks();
    }
    return timer->now;
}

int timer_diff(Timer_t * timer)
{
    return timer_get_now(timer) - timer->start;
}

bool timer_tick(Timer_t * timer, int target)
{
    int diff = timer_diff(timer);

    if (diff >= target) {
        timer_reset(timer);
        return true;
    }
    return false;

}

void timer_wait_fps(Timer_t * timer)
{
    int diff = timer_diff(timer);

    if (diff < FPS_WAIT){
        SDL_Delay(FPS_WAIT - diff);
    }
    timer_reset(timer);
}
