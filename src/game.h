//---------------------------------------------------------
// file:	game.h
// author:	
// email:	
//
// brief:	
//		
//
// Copyright © 2026 DigiPen, All rights reserved.
//---------------------------------------------------------
#define LEVELS 3
#define MAPS_PER_LEVEL 3

void Game_Init(void);

void Game_Update(void);

void Game_Exit(void);

typedef struct MapData
{
    const char* image_path;
    CP_Image image;        
  
} MapData;

extern int random[LEVELS];
extern int current_level;
extern MapData maps[LEVELS][MAPS_PER_LEVEL];