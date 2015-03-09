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
	int spacing; // the amount of space ptr points to;
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

// constructs a node and pushes it onto list
struct Node* list_push(struct Linked_list* list, Addr ptr, int spacing);

// removes and returns the last node in the list
struct Node* list_pop(struct Linked_list* list);

// removes the node from the list
struct Node* list_remove(struct Linked_list* list, struct Node* node);

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

    int test_list(); 
#endif
