/* Date:   14.11.2011  */
/* Author: Janez Puhan */
/* for assembler       */

/* Parameters */
             /* User program key is valid if it contains the 2's complement of    */
             /* the checksum of the remaining interrupt vectors. This causes the  */
             /* checksum of all the vectors together to be zero.                  */
           .equ    user_key,            0x93fffffe /* user program key            */
                                                   /* 32-byte space for IAP       */
           .equ    und_stack,           0x40007fe0 /* undefined mode stack bottom */
           .equ    svc_stack,           0x40007ee0 /* supervisor mode stack bot.  */
           .equ    abt_stack,           0x40007de0 /* abort mode stack bottom     */
           .equ    irq_stack,           0x40007ce0 /* irq mode stack bottom       */
           .equ    fiq_stack,           0x40007be0 /* fiq mode stack bottom       */
           .equ    usr_stack,           0x40007ae0 /* user mode stack bottom      */
             /* vpbdiv_val = 0x00 ... pclk = cclk / 4                             */
             /* vpbdiv_val = 0x01 ... pclk = cclk                                 */
             /* vpbdiv_val = 0x02 ... pclk = cclk / 2                             */
           .equ    vpbdiv_val,          0x00       /* VPB divider value           */
             /* 10MHz  <= fosc <= 25MHz  ... external oscillator (12MHz)          */
             /* 10MHz  <= cclk <= 60MHz  ... processor clock                      */
             /* 156MHz <= fcco <= 320MHz ... current controlled oscillator        */
             /* cclk = m * fosc                                                   */
             /* fcco = 2 * m * p * fosc                                           */
             /* msel = 0x00 ... m = 1                                             */
             /* msel = 0x01 ... m = 2                                             */
             /*      .                                                            */
             /*      .                                                            */
             /* msel = 0x1f ... m = 32                                            */
           .equ    msel,                0x00       /* PLL msel value (pllcfg)     */
             /* psel = 0x00 ... p = 1                                             */
             /* psel = 0x01 ... p = 2                                             */
             /* psel = 0x02 ... p = 4                                             */
             /* psel = 0x03 ... p = 8                                             */
           .equ    psel,                0x03       /* PLL psel value (pllcfg)     */
             /* mam_cyc = 0x01 if 10MHz <= cclk < 20MHz (do not use with WinIDEA) */
             /* mam_cyc = 0x02 if 20MHz <= cclk < 40MHz                           */
             /* mam_cyc = 0x03 if 40MHz <= cclk <= 60MHz                          */
           .equ    mam_cyc,             0x02       /* MAM fetch cycles            */
             /* mam_en = 0x00 ... MAM functions disabled                          */
             /* mam_en = 0x01 ... MAM functions partially enabled                 */
             /* mam_en = 0x02 ... MAM functions fully enabled                     */
           .equ    mam_en,              0x02       /* MAM enable mode             */

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
           .equ    mam_dis,             0x00       /* MAM disable constant    */
           .equ    pllcon_dis,          0x00       /* disable/disconnect PLL  */
           .equ    pll_feed_byte1,      0xaa       /* PLL feed sequence       */
           .equ    pll_feed_byte2,      0x55
           .equ    msel_len,            0x05       /* length of msel in bits  */
           .equ    plle,                0x01       /* PLL enable (pllcon)     */
           .equ    pllc,                0x02       /* PLL connect (pllcon)    */
           .equ    plock,               0x0400     /* PLL lock bit in pllstat */
           .equ    gpio,                0x00       /* general purpose in/out  */
           /* Older S-Arm boards have LEDs from p0.8 to p0.11                 */                
           .equ  out_pins,           0x00000f00    /* output pins mask        */
           /* Newer S-Arm boards have LEDs from p0.4 to p0.7                  */                
           /*.equ    out_pins,          0x000000f0  output pins mask        */

/* Registers */
           .equ    io0dir,              0xe0028008 /* GPIO direction         */
           .equ    io0clr,              0xe002800c /* GPIO output clear      */
           .equ    pinsel0,             0xe002c000 /* pin function select    */
           .equ    mamcr,               0xe01fc000 /* MAM control            */
           .equ    mamtim,              0xe01fc004 /* MAM timing             */
           .equ    memmap,              0xe01fc040 /* memory mapping control */
           .equ    pllcon,              0xe01fc080 /* PLL control            */
           .equ    pllcfg,              0xe01fc084 /* PLL configuration      */
           .equ    pllstat,             0xe01fc088 /* PLL status             */
           .equ    pllfeed,             0xe01fc08c /* PLL feed               */
           .equ    vpbdiv,              0xe01fc100 /* VPB divider            */
           .equ    vicintenclear,       0xfffff014 /* interrupt clear        */
           .equ    vicprotection,       0xfffff020 /* protection             */

           .code   32                   /* compile for ARM 32-bit instruction set */

/* Program code */
           .text   0

/* Exception or interrupt vector table */
           b       reset                           /* reset                  */
und:       b       und                             /* undefined instruction  */
swi:       b       swi                             /* software interrupt     */
pref_abt:  b       pref_abt                        /* prefetch abort         */
data_abt:  b       data_abt                        /* data abort             */
           .long   user_key                        /* user program key       */
irq:       b       irq                             /* interrupt request      */
fiq:       b       fiq                             /* fast interrupt request */

           .long   0x00000000                      /* place holder */
           .long   0x00000000
           .long   0x00000000
           .long   0x00000000
           .long   0x00000000
           .long   0x00000000
           .long   0x00000000
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
           ldr     r0, =mamcr                      /* disable MAM functions      */
           mov     r1, #mam_dis
           str     r1, [r0]
           ldr     r1, =mamtim                     /* set MAM fetch cycles       */
           mov     r2, #mam_cyc
           str     r2, [r1]
           mov     r1, #mam_en                     /* enable MAM functions       */
           str     r1, [r0]
           ldr     r0, =vpbdiv                     /* set VPB divider            */
           mov     r1, #vpbdiv_val
           str     r1, [r0]
           ldr     r0, =pllcon                     /* disconnect and disable PLL */
           mov     r1, #pllcon_dis                 /* plle = 0, pllc = 0         */
           str     r1, [r0]
           ldr     r1, =pllfeed                    /* make changes active        */
           mov     r2, #pll_feed_byte1
           mov     r3, #pll_feed_byte2
           str     r2, [r1]
           str     r3, [r1]
           ldr     r4, =pllcfg                     /* configure PLL              */
           mov     r5, #psel                       /* msel, psel                 */
           mov     r5, r5, lsl #msel_len
           orr     r5, r5, #msel
           str     r5, [r4]
           mov     r4, #plle                       /* plle = 1                   */
           str     r4, [r0]
           str     r2, [r1]                        /* make changes active        */
           str     r3, [r1]
           ldr     r4, =pllstat                    /* wait for lock              */
pll_lock:  ldr     r5, [r4]
           ands    r5, r5, #plock                  /* plock set?                 */
           beq     pll_lock
           mov     r4, #(pllc|plle)                /* connect PLL, pllc = 1      */
           str     r4, [r0]
           str     r2, [r1]                        /* make changes active        */
           str     r3, [r1]
           ldr     r0, =pinsel0                    /* set p0.0-p0.15 as GPIO     */
           mov     r1, #gpio
           str     r1, [r0]
           ldr     r0, =io0dir                     /* set leds as output         */
           ldr     r1, =out_pins
           str     r1, [r0]
           ldr     r0, =io0clr                     /* set output pins to zero    */
           str     r1, [r0]
           ldr     pc, =startup                    /* start main program         */

copy:      cmp     r1, r2                          /* copy from r0 to r1 until r2 */
           ldrlo   r3, [r0], #word_len             /* is reached                  */
           strlo   r3, [r1], #word_len
           blo     copy
           mov     pc, lr                          /* return from subroutine      */

           .end                                    /* stop compiling */
