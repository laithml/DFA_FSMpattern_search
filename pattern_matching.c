//
// Created by Laith on 15/11/2022.
//

#include "pattern_matching.h"

int pm_init(pm_t *root) {
    pm_state_t *zeroState = malloc(sizeof(pm_state_t));
    if (root == NULL || zeroState == NULL)
        return -1;
    zeroState->id = 0;
    zeroState->depth = 0;
    zeroState->_transitions = NULL;
    zeroState->fail = zeroState;
    zeroState->output = NULL;
    root->zerostate = zeroState;
    root->newstate = 0;
    return 0;
}

int pm_makeFSM(pm_t *root) {
    pm_state_t *zeroState = root->zerostate;
    dblist_node_t *head = dblist_head(zeroState->_transitions);
    dblist_t *queue = malloc(sizeof(dblist_node_t));
    dblist_init(queue);
    while (head != NULL) {
        pm_labeled_edge_t *edge = (pm_labeled_edge_t *) dblist_data(head);
        dblist_append(queue, edge->state);
        edge->state->fail = zeroState;
        head = head->next;
    }


    while (dblist_head(queue) != NULL) {
        pm_state_t *r = (pm_state_t *) dblist_data(dblist_head(queue));//father
        dblist_remove(queue, dblist_head(queue), dblist_LEAVE_DATA);
        dblist_node_t *headT=NULL;
        if (r->_transitions != NULL)
            headT = dblist_head(r->_transitions);//son
            while (headT != NULL) {
                pm_labeled_edge_t *edgeR = ((pm_labeled_edge_t *) dblist_data(headT));
                dblist_append(queue, edgeR->state);
                unsigned char currSymbol = edgeR->label;
                pm_state_t *state = r->fail;
                if(state!=root->zerostate) {
                    while (pm_goto_get(state, currSymbol) == NULL) state = state->fail;
                }
                edgeR->state->fail = pm_goto_get(state, currSymbol);
                printf("Setting f(%d) = %d\n", edgeR->state->id, edgeR->state->fail->id);
                headT = headT->next;
            }


    }
    return 0;
}


int pm_addstring(pm_t *root, unsigned char *string, size_t n) {
    if (root == NULL) {
        return -1;
    }
    pm_state_t *currState = root->zerostate;
    pm_state_t *nextState;
    for (int i = 0; i < n; i++) {
        nextState = pm_goto_get(currState, string[i]);
        if (nextState == NULL) {
            nextState = malloc(sizeof(pm_state_t));
            nextState->id = ++(root->newstate);
            pm_goto_set(currState, string[i], nextState);
        }
        currState = nextState;
    }
    return 0;
}


int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state) {
    if (from_state == NULL || to_state == NULL)
        return -1;
    if (from_state == to_state)
        return 0;

    if (from_state->_transitions == NULL) {
        from_state->_transitions = malloc(sizeof(dblist_t));
        dblist_init(from_state->_transitions);
    }

    pm_int_t newDepth = (from_state->depth) + 1;
    to_state->depth = newDepth;
    pm_labeled_edge_t *newEdge = malloc(sizeof(pm_labeled_edge_t));
    if (newEdge == NULL || to_state == NULL)
        return -1;
    printf("Allocating state %d\n", to_state->id);
    newEdge->state = to_state;
    newEdge->label = symbol;
    dblist_append(from_state->_transitions, newEdge);
    to_state->_transitions = NULL;
    to_state->fail = NULL;
    to_state->output = NULL;
    printf("%d->%c->%d\n", from_state->id, symbol, to_state->id);
    return 0;
}

pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol) {
    if (state == NULL || state->_transitions == NULL)
        return NULL;
    dblist_node_t *curr = dblist_head(state->_transitions);
    while (curr != NULL) {
        if (symbol == ((pm_labeled_edge_t *) dblist_data(curr))->label) {
            return ((pm_labeled_edge_t *) dblist_data(curr))->state;
        }
        curr = dblist_next(curr);
    }
    return NULL;
}