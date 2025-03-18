#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/vs_texture.png
rm arm9/source/vs.h source/vs.c

$SQUEEZERW \
    --outputTexture graphics/vs_texture.png \
    --outputBaseName VS \
    --outputH arm9/source/vs.h --outputC arm9/source/vs.c \
    --verbose \
    ./vsimages
