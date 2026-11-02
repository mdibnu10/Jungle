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
#define MAX_OBSTACLE 3
#define MAX_BULLETS 5

void Game_Init(void);

void Game_Update(void);

void Game_Exit(void);

typedef struct MapData
{
    const char* image_path;
    CP_Image image;        
  
} MapData;

typedef struct {
	float x;
	float y;
	int state; // 0 inactive, 1 active
	float size;
	float angle;
	float speed_x;
	float speed_y;
} Bullet;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Obstacle;


extern int random[LEVELS];
extern int current_level;
extern MapData maps[LEVELS][MAPS_PER_LEVEL];