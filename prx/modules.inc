#include "modules/test/testmodule.h"
#include "modules/p5/dc/dc.h"
#include "modules/p5/kasumi/kasumi.h"
#include "modules/p5/p5e/p5e.h"
#include "modules/p5/EXFLW/EXFLW.h"
#include "modules/p5/scarltz/scarltz.h"

static ModuleInfo sModules[] =
{
    //  short name  long name           toggle setting name     init function       shutdown function       module dependencies
#ifdef GAME_TEST
    {   "test",     "test module",      "enableTestModule",     testModuleInit,     testModuleShutdown,     { "test2" } },
    {   "test2",    "test module 2",    "enableTestModule",     NULL,               NULL,                   { "test" } },  
#endif
#ifdef GAME_P5
    {   "EXFLW",    "Expanded Flowscripts Module",    "enableEXFLWModule",     EXFLWInit,  EXFLWShutdown,      NULL    },  
    {   "dc",       "dc Module",             "enableDCModule",     dcInit,          dcShutdown,             { "EXFLW" } },  
    {   "p5e",      "P5 Enhanced Module",    "enableP5EModule",    p5eInit,         p5eShutdown,            { "EXFLW" } },  
    {   "kasumi",   "Kasumi Module",         "enableKasumiModule", KasumiInit,      KasumiShutdown,            { "EXFLW" } },  
    {   "scarltz",  "Scarltz Module",    "enableScarltzModule", scarltzInit,          scarltzShutdown,            { "EXFLW" } },
#endif
};