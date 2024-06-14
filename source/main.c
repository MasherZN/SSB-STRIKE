
#include <stdio.h>
#include <math.h>
#include <gl2d.h>
#include <nds.h>
#include "bg1_.h"
#include "bg2_.h"
#include "fd.h"
#include "atlas_texture.h"
#include "atlas.h"
float timeScale = 1;
int delay = 0;
    int frame = 0;
    int finalframe= 8;
    int initialframe=0;
    int state=1;
    int goal=5;
    int sprite_x=120;
    int sprite_y=127;
    int ground_level=128;
    

typedef struct {
    const char *name;  // Nombre del personaje
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
    int direction;
    float velY;
    float velX;
    float posY;
    float posX;
    int grounded;
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
    .walk_speed = 1.0f,  // Por ejemplo, Mario camina más rápido
    .dash_speed = 0.0f,
    .run_speed = 0.0f,
    .spot_dodge = 0.0f,
    .roll = 0.0f,
    .air_dodge = 0.0f,
    .grab_range = 0.0f,
    .reflector = 0.0f,
    .direction = 0,
    .velY=0,
    .velX=0,
    .posY=127,
    .posX=120,
    .grounded=true,
};
    
void tick(Character *mario, float dt) {
    dt *= timeScale;
    mario->posX += mario->velX / 100 * dt;
    mario->posY += mario->velY / 100 * dt;
    
    // Apply gravity
    mario->velY -= 20 * dt;
    
    // Check if Mario is grounded
    mario->grounded = (mario->posY <= 127);
    
    // If grounded, adjust posY and velocity
    if (mario->grounded == true) {  // Use == for comparison, not =
        mario->posY = 127;
        if (mario->velY > 0) {
            mario->velY = 0;
        }
    }
}
    
     void stand(){
     	if(state!=1){
    	state=1;
    	initialframe=0;
    	frame=initialframe;
    	finalframe=8;
    	}
    }
    void marioJump(Character *player) {
    player->velY = 40.0f;  // Establece la velocidad vertical a -500 (ejemplo)
    // Aquí podrías agregar lógica adicional relacionada con el salto, como cambiar la animación, ajustar el estado del jugador, etc.
}
  void walk(int *sprite_x, int *sprite_y) {
    // Cambiar el estado y las frames de mario
    if(state!=2){
	state = 2;
    initialframe = 8;
    frame = initialframe;
    finalframe = 16;
     }
     
    if(mario.direction==0){
    	*sprite_x += mario.walk_speed;
    	
	} if(mario.direction==1){
		*sprite_x -= mario.walk_speed;
			}
	}
	
   void setVelY(Character *mario, float velY) {
    mario->velY = velY;
    
    
}
   
 

    void jab1(){
    	
    	state=3;
    	initialframe=16;
    	frame=initialframe;
    	finalframe=21;

	}
	void jab2(){
		state=4;
    	initialframe=20;
    	frame=initialframe;
    	finalframe=26;
	}
	void jab3(){
		state=5;
    	initialframe=25;
    	frame=initialframe;
    	finalframe=33;
          
	}
	void stilt(){
		state=6;
    	initialframe=33;
    	frame=initialframe;
    	finalframe=41;
          
	}

const uint32_t screen_width = 256;
const uint32_t screen_height = 192;

glImage ruins[ATLAS_NUM_IMAGES];

int main(int argc, char **argv)
{
    consoleDemoInit();

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
    dmaCopy(bg1_Tiles, bgGetGfxPtr(bg3),bg1_TilesLen);
    dmaCopy(bg1_Map, bgGetMapPtr(bg3),bg1_MapLen);
    dmaCopy(bg1_Pal, &VRAM_E_EXT_PALETTE[bg3][0], bg1_PalLen); 
    
    dmaCopy(bg2_Tiles, bgGetGfxPtr(bg2),bg2_TilesLen);
    dmaCopy(bg2_Map, bgGetMapPtr(bg2),bg2_MapLen);
    dmaCopy(bg2_Pal, &VRAM_E_EXT_PALETTE[2][0], bg2_PalLen); 
   
    vramSetBankE(VRAM_E_BG_EXT_PALETTE); 
    glClearColor(0, 0, 0, 0);
    
    int ruins_texture_id =
        glLoadSpriteSet(ruins,    // Pointer to glImage array
                        ATLAS_NUM_IMAGES, // Number of images
                        ATLAS_texcoords,  // Array of UV coordinates
                        GL_RGB256,    // Texture type for glTexImage2D()
                        ATLAS_BITMAP_WIDTH,  // Full texture size X (image size)
                        ATLAS_BITMAP_HEIGHT, // Full texture size Y (image size)
                        // Parameters for glTexImage2D()
                        TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, 
                        256,          // Length of the palette to use (256 colors)
                        atlas_texturePal,     // Pointer to texture palette data
                        atlas_textureBitmap); // Pointer to texture data

    if (ruins_texture_id < 0)
        printf("Failed to load texture: %d\n", ruins_texture_id);
        
    // Print some controls
    printf("START:  Exit to loader\n");
    printf("\n");
    printf("MODE 5_3D \n");
    
    
     int sprite_offsets_x[] = {2, 0, -1, -1, -1, -1, 0, 2, //stand       --izq ++ der
	                          0, -3, -5, -2, 3, -3, -5, -1   }; //walk
	                          
	                          
	                          
    int sprite_offsets_y[] = {1, 0, 0, 1, 2, 0, 0, 2,
							  1,1,0,2,1,1,1,2};//walk              ++abajo --arriba


	s16 angle = 0;
	
	s16 scrollX = 256; 
	s16 scrollY = 270;

	//scale is fixed point
	s16 scaleX = 1 << 8;
	s16 scaleY = 1 << 8;

	//this is the screen pixel that the image will rotate about
	s16 rcX = 128;
	s16 rcY = 96;

    stand();
	
    while (1)
    {
        // Synchronize game loop to the screen refresh
        swiWaitForVBlank();
        // Set up GL2D for 2D mode
        glBegin2D();
        tick(&mario, 1.0f / 60.0f);
            // Fill screen with a gradient
          /* glBoxFilledGradient(0, 0,
                                screen_width - 50, screen_height - 50,
                                RGB15(31, 0, 0),
                                RGB15(31, 31, 0),
                                RGB15(31, 0, 31),
                                RGB15(0, 31, 31));
*/
            /*// Draw sprite frames individually
            glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(0));
            glColor(RGB15(31, 31, 31));
*/
            
            // Draw animated sprite 
            glSprite(mario.posX + sprite_offsets_x[frame], mario.posY + sprite_offsets_y[frame], GL_FLIP_NONE, &ruins[frame]);   

            // Animate (change animation frame every 10 frames)
           delay++;
            if (delay == goal)
            {
                delay = 0;

                frame++;
                if (frame >= finalframe)
                    frame = initialframe;
            }
            printf("Angle %3d(actual) %3d(degrees)\n", angle, (angle * 360) / (1<<15));
		printf("Scroll  X: %4d Y: %4d\n", scrollX, scrollY);
		printf("Rot center X: %4d Y: %4d\n", rcX, rcY);
		printf("Scale X: %4d Y: %4d\n", scaleX, scaleY);
         printf("mario.velY: %.2f\n", mario.velY);
		


        scanKeys();
        
        u32 keyu = keysUp();
		
		if ((keyu & (KEY_LEFT | KEY_RIGHT)) && state == 2) {
    // Llamar a la función y pasar los valores de sprite_X y sprite_y
   		stand();
		  
		}
		
		u32 keys = keysHeld();
		
		if (keys & KEY_RIGHT) {
    // Llamar a la función y pasar los valores de sprite_X y sprite_y
   		 walk(&sprite_x, &sprite_y);
		}
		if (keys & KEY_LEFT) {
    // Llamar a la función y pasar los valores de sprite_X y sprite_y
   		 walk(&sprite_x, &sprite_y);
		}
		
		
		
		
		
		if( keys & KEY_L ) angle+=20;
		if( keys & KEY_R ) angle-=20;
		if( keys & KEY_LEFT ){
			scrollX--; 
			
		}   
		if( keys & KEY_RIGHT ) {
			scrollX++;
		}
		if( keys & KEY_UP ) {
		scrollY--; }
		if( keys & KEY_DOWN ){
			scrollY++;
		}
		 
        uint16_t keysd = keysDown();
       if (keys & KEY_RIGHT) {
    
   		 mario.direction=0;
		}else{
		mario.direction=1;	
		}
		
		if (keys & KEY_Y) {
			
			 marioJump(&mario);
       
   	
		}
		
		if( keys & KEY_A ){ 
			
			
		scaleX++; scaleY++;}
		if( keys & KEY_B ) {
		scaleX--; scaleY--;}  
		if( keys & KEY_START ) rcX ++;
		if( keys & KEY_SELECT ) rcY++;
	
        //glSpriteScale(sprite_x, sprite_y, -1, GL_FLIP_H, &ruins[frame]);
        
		bgSetCenter(bg2, rcX, rcY);
		
		bgSetRotateScale(bg2, angle, scaleX, scaleY);
		
		bgSetScroll(bg2, scrollX, scrollY);
		
		bgUpdate();

        glEnd2D();

        glFlush(0);
    }

    glDeleteTextures(1, &ruins_texture_id);

    return 0;
}
