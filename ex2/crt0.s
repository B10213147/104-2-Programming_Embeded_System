/* Date:   20.3.2008   */
/* Author: Janez Puhan */

/* Parameters */
             /* User program key is valid if it contains the 2's complement of   */
             /* the checksum of the remaining interrupt vectors. This causes the */
             /* checksum of all the vectors together to be zero.                 */
           .equ    user_key,            0xb4405f52 /* user program key            */
                                                   /* 32-byte space for IAP       */
           .equ    und_stack,           0x40007fe0 /* undefined mode stack bottom */
           .equ    svc_stack,           0x40007ee0 /* supervisor mode stack bot.  */
           .equ    abt_stack,           0x40007de0 /* abort mode stack bottom     */
           .equ    irq_stack,           0x40007ce0 /* irq mode stack bottom       */
           .equ    fiq_stack,           0x40007be0 /* fiq mode stack bottom       */
           .equ    usr_stack,           0x40007ae0 /* user mode stack bottom      */

/* Constants */
           .equ    flash,               0x00000000 /* flash address           */
           .equ    ram,                 0x40000000 /* RAM address             */
           .equ    ram_end_table,       0x40000040 /* end table address       */
           .equ    word_len,            0x04       /* word length in bytes    */
           .equ    und_m,               0x1b       /* undefined mode in cpsr  */
           .equ    svc_m,               0x13       /* supervisor mode in cpsr */
           .equ    abt_m,               0x17       /* abort mode in cpsr      */
           .equ    irq_m,               0x12       /* irq mode in cpsr        */
           .equ    fiq_m,               0x11       /* irq mode in cpsr        */
           .equ    usr_m,               0x10       /* user mode in cpsr       */
           .equ    disable_ints,        0xffffffff /* disable interrupts      */
           .equ    prot_val,            0x00       /* protection value        */
           .equ    remap_val,           0x02       /* re-map value            */

/* Registers */
           .equ    memmap,              0xe01fc040 /* memory mapping control */
           .equ    vicintenclear,       0xfffff014 /* interrupt clear        */
           .equ    vicprotection,       0xfffff020 /* protection             */

           .code   32                   /* compile for ARM 32-bit instruction set */

/* Program code */
           .text   0

/* Exception or interrupt vector table */
           b       reset                           /* reset                  */
           ldr     pc, =und                        /* undefined instruction  */
           ldr     pc, =swi                        /* software interrupt     */
           ldr     pc, =pref_abt                   /* prefetch abort         */
           ldr     pc, =data_abt                   /* data abort             */
           .long   user_key                        /* user program key       */
           ldr     pc, =irq                        /* interrupt request      */
           ldr     pc, =fiq                        /* fast interrupt request */

           .long   0x00000000                      /* place holder */
           .long   0x00000000

           .text   1

/* Reset interrupt service routine */
reset:     ldr     r0, =flash                      /* copy inperrupt vector      */
           ldr     r1, =ram                        /* table to RAM               */
           ldr     r2, =ram_end_table
           bl      copy
           ldr     r0, =_codesrc                   /* copy the code to RAM       */
           ldr     r1, =_code
           ldr     r2, =_ecode
           bl      copy
           ldr     r0, =_datasrc                   /* copy initialised data      */
           ldr     r1, =_data
           ldr     r2, =_edata
           bl      copy
           mov     r0, #0x00                       /* clear bss section          */
           ldr     r1, =_bss
           ldr     r2, =_ebss
clear_bss: cmp     r1, r2
           strlo   r0, [r1], #word_len
           blo     clear_bss
           ldr     r0, =vicintenclear              /* disable all interrupts     */
           ldr     r1, =disable_ints
           str     r1, [r0]
           ldr     r0, =vicprotection              /* enable config in user mode */
           mov     r1, #prot_val
           str     r1, [r0]
           ldr     r0, =memmap                     /* re-map interrupt table     */
           mov     r1, #remap_val
           str     r1, [r0]
           msr     cpsr_c, #und_m                  /* switch to undefined mode   */
           ldr     sp, =und_stack                  /* initialise stack pointer   */
           msr     cpsr_c, #svc_m                  /* switch to supervisor mode  */
           ldr     sp, =svc_stack                  /* initialise stack pointer   */
           msr     cpsr_c, #abt_m                  /* switch to abort mode       */
           ldr     sp, =abt_stack                  /* initialise stack pointer   */
           msr     cpsr_c, #irq_m                  /* switch to irq mode         */
           ldr     sp, =irq_stack                  /* initialise stack pointer   */
           msr     cpsr_c, #fiq_m                  /* switch to fiq mode         */
           ldr     sp, =fiq_stack                  /* initialise stack pointer   */
           msr     cpsr_c, #usr_m                  /* switch to user mode        */
           ldr     sp, =usr_stack                  /* initialise stack pointer   */
           ldr     pc, =start_up                   /* start the program          */

copy:      cmp     r1, r2                          /* copy from r0 to r1 until r2 */
           ldrlo   r3, [r0], #word_len             /* is reached                  */
           strlo   r3, [r1], #word_len
           blo     copy
           mov     pc, lr                          /* return from subroutine      */

           .end                                    /* stop compiling */
