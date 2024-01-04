#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_SIZE 100

typedef struct StackAllocator {
    char *start;
    char *end;
    char *current;
} StackAllocator;

void stack_init(StackAllocator *sa, char *buffer, size_t size) {
    sa->start = buffer;
    sa->end = sa->start + size;
    sa->current = sa->start;
}

void *stack_alloc(StackAllocator *sa, size_t size) {
    void *ptr = NULL;
    if (sa->current + size <= sa->end) {
        ptr = sa->current;
        sa->current += size;
    }
    return ptr;
}

void stack_free(StackAllocator *sa, void *ptr, size_t size) {
    // We don't actually free any memory, but we can update the current pointer
    // to pretend that the memory is freed and available for reuse.
    printf("----------------- \n");

    printf("%p \n",(void*)sa->current);
    if (ptr == sa->current - size) {
        sa->current -= size;

    }
    printf("%p \n",(void*)sa->current);
    printf("----------------- \n");


}

int main() {
    char buffer[STACK_SIZE];
    StackAllocator sa;

    stack_init(&sa, buffer, STACK_SIZE);

    int *p1 = (int *) stack_alloc(&sa, sizeof(int));
    char *p2 = (char *) stack_alloc(&sa, sizeof(char) * 5);

    if (p1 && p2) {
        *p1 = 42;
        p2[0] = 'H';
        p2[1] = 'e';
        p2[2] = 'l';
        p2[3] = 'l';
        p2[4] = 'o';

        printf("%d %s\n", *p1, p2);

        stack_free(&sa, p2, sizeof(char) * 5);
        stack_free(&sa, p1, sizeof(int));

        // These variables should no longer be used after freeing them
        p1 = NULL;
        p2 = NULL;
    } else {
        printf("Error: Unable to allocate memory.\n");
    }

    return 0;
}
