
//{{BLOCK(bg2_)

//======================================================================
//
//	bg2_, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 835 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 64x64 
//	Total size: 512 + 53440 + 8192 = 62144
//
//	Time-stamp: 2024-06-18, 12:05:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG2__H
#define GRIT_BG2__H

#define bg2_TilesLen 53440
extern const unsigned int bg2_Tiles[13360];

#define bg2_MapLen 8192
extern const unsigned short bg2_Map[4096];

#define bg2_PalLen 512
extern const unsigned short bg2_Pal[256];

#endif // GRIT_BG2__H

//}}BLOCK(bg2_)
