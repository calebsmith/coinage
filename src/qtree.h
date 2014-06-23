#ifndef QTREE_H
#define QTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"
#include "box.h"

#define QTREE_CAPACITY 1

typedef struct {
  Point_t point;
  void * data;
} TaggedData_t;

typedef struct QTree_t QTree_t;

struct QTree_t {
  // Represents the boundaries of this quad tree
  Box_t boundary;
  // Point and data in this quad tree node
  bool vacant; // Marks the qtree as containing data
  TaggedData_t * tagged_data; // Stores a point:data pair
  // Child trees
  QTree_t* nw;
  QTree_t* ne;
  QTree_t* sw;
  QTree_t* se;
};

QTree_t qtree_init(Box_t boundary);
void qtree_destroy(QTree_t * qtree);
void qtree_display(QTree_t qtree);
bool qtree_subdivide(QTree_t * qtree);
bool qtree_insert(QTree_t * qtree, Point_t point, void * data);
bool qtree_peek(QTree_t * qtree, Point_t point, void ** result);
bool qtree_pop(QTree_t * qtree, Point_t point, void ** result);

void list_qtree_query_range(QTree_t * qtree, Box_t range, LList_t *list);

#define qtree_is_leaf(x) ((x)->nw==NULL)

#endif
