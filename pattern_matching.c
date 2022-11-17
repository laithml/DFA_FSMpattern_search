//
// Created by Laith on 15/11/2022.
//

#include <asl.h>
#include <stdlib.h>
#include "pattern_matching.h"

int pm_init(pm_t *root){
    pm_state_t *zeroState=malloc(sizeof (pm_state_t));
    if(root == NULL||zeroState==NULL)
        return -1;
    zeroState->id=0;
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
    pm_int_t  currState=root->newstate;
    for(int i=0;i<n;i++)

        return 0;
}
