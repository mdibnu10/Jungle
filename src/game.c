//---------------------------------------------------------
// file:	game.c
// author:	
// email:	
//
// brief:	
//		
//
// Copyright © 2026 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "mainmenu.h"
#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <math.h>

typedef struct {
	float x;
	float y;
	float speed;
	float size;
} Player;

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int counter;
	int chase;
} Dummy;
int move = 1;

typedef struct {
    float x;
    float y;
    int state; // 0 inactive, 1 active
    float size;
	float speed_x;
	float speed_y;
} Bullet;
#define MAX_BULLETS 5

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Obstacle;
#define MAX_OBSTACLE 3

Player player;
Dummy dummy;
Obstacle obstacle[3];
Bullet bullets[MAX_BULLETS];
char text[100];
CP_Font font_menu = NULL;

float Obs_X[MAX_OBSTACLE] = {400, 200, 1200};
float Obs_Y[MAX_OBSTACLE] = {200, 700, 600};
float Obs_W[MAX_OBSTACLE] = {30, 60, 90};
float Obs_H[MAX_OBSTACLE] = {40, 80, 160};

void Game_Init(void)
{
	font_menu = CP_Font_Load("./Assets/Exo2-Regular.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_EllipseMode(CP_POSITION_CENTER);

	//Player spawn location & values
	player.x = 800.0f;
	player.y = 700.0f;
	player.speed = 300.0f;
	player.size = 75.0f;

	// Dummy Values
	dummy.x = 500;
	dummy.y = 300;
	dummy.width = 60;
	dummy.height = 80;
	dummy.counter = 10;
	dummy.chase = 0;

	// Obstacle Values
	for (int i = 0; i < MAX_OBSTACLE; i++) {
		obstacle[i].x = Obs_X[i];
		obstacle[i].y = Obs_Y[i];
		obstacle[i].width = Obs_W[i];
		obstacle[i].height = Obs_H[i];
	}

	// Preset bullet state, inactive
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].state = 0;
	}
}

void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));
	float dt = CP_System_GetDt();

	//Player Controls
	if (CP_Input_KeyDown(KEY_W)) {
		player.y += -player.speed * dt; // Move Up
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		player.y += player.speed * dt; // Move down
	}
	if (CP_Input_KeyDown(KEY_A)) {
		player.x += -player.speed * dt; // Move left
	}
	else if (CP_Input_KeyDown(KEY_D)) {
		player.x += player.speed * dt; // Move right
	}
	//
	/*------------------------------------------------------------------------------*/
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();
	float Vec_X = 0.0f;
	float Vec_Y = 0.0f;
	float dist = 0.0f;
	float Norm_X = 0.0f;
	float Norm_Y = 0.0f;

	if (CP_Input_MouseClicked()) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].state == 0) {
				// Find the Vector between Player and Mouse
				Vec_X = mouseX - player.x;
				Vec_Y = mouseY - player.y;
				// Calculate the Euclidean distance using Pythagoras Theorem
				dist = sqrtf((Vec_X * Vec_X) + (Vec_Y * Vec_Y));
				// Normalize the Vector
				Norm_X = Vec_X / dist;
				Norm_Y = Vec_Y / dist;

				// Bullet comes from player xy coords
				bullets[i].x = player.x;
				bullets[i].y = player.y;
				// Bullet speed
				bullets[i].speed_x = Norm_X * 450.0f;
				bullets[i].speed_y = Norm_Y * 450.0f;
				bullets[i].size = 15.0f; // Size
				bullets[i].state = 1; // Active State
				break;
			}
		}
	}
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].state == 1) {
			bullets[i].x += bullets[i].speed_x * dt;
			bullets[i].y += bullets[i].speed_y * dt;
			// Window Size Reference (1600, 900);
			// If bullet exit window, reset bullet to inactive state to be reuse.
			if (bullets[i].x < -50 || bullets[i].x > 1650 ||
				bullets[i].y < -50 || bullets[i].y > 950) {
				bullets[i].state = 0;
			}
			if (dummy.counter != 0) {
				if (bullets[i].x <= dummy.x + dummy.width / 2.0f &&
					bullets[i].x >= dummy.x - dummy.width / 2.0f &&
					bullets[i].y <= dummy.y + dummy.height / 2.0f &&
					bullets[i].y >= dummy.y - dummy.height / 2.0f) {
					dummy.counter -= 1;
					bullets[i].state = 0;
				}
			}
			for (int y = 0; y < MAX_OBSTACLE; y++) {
				if (bullets[i].x <= obstacle[y].x + obstacle[y].width / 2.0f &&
					bullets[i].x >= obstacle[y].x - obstacle[y].width / 2.0f &&
					bullets[i].y <= obstacle[y].y + obstacle[y].height / 2.0f &&
					bullets[i].y >= obstacle[y].y - obstacle[y].height / 2.0f) {
					bullets[i].state = 0;
				}
			}
		}
	}
	//
	/*------------------------------------------------------------------------------*/
	float half_size = player.size / 2.0f;

	if (player.x - half_size < 0) {
		player.x = half_size;
	}
	if (player.x + half_size > 1600.0f) {
		player.x = 1600.0f - half_size;
	}
	if (player.y - half_size < 0) {
		player.y = half_size;
	}
	if (player.y + half_size > 900.0f) {
		player.y = 900.0f - half_size;
	}
	//AABB INTERSECTION !!!!
	// For Player
	for (int i = 0; i < MAX_OBSTACLE; i++) {
		//Calculate the min and max for x and y coordinates
		float minX = obstacle[i].x - obstacle[i].width / 2.0f;
		float maxX = obstacle[i].x + obstacle[i].width / 2.0f;
		float minY = obstacle[i].y - obstacle[i].height / 2.0f;
		float maxY = obstacle[i].y + obstacle[i].height / 2.0f;
		//Determine point Q in AABB where that is closest to the sphere
		float Qx = findQ(player.x, minX, maxX);
		float Qy = findQ(player.y, minY, maxY);
		//Calculating the squared distance of Q - Player center
		float VQC_X = player.x - Qx;
		float VQC_Y = player.y - Qy;
		float distSq = (VQC_X * VQC_X) + (VQC_Y * VQC_Y);
		//Determined colision occurs whehter radius squared is more than distance squared
		float rSq = half_size * half_size;
		if (distSq < rSq) {
			float distance = sqrtf(distSq);
			float nV_X = VQC_X / distance;
			float nV_Y = VQC_Y / distance;
			//Calculate the distance the player is clipping through
			float collision = half_size - distance;
			//Applies the positional update on circle position outside of rectangle
			player.x += nV_X * collision;
			player.y += nV_Y * collision;
		}
	}
	//For dummy
	//for (int i = 0; i < MAX_OBSTACLE; i++) {
	//	//Calculate the min and max for x and y coordinates
	//	float minX = obstacle[i].x - obstacle[i].width / 2.0f;
	//	float maxX = obstacle[i].x + obstacle[i].width / 2.0f;
	//	float minY = obstacle[i].y - obstacle[i].height / 2.0f;
	//	float maxY = obstacle[i].y + obstacle[i].height / 2.0f;
	//	//Determine point Q in AABB where that is closest to the sphere
	//	float Qx = findQ(dummy.x, minX, maxX);
	//	float Qy = findQ(dummy.y, minY, maxY);
	//	//Calculating the squared distance of Q - Player center
	//	float VQC_X = dummy.x - Qx;
	//	float VQC_Y = dummy.y - Qy;
	//	float distSq = (VQC_X * VQC_X) + (VQC_Y * VQC_Y);
	//	//Determined colision occurs whehter radius squared is more than distance squared
	//	float rSq = half_size * half_size;
	//	if (distSq < rSq) {
	//		float distance = sqrtf(distSq);
	//		if (distance == 0.0f) distance = 0.001f; //Safety net
	//		float nV_X = VQC_X / distance;
	//		float nV_Y = VQC_Y / distance;
	//		//Calculate the distance the player is clipping through
	//		float collision = half_size - distance;
	//		//Applies the positional update on circle position outside of rectangle
	//		dummy.x += nV_X * collision;
	//		dummy.y += nV_Y * collision;
	//	}
	//}
	/*------------------------------------------------------------------------------*/
	float VDP_X = 0.0f;
	float VDP_Y = 0.0f;
	float VDP_dist = 0.0f;
	float NVDP_X = 0.0f;
	float NVDP_Y = 0.0f;
	VDP_X = player.x - dummy.x;
	VDP_Y = player.y - dummy.y;
	// Calculate the Euclidean distance using Pythagoras Theorem
	VDP_dist = sqrtf((VDP_X * VDP_X) + (VDP_Y * VDP_Y));
	if (VDP_dist == 0.0f) { VDP_dist = 1.0f; }
	// Normalize the Vector
	NVDP_X = VDP_X / VDP_dist;
	NVDP_Y = VDP_Y / VDP_dist;

	if (VDP_dist <= 300) {
		dummy.chase = 1;
	}
	else {
		dummy.chase = 0;
	}
	if (dummy.chase == 1) {
		dummy.x += (NVDP_X * 200.0f) * dt;
		dummy.y += (NVDP_Y * 200.0f) * dt;
	}


	// Generating Player Circle
	CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
	CP_Graphics_DrawCircle(player.x, player.y, player.size);

	// Dummy
	if (dummy.counter != 0) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Graphics_DrawRect(dummy.x, dummy.y, dummy.width, dummy.height);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		snprintf(text, sizeof(text), "%d", dummy.counter);
		CP_Font_DrawText(text, dummy.x, dummy.y - (dummy.height / 2.0f) - 12.0f);
		CP_Settings_TextSize(30.0f);
	}
	for (int i = 0; i < MAX_OBSTACLE; i++) {
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawRect(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height);
	}

	// Generating Projectie
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].state == 1) {
			CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			CP_Graphics_DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size);
		}
	}

	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		move = 1;
	}

}

void Game_Exit(void)
{
	CP_Font_Free(&font_menu);
}