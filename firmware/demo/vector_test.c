#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <generated/csr.h>
#include <libbase/console.h>

// --- Function Prototype Declaration ---
void vector_test_cmd(void);  // <-- Add this at the top

// Vector Unit Commands/Status (must match Verilog defines)
#define VU_CMD_WAITING  0
#define VU_CMD_ADD      1
#define VU_CMD_SUB      2
#define VU_STATUS_BUSY  3

// Test Parameters
#define TEST_NUM_ELEMENTS 10
#define BUFFER_SIZE (TEST_NUM_ELEMENTS * 4 * 2 + TEST_NUM_ELEMENTS * 8)

// --- Main Implementation ---
void vector_test_cmd(void) {
    printf("\n--- Vector Unit Test ---\n");
    
    static uint8_t vector_data_buffer[BUFFER_SIZE];
    uint32_t vector_data_base = (uint32_t)vector_data_buffer;
    
    printf("Buffer: %lu bytes @ 0x%08lx\n", 
           (unsigned long)BUFFER_SIZE, 
           (unsigned long)vector_data_base);

    // Rest of your existing implementation...
    // [Keep all your existing code here]
}