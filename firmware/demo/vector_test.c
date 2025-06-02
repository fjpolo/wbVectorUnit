#include <stdio.h>
#include <stdint.h>
#include <generated/csr.h> // This will now correctly define WB_VECTOR_UNIT_BASE (from soc.h)
#include <hw/common.h>   // For csr_read_simple, csr_write_simple

// --- Function Prototype Declaration ---
void vector_test_cmd(void); // Add this line to resolve compiler warning

void vector_test_cmd(void) {
    printf("\n--- Extended Wishbone Vector Unit Test (Manual Wishbone Slave) ---\n");

    uint32_t read_val;
    uint32_t write_val_status = 0xAABBCCDD;
    uint32_t write_val_control = 0x11223344;
    uint32_t write_val_address = 0xFFEEAA55;

    // --- Test status_reg (Offset 0x00) ---
    printf("\nTesting status_reg (Offset 0x00, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to status_reg (0x%08lx)...\n", (unsigned long)write_val_status, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x00));
    csr_write_simple(write_val_status, WB_VECTOR_UNIT_BASE + 0x00);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + 0x00);
    printf("Read 0x%08lx from status_reg. %s\n", (unsigned long)read_val,
           (read_val == write_val_status) ? "PASSED" : "FAILED");


    // --- Test control_register (Offset 0x04) ---
    printf("\nTesting control_register (Offset 0x04, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to control_register (0x%08lx)...\n", (unsigned long)write_val_control, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x04));
    csr_write_simple(write_val_control, WB_VECTOR_UNIT_BASE + 0x04);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + 0x04);
    printf("Read 0x%08lx from control_register. %s\n", (unsigned long)read_val,
           (read_val == write_val_control) ? "PASSED" : "FAILED");

    // --- Test address_register (Offset 0x08) ---
    printf("\nTesting address_register (Offset 0x08, Base 0x%08lx):\n", (unsigned long)WB_VECTOR_UNIT_BASE);
    printf("Writing 0x%08lx to address_register (0x%08lx)...\n", (unsigned long)write_val_address, (unsigned long)(WB_VECTOR_UNIT_BASE + 0x08));
    csr_write_simple(write_val_address, WB_VECTOR_UNIT_BASE + 0x08);
    read_val = csr_read_simple(WB_VECTOR_UNIT_BASE + 0x08);
    printf("Read 0x%08lx from address_register. %s\n", (unsigned long)read_val,
           (read_val == write_val_address) ? "PASSED" : "FAILED");

    printf("\n--- Wishbone Vector Unit Test Complete ---\n");
}