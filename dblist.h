/**

  @file
  
  @brief Implementation of a very simple double linked list.
  
*/

#ifndef dblist_H
#define dblist_H

//! The definition of a double linked list node
struct dblist_node
{
	void *data; // Pointer to data of this node
	struct dblist_node *next; // Pointer to next node on list
	struct dblist_node *prev; // Pointer to previous node on list
};

//! double linked list node type
typedef struct dblist_node dblist_node_t;

//! The definition of a double linked list
struct dblist
{
	dblist_node_t *head; // Pointer to head of list
	dblist_node_t *tail; // Pointer to tail of list
	unsigned int size; // The number of elements in the list
};

//! double linked list type
typedef struct dblist dblist_t;


// you have to use these macros, do not use the inner variables of the list!!
//! Macro to get the head node of a list l
#define dblist_head(l) l->head
//! Macro to get the tail node of a list l
#define dblist_tail(l) l->tail
//! Macro to get the size of a list l
#define dblist_size(l) l->size
//! Macro to get the next node of l
#define dblist_next(n) n->next
//! Macro to get the prev node of l
#define dblist_prev(n) n->prev
//! Macro to get the data of node l
#define dblist_data(n) n->data

//! Specifies whether dblist_destroy should deallocate or not stored elements
typedef enum { dblist_LEAVE_DATA = 0, dblist_FREE_DATA } dblist_destroy_t;

/** Initialize a double linked list
	\param list - the list to initialize */
void dblist_init(dblist_t *);

/** Destroy and de-allocate the memory hold by a list
	\param list - a pointer to an existing list
	\param dealloc flag that indicates whether stored data should also be de-allocated */
void dblist_destroy(dblist_t *,dblist_destroy_t);


/** Append data to list (add as last node of the list)
	\param list - a pointer to a list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure */
int dblist_append(dblist_t *,void *);




/** Prepend data to list (add as first node of the list)
	\param list - a pointer to list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure
*/
int dblist_prepend(dblist_t *,void *);

/** \brief Remove the specific node from the list. 
	\param to a pointer to the list
	\param pointer to the node that should be removed.
	\param dealloc flag that indicates whether to de-allocated the data in the node 
	\return 0 on success, or -1 on failure
*/

int dblist_remove(dblist_t *, dblist_node_t* ,dblist_destroy_t);
#endif
