#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "pattern_matching.h"

#define TEST_HAS_FAILED 1
#define TEST_HAS_SUCCEEDED 0

int PrintList(dbllist_t *list) {

    int i = 0;
    if (!list) return -1;

    printf("-->");

    for (dbllist_node_t *p = dbllist_head(list); p != NULL; p = dbllist_next(p)) {
        printf("%d --> ", ((int *) p->data));
    }

    printf("\n");

    return 0;
}

int InitTest() {
    dbllist_t *list = (dbllist_t *) malloc(sizeof(dbllist_t));

    if (!list) {
        perror("error: ");
        return -1;
    }

    dbllist_init(list);
    dbllist_destroy(list, DBLLIST_LEAVE_DATA);
    // free(list);

    return 0;
}

int InsertTest() {
    dbllist_t *list = (dbllist_t *) malloc(sizeof(dbllist_t));
    int expected[] = {
            9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    if (!list) {
        perror("error: ");
        return -1;
    }

    dbllist_init(list);

    for (size_t i = 0; i < 10; i++)
        dbllist_prepend(list, (void *) i);
    for (size_t j = 0; j < 10; j++)
        dbllist_append(list, (void *) j);

    int isInPlace = TEST_HAS_SUCCEEDED;
    dbllist_node_t *ptr = dbllist_head(list);
    printf("size of %d\n",dbllist_size(list));
    for (size_t i = 0; i < 20; i++) {
        if (((int *) dbllist_data(ptr)) != expected[i]) {
            isInPlace = TEST_HAS_FAILED;
            break;
        }

        ptr = dbllist_next(ptr);
    }
    PrintList(list);

    dbllist_destroy(list, DBLLIST_LEAVE_DATA);

    return isInPlace;
}

int LeaveDestroyTest() {
    dbllist_t *list = (dbllist_t *) malloc(sizeof(dbllist_t));
    dbllist_init(list);
    if (!list) {
        perror("error: ");
        return -1;
    }

    int **dataArray = (int **) malloc(sizeof(int *)*5);

    if (!dataArray) {
        free(list);
        perror("error: ");
        return -1;
    }

    for (size_t i = 0; i < 5; i++) {
        dataArray[i] = (int *) malloc(sizeof(int));

        if (!dataArray[i]) {
            free(list);
            free(dataArray);
            perror("error: ");
            return -1;
        }

        *(dataArray[i]) = (int) i;
        dbllist_append(list, (void *) dataArray[i]);
    }

    dbllist_destroy(list, DBLLIST_LEAVE_DATA);

    int isData = TEST_HAS_SUCCEEDED;

    for (size_t i = 0; i < 5; i++) {
        if (!dataArray[i]) {
            isData = TEST_HAS_FAILED;
            break;
        }

        if (*(dataArray[i]) != i) {
            isData = TEST_HAS_FAILED;
            break;
        }
    }

    for (size_t i = 0; i < 5; i++) {
        free(dataArray[i]);
    }

    free(dataArray);

    return isData;
}

int FreeDestroyTest() {
    dbllist_t *list = (dbllist_t *) malloc(sizeof(dbllist_t));
    dbllist_init(list);

    if (!list) {
        perror("error: ");
        return -1;
    }

    int **dataArray = (int **) malloc(sizeof(int *)*5);

    if (!dataArray) {
        free(list);
        perror("error: ");
        return -1;
    }

    for (size_t i = 0; i < 5; i++) {
        dataArray[i] = (int *) malloc(sizeof(int));

        if (!dataArray[i]) {
            free(list);
            free(dataArray);
            perror("error: ");
            return -1;
        }

        *(dataArray[i]) = (int) i;
        dbllist_append(list, (void *) dataArray[i]);
    }
    PrintList(list);
    dbllist_destroy(list, DBLLIST_FREE_DATA);

    int isData = TEST_HAS_SUCCEEDED;

    for (size_t i = 0; i < 5; i++) {
        if (dataArray[i]) {
            isData = TEST_HAS_FAILED;
            break;
        }
    }

    free(dataArray);

    return isData;
}

int RemoveTest() {
    size_t s = sizeof(int);
    dbllist_t *listEven = (dbllist_t *) malloc(sizeof(dbllist_t));
    dbllist_t *listOdd = (dbllist_t *) malloc(sizeof(dbllist_t));

    int evenListData[] = {3, 2, 1, 0};
    int oddListData[] = {0, 1, 2, 3, 4};

    if (!listEven || !listOdd) {
        perror("error: ");
        return -1;
    }

    // even size list
    for (int i = 3; i >= 0; i--) {
        dbllist_append(listEven, (void *) &i);
    }
    int *expected = NULL;
    expected = malloc(s * 3);
    int test1[] = {3, 2, 0};
    memcpy(expected,test1,sizeof(test1));

    dbllist_node_t *ptr = dbllist_head(listEven);
    dbllist_node_t *removeMe = dbllist_prev(dbllist_tail(listEven));
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 3; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }

    free(expected);
    expected = malloc(s * 2);
    int test2[] = {2,1};
    memcpy(expected,test2,sizeof(test2));

    ptr = dbllist_head(listEven);
    removeMe = dbllist_head(listEven);
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 2; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }

    // let's keep the check within a loop so we can play with the size later on.
//    int expected[] = {2};
    free(expected);
    expected = malloc(s * 1);
    int test3[] = {2};
    memcpy(expected,test3,sizeof(test3));


    ptr = dbllist_head(listEven);
    removeMe = dbllist_tail(listEven);
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 1; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }

    // odd size list
    for (int i = 0; i < 5; i++) {
        dbllist_append(listOdd, (void *) &i);
    }
    free(expected);
    expected = malloc(s * 4);
    int test4[] = {0,1,2,4};
    memcpy(expected,test4,sizeof(test4));

    ptr = dbllist_head(listOdd);
    removeMe = dbllist_prev(dbllist_tail(listOdd));
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 4; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }
    free(expected);
    expected = malloc(s * 3);
    int test5[] = {1,2,4};
    memcpy(expected,test5,sizeof(test5));
//    int expected[] = {1, 2, 4};

    ptr = dbllist_head(listOdd);
    removeMe = dbllist_head(listOdd);
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 3; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }

//    int expected = {1, 2};
    free(expected);
    expected = malloc(s * 2);
    int test6[] = {1,2};
    memcpy(expected,test6,sizeof(test6));

    ptr = dbllist_head(listOdd);
    removeMe = dbllist_tail(listOdd);
    dbllist_remove(listEven, removeMe, DBLLIST_LEAVE_DATA);

    for (int i = 0; i < 2; i++) {
        if (*((int *) dbllist_data(ptr)) != expected[i]) {
            dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
            dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);
            return TEST_HAS_FAILED;
        }

        ptr = dbllist_next(ptr);
    }

    dbllist_destroy(listEven, DBLLIST_LEAVE_DATA);
    dbllist_destroy(oddListData, DBLLIST_LEAVE_DATA);

    return TEST_HAS_SUCCEEDED;
}


int ForwardTransitionTest() {
    char text[] = "Ywa-f0rk$;:M3B[9hEuI=D0sJ:*5$;df0urTKtj[?<8e?u7trans1tionsfJ6xYf0ward0utPfS*`|Ke:T|F-w_9@3GnTJLn3$ma@so2dqMj|WU{@]Sd7v`z|U35;0R?";
    char *patterns[4] = {"f0ur", "f0rward", "7trans1tions", "f0rk"};
    size_t pSize = 4;

    pm_t *pm = (pm_t *) malloc(sizeof(pm_t));

    if (!pm) {
        perror("error: ");
        return -1;
    }

    pm_init(pm);
    pm_addstring(pm, (unsigned char *) "f0ur", 4);
    pm_addstring(pm, (unsigned char *) "f0rward", 7);
    pm_addstring(pm, (unsigned char *) "7trans1tions", 12);
    pm_addstring(pm, (unsigned char *) "f0rk", 4);

    for (size_t i = 0; i < pSize; i++) {
        unsigned char symbol = (unsigned char) patterns[i][0];
        pm_state_t *current = pm_goto_get(pm->zerostate, symbol);

        if (!current) return TEST_HAS_FAILED;

        for (size_t j = 1; j < sizeof(patterns[i]); j++) {
            symbol = (unsigned char) patterns[i][j];
            current = pm_goto_get(current, symbol);
            int isData = 0;

            if (!current) {
                pm_destroy(pm);
                return TEST_HAS_FAILED;
            }

            for (dbllist_node_t *ptr = dbllist_head(current->_transitions); ptr != NULL; ptr = dbllist_next(ptr)) {
                pm_labeled_edge_t *edge = (pm_labeled_edge_t *) dbllist_data(ptr);

                if (edge->label == symbol) isData = 1;
            }

            if (isData!=0) {
                pm_destroy(pm);
                return TEST_HAS_FAILED;
            }

        }
    }

    pm_destroy(pm);
    return TEST_HAS_SUCCEEDED;
}


int FailureTransitionTest() {
    char *patterns[5] = {"fa1lur3", "trans1t1ons", "t3st", "1s", "3asy"};
    int expectedFailIds[27] = {0, 0, 22, 0, 0, 0, 24, 0, 0, 0, 0, 0, 22, 8, 22, 0, 0, 0, 24, 0, 8, 0, 0, 0, 0, 0, 0};
    size_t pSize = 5;

    pm_t *pm = (pm_t *) malloc(sizeof(pm_t));

    if (!pm) {
        perror("error: ");
        return -1;
    }

    pm_init(pm);
    pm_addstring(pm, (unsigned char *) "fa1lur3", 7);
    pm_addstring(pm, (unsigned char *) "trans1t1ons", 11);
    pm_addstring(pm, (unsigned char *) "t3st", 4);
    pm_addstring(pm, (unsigned char *) "1s", 2);
    pm_addstring(pm, (unsigned char *) "3asy", 4);
    pm_makeFSM(pm);

    for (size_t i = 0; i < pSize; i++) {
        pm_state_t *current = pm_goto_get(pm->zerostate, patterns[i][0]);

        if (!current) {
            pm_destroy(pm);
            return TEST_HAS_FAILED;
        }

        // convert 2d array to 1d arra
        // get the corresponding id of patterns[i][0]
        if (current->fail->id != expectedFailIds[pSize * i]) {
            pm_destroy(pm);
            return TEST_HAS_FAILED;
        }

        for (size_t j = 1; j < patterns[i][j] != '\0'; j++) {
            current = pm_goto_get(current, (unsigned char) patterns[i][j]);

            if (!current) {
                pm_destroy(pm);
                return TEST_HAS_FAILED;
            }

            // convert 2d array to 1d array.
            // get the corresponding id of patterns[i][j]
            if (current->fail->id != expectedFailIds[pSize * i + j]) {
                pm_destroy(pm);
                return TEST_HAS_FAILED;
            }
        }
    }

    pm_destroy(pm);
    return TEST_HAS_SUCCEEDED;
}

int main(int argc, char *argv[]) {
    char *runner = "--test forward-transitions";

    if (strcmp(runner, "--test init") == 0)
        return InitTest();

    else if (strcmp(runner, "--test insert") == 0)
        return InsertTest();

    else if (strcmp(runner, "--test leave-destroy") == 0)
        return LeaveDestroyTest();

    else if (strcmp(runner, "--test free-destroy") == 0)
        return FreeDestroyTest();

    else if (strcmp(runner, "--test remove") == 0)
        return RemoveTest();

    else if (strcmp(runner, "--test forward-transitions") == 0) {
        return ForwardTransitionTest();
    }

    else if (strcmp(runner, "--test failure-transitions") == 0) {
        return FailureTransitionTest();
    }

    else if (strcmp(runner, "--test pattern-scan") == 0) {
    }

    else
        printf("Usage: --test <NAME>\n");

    return 0;
}