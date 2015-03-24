#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "my_allocator.h"
#include "memory_map.h"

int main() {
  // if an assert fails, it'll print and throw an abort

  // test allocation functions
  assert(test_init() == 1);
  assert(test_release() == 1);

  // test memory map functions
  assert(test_new_memory_map() == 1);
  assert(test_init_char_map() == 1);
  assert(test_get_block() == 1);
  assert(test_release_block() == 1);
  /*assert(test_split() == 1);
  assert(test_coalesce() == 1);*/
  return 1;
}
