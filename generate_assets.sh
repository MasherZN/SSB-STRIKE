#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/atlas_texture.png
rm arm9/source/atlas.h source/atlas.c

$SQUEEZERW \
    --outputTexture graphics/atlas_texture.png \
    --outputBaseName ATLAS \
    --outputH arm9/source/atlas.h --outputC arm9/source/atlas.c \
    --verbose \
    ./original_images
