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
CP_Image bg, arrow, bg2;
Enemy dummy;
float Vec_X, Vec_Y, dist, Norm_X, Norm_Y, VDP_X, VDP_Y, VDP_dist, NVDP_X, NVDP_Y;

Bullet bullets[MAX_BULLETS];
char text[100];
CP_Font font_menu = NULL;

Obstacle obstacle[12];
float Obs_X[MAX_OBSTACLE] = { 1216.13f, 1072.00f, 351.88f, 688.33f, 1488, 672.06f, 272.13f, 352.00f, 895.82f, 687.95f, 1184.00f, 1215.75f};
float Obs_Y[MAX_OBSTACLE] = { 144.00f, 248.19f, 464.00f, 208, 232.25f, 464.06f, 784.13f, 231.91f, 408.36f, 600.00f, 600.14f, 784.00f};
float Obs_W[MAX_OBSTACLE] = { 64.25f, 32.75, 64.25f, 64.67f, 32.5f, 64.375f, 64.75f, 32.36f, 32, 32.09f, 32.36f, 64.5f};
float Obs_H[MAX_OBSTACLE] = { 32.5f, 16.125f, 32, 32.67f, 16.5f, 32.125f, 32.25f, 15.82f, 16, 16, 16.09f, 32.5f};

//---------------------------------------------------------
//Darrel
//---------------------------------------------------------
Effects effects;
SpeedBoost speedboost;
Timer timer;
KillCount killcount;

int lives = 3;
int GAMEOVER = 0;
int HPBARHEALTH;
int randomX;
int randomY;
float hitTimer = 0.0f;
int ClickedSlot = 1;
int Paused = 0;
int GameMode = 0; //0 = Time-Based, 1 = Kill-Based
//---------------------------------------------------------
float ObsminX, ObsmaxX, ObsminY, ObsmaxY;
float KnightminX, KnightmaxX, KnightminY, KnightmaxY;
float DummmyminX, DummymaxX, DummyminY, DummymaxY;
float overlapX, overlapY;

void Game_Init(void)
{
	knight.x = 800.0f;
	knight.y = 450.0f;
	knight.speed = 200.0f;
	knight.state = IDLE;
	knight.width = 50;
	knight.height = 60;

	dummy.x = random_range(100, 1500);
	dummy.y = random_range(100, 600);
	dummy.width = 60;
	dummy.height = 40;
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

	bg = CP_Image_Load(maps[current_level][random[current_level]].image_path);
	if ((maps[current_level][random[current_level]].obstacle_path) != NULL)
	{
		bg2 = CP_Image_Load((maps[current_level][random[current_level]].obstacle_path));
	}
	else
	{
	
	bg2 = NULL;
	}
	int rac = random[current_level];
	arrow = CP_Image_Load("Assets/Arrow.png");
    Anim_Init(&knight.idle, "Assets/Knight_Idle.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.run, "Assets/Knight_Run.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.attack, "Assets/Knight_Attack.png", 120, 80, 0.10f, 0);
	Anim_Init(&dummy.idle, "Assets/Bear_Idle.png", 32, 32, 0.12f, 1);
	Anim_Init(&dummy.run, "Assets/Bear_Run.png", 32, 32, 0.12f, 1);
	Anim_Init(&dummy.attack, "Assets/Bear_Attack.png", 32, 32, 0.10f, 0);

	font_menu = CP_Font_Load("./Assets/Exo2-Regular.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_EllipseMode(CP_POSITION_CENTER);

	// Obstacle Values based on one map only for testing
	if (bg2 != NULL)
	{
		for (int i = 0; i < MAX_OBSTACLE; i++) {
			obstacle[i].x = Obs_X[i];
			obstacle[i].y = Obs_Y[i];
			obstacle[i].width = Obs_W[i];
			obstacle[i].height = Obs_H[i];
		}
	}

	// Preset bullet state, inactive
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].state = 0;
	}

	//---------------------------------------------------------
	//Darrel
	//---------------------------------------------------------
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

	HPBARHEALTH = knight.width;
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
	float windowLength = CP_System_GetWindowWidth();
	float windowHeight = CP_System_GetWindowHeight();
    CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
    CP_Settings_ImageMode(CP_POSITION_CORNER);
    CP_Image_Draw(bg, 0, 0, windowLength, windowHeight, 255);
    float dt = CP_System_GetDt();
	float half_width = knight.width / 2.0f;
	float half_height = knight.height / 2.0f;
    int moving = 0;
	//---------------------------------------------------------
	//Darrel
	//---------------------------------------------------------
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
	//---------------------------------------------------------
    // Player's Control
	if (GAMEOVER != 1 && Paused == 0) {
		if (CP_Input_KeyDown(KEY_W))
		{
			knight.y -= knight.speed * dt;
			moving = 1;
		}
		if (CP_Input_KeyDown(KEY_S))
		{
			knight.y += knight.speed * dt;
			moving = 1;
		}
		if (CP_Input_KeyDown(KEY_A))
		{
			knight.x -= knight.speed * dt;
			knight.is_right = 0;
			moving = 1;
		}
		if
			(CP_Input_KeyDown(KEY_D))
		{
			knight.x += knight.speed * dt;
			knight.is_right = 1;
			moving = 1;
		}
	}
	//---------------------------------------------------------------------
	//For testing purposes
	//CP_Graphics_DrawRect(knight.x, knight.y, knight.width, knight.height);
	//CP_Graphics_DrawRect(dummy.x, dummy.y, dummy.width, dummy.height);
	//---------------------------------------------------------------------
    // decide the state (can't change state mid-attack)
    if (knight.state != ATTACK)
    {
        if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused != 1)
        {
			knight.state = ATTACK;
			Anim_Reset(&knight.attack);
        }
        else
        {
        knight.state = moving ? RUN : IDLE;
        }
    }
	// reverse the image if player is moving left
    float draw_width = knight.is_right == 1 ? 160 : -160;
    
    // draw image based on the player state
    switch (knight.state)
    {
    case IDLE:
        Anim_Update(&knight.idle, dt);
        Anim_Draw(&knight.idle, knight.x, knight.y - knight.height / 2.0f, draw_width, 120);
        break;
    case RUN:
        Anim_Update(&knight.run, dt);
        Anim_Draw(&knight.run, knight.x, knight.y - knight.height / 2.0f, draw_width, 120);
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
	/*------------------------------------------------------------------------------*/
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	if (CP_Input_MouseClicked() && Paused == 0 && !IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (bullets[i].state == 0) {
				// Find the Vector between Player and Mouse
				Vec_X = mouseX - knight.x;
				Vec_Y = mouseY - knight.y;
				// Calculate the Euclidean distance using Pythagoras Theorem
				dist = sqrtf((Vec_X * Vec_X) + (Vec_Y * Vec_Y));
				// Normalize the Vector
				Norm_X = Vec_X / dist;
				Norm_Y = Vec_Y / dist;
				// Bullet comes from knight xy coords
				bullets[i].x = knight.x;
				bullets[i].y = knight.y;
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
		if (bullets[i].state == 1 && Paused != 1) {
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

	if (knight.x - half_width < 0) {
		knight.x = half_width;
	}
	if (knight.x + half_width > windowLength) {
		knight.x = windowLength - half_width;
	}
	if (knight.y - half_height < 0) {
		knight.y = half_height;
	}
	if (knight.y + half_height > windowHeight) {
		knight.y = windowHeight - half_height;
	}
	//AABB INTERSECTION !!!!
	// For Knight
	for (int i = 0; i < MAX_OBSTACLE; i++) {
		if (IsSquareColliding(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height, knight.x, knight.y, knight.width, knight.height, &overlapX, &overlapY)) {
			if (overlapX < overlapY) {
				knight.x = (knight.x < obstacle[i].x) ? (knight.x -= overlapX) : (knight.x += overlapX);
			}
			else {
				knight.y = (knight.y < obstacle[i].y) ? (knight.y -= overlapY) : (knight.y += overlapY);
			}
		}
	}
	/*------------------------------------------------------------------------------*/
	//For dummy
	for (int i = 0; i < MAX_OBSTACLE; i++) {
		if (IsSquareColliding(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height, dummy.x, dummy.y, dummy.width, dummy.height, &overlapX, &overlapY)) {
			if (overlapX < overlapY) {
				dummy.x = (dummy.x < obstacle[i].x) ? (dummy.x -= overlapX) : (dummy.x += overlapX);
			}
			else {
				dummy.y = (dummy.y < obstacle[i].y) ? (dummy.y -= overlapY) : (dummy.y += overlapY);
			}
		}
	}
	/*------------------------------------------------------------------------------*/
	if (dummy.counter != 0) {
		VDP_X = knight.x - dummy.x;
		VDP_Y = knight.y - dummy.y;
		// Calculate the Euclidean distance using Pythagoras Theorem
		VDP_dist = sqrtf((VDP_X * VDP_X) + (VDP_Y * VDP_Y));
		if (VDP_dist == 0.0f) { VDP_dist = 1.0f; }
		// Normalize the Vector
		NVDP_X = VDP_X / VDP_dist;
		NVDP_Y = VDP_Y / VDP_dist;
	}

	int touching = IsSquareColliding2(dummy.x, dummy.y, dummy.width, dummy.height,
		knight.x, knight.y, knight.width, knight.height);
	if (VDP_dist <= 300 && dummy.counter != 0 && Paused != 1) {
		dummy.chase = 1;
	}
	else {
		dummy.chase = 0;
	}
	if (dummy.chase == 1 && !touching && dummy.state != ATTACK && GAMEOVER != 1) {
		dummy.x += (NVDP_X * 200.0f) * dt;
		dummy.y += (NVDP_Y * 200.0f) * dt;
	}
	if (dummy.counter != 0) 
	{
		if (dummy.state != ATTACK)
		{
			dummy.state = (dummy.chase == 1 && !touching) ? RUN : IDLE;
		}
		dummy.is_right = knight.x > dummy.x ? 1 : 0;
		draw_width = dummy.is_right == 1 ? 80 : -80;
	
		//if (dummy.state != ATTACK) {
		//	dummy.state = dummy.chase == 1 ? RUN : IDLE;
		//}
		//// check if player is on which side of the enemy
		//dummy.is_right = knight.x > dummy.x ? 1 : 0;
		//// turns right if enemy is on the right
		//draw_width = dummy.is_right == 1 ? 80 : -80;

		switch (dummy.state)
		{
		case IDLE:
			Anim_Update(&dummy.idle, dt);
			Anim_Draw(&dummy.idle, dummy.x, dummy.y, draw_width, 80);
			break;
		case RUN:
			Anim_Update(&dummy.run, dt);
			Anim_Draw(&dummy.run, dummy.x, dummy.y, draw_width, 80);
			break;
		case ATTACK:
			if (Anim_Update(&dummy.attack, dt))
			{
				dummy.state = IDLE;
			}
			Anim_Draw(&dummy.attack, dummy.x, dummy.y, draw_width, 80);
			break;
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
	//---------------------------------------------------------
	//Darrel
	//---------------------------------------------------------
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
	//if (IsSquareCircleColliding(dummy.x, dummy.y, dummy.width, dummy.height, knight.x, knight.y, knight.size / 2.0f) && dummy.counter != 0 && Paused == 0)
	if (IsSquareColliding2(dummy.x, dummy.y, dummy.width, dummy.height,knight.x, knight.y, knight.width, knight.height)
		&& dummy.counter != 0 && Paused == 0 && dummy.state != ATTACK) 
	{
		dummy.state = ATTACK;
		Anim_Reset(&dummy.attack);
		// Handle collision
		
		HPBARHEALTH -= 0.10f;
		hitTimer = 0.15f;
		if (HPBARHEALTH < 0) {
			HPBARHEALTH = 0;
		}
		
	}
	//If Player and SpeedBoost collide, gain a speed boost for 5 seconds.
	if (speedboost.collected != 1) {
		if (IsSquareColliding2(randomX, randomY, speedboost.width, speedboost.height, knight.x, knight.y, knight.width, knight.height))
		{
			knight.speed += 10.0f;
			speedboost.collected = 1;
			effects.noOfEffects += 1;
		}
	}

	//Draw a smaller HP bar below the player to show how much health the player has.
	//Disappears when SpeedBoost is collected.
	CP_Settings_Fill(CP_Color_Create(0, 70, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(knight.x - (knight.width / 2.0f), knight.y + (knight.height / 2.0f + 10.0f), HPBARHEALTH, 10.0f);

	//Draw Speed-Boost Power Up Bar
	if (speedboost.collected == 0) {
		CP_Settings_Fill(CP_Color_Create(0, 255, 255, 255));
		CP_Graphics_DrawRect(randomX, randomY, 10.0f, 10.0f);
	}

	//Draw the Effect Status Box
	if (effects.noOfEffects > 0) {
		for (int i = 0; i < effects.noOfEffects; i++) {
			CP_Settings_Fill(CP_Color_Create(255, 0, 255, 255));
			CP_Graphics_DrawRect(windowLength - 100, windowHeight - 100 - (i * 60), 50.0f, 50.0f);
		}
	}
	//Draw the Heart as a a circle for now.
	CP_Settings_Fill(CP_Color_Create(255, 70, 0, 255));
	if (HPBARHEALTH != 0)
	{
		for (int i = lives; i != 0; i--)
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
			knight.x = windowLength / 2.0f;
			knight.y = windowHeight / 2.0f;
			HPBARHEALTH = knight.width;
		}
	}
	//Draw the pause button in the top right corner of the screen.
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(50, 50, 50, 50);

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
		CP_Graphics_DrawRect(windowLength / 2.0f + ((i - 2) * 60), windowHeight - 45, 50, 50);
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
		}

		//If resume button is clicked, close pause mode and resume.
		if (IsAreaClicked(windowLength / 2 - 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			Paused = 0;
		}

		//Menu Button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 + 300, windowHeight / 2 + 200, 200, 100);

		//Resume Button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 - 300, windowHeight / 2 + 200, 200, 100);
	}
	//---------------------------------------------------------
	// Generating Projectie
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].state == 1) {
			CP_Image_DrawSubImageAdvanced(arrow, bullets[i].x, bullets[i].y, 32, 32, 32, 0, 48, 16, 255, bullets[i].angle);
		}
	}
	//CP_Graphics_DrawCircle(knight.x, knight.y, knight.size);
	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	
	if (bg2 != NULL)
	{
		CP_Image_Draw(bg2, 0, 0, windowLength, windowHeight, 255);
	}
}

void Game_Exit(void)
{
	CP_Font_Free(&font_menu);
}