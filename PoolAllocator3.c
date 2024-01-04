#include <stdlib.h>
#include <stdio.h>
/*
 * Here, pool_allocator_create initializes a new pool allocator by allocating memory for num_blocks
 * blocks of size block_size, and then sets up the free list by linking each block together.
 * pool_allocator_alloc returns the next available block from the free list, and pool_allocator_free
 * adds a block back to the free list. Finally, pool_allocator_destroy frees the memory allocated
 * for the pool allocator.
 */

typedef struct PoolAllocator {
    size_t block_size;
    size_t num_blocks;
    void* data;
    void* free_list;
} PoolAllocator;

PoolAllocator* pool_allocator_create(size_t block_size, size_t num_blocks) {
    printf("block size :%zu , num blocks: %zu \n",block_size,num_blocks);

    PoolAllocator* pool = malloc(sizeof(PoolAllocator));
    pool->block_size = block_size;
    pool->num_blocks = num_blocks;
    size_t total_size = block_size * num_blocks;
    pool->data = malloc(total_size);

    printf("size -> %lu  \n",sizeof(pool->data));

    pool->free_list = NULL;
    for (size_t i = 0; i < num_blocks; i++) {
        char* block = (char*)pool->data + i * block_size;
        *(void**)block = pool->free_list;
        pool->free_list = block;
//        printf("%d -> %p \n",i,pool->free_list);
    }

    return pool;
}

void* pool_allocator_alloc(PoolAllocator* pool) {
    if (!pool->free_list) {
        return NULL;
    }
    void* block = pool->free_list;
    pool->free_list = *(void**)block;
    return block;
}

void pool_allocator_free(PoolAllocator* pool, void* block) {
    *(void**)block = pool->free_list;
    pool->free_list = block;
}

void pool_allocator_destroy(PoolAllocator* pool) {
    free(pool->data);
    free(pool);
}

int main() {
    PoolAllocator* pool = pool_allocator_create(sizeof(int*), 10);

    int* ptr1 = (int*)pool_allocator_alloc(pool);
    ptr1[0] = 31;
    int* ptr2 = (int*)pool_allocator_alloc(pool);
    ptr2[0] = 31;
    printf("%p \n",pool->free_list);
    printf("%p \n",ptr1);
    printf("%p \n",ptr2);
    printf("%d \n",ptr1[0]);

    pool_allocator_free(pool,ptr1);
    printf("\n");
    printf("%p \n",pool->free_list);
    printf("%p \n",ptr1);
    printf("%p \n",ptr2);
    printf("%d \n",ptr1[0]);

    int* ptr3 = (int*)pool_allocator_alloc(pool);
    printf("\n");
    printf("%p \n",pool->free_list);
    printf("%p \n",ptr1);
    printf("%p \n",ptr2);
    printf("%p \n",ptr3);
    printf("%d \n",ptr1[0]);

/*
    *ptr1 = 42;
    for (int i = 0; i < 10; ++i) {
        ptr1[i] = i;
    }
    for (int i = 0; i < 10; ++i) {
        printf("ptr3: %d, value: %p\n", ptr1[i], &ptr1[i]);
    }


    printf("->ptr3: %p, value: %d\n", ptr1, *ptr1);

    pool_allocator_free(pool, ptr1);
*/
    pool_allocator_destroy(pool);


    return 0;
}


