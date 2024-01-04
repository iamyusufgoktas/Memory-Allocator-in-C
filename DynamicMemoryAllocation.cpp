#include <iostream>
// linear allocators and fixed size chunks allocator
using namespace std;

int main() {
    int n = 5;
    int *p;
    p = (int*)calloc(n,sizeof(int));
    // The malloc() function allocates memory and leaves the memory uninitialized,
    // whereas the calloc() function allocates memory and initializes all bits to zero.

    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    for (int i = 0; i < n; ++i) { // 0 1 2 3 4
        cout << p[i] << " ";
    }

    cout << endl;
    n = 10;
    p = (int*)realloc(p, n * sizeof(int));
    // resize of it

    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    for (int i = 0; i < n; ++i) { // 0 1 2 3 4 5 6 7 8 9
        cout << p[i] << " ";
    }



    free(p); // free memory
    return 0;
}
