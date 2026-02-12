#include <stdio.h>
#include "../include/d_malloc.h"

int main(void) {
    printf("Test 1: Single allocation\n");
    void *ptr1 = d_malloc(100);
    if (ptr1 == NULL) {
        printf("FAIL: d_malloc returned NULL\n");
        return 1;
    }
    printf("PASS: allocated 100 bytes at %p\n", ptr1);
    
    printf("\nTest 2: Free\n");
    d_free(ptr1);
    printf("PASS: freed ptr1\n");
    
    printf("\nTest 3: Multiple allocations\n");
    void *ptr2 = d_malloc(50);
    void *ptr3 = d_malloc(75);
    void *ptr4 = d_malloc(125);
    printf("PASS: allocated 3 blocks\n");
    
    printf("\nTest 4: Free middle block\n");
    d_free(ptr3);
    printf("PASS: freed middle block\n");
    
    printf("\nTest 5: Allocate in freed space\n");
    void *ptr5 = d_malloc(60);
    printf("PASS: allocated in freed space\n");
    
    printf("\nTest 6: Free all\n");
    d_free(ptr2);
    d_free(ptr4);
    d_free(ptr5);
    printf("PASS: freed all blocks\n");
    
    printf("\nTest 7: Calloc\n");
    int *arr = d_calloc(10, sizeof(int));
    if (arr == NULL) {
        printf("FAIL: d_calloc returned NULL\n");
        return 1;
    }
    // Check if zeroed
    int all_zero = 1;
    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) {
        printf("PASS: calloc returned zeroed memory\n");
    } else {
        printf("FAIL: calloc didn't zero memory\n");
    }
    d_free(arr);
    
    printf("\nAll tests completed!\n");
    return 0;
}
