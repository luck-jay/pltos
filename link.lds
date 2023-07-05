OUTPUT_ARCH(arm)
/* Entry Point */
ENTRY(Reset_Handler)

_system_stack_size = 0x400;

/* Specify the memory areas */
MEMORY
{
  RAM  (rw)  : ORIGIN = 0x20000000, LENGTH = 64K
  FLASH (rx)  : ORIGIN = 0x8000000 , LENGTH = 512K
}

/* Define output sections */
SECTIONS
{
  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */

    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    __init_start = .;
    KEEP(*(SORT(.init_fn*)))
    __init_end = .;

    . = ALIGN(4);
    PROVIDE(__ctors_start__ = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE(__ctors_end__ = .);

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH

  .stack :
  {
    . = ALIGN(4);
    _sstack = .;
    . = . + _system_stack_size;
    . = ALIGN(4);
    _estack = .;
  } >RAM

  /* Uninitialized data section */
  __bss_start = . ;
  __bss_start__ = __bss_start;
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    . = ALIGN(4);
    _sbss = .;         /* define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = . ;         /* define a global symbol at bss end */

    *(.bss.init)
  } >RAM
  __bss_end = . ;
  __bss_end__ = __bss_end;

  PROVIDE(_end = .);
  PROVIDE(end = _end);

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
