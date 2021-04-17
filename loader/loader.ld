.sys_prx_load_module = 0xB45E5C; 
.sys_prx_start_module = 0XB45D3C;

SECTIONS 
{
    . = 0x10250;
    .text.inject : { *(.text.inject) }

    . = 0xA3BE70;
    .text : { *(.text) }
    /DISCARD/ : { *(.opd) }
}