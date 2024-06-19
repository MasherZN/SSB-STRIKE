
#include <stdio.h>
#include <math.h>
#include <gl2d.h>
#include <nds.h>
#include "bg1_.h"
#include "bg2_.h"
#include "fd.h"
#include "atlas_texture.h"
#include "atlas.h"
 	
	int delay = 0;
    int frame = 0;
    int finalframe= 8;
    int initialframe=0;
    int state=1;
    int goal=5;
    int x=120;
    int y=127;
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
    int jump;
    int onair;
    int walking;
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
    .walk_speed = 2.1f,  // Por ejemplo, Mario camina más rápido
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
    .jump=0,
    .onair=true,
    .walking=false,
};
    

 	float getPosX() {return mario.posX;}
	float getPosY() {return mario.posY;}
	float getVelX() {return mario.velX;}
	float getVelY() {return mario.velY;}
    void setPos(float x, float y) {mario.posX = x; mario.posY = y;}
	void setVel(float x, float y) {mario.velX = x; mario.velY = y;}
	

     void stand(){
     	if(state!=1){
    	state=1;
    	initialframe=0;
    	frame=initialframe;
    	finalframe=8;
    	mario.grounded=true;
    	mario.velX=0;
    	}
    }
    void marioJump(Character *player) {
   
       if(mario.jump==0){
       	player->onair=true;
       	player->grounded = false;
       	player->walking=false;
	   initialframe=16;
	   finalframe=16;
	   frame=initialframe;
       setVel(getVelX(), -550);  // Velocidad inicial de salto hacia arriba
          // Ya no está en el suelo
        
	
        state=7;
         	
}
mario.jump=1;
}

 void mariodJump(Character *player) {
 	player->onair=true;
   player->grounded = false;
       if(mario.jump==1 && mario.velY<=-100){
	   
       setVel(getVelX(), mario.velY-500);  // Velocidad inicial de salto hacia arriba
          // Ya no está en el suelo
         
}
   mario.jump=2;
}


  void move(int *sprite_x, int *sprite_y) {
    // Cambiar el estado y las frames de mario
    if(state!=2 && mario.grounded==true){	
	state = 2;
	mario.walking=true;
    initialframe = 8;
    frame = initialframe;
    finalframe = 16;
    
     }
if (mario.walking && mario.grounded) {
    // Movimiento en el suelo
    if (mario.direction == 0) {
        setVel(200, getVelY());
    } else if (mario.direction == 1) {
        setVel(-200, getVelY());
    }
    
    }
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
	                          0, -3, -5, -2, 3, -3, -5, -1, //walk
							  0 };
	                          
	                          
	                          
    int sprite_offsets_y[] = {1, 0, 0, 1, 2, 0, 0, 2,   //  ++abajo --arriba
							  1,1,0,2,1,1,1,2,         //walk   
							  0};      


	s16 angle = 0;
	
	s16 scrollX = 256; 
	s16 scrollY = 270;

	//scale is fixed point
	s16 scaleX = 1 << 8;
	s16 scaleY = 1 << 8;

	//this is the screen pixel that the image will rotate about
	s16 rcX = 128;
	s16 rcY = 96;
	
	float timeScale = 1;
	float dt = 0;
 	dt = timeScale;
    stand();
	
    while (1)
    {
        // Synchronize game loop to the screen refresh
        swiWaitForVBlank();
        // Set up GL2D for 2D mode
        glBegin2D();
        
        
         dt *= timeScale;
   		 mario.posX += mario.velX / 100 * dt;
   		 mario.posY += mario.velY / 100 * dt;
    
   u32 flip = (mario.direction == 0) ? GL_FLIP_NONE : GL_FLIP_H;
    // Apply gravity
    mario.velY += 20 * dt;
    
    // Check if Mario is grounded
 if (mario.posY >= ground_level) {
        mario.grounded = true;
    } else {
        mario.grounded = false;
    }
    
    // Si está en el suelo, ajustar posición y cambiar a estado de stand
    if (mario.grounded) {
        mario.posY = ground_level;  // Ajusta la posición Y a la altura del suelo
        mario.velY = 0;  // Detiene cualquier velocidad vertical residual
        mario.jump = 0;  // Reinicia el estado de salto
         // Cambia al estado de estar de pie
    }




if (mario.velX==0 & mario.grounded==true){
	mario.walking=false;
	stand(); 
}
     
	if (mario.onair==true && mario.grounded==true) {
    
    	mario.onair=false;
        mario.velX=0;
   		stand();
		  }
            
            // Draw animated sprite 
           glSprite(mario.posX + sprite_offsets_x[frame], mario.posY + sprite_offsets_y[frame], 
             flip, &ruins[frame]);   
            // Animate (change animation frame every 10 frames)
           delay++;
            if (delay == goal)
            {
                delay = 0;

                frame++;
                if (frame >= finalframe)
                    frame = initialframe;
            }
        //printf("Angle %3d(actual) %3d(degrees)\n", angle, (angle * 360) / (1<<15));
		//printf("Scroll  X: %4d Y: %4d\n", scrollX, scrollY);
	//	printf("Rot center X: %4d Y: %4d\n", rcX, rcY);
		//printf("Scale X: %4d Y: %4d\n", scaleX, scaleY);
       printf("Frame: %d, PosX: %.2f, PosY: %.2f, VelX: %.2f, VelY: %.2f, State: %d\n, dir: %d", 
       frame, mario.posX, mario.posY, mario.velX, mario.velY, state, mario.direction);
       printf("Mario.grounded: %s\n", (mario.grounded ? "true" : "false"));
        printf("Mario.onair: %s\n", (mario.onair ? "true" : "false"));
		 printf("Mario.walk: %s\n", (mario.walking ? "true" : "false"));


        scanKeys();
        
        
        
        u32 keyu = keysUp();
        
        
		if ((keyu & (KEY_LEFT | KEY_RIGHT)) && mario.grounded==true) {

	stand();
	  
	}
		
		u32 keys = keysHeld();
		
		//moverse
		if (keys & KEY_RIGHT) {
    
   		move(&x, &y);
		}
		if (keys & KEY_LEFT) {
   
   
   		move(&x, &y);
		}
	
		 uint16_t keysd = keysDown();
       if (keys & KEY_RIGHT&& mario.grounded==true ) {
    
   		 mario.direction=0;
		}
		
		
		if (keys & KEY_LEFT && mario.grounded==true) {
    
   		 mario.direction=1;
		}
		if (keys & KEY_RIGHT&& mario.onair==true ) {
    
   		  setVel(220, getVelY());
		}
			if (keys & KEY_LEFT&& mario.onair==true&& mario.direction==0 ) {
    
   		  setVel(-180, getVelY());
		}
			if (keys & KEY_LEFT&& mario.onair==true ) {
    
   		  setVel(-220, getVelY());
		}
			if (keys & KEY_RIGHT&& mario.onair==true&& mario.direction==1 ) {
    
   		  setVel(180, getVelY());
		}
		
		
			if (keyu & KEY_LEFT&& mario.onair==true && mario.direction==1) {
    
   		  setVel(-130, getVelY());
		}
			if (keyu & KEY_RIGHT&& mario.onair==true && mario.direction==0) {
    
   		  setVel(140, getVelY());
		}
		
		
		if (keysd &( KEY_Y  | KEY_X)) {
			
			 marioJump(&mario);
		
   	
		}
			if ((keyu & (KEY_X | KEY_Y)) && mario.velY<-480 && mario.onair) { //shorthop
    
   		  setVel(getVelX(), -200);
		}
		
		
	
	/*	if( keys & KEY_LEFT ){
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
	*/	 
       
		
		
		if( keys & KEY_A ){ 
			
			
		scaleX++; scaleY++;}
		if( keys & KEY_B ) {
		scaleX--; scaleY--;}  
		if( keys & KEY_START ) rcX ++;
		if( keys & KEY_SELECT ) rcY++;
		if( keys & KEY_L ) angle+=20;
		if( keys & KEY_R ) angle-=20;
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

