#include "box.h"

void point_display(Point_t point)
{
    printf("{%d, %d}", point.x, point.y);
}

void box_display(Box_t box)
{
    printf("{%d, %d} - {%d, %d}", box_get_w(box), box_get_n(box),
           box_get_e(box), box_get_s(box));
}

bool box_has_point(Box_t box, Point_t point)
{
    int x = point.x;
    int y = point.y;

    if ((x >= box_get_w(box) && x <= box_get_e(box)) &&
        (y >= box_get_n(box) && y <= box_get_s(box))) {
        return true;
    }
    return false;
}

bool box_intersects_box(Box_t self, Box_t other)
{
    if ((box_get_s(other) < box_get_n(self)) ||
        (box_get_n(other) > box_get_s(self)) ||
        (box_get_e(other) < box_get_w(self)) ||
        (box_get_w(other) > box_get_e(self))) {
        return false;
    }
    return true;
}
