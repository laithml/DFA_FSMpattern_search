//
// Created by Laith on 15/11/2022.
//

#include "pattern_matching.h"
int currID;
int pm_init(pm_t *root){
    pm_state_t *zeroState=malloc(sizeof (pm_state_t));
    if(root == NULL||zeroState==NULL)
        return -1;
//    printf("Allocating state 0\n");
    zeroState->id=0;
    currID++;
    zeroState->depth=0;
    zeroState->_transitions=NULL;
    zeroState->fail=zeroState;
    zeroState->output=NULL;
    root->zerostate=zeroState;
    return 0;
}
int pm_addstring(pm_t *root,unsigned char *string, size_t n){
    if(root==NULL){
        return -1;
    }
    pm_state_t *currState=root->zerostate;
    pm_state_t *nextState;
    for(int i=0;i<n;i++){
      nextState= pm_goto_get(currState,string[i]);
      if(nextState==NULL){
          nextState= malloc(sizeof(pm_state_t));
          pm_goto_set(currState,string[i],nextState);
      }
        currState=nextState;
    }
        return 0;
}


int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state){
    if(from_state==NULL||to_state==NULL)
        return -1;
    if(from_state==to_state)
        return 0;
    
   if(from_state->_transitions==NULL){
       from_state->_transitions= malloc(sizeof (dblist_t));
       dblist_init(from_state->_transitions);
   }

    pm_int_t newDepth=(from_state->depth)+1;
    to_state->depth=newDepth;
    to_state->id=currID;
    currID++;
    pm_labeled_edge_t *newEdge= malloc(sizeof(pm_labeled_edge_t));
    if(newEdge==NULL||to_state==NULL)
        return -1;
    printf("Allocating state %d\n",to_state->id);
    newEdge->state=to_state;
    newEdge->label=symbol;
    dblist_append(from_state->_transitions,newEdge);
    to_state->_transitions=NULL;
    to_state->fail=NULL;
    to_state->output=NULL;
    printf("%d->%c->%d\n",from_state->id,symbol,to_state->id);
    return 0;
}

pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol){
    if(state==NULL||state->_transitions==NULL)
        return NULL;
    dblist_node_t *curr=dblist_head(state->_transitions);
    while(curr!=NULL){
        if(symbol==((pm_labeled_edge_t*) dblist_data(curr))->label){
            return ((pm_labeled_edge_t*) dblist_data(curr))->state;
        }
        curr=dblist_next(curr);
    }
    return NULL;
}