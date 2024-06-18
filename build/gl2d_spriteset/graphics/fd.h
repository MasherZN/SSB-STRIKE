
//{{BLOCK(fd)

//======================================================================
//
//	fd, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 601 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 64x64 
//	Total size: 512 + 38464 + 8192 = 47168
//
//	Time-stamp: 2024-06-13, 22:58:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FD_H
#define GRIT_FD_H

#define fdTilesLen 38464
extern const unsigned int fdTiles[9616];

#define fdMapLen 8192
extern const unsigned short fdMap[4096];

#define fdPalLen 512
extern const unsigned short fdPal[256];

#endif // GRIT_FD_H

//}}BLOCK(fd)
