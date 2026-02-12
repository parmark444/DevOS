#include <stdio.h>
#include <sys/time.h>
#include "../include/d_malloc.h"

long get_usec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(void) {
    printf("Running sequential test (1000 iterations)...\n");
    void *ptrs[1000];
    long start = get_usec();
    
    for (int i = 0; i < 1000; i++) {
        // Allocate
        for (int j = 0; j < 1000; j++) {
            size_t size = (i * 37 + j * 13) % 4096 + 1;
            ptrs[j] = d_malloc(size);
        }
        
        // Free
        for (int j = 0; j < 1000; j++) {
            d_free(ptrs[j]);
        }
        
        if (i % 100 == 0) {
            printf("  Progress: %d/1000 iterations\n", i);
        }
    }
    
    long end = get_usec();
    printf("Completed in %ld μs\n", end - start);
    
    return 0;
}
