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

#include "linked_list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* FORWARD DECLARES */
/*--------------------------------------------------------------------------*/

static struct Node* node_create_at(struct Linked_list* list, struct Node* addr, Addr ptr, int len);
static struct Node* node_create_at_offset(struct Linked_list* list, struct Node* node, int offset, Addr ptr, int len);
static int list_insert_before(struct Linked_list* list, struct Node* node, Addr ptr, int len);
static int list_insert_after(struct Linked_list* list, struct Node* node, Addr ptr, int len);
static int list_insert_front(struct Linked_list* list, Addr ptr, int len);

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE LINKED_LIST */
/*--------------------------------------------------------------------------*/

Addr node_value(struct Node* node) {
    return (Addr)node + sizeof(struct Node);
}

struct Linked_list list_create(int max_data_size, int blocks) {
    struct Linked_list list;
    list.block_size = sizeof(struct Node) + max_data_size;
    list.memory_pool = malloc(list.block_size * blocks);
    list.head = NULL;
    list.tail = NULL;
    return list;
}

void list_destroy(struct Linked_list* list) {
    free(list->memory_pool);
}

int list_insert(struct Linked_list* list, Addr ptr, int len) {
    if (list->head == NULL || list->head != list->memory_pool) {
        return list_insert_front(list, ptr, len);
    }
    struct Node* node = list->memory_pool;
    // keeps interating until it finds a gap
    while (node->next != NULL && node->next - node <= list->block_size) {
        node = node->next;
    }
    return list_insert_after(list, (struct Node*)node, ptr, len);
}

int list_remove(struct Linked_list* list, struct Node* node) {
    if (node == list->head) {
        list->head = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }
}

/*--------------------------------------------------------------------------*/
/* STATIC FUNCTIONS */
/*--------------------------------------------------------------------------*/

// creates a linked list node at the given address
static struct Node* node_create_at(struct Linked_list* list, struct Node* addr, Addr ptr, int len) {
    struct Node* node = addr;
    node->next = NULL;
    node->prev = NULL;
    memcpy(node_value(node), ptr, len);
    return node;
}

static struct Node* node_create_at_offset(struct Linked_list* list, struct Node* node, int offset, Addr ptr, int len) {
    return node_create_at(list, (struct Node*)((Addr)node + offset), ptr, len);
}

static int list_insert_before(struct Linked_list* list, struct Node* node, Addr ptr, int len) {
    struct Node* new_node = node_create_at_offset(list, node, -list->block_size, ptr, len);
    new_node->prev = node->prev;
    new_node->next = node;
    if (node == list->head) {
        list->head = new_node;
    } else {
        node->prev->next = new_node;
    }
}

static int list_insert_after(struct Linked_list* list, struct Node* node, Addr ptr, int len) {
    struct Node* new_node = node_create_at_offset(list, node, list->block_size, ptr, len);
    new_node->next = node->next;
    new_node->prev = node;
    if (list->tail == node) {
        list->tail = new_node;
    } else {
        node->next->prev = new_node;
    }
    node->next = new_node;
}

static int list_insert_front(struct Linked_list* list, Addr ptr, int len) {
    if (list->head != NULL) {
        return list_insert_before(list, list->head, ptr, len);
    }
    struct Node* new_node = node_create_at(list, list->memory_pool, ptr, len);
    list->head = list->tail = new_node;
}

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

// helper function
void populate_test_list(struct Linked_list* list, int* array) {
    int i;
    for (i = 0; i < 3; ++i) {
        list_insert(list, array + i, sizeof(int));
    }
}

// helper function
int* init_test_array() {
    int* array = (int*)malloc(sizeof(int) * 3);
    int i;
    for (i = 0; i < 3; ++i) {
        array[i] = i;
    }    
    return array;
}

// helps test forward and backward iteration
int test_list_iterate_helper(struct Node* node) {
    int success = 1;
    int i = 0;
    while (1) {
        success &= (*(int*)node_value(node) == i);
        ++i;
        if (node->next == NULL) {
            break;
        }
        node = node->next;
    }
    i = 2;
    while (node != NULL) {
        success &= (*(int*)node_value(node) == i);
        --i;
        node = node->prev;
    }
    return success;
}

int test_list_insert() {
    int success = 1;
    struct Linked_list list = list_create(sizeof(int), 3);
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    // test forward and backward iteration
    success &= test_list_iterate_helper(list.head);
    list_destroy(&list);
    free(array);
    return success;
}

int test_list_remove() {
    int success = 1;
    struct Linked_list list = list_create(sizeof(int), 3);
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    // test remove on a non-head, non-tail node
    list_remove(&list, list.head->next);
    success &= (*(int*)node_value(list.tail) == 2 && *(int*)node_value(list.head) == 0);
    // test remove on a head node
    list_remove(&list, list.head);
    success &= *(int*)node_value(list.head) == 2;
    list_destroy(&list);
    free(array);
    return success;
}
