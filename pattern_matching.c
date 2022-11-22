//
// Created by Laith on 15/11/2022.
//

#include <string.h>
#include "pattern_matching.h"


dblist_t* uni(dblist_t *l1,dblist_t *l2){
    if(l1!=NULL)
        return l2;
    if(l2!=NULL)
        return l1;
    dblist_node_t *temp=dblist_head(l2);
    while(temp!=NULL){
        dblist_append(l1,temp->data);
        temp=dblist_next(temp);
    }
    return l1;
}

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
            pm_state_t *state =r->fail;
            if(state!=NULL) {
                while (pm_goto_get(state, currSymbol) == NULL) {
                    if (state == root->zerostate){
                        state = NULL;
                        edgeR->state->fail=root->zerostate;
                        break;
                }
                    state = state->fail;
                }if(state != NULL){
                    edgeR->state->fail = pm_goto_get(state, currSymbol);
                    if(edgeR->state->output ==NULL){
                        edgeR->state->output= malloc(sizeof(dblist_t));
                        dblist_init( edgeR->state->output);
                    }
                        edgeR->state->output= uni(edgeR->state->output,edgeR->state->fail->output);
                    printf("Setting f(%d) = %d\n", edgeR->state->id, edgeR->state->fail->id);
                }
            }else{
                edgeR->state->fail=root->zerostate;
            }

            headT = headT->next;
        }
    }
    free(queue);
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
    currState->output= malloc(sizeof(dblist_t));
    dblist_init(currState->output);
    dblist_append(currState->output,string);
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
    printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);
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

dblist_t* pm_fsm_search(pm_state_t *zerostate,unsigned char *string,size_t n){
    dblist_t *matched_list= malloc(sizeof(dblist_t));
    dblist_init(matched_list);
    pm_state_t *currState;

    for(int i=0;i<n;i++){
        currState= pm_goto_get(zerostate,string[i]);
        pm_state_t *failState=zerostate;
        while(currState==NULL){
            if(failState->depth==0){
                currState=failState;
                break;
            }
            failState=failState->fail;
            if(failState==NULL)
                return NULL;
            currState= pm_goto_get(failState,string[i]);
        }

        if(dblist_head(currState->output)!=NULL){
            dblist_node_t *tempHead=dblist_head(currState->output);
            int j=0;
            while(tempHead!=NULL){
                pm_match_t *newMatch= malloc(sizeof(pm_match_t));
                if(newMatch==NULL)
                    return NULL;
                newMatch->pattern=dblist_data(tempHead);
                newMatch->start_pos=j -strlen(newMatch->pattern)+1;
                newMatch->end_pos=j;
                newMatch->fstate=currState;
                dblist_append(matched_list,newMatch);
                printf("pattern: %s ,start at: %d ,ends at: %d ,last state: %d\n",newMatch->pattern,newMatch->start_pos,newMatch->end_pos,currState->id);
                tempHead=dblist_next(tempHead);
                j++;
            }
        }
        zerostate=currState;
    }
    return matched_list;
}