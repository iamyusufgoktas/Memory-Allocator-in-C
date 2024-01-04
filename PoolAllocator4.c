#include <stdio.h>
#include <stdlib.h>

typedef struct pool_allocator {
    char* memory_pool;
    size_t block_size;
    size_t num_blocks;
    unsigned char* free_blocks;
} pool_allocator;

pool_allocator* pool_allocator_create(size_t block_size, size_t num_blocks) {
    pool_allocator* allocator = malloc(sizeof(pool_allocator));
    allocator->memory_pool = malloc(block_size * num_blocks);
    allocator->block_size = block_size;
    allocator->num_blocks = num_blocks;
    allocator->free_blocks = malloc(num_blocks);
    for (size_t i = 0; i < num_blocks; i++) {
        allocator->free_blocks[i] = 1;
    }
    return allocator;
}

void* pool_allocator_alloc(pool_allocator* allocator) {
    for (size_t i = 0; i < allocator->num_blocks; i++) {
        if (allocator->free_blocks[i]) {
            allocator->free_blocks[i] = 0;
            return allocator->memory_pool + (i * allocator->block_size);
        }
    }
    return NULL;
}

void pool_allocator_free(pool_allocator* allocator, void* ptr) {
    if (ptr == NULL) {
        return;
    }
    size_t index = ((char*)ptr - allocator->memory_pool) / allocator->block_size;
    allocator->free_blocks[index] = 1;
}

void pool_allocator_destroy(pool_allocator* allocator) {
    free(allocator->free_blocks);
    free(allocator->memory_pool);
    free(allocator);
}

#define size 100
int main() {
    pool_allocator* allocator = pool_allocator_create(sizeof(int), 10);
    int* p1[size];

    p1[0]=(int*)pool_allocator_alloc(allocator);

    for (int i = 0; i < 10; ++i) {
        p1[0][i] = i;
    }

    pool_allocator_destroy(allocator);
    return 0;
}
