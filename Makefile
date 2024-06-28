
BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= gl2d_spriteset
GAME_TITLE	:= Smash Bros. Strike! Test
GAME_SUBTITLE2 := Mario sprites
GAME_ICON 		:= icon.bmp
NITRODIR	:= -d nitrofiles
include $(BLOCKSDS)/sys/default_makefiles/rom_arm9arm7/Makefile
