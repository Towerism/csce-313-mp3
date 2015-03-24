#include "ackerman.h"

int main(int argc, char ** argv) {

  // input parameters (basic block size, memory length)

  init_allocator(128, 1024000);

  ackerman_main();

  release_allocator();
}
