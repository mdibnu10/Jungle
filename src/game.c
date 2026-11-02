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
	int dead;
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

Player player;
Dummy dummy;
Effects effects;
Obstacle obstacle;
SpeedBoost speedboost;
Bullet bullets[MAX_BULLETS];
Timer timer;
KillCount killcount;
char text[10];
CP_Font font_menu = NULL;
int lives = 3;
int GAMEOVER = 0;
int HPBARHEALTH;
int randomX;
int randomY;
float hitTimer = 0.0f;
int ClickedSlot = 1;
int Paused = 0;
int GameMode = 0; //0 = Time-Based, 1 = Kill-Based

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
	dummy.x = random_range(100, 1500);
	dummy.y = random_range(100, 600	);
	dummy.width = 60;
	dummy.height = 80;
	dummy.counter = 10;
	dummy.dead = 0;
	dummy.chase = 0;

	// Obstacle Values
	obstacle.x = 800;
	obstacle.y = 300;
	obstacle.width = 30;
	obstacle.height = 40;

	//SpeedBoost Values
	speedboost.x = 100;
	speedboost.y = 100;
	speedboost.width = 30;
	speedboost.height = 30;
	speedboost.collected = 0;

	//Effects Values
	effects.noOfEffects = 1;

	//Pause Mode
	Paused = 0;

	// Preset bullet state, inactive
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].state = 0;
	}

	HPBARHEALTH = player.size;
	randomX = random_range(100, CP_System_GetDisplayWidth() - 110);
	randomY = random_range(100, CP_System_GetDisplayHeight() - 110);

	//Timer Values
	timer.y = 50.0f;
	timer.width = 200.0f;
	timer.height = 70.0f;
	timer.timeLeft = 60.0f;
	//KillCount Values
	killcount.y = 50.0f;
	killcount.width = 200.0f;
	killcount.height = 70.0f;
	killcount.total = 10;
	killcount.amount = 0;

	GameMode = random_range(0, 1); // 0 for Time-Based, 1 for Kill-Based
	GAMEOVER = 0; // Reset game over state
	lives = 3; // Reset lives
}

void Game_Update(void)
{

	//EASIER CODING
	double windowLength = CP_System_GetWindowWidth();
	double windowHeight = CP_System_GetWindowHeight();

	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));
	float dt = CP_System_GetDt();
	if (hitTimer > 0.0f && Paused == 0) {
		hitTimer -= dt;
	}
	if (GAMEOVER != 1 && Paused == 0 && timer.timeLeft > 0.0f) {
		timer.timeLeft -= dt;
		if (timer.timeLeft <= 0.0f) {
			timer.timeLeft = 0.0f;
			GAMEOVER = 1; // time's up
		}
	}

	//Player Controls
	if (GAMEOVER != 1 && Paused == 0) {
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

		if (dummy.counter != 0) {
			// Calculate the Euclidean distance using Pythagoras Theorem
			VDP_dist = sqrtf((VDP_X * VDP_X) + (VDP_Y * VDP_Y));
			if (VDP_dist == 0.0f) { VDP_dist = 1.0f; }
			// Normalize the Vector
			NVDP_X = VDP_X / VDP_dist;
			NVDP_Y = VDP_Y / VDP_dist;
		}

		if (VDP_dist <= 300 && dummy.counter != 0) {
			dummy.chase = 1;
		}
		else {
			dummy.chase = 0;
		}
		if (dummy.chase == 1 && GAMEOVER != 1) { //SPEED OF ENEMY CHASE
			dummy.x += (NVDP_X * 100.0f) * dt;
			dummy.y += (NVDP_Y * 100.0f) * dt;
		}
	}

	// Generating Projectie
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].state == 1) {
			CP_Graphics_DrawCircle(bullets[i].x, bullets[i].y, bullets[i].size);
		}
	}
	// Generating Player Circle
	if (hitTimer > 0.0f && ((int)(CP_System_GetSeconds() * 3) % 2 == 0)) {
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255)); // red when hit
	}
	else {
		CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255)); // normal blue
	}
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

	/*
	====================================================================================================================================
	TESTING UI
	====================================================================================================================================
	*/

	if (dummy.counter == 0 && dummy.dead == 0)
	{
		dummy.dead = 1;
		killcount.amount++;
	}

	//If Pause is clicked, pause the game first and stop everything.
	if (IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
	{
		if (Paused == 0)
		{
			Paused = 1;
		}
		else
		{
			Paused = 0;
		}
	}
	
	//If Player and Dummt collide, lose a life and reset player position.
	if (IsSquareCircleColliding(dummy.x, dummy.y, dummy.width, dummy.height, player.x, player.y, player.size / 2.0f) && dummy.counter != 0 && Paused == 0)
	{
		// Handle collision
		HPBARHEALTH -= 0.10f;
		hitTimer = 0.15f;

		if (HPBARHEALTH < 0) {
			HPBARHEALTH = 0;
		}

	}

	//If Player and SpeedBoost collide, gain a speed boost for 5 seconds.
	if (speedboost.collected != 1) {
		if (IsSquareCircleColliding(randomX, randomY, speedboost.width, speedboost.height, player.x, player.y, player.size / 2.0f))
		{
			player.speed += 10.0f;
			speedboost.collected = 1;
			effects.noOfEffects += 1;
		}
	}

	//Draw a smaller HP bar below the player to show how much health the player has.
	//Disappears when SpeedBoost is collected.
	CP_Settings_Fill(CP_Color_Create(0, 70, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(player.x - (player.size / 2.0f), player.y + (player.size / 2.0f + 10.0f), HPBARHEALTH, 10.0f);


	//Draw Speed-Boost Power Up Bar
	if (speedboost.collected == 0) {
		CP_Settings_Fill(CP_Color_Create(0, 255, 255, 255));
		CP_Graphics_DrawRect(randomX, randomY, 10.0f, 10.0f);
	}

	//Draw the Effect Status Box
	if ( effects.noOfEffects > 0) {
		for (int i = 0; i < effects.noOfEffects; i++) {
			CP_Settings_Fill(CP_Color_Create(255, 0, 255, 255));
			CP_Graphics_DrawRect(windowLength - 100, windowHeight - 100 - (i * 60), 50.0f, 50.0f);
		}
	}

	//Draw the Heart as a a circle for now.
	CP_Settings_Fill(CP_Color_Create(255, 70, 0, 255));
	if (HPBARHEALTH != 0)
	{
		for (int i = lives ; i != 0 ; i--)
		{
			CP_Graphics_DrawCircle(15 + (i * 80), windowHeight - 60, 70);
		}
	}
	else
	{
		if (lives == 1)
		{
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Font_DrawText("GAME OVER", windowLength / 2.0f - 200.0f, windowHeight / 2.0f);
			GAMEOVER = 1;
		}
		else
		{
			lives--;
			player.x = windowLength / 2.0f;
			player.y = windowHeight / 2.0f;
			HPBARHEALTH = player.size;
		}
	}

	

	//Draw the pause button in the top right corner of the screen.
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(50, 50 , 50, 50);

	//Inventory Slot Selection
	if (CP_Input_KeyTriggered(KEY_1)) { ClickedSlot = 1; }
	if (CP_Input_KeyTriggered(KEY_2)) { ClickedSlot = 2; }
	if (CP_Input_KeyTriggered(KEY_3)) { ClickedSlot = 3; }

	//Draw the Inventory in white (3 boxes)
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);

	for (int i = 1; i <= 3; i++) {
		if (i == ClickedSlot) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));   
		}
		else {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255)); 
		}
		CP_Graphics_DrawRect(windowLength / 2.0f + ((i - 2) * 60), windowHeight -  45 , 50, 50);
	}

	if (GameMode == 0) {
		//Timer Progression Code
		int totalSeconds = (int)(timer.timeLeft + 0.999f);
		static int lastSeconds = -1;

		snprintf(timer.text, sizeof(timer.text), "%02d:%02d", totalSeconds / 60, totalSeconds % 60);

		if (totalSeconds != lastSeconds) {
			printf("%s\n", timer.text);
			lastSeconds = totalSeconds;
		}

		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255)); // white box
		CP_Graphics_DrawRect(windowLength / 2.0f, timer.y, timer.width, timer.height);

		if (timer.timeLeft <= 10.0f) {
			CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255)); // red in last 10 seconds
		}
		else {
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));   // black text
		}
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(timer.text, windowLength / 2.0f, timer.y);
	}
	else if (GameMode == 1) {
		// Kill-Based Game Mode Code

		snprintf(killcount.text, sizeof(killcount.text), "%02d / %02d", killcount.amount, killcount.total);

		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255)); // white box
		CP_Graphics_DrawRect(windowLength / 2.0f, killcount.y, killcount.width, killcount.height);

		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(killcount.text, windowLength / 2.0f, killcount.y);
	}

	if (Paused == 1 && GAMEOVER == 0)
	{
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(100.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", windowLength / 2.0f, windowHeight / 2.0f);

		//If menu button is clicked, go back to main menu.
		if (IsAreaClicked(windowLength / 2 + 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			move = 1;
		}

		//If resume button is clicked, close pause mode and resume.
		if (IsAreaClicked(windowLength / 2 - 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			Paused = 0;
		}

		//Menu Button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 +300, windowHeight /2 + 200 , 200, 100);

		//Resume Button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 -300, windowHeight / 2 + 200, 200, 100);
	}


	/*
	====================================================================================================================================

	====================================================================================================================================
	*/

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
