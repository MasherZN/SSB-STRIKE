
BLOCKSDS	?= /opt/blocksds/core

# User config

NAME		:= ssbds
GAME_TITLE	:= Smash Bros. Strike! Test
GAME_SUBTITLE  := Mario Sprites and Pyhisics 
GAME_SUBTITLE2  := DEMO
GAME_ICON 		:= icon.bmp
NITRODIR	:= -d nitrofiles
include $(BLOCKSDS)/sys/default_makefiles/rom_arm9arm7/Makefile
