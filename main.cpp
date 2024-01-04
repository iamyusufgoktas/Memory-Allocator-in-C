#include <iostream>
#include <assert.h>
using namespace std;
#define HEAP_CAP 640000
#define CHUNK_LIST_CAP 1024

typedef struct {
    uintptr_t * start;
    size_t size;
}Heap_Chunk;

typedef struct {
    size_t count;
    Heap_Chunk heapChunk[CHUNK_LIST_CAP];
}Chunk_List;

uintptr_t heap[HEAP_CAP] = {0};

void Chunk_List_Dump(const Chunk_List* list)
{
    cout << list->count << endl;

    for (size_t i = 0; i < list->count; ++i) {
        cout << &list->heapChunk[i].start << " " << list->heapChunk[i].size<<endl;
    }
}

void Chunk_List_Remove(Chunk_List *list, size_t ptr)
{
    assert(ptr < list->count);

    for (size_t i = ptr; i <list->count - 1; ++i) {
        list->heapChunk[i] = list->heapChunk[i+1];
    }
    list->count--;

}
void Chunk_List_Insert(Chunk_List *list,void *ptr, size_t ind)
{
    assert(list->count < CHUNK_LIST_CAP);
    list->heapChunk[list->count].start = static_cast<uintptr_t*>(ptr);
    list->heapChunk[list->count].size = ind;

    for (size_t i = list->count;
         i > 0 && list->heapChunk[i].start < list->heapChunk[i-1].start ;
         --i) {
        const Heap_Chunk t = list->heapChunk[i];
        list->heapChunk[i] = list->heapChunk[i-1];
        list->heapChunk[i-1] = t;
    }
    list->count ++;
}

void Chunk_List_Merge(Chunk_List* dest, const Chunk_List * src)
{
    dest->count = 0;
    for (size_t i = 0; i < src->count ; ++i) {
        const Heap_Chunk chunk = src->heapChunk[i];

        if(dest->count > 0)
        {
            Heap_Chunk * top_chunk = &dest->heapChunk[dest->count - 1];
            if(top_chunk->start + top_chunk->size == chunk.start)
            {
                top_chunk->size += chunk.size;
            }
            else
            {
                Chunk_List_Insert(dest,chunk.start,chunk.size);
            }
        }
        else {
            Chunk_List_Insert(dest,chunk.start,chunk.size);
        }
    }
}
int chunk_start_compar(const void *a, const void *b)
{
    const auto *a_chnk = static_cast<const Heap_Chunk *>(a);
    const auto *b_chnk = static_cast<const Heap_Chunk *>(b);

    return a_chnk->start - b_chnk->start;

}

int Chunk_List_Find(const Chunk_List *list, void *ptr)
{
    for (size_t i = 0; i < list->count; ++i) {
        if(list->heapChunk[i].start == ptr)
        {
            return (int)i;
        }
    }
    return -1;

    // binary search
/*    Heap_Chunk key = {
        .start = static_cast<char *>(ptr)
    };
    auto *r = static_cast<Heap_Chunk *>(bsearch(&key, list->heapChunk, list->count, sizeof(list->heapChunk[0]),
                                                      chunk_start_compar
    ));
    if(r != nullptr)
    {
        assert(list->heapChunk <= r);
        return (r - list->heapChunk) / sizeof (list->heapChunk[0]);
    }
    else
       return -1;*/
}



Chunk_List alloced_chunks = {0};
Chunk_List freed_chunks = {
        .count = 1,
        .heapChunk = {
                [0] = {.start = heap, .size = sizeof(heap)}
        },
};
Chunk_List tmp_chunk = {0};

void *Heap_Alloc(size_t bytes)
{
    const size_t size = (bytes + sizeof(uintptr_t)-1)/sizeof(uintptr_t);
    if(size > 0)
    {
        Chunk_List_Merge(&tmp_chunk,&freed_chunks);
        freed_chunks = tmp_chunk;
        for (size_t i = 0; i < freed_chunks.count; ++i) {
            const Heap_Chunk chunk= freed_chunks.heapChunk[i];
            if(chunk.size >= size)
            {
                Chunk_List_Remove(&freed_chunks,i);
                const size_t tail = chunk.size - size;
                Chunk_List_Insert(&alloced_chunks,chunk.start,size);

                if(tail > 0)
                {
                    Chunk_List_Insert(&freed_chunks,chunk.start + size, tail);
                }
                return chunk.start;
            }
        }
    }

    return nullptr;
}

void Heap_Free(void *ptr)
{
    if(ptr != nullptr)
    {
        const int ind = Chunk_List_Find(&alloced_chunks,ptr);
        assert(ind >= 0);
        assert(ptr == alloced_chunks.heapChunk[ind].start);
        Chunk_List_Insert(&freed_chunks,alloced_chunks.heapChunk[ind].start,
                          alloced_chunks.heapChunk[ind].size);
        Chunk_List_Remove(&alloced_chunks,(size_t) ind);
    }

}
void Heap_Collect()
{
    assert(false && "COLLECT NOT IMPLEMENTED");
}

#define N 10
void *ptr[N] = {0};
int main()
{
    for (size_t i = 0; i < N; ++i) {
        ptr[i] = Heap_Alloc(i);
    }
    for (size_t i = 0; i < N; ++i) {
        if(i % 2 == 0)
        {
            Heap_Free(ptr[i]);
        }
    }

    Heap_Alloc(10);

    Chunk_List_Dump(&alloced_chunks);
    Chunk_List_Dump(&freed_chunks);



//    Heap_Free(ro)
    return 0;
}
