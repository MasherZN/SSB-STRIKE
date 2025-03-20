#include <fat.h>
#include <filesystem.h>
#include <gl2d.h>
#include <math.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "solo_texture.h"
#include "solo.h"
#include "vs_texture.h"
#include "vs.h"
#include "mhud_texture.h"
#include "mhud.h"
#include "locked_texture.h"
#include "locked.h"
#include "opt_texture.h"
#include "opt.h"
#include "vault_texture.h"
#include "vault.h"
#include "menubg.h"
#include "menubg.h"
#include "menubg2.h"
#include "emptybg.h"
#include "bgtitlebottom.h"
#include "vs.h"
#include "mp3_shared.h"




const uint32_t screen_width = 256;
const uint32_t screen_height = 192;

int currentRoom = 2;
int newRoom = 2;

bool draw = false;
bool cssload=false;
bool cssbg=false;
int menusb=1<<12;  
int actualsc=4250;
int selectedButton=1;



s16 scrollX = 128;
s16 scrollY = 96;
wav_handle *sonido;
wav_handle *airdodgem;
wav_handle *dash;
wav_handle *attack1;
wav_handle *attackspin;
wav_handle *mdjump;
wav_handle *spinsfx;
wav_handle *cursor;


glImage solo;
glImage vs;
glImage mhud;
glImage lockedb;
glImage optb;
glImage vaultb;
glImage titlechars;

int bg2;
  
int bg3;

int bgSub2;

int main(int argc, char **argv) {
  mp3_init();
  fatInitDefault();
  consoleDemoInit();

  glScreen2D();

  videoSetMode(MODE_5_3D);
  videoSetModeSub(MODE_5_2D);
  
  vramSetBankA(VRAM_A_MAIN_BG);
  vramSetBankB(VRAM_B_TEXTURE);
  vramSetBankG(VRAM_G_TEX_PALETTE);
  vramSetBankE(VRAM_E_LCD);
  vramSetBankC(VRAM_C_SUB_BG);

  bgExtPaletteEnable();

   bg2 = bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 2, 3);
  
   bg3 = bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 1, 1);
  
  bgSub2= bgInitSub(2, BgType_Rotation, BgSize_R_256x256, 0, 1);

  consoleInit(NULL, // Usa la instancia por defecto
    1, // Capa 0
    BgType_Text4bpp, // Fondo de texto de 16 colores
    BgSize_T_256x256, // Tamaño 256x256 píxeles
    31, // Base del mapa de tiles en VRAM
    6, // Base de los tiles en VRAM
    false, // Se muestra en la pantalla inferior
    true); // Cargar la fuente y mapa de la consola automáticamente


   /*int title_chars= glLoadSpriteSet(&titlechars,               
                      TSC_NUM_IMAGES,     // Number of images
                      TSC_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      TSC_BITMAP_WIDTH,   
                      TSC_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      title_charsPal,      // Pointer to texture palette data
                      title_charsBitmap);  // Pointer to texture data 
    
*/
extern void cssbgs();
extern void drawcss();
extern void loadcss();
extern float handx;
extern float handy;

   int solobutton= glLoadSpriteSet(&solo,                // Pointer to glImage array
                      SOLO_NUM_IMAGES,     // Number of images
                      SOLO_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      SOLO_BITMAP_WIDTH,   // Full texture size X (image size)
                      SOLO_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      solo_texturePal,      // Pointer to texture palette data
                      solo_textureBitmap);  // Pointer to texture data 
 int vsbutton= glLoadSpriteSet(&vs,                // Pointer to glImage array
                      VS_NUM_IMAGES,     // Number of images
                      VS_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      VS_BITMAP_WIDTH,   // Full texture size X (image size)
                      VS_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      vs_texturePal,      // Pointer to texture palette data
                      vs_textureBitmap);  // Pointer to texture data 
    int mhudb= glLoadSpriteSet(&mhud,                // Pointer to glImage array
                      MHUD_NUM_IMAGES,     // Number of images
                      MHUD_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      MHUD_BITMAP_WIDTH,   // Full texture size X (image size)
                      MHUD_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      mhud_texturePal,      // Pointer to texture palette data
                      mhud_textureBitmap);  // Pointer to texture data 
     
     int opbtn= glLoadSpriteSet(&optb,                // Pointer to glImage array
                      OPT_NUM_IMAGES,     // Number of images
                      OPT_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      OPT_BITMAP_WIDTH,   // Full texture size X (image size)
                      OPT_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      opt_texturePal,      // Pointer to texture palette data
                      opt_textureBitmap);  // Pointer to texture data                   
     
    int vaultbtn= glLoadSpriteSet(&vaultb,                // Pointer to glImage array
                      VAULT_NUM_IMAGES,     // Number of images
                      VAULT_texcoords,      // Array of UV coordinates
                      GL_RGB256,            // Texture type for glTexImage2D()
                      VAULT_BITMAP_WIDTH,   // Full texture size X (image size)
                      VAULT_BITMAP_HEIGHT,  // Full texture size Y (image size)
                      // Parameters for glTexImage2D()
                      TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
                      256,  // Length of the palette to use (256 colors)
                      vault_texturePal,      // Pointer to texture palette data
                      vault_textureBitmap);  // Pointer to texture data 

                      
 
  float sprite_offsets_x[] = {
      -5,  -7,  -8,  -8,  -8,  -8,  -7,  -5,  // stand       --izq ++ der
      -6,  -9,  -11, -9,  -3,  -9,  -11, -7,  // walk
      -6,                                     // jump
      -12,                                    // airdodge
      -8,  -8,  -8,  -8,  -8,  -8,  -8,  -8,  // run
      -14, -14, -14, -29, -30, -30, -24, -24, -22, -19, -12, -17,  // bair
      -21, -24, -14, -31, -27, -31, -31, -30, -30, -29, // downb
	  -12, -12, -12, -12, -12, -12, -12, -12, -12,-12,  //djump
      -4, -4, -4, -4, -5, -2, -1,-2, -4, -7,4, 8, 5, 1, -4, -2,-2, //jab
      0,-5}; //crouch
      
  float sprite_offsets_y[] = {
      1,  0,   0,   1,   2,   0,   0,   2, //  ++abajo --arriba
      1,  1,   0,   2,   1,   1,   1,   2, // walk
     -1,                                    // jump
	  0,                                   // airdodge
	  4,   4,   4,   4,   4,   4,   4,   4,   // run
	  0, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // bair
      4,  -11, -11, -11, -26, -27, -27, -27, -26, -16, // downb
	  0,  0,   0,   0,   0,   0,   0,   0,   0,0,  //djump
	  1,1,1,1,1,6,7,9,4,1,-3,-4,-7,-7,-4,4,3,//jab
	  19,18}; //crouch

 	  s16 scaleX = 1 << 8;
      s16 scaleY = 1 << 8;
      s16 rcX = 128;
      s16 rcY = 96;
      s16 angle = 0;
      float timeScale = 1;
      float dt = 0;
      dt = timeScale;

      int isPlaying = mp3_is_playing();
  	  int playvictory=false;
  

    
		
		
    switch (currentRoom) { 
        case 0:
            // Cargar y configurar los fondos para la sala 0
            // Ejemplo:
          
            break;
        case 1:
        /*
        dmaCopy(bgtitlebottomTiles, bgGetGfxPtr(bgSub2), bgtitlebottomTilesLen);
  			dmaCopy(bgtitlebottomMap, bgGetMapPtr(bgSub2), bgtitlebottomMapLen);
  			dmaCopy(bgtitlebottomPal, BG_PALETTE_SUB, bgtitlebottomPalLen);
        vramSetBankE(VRAM_E_BG_EXT_PALETTE);
        bgUpdate();*/
        //lSpriteScale(9, 31, 1<<12, GL_FLIP_NONE, &title_chars[1]); 
           
            break;
             case 2:
            // Cargar y configurar los fondos para la sala 2
            lcdSwap();
            
           
        dmaCopy(menubgTiles, bgGetGfxPtr(bg3), menubgTilesLen);
  			dmaCopy(menubgMap, bgGetMapPtr(bg3), menubgMapLen);
  			dmaCopy(menubgPal, &VRAM_E_EXT_PALETTE[bg3][0], menubgPalLen);
           
        dmaCopy(menubg2Tiles, bgGetGfxPtr(bg2), menubg2TilesLen);
  			dmaCopy(menubg2Map, bgGetMapPtr(bg2), menubg2MapLen);
  			dmaCopy(menubg2Pal, &VRAM_E_EXT_PALETTE[bg2][0], menubg2PalLen);
        
  	
  			dmaCopy(emptybgTiles, bgGetGfxPtr(bgSub2), emptybgTilesLen);
  			dmaCopy(emptybgMap, bgGetMapPtr(bgSub2), emptybgMapLen);
  			dmaCopy(emptybgPal,BG_PALETTE_SUB, emptybgPalLen);
  		
  			vramSetBankE(VRAM_E_BG_EXT_PALETTE);
        bgUpdate();
  			cursor = wav_load_handle("/data/strike/sfx/menu/sfx_cursor.wav");
        mp3_play("/data/strike/music/menu.mp3", 1, 0);
  			
  			
  			
  			
  			
  			
  			
  			
  			
            break;
        // Agregar m�s casos para cada sala adicional
         case 3:
       
        
         
         
         	
         	 break;
         	 
         	 
         	 
        default:
            // Sala no v�lida, maneja el caso seg�n sea necesario
            break;
    }
      // Actualizar la sala actual
     glClearColor(0, 0, 0, 0);
  while (1) {
  	
    mp3_fill_buffer();
    // Synchronize game loop to the screen refresh
    swiWaitForVBlank();
    // Set up GL2D for 2D mode
    glBegin2D();
    if (currentRoom!=newRoom)
    {
      currentRoom = newRoom;
      printf("currentroom %d \n", currentRoom);  // Primera línea
    printf("newroom %d \n", newRoom);  
    }
  
   

    scanKeys();
    u32 keyu = keysUp();
   
    u32 keys = keysHeld();

    u32 keysd = keysDown();

    if(keys & (KEY_LEFT)){
      handx-=10;
      
      
    }
    if(keys & (KEY_RIGHT)){
      handx+=10;
      
      
    }
    if(keys & (KEY_UP)){
      handy-=10;
      
      
    }
    if(keys & (KEY_DOWN)){
      handy+=10;
      
      
    }
    if(keysd & (KEY_A)){
      newRoom=3;
      
      
    }
    if(keysd & (KEY_START)){
      newRoom=newRoom+1;
      
      
    }
    
   if (keysd & (KEY_RIGHT) && currentRoom==2) {
     selectedButton=selectedButton+1;
     
    wav_play(cursor);
	}
	 if (keysd & (KEY_LEFT)&&currentRoom==2) {
     selectedButton=selectedButton-1;
     wav_play(cursor);
    }
    if(currentRoom==2){

  
      int soloScale = (selectedButton == 1) ? actualsc : menusb;
      glSpriteScale(9, 31, soloScale, GL_FLIP_NONE, &solo); 
      
      // Dibuja el bot�n 'vs'
      int vsScale = (selectedButton == 2) ? actualsc : menusb;
      glSpriteScale(43, 34, vsScale, GL_FLIP_NONE, &vs);
      
      // Dibuja el bot�n 'vault'
      int vaultScale = (selectedButton == 4) ? actualsc : menusb;
      glSpriteScale(141, 93, vaultScale, GL_FLIP_NONE, &vaultb);
      
      // Dibuja el bot�n 'opt'
      int optScale = (selectedButton == 3) ? actualsc : menusb;
      glSpriteScale(127, 132,optScale, GL_FLIP_NONE, &optb);
      glSpriteScale(0, 0,1<<12, GL_FLIP_NONE, &mhud);
       }    
          
       
      
      if (selectedButton <=1 && newRoom==2){
        selectedButton =1;
      }
      if (selectedButton>4 && newRoom==2){
        selectedButton =4;
        
      }
      if(currentRoom==3){
        // Desactivar los fondos 2 y 3 limpiando los bits correspondientes
      //REG_DISPCNT &= ~(DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE);
      glDeleteTextures(1, &mhudb);
      glDeleteTextures(1, &solobutton);
      glDeleteTextures(1, &vsbutton);
      glDeleteTextures(1, &opbtn);
      glDeleteTextures(1, &vaultbtn);
      
      
      cssbgs();
      loadcss();
      drawcss();
      
      }
    
    
    bgSetCenter(bg2, rcX, rcY);

    bgSetRotateScale(bg2, angle, scaleX, scaleY);

    bgSetScroll(bg2, scrollX, scrollY);


    bgUpdate();

    glEnd2D();

    glFlush(0);
    swiWaitForVBlank();
  }



  return 0;
}
