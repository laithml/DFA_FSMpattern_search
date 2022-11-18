#include <stdio.h>
#include <string.h>
#include "pattern_matching.h"
int main() {
    char *pattern[]={"abc","bca","cab","acb"};
//    char *pattern[]={"abc","acb"};
    char text[]="xyzabcabde";
    pm_state_t *zeroState= malloc(sizeof(pm_state_t));
    pm_init(zeroState);
    for(int i=0;i<4;i++){
        pm_addstring(zeroState,pattern[i],strlen(pattern[i]));
    }

    return 0;
}
