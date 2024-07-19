#include <gl2d.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mp3_shared.h"
#include <fat.h>
#include <filesystem.h>
#include <nds.h>

#include "atlas.h"
#include "atlas_texture.h"
#include "bfbg.h"
#include "bg1_.h"
#include "bg2_.h"
#include "btf.h"
#include "ds.h"
#include "dsbg.h"
#include "fd.h"
#include "mario_winportrait.h"
#include "winscreen.h"

const uint32_t screen_width = 256;
const uint32_t screen_height = 192;

int delay = 0;
int frame = 0;
int finalframe = 8;
int initialframe = 0;
int state = 1;
int goal = 5;
int x = 120;
int y = 127;
int ground_level = 128;
int tapcount = 0;
int tap = 0;
int bi = 0;
int pi = 0;
int wait = 0;
int portmove = 20;
bool draw = true;

s16 scrollX = 256;
s16 scrollY = 270;
wav_handle *sonido;
wav_handle *airdodgem;
wav_handle *dash;
wav_handle *attack1;
wav_handle *attackspin;
wav_handle *mdjump;
wav_handle *spinsfx;

typedef struct {
      const char *name;
      float air_acceleration;
      float air_speed;
      float fall_speed;
      float gravity;
      float jump_height;
      float jump_duration;
      float weight;
      float landing;
      float walk_speed;
      float dash_speed;
      float run_speed;
      float spot_dodge;
      float roll;
      float air_dodge;
      float grab_range;
      float reflector;
      float direction;
      float velY;
      float velX;
      float posY;
      float posX;
      int grounded;
      int jump;
      int onair;
      int walking;
      int dodging;
      int run;
      int attacking;
} Character;

Character mario = {
    .name = "Mario",
    .air_acceleration = 0.0f,
    .air_speed = 1.20f,
    .fall_speed = 1.5f,
    .gravity = 0.87f,
    .jump_height = 35.33f,
    .jump_duration = 2.0f,
    .weight = 98.0f,
    .landing = 0.0f,
    .walk_speed = 2.1f,
    .dash_speed = 0.0f,
    .run_speed = 0.0f,
    .spot_dodge = 0.0f,
    .roll = 0.0f,
    .air_dodge = 0.0f,
    .grab_range = 0.0f,
    .reflector = 0.0f,
    .direction = 1,
    .velY = 0,
    .velX = 0,
    .posY = 127,
    .posX = 120,
    .grounded = true,
    .jump = 0,
    .onair = true,
    .walking = false,
    .dodging = false,
    .run = false,
    .attacking = false,
};

float getPosX() { return mario.posX; }
float getPosY() { return mario.posY; }
float getVelX() { return mario.velX; }
float getVelY() { return mario.velY; }
void setPos(float x, float y) {
      mario.posX = x;
      mario.posY = y;
}
void setVel(float x, float y) {
      mario.velX = x;
      mario.velY = y;
}

void stand() { // funciones de acciones de mario

      if (state != 1 && mario.grounded && !mario.attacking) {
            mario.attacking = false;
            mario.dodging = false;
            mario.grounded = true;
            state = 1;
            goal = 5;
            initialframe = 0;
            frame = initialframe;
            finalframe = 8;
            mario.velX = 0;
      }
}
void marioJump(Character *player) {

      if (mario.jump >= 0 && mario.jump <= 2 && !mario.attacking &&
          !mario.dodging) {
            mario.jump++;
            setVel(getVelX(), -550);
            wav_play(sonido);
            state = 7;
            player->onair = true;
            player->grounded = false;
            player->walking = false;
            initialframe = 16;
            finalframe = 16;
            frame = initialframe;
      }
      if (mario.jump == 2 && !mario.attacking) {
            mario.jump++; // double jump
            goal = 3;
            wav_play(mdjump);
            state = 12;
            player->onair = true;
            player->grounded = false;
            player->walking = false;
            initialframe = 48;
            finalframe = 58;
            frame = initialframe;
      }
}

void mariofall() {
      mario.attacking = false;
      state = 10;
      goal = 5;
      initialframe = 37;
      frame = initialframe;
      finalframe = 37;
}

void move() {

      if (state != 2 && mario.grounded && tap < 2 && !mario.attacking) {
            state = 2;
            mario.walking = true;
            initialframe = 8;
            frame = initialframe;
            finalframe = 16;
      }

      if (mario.walking && mario.grounded && !mario.attacking) {

            if (mario.direction == 1) {
                  setVel(200, getVelY());
            } else if (mario.direction == -1) {
                  setVel(-200, getVelY());
            }
      }
}

void mariorun() {

      if (state != 8 && mario.grounded && tap >= 2 && !mario.attacking) {
            wav_play(dash);
            state = 8;
            mario.walking = false;
            mario.run = true;
            initialframe = 18;
            frame = initialframe;
            finalframe = 26;
            if (mario.run && mario.grounded) {
                  if (mario.direction == 1) {
                        setVel(255, getVelY());
                  } else if (mario.direction == -1) {
                        setVel(-255, getVelY());
                  }
            }
      }
}

void marioairdodge(Character *player, float dodgex, float dodgey) {

      if (mario.onair && mario.dodging == false && !mario.attacking) {
            wav_play(airdodgem);
            mario.attacking = false;
            mario.dodging = true;
            initialframe = 17;
            finalframe = 17;
            frame = initialframe;
            setVel(dodgex, dodgey); // Velocidad inicial de salto hacia arriba
            state = 8;
      }
}

void jab1() {

      state = 3;
      initialframe = 16;
      frame = initialframe;
      finalframe = 21;
}
void jab2() {
      state = 4;
      initialframe = 20;
      frame = initialframe;
      finalframe = 26;
}
void jab3() {
      state = 5;
      initialframe = 25;
      frame = initialframe;
      finalframe = 33;
}
void stilt() {
      state = 6;
      initialframe = 33;
      frame = initialframe;
      finalframe = 41;
}
void mariobair() {

      if ((state != 9) && (mario.onair) && (!mario.attacking))
            wav_play(attack1);
      goal = 3;
      state = 9;
      initialframe = 26;
      frame = initialframe;
      finalframe = 38;
}
void mariodownb() {
      if ((state != 11) && (!mario.attacking)) {

            mario.attacking = true;
            wav_play(spinsfx);
            wav_play(attackspin);

            goal = 2;
            state = 11;
            initialframe = 38;
            frame = initialframe;
            finalframe = 49;
      }
      if (mario.grounded) {
            setVel(getVelX() * 0.5, getVelY());
      }
      if (mario.onair) {
            setVel(getVelX() * 0.6, -300);
      }
}
void fadein() {
      for (bi = -31; bi < 0; bi++) {

            setBrightness(3, bi);
            swiWaitForVBlank();
      }
}

glImage ruins[ATLAS_NUM_IMAGES];

void deleteTextures(glImage *ruins, int num_images) {
      int i = 0;
      for (i = 0; i < num_images; ++i) {
            glDeleteTextures(1, &ruins[i].textureID);
      }
}

int main(int argc, char **argv) {

      mp3_init();
      fatInitDefault();
      consoleDemoInit();
      sonido = wav_load_handle("/data/strike/sfx/mario/m_jump1.wav");
      airdodgem = wav_load_handle("/data/strike/sfx/mario/m_airdodge.wav");
      dash = wav_load_handle("/data/strike/sfx/mario/m_dash.wav");
      attack1 = wav_load_handle("/data/strike/sfx/mario/m_attack1.wav");
      attackspin = wav_load_handle("/data/strike/sfx/mario/m_spin.wav");
      spinsfx = wav_load_handle("/data/strike/sfx/mario/m_spinsfx.wav");
      mdjump = wav_load_handle("/data/strike/sfx/mario/m_djump.wav");
      // Initialize OpenGL to some sensible defaults
      glScreen2D();

      videoSetMode(MODE_5_3D);

      // Setup some memory to be used for textures and for texture palettes

      vramSetBankA(VRAM_A_MAIN_BG);
      vramSetBankB(VRAM_B_TEXTURE);
      vramSetBankG(VRAM_G_TEX_PALETTE);
      vramSetBankE(VRAM_E_LCD);
      bgExtPaletteEnable();

      int bg2 = bgInit(2, BgType_ExRotation, BgSize_ER_512x512, 2, 3);
      int bg3 = bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 1, 1);

      dmaCopy(bfbgTiles, bgGetGfxPtr(bg3), bfbgTilesLen);
      dmaCopy(bfbgMap, bgGetMapPtr(bg3), bfbgMapLen);
      dmaCopy(bfbgPal, &VRAM_E_EXT_PALETTE[bg3][0], bfbgPalLen);

      dmaCopy(btfTiles, bgGetGfxPtr(bg2), btfTilesLen);
      dmaCopy(btfMap, bgGetMapPtr(bg2), btfMapLen);
      dmaCopy(btfPal, &VRAM_E_EXT_PALETTE[2][0], btfPalLen);
      vramSetBankE(VRAM_E_BG_EXT_PALETTE);
      glClearColor(0, 0, 0, 0);

      int ruins_texture_id = glLoadSpriteSet(
          ruins,               // Pointer to glImage array
          ATLAS_NUM_IMAGES,    // Number of images
          ATLAS_texcoords,     // Array of UV coordinates
          GL_RGB256,           // Texture type for glTexImage2D()
          ATLAS_BITMAP_WIDTH,  // Full texture size X (image size)
          ATLAS_BITMAP_HEIGHT, // Full texture size Y (image size)
          // Parameters for glTexImage2D()
          TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
          256,                  // Length of the palette to use (256 colors)
          atlas_texturePal,     // Pointer to texture palette data
          atlas_textureBitmap); // Pointer to texture data

      if (ruins_texture_id < 0)
            printf("Failed to load texture: %d\n", ruins_texture_id);

      float sprite_offsets_x[] = {
          -5,  -7,  -8,  -8,  -8,  -8,  -7,  -5, // stand       --izq ++ der
          -6,  -9,  -11, -9,  -3,  -9,  -11, -7, // walk
          -6,                                    // jump
          -12,                                   // airdodge
          -8,  -8,  -8,  -8,  -8,  -8,  -8,  -8, // run
          -14, -14, -14, -29, -30, -30, -24, -24, -22, -19, -12, -17, // bair
          -21, -24, -14, -31, -27, -31, -31, -30, -30, -29, -12, -12,
          -12, -12, -12, -12, -12, -12, -12}; // downb

      float sprite_offsets_y[] = {
          1,  0,   0,   1,   2,   0,   0,   2, //  ++abajo --arriba
          1,  1,   0,   2,   1,   1,   1,   2, // walk
          -1, 0,   4,   4,   4,   4,   4,   4,   4,   4,   0,
          0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // bair
          4,  -11, -11, -11, -26, -27, -27, -27, -26, -16,    // downb
          0,  0,   0,   0,   0,   0,   0,   0,   0};

      s16 scaleX = 1 << 8;
      s16 scaleY = 1 << 8;
      s16 rcX = 128;
      s16 rcY = 96;
      s16 angle = 0;
      float timeScale = 1;
      float dt = 0;
      dt = timeScale;
      mp3_play("/data/strike/music/bf.mp3", 1, 0);
      int isPlaying = mp3_is_playing();
      stand();

      while (1) {
            mp3_fill_buffer();

            // Synchronize game loop to the screen refresh
            swiWaitForVBlank();
            // Set up GL2D for 2D mode
            glBegin2D();

            printf("Wait: %d\n", wait);
            printf("Tapcount: %d\n", tapcount);
            printf("Tap: %d\n", tap);
            printf("jump count: %d\n", mario.jump);
            printf("is attacking: %s\n", (mario.attacking ? "true" : "false"));
            printf("MP3 is playing: %s\n", (isPlaying ? "true" : "false"));

            u32 flip = (mario.direction == 1) ? GL_FLIP_NONE : GL_FLIP_H;

            if (draw) {
                  glSprite(mario.posX + sprite_offsets_x[frame],
                           mario.posY + sprite_offsets_y[frame], flip,
                           &ruins[frame]);
            }

            dt *= timeScale;
            mario.posX += mario.velX / 100 * dt;
            mario.posY += mario.velY / 100 * dt;
            // gravedad
            mario.velY += 20 * dt;

            // revisar si mario esta en el suelo
            if (mario.posY >= ground_level) {
                  mario.grounded = true;
            } else {
                  mario.grounded = false;
            }

            // mario está en el suelo
            if (mario.grounded) {
                  mario.posY = ground_level;
                  mario.velY = 0;
                  mario.jump = 0;
            }

            if ((mario.velX == 0) & (mario.grounded == true)) {
                  mario.walking = false;
                  stand();
            }

            if (mario.onair == true && mario.grounded == true) {

                  mario.onair = false;
                  mario.velX = 0;
                  stand();
            }

            // sistema de animación
            delay++;
            if (delay >= goal) {
                  delay = 0;

                  frame++;
                  if (frame >= finalframe)
                        frame = initialframe;
            }
            // double tap for run detection
            tapcount--;
            if (tapcount <= 0) {
                  tapcount = 0;
            }

            if (tap > 2) {
                  tap = 2;
            }
            // mario jump limit
            if (mario.jump >= 3 && mario.onair) {
                  mario.jump = 3;
            }
            // reset mario jump limit
            if (mario.jump >= 3 && mario.grounded) {
                  mario.jump = 0;
            }

            if (scrollX < 180 && !draw && wait >= 3) {
                  // Si ScrollX es mayor que 120, se queda en ese valor y
                  // siempre se incrementa
                  scrollX += portmove;
            }
            if (scrollX >= 130 && !draw) {
                  portmove = 1;
            }

            /*
            printf("Angle %3d(actual) %3d(degrees)\n", angle, (angle * 360) /
           (1<<15)); printf("Scroll  X: %4d Y: %4d\n", scrollX, scrollY);
               printf("Rot center X: %4d Y: %4d\n", rcX, rcY);
               printf("Scale X: %4d Y: %4d\n", scaleX, scaleY);
           printf("Frame: %d, PosX: %.2f, PosY: %.2f, VelX: %.2f, VelY: %.2f,
           State: %d\n, dir: %.2f\n", frame, mario.posX, mario.posY, mario.velX,
           mario.velY, state, mario.direction); printf("Mario.grounded: %s\n",
           (mario.grounded ? "true" : "false")); printf("Mario.onair: %s\n",
           (mario.onair ? "true" : "false")); printf("Mario.walk: %s\n",
           (mario.walking ? "true" : "false")); printf("Mario.dodge: %s\n",
           (mario.dodging ? "true" : "false"));
               */

            scanKeys();
            u32 keyu = keysUp();

            if ((keyu & (KEY_LEFT | KEY_RIGHT)) && mario.grounded == true &&
                mario.attacking == false) {

                  stand();
            }

            u32 keys = keysHeld();

            // moverse
            if (keys & KEY_RIGHT) {

                  move();
            }
            if (keys & KEY_LEFT) {

                  move();
            }
            if (state == 9 && frame == 37) {
                  mariofall();
            }
            if (state == 11 && frame >= 48 && mario.onair) {
                  mario.attacking = false;
                  mariofall();
            }
            if (state == 12 && frame >= 57) {
                  mario.attacking = false;
                  mariofall();
            }
            if (state == 11 && frame >= 48 && mario.grounded) {
                  mario.attacking = false;
                  stand();
            }

            uint16_t keysd = keysDown();
            if (keys & KEY_RIGHT && mario.grounded == true) {

                  mario.direction = 1;
            }

            if (keys & KEY_LEFT && mario.grounded == true) {

                  mario.direction = -1;
            }
            if (keys & KEY_RIGHT && mario.onair == true &&
                mario.dodging == false) {

                  setVel(220, getVelY());
            }
            if (keys & KEY_LEFT && mario.onair == true &&
                mario.direction == 1 && mario.dodging == false) {

                  setVel(-180, getVelY());
            }
            if (keys & KEY_LEFT && mario.onair == true &&
                mario.dodging == false) {

                  setVel(-220, getVelY());
            }
            if (keys & KEY_RIGHT && mario.onair == true &&
                mario.direction == -1 && mario.dodging == false) {

                  setVel(180, getVelY());
            }

            if (keyu & KEY_LEFT && mario.onair == true &&
                mario.direction == -1 &&
                mario.dodging == false) { // on key up facing direction

                  setVel(-130, getVelY());
            }
            if (keyu & KEY_RIGHT && mario.onair == true &&
                mario.direction == 1 && mario.dodging == false) {

                  setVel(140, getVelY());
            }

            if (keyu & KEY_LEFT && mario.onair == true &&
                mario.direction == 1 &&
                mario.dodging == false) { // key up opposite air direction

                  setVel(-100, getVelY());
            }
            if (keyu & KEY_RIGHT && mario.onair == true &&
                mario.direction == -1 && mario.dodging == false) {

                  setVel(100, getVelY());
            }

            if (keysd & (KEY_Y | KEY_X)) {

                  marioJump(&mario);
            }
            if ((keyu & (KEY_X | KEY_Y)) && mario.velY < -480 &&
                mario.onair) { // shorthop

                  setVel(getVelX(), -200);
            }
            if ((keysd & KEY_L) &&
                !(keysHeld() & (KEY_UP | KEY_DOWN | KEY_RIGHT | KEY_LEFT)) &&
                mario.onair) {
                  // Realiza la acción si el botón L está presionado y ninguno
                  // de los botones direccionales está presionado
                  mario.direction = mario.direction;
                  marioairdodge(&mario, getVelX(), -300);
            }
            if ((keysd & KEY_L) && (keys & KEY_DOWN) &&
                mario.onair) { // airdodge
                  mario.direction = mario.direction;
                  marioairdodge(&mario, getVelX(), 300);
            }
            if ((keysd & KEY_L) && (keys & KEY_RIGHT) && mario.onair) {
                  marioairdodge(&mario, 340, -80);
            }
            if ((keysd & KEY_L) && (keys & KEY_LEFT) && mario.onair) {
                  marioairdodge(&mario, -340, -80);
            }

            // Detectar la presión inicial y cambiar la dirección
            if ((keysd & (KEY_LEFT | KEY_RIGHT)) && mario.grounded &&
                tap <= 2) {
                  tapcount = 30;
                  tap++;
            }

            // Mantener la dirección mientras se mantiene presionada la tecla
            if (keysHeld() & (KEY_LEFT | KEY_RIGHT) && mario.grounded) {
                  mariorun();
            }

            // Reiniciar tap si se presiona la tecla opuesta
            if ((keysd & KEY_LEFT) && (keysHeld() & KEY_RIGHT) &&
                mario.grounded) {
                  tap = 0;
                  tapcount = 20;
            }
            if ((keysd & KEY_RIGHT) && (keysHeld() & KEY_LEFT) &&
                mario.grounded) {
                  tap = 0;
                  tapcount = 20;
            }

            // Reiniciar tap si se deja de presionar las teclas de dirección y
            // tap >= 2
            if (!(keysHeld() & (KEY_LEFT | KEY_RIGHT)) && (tap >= 2) &&
                (tapcount < 6) && (tapcount >= 0)) {
                  tap = 0;
            }

            // Reiniciar tap si tapcount está en un rango específico y tap <= 1
            if ((tapcount <= 20) && (tapcount >= 0) && (tap <= 1)) {
                  tap = 0;
            }

            // Mantener la dirección mientras se mantiene presionada la tecla
            if (keysHeld() & (KEY_LEFT | KEY_RIGHT) && mario.grounded) {
                  mariorun();
            }
            if ((keysd & KEY_A) && (keysHeld() & KEY_LEFT) && mario.onair &&
                mario.direction == 1 && mario.dodging == false && state != 9) {

                  mariobair();
            } // bair
            if ((keysd & KEY_A) && (keysHeld() & KEY_RIGHT) && mario.onair &&
                mario.direction == -1 && mario.dodging == false && state != 9) {

                  mariobair();
            } // bair
            if ((keysd & KEY_B) && (keysHeld() & KEY_DOWN) &&
                mario.dodging == false && state != 11) {

                  mariodownb();
            }
            if (keysd & KEY_SELECT && draw) {
                  setBrightness(3, -31);
                  draw = false;
                  mp3_stop("/data/strike/music/bf.mp3", 1, 0);
                  deleteTextures(ruins, ATLAS_NUM_IMAGES);
                  bi = -31;
                  scrollX = -85;
                  scrollY = 118;
                  mp3_play("/data/strike/victory/mario.mp3", 0, 0);
            }
            if (wait == 2 && !draw) {
                  vramSetBankE(VRAM_E_LCD);
                  int bg3 =
                      bgInit(3, BgType_ExRotation, BgSize_ER_256x256, 1, 1);
                  dmaCopy(winscreenTiles, bgGetGfxPtr(bg3), winscreenTilesLen);
                  dmaCopy(winscreenMap, bgGetMapPtr(bg3), winscreenMapLen);
                  dmaCopy(winscreenPal, &VRAM_E_EXT_PALETTE[bg3][0],
                          winscreenPalLen);
                  int bg2 =
                      bgInit(2, BgType_ExRotation, BgSize_ER_256x256, 2, 3);
                  dmaCopy(mario_winportraitTiles, bgGetGfxPtr(bg2),
                          mario_winportraitTilesLen);
                  dmaCopy(mario_winportraitMap, bgGetMapPtr(bg2),
                          mario_winportraitMapLen);
                  dmaCopy(mario_winportraitPal, &VRAM_E_EXT_PALETTE[2][0],
                          mario_winportraitPalLen);
                  vramSetBankE(VRAM_E_BG_EXT_PALETTE);
            }
            //si la espera llega al frame 3
            if (wait == 3 && !draw) {
                 
                  fadein();
            }
            //draw es false
            if (!draw) {
                  wait++;
            }
            //limite de espera
            if (wait >= 4) {

                  wait = 4;
            }

            if (keys & KEY_LEFT) {
                  scrollX--;
            }
            if (keys & KEY_RIGHT) {
                  scrollX++;
            } 
            /*
            if( keys & KEY_UP ) {
            scrollY--; }
            if( keys & KEY_DOWN ){
                    scrollY++;
            }
    */

            /*	if( keys & KEY_A ){


                    scaleX++; scaleY++;}
                    if( keys & KEY_B ) {
                    scaleX--; scaleY--;}
                    if( keys & KEY_START ) rcX ++;
                    if( keys & KEY_SELECT ) rcY++;
                    if( keys & KEY_L ) angle+=20;
                    if( keys & KEY_R ) angle-=20;
            //glSpriteScale(sprite_x, sprite_y, -1, GL_FLIP_H, &ruins[frame]);
           */ 

            bgSetCenter(bg2, rcX, rcY);

            bgSetRotateScale(bg2, angle, scaleX, scaleY);

            bgSetScroll(bg2, scrollX, scrollY);

            // glBoxFilled(120, 120, 127, 127, 0x7C00);
            bgUpdate();

            glEnd2D();

            glFlush(0);
      }

      glDeleteTextures(1, &ruins_texture_id);

      return 0;
}

