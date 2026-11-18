//---------------------------------------------------------
// file:	utils.h
// author:	
// email:	
//
// brief:	
//		
//
// Copyright © 2026 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "cprocessing.h"
#include "game.h"
#include <stdlib.h>

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);


typedef struct Animation
{
	CP_Image img;
	int   img_w, img_h;
	int   frame_count;
	float frame_time;   // seconds per frame
	int   frame;
	float timer;
	int   loop;         // 1 = loops, 0 = plays once
} Animation;

typedef enum {
	BOSS_IDLE,
	BOSS_CHASE,
	BOSS_ATTACK1,
	BOSS_ATTACK2,
	BOSS_DEAD
} BossState;

typedef struct Boss {
	float x, y;
	float width, height;      // hitbox
	float draw_w, draw_h;     // sprite size
	float speed;
	float hp, maxHp;
	int   phase;
	int   is_right;
	BossState state;
	float stateTimer;
	float attackCooldown;
	int   contactDmg;
	Animation attack;
} Boss;

void Boss_Init(Boss* b);
void Boss_Update(Boss* b, float dt, float knightX, float knightY);
void Boss_Draw(Boss* b);
void Boss_TakeDamage(Boss* b, float dmg);
void Boss_Free(Boss* b);

typedef enum { IDLE, RUN, ATTACK } PlayerState;

typedef struct Melee {
	float x, y, width, height, timer;
	int state;
} Melee;

typedef struct Player
{
	float x, y;
	float speed;
	float width;
	float height;
	int is_right;
	int iFrame;
	float iFrameTimer;
	PlayerState state;
	Animation idle, run, attack;
} Player;

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int counter;
	int chase;
	int is_right;
	int dead;
	float shootTimer;
	PlayerState state;
	Animation idle, run, attack;
} Enemy;

void Anim_Init(Animation* a, const char* path, int fw, int fh, float frame_time, int loop);
void Anim_Reset(Animation* a);
int  Anim_Update(Animation* a, float dt);
void Anim_Draw(const Animation* a, float x, float y, float w, float h);
//=======
float findQ(float value, float min, float max);

//>>>>>>> main

//---------------------------------------------------------
//Darrel

//int IsSquareCircleColliding(float square_center_x, float square_center_y, float square_width, float square_height, float circle_center_x, float circle_center_y, float circle_radius);
int IsSquareColliding(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h, float* overlapX, float* overlapY);
int IsSquareColliding2(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h);

int euclidean_distance(int x1, int y1, int x2, int y2);
unsigned int random_range(unsigned int min, unsigned int max);

void SpawnBoost(BoostType* type, float* x, float* y, int* collected, float* timer, float windowW, float windowH);