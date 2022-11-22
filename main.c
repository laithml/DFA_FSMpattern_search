
#include <string.h>
#include "pattern_matching.h"
int main() {
//    char *pattern[]={"abc","bca","cab","acb"};
    char *pattern[]={"E","BE","BD","BCD","CDBCAB","BCAA"};
    char text[]="xyzabcabde";
    pm_t *zeroState= malloc(sizeof(pm_t));
    pm_init(zeroState);
    for(int i=0;i<6;i++){
        pm_addstring(zeroState,pattern[i],strlen(pattern[i]));
    }
    pm_makeFSM(zeroState);
    pm_fsm_search(zeroState,text, strlen(text));

    return 0;
}
