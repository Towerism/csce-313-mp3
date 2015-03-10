/*
    File: linked_list.h

    Author: Martin Fracker & Christopher Findeisen
            Department of Computer Science
            Texas A&M University
    Date  : 03/06/15

*/

#ifndef _linked_list_h_                   // include file only once
#define _linked_list_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

typedef void* Addr;

/* This Node struct represents a node in a linked list.
*/
struct Node {
    struct Node* next;
    struct Node* prev;
};

struct Linked_list {
    struct Node* head;
    struct Node* tail;
    int block_size;
    struct Node* memory_pool;
};

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE LINKED_LIST */
/*--------------------------------------------------------------------------*/

// returns a pointer to the data that the node points to
Addr node_value(struct Node* node);

// constructs a linked_list
struct Linked_list list_create(int data_size, int blocks);

// destroys a linked list
void list_destroy(struct Linked_list* list);

// constructs a node and inserts it at the end of the list
int list_insert(struct Linked_list* list, Addr ptr, int len);

// removes the node from the list
int list_remove(struct Linked_list* list, struct Node* node);

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

int test_list_insert();
int test_list_remove();
#endif
