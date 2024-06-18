
//{{BLOCK(bg1_)

//======================================================================
//
//	bg1_, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 443 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 28352 + 2048 = 30912
//
//	Time-stamp: 2024-06-12, 17:46:20
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG1__H
#define GRIT_BG1__H

#define bg1_TilesLen 28352
extern const unsigned int bg1_Tiles[7088];

#define bg1_MapLen 2048
extern const unsigned short bg1_Map[1024];

#define bg1_PalLen 512
extern const unsigned short bg1_Pal[256];

#endif // GRIT_BG1__H

//}}BLOCK(bg1_)
