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
    list.head = NULL;
    list.tail = list.head;
    list.size = 0;
    return list;
}

void list_destroy(struct Linked_list* list) {
    while (list->size > 0) {
        Addr ptr = list_pop_front(list);
    }
}

Addr list_push_front(struct Linked_list* list, Addr ptr) {
    if (list->head != NULL) {
        return list_insert_before(list, list->head, ptr);
    }
    struct Node* new_node = malloc(sizeof(struct Node));
    ++list->size;
    list->head = list->tail = new_node;
    new_node->next = new_node->prev = NULL;
    new_node->ptr = ptr;
    return ptr;
}

Addr list_push_back(struct Linked_list* list, Addr ptr) {
    if (list->tail == NULL) {
        return list_push_front(list, ptr);
    }
    return list_insert_after(list, list->tail, ptr);
}

Addr list_pop_front(struct Linked_list* list) {
    if (list->head == NULL) {
        return NULL;
    }
    return list_remove(list, list->head);
}

Addr list_pop_back(struct Linked_list* list) {
    if (list->tail == NULL) {
        return NULL;
    }
    return list_remove(list, list->tail);
}

Addr list_insert_before(struct Linked_list* list, struct Node* node, Addr ptr) {
    struct Node* new_node = malloc(sizeof(struct Node));
    ++list->size;
    new_node->prev = node->prev;
    new_node->next = node;
    if (node == list->head) {
        list->head = new_node;
    } else {
        node->prev->next = new_node;
    }
    node->prev = new_node;
    new_node->ptr = ptr;
    return ptr;
}

Addr list_insert_after(struct Linked_list* list, struct Node* node, Addr ptr) {
    struct Node* new_node = malloc(sizeof(struct Node));
    ++list->size;
    new_node->next = node->next;
    new_node->prev = node;
    if (list->tail == node) {
        list->tail = new_node;
    } else {
        node->next->prev = new_node;
    }
    node->next = new_node;
    new_node->ptr = ptr;
    return ptr;
}

Addr list_remove(struct Linked_list* list, struct Node* node) {
    Addr ret = node->ptr;
    --list->size;
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
    free(node);
    return ret;
}

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

// helper function
void populate_test_list(struct Linked_list* list, int* array) {
    int i;
    for (i = 0; i < 3; ++i) {
        list_push_back(list, array + i);
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
    while (node != NULL) {
        success &= (*(int*)node->ptr == i);
        ++i;
        node = node->next;
    }
    i = 2;
    while (node != NULL) {
        success &= (*(int*)node->ptr == i);
        --i;
        node = node->prev;
    }
    return success;
}

int test_list_push_front() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    int i;
    for (i = 2; i >= 0; --i) {
        list_push_front(&list, array + i);
    }
    // test forward and backward iteration
    success &= test_list_iterate_helper(list.head);
    list_destroy(&list);
    free(array);
    return success;
}

int test_list_push_back() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    // test forward and backward iteration
    success &= test_list_iterate_helper(list.head);
    list_destroy(&list);
    free(array);
    return success;
}

int test_list_pop_front() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    // test list_pop_front
    int i;
    for (i = 0; i < 3; ++i) {
        int x = *(int*)list_pop_front(&list);
        success &= (x == i);
    }
    // make sure we have a list which is empty
    success &= (list.head == NULL && list.tail == NULL);
    list_destroy(&list);
    free(array);
    return success;
}

int test_list_pop_back() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    struct Node* node = list.head;
    // test list_pop_back
    int i;
    for (i = 2; i >= 0; --i) {
        int x = *(int*)list_pop_back(&list);
        success &= (x == i);
    }
    // make sure we have a list which is empty
    success &= (list.head == NULL && list.tail == NULL);
    list_destroy(&list);
    free(array);
    return success;
}

int test_list_insert_before() {    
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    struct Node* base = list.head;
    int* x = malloc(sizeof(int));
    *x = 10;
    // test insertion before head 
    list_insert_before(&list, base, x);
    int* y = malloc(sizeof(int));
    *y = 101;
    // test insertion in between two nodes
    list_insert_before(&list, base, y);
    success &= *(int*)list.head->ptr == 10;
    success &= *(int*)list.head->next->next->prev->ptr == 101;
    list_destroy(&list);
    free(array);
    free(x);
    free(y);
    return success;
}

int test_list_insert_after() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    struct Node* base = list.tail;
    int* x = malloc(sizeof(int));
    *x = 23;
    // test insertion after tail 
    list_insert_after(&list, base, x);
    int* y = malloc(sizeof(int));
    *y = 123;
    // test insertion in between two nodes
    list_insert_after(&list, base, y);
    success &= *(int*)list.tail->ptr == 23;
    success &= *(int*)list.tail->prev->prev->next->ptr == 123;
    list_destroy(&list);
    free(array);
    free(x);
    free(y);
    return success;
}

int test_list_remove() {
    int success = 1;
    struct Linked_list list = list_create();
    int* array = init_test_array(); // { 0, 1, 2 }
    populate_test_list(&list, array);
    // test remove on a non-head, non-tail node
    list_remove(&list, list.head->next);
    success &= (*(int*)list.tail->ptr == 2 && *(int*)list.head->ptr == 0);
    // test remove on a head node
    list_remove(&list, list.head);
    success &= list.size == 1 && list.head != NULL;
    success &= *(int*)list.head->ptr == 2;
    list_destroy(&list);
    free(array);
    return success;
}
