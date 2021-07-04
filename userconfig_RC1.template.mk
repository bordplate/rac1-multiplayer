# here you can set the user-specific variables for the build
# required:
# 	PYTHON			path to python 3 launcher
# 	RPCS3_DIR		path to the root of the RPCS3 installation directory
#	GAME			game target id, one of:
#		P5			Persona 5
#		SU			Sonic Unleashed
#		RC1			Ratchet & Clank 1 HD
#	GAME_ID			the BL/NP game serial, eg. BLUS30244, NPEB02436
#	GAME_VER		game version in the format of: M.mm ([M]ajor version, [m]inor version)
#	GAME_CAT		game category, one of: HDD (PSN), DISC (Bluray)
# optional:
# 	PATCH_FILE		path to the RPCS3 patch yml
# 	GAME_DIR		path to the game USRDIR

PYTHON = py -3
RPCS3_DIR = X:\emu\rpcs3
GAME = RC1
GAME_ID = NPEA00385
GAME_VER = 1.00
GAME_CAT = HDD