
#include <sys/syscall.h>
#include <stddef.h>
#include <sys/prx.h>

// inline int sys_prx_load_module(const char* a1, int a2, int a3)
// {
//     system_call_3( _SYS_PRX_LOAD_MODULE, (uint64_t)a1, (uint64_t)a2, (uint64_t)a3 );
// }

// inline int sys_prx_start_module(int a1, int a2, int a3, int* a4, int a5, int a6)
// {
//     system_call_6( _SYS_PRX_START_MODULE, (uint64_t)a1, (uint64_t)a2, (uint64_t)a3, (uint64_t)a4, a5, (uint64_t)a6 );
// }

extern "C" void loaderMain( void ) 
{
    __attribute((section(".text"))) static const char filename[]  = "/dev_hdd0/mod.sprx";
    int id = sys_prx_load_module( filename, 0, NULL );
    int modres;
    int res = sys_prx_start_module( id, 0, NULL, &modres, 0, NULL );
}

typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct
{
    u64 gpr[32];
    f32 fpr[32];    
} context;

void saveContext( void )
{
    context c;
    register u64 r0 __asm__ ("0");
    register u64 r1 __asm__ ("1");
    register u64 r2 __asm__ ("2");
    register u64 r3 __asm__ ("3"); 
    register u64 r4 __asm__ ("4");
    register u64 r5 __asm__ ("5");
    c.gpr[0] = r0;
    c.gpr[1] = r1;
    c.gpr[2] = r2;
    c.gpr[3] = r3;
    c.gpr[4] = r4;
    c.gpr[5] = r5;
}
