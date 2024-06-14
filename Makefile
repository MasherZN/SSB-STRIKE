
BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= gl2d_spriteset
GAME_TITLE	:= Smash Bros. Strike! Test
GAME_SUBTITLE2 := Mario sprites
GAME_ICON 		:= C:/msys64/opt/blocksds/core/examples/gl2d/spriteset/icon.bmp
# Source code paths

GFXDIRS		= graphics

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
