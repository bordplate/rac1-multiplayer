#include "modules/rc1/rc1.h"

static ModuleInfo sModules[] =
{
    //  short name  long name           toggle setting name     init function       shutdown function       module dependencies
#ifdef GAME_RC1
	{ "ratchetTest", "Ratchet test mod", "enableRatchetTestModule", rc1_init, rc1_shutdown, NULL },
#endif
};