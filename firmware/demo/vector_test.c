#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> // For malloc/free if dynamic allocation is preferred
#include <string.h> // For memset/memcpy

#include <libbase/console.h> // For busy_wait
#include <generated/csr.h>   // For CSR_WB_VECTOR_UNIT_BASE macro and individual CSRs

// --- Prototype for vector_test_cmd ---
void vector_test_cmd(void);

// --- Vector Unit Register Structure Definition ---
// This C structure maps directly to the memory-mapped registers of your wbVectorUnit.
// The addresses (offsets) are automatically managed by LiteX.
// The names here (e.g., "control", "mem_base_addr") must match the names given to CSRStorage/CSRStatus in wbVectorUnit.py.
typedef volatile struct {
    uint32_t control;        // Corresponds to self.control in Python
    uint32_t mem_base_addr;  // Corresponds to self.mem_base_addr in Python
    uint32_t num_elements;   // Corresponds to self.num_elements in Python
    uint32_t status;         // Corresponds to self.status in Python
    uint32_t irq_status;     // Corresponds to self.irq_status in Python
    // LiteX will automatically pad/order these based on allocation.
    // We access them via CSR_WB_VECTOR_UNIT_CONTROL_ADDR etc., so the struct
    // is mainly for conceptual grouping. Direct CSR access is more robust.
} wb_vector_unit_regs_t;

// --- Control Register Commands/States (from Verilog) ---
#define VU_CMD_WAITING  0
#define VU_CMD_ADD      1
#define VU_CMD_SUB      2
#define VU_STATUS_BUSY  3 // VU sets this when busy

// --- Memory Layout Definitions (relative to the base address provided to VU) ---
#define VU_OP_A_OFFSET(i) ((i) * 4)
#define VU_OP_B_OFFSET(num_elements, i) ((num_elements) * 4 + (i) * 4)
#define VU_OUT_LSB_OFFSET(num_elements, i) ((num_elements) * 4 * 2 + (i) * 8)
#define VU_OUT_MSB_OFFSET(num_elements, i) ((num_elements) * 4 * 2 + (i) * 8 + 4)

// --- Main Vector Unit Test Function ---
#define TEST_NUM_ELEMENTS 10

void vector_test_cmd(void) {
    printf("\n--- Starting Vector Unit Test ---\n");

    const size_t BUFFER_SIZE = TEST_NUM_ELEMENTS * 4 * 2 + TEST_NUM_ELEMENTS * 8;

    static uint8_t vector_data_buffer[BUFFER_SIZE];
    uint32_t vector_data_base_addr = (uint32_t)&vector_data_buffer[0];
    (void)vector_data_buffer; // Suppress unused variable warning

    printf("Allocated vector data buffer of %lu bytes at 0x%08lX\n",
           (unsigned long)BUFFER_SIZE, vector_data_base_addr);
    printf("Testing with %u elements.\n", TEST_NUM_ELEMENTS);

    uint32_t *op_a_ptr = (uint32_t *)(vector_data_base_addr + VU_OP_A_OFFSET(0));
    uint32_t *op_b_ptr = (uint32_t *)(vector_data_base_addr + VU_OP_B_OFFSET(TEST_NUM_ELEMENTS, 0));
    uint64_t *out_ptr = (uint64_t *)(vector_data_base_addr + VU_OUT_LSB_OFFSET(TEST_NUM_ELEMENTS, 0));

    memset(vector_data_buffer, 0, BUFFER_SIZE);

    printf("Populating Operand A and B...\n");
    for (uint32_t i = 0; i < TEST_NUM_ELEMENTS; i++) {
        op_a_ptr[i] = i + 100;
        op_b_ptr[i] = i + 1;
        printf("  OpA[%u] = 0x%08lX, OpB[%u] = 0x%08lX\n", i, (unsigned long)op_a_ptr[i], i, (unsigned long)op_b_ptr[i]);
    }

    printf("Configuring Vector Unit via CSRs...\n");

    // Set number of elements using LiteX-generated CSR write function
    wb_vector_unit_num_elements_write(TEST_NUM_ELEMENTS);
    printf("  Set num_elements to %u (readback: %u)\n",
           TEST_NUM_ELEMENTS, wb_vector_unit_num_elements_read());

    // Set memory base address using LiteX-generated CSR write function
    wb_vector_unit_mem_base_addr_write(vector_data_base_addr);
    printf("  Set mem_base_addr to 0x%08lX (readback: 0x%08lX)\n",
           (unsigned long)vector_data_base_addr, (unsigned long)wb_vector_unit_mem_base_addr_read());

    printf("Triggering ADD operation...\n");
    // Trigger ADD operation using LiteX-generated CSR write function
    wb_vector_unit_control_write(VU_CMD_ADD);
    printf("  Control_cmd written. Current status: %u (expecting %u - BUSY)\n",
           wb_vector_unit_status_read(), VU_STATUS_BUSY);

    printf("Polling for Vector Unit to complete operation...\n");
    // Poll the status register using LiteX-generated CSR read function
    while (wb_vector_unit_status_read() == VU_STATUS_BUSY) {
        busy_wait(100);
    }
    printf("Vector Unit: Operation finished (polled status: %u).\n", wb_vector_unit_status_read());

    printf("Verifying results...\n");
    uint32_t errors = 0;
    for (uint32_t i = 0; i < TEST_NUM_ELEMENTS; i++) {
        uint64_t expected_result = (uint64_t)(op_a_ptr[i]) + (uint64_t)(op_b_ptr[i]);
        uint64_t actual_result = out_ptr[i];

        printf("  Out[%u]: OpA=0x%08lX + OpB=0x%08lX => Read 0x%016llX (Expected 0x%016llX)\n",
               i, (unsigned long)op_a_ptr[i], (unsigned long)op_b_ptr[i], actual_result, expected_result);

        if (actual_result != expected_result) {
            printf("    MISMATCH! Expected 0x%016llX, Got 0x%016llX\n", expected_result, actual_result);
            errors++;
        }
    }

    if (errors == 0) {
        printf("--- Vector Unit Test PASSED! ---\n");
    } else {
        printf("--- Vector Unit Test FAILED with %u errors! ---\n", errors);
    }
    printf("----------------------------------\n");
}