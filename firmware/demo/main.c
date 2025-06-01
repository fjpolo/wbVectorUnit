// This file is Copyright (c) 2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Conditional includes for interrupt handling
#ifdef CONFIG_CPU_HAS_INTERRUPT
#include <irq.h>           // For irq_setmask, irq_setie, irq_pending, irq_getmask
#endif

#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h> // For CSR access macros and interrupt definitions

// Declare the new test function
void vector_test_cmd(void);

/*-----------------------------------------------------------------------*/
/* Uart                                                                  */
/*-----------------------------------------------------------------------*/

static char *readstr(void)
{
    char c[2];
    static char s[64];
    static int ptr = 0;

    if(readchar_nonblock()) {
        c[0] = getchar();
        c[1] = 0;
        switch(c[0]) {
            case 0x7f:
            case 0x08:
                if(ptr > 0) {
                    ptr--;
                    fputs("\x08 \x08", stdout);
                }
                break;
            case 0x07:
                break;
            case '\r':
            case '\n':
                s[ptr] = 0x00;
                fputs("\n", stdout);
                ptr = 0;
                return s;
            default:
                if(ptr >= (sizeof(s) - 1))
                    break;
                fputs(c, stdout);
                s[ptr] = c[0];
                ptr++;
                break;
        }
    }

    return NULL;
}

static char *get_token(char **str)
{
    char *c, *d;

    c = (char *)strchr(*str, ' ');
    if(c == NULL) {
        d = *str;
        *str = *str+strlen(*str);
        return d;
    }
    *c = 0;
    d = *str;
    *str = c+1;
    return d;
}

static void prompt(void)
{
    printf("\e[92;1mlitex-demo-app\e[0m> ");
}

/*-----------------------------------------------------------------------*/
/* Help                                                                  */
/*-----------------------------------------------------------------------*/

static void help(void)
{
    puts("\nLiteX minimal demo app built "__DATE__" "__TIME__"\n");
    puts("Available commands:");
    puts("help             - Show this command");
    puts("reboot           - Reboot CPU");
#ifdef CSR_LEDS_BASE
    puts("led              - Led demo");
#endif
    puts("donut            - Spinning Donut demo");
    puts("helloc           - Hello C");
#ifdef WITH_CXX
    puts("hellocpp         - Hello C++");
#endif
    puts("vector           - Tests Vector Unit module"); // Added
}

/*-----------------------------------------------------------------------*/
/* Commands                                                              */
/*-----------------------------------------------------------------------*/

static void reboot_cmd(void)
{
    ctrl_reset_write(1);
}

#ifdef CSR_LEDS_BASE
static void led_cmd(void)
{
    int i;
    printf("Led demo...\n");

    printf("Counter mode...\n");
    for(i=0; i<32; i++) {
        leds_out_write(i);
        busy_wait(100);
    }

    printf("Shift mode...\n");
    for(i=0; i<4; i++) {
        leds_out_write(1<<i);
        busy_wait(200);
    }
    for(i=0; i<4; i++) {
        leds_out_write(1<<(3-i));
        busy_wait(200);
    }

    printf("Dance mode...\n");
    for(i=0; i<4; i++) {
        leds_out_write(0x55);
        busy_wait(200);
        leds_out_write(0xaa);
        busy_wait(200);
    }
}
#endif

extern void donut(void);

static void donut_cmd(void)
{
    printf("Donut demo...\n");
    donut();
}

extern void helloc(void);

static void helloc_cmd(void)
{
    printf("Hello C demo...\n");
    helloc();
}

#ifdef WITH_CXX
extern void hellocpp(void);

static void hellocpp_cmd(void)
{
    printf("Hello C++ demo...\n");
    hellocpp();
}
#endif

/*-----------------------------------------------------------------------*/
/* Console service / Main                                                */
/*-----------------------------------------------------------------------*/

static void console_service(void)
{
    char *str;
    char *token;

    str = readstr();
    if(str == NULL) return;
    token = get_token(&str);
    if(strcmp(token, "help") == 0) {
        help();
    } else if(strcmp(token, "reboot") == 0) {
        reboot_cmd();
    }
#ifdef CSR_LEDS_BASE
    else if(strcmp(token, "led") == 0) {
        led_cmd();
    }
#endif
    else if(strcmp(token, "donut") == 0) {
        donut_cmd();
    } else if(strcmp(token, "helloc") == 0) {
        helloc_cmd();
    }
#ifdef WITH_CXX
    else if(strcmp(token, "hellocpp") == 0) {
        hellocpp_cmd();
    }
#endif
    else if(strcmp(token, "vector") == 0) {
        vector_test_cmd();
    }
    prompt();
}

// --- Interrupt Dispatch (Conditional compilation) ---
// This entire block is now conditionally compiled.
// For SERV CPU, CONFIG_CPU_HAS_INTERRUPT is typically NOT defined.
#ifdef CONFIG_CPU_HAS_INTERRUPT
void isr(void) {
    uint32_t irqs = irq_pending() & irq_getmask();

    if (irqs & VECTOR_UNIT_IRQ_VECTOR_UNIT_IRQ) {
        // If you still want an interrupt handler for the vector unit,
        // vu_irq_handler should be defined elsewhere (e.g., in vector_test.c)
        // and its prototype included (if CONFIG_CPU_HAS_INTERRUPT is defined).
        // However, for SERV, polling is generally preferred.
        // vu_irq_handler(); // Uncomment if you add an actual handler and it's needed
    }
    // Add other interrupt handlers here if needed
}
#endif

int main(void)
{
#ifdef CONFIG_CPU_HAS_INTERRUPT
    irq_setmask(0);
    irq_setie(1);
#endif
    uart_init();

    help();
    prompt();

    while(1) {
        console_service();
    }

    return 0;
}