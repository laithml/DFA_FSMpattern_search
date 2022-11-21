#include <stdio.h>
#include <string.h>
#include "pattern_matching.h"
int main() {
//    char *pattern[]={"abc","bca","cab","acb"};
    char *pattern[]={"E","BE","BD","BCD","CDBCAB","BCAA"};
    char text[]="xyzabcabde";
    pm_state_t *zeroState= malloc(sizeof(pm_state_t));
    pm_init(zeroState);
    for(int i=0;i<6;i++){
        pm_addstring(zeroState,pattern[i],strlen(pattern[i]));
    }
    pm_makeFSM(zeroState);

    return 0;
}
