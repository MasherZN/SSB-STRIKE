#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/cssmugs_texture.png
rm arm9/source/cssmugs.h source/cssmugs.c

$SQUEEZERW \
    --outputTexture graphics/cssmugs_texture.png \
    --outputBaseName CSSMUGS \
    --outputH arm9/source/cssmugs.h --outputC arm9/source/cssmugs.c \
    --verbose \
    ./cssmugs
