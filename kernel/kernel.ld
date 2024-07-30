OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{
  /*
   * ensure that entry.S / _entry is at 0x80000000,
   * where qemu's -kernel jumps.
   */

  .text 0x40000000  : {
    *(.text .text.*)
    /*
    . = ALIGN(0x1000);
    _trampoline = .;
    *(trampsec)
    . = ALIGN(0x1000);
    /* ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page"); */
    PROVIDE(etext = .);
    */
    . = ALIGN(8);
  }

  .rodata : {
    . = ALIGN(8);
    *(.srodata .srodata.*) /* do not need to distinguish this from .rodata */
    *(.rodata .rodata.*)
    . = ALIGN(8);
  }

  .data : {
    . = ALIGN(8);
    *(.sdata .sdata.*) /* do not need to distinguish this from .data */
    *(.data .data.*)
    . = ALIGN(8);
  }

  .bss : {
    . = ALIGN(8);
    *(.sbss .sbss.*) /* do not need to distinguish this from .bss */
    *(.bss .bss.*)
    . = ALIGN(8);
  }

  PROVIDE(end = .);
}
