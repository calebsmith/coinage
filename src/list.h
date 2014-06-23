#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct LListNode_t_ {
    void *data;
    struct LListNode_t_ *next;
} LListNode_t;

typedef struct LList_t_ {
    int size;
    // Pointers to functions for comparison and freeing
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    // Head/Tail nodes
    LListNode_t * head;
    LListNode_t * tail;
} LList_t;

typedef struct {
    LList_t * list;
    LListNode_t * node;
} Stream_t;

void list_init(LList_t *list, void (*destroy)(void *data));
void list_destroy(LList_t * list);
int list_insert_next(LList_t * list, LListNode_t * node, const void *data);
int list_remove_next(LList_t* list, LListNode_t * node, void **data);
int list_append(LList_t* list, const void *data);
void * list_get_next(LList_t* list, LListNode_t * node);

void * list_stream_get(Stream_t * stream);
void list_stream_init(Stream_t * stream, LList_t * list);
bool list_stream_is_empty(Stream_t * stream);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? true: false)
#define list_is_tail(list, element) ((element) == (list)->tail ? true: false)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif
