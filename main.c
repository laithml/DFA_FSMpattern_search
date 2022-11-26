
#include <string.h>
#include "pattern_matching.h"
int main() {
//    char *pattern[]={"abc","bca","cab","acb"};
    char *pattern[]={"E","BE","BD","BCD","CDBCAB","BCAA"};
    char text[]="xyzabcabde";
    pm_t *pm= malloc(sizeof(pm_t));
    pm_init(pm);
    for(int i=0;i<6;i++){
        pm_addstring(pm,pattern[i],strlen(pattern[i]));
    }
    pm_makeFSM(pm);
    dblist_t *dblist=pm_fsm_search(pm->zerostate,text, strlen(text));
    free(dblist);
    pm_destroy(pm);
    return 0;
}
