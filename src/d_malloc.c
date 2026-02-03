#define _GNU_SOURCE

#include "../include/d_malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

// Global variables
static block_t *free_list_head = NULL;
static void *heap_start = NULL;
static void *heap_end = NULL;
static bool heap_initialized = false;

// Page size for mmap
#define PAGE_SIZE 4096
#define HEAP_INCREMENT (PAGE_SIZE * 4) // 16KB at a time

// Helper functions
block_t *get_header(void *ptr) 
{
    return (block_t *)((char *)ptr - sizeof(block_t));
}

void *get_payload(block_t *block)
 {
    return (void *)((char *)block + sizeof(block_t));
}

void init_heap(void) 
{
    if (heap_initialized) 
    {
        return;
    }
    heap_initialized = true;
    free_list_head = NULL;
}

void *extend_heap(size_t size) 
{
    // TODO: Implement mmap allocation
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if  (ptr == MAP_FAILED)
    {
        cerror("mmap failed in extend_heap\n");
        return NULL;
    }
    return NULL;
}

void *find_fit(size_t size) 
{
    // TODO: Implement first-fit search
    return NULL;
}

void place(block_t *block, size_t size) {
    // TODO: Implement block placement
}

void split_block(block_t *block, size_t size) {
    // TODO: Implement block splitting
}

void coalesce(block_t *block) {
    // TODO: Implement coalescing
}

void add_to_free_list(block_t *block) {
    // TODO: Implement adding to free list
}

void remove_from_free_list(block_t *block) {
    // TODO: Implement removing from free list
}

// Main allocator functions
void *d_malloc(size_t size) {
    // TODO: Implement malloc
    return NULL;
}

void d_free(void *ptr) {
    // TODO: Implement free
}

void *d_realloc(void *ptr, size_t size) {
    // TODO: Implement realloc
    return NULL;
}

void *d_calloc(size_t num, size_t size) {
    // TODO: Implement calloc
    return NULL;
}
