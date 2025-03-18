#!/bin/sh

BLOCKSDS=/opt/blocksds/core
SQUEEZERW=$BLOCKSDS/tools/squeezer/squeezerw

rm graphics/vault_texture.png
rm arm9/source/vault.h source/vault.c

$SQUEEZERW \
    --outputTexture graphics/vault_texture.png \
    --outputBaseName VAULT \
    --outputH arm9/source/vault.h --outputC arm9/source/vault.c \
    --verbose \
    ./btnv_images
