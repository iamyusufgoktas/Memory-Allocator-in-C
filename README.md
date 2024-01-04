# Memory Allocator in C for fun(fun)

### Include
 * Dynamic file
   * malloc recalloc calloc free implementation
 * In main.cpp 
   * it is malloc implementation in C 
 * Pool allocator
 * free list(basic)
 * stack allocator

### Bump allocator(linear allocator)
  * Linearly allocate 
  * This implementation uses a statically allocated buffer
 as the memory pool, and a pointer to keep track 
of the current position in the pool.
  * This allocator uses a simple bump-pointer allocation strategy, which simply moves a pointer forward for each allocation request. It does not perform any memory management, such as garbage collection or compaction. 
 
