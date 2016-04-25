/* Date:   27.7.2011   */
/* Author: Janez Puhan */

/* Constant */
              .equ    intr,   0xc0      /* irq and fiq flags mask                 */

              .code   32                /* compile for ARM 32-bit instruction set */

/* Global symbols */
              .global disable_intr
              .global enable_intr

/* Program code */
              .text

/* Functions */
/* Function disable_intr() sets I and F flags in status register as given in r0.  */
/* It returns the initial I and F flags mask.                                     */
disable_intr: stmfd   sp!,    {r4-r5}   /* save registers                         */
              mrs     r4,     cpsr      /* get status                             */
              and     r5,     r4, #intr /* get I and F flags                      */
              orr     r4,     r4, r0    /* set I and F flags                      */
              msr     cpsr_c, r4
              mov     r0,     r5        /* return initial I and F values          */
              ldmfd   sp!,    {r4-r5}   /* restore registers                      */
              mov     pc,     lr        /* return from subroutine                 */

/* Function enable_intr() restores I and F flags in status register as given in   */
/* r0.                                                                            */
enable_intr:  stmfd   sp!,    {r4}      /* save register                          */
              mrs     r4,     cpsr      /* get status                             */
              bic     r4,     r4, #intr /* restore I anf F flags                  */
              orr     r4,     r4, r0
              msr     cpsr_c, r4
              ldmfd   sp!,    {r4}      /* restore register                       */
              mov     pc,     lr        /* return from subroutine                 */

              .end                      /* stop compiling                         */
