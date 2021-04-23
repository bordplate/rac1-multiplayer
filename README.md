# Persona 5/PS3 C code injection toolkit
Welcome to the C code injection toolkit for Persona 5 / PS3. This toolkit will allow you to run arbitrary C code in the game, hook existing functions to extend their functionality or insert your own custom functionality entirely.

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

At the time of writing, the build is configured through the userconfig.mk file.

In this file are defined:
- The paths to locations used for building, such as the RPCS3 installation directory.
- The hooks used by the PRX module. These must be specified here in order to generate the necessary RPCS3 patch.

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

## Base RPCS3 patch
This is for the european PSN version of the game, but other versions should work.

```yml
  Mod SPRX:
    Games: *p5_Title_EU
    Author: TGE
    Notes: 
    Patch Version: 1.0
    Patch:
      - [ be32, 0xB4669C, 0x26090058 ] # replace _sys_heap_delete_heap with sys_prx_load_module
      - [ be32, 0xB46678, 0x9f18429d ] # replace _sys_heap_malloc with sys_prx_start_module
      - [ byte, 0xB4695F, 0 ]          # set cellSail import count to 0

      # inject loader code into main
      # bin2rpcs3patch begin shk_elf_loader
      # bin2rpcs3patch end

      # bin2rpcs3patch begin shk_elf_inject
      # bin2rpcs3patch end
```

To perform the initial setup of the build environment, open a command prompt in the root folder of the project and execute:
```sh
make setup
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
2. An entrypoint code address pointing to a ``nop`` instruction that is used to jump the oader code
3. An ELF substitute subroutine: a subroutine in the ELF whose assembly you can trivially copy to the PRX for use as code space
4. The address to the sys_prx_module_load import function in the ELF
5. The address to the sys_prx_module_start import function in the ELF
6. A read/write data address with 4 bytes freely available for reading and writing to

To add the target, you must add the following to the makefile (after the P5 game config but before the else):
```mk
ifeq ($(GAME), <game name>)
TOC                               = <The TOC (r2) address>
LOADER_INJECT_ADDR                = <Loader entrypoint code address>
LOADER_START_ADDR                 = <ELF substitute subroutine address>
LOADER_END_ADDR                   = <LOADER_START_ADDR + 0x80>
LOADER_SYS_PRX_MODULE_LOAD_ADDR   = <The address to the sys_prx_module_load import function in the ELF>
LOADER_SYS_PRX_MODULE_START_ADDR  = <The address to the sys_prx_module_start import function in the ELF>

HOOK_SHARED_TEXT_BEGIN_ADDR = $(LOADER_END_ADDR)
HOOK_SHARED_TEXT_END_ADDR 	= <end address of ELF substitute code> 

HOOK_SHARED_DATA_BEGIN_ADDR = <A read/write data address with 4 bytes freely available for reading and writing to>
HOOK_SHARED_DATA_END_ADDR 	= <HOOK_SHARED_DATA_BEGIN_ADDR + 4>

HOOK_SUBSTITUTE = _shk_prx_elf_substitute/<address of ELF substitute subroutine>/""
```

Make sure to specify the newly added game in the userconfig.mk file.