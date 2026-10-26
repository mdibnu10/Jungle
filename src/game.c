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

Player player;
Dummy dummy;
Obstacle obstacle;
Bullet bullets[MAX_BULLETS];
char text[10];
CP_Font font_menu = NULL;


void Game_Init(void)
{
	font_menu = CP_Font_Load("./Assets/Exo2-Regular.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);

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
	obstacle.x = 800;
	obstacle.y = 300;
	obstacle.width = 30;
	obstacle.height = 40;

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
			if (bullets[i].x <= obstacle.x + obstacle.width / 2.0f &&
				bullets[i].x >= obstacle.x - obstacle.width / 2.0f &&
				bullets[i].y <= obstacle.y + obstacle.height / 2.0f &&
				bullets[i].y >= obstacle.y - obstacle.height / 2.0f) {
				bullets[i].state = 0;
			}

		}
	}
	//
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

	// Generating Projectie
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].state == 1) {
			CP_Graphics_DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size);
		}
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

	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawRect(obstacle.x, obstacle.y, obstacle.width, obstacle.height);

	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		move = 1;
	}

}

void Game_Exit(void)
{
	CP_Font_Free(&font_menu);
}

//Temp

	//switch (move) {
	//case 0: break;
	//case 1: dummy.x += 300.0f * dt; break;
	//case 2: dummy.x -= 300.0f * dt; break;
	//case 3: dummy.y += 300.0f * dt; break;
	//case 4: dummy.y -= 300.0f * dt; break;
	//}
	////// Obstacle Values
	////obstacle.x = 800;
	////obstacle.y = 300;
	////obstacle.width = 30;
	////obstacle.height = 40;

	//if (dummy.x >= 1200) {
	//	move = 2;
	//	dummy.active = 0;
	//}
	//else if (dummy.x <= 400) {
	//	move = 1;
	//	dummy.active = 0;
	//}	

	////if (dummy.active == 0) {
	////	if (dummy.x + dummy.width / 2.0f >= obstacle.x - obstacle.width / 2.0f) {
	////		move = 4;
	////		dummy.active = 1;
	////	}
	////}
	////if (dummy.active == 1){
	////	if (dummy.y + dummy.height / 2.0f <= obstacle.y - obstacle.height / 2.0f) {
	////		move = 1;
	////		dummy.active = 2;
	////	}
	////}
	////if (dummy.active == 2) {
	////	if (dummy.x - dummy.width / 2.0f >= obstacle.x + obstacle.width / 2.0f) {
	////		move = 3;
	////		dummy.active = 3;
	////	}
	////}
	////if (dummy.active == 3) {
	////	if (dummy.y >= obstacle.y) {
	////		move = 1;
	////		dummy.active = 4;
	////	}
	////}
