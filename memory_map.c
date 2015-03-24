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
#include <assert.h>
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

// epsilon represents a basic block in the memory map which is a
// part of a larger block
static const char eps = '-';
typedef enum {EMPTY, FREE, RESERVED, BUDDY} search_type;
/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

static int calc_map_size(int bbs, int bc);
static void init_char_map(Memory_map* mm);
static int find_candidate_position(Memory_map* mm, char c, search_type st);
static int char_to_order(Memory_map* mm, char c);
static int calc_char_offset(Memory_map* mm, char c);
static char calc_order_char(Memory_map* mm, int ord);
static void abbrev_char_map(Memory_map* mm, char* dest);
static void print_char_map(Memory_map* mm);
/* static int  calc_offset_map(Memory_map* mm, int i); */
static Addr map_pos_to_addr(Memory_map* mm, int i);
static int addr_to_map_pos(Memory_map* mm, Addr a);
static void coalesce(Memory_map* mm, int ord);

// splits a block of memory
// returns the index of the left buddy if successful
// returns -1 otherwise
static int split(Memory_map* mm, int ord);

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MEMORY_MAP */
/*--------------------------------------------------------------------------*/

Memory_map* new_memory_map(short int bbs, int bc, Addr mp) {
    int ms = calc_map_size(bbs, bc);
    // add one for the null byte at the end of char_map
    Memory_map* mem_map = (Memory_map*)malloc(sizeof(Memory_map) + ms + 1 );
    mem_map->memory_pool = mp;
    mem_map->basic_block_size = bbs;
    mem_map->high_order = floor(log2((double) ms));
    mem_map->byte_count = bc;
    mem_map->map_size = ms;
    init_char_map(mem_map);
    return mem_map;
}

// It is idiomatic to have a matching delete function
// Also easier to maintain
void delete_memory_map(Memory_map* mm) {
    free(mm);
}


Addr get_block(Memory_map* mm, int bs) {
    //get lowest order free block that can contain bs
    if (bs > mm->byte_count){
        return NULL;
    }
    int block_count = ceil((double)bs / mm->basic_block_size);
    Addr free_block_loc = NULL;
    // consider using basic_block_size in this calculation
    int ord = ceil(log2( block_count ));
    char c = calc_order_char(mm, ord);
    int index = find_candidate_position(mm, c, FREE);
    assert(index < 8000);
    if (index == -1) {
        index = split(mm, ord + 1);
    }
    if (index != -1) {
        char reserved_c = toupper(mm->char_map[index]);
        mm->char_map[index] = reserved_c;
        free_block_loc = map_pos_to_addr(mm, index);
    }
    print_char_map( mm);
    return free_block_loc;
}

int release_block(Memory_map* mm, Addr addr) {

    int pos = addr_to_map_pos(mm, addr);

    if (pos < 0 || pos >= mm->map_size) {
        fprintf(stderr, "Error: Release_block: invalid release location\n");
        return 0;
    }

    char c  = mm->char_map[pos];
    if (isupper(c)) {
        mm->char_map[pos] = tolower(c);
        coalesce(mm, char_to_order(mm, c));
        print_char_map(mm);
        return 1;
    }
    return 0; //wasn't allocated
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
    memset(mm->char_map, '-', mm->map_size);
    mm->char_map[mm->map_size] = '\0'; // set null byte at the end of map
    int order;
    for (order = mm->high_order; order >= 0; --order) {
        //allocate as many high-order blocks as possible
        char order_char = calc_order_char(mm, order);
        // find the correct position
        int found = find_candidate_position(mm, order_char, EMPTY);

    assert(found < 8000);
        if (found != -1) {
            mm->char_map[found] = order_char;
        }
    }
}

static int find_candidate_position(Memory_map* mm, char c, search_type st) {
    int c_offset = calc_char_offset(mm, c);
    int pos = 0;
    while (pos < mm->map_size) {

    assert(pos < 8000);
        char current = mm->char_map[pos];
        // break statements are needed here
        // otherwise cases fall through when the if-conditions are false
        switch (st) {
            case EMPTY:
                if (current == eps)
                    return pos;
                break;
            case FREE:
                if (current == tolower(c))
                    return pos;
                break;
            case RESERVED:
                if (current == toupper(c))
                    return pos;
                break;
            case BUDDY:
                //c_offset == order^2
                if ((pos / c_offset) % 2 == 0) {//we have a left buddy
                    char left = current;
                    if (pos + c_offset >= mm->map_size)
                        break;
                    char right = mm->char_map[pos + c_offset];
                    if (islower(left) && left == right) {
                        return pos;
                    }
                }
                break;
        }

        int current_offset = calc_char_offset(mm, current);
        if (c_offset > current_offset)
            pos += c_offset;
        else
            pos += current_offset;
    }
    return -1;
}

static int char_to_order(Memory_map* mm, char c) {
    /* printf("Char_to_order call: a: %d high_order: %d c: %d\n", (int)'a' , mm->high_order , tolower(c)); */
    int order = (int)'a' + mm->high_order - tolower(c);
    return order;
}

static int calc_char_offset(Memory_map* mm, char c) {
    int order = char_to_order(mm, c);
    int offset = pow(2, order);
    return offset;
}

static char calc_order_char(Memory_map* mm, int ord) {
    char order_char = (int)'a' + mm->high_order - ord;
    return order_char;
}

// Copies an abbreviated char_map into the dest string
// Note that the more we split blocks of memory, the less
// we are able to abbreviate
static void abbrev_char_map(Memory_map* mm, char* dest) {
    int i;
    memset(dest, 0, mm->map_size);
    for (i = 0; i < mm->map_size; ++i) {
    assert(i < 8000);
        char c = mm->char_map[i];
        if (c == eps) {
            continue;
        }
        strncat(dest, &c, 1);
    }
}

static void print_char_map(Memory_map* mm) {
    char str1[mm->map_size];
    abbrev_char_map(mm, str1);
    printf("Current Char Map: %s \n", str1);
}

static Addr map_pos_to_addr(Memory_map* mm, int i) {
    int offset = i * mm->basic_block_size;
    Addr calculated_addr = mm->memory_pool + offset;
    return calculated_addr;
}

static int addr_to_map_pos(Memory_map* mm, Addr a) {
    int offset =   a - mm->memory_pool ;
    int i = offset / mm->basic_block_size;
    return i;
}

//implement buddy finder by checking if char_map(pos/2^order) %2 == 0 then we we
//have left buddy, otherwise we have right buddy
static void coalesce(Memory_map* mm, int ord) {
    while (1) {
        char c = calc_order_char(mm, ord);
        int pos = find_candidate_position(mm, c, BUDDY);
        if (pos != -1) {
            char* left = mm->char_map + pos;
            char* right = mm->char_map + pos + calc_char_offset(mm, c);
            *left = calc_order_char(mm, ord + 1);
            *right = eps;
        } else {
            break;
        }
    }
    if (ord < mm->high_order) {
        coalesce(mm, ord + 1);
    }
}

static int split(Memory_map* mm, int ord) {
    if (ord > mm->high_order) {
        return -1;
    }
    int pos = find_candidate_position(mm, calc_order_char(mm, ord), FREE);
    if (pos == -1) {
        pos = split(mm, ord + 1);
    }
    char* left = mm->char_map + pos;
    char* right = mm->char_map + pos + (int)pow(2, ord - 1);
    char lower_order = calc_order_char(mm, ord - 1);
    *left = lower_order;
    *right = lower_order;
    return pos;
}

/*--------------------------------------------------------------------------*/
/* UnitTests */
/*--------------------------------------------------------------------------*/

int test_new_memory_map() {
    int success = 1;
    Addr memory = (Addr)malloc(128);
    Memory_map* mem_map = new_memory_map(2, 128, memory);
    success &= mem_map->basic_block_size == 2;
    success &= mem_map->high_order == 6;
    success &= mem_map->byte_count == 128;
    success &= mem_map->map_size == 64;
    success &= mem_map->memory_pool == memory;
    delete_memory_map(mem_map);
    return success;
}

int test_init_char_map() {
    int success = 1;
    Addr memory1 = (Addr)malloc(128);
    Memory_map* mem_map1 = new_memory_map(2, 128, memory1);
    char str1[mem_map1->map_size];
    abbrev_char_map(mem_map1, str1);
    success &= strcmp(str1, "a") == 0;

    Addr memory2 = (Addr)malloc(254);
    Memory_map* mem_map2 = new_memory_map(2, 254, memory2);
    int i;
    char str2[mem_map2->map_size];
    abbrev_char_map(mem_map2, str2);
    success &= strcmp(str2, "abcdefg") == 0;
    delete_memory_map(mem_map1);
    delete_memory_map(mem_map2);
    return success;
}

int test_get_block() {
    int success = 1;
    Addr memory1 = (Addr)malloc(128);
    Memory_map* mem_map1 = new_memory_map(2, 128, memory1);
    Addr block_15 = get_block(mem_map1, 15);

    char str1[mem_map1->map_size];
    abbrev_char_map(mem_map1, str1);
    success &= strcmp(str1, "Ddcb") == 0;

    Addr memory2 = (Addr)malloc(254);
    Memory_map* mem_map2 = new_memory_map(2, 254, memory2);

    char str2[mem_map2->map_size];
    block_15 = get_block(mem_map2, 15);
    Addr block_4 = get_block(mem_map2, 4);
    Addr block_200 = get_block(mem_map2, 200);

    abbrev_char_map(mem_map2, str2);
    printf("%s\n", str2);
    success &= strcmp(str2, "abcDeFg") == 0;
    success &= block_200 == NULL; //too big should fail

    delete_memory_map(mem_map1);
    delete_memory_map(mem_map2);
    return success;
}


int test_release_block() {
    int success = 1;
    Addr memory1 = (Addr)malloc(2048);
    Memory_map* mem_map1 = new_memory_map(2, 2048, memory1);
    char str1[mem_map1->map_size];
    Addr blocks[mem_map1->map_size];
    //gets
    abbrev_char_map(mem_map1, str1);
    success &= strcmp(str1, "a") == 0;
    for(int i=0; i<mem_map1->map_size; ++i){
        blocks[i] = get_block(mem_map1, 1);
        /* printf("Giving you block # %d \n", i); */
        assert(blocks[i] != NULL);
    }
    abbrev_char_map(mem_map1, str1);
    //check j's
    for(int i=0; i<mem_map1->map_size; ++i){
        /* printf("Got %c \n", str1[i]); */
        assert(str1[i]=='K');
    }

    //release
        printf("Releasing..\n");
    for(int i=0; i<mem_map1->map_size; ++i){
        release_block(mem_map1, blocks[i]);
    }
        printf("Done..\n");
    abbrev_char_map(mem_map1, str1);
    success &= (strcmp(str1, "a") == 0);
    return success;

}

/* int test_release_block() { */
/*     int success = 1; */
/*     Addr memory1 = (Addr)malloc(254); */
/*     Memory_map* mem_map1 = new_memory_map(2, 254, memory1); */

/*     char str1[mem_map1->map_size]; */
/*     Addr block_15 = get_block(mem_map1, 15); */
/*     Addr block_4 = get_block(mem_map1, 4); */
/*     abbrev_char_map(mem_map1, str1); */
/*     success &= strcmp(str1, "abCdEfg") == 0; */
/*     release_block(mem_map1, block_15); */
/*     release_block(mem_map1, block_4); */
/*     success &= (release_block(mem_map1, memory1) == 0);//should fail */
/*     abbrev_char_map(mem_map1, str1); */
/*     success &= strcmp(str1, "abcdefg") == 0; */

/*     Addr memory2 = (Addr)malloc(128); */
/*     Memory_map* mem_map2 = new_memory_map(2, 128, memory2); */
/*     block_15 = get_block(mem_map2, 15); */

/*     char str2[mem_map2->map_size]; */
/*     abbrev_char_map(mem_map2, str2); */
/*     success &= strcmp(str2, "Ccb") == 0; */

/*     release_block(mem_map2, block_15); */
/*     abbrev_char_map(mem_map2, str2); */
/*     success &= strcmp(str2, "a") == 0; */

/*     delete_memory_map(mem_map1); */
/*     delete_memory_map(mem_map2); */
/*     return success; */
/* } */
/*
   int test_coalesce() {
   int success = 1;
   Addr memory1 = malloc(128);
   Memory_map* mem_map1 = new_memory_map(2, 128, memory1);
   Addr block_ = get_block
   char str1[mem_map1->map_size];
   coalesce(mem_map1, 3);
   abbrev_char_map(mem_map1, str1);
//success &= strcmp(str1, "ccb") == 0;
return success;
}

int test_split() {
int success = 1;
Addr memory1 = malloc(128);
Memory_map* mem_map1 = new_memory_map(2, 128, memory1);
split(mem_map1, 1);
char str1[mem_map1->map_size];
abbrev_char_map(mem_map1, str1);
success &= strcmp(str1, "ggfedcb") == 0;
return success;
}*/
