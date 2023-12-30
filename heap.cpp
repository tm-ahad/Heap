#include <sys/mman.h>
#include <iostream>

#define CHUNK_SIZE 128

class Mem 
{
public:
    char mem[CHUNK_SIZE];
    uint8_t offset;
    Mem* next;

    Mem() 
    {
        this->offset = 0;
    }
};


class Heap 
{
    void* raw_mem;
    size_t size;
    Mem* mem;
    Mem* es;
    
public:
    Heap(size_t size) 
    {
        this->raw_mem = mmap
        (
            nullptr, 
            size, 
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, 
            -1, 
            0
        );
        
        if (this->raw_mem == MAP_FAILED) 
        {
            std::cerr << "Mapping memory failed.\n";
        }
        
        this->mem = (Mem*)this->raw_mem;
        this->mem->next = nullptr;

        this->es = this->mem;
        this->size = size;
    }

    template <typename ALLOC_T>
    ALLOC_T* mem_alloc() 
    {
        uint8_t alloc_size = sizeof(ALLOC_T);

        if (this->size <= sizeof(this->mem)+alloc_size) 
        {
            std::cerr << "Heap is filled. Can't store any more data\n";
        } else if (alloc_size > CHUNK_SIZE) 
        {
            std::cerr << "Cannot alloc more than  bytes for one ptr.\n";
        } else 
        {
            ALLOC_T* ptr = (ALLOC_T*)&this->es->mem;
            ptr += this->mem->offset;

            this->mem->offset += alloc_size;

            if (
                alloc_size == CHUNK_SIZE || 
                this->mem->offset >= CHUNK_SIZE
            ) {
                Mem* next = new Mem;
                this->es->next = next;
                this->es = this->es->next;
            }


            return ptr;      
        }
    }

    void mem_free(void* ptr) {
        //Unimplemented for now
    }

    ~Heap() {
        munmap(this->raw_mem, this->size);
    }
};
