#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../include/d_malloc.h"

/* Benchmark tests for custom d_malloc vs standard malloc created using Claude Sonnet 4.5 */

#define NUM_ITERATIONS 10000
#define MAX_ALLOC_SIZE 4096

// Get current time in microseconds
long get_usec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

// Test 1: Sequential allocations and frees
void test_sequential(int use_custom) {
    long start = get_usec();
    void *ptrs[1000];
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Allocate
        for (int j = 0; j < 1000; j++) {
            size_t size = (i * 37 + j * 13) % MAX_ALLOC_SIZE + 1;
            if (use_custom) {
                ptrs[j] = d_malloc(size);
            } else {
                ptrs[j] = malloc(size);
            }
        }
        
        // Free
        for (int j = 0; j < 1000; j++) {
            if (use_custom) {
                d_free(ptrs[j]);
            } else {
                free(ptrs[j]);
            }
        }
    }
    
    long end = get_usec();
    printf("Sequential (10M alloc/free): %ld μs\n", end - start);
}

// Test 2: Random allocation pattern
void test_random(int use_custom) {
    long start = get_usec();
    void *ptrs[1000] = {NULL};
    
    for (int i = 0; i < NUM_ITERATIONS * 10; i++) {
        int idx = rand() % 1000;
        
        // Free if occupied
        if (ptrs[idx] != NULL) {
            if (use_custom) {
                d_free(ptrs[idx]);
            } else {
                free(ptrs[idx]);
            }
            ptrs[idx] = NULL;
        }
        
        // Allocate with 70% probability
        if (rand() % 10 < 7) {
            size_t size = rand() % MAX_ALLOC_SIZE + 1;
            if (use_custom) {
                ptrs[idx] = d_malloc(size);
            } else {
                ptrs[idx] = malloc(size);
            }
        }
    }
    
    // Cleanup
    for (int i = 0; i < 1000; i++) {
        if (ptrs[i] != NULL) {
            if (use_custom) {
                d_free(ptrs[i]);
            } else {
                free(ptrs[i]);
            }
        }
    }
    
    long end = get_usec();
    printf("Random pattern (100K ops):   %ld μs\n", end - start);
}

// Test 3: Small allocations (cache-like behavior)
void test_small_allocs(int use_custom) {
    long start = get_usec();
    
    for (int i = 0; i < NUM_ITERATIONS * 100; i++) {
        size_t size = (i % 64) + 1; // 1-64 bytes
        void *ptr;
        if (use_custom) {
            ptr = d_malloc(size);
            d_free(ptr);
        } else {
            ptr = malloc(size);
            free(ptr);
        }
    }
    
    long end = get_usec();
    printf("Small allocs (1M x 1-64B):   %ld μs\n", end - start);
}

// Test 4: Large allocations
void test_large_allocs(int use_custom) {
    long start = get_usec();
    
    for (int i = 0; i < 1000; i++) {
        size_t size = 1024 * 1024; // 1MB
        void *ptr;
        if (use_custom) {
            ptr = d_malloc(size);
            d_free(ptr);
        } else {
            ptr = malloc(size);
            free(ptr);
        }
    }
    
    long end = get_usec();
    printf("Large allocs (1000 x 1MB):   %ld μs\n", end - start);
}

// Test 5: Realloc performance
void test_realloc(int use_custom) {
    long start = get_usec();
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        void *ptr;
        if (use_custom) {
            ptr = d_malloc(16);
            ptr = d_realloc(ptr, 64);
            ptr = d_realloc(ptr, 256);
            ptr = d_realloc(ptr, 1024);
            ptr = d_realloc(ptr, 128);
            d_free(ptr);
        } else {
            ptr = malloc(16);
            ptr = realloc(ptr, 64);
            ptr = realloc(ptr, 256);
            ptr = realloc(ptr, 1024);
            ptr = realloc(ptr, 128);
            free(ptr);
        }
    }
    
    long end = get_usec();
    printf("Realloc (50K sequences):     %ld μs\n", end - start);
}

// Test 6: Calloc performance
void test_calloc(int use_custom) {
    long start = get_usec();
    
    for (int i = 0; i < NUM_ITERATIONS * 10; i++) {
        void *ptr;
        if (use_custom) {
            ptr = d_calloc(100, 50);
            d_free(ptr);
        } else {
            ptr = calloc(100, 50);
            free(ptr);
        }
    }
    
    long end = get_usec();
    printf("Calloc (100K x 5KB):         %ld μs\n", end - start);
}

int main(void) {
    srand(12345); // Fixed seed for reproducibility
    
    printf("=== Memory Allocator Benchmark ===\n\n");
    
    printf("Standard malloc:\n");
    test_sequential(0);
    test_random(0);
    test_small_allocs(0);
    test_large_allocs(0);
    test_realloc(0);
    test_calloc(0);
    
    printf("\nCustom d_malloc:\n");
    srand(12345); // Reset for fair comparison
    test_sequential(1);
    test_random(1);
    test_small_allocs(1);
    test_large_allocs(1);
    test_realloc(1);
    test_calloc(1);
    
    printf("\n=== Benchmark Complete ===\n");
    
    return 0;
}
