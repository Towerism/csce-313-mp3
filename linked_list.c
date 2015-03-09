/*
    File: linked_list.c

    Author: Martin Fracker & Christopher Findeisen 
            Department of Computer Science
            Texas A&M University
    Date  : <March 6, 2015>

    This file contains the implementation of the module "LINKED_LIST".

*/

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

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE LINKED_LIST */
/*--------------------------------------------------------------------------*/

struct Linked_list list_create() {
    struct Linked_list list;
    list.root = NULL;
    list.tail = list.root;
    list.size = 0;
    return list;
}

struct Node* list_push(struct Linked_list* list, Addr ptr, int spacing) {
    struct Node* new_node = malloc(sizeof(struct Node));
    ++list->size;
    if (list->root == NULL) {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->root = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->next = NULL;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
    new_node->ptr = ptr;
    new_node->spacing = spacing;
    return new_node;
}

struct Node* list_pop(struct Linked_list* list) {
    struct Node* popped = list->tail;
    --list->size;
    if (list->root == list->tail) {
        list->root = list->tail = NULL;
    } else {
        list->tail = popped->prev;
        list->tail->next = NULL;
    }
    return popped;
}

struct Node* list_remove(struct Linked_list* list, struct Node* node) {
    --list->size;
    if (node == list->root) {
        list->root = node->next;
    } else if (list->root == list->tail) {
        list->root = list->tail = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    return node;
}

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

// if any subtest fails, the entire test will fail
int test_list() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = (int*)malloc(sizeof(int) * 3);
    int i;
    for (i = 0; i < 3; ++i) {
        array[i] = i;
        list_push(&list, array + i, 0);
    }
    struct Node* node = list.root;
    i = 0;
    // test that list_push worked correctly by
        // checking that forward iteration works correctly
    while (node != NULL) {
        success &= (*(int*)node->ptr == i);
        ++i;
        node = node->next;
    }
    i = 2;
        // and reverse iteration works correctly
    while (node != NULL) {
        success &= (*(int*)node->ptr == i);
        --i;
        node = node->prev;
    }
    // test list_pop
    for (i = 2; i >= 0; --i) {
        struct Node* node = list_pop(&list);
        success &= (*(int*)node->ptr == i);
    }
    // make sure we have a list which is empty
    success &= (list.root == NULL && list.tail == NULL);
    // populate the list again so we can test remove
    for (i = 0; i < 3; ++i) {
        list_push(&list, array + i, 0);
    }
    // test remove on a non-root, non-tail node
    list_remove(&list, list.root->next);
    success &= (*(int*)list.tail->ptr == 2 && *(int*)list.root->ptr == 0);
    // test remove on a root node
    list_remove(&list, list.root);
    success &= list.size == 1 && list.root != NULL;
    success &= *(int*)list.root->ptr == 2;
    return success;
}

