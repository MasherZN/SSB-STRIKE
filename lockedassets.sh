#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/locked_texture.png
rm arm9/source/locked.h source/locked.c

$SQUEEZERW \
    --outputTexture graphics/locked_texture.png \
    --outputBaseName LOCKED \
    --outputH arm9/source/locked.h --outputC arm9/source/locked.c \
    --verbose \
    ./btnl_images
