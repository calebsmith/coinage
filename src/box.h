#ifndef BOX_H
#define BOX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// Simple coordinate object to represent points and vectors
typedef struct {
    int x;
    int y;
} Point_t;

// Axis-aligned bounding box with half dimension and center
typedef struct {
    Point_t nw;
    int width, height;
} Box_t;

/* Box functions */
void box_display(Box_t box);
void point_display(Point_t point);
int box_get_n(Box_t box);
int box_get_w(Box_t box);
int box_get_e(Box_t box);
int box_get_s(Box_t box);
bool box_has_point(Box_t box, Point_t point);
bool box_intersects_box(Box_t self, Box_t other);

#define box_get_n(box) ((box).nw.y)
#define box_get_w(box) ((box).nw.x)
#define box_get_e(box) ((box).nw.x + (box).width)
#define box_get_s(box) ((box).nw.y + (box).height)
#endif
