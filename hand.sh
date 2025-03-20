#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/hand_texture.png
rm arm9/source/hand.h source/hand.c

$SQUEEZERW \
    --outputTexture graphics/hand_texture.png \
    --outputBaseName HAND \
    --outputH arm9/source/hand.h --outputC arm9/source/hand.c \
    --verbose \
    ./hand
