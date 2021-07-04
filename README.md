# PS3 C code injection toolkit
Welcome to the C code injection toolkit for PS3. This toolkit will allow you to run arbitrary C code in the game, hook existing functions to extend their functionality or insert your own custom functionality entirely.

## Note
This overview is in no way extensive and could use elaboration. 

# Structure
The toolkit is divided into 3 main sections:
- The loader
- The PRX module
- The build scripts

## The loader
The loader consists out of handwritten assembly to load the mod.sprx from the game directory. This code is inserted into an arbitrarily chosen address, in this case during the execution of the main function.

## The PRX module
The PRX module is loaded by the injected loader into the game process. From that point on the PRX loader will communicate with the ELF loader to share addresses with each other, allowing both to interoperate. The sharing of addresses is how the bulk of data, including code, can be moved to the SPRX where code space is less of a concern than it is in the ELF. The PRX module loads its own configuration and runs its submodules accordingly. Submodules have the ability to hook functions and insert custom functionality however they see fit.

## The build scripts
The build scripts are activated through Makefiles in the respective directories, but the bulk of the work is done in the tools placed in the tools directory. Those tools are used to generate the build files (assembly, linkerscripts and makefiles) used to generate the code needer for the ELF loader, PRX loader and function hooks.

The build is configured through 2 files:
- The userconfig.mk for user settings such as paths
- The game config (config_[GAME]) for game target specific settings.

In userconfig.mk are defined:
- The paths to locations used for building, such as the RPCS3 installation directory
- The target game ID, version and serial.

The config_*.mk file defines the game specific addresses used to build the loader and PRX module.

# Building
To build this project on Windows 10, you need:
- chocolatey
- make, installed through ``chocolatey install make``
- python 3.8, available through ``py -3``
- PS3 SDK compilers
- PS3_SDK environment variable that points to the root of the PS3 SDK
- RPCS3 0.0.15-12057-d10584ac Alpha (tested version, ymmv)
- text editor of choice, such as vscode
- base RPCS3 patch in patch.yml (see below)
- bit of luck
- coffee

## Game targets
Support for each game has to be manually added. Contributions are welcome! The goal is to have one codebase that can be conditionally compiled for different game targets, so that every game can benefit from additions and improvements to the toolkit. This is achieved through game IDs.

### List of supported games
| Game ID | Title                     | Serials                   | Versions  |
| -       | -                         | -                         | -         |
| P5      | Persona 5                 | NPEB02436                 | 1.00      |
| SU      | Sonic Unleashed           | BLUS30244                 | 1.02      |
| RC1     | Rachet & Clank 1 HD       | NPEA00385                 | 1.00      |


### Base RPCS3 patches
Each game target requires its own base patch. This base patch provides the loader with the needed import functions to load the SPRX module at game boot. It also defines the needed patch section tags to aid in automatically inserting the generated patches into the patch file.
The base patches can be found in the 'basepatches' folder.

## Setup
To perform the initial setup of the build environment, open a command prompt in the root folder of the project and execute:
```sh
make setup GAME=<game id>
```

After this step, you must update the userconfig.mk file with your own personal settings.

To perform the build, execute:
```
make
```

If all goes well there shouldn't be any errors, and the USRDIR of your game installation should contain mod.sprx. Moreover, your base patch should be been populated with the generated code.

To clean up any generated build files, execute:
```
make clean
```

# Adding a new game target
To add a game target, you must have:
1. The TOC (r2) address
2. An entrypoint code address pointing to a ``nop`` instruction that is used to jump the loader code
3. An ELF substitute subroutine: a subroutine in the ELF whose assembly you can trivially copy to the PRX for use as code space
4. The address to the sys_prx_module_load import function in the ELF
5. The address to the sys_prx_module_start import function in the ELF
6. A read/write data address with 4 bytes freely available for reading and writing to

## Notes
1: Games with large executables will have multiple TOC addresses. The TOC to be used in the config should be the TOC used for the substitute. Hooks can have their TOC address specified individually by specifying ``toc: 0x1234`` in the hook YAML file when needed.

4, 5: Not all games will import these functions by default. If the game does not import these functions, you'll have to replace another (unused) imported **sysPrxForUser** function. This can be done by replacing the hash of the imported functions, to the hash of the function you wish to import. RPCS3 logs all library imports and exports, so use this to your advantage.

To add the game target, you need to create a new config file, named config_**gameid**.mk
Inside the file, you'll have to specify the following:
```mk
TOC                               = <The TOC (r2) address>
LOADER_INJECT_ADDR                = <Loader entrypoint code address (must be a NOP)>
LOADER_START_ADDR                 = <ELF substitute subroutine address + 4 + 16 byte boundary alignment>
LOADER_END_ADDR                   = <LOADER_START_ADDR + 0x80>
LOADER_SYS_PRX_MODULE_LOAD_ADDR   = <The address to the sys_prx_module_load import function in the ELF>
LOADER_SYS_PRX_MODULE_START_ADDR  = <The address to the sys_prx_module_start import function in the ELF>

HOOK_SHARED_TEXT_BEGIN_ADDR = $(LOADER_END_ADDR)
HOOK_SHARED_TEXT_END_ADDR 	= <end address of ELF substitute code> 

HOOK_SHARED_DATA_BEGIN_ADDR = <A read/write data address with 4 bytes freely available for reading and writing to>
HOOK_SHARED_DATA_END_ADDR 	= <HOOK_SHARED_DATA_BEGIN_ADDR + 4>
```
It's possible to specify different addresses for different serials or versions of the same game by comparing against the variables declared in the userconfig.

You'll also have to create a hooks.yml for the game at prx/modules/**gameid**/hooks.yml, containing the hook to use for the substitute, eg:
```yml
# ASM function ported to PRX to make room for the loader
# Don't remove this
_shk_prx_elf_substitute:
    addr: 0x5F7140
    emitTrampoline: no
```

Make sure to specify the newly added game in the userconfig.mk file.