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

#define MAX_BULLETS 5

typedef struct {
    float x;
    float y;
    int active;
    float size;
	float speed_x;
	float speed_y;
} Bullet;

typedef struct {
    float x;
    float y;
    float speed;
    float size;
} Player;

Player player;
Bullet bullets[MAX_BULLETS];

void Game_Init(void)
{
	//test
	player.x = 800.0f;
	player.y = 475.0f;
	player.speed = 300.0f;
	player.size = 75.0f;

	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].active = 0;
	}
}

void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));
	float dt = CP_System_GetDt();

	//Player 1 Controls
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


	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();
	float dir_X = 0.0f;
	float dir_Y = 0.0f;

	if (CP_Input_MouseClicked()) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].active == 0) {
				//Calculate the distance between Mouse and Player
				dir_X = mouseX - player.x;
				dir_Y = mouseY - player.y;
				float angle = atan2f(dir_Y, dir_X); // Calculate angle using Tan inverse (Opposite / Adjacent)

				// Spawn bullet at the player's center position
				bullets[i].x = player.x;
				bullets[i].y = player.y;
				bullets[i].speed_x = cosf(angle) * 450.0f;
				bullets[i].speed_y = sinf(angle) * 450.0f;
				bullets[i].size = 15.0f;
				bullets[i].active = 1;
				break; // Exit loop so only one bullet spawns per click
			}
		}
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active == 1) {
			// Apply delta time to BOTH X and Y speeds
			bullets[i].x += bullets[i].speed_x * dt;
			bullets[i].y += bullets[i].speed_y * dt;

			// Updated Clean-Up: Deactivate if it goes off ANY edge of the screen
			// Assumes a standard window size width (e.g. 800) and height (e.g. 600)
			if (bullets[i].x < -50 || bullets[i].x > 1800 ||
				bullets[i].y < -50 || bullets[i].y > 1100) {
				bullets[i].active = 0;
			}
		}
	}

	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255)); // Red projectiles
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active == 1) {
			CP_Graphics_DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size);
		}
	}
	CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
	CP_Graphics_DrawCircle(player.x, player.y, player.size);

}

void Game_Exit(void)
{
}