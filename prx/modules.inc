#include "rc1/rc1.h"

#ifdef __cplusplus
extern "C" {
#endif

static ModuleInfo sModules[] =
{
    //  short name  long name           toggle setting name     init function       shutdown function       module dependencies
#ifdef GAME_RC1
	{ "ratchetMP", "Ratchet multiplayer mod", "enableRatchetMPModule", rc1_init, rc1_shutdown, NULL },
#endif
};

#ifdef __cplusplus
};
#endif