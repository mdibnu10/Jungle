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
#define MAX_OBSTACLE 12
#define MAX_BULLETS 5
#define MAX_BULLET_DISTANCE 280.0f

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
	float start_x;
	float start_y;
} Bullet;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Obstacle;
//---------------------------------------------------------
typedef struct {
	float x;
	float y;
	float width;
	float height;
	int collected; // 0 not collected, 1 collected
} SpeedBoost;

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int noOfEffects;
} Effects;

typedef enum
{
	SPEED_BOOST,
	MORE_MAXHP,
	DAMAGE_BOOST,
	BOOST_COUNT
} BoostType;

typedef struct {
	float y;          // vertical position of the box
	float width;
	float height;
	float timeLeft;   // seconds remaining
	char text[8];    // the "01:00" string
} Timer;

typedef struct {
	float y;          // vertical position of the box
	float width;
	float height;
	int total;
	int amount;
	char text[10];    // the "0 / 10" string
} KillCount;
//---------------------------------------------------------

extern int random[LEVELS];
extern int current_level;
extern MapData maps[LEVELS][MAPS_PER_LEVEL];