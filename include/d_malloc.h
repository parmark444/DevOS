#ifndef D_MALLOC_H
#define D_MALLOC_H 

#include <stddef.h>
#include <stdbool.h>

#define NUM_CLASSES 8

typedef struct block_header 
{
    size_t size;            // Total size including header and footer
    struct block_header* next;
    struct block_header* prev;
    bool free;
} block_t;

typedef struct block_footer
{
    size_t size;            // Same as header size
} footer_t;

// Main allocator functions
void *d_malloc(size_t size);
void d_free(void *ptr);
void *d_realloc(void *ptr, size_t size);
void *d_calloc(size_t num, size_t size);

// Helper functions
void init_heap(void);
void *extend_heap(size_t size);
void *find_fit(size_t size);
void split_block(block_t *block, size_t size);
void coalesce(block_t *block);
void add_to_free_list(block_t *block);
void remove_from_free_list(block_t *block);
block_t *get_header(void *ptr);
void *get_payload(block_t *block);
footer_t *get_footer(block_t *block);
void set_footer(block_t *block);
size_t get_size_class(size_t size);

// Visualization and debug helpers, added for later display capabilities
void d_get_stats(size_t *total_allocated, size_t *total_free, size_t *num_free_blocks);
block_t *d_get_free_list(int size_class, block_t *current);
void d_get_block_info(block_t *block, size_t *size, int *is_free, void **payload_ptr);
void d_get_heap_bounds(void **start, void **end);

#endif // D_MALLOC_H