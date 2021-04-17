#include <sys/prx.h>
#include <sys/types.h>
#include <sys/return_code.h>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#define true 1
#define false 0

typedef void (*prx_func_t)();
volatile prx_func_t prxFunc;

#ifndef MINIMAL
#define LOG( msg, ... ) printf( msg, ##__VA_ARGS__ )
#else
#define LOG( msg, ... ) do {} while ( false )
#endif

typedef void(*foo_t)(void);
void* pfnFoo;

void foo( void ) 
{
    printf( "original foo called\n" );
}

void foo_thunk( void )
{
    //pfnFoo();
    //printf( "original foo called\n" );
}

void call_pfnFoo( void )
{
    ((foo_t)pfnFoo)();
}


int main(void)
{
    prxFunc = NULL;
    LOG( "hello world from elf\n" );
    sys_prx_id_t id = sys_prx_load_module( "/app_home/mod.sprx", 0, NULL );
    if ( id <= 0 ) LOG( "sys_prx_load_module() failed with %d\n", id );

    //pfnFoo = foo;
    //foo_thunk();
    //foo();

    int modres;
    int res = sys_prx_start_module( id, 0, NULL, &modres, 0, NULL );
    if ( res <= 0 ) LOG( "sys_prx_start_module() failed with %d, modres %d\n", res, modres );
    //prxFunc();

    sys_prx_module_info_v2_t info;
    sys_prx_get_module_info( id, 0, &info );

    LOG( "info addr = 0x%08X\n", (uint32_t)&info );
    LOG( "size = 0x%08X\n", info.size );
    LOG( "name = %s\n", &info.name );
    LOG( "version = %s\n", &info.version );
    LOG( "modattribute = 0x%08X\n", info.modattribute );
    LOG( "start_entry = 0x%08X\n", info.start_entry );
    LOG( "stop_entry = 0x%08X\n", info.stop_entry );
    LOG( "all_segments_num=%d\n", info.all_segments_num );
    LOG( "filename = %s\n", info.filename );
    LOG( "segments_num=%d\n", info.segments_num );
    LOG( "libent_addr = 0x%08X\n", info.libent_addr );
    LOG( "libent_size = 0x%08X\n", info.libent_size );
    LOG( "libstub_addr = 0x%08X\n", info.libstub_addr );
    LOG( "libstub_size = 0x%08X\n", info.libstub_size );
    
    // for ( size_t i = 0; i < info.all_segments_num; ++i )
    // {
    //     sys_prx_segment_info_t* pSeg = &info.segments[i];
    //     LOG( "%d\n", i );
    //     LOG( "segment[%d] base=0x%08X filesz=0x%08X index=%d memsz=0x%08X type=%d\n", i, pSeg->base, pSeg->filesz, pSeg->index, pSeg->memsz, pSeg->type );
    // }

    //((foo_t)pfnFoo)();
    //foo();
    //foo_thunk();
    foo();

    printf( "goodbye world from elf" );
    return 1;
}


volatile char hookCodeBuffer[1024] __attribute__((section(".text")));
volatile char hookDataBuffer[1024];

