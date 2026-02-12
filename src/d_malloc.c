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

#define MIN_BLOCK_SIZE (sizeof(block_t) + sizeof(footer_t) + 8) // Minimum block size

// Size class boundaries 
static const size_t size_classes[NUM_CLASSES] = 
{
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

footer_t *get_footer(block_t *block)
{
    return (footer_t *)((char *)block + block->size - sizeof(footer_t));
}

void set_footer(block_t *block)
{
    footer_t *footer = get_footer(block);
    footer->size = block->size;
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
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if  (ptr == MAP_FAILED)
    {
        return NULL;
    }
    
    // Update heap bounds
    if (heap_start == NULL)
    {
        heap_start = ptr;
    }
    heap_end = (char *)ptr + size;
    
    return ptr;
}

void *find_fit(size_t size) 
{
    if (size == 0)
    {
        return NULL;
    }

    size_t class = get_size_class(size);
    for (size_t i = class; i < NUM_CLASSES; i++)
    {
        block_t *current = free_lists[i];
        int search_limit = 100; // Limit search depth to avoid O(n) behavior
        
        while (current != NULL && search_limit > 0)
        {
            if (current->free && current->size >= size)
            {
                return current;
            }
            current = current->next;
            search_limit--;
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
    return aligned_size(size) + sizeof(block_t) + sizeof(footer_t); // Header + Footer
}

void split_block(block_t *block, size_t size)
{
    // Assumes block is NOT in free list
    size_t leftover = block->size - size;

    if (leftover >= MIN_BLOCK_SIZE) 
    {
        // Update original block size first
        block->size = size;
        set_footer(block);
        
        // Create new block from leftover space
        block_t *new_block = (block_t *)((char *)block + size);
        new_block->size = leftover;
        new_block->free = true;
        new_block->next = NULL;
        new_block->prev = NULL;
        set_footer(new_block);
        
        // Add the new free block to the appropriate free list
        add_to_free_list(new_block);
    }
}

void coalesce(block_t *block) 
{
    if (block == NULL)
    {
        return;
    }
    
    block_t *next = (block_t *)((char *)block + block->size);
    
    // Simple bounds check: only coalesce if next block is close
    // This avoids accessing unmapped memory between mmap regions
    if ((char *)next + sizeof(block_t) <= (char *)heap_end && next->free)
    {
        // Merge next block into current
        remove_from_free_list(next);
        block->size += next->size;
        set_footer(block);
    }
}

void add_to_free_list(block_t *block) 
{
    size_t currentSize = get_size_class(block->size);
    block->next = free_lists[currentSize];
    block->prev = NULL;
    
    if (free_lists[currentSize] != NULL) 
    {
        free_lists[currentSize]->prev = block;
    }

    free_lists[currentSize] = block;
}

void remove_from_free_list(block_t *block) 
{
    size_t currentSize = get_size_class(block->size);
    
    if (block->prev != NULL)
    {
        block->prev->next = block->next;
    }
    else  
    {
        free_lists[currentSize] = block->next;
    }

    if (block->next != NULL)
    {
        block->next->prev = block->prev;
    }

    block->next = NULL;
    block->prev = NULL;
}

// Main allocator functions
void *d_malloc(size_t size) 
{
    init_heap();

    if (size == 0)
    {
        return NULL;
    }

    size_t internalSize = internal_size(size);
    block_t *block = find_fit(internalSize);

    if (block != NULL)
    {
        // Remove from free list and mark as allocated
        remove_from_free_list(block);
        block->free = false;
        
        // Split if needed (adds remainder to free list)
        split_block(block, internalSize);
        
        // Update footer
        set_footer(block);
        
        return get_payload(block);
    }
    else 
    {
        // Allocate new heap space
        size_t allocSize = (internalSize > HEAP_INCREMENT) ? internalSize : HEAP_INCREMENT;
        void *ptr = extend_heap(allocSize);
        if (ptr == NULL)
        {
            return NULL;
        }
        
        block_t *new_block = (block_t *)ptr;
        new_block->size = allocSize;
        new_block->free = false;
        new_block->next = NULL;
        new_block->prev = NULL;
        set_footer(new_block);
        
        // Split if we allocated more than needed
        split_block(new_block, internalSize);
        
        return get_payload(new_block);
    }
}

void d_free(void *ptr) 
{
    if (ptr == NULL)
    {
        return;
    }

    block_t *block = get_header(ptr);
    block->free = true;
    coalesce(block);
    add_to_free_list(block);
}

void *d_realloc(void *ptr, size_t size) 
{
    // Edge case: ptr is NULL, act like malloc
    if (ptr == NULL)
    {
        return d_malloc(size);
    }
    
    // Edge case: size is 0, act like free
    if (size == 0)
    {
        d_free(ptr);
        return NULL;
    }
    
    block_t *block = get_header(ptr);
    size_t new_size = internal_size(size);
    
    // Same size, do nothing
    if (block->size == new_size)
    {
        return ptr;
    }

    // Shrinking
    if (block->size >= new_size)
    {
        split_block(block, new_size);
        return ptr;
    }

    // Growing - need to allocate new block
    void *new_ptr = d_malloc(size);
    if (new_ptr == NULL)
    {   
        return NULL;
    }
    
    // Copy minimum of old and new payload size
    size_t old_payload_size = block->size - sizeof(block_t);
    size_t copy_size = (old_payload_size < size) ? old_payload_size : size;
    memcpy(new_ptr, ptr, copy_size);
    d_free(ptr);
    return new_ptr;
}

void *d_calloc(size_t num, size_t size) 
{
    if (num != 0 && size > SIZE_MAX / num)
    {
        return NULL; // Prevent overflow
    }

    size_t total_size = num * size;
    void *ptr = d_malloc(total_size);
    if (ptr != NULL)
    {
        memset(ptr, 0, total_size);
    } 

    return ptr;
}

// ============================================================================
// VISUALIZATION AND DEBUG HELPERS
// ============================================================================

// Get statistics about allocator state (useful for visualization)
void d_get_stats(size_t *total_allocated, size_t *total_free, size_t *num_free_blocks)
{
    *total_allocated = 0;
    *total_free = 0;
    *num_free_blocks = 0;
    
    if (heap_start == NULL) {
        return;
    }
    
    // Count free blocks
    for (int i = 0; i < NUM_CLASSES; i++) {
        block_t *current = free_lists[i];
        while (current != NULL) {
            (*num_free_blocks)++;
            *total_free += current->size;
            current = current->next;
        }
    }
    
    // Calculate total heap size
    if (heap_end != NULL && heap_start != NULL) {
        size_t total_heap = (char *)heap_end - (char *)heap_start;
        *total_allocated = total_heap - *total_free;
    }
}

// Iterate through all free blocks in a size class (for visualization)
// Returns pointer to next block, or NULL if no more blocks
block_t *d_get_free_list(int size_class, block_t *current)
{
    if (size_class < 0 || size_class >= NUM_CLASSES) {
        return NULL;
    }
    
    if (current == NULL) {
        // Start of list
        return free_lists[size_class];
    } else {
        // Next block
        return current->next;
    }
}

// Get information about a block (for visualization)
void d_get_block_info(block_t *block, size_t *size, int *is_free, void **payload_ptr)
{
    if (block == NULL) {
        return;
    }
    
    if (size) *size = block->size;
    if (is_free) *is_free = block->free ? 1 : 0;
    if (payload_ptr) *payload_ptr = get_payload(block);
}

// Get heap bounds (for visualization)
void d_get_heap_bounds(void **start, void **end)
{
    if (start) *start = heap_start;
    if (end) *end = heap_end;
}
