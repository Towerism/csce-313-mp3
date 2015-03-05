#include <assert.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "my_allocator.h"


int main() {
  //if an assert fails, it'll print and throw an abort
  assert(testInit() == 1);
  assert(testRelease() == 1);
  printf("Unit Tests have been run successfully");
  return 1;
}
