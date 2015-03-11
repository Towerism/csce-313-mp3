/*
    File: my_allocator.c

    Author: Christopher Findeisen & Martin Fracker
            Department of Computer Science
            Texas A&M University
    Date  : <March 4, 2015>

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "my_allocator.h"
#include "linked_list.h"
#include "unistd.h"
#include "errno.h"
#include <assert.h>
#include <math.h>
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */


/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

void * memory_pool = NULL;
unsigned int b_b_s;
void * list_offset;
int high_order = 1;
struct Linked_list* free_lists[8];
//big-endian. That is..the largest LL is stored first.
void * free_list_allocator( int total_mem_request ){
  //linked List + all the nodes.
//log takes a double but we want it rounded to an int
  high_order = log2((double) total_mem_request / b_b_s);
  int remaining_mem = total_mem_request;

  int offset = 0;//some num that = the size of the linked list

  for(int order = high_order; order > 0; --order){
    offset += sizeof(struct Linked_list) ;

    *free_lists[order] = list_create(pow(2,order) * b_b_s, 10);
    struct Linked_list * ll = free_lists[order];
        ll->block_size = pow(2, order) * b_b_s;
        //allocate as many high-order blocks as possible
        while(remaining_mem >= ll->block_size){
        offset += (sizeof(struct Node);
        list_insert(ll, , ); //struct Linked_list* list, struct Node* node, Addr ptr, int len
        remaining_mem -= ll->block_size;
        }
        offset += sizeof(struct Linked_list);
        }
        return memory_pool + offset;
        }

        unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length){
        //reset error flag
        errno = 0;

        if(_length < _basic_block_size || _basic_block_size == 0){
        printf("Provide sane input please. \n");
        return 0;
        }
        if(memory_pool != NULL){
          printf("You must release the memory you already allocated \n");
          return 0;
        }

        memory_pool = sbrk(_length);
        b_b_s = _basic_block_size;
        if(errno)
          return 0;

        else {
          list_offset = free_list_allocator(_length);
          return _length;
        }
        }
int release_allocator(){
  if(memory_pool == NULL) {
    return 0;
  }

  brk(memory_pool);
  //update back to NULL
  memory_pool = NULL;
  //what is this fxn supposed to return?
  return 1;
}


//inspired by Knuth
Addr findBuddy(Addr x, int size){
  //this can be replaced with ^= left XOR (0x1 << s-1), but I want it to be
  //tested with unit tests.
  int remainder = (uintptr_t)x % (int)pow(2, size+1);
  if (remainder == 0)
    return x - (int)pow(2, size);
  else if (remainder == pow(2, size))
    return x - (int)pow(2, size);
  else{
    fprintf(stderr, "Didn't find a buddy...\n");
    exit(1);
  }
}


struct Linked_list* getList(int order){
  struct Linked_list* point_list* = list_create(5,5);
  return point_list;
}

Addr split(unsigned int order){
  //C error handling is nonexistent(almost) this will abort if we run out of memory..
  if(order > high_order){
    fprintf(stderr, "Ran out of memory while splitting..\n");
    exit(2);
  }

  struct Linked_list* free_list = getList(order);
  struct Linked_list* lower_free_list = getList(order-1);

  if(free_list->head == NULL){
    split(order+1);
  }

  //assuming ^ worked.. otherwise it would have aborted.
  Addr left = list_pop_back(free_list);
  Addr right = (uintptr_t)left ^ (0x1 << (free_list->size-1)); //toggling bit..
  //yikes..pointer bit operations are illegal? hmm..

  list_push_back(lower_free_list, left);
  return right;
}


extern Addr my_malloc(unsigned int _length) {
  double exact = log2((double) _length / b_b_s) ;
  int order = ceil(exact);
  Addr free_block = NULL;
  struct Linked_list* free_list = getList(order);
  if(free_list->head == NULL)
    free_block = split(order);
  else {
    free_block = list_pop_back(free_list);
  }
  return free_block;

}

extern int my_free(Addr _a) {
  /* Same here! */

  free(_a);
  return 0;

}

/* Addr coalesce(unsigned int block_size) */

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

int test_init(){

  init_allocator(4, 8);
  if(memory_pool==NULL){return 0;}

  int *four_bytes = (int*)memory_pool;
  int *another_four_bytes = (int*)memory_pool + 4;
  *four_bytes = 3;
  *another_four_bytes = 13;
  int sum = *four_bytes + *another_four_bytes;

  assert(sum == 16);

  return 1;
}

int test_release(){
  int success = (int)release_allocator();
  if(memory_pool != NULL){
    success = 0;
  }
  return success;
}
