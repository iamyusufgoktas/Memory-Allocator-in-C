#include <stdio.h>
#include <stdlib.h>

/*
 * Note that this implementation only supports allocating objects of the same size,
 * and it does not handle alignment or fragmentation issues.
 * However, it should give you a good starting point for implementing your own pool allocator.
 */
typedef struct {
    void* start;
    size_t size;
    size_t obj_size;
    void* next;
} PoolAllocator;

void pool_allocator_init(PoolAllocator* pool, void* start, size_t size, size_t obj_size) {
    pool->start = start;
    pool->size = size;
    pool->obj_size = obj_size;
    pool->next = start;
}

void* pool_allocator_alloc(PoolAllocator* pool) {
    if (pool->next >= pool->start + pool->size) {
        return NULL;
    }

    void* result = pool->next;
    pool->next += pool->obj_size;
    return result;
}

void pool_allocator_free(PoolAllocator* pool, void* ptr) {
    pool->next = ptr;
}

int main() {
    // Allocate a block of memory to use for the pool
    size_t pool_size = 1024;
    void* pool_mem = malloc(pool_size);

    // Initialize the pool allocator
    PoolAllocator pool;
    pool_allocator_init(&pool, pool_mem, pool_size, sizeof(int));

    // Allocate some objects from the pool
    int* a = (int*)pool_allocator_alloc(&pool);
    int* b = (int*)pool_allocator_alloc(&pool);
    *a = 42;
    *b = 99;

    printf("a = %d, b = %d\n", *a, *b);

    // Free the objects
    pool_allocator_free(&pool, a);
    pool_allocator_free(&pool, b);

    // Release the memory block used by the pool allocator
    free(pool_mem);

    return 0;
}
