#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/mhud_texture.png
rm arm9/source/mhud.h source/mhud.c

$SQUEEZERW \
    --outputTexture graphics/mhud_texture.png \
    --outputBaseName MHUD \
    --outputH arm9/source/mhud.h --outputC arm9/source/mhud.c \
    --verbose \
    ./menuhud_images
