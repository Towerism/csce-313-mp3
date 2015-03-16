#include "ackerman.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)

  init_allocator(4, 4*1000000);

  ackerman_main();

  release_allocator();
}
