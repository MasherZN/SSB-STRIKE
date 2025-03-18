#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/title_chars.png
rm arm9/source/chars.h source/chars.c

$SQUEEZERW \
    --outputTexture graphics/title_chars.png \
    --outputBaseName TSC \
    --outputH arm9/source/chars.h --outputC arm9/source/chars.c \
    --verbose \
    ./titlechars
