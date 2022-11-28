//
// Created by Laith on 14/11/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

void dbllist_init(dbllist_t *list) {
    dbllist_head(list) = NULL;
    dbllist_tail(list) = NULL;
    dbllist_size(list) = 0;
}

void dbllist_destroy(dbllist_t *list, dbllist_destroy_t dealloc) {
    if(list==NULL||dbllist_head(list)==NULL) return;
    dbllist_node_t *curr = dbllist_head(list);
    dbllist_node_t *temp;
    while (curr != NULL) {
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(curr));
        temp = dbllist_next(curr);
        free(curr);
        curr = temp;
    }

    list = NULL;
}

int dbllist_append(dbllist_t *list, void *data) {
    dbllist_node_t *newNode = calloc(1,sizeof(dbllist_node_t));
    if (newNode == NULL)
        return -1;
    dbllist_next(newNode) = NULL;
    dbllist_data(newNode) = data;
    if(dbllist_head(list)==NULL){
        dbllist_head(list)=newNode;
        dbllist_tail(list)=newNode;
        dbllist_size(list)++;
    }else{
        dbllist_next(dbllist_tail(list)) = newNode;
        dbllist_prev(newNode) = dbllist_tail(list);
        dbllist_tail(list) = newNode;
    }

    return 0;
}



int dbllist_prepend(dbllist_t *list, void *data) {
    dbllist_node_t *newNode = calloc(1,sizeof(dbllist_node_t));
    if (newNode == NULL)
        return -1;
    dbllist_prev(newNode) = NULL;
    dbllist_data(newNode) = data;
    dbllist_prev(dbllist_head(list)) = newNode;
    dbllist_next(newNode) = dbllist_head(list);
    dbllist_head(list) = newNode;
    return 0;
}

int dbllist_remove(dbllist_t *to, dbllist_node_t *pointer, dbllist_destroy_t dealloc) {

    if (pointer == NULL)
        return -1;

    if(pointer==dbllist_head(to)){
        dbllist_head(to)=dbllist_next(pointer);
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(pointer));
        free(pointer);
        pointer=NULL;
    }
    else if (dbllist_next(pointer) == NULL && dbllist_prev(pointer) == NULL) {
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(pointer));
        free(pointer);
        pointer=NULL;
    } else if (dbllist_next(pointer) == NULL && dbllist_prev(pointer) != NULL) {
        dbllist_tail(to) = dbllist_prev(pointer);
        dbllist_next(dbllist_prev(pointer)) = NULL;
        dbllist_prev(pointer) = NULL;
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(pointer));
        free(pointer);
        pointer=NULL;

    } else if (dbllist_next(pointer) != NULL && dbllist_prev(pointer) == NULL) {
        dbllist_head(to) = dbllist_next(pointer);
        dbllist_prev(dbllist_next(pointer)) = NULL;
        dbllist_next(pointer) = NULL;
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(pointer));
        free(pointer);
        pointer=NULL;
    } else {
        dbllist_node_t *prev = dbllist_prev(pointer);
        dbllist_node_t *next = dbllist_next(pointer);
        dbllist_next(prev) = dbllist_next(pointer);
        dbllist_prev(next) = dbllist_prev(pointer);
        if (dealloc == DBLLIST_FREE_DATA)
            free(dbllist_data(pointer));
        free(pointer);
        pointer = NULL;
    }
    dbllist_size(to)--;
    return 0;
}