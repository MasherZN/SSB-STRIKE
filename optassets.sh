#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/opt_texture.png
rm arm9/source/opt.h source/opt.c

$SQUEEZERW \
    --outputTexture graphics/opt_texture.png \
    --outputBaseName OPT \
    --outputH arm9/source/opt.h --outputC arm9/source/opt.c \
    --verbose \
    ./btno_images
