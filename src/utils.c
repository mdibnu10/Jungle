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