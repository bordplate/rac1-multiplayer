# Multiplayer for Ratchet & Clank

![Ratchet Multiplayer logo](https://raw.githubusercontent.com/bordplate/rac1-multiplayer/main/app/LOGO.png)  
Logo courtesy of Discord user \_triippy\_

This is a multiplayer mod for the PS3 HD release of the original 2002 Ratchet & Clank. It runs on console and emulator thanks
to [PS3-CKit](https://github.com/tge-was-taken/ps3-ckit) by TGE.  

For the server side, check out [bordplate/Lawrence](https://github.com/bordplate/Lawrence).  

We also have a [community over on Discord](https://discord.gg/5DXGkhb2zZ) for discussing the multiplayer and 
[Randomizer](https://github.com/bordplate/rac1-randomizer) mods. 

## Installation
Go to the releases and download the .pkg file. Install on either console or emulator the way you install .pkg files normally. 
The game will show up on the XMB/Home screen on your PS3 or emulator. 

You must have a legal copy of the european version (NPEA00385) of Ratchet & Clank already installed to start the mod. 

# Structure
## Launcher `/app/`
This is the main executable for the game launcher, it only spawns our modded EBOOT. 

## Loader `/loader/`
The loader consists out of handwritten assembly to load the mod.sprx from the game directory. When building, we also 
decrypt, patch and resign a copy of the original game code such that we have a static binary with our loader code. 

## PRX module `/prx/`
The PRX module is loaded by the injected loader into the game process. From that point on the PRX loader will communicate
with the ELF loader to share addresses with each other, allowing both to interoperate. The sharing of addresses is how the
bulk of data, including code, can be moved to the SPRX where code space is less of a concern than it is in the ELF. The 
PRX module loads its own configuration and runs its submodules accordingly. Submodules have the ability to hook functions
and insert custom functionality however they see fit.  

This module contains all the multiplayer logic in the game.

## The build scripts
The build scripts are activated through Makefiles in the respective directories, but the bulk of the work is done in the tools placed in the tools directory. Those tools are used to generate the build files (assembly, linkerscripts and makefiles) used to generate the code needer for the ELF loader, PRX loader and function hooks.

The build is configured through 2 files:
- The userconfig.mk for user settings such as paths
- The game config (config_RC1.mk) for game target specific settings.

In userconfig.mk are defined:
- The paths to locations used for building, such as the RPCS3 installation directory
- The target game ID, version and serial.

The config_*.mk file defines the game specific addresses used to build the loader and PRX module.

# Building
I strongly suggest you have a look at how the building works in practice by looking at the 
[Github Action workflow](https://github.com/bordplate/rac1-multiplayer/blob/main/.github/workflows/release.yml) in this
repository.  

To build this project on Windows 10, you need:
- chocolatey
- make, installed through ``chocolatey install make``
- python 3.8, available through ``py -3``
- PS3 SDK compilers (check GitHub Action workflow mentioned above)
- PS3_SDK environment variable that points to the root of the PS3 SDK
- RPCS3
- text editor of choice, such as vscode
- a copy of Ratchet & Clank (NPEA00385) installed in RPCS3 in `{RPCS3_INSTALL_DIR}/dev_hdd0/game/NPEA00385`
- bit of luck
- coffee


### Base patches
This base patch provides the loader with the needed import functions to load the SPRX module at game boot. And including
some patches to change behavior of some library calls that determine the game content paths for the game. Only the rc1.txt
patch file is actually applied, but the "legacy" yaml file contains comments. 
The base patches can be found in the 'basepatches' folder.

## Setup
To perform the build, execute:
```
make
```

If all goes well there shouldn't be any errors, and a .pkg installer file should be created in the `/build/` folder, including
a `/build/PS3_GAME/USRDIR` folder with the app launcher that you can start directly from RPCS3. Subsequent builds update
the `PS3_GAME` folder and when you restart the game in RPCS3, the updates should be applied.