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
#include<stdlib.h>
#include "my_allocator.h"
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
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

/* Don't forget to implement "init_allocator" and "release_allocator"! */

void * memory_pool = NULL;
unsigned int b_b_s;

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

  else
    return _length;
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


extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the
     the C standard library!
     Of course this needs to be replaced by your implementation.
     */
  return malloc((size_t)_length);



}

extern int my_free(Addr _a) {
  /* Same here! */

  free(_a);
  return 0;

}


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
