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
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include "my_allocator.h"
#include "memory_map.h"

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

void* memory_pool = NULL;
Memory_map* mem_map = NULL;
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

    memory_pool = malloc(_length);
    b_b_s = _basic_block_size;
    mem_map = new_memory_map(b_b_s, _length, memory_pool);
    if(errno)
        return 0;

    else {
        return _length;
    }
}

int release_allocator(){
    if(memory_pool == NULL) {
        return 0;
    }

    free(memory_pool);
    delete_memory_map(mem_map);
    return 1;
}

extern Addr my_malloc(unsigned int _length) {
    // add offset to be sizeof(Memory_map) + mm->map_size + 1;
    Addr free_block = get_block(mem_map, _length);
    return free_block;
}

extern int my_free(Addr _a) {

    release_block(mem_map, _a);
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
    if(memory_pool == NULL){
        success = 0;
    }
    return success;
}
