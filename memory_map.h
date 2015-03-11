/*
    File: memory_map.h

    Author: Martin Fracker & Christopher Findeisen
            Department of Computer Science
            Texas A&M University
    Date  : 03/10/15

*/

#ifndef _memory_map_h_                   // include file only once
#define _memory_map_h_

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

typedef struct Memory_map {
    short int basic_block_size;
    short int high_order;
    Addr memory_pool;
    int byte_count;
    int map_size;
    char char_map[0];
} Memory_map;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MEMORY_MAP */
/*--------------------------------------------------------------------------*/

// Constructs a new memory map with a basic block size and a byte count
Memory_map* new_memory_map(short int bbs, int bc);

// Destructs the memory map and frees dynamically allocated memory
// Needs to be tested
void delete_memory_map(Memory_map* mm);

// Returns the address of a free block given the block size 
// returns **NULL** if no free block is found with given block size
// the block returned is no longer free
Addr get_block(Memory_map* mm, int bs);

// Releases a block with the given address
// Returns 1 if the release was successful, 0 otherwise
int release_block(Memory_map* mm, Addr addr);

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

int test_new_memory_map();
int test_init_char_map();

#endif
