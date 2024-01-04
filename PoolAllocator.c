#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint-gcc.h>

typedef struct Chunk Chunk;

struct Chunk{
    Chunk *next;
};

size_t mChunksPerBlock;
Chunk *mAlloc = NULL;

Chunk *allocateBlock(size_t size)
{
    printf(" %zu ",mChunksPerBlock);

    Chunk *chunk_begin = (Chunk *) malloc(size);

    Chunk *curr_chunk = chunk_begin;

    for (int i = 0; i < mChunksPerBlock - 1; ++i) {
        curr_chunk->next = (Chunk *)((char *) (curr_chunk) + size);
        curr_chunk = (Chunk *) curr_chunk->next;
    }
    curr_chunk->next = NULL;

    return chunk_begin;
}

void *allocate(size_t size)
{
    if(mAlloc == NULL) mAlloc = allocateBlock(size);

    Chunk  *free_chunk = mAlloc;

    mAlloc = mAlloc->next;

    return free_chunk;
}
void deallocate(void *ptr, size_t size)
{
    ((Chunk *)ptr)->next = mAlloc;

    mAlloc = ptr;

};

void PoolAllocator_Init(size_t chunksPerBlock)
{
    mChunksPerBlock = chunksPerBlock;
}
struct Object {
    uint64_t data[2];
};


#define SIZE 10

int main(int argc, char* argv[])
{
    PoolAllocator_Init(8);

    struct Object *object[SIZE];

    for (int i = 0; i < SIZE; ++i) {
        object[i] = allocate(sizeof(struct Object));
        printf("%p \n",object[i]);
    }

    for (int i = 0; i < SIZE; ++i) {
        deallocate(object[i],sizeof(&object[i]));
    }





}
