#ifndef D_MALLOC_H
#define D_MALLOC_H 

#include <stddef.h>
#include <stdbool.h>

#define NUM_CLASSES 8

typedef struct block_header 
{
    size_t size;
    struct block_header* next;
    struct block_header* prev;
    bool free;
} block_t;

// Main allocator functions
void *d_malloc(size_t size);
void d_free(void *ptr);
void *d_realloc(void *ptr, size_t size);
void *d_calloc(size_t num, size_t size);

// Helper functions
void init_heap(void);
void *extend_heap(size_t size);
void *find_fit(size_t size);
void place(block_t *block, size_t size);
void split_block(block_t *block, size_t size);
void coalesce(block_t *block);
void add_to_free_list(block_t *block);
void remove_from_free_list(block_t *block);
block_t *get_header(void *ptr);
void *get_payload(block_t *block);
int get_size_class(size_t size);

#endif // D_MALLOC_H