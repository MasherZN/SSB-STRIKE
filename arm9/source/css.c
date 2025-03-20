#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fat.h>
#include <filesystem.h>
#include <math.h>
#include "bgcss.h"
#include "cssmugs.h"
#include "cssmugs_texture.h"
#include "hand.h"
#include "hand_texture.h"
#include <gl2d.h>
#include "emptybg.h"
extern int newRoom;
extern int bg2;
extern int bg3;
extern int bgSub2;
extern int currentRoom;
extern bool cssload;
extern bool cssbg;
float handx=100;
float handy=100;
glImage mugs[CSSMUGS_NUM_IMAGES];
glImage hand;


void loadcss(){
    if (cssload!=true){
        cssload = true;
int smugs = glLoadSpriteSet(mugs,
        CSSMUGS_NUM_IMAGES,          // Número de imágenes
        CSSMUGS_texcoords,           // Coordenadas UV
        GL_RGB256,                   // Tipo de textura para glTexImage2D()
        CSSMUGS_BITMAP_WIDTH,        // Tamaño total de la textura X
        CSSMUGS_BITMAP_HEIGHT,       // Tamaño total de la textura Y
        TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, // Parámetros para la textura
        256,                         // Longitud de la paleta (256 colores)
        cssmugs_texturePal,          // Puntero a la paleta de textura
        cssmugs_textureBitmap);      // Puntero a los datos de la textura
        printf("smugs num images %d \n",CSSMUGS_NUM_IMAGES);
        printf("loaded mugs: %d\n", smugs);
int handcss = glLoadSpriteSet(&hand,
            HAND_NUM_IMAGES,          // Número de imágenes
            HAND_texcoords,           // Coordenadas UV
            GL_RGB256,                   // Tipo de textura para glTexImage2D()
            HAND_BITMAP_WIDTH,        // Tamaño total de la textura X
            HAND_BITMAP_HEIGHT,       // Tamaño total de la textura Y
            TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, // Parámetros para la textura
            256,                         // Longitud de la paleta (256 colores)
            hand_texturePal,          // Puntero a la paleta de textura
            hand_textureBitmap);      // Puntero a los datos de la textura
            
        
    }
}
void cssbgs() {
    if (cssbg!=true){
        cssbg=true;
    // Copiar fondos y paletas a la pantalla secundaria (bgSub2)
    dmaCopy(bgcssTiles, bgGetGfxPtr(bgSub2), bgcssTilesLen);
    dmaCopy(bgcssMap, bgGetMapPtr(bgSub2), bgcssMapLen);
    dmaCopy(bgcssPal, BG_PALETTE_SUB, bgcssPalLen);
    vramSetBankE(VRAM_C_SUB_BG);
    
   dmaCopy(bgcssTiles, bgGetGfxPtr(bg2), bgcssTilesLen);
    dmaCopy(bgcssMap, bgGetMapPtr(bg2),bgcssMapLen);
    dmaCopy(bgcssPal,BG_PALETTE,bgcssPalLen); 

    // Configurar la VRAM para la paleta extendida (solo para la pantalla secundaria)
    vramSetBankE(VRAM_E_BG_EXT_PALETTE);
}
}

void drawcss() {
   
    glSpriteScale(33, 53, 1 << 12, GL_FLIP_NONE, &mugs[0]);
    glSpriteScale(64, 53, 1 << 12, GL_FLIP_NONE, &mugs[1]);
    glSpriteScale(95, 53, 1 << 12, GL_FLIP_NONE, &mugs[2]);
    glSpriteScale(126, 53, 1 << 12, GL_FLIP_NONE, &mugs[3]);
    glSpriteScale(157, 53, 1 << 12, GL_FLIP_NONE, &mugs[4]);
    glSpriteScale(188, 53, 1 << 12, GL_FLIP_NONE, &mugs[5]);
    glSpriteScale(33, 84, 1 << 12, GL_FLIP_NONE, &mugs[6]);
    glSpriteScale(64, 84, 1 << 12, GL_FLIP_NONE, &mugs[7]);
    glSpriteScale(95, 84, 1 << 12, GL_FLIP_NONE, &mugs[8]);
    glSpriteScale(126, 84, 1 << 12, GL_FLIP_NONE, &mugs[9]);
    glSpriteScale(157, 84, 1 << 12, GL_FLIP_NONE, &mugs[10]);
    glSpriteScale(188, 84, 1 << 12, GL_FLIP_NONE, &mugs[11]);
    glSpriteScale(handx, handy, 1 << 12, GL_FLIP_NONE, &hand);
  
}
