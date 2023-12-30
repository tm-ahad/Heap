#include <iostream>
#include "heap.cpp"

int main() {
    Heap heap(4096);
    int* ptr = heap.mem_alloc<int>();
    int* ptr2 = heap.mem_alloc<int>();

    *ptr = 12312;
    *ptr2 = 1;

    printf("ptr1: Addr: %p Value: %d;\n", ptr, *ptr);
    printf("ptr2: Addr: %p Value: %d;\n", ptr2, *ptr2);
    return 0;
}
