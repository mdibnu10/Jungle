//---------------------------------------------------------
// file:	utils.c
// author:
// email:	
//
// brief:	
//		
//
// Copyright © 2026 DigiPen, All rights reserved.
//---------------------------------------------------------

#include <math.h>
#include "cprocessing.h"
#include "utils.h"
#include "game.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float aw = area_width;
	float ah = area_height;

	float ax = area_center_x;
	float ay = area_center_y;

	float x = click_x;
	float y = click_y;

	if (x >= ax - (aw / 2) && x <= ax + (aw / 2) && y >= ay - (ah / 2) && y <= ay + (ah / 2) && CP_Input_MouseClicked()) { // if mouse clicks on button
		return 1;
	}
	else {
		return 0;
	}
}

//<<<<<<< HEAD
void Anim_Init(Animation* a, const char* path, int fw, int fh, float frame_time, int loop)
{
	a->img = CP_Image_Load(path);
	a->img_w = fw;
	a->img_h = fh;
	a->frame_count = CP_Image_GetWidth(a->img) / fw;
	a->frame_time = frame_time;
	a->loop = loop;
	a->frame = 0;
	a->timer = 0.0f;
}

void Anim_Reset(Animation* a)
{
	a->frame = 0;
	a->timer = 0.0f;
}

int Anim_Update(Animation* a, float dt)
{
	a->timer += dt;
	if (a->timer >= a->frame_time)
	{
		a->timer -= a->frame_time;
		a->frame++;
		if (a->frame >= a->frame_count)
		{
			if (a->loop) a->frame = 0;
			else { a->frame = a->frame_count - 1; return 1; }
		}
	}
	return 0;
}

void Anim_Draw(const Animation* a, float x, float y, float w, float h)
{
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_DrawSubImage(a->img, x, y, w, h,
		(float)(a->frame * a->img_w), 0,
		(float)((a->frame + 1) * a->img_w), (float)a->img_h, 255);
}
//=======
float findQ(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
//>>>>>>> main
}
//---------------------------------------------------------
//Darrel
//---------------------------------------------------------
//int IsSquareCircleColliding(float square_center_x, float square_center_y, float square_width, float square_height, float circle_center_x, float circle_center_y, float circle_radius)
//{
//	if (
//		fabs(circle_center_x - square_center_x) <= (square_width / 2 + circle_radius) &&
//		fabs(circle_center_y - square_center_y) <= (square_height / 2 + circle_radius)
//		)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}

int IsSquareColliding(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h, float *overlapX, float *overlapY) {
	// Setting up boundary for Obstacle
	float ObsminX = sq1x - sq1w / 2.0f;
	float ObsmaxX = sq1x + sq1w / 2.0f;
	float ObsminY = sq1y - sq1h / 2.0f;
	float ObsmaxY = sq1y + sq1h / 2.0f;
	// Setting up boundary for dummy
	float DummmyminX = sq2x - sq2w / 2.0f;
	float DummymaxX = sq2x + sq2w / 2.0f;
	float DummyminY = sq2y - sq2h / 2.0f;
	float DummymaxY = sq2y + sq2h / 2.0f;
	// If knight currently outside boundary of Obstacle, continue with the rest of the code
	if (DummymaxX <= ObsminX || DummmyminX >= ObsmaxX ||
		DummymaxY <= ObsminY || DummyminY >= ObsmaxY) {
		return 0;
	}
	// Calculate the XY Overlap
	*overlapX = (sq2x < sq1x) ? (DummymaxX - ObsminX) : (ObsmaxX - DummmyminX);
	*overlapY = (sq2y < sq1y) ? (DummymaxY - ObsminY) : (ObsmaxY - DummyminY);
	
	return 1;
}

int IsSquareColliding2(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h) {
	// Setting up boundary for Obstacle
	float ObsminX = sq1x - sq1w / 2.0f;
	float ObsmaxX = sq1x + sq1w / 2.0f;
	float ObsminY = sq1y - sq1h / 2.0f;
	float ObsmaxY = sq1y + sq1h / 2.0f;
	// Setting up boundary for dummy
	float DummmyminX = sq2x - sq2w / 2.0f;
	float DummymaxX = sq2x + sq2w / 2.0f;
	float DummyminY = sq2y - sq2h / 2.0f;
	float DummymaxY = sq2y + sq2h / 2.0f;
	// If knight currently outside boundary of Obstacle, continue with the rest of the code
	if (DummymaxX <= ObsminX || DummmyminX >= ObsmaxX ||
		DummymaxY <= ObsminY || DummyminY >= ObsmaxY) {
		return 0;
	}
	return 1;
}

int euclidean_distance(int x1, int y1, int x2, int y2) {
	float VDP_X = x1 - x2;
	float VDP_Y = y1 - y2;
	// Calculate the Euclidean distance using Pythagoras Theorem
	return (VDP_X * VDP_X) + (VDP_Y * VDP_Y);
}

unsigned int random_range(unsigned int min, unsigned int max)
{
	return min + rand() % (max - min + 1);
}


void SpawnBoost(BoostType* type, float* x, float* y, int* collected, float* timer, float windowW, float windowH)
{
	*type = random_range(0, BOOST_COUNT - 1);
	*x = random_range(100, (unsigned int)(windowW - 100));
	*y = random_range(130, (unsigned int)(windowH - 180));
	*collected = 0;
	*timer = 25.0f;
}

void Anim_Free(Animation* anim)
{
	CP_Image_Free(&anim->img);
}

void Boss_Init(Boss* b)
{
	b->x = 1200.0f;  b->y = 500.0f;
	b->width = 150.0f;  b->height = 120.0f;
	b->draw_w = 180.0f; b->draw_h = 180.0f;
	b->speed = 180.0f;
	b->maxHp = 100.0f;
	b->hp = b->maxHp;
	b->has_hit = 0;
	b->is_right = 1;
	b->state = BOSS_IDLE;
	b->stateTimer = 1.0f;
	b->attackCooldown = 2.0f;
	b->attack2Cooldown = 4.0f;
	Anim_Init(&b->idle, "Assets/Characters/Boss/Boss_Idle.png", 180, 180, 0.12f, 1);
	Anim_Init(&b->run, "Assets/Characters/Boss/Boss_Run.png", 180, 180, 0.12f, 1);
	Anim_Init(&b->attack, "Assets/Characters/Boss/Boss_Attack.png", 180, 180, 0.05f, 0);
	Anim_Init(&b->attack2, "Assets/Characters/Boss/Boss_Attack2.png", 180, 180, 0.05f, 0);
}

void Boss_Update(Boss* b, float dt, float knightX, float knightY)
{
	if (b->state == BOSS_DEAD) return;

	b->is_right = knightX > b->x;

	switch (b->state) {
	case BOSS_IDLE:
		Anim_Update(&b->idle, dt);	
		break;   // later: pick next state
	case BOSS_CHASE:
		Anim_Update(&b->run, dt);
		break;   // later: move toward knight
	case BOSS_ATTACK1:
		if (Anim_Update(&b->attack, dt))
		{
			Anim_Reset(&b->attack);
			b->state = BOSS_IDLE;
			b->attackCooldown = 2.0f;
			b->has_hit = 1;
		}
		
		break;
	case BOSS_ATTACK2:
		b->slash = 1;
		if(Anim_Update(&b->attack, dt))
		{
		Anim_Reset(&b->attack);
		b->state = BOSS_IDLE;
		b->attack2Cooldown = 4.0f;
		}
		break;
		
	default: break;
	}
}

void Boss_Draw(Boss* b)
{
	if (b->state == BOSS_DEAD) return;
	float w = b->is_right ? b->draw_w : -b->draw_w;
	switch (b->state) {
	case BOSS_IDLE:
		Anim_Draw(&b->idle, b->x, b->y, w, b->draw_h);
		break; 
	case BOSS_CHASE:
		Anim_Draw(&b->run, b->x, b->y, w, b->draw_h);
		break; 
	case BOSS_ATTACK1: 
		
		Anim_Draw(&b->attack, b->x, b->y, w, b->draw_h);
		break;
	case BOSS_ATTACK2:
		Anim_Draw(&b->attack2, b->x, b->y, w, b->draw_h);
		
		break;
	default: break;
	}
}

void Boss_TakeDamage(Boss* b, float dmg)
{
	if (b->state == BOSS_DEAD) return;
	b->hp -= dmg;
	if (b->hp <= 0.0f) { b->hp = 0.0f; b->state = BOSS_DEAD; }
}

void Boss_Free(Boss* b)
{
	Anim_Free(&b->attack);
}