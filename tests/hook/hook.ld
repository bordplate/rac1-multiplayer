SECTIONS 
{
    . = 0x6CF04;
    .text.func_patch : { *(.text.func_patch) }

    . = 0xb44b20;
    .text.func_hook : { *(.text.func_hook) }

    . = 0xCE2CB8;
    .data.func_hook : { *(.data.func_hook ) }

    /DISCARD/ : { *(.opd) }
}