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

MapData* current_map;
Player knight;
CP_Image bg, arrow, bg2;
Enemy dummy;
float Vec_X, Vec_Y, dist, Norm_X, Norm_Y, VDP_X, VDP_Y, VDP_dist, NVDP_X, NVDP_Y;

Bullet bullets[MAX_BULLETS];
char text[100];
CP_Font font_menu = NULL;

int max_obs;
Obstacle obstacle[MAX_OBSTACLE];

//---------------------------------------------------------
//Darrel
//---------------------------------------------------------
Timer timer;
KillCount killcount;

int lives = 3;
int GAMEOVER = 0;
int HPBARHEALTH;
float hitTimer = 0.0f;
int ClickedSlot = 1;
int Paused = 0;
int GameMode = 0; //0 = Time-Based, 1 = Kill-Based

// ++ Darrell's Boost Spawn Variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

BoostType boostType;
float x;
float y;
int boostCollected = 0;

float enemyDmg; //How much damage the enemy is dealing
float enemyDmgMultiplier; //Dmg Multiplier

int DummyDmgTaken;
float boostSpawnTimer;

float boostWidth;
float boothHeight;

Effect effects[BOOST_COUNT];
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------
float ObsminX, ObsmaxX, ObsminY, ObsmaxY;
float KnightminX, KnightmaxX, KnightminY, KnightmaxY;
float DummmyminX, DummymaxX, DummyminY, DummymaxY;
float overlapX, overlapY;

CP_Sound slash;
CP_Sound bowarrow;
CP_Sound player_damage;
CP_Sound hp;

CP_Image bow, sword, potion;

void Game_Init(void)
{
	knight.x = 800.0f;
	knight.y = 450.0f;
	knight.speed = 210.0f;
	knight.state = IDLE;
	knight.width = 50;
	knight.height = 60;
	knight.iFrame = 0;
	knight.iFrameTimer = 0.0f;

	dummy.x = random_range(100, 1500);
	dummy.y = random_range(100, 600);
	dummy.width = 60;
	dummy.height = 40;
	dummy.counter = 10;
	dummy.chase = 0;
	dummy.dead = 0;

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

	current_map = &maps[current_level][random_map[current_level]];
	max_obs = current_map->size;
	bg = CP_Image_Load(current_map->image_path);
	bg2 = CP_Image_Load(current_map->obstacle_path);

	arrow = CP_Image_Load("Assets/Arrow.png");
	Anim_Init(&knight.idle, "Assets/Knight_Idle.png", 120, 80, 0.12f, 1);
	Anim_Init(&knight.run, "Assets/Knight_Run.png", 120, 80, 0.12f, 1);
	Anim_Init(&knight.attack, "Assets/Knight_Attack.png", 120, 80, 0.10f, 0);
	Anim_Init(&dummy.idle, "Assets/Bear_Idle.png", 32, 32, 0.12f, 1);
	Anim_Init(&dummy.run, "Assets/Bear_Run.png", 32, 32, 0.12f, 1);
	Anim_Init(&dummy.attack, "Assets/Bear_Attack.png", 32, 32, 0.10f, 0);

	font_menu = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_EllipseMode(CP_POSITION_CENTER);

	// Obstacle Values based on one map only for testing
	if (bg2 != NULL)
	{
		for (int i = 0; i < max_obs; i++) {
			obstacle[i].x = *(current_map->arr_x + i);
			obstacle[i].y = *(current_map->arr_y + i);
			obstacle[i].width = *(current_map->arr_w + i);
			obstacle[i].height = *(current_map->arr_h + i);
		}
	}

	// Preset bullet state, inactive
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].state = 0;
	}

	//---------------------------------------------------------
	//Darrel
	//---------------------------------------------------------
	// ++ Darrell's Boost Variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Boost Sizing
	boostWidth = 70.0f;
	boothHeight = 70.0f;

	//First boost spawns right away, the next one after boostSpawnTimer runs out
	SpawnBoost(&boostType, &x, &y, &boostCollected, &boostSpawnTimer,
		CP_System_GetWindowWidth(), CP_System_GetWindowHeight());

	//Enemy Damage by % (after knight.width is set above)
	enemyDmgMultiplier = 15.0f; // Adjust this multiplier to change the enemy's damage
	enemyDmg = (knight.width * (enemyDmgMultiplier / 100.0f));

	//Dummy Damage Taken
	DummyDmgTaken = 1;

	//Effects Values
	for (int i = 0; i < BOOST_COUNT; i++) {
		effects[i].stacks = 0;
	}
	effects[SPEED_BOOST].color = CP_Color_Create(0, 255, 0, 255);
	effects[SPEED_BOOST].name = "Speed Boost";
	effects[SPEED_BOOST].description = "Move faster.";

	effects[MORE_MAXHP].color = CP_Color_Create(0, 0, 255, 255);
	effects[MORE_MAXHP].name = "Toughness";
	effects[MORE_MAXHP].description = "Enemies deal less damage.";

	effects[DAMAGE_BOOST].color = CP_Color_Create(255, 0, 0, 255);
	effects[DAMAGE_BOOST].name = "Damage Boost";
	effects[DAMAGE_BOOST].description = "You deal higher damage.";

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Pause Mode
	Paused = 0;

	HPBARHEALTH = knight.width;

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

	// qusyairie -------------------------
	// sound effects
	bowarrow = CP_Sound_Load("./Assets/Audios/bow and arrow.wav"); // projectile sound effect
	slash = CP_Sound_Load("./Assets/Audios/slashing.wav"); // sword attack sound
	player_damage = CP_Sound_Load("./Assets/Audios/player damage.wav"); // player taking damage sound effect
	hp = CP_Sound_Load("./Assets/Audios/potion.wav"); // potion sound effect

	// items in inventory
	bow = CP_Image_Load("./Assets/Weapons/bow.png");
	sword = CP_Image_Load("./Assets/Weapons/sword.png");
	potion = CP_Image_Load("./Assets/health potion.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
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
	//Countdown only runs in the time-based mode
	if (GameMode == 0 && GAMEOVER != 1 && Paused == 0 && timer.timeLeft > 0.0f) {
		timer.timeLeft -= dt;
		if (timer.timeLeft <= 0.0f) {
			timer.timeLeft = 0.0f;
			GAMEOVER = 1; // time's up
		}
	}

	//Spawn a new random boost every time the spawn timer runs out
	if (GAMEOVER != 1 && Paused == 0) {
		boostSpawnTimer -= dt;
		if (boostSpawnTimer <= 0.0f) {
			SpawnBoost(&boostType, &x, &y, &boostCollected, &boostSpawnTimer, windowLength, windowHeight);
		}
	}
	//---------------------------------------------------------
	// Player's Control
	if (GAMEOVER != 1 && Paused == 0) {
		float move_x = 0, move_y = 0;
		if (CP_Input_KeyDown(KEY_A))
		{
			move_x -= 1.0f; moving = 1; knight.is_right = 0;
		}
		if (CP_Input_KeyDown(KEY_D))
		{
			move_x += 1.0f; moving = 1; knight.is_right = 1;
		}
		if (CP_Input_KeyDown(KEY_W))
		{
			move_y -= 1.0f; moving = 1;
		}
		if (CP_Input_KeyDown(KEY_S))
		{
			move_y += 1.0f; moving = 1;
		}
		if (move_x != 0.0f || move_y != 0.0f)
		{
			float length = sqrtf((move_x * move_x) + (move_y * move_y));
			float unit_x = move_x / length;
			float unit_y = move_y / length;
			knight.x += unit_x * knight.speed * dt;
			knight.y += unit_y * knight.speed * dt;
		}
	}
	//---------------------------------------------------------------------
	//For testing purposes
	CP_Graphics_DrawRect(knight.x, knight.y, knight.width, knight.height);
	CP_Graphics_DrawRect(dummy.x, dummy.y, dummy.width, dummy.height);
	//---------------------------------------------------------------------
	// decide the state (can't change state mid-attack)
	if (knight.state != ATTACK && ClickedSlot == 1)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused != 1)
		{
			knight.state = ATTACK;
			Anim_Reset(&knight.attack);
			CP_Sound_Play(slash);
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
		Anim_Draw(&knight.attack, knight.x, knight.y - knight.height / 2.0f, draw_width, 120);
		break;
	}
	draw_width = knight.is_right == 1 ? 110 : -110;
	/*------------------------------------------------------------------------------*/
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	// when slot 2 is selected and used
	if (knight.state != ATTACK && ClickedSlot == 2)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused == 0 && !IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
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
					bullets[i].start_x = knight.x;
					bullets[i].start_y = knight.y;
					// Bullet speed
					bullets[i].speed_x = Norm_X * 450.0f;
					bullets[i].speed_y = Norm_Y * 450.0f;
					// Calculate and store the value of the direction the bullet is facing
					bullets[i].angle = CP_Math_Degrees(atan2f(Vec_Y, Vec_X));
					bullets[i].size = 15.0f; // Size
					bullets[i].state = 1; // Active State
					CP_Sound_Play(bowarrow);
					break;
				}
			}
		}
		else
		{
			knight.state = moving ? RUN : IDLE;
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
					dummy.counter -= DummyDmgTaken;
					if (dummy.counter < 0) {
						dummy.counter = 0;
					}
					bullets[i].state = 0;
				}
			}
			for (int y = 0; y < max_obs; y++) {
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
	for (int i = 0; i < max_obs; i++) {
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
	for (int i = 0; i < max_obs; i++) {
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

	// only chase when not in collision
	if (dummy.chase == 1 && !touching && dummy.state != ATTACK && GAMEOVER != 1) {
		dummy.x += (NVDP_X * 180.0f) * dt;
		dummy.y += (NVDP_Y * 180.0f) * dt;

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
			Anim_Draw(&dummy.idle, dummy.x, dummy.y - dummy.height / 2, draw_width, 80);
			break;
		case RUN:
			Anim_Update(&dummy.run, dt);
			Anim_Draw(&dummy.run, dummy.x, dummy.y - dummy.height / 2, draw_width, 80);
			break;
		case ATTACK:
			if (Anim_Update(&dummy.attack, dt))
			{
				dummy.state = IDLE;
			}
			Anim_Draw(&dummy.attack, dummy.x, dummy.y - dummy.height / 2, draw_width, 80);
			break;
		}
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		snprintf(text, sizeof(text), "%d", dummy.counter);
		CP_Font_DrawText(text, dummy.x, dummy.y - (dummy.height / 2.0f) - 12.0f);
		CP_Settings_TextSize(30.0f);
	}
	for (int i = 0; i < max_obs; i++) {
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

	//Testing dummy respawn
	if (dummy.dead == 1 && killcount.amount != 10) {
		dummy.dead = 0;
		do {
			dummy.x = random_range(100, 1500);
			dummy.y = random_range(100, 600);
			// Calculate the Euclidean distance using Pythagoras Theorem
			VDP_dist = sqrtf(euclidean_distance(knight.x, knight.y, dummy.x, dummy.y));
		} while (VDP_dist <= 250.0f);
		dummy.counter = 10;
	}

	//If Pause is clicked, pause the game first and stop everything.
	if (IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY()) || CP_Input_KeyTriggered(KEY_ESCAPE))
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
	//If Player and Dummy collide, the player loses HP.
	if (IsSquareColliding2(dummy.x, dummy.y, dummy.width, dummy.height, knight.x, knight.y, knight.width, knight.height) && dummy.counter != 0 && Paused == 0 && knight.iFrame == 0 && GAMEOVER != 1) {
		dummy.state = ATTACK;
		Anim_Reset(&dummy.attack);
		// Handle collision
		HPBARHEALTH -= enemyDmg;
		hitTimer = 0.15f;
		CP_Sound_Play(player_damage);
		if (HPBARHEALTH < 0) {
			HPBARHEALTH = 0;
		}
		knight.iFrame = 1;
		knight.iFrameTimer = 1.0f;
		if (ClickedSlot == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) { // if player and enemy collide, and player uses melee attack
			dummy.counter -= 1; // enemy taking damage
			if (dummy.counter < 0) {
				dummy.counter = 0;
			}
		}
	}
	if (knight.iFrame == 1) {
		if (knight.iFrameTimer != 0) {
			knight.iFrameTimer -= dt;
		}
		if (knight.iFrameTimer <= 0.0f) {
			knight.iFrame = 0;
			knight.iFrameTimer = 0.0f;
		}
	}

	// Drawing UI border
	CP_Settings_Fill(CP_Color_Create(140, 90, 50, 100));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(windowLength / 2, windowHeight - 65.0f, windowLength, 130.0f);

	//Draw a smaller HP bar below the player to show how much health the player has.
	CP_Settings_Fill(CP_Color_Create(0, 70, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Graphics_DrawRect(knight.x - (knight.width / 2.0f), knight.y + (knight.height / 2.0f + 10.0f), HPBARHEALTH, 10.0f);

	// ++ Darrell's Boost Spawn Variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if (boostCollected == 0) {
		// Draw the boost (only while it hasn't been collected)
		CP_Settings_RectMode(CP_POSITION_CENTER);   // match how the collision treats x,y
		CP_Settings_Fill(effects[boostType].color);
		CP_Graphics_DrawRect(x, y, boostWidth, boothHeight);

		// Pickup check
		if (IsSquareColliding2(x, y, boostWidth, boothHeight, knight.x, knight.y, knight.width, knight.height))
		{
			switch (boostType) {
			case SPEED_BOOST:
				knight.speed += 20.0f;
				break;
			case MORE_MAXHP:
				enemyDmgMultiplier -= 2.0f;
				enemyDmg = knight.width * (enemyDmgMultiplier / 100.0f);
				break;
			case DAMAGE_BOOST:
				DummyDmgTaken += 1;
				break;
			}
			effects[boostType].stacks += 1;
			boostCollected = 1;
		}
	}

	//Draw the Effect Status Icons + hover tooltip
	int slot = 0;
	int hovered = -1;
	float hoverX = 0.0f, hoverY = 0.0f;

	for (int i = 0; i < BOOST_COUNT; i++) {
		if (effects[i].stacks > 0) {
			float cx = windowLength - 95.0f - (slot * 80.0f);
			float cy = windowHeight - 60.0f;

			CP_Settings_Fill(effects[i].color);
			CP_Graphics_DrawCircle(cx, cy, 70);

			float dx = mouseX - cx; //For hovering over the effect icons
			float dy = mouseY - cy;
			if ((dx * dx) + (dy * dy) <= 35.0f * 35.0f) {
				hovered = i;
				hoverX = cx;
				hoverY = cy;
			}
			slot++;
		}
	}

	if (hovered != -1) {
		//Draw the box
		float boxW = 300.0f;
		float boxH = 120.0f;
		float boxX = hoverX + 35.0f - boxW;
		float boxY = hoverY - 35.0f - boxH - 10.0f;
		if (boxX < 0.0f)
		{
			boxX = 0.0f; //In case it exceeds out
		}

		// Background box
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(CP_Color_Create(20, 20, 20, 230));
		CP_Graphics_DrawRect(boxX, boxY, boxW, boxH);

		// Text
		char buf[255];
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

		CP_Settings_TextSize(30.0f);
		CP_Font_DrawText(effects[hovered].name, boxX + 15.0f, boxY + 10.0f);

		CP_Settings_TextSize(15.0f);
		CP_Font_DrawText(effects[hovered].description, boxX + 15.0f, boxY + 50.0f);

		snprintf(buf, sizeof(buf), "Stacks: x%d", effects[hovered].stacks);
		CP_Font_DrawText(buf, boxX + 15.0f, boxY + 85.0f);

		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_RectMode(CP_POSITION_CENTER);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
	CP_Settings_Fill(CP_Color_Create(130, 130, 130, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(50, 50, 50, 50);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(30.0f);
	CP_Font_DrawText("| |", 50, 50);

	//Inventory Slot Selection
	if (CP_Input_KeyTriggered(KEY_1)) { ClickedSlot = 1; }
	if (CP_Input_KeyTriggered(KEY_2)) { ClickedSlot = 2; }
	if (CP_Input_KeyTriggered(KEY_3)) { ClickedSlot = 3; }

	//Draw the Inventory in white (3 boxes)
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CENTER);

	for (int i = 1; i <= 3; i++) {
		if (i == ClickedSlot) {
			CP_Settings_Fill(CP_Color_Create(235, 255, 0, 255)); // yellow colour for the selected slot
		}
		else {
			CP_Settings_Fill(CP_Color_Create(210, 125, 25, 255)); // brown colour for the other slots
		}
		CP_Graphics_DrawRect(windowLength / 2.0f + ((i - 2) * 120), windowHeight - 65, 100, 100);
	}

	CP_Image_Draw(sword, windowLength / 2.0f + ((-1) * 120), windowHeight - 65.0f, 48.0f, 100.0f, 255); // drawing sword in 1st inventory slot
	CP_Image_Draw(bow, windowLength / 2, windowHeight - 65.0f, 60.0f, 91.0f, 255); // drawing bow in 2nd inventory slot
	CP_Image_Draw(potion, windowLength / 2.0f + ((1) * 120), windowHeight - 65.0f, 48.0f, 78.0f, 255); // drawing health potion in 3rd inventory slot

	// when slot 3 is selected and used
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused == 0 && !IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY()) && ClickedSlot == 3) {
		if (HPBARHEALTH < knight.width) {
			CP_Sound_Play(hp);
			HPBARHEALTH += 10; // regains 10 hp
			if (HPBARHEALTH > knight.width) {
				HPBARHEALTH = knight.width;
			}
		}
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

		if (timer.timeLeft == 0.0f) {
			CP_Settings_Fill(CP_Color_Create(45, 255, 0, 255));
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(windowLength - 200, killcount.y, 250, killcount.height);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Settings_TextSize(50.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("CONTINUE", windowLength - 200, killcount.y);
		}
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

		if (killcount.amount == killcount.total) {
			CP_Settings_Fill(CP_Color_Create(45, 255, 0, 255));
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(windowLength - 200, killcount.y, 250, killcount.height);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			CP_Settings_TextSize(50.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("CONTINUE", windowLength - 200, killcount.y);
		}
	}

	if (Paused == 1 && GAMEOVER == 0)
	{
		// drawing grey overlay
		CP_Settings_Fill(CP_Color_Create(130, 130, 130, 100));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2, windowLength, windowHeight);

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
		CP_Graphics_DrawRect(windowLength / 2 + 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("MENU", windowLength / 2 + 300, windowHeight / 2 + 200);

		//Resume Button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 - 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("RESUME", windowLength / 2 - 300, windowHeight / 2 + 200);
	}
	//---------------------------------------------------------
	// Generating Projectie
	for (int i = 0; i < MAX_BULLETS; i++) {
		dist = euclidean_distance(bullets[i].x, bullets[i].y, bullets[i].start_x, bullets[i].start_y);
		float max = MAX_BULLET_DISTANCE * MAX_BULLET_DISTANCE;
		if (dist >= max) {
			bullets[i].state = 0;
		}
		if (bullets[i].state == 1) {
			CP_Image_DrawSubImageAdvanced(arrow, bullets[i].x, bullets[i].y, 32, 32, 32, 0, 48, 16, 255, bullets[i].angle);
		}
	}

	// CHI SHI
	if (CP_Input_KeyTriggered(KEY_8))
	{ 
		random_map[current_level] = 0; CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_9))
	{ 
		random_map[current_level] = 1; CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_0))
	{ 
		random_map[current_level] = 2; CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}

	// QUS EDIT THIS FOR NXT LVL
	if (CP_Input_KeyTriggered(KEY_N) && current_level < LEVELS - 1) {
		current_level++;
		CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	//if (CP_Input_KeyDown(KEY_Q)) {
	//	current_level = 0;
	//	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	//}

	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(bg2, 0, 0, windowLength, windowHeight, 255);
	

	if (GAMEOVER == 1) { // when player dies
		Paused = 1; // pauses the game so the player has no inputs

		// drawing red overlay
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 100));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2, windowLength, windowHeight);

		// game over text
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(100.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("GAME OVER", windowLength / 2.0f, windowHeight / 2.0f);

		// reassurance over text
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(20.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("everybody makes mistakes...", windowLength / 2.0f, windowHeight / 2.0f + 60);

		// menu button
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("MENU", windowLength / 2, windowHeight / 2 + 200);

		// same logic as menu button previosuly
		if (IsAreaClicked(windowLength / 2, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

void Game_Exit(void)
{
	CP_Font_Free(font_menu);
}