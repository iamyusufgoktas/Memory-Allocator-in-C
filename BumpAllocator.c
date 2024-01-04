#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define KB(size) ((size_t) size * 1024)
#define MB(size) (KB(size) * 1024)
#define GB(size) (MB(size) * 1024)
#define HEAP_SIZE GB(1)

#define HEAP_CAP KB(1)

typedef struct
{
    uintptr_t heap[HEAP_CAP];
    uintptr_t* curr_ptr;
}Heap;

Heap Allocated_chunk =
        {
            .heap = {(uintptr_t) NULL},
            .curr_ptr = Allocated_chunk.heap,
        };


void* Bump_Allocator_Alloc(size_t size)
{
    assert(Allocated_chunk.curr_ptr + size <= Allocated_chunk.heap + HEAP_CAP);

    void* result = Allocated_chunk.curr_ptr;
    Allocated_chunk.curr_ptr += size;
    return result;

}
// this allocator doesn't allow specific positions of memory to be freed
void Bump_Allocator_Free()
{
    Allocated_chunk.curr_ptr = Allocated_chunk.heap;
}


#define N 10

int main()
{
//    ptr = (int *) malloc(N*sizeof(int *));
    int *b = (int *) malloc(N*sizeof(int *));
    int *a = (int *) Bump_Allocator_Alloc(N*sizeof(int));

    assert(sizeof(a) == sizeof(b)); // ok
    a[0] = 31;

    for (int i = 0; i < N; ++i) {
        printf(" %d ",*(a+i));
    }
    //  31  0  0  0  0  0  0  0  0  0

    char *foo =(char *) Bump_Allocator_Alloc(N*sizeof(char *));

    char * f = "hello world!";
    for (int i = 0; i < 12; ++i) {
        foo[i] = f[i];
    }
    printf("%s",foo); // hello world!

    Bump_Allocator_Free();

    return 0;
}