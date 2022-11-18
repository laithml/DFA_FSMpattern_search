//
// Created by Laith on 14/11/2022.
//
#include <stdlib.h>
#include "dblist.h"

void dblist_init(dblist_t *list) {
    dblist_head(list) = NULL;
    dblist_tail(list) = NULL;
    dblist_size(list) = 0;
}

void dblist_destroy(dblist_t *list, dblist_destroy_t dealloc) {
    dblist_node_t *curr = dblist_head(list);
    dblist_node_t *temp;
    while (dblist_next(curr) != NULL) {
        if (dealloc == dblist_FREE_DATA)
            free(dblist_data(curr));
        temp = dblist_next(curr);
        free(curr);
        curr = temp;
    }
    list = NULL;
}

int dblist_append(dblist_t *list, void *data) {
    dblist_node_t *newNode = malloc(sizeof(dblist_node_t));
    if (newNode == NULL)
        return -1;
    dblist_next(newNode) = NULL;
    dblist_data(newNode) = data;
    if(dblist_head(list)==NULL){
        dblist_head(list)=newNode;
        dblist_tail(list)=newNode;
        dblist_size(list)++;
    }else{
        dblist_next(dblist_tail(list)) = newNode;
        dblist_prev(newNode) = dblist_tail(list);
        dblist_tail(list) = newNode;
    }

    return 0;
}

int dblist_prepend(dblist_t *list, void *data) {
    dblist_node_t *newNode = malloc(sizeof(dblist_node_t));
    if (newNode == NULL)
        return -1;
    dblist_prev(newNode) = NULL;
    dblist_data(newNode) = data;
    dblist_prev(dblist_head(list)) = newNode;
    dblist_next(newNode) = dblist_head(list);
    dblist_head(list) = newNode;
    return 0;
}

int dblist_remove(dblist_t *to, dblist_node_t *pointer, dblist_destroy_t dealloc) {

    if (pointer == NULL)
        return -1;

    if (dblist_next(pointer) == NULL && dblist_prev(pointer) == NULL) {
        if (dealloc == dblist_FREE_DATA)
            free(dblist_data(pointer));
        free(pointer);
    } else if (dblist_next(pointer) == NULL && dblist_prev(pointer) != NULL) {
        dblist_tail(to) = dblist_prev(pointer);
        dblist_next(dblist_prev(pointer)) = NULL;
        dblist_prev(pointer) = NULL;
        if (dealloc == dblist_FREE_DATA)
            free(dblist_data(pointer));
        free(pointer);
    } else if (dblist_next(pointer) != NULL && dblist_prev(pointer) == NULL) {
        dblist_head(to) = dblist_next(pointer);
        dblist_prev(dblist_next(pointer)) = NULL;
        dblist_next(pointer) = NULL;
        if (dealloc == dblist_FREE_DATA)
            free(dblist_data(pointer));
        free(pointer);
    } else {
        dblist_node_t *prev = dblist_prev(pointer);
        dblist_node_t *next = dblist_next(pointer);
        dblist_next(prev) = dblist_next(pointer);
        dblist_prev(next) = dblist_prev(pointer);
        if (dealloc == dblist_FREE_DATA)
            free(dblist_data(pointer));
        free(pointer);
        pointer = NULL;
    }
    dblist_size(to)--;
    return 0;
}