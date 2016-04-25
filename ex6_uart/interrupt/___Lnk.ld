INPUT(Z:\vs\Harvard_Tseng\uart\interrupt\crt0.o)
INPUT(Z:\vs\Harvard_Tseng\uart\interrupt\startup.o Z:\vs\Harvard_Tseng\uart\interrupt\interrupts.o Z:\vs\Harvard_Tseng\uart\interrupt\main.o Z:\vs\Harvard_Tseng\uart\interrupt\vpbdiv.o Z:\vs\Harvard_Tseng\uart\interrupt\extint.o Z:\vs\Harvard_Tseng\uart\interrupt\gpio.o Z:\vs\Harvard_Tseng\uart\interrupt\init.o Z:\vs\Harvard_Tseng\uart\interrupt\mam.o Z:\vs\Harvard_Tseng\uart\interrupt\pll.o Z:\vs\Harvard_Tseng\uart\interrupt\timer.o Z:\vs\Harvard_Tseng\uart\interrupt\vic.o Z:\vs\Harvard_Tseng\uart\interrupt\rtos_tasks.o Z:\vs\Harvard_Tseng\uart\interrupt\rtos.o Z:\vs\Harvard_Tseng\uart\interrupt\i2c.o Z:\vs\Harvard_Tseng\uart\interrupt\uart.o Z:\vs\Harvard_Tseng\uart\interrupt\uart1_ISR.o)

/* Memory definitions */

MEMORY
{
  code (rx) : ORIGIN = 0x00000000, LENGTH = 0x0007d000
  data (rw) : ORIGIN = 0x40000040, LENGTH = 0x00007fc0
}

/* Section definitions */

SECTIONS
{
  .text :
  {
    /* Startup code */
    *crt0.o(.text)
  } > code

  . = ALIGN(4);

  .rodata :
  {
    /* Read-only data (constants) */
    *(.rodata)
  } > code

  . = ALIGN(4);

  _etext = . ;
  PROVIDE(etext = .);

  _codesrc = . ;

  .code : AT(_codesrc)
  {
    _code = . ;
    /* Code to be run from RAM */
    *interrupts.o(.text)
    *(.text)
    *(.glue_7t) *(.glue_7)
  } > data

  . = ALIGN(4);

  _ecode = . ; 

  _datasrc = _etext + SIZEOF(.code);

  .data : AT(_datasrc)
  {
    _data = . ;
    /* Initialised read-write data */
    *(.data)
    SORT(CONSTRUCTORS)
  } > data

  . = ALIGN(4);

  _edata = . ;
  PROVIDE(edata = .);

  .bss :
  {
    _bss = . ;
    /* Uninitialised read-write data */
    *(.bss)
    *(COMMON)
  } > data

  . = ALIGN(4);

  _ebss = . ;

  _end = .;
  PROVIDE(end = .);

  /* Stabs debugging sections */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.                                                        */
  /* Symbols in the DWARF debugging sections are relative to the beginning of the */
  /* section so we begin them at 0.                                               */
  /* DWARF 1 */
  .debug 0 : { *(.debug) }
  .line  0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo 0 : { *(.debug_srcinfo) }
  .debug_sfnames 0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info    0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev  0 : { *(.debug_abbrev) }
  .debug_line    0 : { *(.debug_line) }
  .debug_frame   0 : { *(.debug_frame) }
  .debug_str     0 : { *(.debug_str) }
  .debug_loc     0 : { *(.debug_loc) }
  .debug_macinfo 0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}
