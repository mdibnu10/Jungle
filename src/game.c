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


Player knight;
Player archer;
CP_Image bg;
CP_Image arrow;
Enemy dummy;
Obstacle obstacle[3];
float Obs_X[MAX_OBSTACLE] = { 400, 200, 1200 };
float Obs_Y[MAX_OBSTACLE] = { 200, 700, 600 };
float Obs_W[MAX_OBSTACLE] = { 30, 60, 90 };
float Obs_H[MAX_OBSTACLE] = { 40, 80, 160 };
float Vec_X;
float Vec_Y;
float dist;
float Norm_X;
float Norm_Y;
float VDP_X;
float VDP_Y;
float VDP_dist;
float NVDP_X;
float NVDP_Y;

Bullet bullets[MAX_BULLETS];
char text[100];
CP_Font font_menu = NULL;


void Game_Init(void)
{

    knight.x = 800.0f;
    knight.y = 450.0f;
    knight.speed = 3.0f;
    knight.state = IDLE;
	knight.size = 50.0f;

	dummy.x = 500;
	dummy.y = 300;
	dummy.width = 60;
	dummy.height = 80;
	dummy.counter = 10;
	dummy.chase = 0;

	VDP_X = 0.0f;
	VDP_Y = 0.0f;
	VDP_dist = 0.0f;
	NVDP_X = 0.0f;
	NVDP_Y = 0.0f;
	Vec_X = 0.0f;
	Vec_Y = 0.0f;
	dist = 0.0f;
	Norm_X = 0.0f;
	Norm_Y = 0.0f;

    archer.x = 1000.0f;
    archer.y = 450.0f;
    archer.speed = 3.0f;
    archer.state = IDLE;


    bg = CP_Image_Load(maps[current_level][random[current_level]].image_path);
	arrow = CP_Image_Load("Assets/Arrow.png");
    Anim_Init(&knight.idle, "Assets/Knight_Idle.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.run, "Assets/Knight_Run.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.attack, "Assets/Knight_Attack.png", 120, 80, 0.10f, 0);
    Anim_Init(&archer.idle, "Assets/Archer_Idle.png", 64, 64, 0.12f, 1);
    Anim_Init(&archer.run, "Assets/Archer_Run.png", 64, 64, 0.12f, 1);
    Anim_Init(&archer.attack, "Assets/Archer_Attack.png", 64, 64, 0.10f, 0);
	Anim_Init(&dummy.idle, "Assets/RP_Idle.png", 32, 32, 0.12f, 1);
	Anim_Init(&dummy.run, "Assets/RP_Run.png", 32, 32, 0.12f, 1);



	font_menu = CP_Font_Load("./Assets/Exo2-Regular.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_EllipseMode(CP_POSITION_CENTER);




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

    CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(bg, 0, 0, 1600, 900, 255);
    float dt = CP_System_GetDt();

    int moving = 0;
    
    if (CP_Input_KeyDown(KEY_W)) 
    { 
        knight.y -= knight.speed;
        archer.y -= archer.speed;
        moving = 1;
    }
    if (CP_Input_KeyDown(KEY_S))
    { 
        knight.y += knight.speed;
        archer.y += archer.speed;
        moving = 1; 
    }
    if (CP_Input_KeyDown(KEY_A)) 
    { 
        knight.x -= knight.speed;
        archer.x -= archer.speed;
        knight.is_right = 0;
        moving = 1;
    }
    if 
        (CP_Input_KeyDown(KEY_D))
    { 
        knight.x += knight.speed;
        archer.x += archer.speed;
        knight.is_right = 1;
        moving = 1;
    }

    // decide the state (can't change state mid-attack)
    if (knight.state != ATTACK)
    {
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
        {
            knight.state = ATTACK;
            Anim_Reset(&knight.attack);
       
        }
        else
        {
        knight.state = moving ? RUN : IDLE;
        }
    }
    if (archer.state != ATTACK)
    {
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
        {
            archer.state = ATTACK;      
            Anim_Reset(&archer.attack);
        }
        else
        {
            archer.state = moving ? RUN : IDLE;
        }
    }

	 // reverse the image if player is moving left
    float draw_width = knight.is_right == 1 ? 160 : -160;
    
   // draw image based on the player state
    switch (knight.state)
    {
    case IDLE:
        Anim_Update(&knight.idle, dt);

        Anim_Draw(&knight.idle, knight.x, knight.y, draw_width, 120);
    
        break;
    case RUN:
        Anim_Update(&knight.run, dt);

        Anim_Draw(&knight.run, knight.x, knight.y, draw_width, 120);
    
        break;
    case ATTACK:
        if (Anim_Update(&knight.attack, dt))
        {
            knight.state = IDLE;

        }
        Anim_Draw(&knight.attack, knight.x, knight.y, draw_width, 100);
        break;
    }

	draw_width = knight.is_right == 1 ? 110 : -110;

    switch (archer.state)
    {
    case IDLE:
  
        Anim_Update(&archer.idle, dt);
    
        Anim_Draw(&archer.idle, archer.x, archer.y, draw_width, 90);
        break;
    case RUN:
      
        Anim_Update(&archer.run, dt);

        Anim_Draw(&archer.run, archer.x, archer.y, draw_width, 90);
        break;
    case ATTACK:
        if (Anim_Update(&archer.attack, dt))
        {

            archer.state = IDLE;
        }
        Anim_Draw(&archer.attack, archer.x, archer.y, draw_width, 100);
        break;
    }



	//
	/*------------------------------------------------------------------------------*/
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();


	if (CP_Input_MouseClicked()) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].state == 0) {
				// Find the Vector between Player and Mouse
				Vec_X = mouseX - archer.x;
				Vec_Y = mouseY - archer.y;
				// Calculate the Euclidean distance using Pythagoras Theorem
				dist = sqrtf((Vec_X * Vec_X) + (Vec_Y * Vec_Y));
				// Normalize the Vector
				Norm_X = Vec_X / dist;
				Norm_Y = Vec_Y / dist;

				// Bullet comes from knight xy coords
				bullets[i].x = archer.x;
				bullets[i].y = archer.y;
				// Bullet speed
				bullets[i].speed_x = Norm_X * 450.0f;
				bullets[i].speed_y = Norm_Y * 450.0f;
				// Calculate and store the value of the direction the bullet is facing
				bullets[i].angle = CP_Math_Degrees(atan2f(Vec_Y, Vec_X));
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

	// get coordinates of top, bottom, left, right
	float half_size = knight.size / 2.0f;

	if (knight.x - half_size < 0) {
		knight.x = half_size;
	}
	if (knight.x + half_size > 1600.0f) {
		knight.x = 1600.0f - half_size;
	}
	if (knight.y - half_size < 0) {
		knight.y = half_size;
	}
	if (knight.y + half_size > 900.0f) {
		knight.y = 900.0f - half_size;
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
		float Qx = findQ(knight.x, minX, maxX);
		float Qy = findQ(knight.y, minY, maxY);
		//Calculating the squared distance of Q - Player center
		float VQC_X = knight.x - Qx;
		float VQC_Y = knight.y - Qy;
		float distSq = (VQC_X * VQC_X) + (VQC_Y * VQC_Y);
		//Determined colision occurs whehter radius squared is more than distance squared
		float rSq = half_size * half_size;
		if (distSq < rSq) {
			float distance = sqrtf(distSq);
			if (distance == 0.0f) distance = 0.001f;
			float nV_X = VQC_X / distance;
			float nV_Y = VQC_Y / distance;
			//Calculate the distance the player is clipping through
			float collision = half_size - distance;
			//Applies the positional update on circle position outside of rectangle
			knight.x += nV_X * collision;
			knight.y += nV_Y * collision;
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
	
	VDP_X = knight.x - dummy.x;
	VDP_Y = knight.y - dummy.y;
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

	
	if (dummy.counter != 0) {
	
		dummy.state = dummy.chase == 1 ? RUN : IDLE;
		// check if player is on which side of the enemy
		dummy.is_right = knight.x > dummy.x ? 1 : 0;
		// turns right if enemy is on the right
		draw_width = dummy.is_right == 1 ? 80 : -80;

		switch (dummy.state)
		{
		case IDLE:
			Anim_Update(&dummy.idle, dt);
			Anim_Draw(&dummy.idle, dummy.x, dummy.y, draw_width, 80);
			break;
		case RUN:
			Anim_Update(&dummy.run, dt);
			Anim_Draw(&dummy.run, dummy.x, dummy.y, draw_width, 80);
		}
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
			
			CP_Image_DrawSubImageAdvanced(arrow, bullets[i].x, bullets[i].y, 32, 32, 32, 0, 48, 16, 255, bullets[i].angle);
			//CP_Graphics_DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size);
		}
	}

	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		
	}

}

void Game_Exit(void)
{
	CP_Font_Free(&font_menu);

}