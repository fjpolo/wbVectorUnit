#include <stdio.h>
#include <stdint.h>
#include <generated/csr.h> 
#include <generated/soc.h> 
#include <hw/common.h>

#define WB_VU_STATUS_REG_OFFSET           0x00
#define WB_VU_CONTROL_REG_OFFSET          0x04
#define WB_VU_ADDRESS_REG_OFFSET          0x08
#define WB_VU_ELEMENTS_REG_OFFSET         0x0C
#define WB_VU_COUNT_REG_OFFSET            0x10


// --- Function Prototype Declaration ---
void vector_test_cmd(void); // Add this line to resolve compiler warning

void vector_test_cmd(void) {
    printf("\n--- Extended Wishbone Vector Unit Test (Manual Wishbone Slave) ---\n");

    uint32_t read_val;
    const uint32_t write_val_status  = 0xAABBCCDD;
    const uint32_t write_val_control = 0x11223344;
    const uint32_t write_val_address = 0xFFEEAA55;
    const uint32_t write_val_element_count = 32;
//     const uint32_t write_val_counter = 0xA5A5A5A5; // A distinct value for testing


    // --- Test status_reg (Offset 0x00) ---
    printf("\nTesting status_reg (Offset 0x00, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to status_reg (0x%08lx)...\n", (unsigned long)write_val_status, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x00));
    csr_write_simple(write_val_status, WB_VECTOR_UNIT_BASE + WB_VU_STATUS_REG_OFFSET);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + WB_VU_STATUS_REG_OFFSET);
    printf("Read 0x%08lx from status_reg. %s\n", (unsigned long)read_val,
           (read_val == write_val_status) ? "PASSED" : "FAILED");

    // --- Test control_register (Offset 0x04) ---
    printf("\nTesting control_register (Offset 0x04, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to control_register (0x%08lx)...\n", (unsigned long)write_val_control, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x04));
    csr_write_simple(write_val_control, WB_VECTOR_UNIT_BASE + WB_VU_CONTROL_REG_OFFSET);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + WB_VU_CONTROL_REG_OFFSET);
    printf("Read 0x%08lx from control_register. %s\n", (unsigned long)read_val,
           (read_val == write_val_control) ? "PASSED" : "FAILED");

    // --- Test address_register (Offset 0x08) ---
    printf("\nTesting address_register (Offset 0x08, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to address_register (0x%08lx)...\n", (unsigned long)write_val_address, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x08));
    csr_write_simple(write_val_address, WB_VECTOR_UNIT_BASE + WB_VU_ELEMENTS_REG_OFFSET);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + WB_VU_ELEMENTS_REG_OFFSET);
    printf("Read 0x%08lx from address_register. %s\n", (unsigned long)read_val,
           (read_val == write_val_address) ? "PASSED" : "FAILED");

    // --- Test element_count_register (Offset 0x0C) ---
    printf("\nTesting element_count_register (Offset 0x0C, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to element_count_register (0x%08lx)...\n", (unsigned long)write_val_element_count, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x08));
    csr_write_simple(write_val_element_count, WB_VECTOR_UNIT_BASE + WB_VU_ADDRESS_REG_OFFSET);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + WB_VU_ADDRESS_REG_OFFSET);
    printf("Read 0x%08lx from element_count_register. %s\n", (unsigned long)read_val,
           (read_val == write_val_element_count) ? "PASSED" : "FAILED");

    // --- Test counter_register (Offset 0x10) ---
    printf("\nTesting counter_register (Offset 0x10, Base 0x%08lx - Continuous Increment):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("  Reading counter_register 5 times with 100ms delay to observe increment:\n");
    for (int i = 0; i < 10; i++) {
        read_val = csr_read_simple(0x83000010);
        printf("    Read %d: 0x%08lx\n", i + 1, (unsigned long)read_val);
        cdelay(100000); // Wait 100 milliseconds (100,000 microseconds)
    }

    printf("\n--- Wishbone Vector Unit Test Complete ---\n");
}