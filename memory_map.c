/*
    File: memory_map.c

    Author: Martin Fracker & Christopher Findeisen
            Department of Computer Science
            Texas A&M University
    Date  : 03/10/15

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
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
/* FORWARDS */
/*--------------------------------------------------------------------------*/

static int calc_map_size(int bbs, int bc);
static void init_char_map(Memory_map* mm);
static int find_avail_position(Memory_map* mm, char c);
static int calc_char_offset(Memory_map* mm, char c);
static char calc_order_char(Memory_map* mm, int ord);
static void char_map_to_string(Memory_map* mm, char* res);

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MEMORY_MAP */
/*--------------------------------------------------------------------------*/
    
Memory_map* new_memory_map(short int bbs, int bc) {
    int ms = calc_map_size(bbs, bc);
    Memory_map* mem_map = (Memory_map*)malloc(sizeof(Memory_map) + ms);
    // set and test mem_map->memory_pool
    mem_map->basic_block_size = bbs;
    mem_map->high_order = floor(log2((double) ms));
    mem_map->byte_count = bc;
    mem_map->map_size = ms;
    memset(mem_map->char_map, '\0', ms);
    init_char_map(mem_map);
    return mem_map;
}

/*--------------------------------------------------------------------------*/
/* STATIC FUNCTIONS */
/*--------------------------------------------------------------------------*/

static int calc_map_size(int bbs, int bc) {
    if (bc % bbs != 0) {
        fprintf(stderr, "Error: Memory_map: Byte count must be a multiple of the basic block size.\n");
        exit(-1);
    }
    return bc / bbs;
}

static void init_char_map(Memory_map* mm) {
    int empty_block_count = mm->map_size;
    for(int order = mm->high_order; order >= 0; --order){
        //allocate as many high-order blocks as possible
        char order_char = calc_order_char(mm, order);
        // find the correct position
        int found = find_avail_position(mm, order_char);
        if (found != -1) {
            mm->char_map[found] = order_char;
            empty_block_count -= calc_char_offset(mm, order_char);
        } 
    }
}

static int find_avail_position(Memory_map* mm, char c) {
    int c_offset = calc_char_offset(mm, c);
    int pos = 0;
    while (pos < mm->map_size) {
        char current = mm->char_map[pos];
        if (current == '\0') {
            break;
        }
        int current_offset = calc_char_offset(mm, current);
        if (c_offset > current_offset) {
            pos += c_offset;
        } else {
            pos += current_offset;
        }
    }
    if (pos >= mm->map_size) {
        return -1;
    }
    return pos;
}

static int calc_char_offset(Memory_map* mm, char c) {
    int order = (int)'a' + mm->high_order - tolower(c);
    int offset = pow(2, order);
    return offset;
}

static char calc_order_char(Memory_map* mm, int ord) {
    char order_char = (int)'a' + mm->high_order - ord;
    return order_char;
}

static void char_map_to_string(Memory_map* mm, char* result) {
    int i;
    memset(result, 0, mm->map_size);
    for (i = 0; i < mm->map_size; ++i) {
        char c = mm->char_map[i];
        if (c == '\0') {
            continue;
        }
        strncat(result, &c, 1);
    }
}

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

int test_new_memory_map() {
    int success = 1;
    Memory_map* mem_map = new_memory_map(2, 128);
    success &= mem_map->basic_block_size == 2;
    success &= mem_map->high_order == 6;
    success &= mem_map->byte_count == 128;
    success &= mem_map->map_size == 64;
    return success;
}

int test_init_char_map() {
    int success = 1;
    Memory_map* mem_map1 = new_memory_map(2, 128);
    char str1[mem_map1->map_size];
    char_map_to_string(mem_map1, str1);
    success &= strcmp(str1, "a") == 0;
    Memory_map* mem_map2 = new_memory_map(2, 254);
    int i;
    char str2[mem_map2->map_size];
    char_map_to_string(mem_map2, str2);
    success &= strcmp(str2, "abcdefg") == 0;
    return success;
}
