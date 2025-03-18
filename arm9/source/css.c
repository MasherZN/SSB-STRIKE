#include <nds.h>
#include <stdio.h>
#include "bgcss.h"
#include "cssmugs.h"
#include "cssmugs_texture.h"
#include <gl2d.h>

extern int newRoom;
extern int bg2;
extern int bg3;
extern int bgSub2;
extern int currentRoom;
glImage mugs[CSSMUGS_NUM_IMAGES];
int smugs;

void cssbgs() {
    // Cargar sprites en la memoria usando gl2d
    smugs = glLoadSpriteSet(mugs,
        CSSMUGS_NUM_IMAGES,          // Número de imágenes
        CSSMUGS_texcoords,           // Coordenadas UV
        GL_RGB256,                   // Tipo de textura para glTexImage2D()
        CSSMUGS_BITMAP_WIDTH,        // Tamaño total de la textura X
        CSSMUGS_BITMAP_HEIGHT,       // Tamaño total de la textura Y
        TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, // Parámetros para la textura
        256,                         // Longitud de la paleta (256 colores)
        cssmugs_texturePal,          // Puntero a la paleta de textura
        cssmugs_textureBitmap);      // Puntero a los datos de la textura

    // Copiar fondos y paletas a la pantalla secundaria (bgSub2)
    dmaCopy(bgcssTiles, bgGetGfxPtr(bgSub2), bgcssTilesLen);
    dmaCopy(bgcssMap, bgGetMapPtr(bgSub2), bgcssMapLen);
    dmaCopy(bgcssPal, BG_PALETTE_SUB, bgcssPalLen);

    // liberar el fondo 2
    memset(bgGetGfxPtr(bg2), 0, 0x4000); 
    memset(bgGetMapPtr(bg2), 0, 0x400);  
    memset(BG_PALETTE, 0, 0x200);  
    
    dmaCopy(bgcssTiles, bgGetGfxPtr(bg3), bgcssTilesLen);
    dmaCopy(bgcssMap, bgGetMapPtr(bg3), bgcssMapLen);
    dmaCopy(bgcssPal, BG_PALETTE, bgcssPalLen);    

    // Configurar la VRAM para la paleta extendida (solo para la pantalla secundaria)
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);
}

void drawcss() {
    // Cambiar entre pantallas (puede ser innecesario dependiendo de tu flujo)
    lcdSwap();

    // Dibujar los sprites con un tamaño y escala ajustados
    glSpriteScale(33, 53, 1 << 12, GL_FLIP_NONE, &mugs[0]);
    glSpriteScale(64, 53, 1 << 12, GL_FLIP_NONE, &mugs[1]);
}
