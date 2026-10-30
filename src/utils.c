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

float findQ(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}