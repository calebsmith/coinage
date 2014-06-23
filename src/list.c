#include <string.h>

#include "list.h"

void list_init(LList_t * list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

void list_destroy(LList_t * list)
{
    void * data;

    // Destroy all data contained inside of list
    while(list_size(list) > 0) {
        if (list_remove_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }
    // zero out the data in list
    memset(list, 0, sizeof(LList_t));
}

/*
 * Given a linked list `list`, a list node `node`, and a void * `data` create a
 * new new node in the list containing that data after the given node.
 * If the node is NULL, insert the node at the head of the list
 */
int list_insert_next(LList_t * list, LListNode_t * node, const void *data)
{
    LListNode_t *new_node;

    if ((new_node = (LListNode_t *) malloc(sizeof(LListNode_t))) == NULL) {
        return -1;
    }
    new_node->data = (void *) data;
    // Insertion at head
    if (node == NULL) {
        // For an empty list, set the tail to match the new head
        if (list_size(list) == 0) {
            list->tail = new_node;
        }
        new_node->next = list->head;
        list->head = new_node;
    // Insertion after head
    } else {
        // Insertion at tail
        if (node->next == NULL) {
            list->tail = new_node;
        }
        new_node->next = node->next;
        node->next = new_node;
    }
    list->size++;
    return 0;
}


int list_remove_next(LList_t * list, LListNode_t * node, void **data)
{
    LListNode_t * old_node;

    if (list_size(list) == 0) {
        return -1;
    }
    // If removing the head
    if (node == NULL) {
        *data = list->head->data;
        old_node = list->head;
        list->head = list->head->next;
        // if the head is tail, set it to NULL
        if (list_size(list) == 1) {
            list->tail = NULL;
        }
    } else {
        // If next is NULL it can't be removed
        if (node->next == NULL) {
            return -1;
        }
        *data = node->next->data;
        old_node = node->next;
        node->next = node->next->next;
        if (node->next == NULL) {
            list->tail = node;
        }
    }
    free(old_node);
    list->size--;
    return 0;
}

int list_append(LList_t * list, const void *data)
{
    return list_insert_next(list, list_tail(list), data);
}


void * list_stream_get(Stream_t * stream)
{
    void * result;
    if (stream->node != NULL) {
        result = stream->node->data;
        stream->node = stream->node->next;
        return result;
    } else {
        return NULL;
    }
}

void list_stream_init(Stream_t * stream, LList_t * list)
{
    stream->list = list;
    stream->node = list->head;
}

bool list_stream_is_empty(Stream_t * stream)
{
    return (stream->node == NULL) ? true: false;
}
