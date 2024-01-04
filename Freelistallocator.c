#include <stdio.h>
#include <stdlib.h>
#define MEM_CAP 1024

char memory[MEM_CAP];

typedef struct mem_block
{
    int size;
    struct mem_block* next;
}mem_block_t;

mem_block_t* freelist = NULL;

void init()
{
    freelist = (mem_block_t*)memory;
    freelist->size = MEM_CAP -sizeof(mem_block_t);
    freelist->next = NULL;
}

void* Allocate_Memory(int size)
{
    mem_block_t* curr = freelist;
    mem_block_t* prev = NULL;

    while(curr != NULL)
    {
        if(curr->size >= size)
        {
            if(prev == NULL)
                freelist = curr->next;
            else
                prev->next = curr->next;
            return ((void*) curr) + sizeof(mem_block_t);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

// Deallocate memory by returning the block to the free list
void deallocate_memory(void* ptr) {
    mem_block_t* current = (mem_block_t*) (((char*) ptr) - sizeof(mem_block_t));
    current->next = freelist;
    freelist = current;
}

int main()
{
    init();

    int* arr = (int*) Allocate_Memory(10 * sizeof (int));

    for (int i = 0; i < 11; ++i) {
        arr[i] = i;
        printf("%i %p \n",i,&arr[i]);
    }
    deallocate_memory(arr);

    arr = (int*) Allocate_Memory(10 * sizeof (int));

    for (int i = 0; i < 11; ++i) {
        arr[i] = i;
        printf("%i %p \n",i,&arr[i]);
    }


}
