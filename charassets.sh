#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/title_chars.png
rm arm9/source/titlechars.h source/titlechars.c

$SQUEEZERW \
    --outputTexture graphics/title_chars.png \
    --outputBaseName TSC \
    --outputH arm9/source/titlechars.h --outputC arm9/source/titlechars.c \
    --verbose \
    ./titlechars