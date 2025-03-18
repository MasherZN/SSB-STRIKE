#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/solo_texture.png
rm arm9/source/solo.h source/solo.c

$SQUEEZERW \
    --outputTexture graphics/solo_texture.png \
    --outputBaseName SOLO \
    --outputH arm9/source/solo.h --outputC arm9/source/solo.c \
    --verbose \
    ./soloimages
