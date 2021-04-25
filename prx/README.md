# (S)PRX module source code #
In here you'll find the source code for mod.sprx
It is structured in 3 main parts:
- The loader (initialisation)
- The modules 
- The library

## The loader
The loader is contained in part in runtime/init.c, but mostly inside the ELF in which the sprx is injected.
The PRX side of the loader is responsible for passing data to the ELF side of the loader, such as function addresses.

During initialisation of the PRX module, necessary data is written to ELF memory, the config is loaded and the enabled modules are initialised.

The configuration is handled by the config.inc file.

## The (sub)modules
The main functionality of the SPRX is implemented through (sub)modules. Each module has an init function and a shutdown function, executed by the loader. Each module will be responsible for hooking functions, writing data to memory, and communicating with other modules. One example of suh a module is the test module (testmodule.c). 

The list of modules to load is defined in the module list (modules.inc).

## The library (lib/)
Due to technical restrictions, the PRX does not have access to the full C library. In order to overcome this, replacements for some of the C standard library API have been added in lib folder. Due to the limited nature of the C library, additional functions write created to help ease the transition to a lower level language such as C and to boost productivity. 

Notable examples include:
- clib.h
    - Contains implementations for a handful of common C library functions, such as printf, sprintf, memcpy, strlen, rand, etc.
- config.h
    - Contains the API used to read configuration settings at runtime.
- shk.h
    - Contains the API used for hooking functions and calling them from the PRX
- utils.h
    - Contains various useful functions to help speed up development, including:
        - string functions based off the .NET string API (Substring, Replace, IndexOf, etc.)
        - range (min, max) based random functions 
- common.h
    - Includes most of the most frequently used headers for ease of use.


# Adding a new module #
In order to add a new module, you must add a new entry to the module list.

The module list, located in modules.inc, contains information about every module that can be loaded and unloaded by the loader. 

To add your own module, for example 'foo' for game 'bar', first you must create 2 source files: foo.c and foo.h

After creating those, make sure to declare and define the 'fooInit' and the 'fooShutdown' functions, both taking no parameters and returning no value.

Your header file (foo.h) should look like something like this:
```c
// Make sure include the include guard to ensure only files
// for the specific game are included during the build
#ifdef GAME_BAR

#ifndef FOO_H
#define FOO_H

// Pre-prepared libraries exist in lib
// Common includes things like printf for printf, strlen, etc.
// PRX dont have access to the usual C libraries, so any functionality that you need from it
// will have to be reimplemented in there.
// Make sure you include headers in the right order!
// You can only use things that have been declared before their usage.
#include "lib/common.h"

// This is how you export functions for other C files to use.
// The PRX loader calls these 2 functions for you.

// The module initialisation function.
void fooInit( void );

// The module shutdown function.
void fooShutdown( void );

#endif // FOO_H
#endif // GAME_BAR
```

Your source file (foo.c) should look something like this:
```c
// Make sure include the include guard to ensure only files
// for the specific game are included during the build
#ifdef GAME_BAR

// Include the header file in which type are defined
// See the header file for more information
#include "foo.h"

// Include this to use hooks
// SHK (Static Hook library)
#include "lib/shk.h"

// Include this to use config variables
#include "lib/config.h"

// The start function of the PRX. This gets executed when the loader loads the PRX at boot.
// This means game data is not initialized yet! If you want to modify anything that is initialized after boot, hook a function that is called after initialisation.
void fooInit( void )
{
    // These prints show up in the TTY log if everything is working as it should.
    printf( "foo: hello world :)\n" );
}

void fooShutdown( void )
{
    // Executed when the PRX module is unloaded.    
    printf( "foo: goodbye world :(\n" );
}

#endif
```

If you are unsure on how to do this, refer to the testmodule for a practical example.

Second, include your newly created header file in modules.inc, like in this example:
```c
#ifdef GAME_BAR
#include "modules/bar/foo.h"
#endif
```

Finally, simply add an entry to the Module list following the given format:

```c
#ifdef GAME_BAR
{ "foo", "The Foo module", "enableFoo", fooInit, fooShutdown, {} },
#endif
```

In case the ifdef guard already exists for the game, you add the code inside it instead of adding another one.

The values are as follows:
- The short name used to refer to the module
- The longer descriptive name used to describe the module to the user
- The name of the config setting used to enable the module
- The name of the module initialization function
- The name of the module shutdown/deinitialization function
- List of module dependencies. 
    - The short names of modules are added to the list to force them to be loaded as a dependency, for example:
        ```c
        { "foo", "The Foo module", "enableFoo", fooInit, fooShutdown, { "bar", "qux" } }
        { "bar", ... }
        { "qux", ... }
        ```
    - If the dependency is not enabled, or can't be loaded, then the dependent module won't be loaded. Check the log to troubleshoot why your module isn't loading.

If you have followed these steps correctly and have added a new config setting to enable your module, your module name should come up during the initialisation stage in the RPCS3 TTY log, like in the following example:
```
config: Enables the test module (enableTestModule): true
config: Enable debugging features (debug): false
config: Message of the day (motd): Hello World
modprx: initialising modules
modprx: initialising module The Foo module (foo)
foo: hello world
```

# Adding an S file #
Similar to C files, S files, assembly source files, are specified in the Makefile as well. 

To include your S file, simply add the name of your S file to the SFILES variable.

# Adding a config setting #
Want to add a feature toggle for your mod? Add it to the config file by editing config.inc.

Each config setting follows the format of:

```
CONFIG_SETTING( type, shortName, "Long, descriptive user friendly name", default value )
```

Example:
```c
#ifdef GAME_NAME
CONFIG_SETTING( BOOL, enableTestModule, "Enables the test module", true )
#endif
```

In case the ifdef guard already exists for the game, you add the code inside it instead of adding another one.

The available types are:
- BOOL: boolean true/false values for on/off toggles
- INT: integer values
- FLOAT: decimal values
- STRING: text values

You can access the config files by using the following access macros defined in lib/config.h:
- ``CONFIG_ENABLED( shortName )``
    - returns boolean value true/false
- ``CONFIG_BOOL( shortName )``
    - alternative for bools
- ``CONFIG_INT( shortName )``
    - returns integer value
- ``CONFIG_FLOAT( shortName )``
    - returns decimal (float) value

Example usage:
```c
if ( CONFIG_ENABLED( debug ) )
{
    // Example 
    printf( "debug enabled via config\n" );
}

printf( "Message of the day: %s\n", CONFIG_STRING( motd ) );
```
