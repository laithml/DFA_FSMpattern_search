Pattern-Matching
Authored by Laith Al-Muhtaseb
211353297

==Description==
This program made to match strings using FSM and DFA algorithms, first the program build a tree of the patterns,
and make the failure of all node by the longest suffix.

==methods==
 --slist--
dbllist_remove: remove the specific node from the list.
dbllist_prepend: prepend data to list (add as first node of the list).
dbllist_append: append data to list (add as last node of the list).
dbllist_destroy: destroy and deallocate the memory hold by a list.
dbllist_init: initialize a double linked list.

 --pattern_matching--
pm_init: initialize the fsm parameters.

pm_addstring: Adds a new string to the fsm, start from the zeroState, pass in loop, and run goto_get function,
if the symbol already added at this level in the fsm, so we move on to the state that the label point at.
else define a new state and send to goto_set function.

pm_goto_set: input from and to state and the symbol that connect these two state, if the from state doesn't have any children
(transitions list is null), so allocate and initialize the transitions list, then append a node that contain pointer of data
from pm_labeled_edge_t type,the pointer of state is to_state and the label is the symbol we get as input to the method.

pm_goto_get: input a state and a symbol, pass at transitions list and search inside the node data (pm_labeled_edge_t)
if the symbol equal the label, return the state that the label point at,else return null.




==Program Files==
slist.h, slist.c, pattern_matching.c, pattern_matching.h, main.c

==How to compile==
compile:gcc slist.c pattern_matching.c main.c -o main
run: ./main

==input==
files: patterns and the text

==output==
list of pm_match struct that contain the founded pattern and in where start position,
and where end it and in which state of the tree end.