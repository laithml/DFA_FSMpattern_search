//
// Created by Laith on 15/11/2022.
//
#include "pattern_matching.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void rec_destroy(pm_state_t *state){
    if(state->_transitions==NULL){
        if (state->output != NULL) {
            dbllist_destroy(state->output, DBLLIST_LEAVE_DATA);
            free(state->output);
        }
        free(state);
    }else {
        dbllist_node_t *curr = dbllist_head(state->_transitions);

        while (curr != NULL) {
            rec_destroy(((pm_labeled_edge_t *) dbllist_data(curr))->state);
            curr = dbllist_next(curr);
        }
        if (state->output != NULL) {
            dbllist_destroy(state->output, DBLLIST_LEAVE_DATA);
            free(state->output);
        }

        dbllist_destroy(state->_transitions, DBLLIST_FREE_DATA);
        free(state->_transitions);
        free(state);
    }
}

dbllist_t* uni(dbllist_t *l1,dbllist_t *l2){
    if(l1==NULL)
        return l2;
    if(l2==NULL)
        return l1;
    dbllist_node_t *temp=dbllist_head(l2);
    while(temp!=NULL){
        dbllist_append(l1,temp->data);
        temp=dbllist_next(temp);
    }
    return l1;
}

int pm_init(pm_t *root) {
    pm_state_t *zeroState = calloc(1,sizeof(pm_state_t));
    if( zeroState == NULL){
        pm_destroy(root);
        return -1;
    }
    if (root == NULL )
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
    if(root==NULL)
        return -1;
    pm_state_t *zeroState = root->zerostate;
    dbllist_node_t *head = dbllist_head(zeroState->_transitions);
    dbllist_t *queue = calloc(1,sizeof(dbllist_node_t));
    if( queue == NULL ){
        pm_destroy(root);
        return -1;
    }
//first level
    dbllist_init(queue);
    while (head != NULL) {
        pm_labeled_edge_t *edge = (pm_labeled_edge_t *) dbllist_data(head);
        if(dbllist_append(queue, edge->state)==-1){
            pm_destroy(root);
            return -1;
        }
        edge->state->fail = zeroState;
        head = head->next;
    }

//other level
    while (dbllist_head(queue) != NULL) {
        pm_state_t *r = (pm_state_t *) dbllist_data(dbllist_head(queue));//father
        if(dbllist_remove(queue, dbllist_head(queue), DBLLIST_LEAVE_DATA)==-1){
            pm_destroy(root);
            return -1;
        }
        dbllist_node_t *headT=NULL;
        if (r->_transitions != NULL)
            headT = dbllist_head(r->_transitions);//son

        while (headT != NULL) {
            pm_labeled_edge_t *edgeR = ((pm_labeled_edge_t *) dbllist_data(headT));
            if(dbllist_append(queue, edgeR->state)==-1){
                pm_destroy(root);
                return -1;
            }
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
                        edgeR->state->output= calloc(1,sizeof(dbllist_t));
                        if(edgeR->state->output == NULL){
                            pm_destroy(root);
                            return -1;
                        }
                        dbllist_init( edgeR->state->output);
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


int pm_addstring(pm_t *root, unsigned char *string, _size_t n) {
    if (root == NULL) {
        return -1;
    }
    pm_state_t *currState = root->zerostate;
    pm_state_t *nextState;
    for (int i = 0; i < n; i++) {
        nextState = pm_goto_get(currState, string[i]);
        if (nextState == NULL) {
            nextState = calloc(1,sizeof(pm_state_t));
            if( nextState == NULL){
                pm_destroy(root);
                return -1;
            }printf("Allocating state %d\n", root->newstate+1);
            nextState->id = ++(root->newstate);
            if(pm_goto_set(currState, string[i], nextState)==-1){
                pm_destroy(root);
                return -1;
            }
        }
        currState = nextState;
    }
    currState->output= calloc(1,sizeof(dbllist_t));
    if( currState->output == NULL) {
        pm_destroy(root);
        return -1;
    }dbllist_init(currState->output);
    if(dbllist_append(currState->output,string)==-1){
        pm_destroy(root);
        return -1;
    }
    return 0;
}


int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state) {
    if (from_state == NULL || to_state == NULL)
        return -1;
    if (from_state == to_state)
        return 0;

    if (from_state->_transitions == NULL) {
        from_state->_transitions = calloc(1,sizeof(dbllist_t));
        if( from_state->_transitions == NULL) {
            return -1;
        }
            dbllist_init(from_state->_transitions);
    }

    pm_int_t newDepth = (from_state->depth) + 1;
    to_state->depth = newDepth;
    pm_labeled_edge_t *newEdge = calloc(1,sizeof(pm_labeled_edge_t));
    if (newEdge == NULL || to_state == NULL)
        return -1;

    newEdge->state = to_state;
    newEdge->label = symbol;
    if(dbllist_append(from_state->_transitions, newEdge)==-1){
        rec_destroy(from_state);
        return -1;
    }
    to_state->_transitions = NULL;
    to_state->fail = NULL;
    to_state->output = NULL;
    printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);
    return 0;
}

pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol) {
    if (state == NULL || state->_transitions == NULL)
        return NULL;
    dbllist_node_t *curr = dbllist_head(state->_transitions);
    while (curr != NULL) {
        if (symbol == ((pm_labeled_edge_t *) dbllist_data(curr))->label) {
            return ((pm_labeled_edge_t *) dbllist_data(curr))->state;
        }
        curr = dbllist_next(curr);
    }
    return NULL;
}

dbllist_t* pm_fsm_search(pm_state_t *state,unsigned char *string,_size_t n){
    dbllist_t *matched_list= calloc(1,sizeof(dbllist_t));
    if(matched_list == NULL){
        return NULL;
    }
    dbllist_init(matched_list);
    pm_state_t *currState;


    for(int i=0;i<n;i++){
        currState= pm_goto_get(state,string[i]);
        pm_state_t *failState=state;
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

        if(currState->output!=NULL){
            dbllist_node_t *tempHead=dbllist_head(currState->output);
            while(tempHead!=NULL){
                pm_match_t *newMatch= calloc(1,sizeof(pm_match_t));
                if(newMatch==NULL)
                    return NULL;
                newMatch->pattern=dbllist_data(tempHead);
                newMatch->start_pos=i - (int)strlen(newMatch->pattern)+1;
                newMatch->end_pos=i;
                newMatch->fstate=currState;
                if(dbllist_append(matched_list,newMatch)==-1){
                    rec_destroy(state);
                    return NULL;
                }
                tempHead=dbllist_next(tempHead);
            }
        }
        state=currState;
    }
    return matched_list;
}



void pm_destroy(pm_t *pm){
    if(pm==NULL||pm->zerostate== NULL)
        return;
    rec_destroy(pm->zerostate);
    free(pm);

}
