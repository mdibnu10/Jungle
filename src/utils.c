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