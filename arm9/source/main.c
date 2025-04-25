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
#include "menubg2.h"
#include "emptybg.h"
#include "bgtitlebottom.h"
#include "vs.h"
#include "mp3_shared.h"
#include "titlechars.h"
#include "title_chars.h"
#include "title.h"




const uint32_t screen_width = 256;
const uint32_t screen_height = 192;

int currentRoom = 0;
int newRoom = 1;
bool transitionTo2AfterFade = false; 
bool draw = false;
bool cssload=false;
bool cssbg=false;
int menusb=1<<12;  
int actualsc=4250;
int selectedButton=1;



s16 tscrollX = 0;
s16 tscrollY = -192;
int fadeValue = 30; // Valor inicial de brillo
bool isWaitingToEnterRoom2 = false;
int waitToEnterRoom2Timer = 0;
bool isFadingOut = false;
bool isFadingIn = false;
bool isFadingToWhite = false;
bool FadedToWhite = false;
bool started = false;
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
wav_handle *start;
wav_handle *select;

glImage solo[SOLO_NUM_IMAGES];
glImage vs;
glImage mhud;
glImage lockedb;
glImage optb;
glImage vaultb;
glImage titlechars[TSC_NUM_IMAGES];

int bg2;
  
int bg3;

int bgSub2;

void startFadeOut() {
  isFadingOut = true;
  fadeValue = 30;
}

void startFadeIn() {
  isFadingIn = true;
  fadeValue = -31;
}

void startFadeToWhite() {
  isFadingToWhite = true;
  fadeValue = 0;
}


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
  setBrightness(3,30);

  float mariox=0;
  float marioy=200;
  float kirbx=200;
  float kirby=200;
  float pikax=200;
  float pikay=80;
  float linkx=0;
  float foxx=0;
  float samx=200;
  float yoshiy=-40;
  float yoshix=200;
  float dky=-40;
  float dkx=0; 

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

int title_chars= 
glLoadSpriteSet(titlechars,               
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
 
   int solobutton= glLoadSpriteSet(solo,                // Pointer to glImage array
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
      bool menusounds = false;
  	  
  
    
      glClearColor(0, 0, 0, 0);
    
		
		
    
        
  

  while (1) {
  	
    mp3_fill_buffer();
    // Synchronize game loop to the screen refresh
    swiWaitForVBlank();
    // Set up GL2D for 2D mode
    glBegin2D();

    printf("Is playing %d \n", isPlaying);

    if (currentRoom!=newRoom)
    {
      currentRoom = newRoom;
      printf("currentroom %d \n", currentRoom);  
    printf("newroom %d \n", newRoom);  
    
  
    switch (currentRoom) {
      case 0:
          // Sala vacía o inicial
          break;
  
      case 1:
          if(isPlaying){
            mp3_stop();
            isPlaying=0;
          }
          startFadeOut();
         
          // Pantalla de título
          dmaCopy(bgtitlebottomTiles, bgGetGfxPtr(bg2), bgtitlebottomTilesLen);
          dmaCopy(bgtitlebottomMap, bgGetMapPtr(bg2), bgtitlebottomMapLen);  
          dmaCopy(bgtitlebottomPal, &VRAM_E_EXT_PALETTE[bg2][0], bgtitlebottomPalLen);
  
          vramSetBankE(VRAM_E_BG_EXT_PALETTE);
          bgUpdate();
  
          // Cargar sonido de "start"
          start = wav_load_handle("/data/strike/sfx/menu/start.wav");
          
          swiWaitForVBlank();
          printf("Estás en el case 1\n");
          break;
  
      case 2:
          // Sonidos de navegación del menú
          startFadeOut();
          if(!menusounds){
          cursor = wav_load_handle("/data/strike/sfx/menu/sfx_cursor.wav");
          select = wav_load_handle("/data/strike/sfx/menu/select.wav");
          menusounds=true;
        }
          // Fondo principal superior
          dmaCopy(menubgTiles, bgGetGfxPtr(bg3), menubgTilesLen);
          dmaCopy(menubgMap, bgGetMapPtr(bg3), menubgMapLen);
          dmaCopy(menubgPal, &VRAM_E_EXT_PALETTE[bg3][0], menubgPalLen);
  
          // Fondo adicional superior
          dmaCopy(menubg2Tiles, bgGetGfxPtr(bg2), menubg2TilesLen);
          dmaCopy(menubg2Map, bgGetMapPtr(bg2), menubg2MapLen);
          dmaCopy(menubg2Pal, &VRAM_E_EXT_PALETTE[bg2][0], menubg2PalLen);
  
          // Fondo inferior
          dmaCopy(menubgTiles, bgGetGfxPtr(bgSub2), menubgTilesLen);
          dmaCopy(menubgMap, bgGetMapPtr(bgSub2), menubgMapLen);
          dmaCopy(menubgPal, BG_PALETTE_SUB, menubgPalLen);
  
          vramSetBankE(VRAM_E_BG_EXT_PALETTE);
          lcdSwap();
          bgUpdate();
  
         if (isPlaying==0){
          mp3_play("/data/strike/music/menu.mp3", 1, 0);
          isPlaying=1;
         }
         
         
  
          swiWaitForVBlank();
          printf("Estás en el case 2\n");
          break;
  
      case 3:
          // Preparado para futuras pantallas
          break;
  
      default:
          // Por si currentRoom tiene un valor inesperado
          printf("currentRoom desconocido: %d\n", currentRoom);
          break;
  }
}
    if (isFadingOut) {
      if (fadeValue >= 0) {
          setBrightness(3, fadeValue);
          fadeValue-=3;
      } else {
          isFadingOut = false; // Termina la transición
      }
  }
  
  if (isFadingIn) {
      if (fadeValue < 0) {
          setBrightness(3, fadeValue);
          fadeValue++;
      } else {
          isFadingIn = false;
      }
  }
  
  if (isFadingToWhite) {
      if (fadeValue < 30) {
          setBrightness(3, fadeValue);
          fadeValue += 5;
      } else {
          isFadingToWhite = false;

        
      }
  }

  if(currentRoom==1){
    glSpriteScale(yoshix, yoshiy, 1<<12, GL_FLIP_H  ,&titlechars[6]);
   glSpriteScale(dkx,dky, 1<<12, GL_FLIP_NONE, &titlechars[7]);  
   glSpriteScale(samx, 60, 1<<12, GL_FLIP_H ,&titlechars[4]);
   glSpriteScale(foxx,60, 1<<12, GL_FLIP_NONE, &titlechars[5]);  
   glSpriteScale(linkx,80, 1<<12, GL_FLIP_NONE, &titlechars[3]);
   glSpriteScale(mariox,marioy, 1<<12, GL_FLIP_NONE, &titlechars[0]); //mario
   glSpriteScale(pikax,pikay, 1<<12, GL_FLIP_H, &titlechars[2]); //pika
   glSpriteScale(kirbx, kirby, 1<<12, GL_FLIP_H, &titlechars[1]); //kirby

   if(mariox<200 ){
    mariox+=20;
   }
   if(marioy>100){
    marioy-=10;
   }
   if(kirbx>25){
    kirbx-=20;
   }
   if( kirby>100){
    kirby-=10;
   }
   if(pikax>25){
    pikax-=20;
   }
   if( pikay>100){
    pikay-=10;
   }
   if(linkx<200){
    linkx+=20;
  }
  if(foxx<200){
  foxx+=20;
}
if(samx>25){
  samx-=20;
 }
 if( yoshiy<40){
  yoshiy+=10;
 }
 if(yoshix>25){
  yoshix-=20;
 }
 if(dkx<200){
  dkx+=20;
 }
 if(dky<40){
  dky+=10;
 }
  
    }


  
   

    scanKeys();
    u32 keyu = keysUp();
   
    u32 keys = keysHeld();

    u32 keysd = keysDown();
    
  if (keysd & KEY_START && currentRoom==1 ) {
    
    wav_play(start);
    startFadeToWhite();
    isWaitingToEnterRoom2 = true;     
    waitToEnterRoom2Timer = 0;        
   }
   if (currentRoom == 1 && isWaitingToEnterRoom2) {
    waitToEnterRoom2Timer++;

    if (waitToEnterRoom2Timer >= 10) {  // 60 frames = ~1 segundo a 60FPS
        newRoom = 2;
        isWaitingToEnterRoom2 = false;  // para que no vuelva a entrar
    }
}
 
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
    if(keysd & (KEY_A && currentRoom==2)){
      wav_play(select);
      
      
    }
    if(keysd & (KEY_A)){
      newRoom=newRoom+1;
      
      
    }
    if(keysd & (KEY_B)){
      newRoom=newRoom-1;
      
      
    }
    
    if (currentRoom == 1 && isFadingToWhite && fadeValue >= 30) {
      FadedToWhite = true;
  }

  if (FadedToWhite) {
    currentRoom = 2;
    FadedToWhite = false;
    isFadingToWhite = false;
    fadeValue = 0;
}
    
   if (keysd & (KEY_RIGHT) && currentRoom==2 ) {
     selectedButton=selectedButton+1;
     
    wav_play(cursor);
	}
	 if (keysd & (KEY_LEFT)&&currentRoom==2) {
     selectedButton=selectedButton-1;
     wav_play(cursor);
    }
    if(currentRoom==2){

  
      int soloScale = (selectedButton == 1) ? actualsc : menusb;
      glSpriteScale(9, 31, soloScale, GL_FLIP_NONE, &solo[0]); 
      
       
      int vsScale = (selectedButton == 2) ? actualsc : menusb;
      glSpriteScale(43, 34, vsScale, GL_FLIP_NONE, &solo[1]);
      
       
      int vaultScale = (selectedButton == 4) ? actualsc : menusb;
      glSpriteScale(141, 93, vaultScale, GL_FLIP_NONE, &solo[2]);
      
   
      int optScale = (selectedButton == 3) ? actualsc : menusb;
      glSpriteScale(127, 132,optScale, GL_FLIP_NONE, &solo[3]);
      glSpriteScale(0, 10,1<<12, GL_FLIP_NONE, &mhud);
      
      int lockedScale = (selectedButton == 5) ? actualsc : menusb;
      glSpriteScale(138,51,lockedScale, GL_FLIP_NONE, &solo[4]);
      glSpriteScale(0, 10,1<<12, GL_FLIP_NONE, &mhud);
       }    
          
       
      
      if (selectedButton <=1 && newRoom==2){
        selectedButton =1;
      }
      if (selectedButton>5 && newRoom==2){
        selectedButton =5;
        
      }
      if(currentRoom==2){
 
        glDeleteTextures(1, &title_chars);
    }
      if(currentRoom==3){

        
      glDeleteTextures(1, &solobutton);
      glDeleteTextures(1, &mhudb);
      
      
      
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
