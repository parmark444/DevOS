#define _GNU_SOURCE

#include "../include/d_malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

// Global variables
static block_t *free_lists[NUM_CLASSES] = {NULL};
static void *heap_start = NULL;
static void *heap_end = NULL;
static bool heap_initialized = false;

// Page size for mmap
#define PAGE_SIZE 4096
#define HEAP_INCREMENT (PAGE_SIZE * 4) // 16KB at a time

#define MIN_BLOCK_SIZE (sizeof(block_t) + 8) // Minimum block size to hold header and payload

// Size class boundaries 
static const size_t size_classes[NUM_CLASSES] = {
    32,      // Class 0: 1-32 bytes
    64,      // Class 1: 33-64 bytes
    128,     // Class 2: 65-128 bytes
    256,     // Class 3: 129-256 bytes
    512,     // Class 4: 257-512 bytes
    1024,    // Class 5: 513-1024 bytes
    2048,    // Class 6: 1025-2048 bytes
    SIZE_MAX // Class 7: 2049+ bytes (large blocks)
};

// Helper functions
block_t *get_header(void *ptr) 
{
    return (block_t *)((char *)ptr - sizeof(block_t));
}

void *get_payload(block_t *block)
{
    return (void *)((char *)block + sizeof(block_t));
}

size_t get_size_class(size_t size)
{
    for (int i = 0; i < NUM_CLASSES; i++)
    {
        if (size <= size_classes[i])
        {
            return i;
        }
    }
    return NUM_CLASSES - 1;  // Largest class
}

void init_heap(void)
{
    if (heap_initialized)
    {
        return;
    }
    heap_initialized = true;
    for (int i = 0; i < NUM_CLASSES; i++)
    {
        free_lists[i] = NULL;
    }
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
    if (size == 0)
    {
        return NULL;
    }

    size_t class = get_size_class(size);
    for (size_t i = class; i < NUM_CLASSES; i++)
    {
        block_t *current = free_lists[i];
        while (current != NULL)
        {
            if (current->free && current->size >= size)
            {
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

size_t aligned_size(size_t size) 
{
    return (size + 7) & ~0x7; // Align to 8 bytes
}

size_t internal_size(size_t size) 
{
    return aligned_size(size) + sizeof(block_t); // Account for block header
}

void place(block_t *block, size_t size) 
{
    // TODO: Implement block placement
    block->free = false;
    remove_from_free_list(block);
}

void split_block(block_t *block, size_t size)
{
    // TODO: Implement block splitting
    size_t leftover = block->size - size;

    if (leftover >= MIN_BLOCK_SIZE) 
    {
        block_t *new_block = (block_t *)((char *)block + size);
        new_block->size = leftover;
        new_block->free = true;
        block->size = size;
        add_to_free_list(new_block);
    }
    
    block->size = size;
    add_to_free_list(block);
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
