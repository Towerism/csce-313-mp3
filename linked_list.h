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

#include<stdio.h>
#include<stdlib.h>
#include "linked_list.h"
#include "unistd.h"
#include "errno.h"
#include <assert.h>
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

typedef void* Addr;

/* This Node struct represents a node in a linked list.
*/
struct Node {
	Addr ptr;
	struct Node* next;
	struct Node* prev;
};

struct Linked_list {
    struct Node* root;
    struct Node* tail;
    int size;
};

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE LINKED_LIST */
/*--------------------------------------------------------------------------*/

// constructs a linked_list
struct Linked_list list_create();

// destroys a linked list
void list_destroy(struct Linked_list* list);

// constructs a node and pushes it onto list
Addr list_push(struct Linked_list* list, Addr ptr);

// removes and returns the last node in the list
Addr list_pop(struct Linked_list* list);

// constructs a node and inserts it after the given node
Addr list_insert_after(struct Linked_list* list, struct Node* node, Addr ptr);

// removes the node from the list
Addr list_remove(struct Linked_list* list, struct Node* node);

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

    int test_list_push();
    int test_list_pop();
    int test_list_remove();
#endif
