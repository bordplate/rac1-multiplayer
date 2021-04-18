#ifndef RUNTIME_H
#define RUNTIME_H

// Initializes the mod PRX runtime.
// If you forget to do this, the game will crash on boot trying to access 0x0BADF00D
void runtimeInit();

#endif