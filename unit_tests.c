#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "my_allocator.h"
#include "linked_list.h"

int main() {
  // if an assert fails, it'll print and throw an abort
  
  // test allocation functions
  assert(test_init() == 1);
  assert(test_release() == 1);

  // test linked list functions
  assert(test_list_push() == 1);
  assert(test_list_pop() == 1);
  assert(test_list_remove() == 1);
  printf("Unit Tests have been run successfully\n");
  return 1;
}
