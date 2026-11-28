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
const char* exception[2] = {"Assets/Characters/Mobs/Spider_Idle.png", "Assets/Characters/Mobs/Pigeon_Fly.png"};
Player knight;
CP_Image bg, arrow, bg2, web, slashimg;
Enemy dummy[MAX_DUMMY];
float Vec_X, Vec_Y, dist, Norm_X, Norm_Y, VDP_X, VDP_Y, VDP_dist, NVDP_X, NVDP_Y;
Melee melee;
Bullet webshots[MAX_BULLETS];
Bullet bullets[MAX_BULLETS];
Bullet slashes[MAX_BULLETS];
char text[100];
CP_Font font_menu = NULL;
int dummy_width, dummy_height, max_obs;
float animTimer;
Obstacle obstacle[MAX_OBSTACLE];

//---------------------------------------------------------
//Darrell
//---------------------------------------------------------
Timer timer;
KillCount killcount;

int lives = 3, GAMEOVER = 0, GAMEWIN, HPBARHEALTH, ClickedSlot = 1, Paused = 0;
int GameMode = 0; //0 = Time-Based, 1 = Kill-Based, 2 = Boss Fight
float hitTimer = 0.0f;

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
float boostHeight;

Effect effects[BOOST_COUNT];
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------
float ObsminX, ObsmaxX, ObsminY, ObsmaxY;
float KnightminX, KnightmaxX, KnightminY, KnightmaxY;
float DummmyminX, DummymaxX, DummyminY, DummymaxY;
float overlapX, overlapY;

// weapons and sounds
CP_Sound slash, bowarrow, player_damage, hp, igm, player_death;
CP_Image bow, sword, potion;

CP_Color black, white, red, yellow, green, orange, blue, grey, darkred;

Boss boss;
float boss2player_angle;

// tutorial
float tut_timer = 0.0f;
int tut_showing = 1;
int tut_stage = 0;
CP_Image tutorial;

char const* slotNum[] = {" ", "1", "2", "3" };

int level_complete;
//Used for potion
int usedState = 0;
float PotionCD = 0.0f;
float ArrowUsed = 0;
float ArrowCD = 3.0f;

void Game_Init(void)
{
	GameMode = (current_level == LEVELS - 1) ? 2 : random_range(0, 1);
	black = CP_Color_Create(0, 0, 0, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	red = CP_Color_Create(255, 0, 0, 255);
	yellow = CP_Color_Create(255, 255, 0, 255);
	green = CP_Color_Create(0, 255, 0, 255);
	orange = CP_Color_Create(255, 128, 0, 255);
	blue = CP_Color_Create(0, 0, 255, 255);
	grey = CP_Color_Create(130, 130, 130, 255);
	darkred = CP_Color_Create(123, 14, 14, 255);

	knight.x = 800.0f;
	knight.y = 450.0f;
	if (GameMode == 2) knight.x = 300.0f;
	knight.speed = 210.0f;
	knight.state = IDLE;
	knight.width = 50;
	knight.height = 60;
	knight.iFrame = 0;
	knight.iFrameTimer = 0.0f;
	knight.dead = 0;
	melee.width = 100;
	melee.height = 120;
	melee.state = 0;
	melee.timer = 0.0f;
	level_complete = 0;

	//Reset when next game state
	usedState = 0;
	PotionCD = 0.0f;
	ArrowUsed = 0;
	ArrowCD = 3.0f;

	for (int i = 0; i < MAX_DUMMY; ++i)
	{
		dummy[i].x = random_range(100, 1500);
		dummy[i].y = random_range(100, 600);
		dummy[i].width = 60;
		dummy[i].height = 40;
		dummy[i].counter = 10;
		dummy[i].chase = 0;
		dummy[i].shootTimer = random_range(0, 2);
		dummy[i].deathTimer = 2.0f;
	}

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
	animTimer = current_map->char_moves[0] == exception[0] ? 0.10f : 0.30f;
	max_obs = current_map->size;
	dummy_width = current_map->char_width;
	dummy_height = current_map->char_height;
	bg = CP_Image_Load(current_map->image_path);
	bg2 = CP_Image_Load(current_map->obstacle_path);
	web = CP_Image_Load("Assets/Weapons/web.png");
	slashimg = CP_Image_Load("Assets/Characters/Boss/Boss_Slash.png");

	arrow = CP_Image_Load("Assets/Weapons/Arrow.png");
	Anim_Init(&knight.idle, "Assets/Characters/Player/Knight_Idle.png", 120, 80, 0.12f, 1);
	Anim_Init(&knight.run, "Assets/Characters/Player/Knight_Run.png", 120, 80, 0.12f, 1);
	Anim_Init(&knight.attack, "Assets/Characters/Player/Knight_Attack.png", 120, 80, 0.10f, 0);
	Anim_Init(&knight.die, "Assets/Characters/Player/Knight_Death.png", 120, 80, 0.10f, 0);
	if (GameMode != 2)
	{
		for (int i = 0; i < MAX_DUMMY; i++)
		{
			Anim_Init(&dummy[i].idle, current_map->char_moves[0], current_map->frame_width, current_map->frame_height, 0.12f, 1);
			Anim_Init(&dummy[i].run, current_map->char_moves[1], current_map->frame_width, current_map->frame_height, 0.12f, 1);
			Anim_Init(&dummy[i].attack, current_map->char_moves[2], current_map->frame_width, current_map->frame_height, 0.10f, 0);
			Anim_Init(&dummy[i].die, current_map->char_moves[3], current_map->frame_width, current_map->frame_height, animTimer, 0);
		}
	}

	if (GameMode == 2)
	{
		for (int i = 0; i < MAX_DUMMY; i++)
		{
			dummy[i].counter = 0;
			dummy[i].dead = 1;
		}
	}
	font_menu = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Font_Set(font_menu); //set the font used
	CP_Settings_TextSize(72); //set the font size
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_EllipseMode(CP_POSITION_CENTER);

	
	for (int i = 0; i < max_obs; i++) 
	{
		for (int i = 0; i < max_obs; i++) {
			obstacle[i].x = *(current_map->arr_x + i);
			obstacle[i].y = *(current_map->arr_y + i);
			obstacle[i].width = *(current_map->arr_w + i);
			obstacle[i].height = *(current_map->arr_h + i);
		}
		// Initialise all obstacle state as 0 (deactivated)
		for (int i = 0; i < max_obs; i++)
		{
			obstacle[i].state = 0;
		}
	}


	// Preset bullet state, inactive
	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].state = 0;
		webshots[i].state = 0;
		slashes[i].state = 0;
	}

	//---------------------------------------------------------
	//Darrel
	//---------------------------------------------------------
	// ++ Darrell's Boost Variables +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//Boost Sizing
	boostWidth = 70.0f;
	boostHeight = 70.0f;

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
	effects[SPEED_BOOST].color = green;
	effects[SPEED_BOOST].name = "Speed Boost";
	effects[SPEED_BOOST].description = "Move faster.";

	effects[MORE_MAXHP].color = blue;
	effects[MORE_MAXHP].name = "Toughness";
	effects[MORE_MAXHP].description = "Enemies deal less damage.";

	effects[DAMAGE_BOOST].color = red;
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
	GAMEWIN = 0;
	GAMEOVER = 0; // Reset game over state
	lives = 3; // Reset lives
	if (GameMode == 2) Boss_Init(&boss);

	// sound effects
	bowarrow = CP_Sound_Load("./Assets/Audios/bow and arrow.wav"); // projectile sound effect
	slash = CP_Sound_Load("./Assets/Audios/slashing.wav"); // sword attack sound
	player_damage = CP_Sound_Load("./Assets/Audios/player damage.wav"); // player taking damage sound effect
	hp = CP_Sound_Load("./Assets/Audios/potion.wav"); // potion sound effect
	igm = CP_Sound_Load("./Assets/Audios/igm.wav"); // in game music
	player_death = CP_Sound_Load("./Assets/Audios/player death.wav"); // player dying sound effect

	// items in inventory
	bow = CP_Image_Load("./Assets/Weapons/bow.png");
	sword = CP_Image_Load("./Assets/Weapons/sword.png");
	potion = CP_Image_Load("./Assets/health potion.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);

	// tutorial
	tutorial = CP_Image_Load("./Assets/tutorial1.png");

	CP_Sound_PlayAdvanced(igm, 0.3f, 1.0f, TRUE, CP_SOUND_GROUP_MUSIC);
}

void Game_Update(void)
{
	float windowLength = CP_System_GetWindowWidth();
	float windowHeight = CP_System_GetWindowHeight();

	if (tut_showing) {
		tut_timer += CP_System_GetDt();
		CP_Graphics_ClearBackground(black);
		float fadeDuration = 1.0f;
		float stageDuration = 30.0f;
		int alpha = 255;

		// 1. Calculate fade in / fade out alpha for the current 30s stage
		if (tut_timer <= fadeDuration) {
			alpha = (int)((tut_timer / fadeDuration) * 255.0f);
		}
		else if (tut_timer >= (stageDuration - fadeDuration)) {
			alpha = (int)(((stageDuration - tut_timer) / fadeDuration) * 255.0f);
		}
		if (alpha < 0) alpha = 0;
		if (alpha > 255) alpha = 255;

		// 2. Render based on the current stage
		if (tut_stage == 0) {
			// STAGE 0: Display only the tutorial image
			CP_Settings_ImageMode(CP_POSITION_CORNER);
			CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
			CP_Image_Draw(tutorial, 0, 0, 1600, 900, alpha);
		}
		else if (tut_stage == 1) {
			// STAGE 1: Display only the tutorial texts
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
			CP_Settings_TextSize(50);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("EACH LEVEL'S GAME MODE IS RANDOMISED", windowLength / 2, 300);
			CP_Font_DrawText("REACH THE KILL COUNT OR SURVIVE THE CLOCK", windowLength / 2, 350);
			CP_Font_DrawText("DO NOT LOSE ALL 3 LIVES", windowLength / 2, 450);
			CP_Font_DrawText("YOU WILL NEED IT AGAINST THE BOSS", windowLength / 2, 550);
			CP_Font_DrawText("PRESS ENTER TO START, GOOD LUCK", windowLength / 2, 600);
		}

		// 3. Advance stage or exit after 30s or pressing Enter
		if (tut_timer >= stageDuration || CP_Input_KeyTriggered(KEY_ENTER)) {
			tut_timer = 0.0f; // Reset timer for the next stage
			tut_stage++;      // Move to the next stage

			if (tut_stage > 1) {
				tut_showing = 0;
				tut_stage = 0; // Reset stage in case tutorial is replayed later
				CP_Graphics_ClearBackground(grey);
			}
		}
		return;
	}

	CP_Graphics_ClearBackground(grey);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(bg, 0, 0, windowLength, windowHeight, 255);
	float dt = CP_System_GetDt();
	float half_width = knight.width / 2.0f;
	float half_height = knight.height / 2.0f;
	int moving = 0;
	//---------------------------------------------------------
	//DarrelL
	//---------------------------------------------------------


	if (hitTimer > 0.0f && Paused == 0) {
		hitTimer -= dt;
	}

	//Countdown only runs in the time-based mode
	if (GameMode == 0 && GAMEOVER != 1 && Paused == 0 && timer.timeLeft > 0.0f) {
		timer.timeLeft -= dt;
		if (timer.timeLeft <= 0.0f) {
			timer.timeLeft = 0.0f;
			//GAMEOVER = 1; // time's up
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
	if (GAMEOVER != 1 && Paused == 0 && knight.state != DIE) {
		float move_x = 0, move_y = 0;
		if (CP_Input_KeyDown(KEY_A))
		{
			move_x -= 1.0f; moving = 1; if (melee.state != 1) knight.is_right = 0;
		}
		if (CP_Input_KeyDown(KEY_D))
		{
			move_x += 1.0f; moving = 1; if (melee.state != 1) knight.is_right = 1;
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
	// Melee facing direction & hitbox
	if (ClickedSlot == 1) {
		if (knight.is_right) {
			melee.x = knight.x + 25.0f;
			melee.y = knight.y;
		}
		else if (!knight.is_right) {
			melee.x = knight.x - 25.0f;
			melee.y = knight.y;
		}
	}
	//---------------------------------------------------------------------
	//For testing purposes (hitboxes)
	//if (ClickedSlot == 1)
	//{
	//	CP_Settings_Fill(green);
	//	CP_Graphics_DrawRect(melee.x, melee.y, melee.width, melee.height);
	//}
	//CP_Settings_Fill(black);
	//CP_Graphics_DrawRect(knight.x, knight.y, knight.width, knight.height);
	//for (int i = 0; i < MAX_DUMMY; ++i)
	//{
	//	if (dummy[i].dead != 1)
	//	{
	//		CP_Graphics_DrawRect(dummy[i].x, dummy[i].y, dummy[i].width, dummy[i].height);
	//	}
	//}
	//---------------------------------------------------------------------
	// decide the state (can't change state mid-attack)
	if (knight.state != ATTACK && ClickedSlot == 1 && knight.state != DIE)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused != 1)
		{
			//---------------------------------------------------------------------
			float mouseX = CP_Input_GetMouseX();
			if (mouseX < knight.x)
			{
				knight.is_right = 0;
				melee.state = 1;
				melee.timer = 0.4f;
				melee.x = knight.x - 25.0f;
			}
			else
			{
				knight.is_right = 1;
				melee.state = 1;
				melee.timer = 0.4f;
				melee.x = knight.x + 25.0f;
			}
			//---------------------------------------------------------------------
			knight.state = ATTACK;
			Anim_Reset(&knight.attack);
			
			CP_Sound_Play(slash);
			//----------------------------------------------------
			// Melee Hitbox into enemy but not for birds
			if (current_map->char_moves[0] != exception[1])
			{
				for (int i = 0; i < MAX_DUMMY; ++i)
				{
					if (IsSquareColliding2(dummy[i].x, dummy[i].y, dummy[i].width, dummy[i].height, melee.x, melee.y, melee.width, melee.height) && dummy[i].counter != 0 && Paused == 0 && GAMEOVER != 1)
					{
						dummy[i].counter -= 2 + DummyDmgTaken - 1;
						if (dummy[i].counter < 0) dummy[i].counter = 0;
					}
				}
				if (GameMode == 2 && IsSquareColliding2(boss.x, boss.y, boss.width, boss.height, melee.x, melee.y, melee.width, melee.height))
					Boss_TakeDamage(&boss, 20);
			}
			//----------------------------------------------------
		}
		else
		{
			knight.state = moving ? RUN : IDLE;
		}
	}
	//---------------------------------------------------------------------
	// Melee swing cd
	if (melee.state == 1) {
		if (melee.timer != 0) {
			melee.timer -= dt;
		}
		if (melee.timer <= 0.0f) {
			melee.state = 0;
			melee.timer = 0.0f;
		}
	}
	//---------------------------------------------------------------------
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
	case DIE:
		if (Anim_Update(&knight.die, dt))
		{
			knight.dead = 1;
		}
		Anim_Draw(&knight.die, knight.x, knight.y - knight.height / 2.0f, draw_width, 120);
		break;
	}
	draw_width = knight.is_right == 1 ? 110 : -110;
	/*------------------------------------------------------------------------------*/
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	// when slot 2 is selected and used
	if (knight.state != ATTACK && ClickedSlot == 2 && knight.state != DIE)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused == 0 && !IsAreaClicked(50, 50, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bullets[i].state == 0 && ArrowUsed < 5) {
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
					ArrowUsed += 1;
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
			for (int y = 0; y < MAX_BULLETS; y++) {
				if (dummy[y].counter != 0) {
					if (bullets[i].x <= dummy[y].x + dummy[y].width / 2.0f &&
						bullets[i].x >= dummy[y].x - dummy[y].width / 2.0f &&
						bullets[i].y <= dummy[y].y + dummy[y].height / 2.0f &&
						bullets[i].y >= dummy[y].y - dummy[y].height / 2.0f) {
						dummy[y].counter -= 1;
						bullets[i].state = 0;
					}
				}
			}
			
			if (boss.state != DIE) {
				if (bullets[i].x <= boss.x + boss.width / 2.0f &&
					bullets[i].x >= boss.x - boss.width / 2.0f &&
					bullets[i].y <= boss.y + boss.height / 2.0f &&
					bullets[i].y >= boss.y - boss.height / 2.0f)
				{
					Boss_TakeDamage(&boss, 0.5);
					bullets[i].state = 0;
				}
			}
			
			for (int y = 0; y < max_obs; y++) {
				if (bullets[i].x <= obstacle[y].x + obstacle[y].width / 2.0f &&
					bullets[i].x >= obstacle[y].x - obstacle[y].width / 2.0f &&
					bullets[i].y <= obstacle[y].y + obstacle[y].height / 2.0f &&
					bullets[i].y >= obstacle[y].y - obstacle[y].height / 2.0f &&
					obstacle[i].state != 0) {
					bullets[i].state = 0;
				}
			}
		}
	}

	if (ArrowUsed >= 5)
	{
		ArrowCD -= dt;
		if (ArrowCD <= 0)
		{
			ArrowCD = 3.0f;
			ArrowUsed = 0;
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
		if (IsSquareColliding(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height, knight.x, knight.y, knight.width, knight.height, &overlapX, &overlapY) && obstacle[i].state != 0) {
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
	if (current_map->char_moves[0] != exception[1])
	{
		for (int i = 0; i < max_obs; i++) {
			for (int y = 0; y < MAX_DUMMY; y++)
			{
				if (IsSquareColliding(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height, dummy[y].x, dummy[y].y, dummy[y].width, dummy[y].height, &overlapX, &overlapY)) {
					if (overlapX < overlapY) {
						dummy[y].x = (dummy[y].x < obstacle[i].x) ? (dummy[y].x -= overlapX) : (dummy[y].x += overlapX);
					}
					else {
						dummy[y].y = (dummy[y].y < obstacle[i].y) ? (dummy[y].y -= overlapY) : (dummy[y].y += overlapY);
					}
				}
			}
		}
	}
	/*------------------------------------------------------------------------------*/
	if (current_map->char_moves[0] == exception[1])
	{
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(bg2, 0, 0, windowLength, windowHeight, 255);
	}
	for (int i = 0; i < MAX_DUMMY; i++)
	{
		if (dummy[i].counter != 0)
		{
			VDP_X = knight.x - dummy[i].x;
			VDP_Y = knight.y - dummy[i].y;
			// Calculate the Euclidean distance using Pythagoras Theorem
			VDP_dist = sqrtf((VDP_X * VDP_X) + (VDP_Y * VDP_Y));
			if (VDP_dist == 0.0f) { VDP_dist = 1.0f; }
			// Normalize the Vector
			NVDP_X = VDP_X / VDP_dist;
			NVDP_Y = VDP_Y / VDP_dist;
		}

		int touching = IsSquareColliding2(dummy[i].x, dummy[i].y, dummy[i].width, dummy[i].height,
			knight.x, knight.y, knight.width, knight.height);
		if (dummy[i].counter != 0 && Paused != 1 && GAMEOVER != 1)
		{
			if (dummy[i].shootTimer > 0.0f)
			{
				dummy[i].shootTimer -= dt;
			}
			if (current_map->char_moves[0] == exception[0] && VDP_dist >= 300 && dummy[i].shootTimer <= 0.0f || current_map->char_moves[0] ==
				exception[1] && dummy[i].shootTimer <= 0.0f)
			{
				for (int s = 0; s < MAX_BULLETS; s++)
				{
					if (webshots[s].state == 0)
					{
						webshots[s].x = dummy[i].x;
						webshots[s].y = dummy[i].y;
						webshots[s].speed_x = NVDP_X * 300;
						webshots[s].speed_y = NVDP_Y * 300;
						webshots[s].angle = CP_Math_Degrees(atan2f(NVDP_Y, NVDP_X));
						webshots[s].state = 1;
						dummy[i].shootTimer = 2.0f;
						break;
					}
				}
			}
		}

		if (VDP_dist <= 300 && dummy[i].counter != 0 && Paused != 1 && current_map->char_moves[0] != exception[1])
		{
			dummy[i].chase = 1;
		}
		else if (VDP_dist >= 300 && VDP_dist <= 500 && dummy[i].counter != 0 && Paused != 1 && current_map->char_moves[0] == exception[1])
		{
			dummy[i].chase = 1;
		}
		else
		{
			dummy[i].chase = 0;
		}
		// only chase when not in collision
		if (dummy[i].chase == 1 && !touching && dummy[i].state != ATTACK && GAMEOVER != 1)
		{
			dummy[i].x += (NVDP_X * 180.0f) * dt;
			dummy[i].y += (NVDP_Y * 180.0f) * dt;
		}
		//--------------------------------------------------
		if (dummy[i].counter != 0 || (dummy[i].state == DIE && dummy[i].dead == 0))
		{
			if (dummy[i].state != ATTACK && dummy[i].state != DIE)
			{
				dummy[i].state = (dummy[i].chase == 1 && !touching) ? RUN : IDLE;
			}
			dummy[i].is_right = knight.x > dummy[i].x ? 1 : 0;
			draw_width = dummy[i].is_right == 1 ? dummy_width : -dummy_width;

			//if (dummy.state != ATTACK) {
			//	dummy.state = dummy.chase == 1 ? RUN : IDLE;
			//}
			//// check if player is on which side of the enemy
			//dummy.is_right = knight.x > dummy.x ? 1 : 0;
			//// turns right if enemy is on the right
			//draw_width = dummy.is_right == 1 ? 80 : -80;

			switch (dummy[i].state)
			{
			case IDLE:
				Anim_Update(&dummy[i].idle, dt);
				Anim_Draw(&dummy[i].idle, dummy[i].x, dummy[i].y - dummy[i].height / 2, draw_width, dummy_height);
				break;
			case RUN:
				Anim_Update(&dummy[i].run, dt);
				Anim_Draw(&dummy[i].run, dummy[i].x, dummy[i].y - dummy[i].height / 2, draw_width, dummy_height);
				break;
			case ATTACK:
				if (Anim_Update(&dummy[i].attack, dt))
				{
					dummy[i].state = IDLE;
				}
				Anim_Draw(&dummy[i].attack, dummy[i].x, dummy[i].y - dummy[i].height / 2, draw_width, dummy_height);
				break;
			case DIE:
				if (Anim_Update(&dummy[i].die, dt))
				{
					dummy[i].dead = 1;
				}
				if(current_map->char_moves[0] != exception[1])
				Anim_Draw(&dummy[i].die, dummy[i].x,
					dummy[i].y - dummy[i].height / 2,
					draw_width, dummy_height);
				break;
			}
			CP_Settings_Fill(black);
			//snprintf(text, sizeof(text), "%d", dummy[i].counter); //TEXT DRAW, CHANGING THIS!!
			//TAG: EnemyHPUI

			float barWidth = dummy[i].width;
			float barHeight = 8.0f;
			float barX = dummy[i].x - barWidth / 2.0f;
			float barY = dummy[i].y + dummy[i].height / 2.0f + 10.0f;   // above the enemy's head
			float hpPercent = (float)dummy[i].counter / 10.0f;          // Get each dummy's current HP percentage

			CP_Settings_RectMode(CP_POSITION_CORNER);

			// foreground (current HP)
			CP_Settings_Fill(green);
			CP_Graphics_DrawRect(barX, barY, barWidth * hpPercent, barHeight);

			CP_Settings_RectMode(CP_POSITION_CENTER);

			CP_Font_DrawText(text, dummy[i].x, dummy[i].y - (dummy[i].height / 2.0f) - 12.0f);
			CP_Settings_TextSize(30.0f);
		}
	}
	// ============================================= BOSS CODES ============================================================== //
	if (GameMode == 2)
	{
		if (Paused == 0 && GAMEOVER != 1 && GAMEWIN != 1)
		{
			Boss_Update(&boss, dt, knight.x, knight.y);
		}
		if (boss.dead) GAMEWIN = 1;

		CP_Graphics_DrawRect(boss.x, boss.y, boss.width, boss.height);
		Boss_Draw(&boss);
		VDP_X = knight.x - boss.x;
		VDP_Y = knight.y - boss.y;
		// Calculate the Euclidean distance using Pythagoras Theorem
		VDP_dist = sqrtf((VDP_X * VDP_X) + (VDP_Y * VDP_Y));
		if (VDP_dist == 0.0f) { VDP_dist = 1.0f; }
		// Normalize the Vector
		NVDP_X = VDP_X / VDP_dist;
		NVDP_Y = VDP_Y / VDP_dist;
		boss2player_angle = CP_Math_Degrees(atan2f(NVDP_Y, NVDP_X));
		// ensure angle is based on 360 instead of going negative
		boss2player_angle = fmodf(boss2player_angle + 360.0f, 360.0f);
		change_dir(&boss, boss2player_angle);

		int playerinrange = boss2player_angle >= 355 && boss2player_angle <= 360 || boss2player_angle >= 0 && boss2player_angle <= 5 ||
			boss2player_angle >= 40 && boss2player_angle <= 50 || boss2player_angle >= 85 && boss2player_angle <= 95 ||
			boss2player_angle >= 130 && boss2player_angle <= 140 || boss2player_angle >= 175 && boss2player_angle <= 185 ||
			boss2player_angle >= 220 && boss2player_angle <= 230 || boss2player_angle >= 265 && boss2player_angle <= 275 ||
			boss2player_angle >= 310 && boss2player_angle <= 320;
		int touching = IsSquareColliding2(boss.x, boss.y, boss.width, boss.height,
			knight.x, knight.y, knight.width, knight.height);

		if (boss.state != BOSS_ATTACK1 && boss.state != BOSS_ATTACK2 &&
			boss.state != BOSS_ATTACK3 && boss.state != BOSS_ATTACK4 &&
			boss.state != BOSS_ATTACK5 && boss.state != BOSS_DIE)
		{
			if (boss.attackCooldown <= 0) boss.has_hit = 0;
			if (boss.attack2Cooldown > 0.0f) boss.attack2Cooldown -= dt;
			if (boss.attack3Cooldown > 0.0f) boss.attack3Cooldown -= dt;
			if (boss.attack4Cooldown > 0.0f) boss.attack4Cooldown -= dt;
			if (boss.attack5Cooldown > 0.0f) boss.attack5Cooldown -= dt;
			if (touching == 1 && boss.has_hit == 0)
			{
				boss.state = BOSS_ATTACK1;
				HPBARHEALTH -= 30.0f;
				if (HPBARHEALTH < 0) HPBARHEALTH = 0;
			}
			else if (boss.has_hit == 1)
			{
				boss.attackCooldown -= dt;
			}
			else if (VDP_dist > 100 && boss.attack5Cooldown <= 0.0f)
			{
				boss.state = BOSS_ATTACK5;
				Anim_Reset(&boss.attack5);
				boss.spinTimer = 4.0f;
			}
			else if (VDP_dist > 100 && boss.attack2Cooldown <= 0.0f)
			{
				boss.state = BOSS_ATTACK2;
				boss.slash = 0;
				Anim_Reset(&boss.attack2);
			}
			else if (VDP_dist > 200 && boss.attack4Cooldown <= 0.0f && playerinrange)
			{
				boss.state = BOSS_ATTACK4;
			}
			else if (VDP_dist > 250 && VDP_dist <= 700 && boss.attack3Cooldown <= 0.0f)
			{
				boss.state = BOSS_ATTACK3;
				boss.jumpStartX = boss.x;      // 1000
				boss.jumpStartY = boss.y;
				boss.jumpTargetX = knight.x;    // 400
				boss.jumpTargetY = knight.y;
				boss.jumpHit = 0;
				Anim_Reset(&boss.attack3);
			}
			else if (VDP_dist <= 600 && touching != 1 && boss.has_hit == 0)
			{
				boss.state = BOSS_CHASE;
				boss.x += (NVDP_X * boss.speed) * dt;
				boss.y += (NVDP_Y * boss.speed) * dt;
			}
		}

		if (boss.state == BOSS_ATTACK5)
		{
			if (VDP_dist >= 100)
			{
				boss.x += (NVDP_X * boss.speed) * dt;
				boss.y += (NVDP_Y * boss.speed) * dt;
			}
			else if (knight.iFrame == 0)
			{
				knight.iFrame = 1;
				knight.iFrameTimer = 0.5f;
				HPBARHEALTH -= 5.0f;
				if (HPBARHEALTH < 0) HPBARHEALTH = 0;
			}
		}
		if (BeamHitsKnight(&boss, knight.x, knight.y, knight.width, knight.height)
			&& !boss.beam_hit)
		{
			boss.beam_hit = 1;
			HPBARHEALTH -= 10.0f;
			if (HPBARHEALTH < 0) HPBARHEALTH = 0;

		}

		if (boss.state == BOSS_ATTACK3 && boss.attack3.frame == 5 && boss.jumpHit == 0)
		{
			boss.jumpHit = 1;
			if (VDP_dist < 100) {
				HPBARHEALTH -= 10.0f;
				if (HPBARHEALTH < 0) HPBARHEALTH = 0;
			}
		}

		if (boss.slash == 1)
		{
			boss.slash = 2; // means fired
			for (int s = 0; s < MAX_BULLETS; s++)
			{
				if (slashes[s].state == 0)
				{
					slashes[s].x = boss.x;
					slashes[s].y = boss.y;
					slashes[s].speed_x = NVDP_X * 500.0f;
					slashes[s].speed_y = NVDP_Y * 500.0f;
					slashes[s].angle = CP_Math_Degrees(atan2f(NVDP_Y, NVDP_X));
					slashes[s].size = 40.0f;
					slashes[s].state = 1;
					break;
				}
			}
		}

		for (int s = 0; s < MAX_BULLETS; s++)
		{
			if (slashes[s].state != 1) continue;

			if (Paused == 0 && GAMEOVER != 1)
			{
				slashes[s].x += slashes[s].speed_x * dt;
				slashes[s].y += slashes[s].speed_y * dt;
			}

			if (slashes[s].x < -50 || slashes[s].x > 1650 ||
				slashes[s].y < -50 || slashes[s].y > 950)
			{
				slashes[s].state = 0;
				continue;
			}

			if (IsSquareColliding2(slashes[s].x, slashes[s].y, slashes[s].size, slashes[s].size,
				knight.x, knight.y, knight.width, knight.height))
			{
				slashes[s].state = 0;
				if (knight.iFrame == 0 && GAMEOVER != 1)
				{
					HPBARHEALTH -= 10.0f;
					if (HPBARHEALTH < 0) HPBARHEALTH = 0;
					CP_Sound_Play(player_damage);
					knight.iFrame = 1;
					knight.iFrameTimer = 1.0f;
				}
				continue;
			}

			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(CP_Color_Create(80, 160, 255, 220));
			//CP_Graphics_DrawRectAdvanced(slashes[s].x, slashes[s].y, slashes[s].size * 1.5f, 12.0f, slashes[s].angle, 6.0f);
			CP_Image_DrawAdvanced(slashimg, slashes[s].x, slashes[s].y, 128, 128, 255, slashes[s].angle);
		}

		if (GAMEWIN == 1)
		{
			Paused = 1;

			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 180));
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2, windowLength, windowHeight);

			CP_Settings_Fill(white);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Settings_TextSize(100.0f);
			CP_Font_DrawText("YOU WIN!", windowLength / 2.0f, windowHeight / 2.0f);

			CP_Settings_TextSize(30.0f);
			CP_Font_DrawText("PRESS ENTER TO CONTINUE", windowLength / 2.0f, windowHeight / 2.0f + 80);

			if (CP_Input_KeyTriggered(KEY_ENTER))
			{
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
		}
	}
	// ======================================================================================================== //
	// Actuvate use state for current map
	
	for (int i = 0; i < max_obs; i++) {
		//CP_Settings_Fill(red);
		//CP_Graphics_DrawRect(obstacle[i].x, obstacle[i].y, obstacle[i].width, obstacle[i].height);
		obstacle[i].state = 1;
	}
	// Reset unused obstacle state
	for (int i = max_obs; i < MAX_OBSTACLE; i++) {
		obstacle[i].state = 0;
	}
	//---------------------------------------------------------
	//Darrell
	//---------------------------------------------------------
	for (int i = 0; i < MAX_DUMMY; i++)
	{
		if (dummy[i].counter <= 0 && dummy[i].dead == 0)
		{
			if (dummy[i].state != DIE)          // runs once, on the frame it dies
			{
				dummy[i].state = DIE;
				Anim_Reset(&dummy[i].die);
				killcount.amount++;
			}
		}
	}


	//Testing dummy respawn - Not Darrell's code
	for (int i = 0; i < MAX_DUMMY; i++)
	{
		if (GameMode == 2) {
			continue;
		}
		if (killcount.amount >= 10)
		{
			killcount.amount = 10;
			dummy[i].counter = 0;
		}
		else if (timer.timeLeft <= 0.0f)
		{
			dummy[i].counter = 0;
		}
		else
		{
			if (dummy[i].dead == 1 && (GameMode == 0 || GameMode == 1)) {
				dummy[i].dead = 0;
				do {
					dummy[i].x = random_range(100, 1500);
					dummy[i].y = random_range(100, 600);
					// Calculate the Euclidean distance using Pythagoras Theorem
					VDP_dist = sqrtf(euclidean_distance(knight.x, knight.y, dummy[i].x, dummy[i].y));
				} while (VDP_dist <= 250.0f);
				dummy[i].counter = 10;
				dummy[i].deathTimer = 2.0f;
				dummy[i].state = IDLE;
			}
		}
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
	// If Player and Dummy collide, the player loses HP.
	for (int i = 0; i < MAX_DUMMY; i++)
	{
		if (IsSquareColliding2(dummy[i].x, dummy[i].y, dummy[i].width, dummy[i].height, knight.x, knight.y, knight.width, knight.height) && dummy[i].counter != 0 && Paused == 0 && knight.iFrame == 0 && GAMEOVER != 1) {
			dummy[i].state = ATTACK;
			Anim_Reset(&dummy[i].attack);
			// Handle collision
			HPBARHEALTH -= 5.0f;
			hitTimer = 0.15f;
			CP_Sound_Play(player_damage);
			if (HPBARHEALTH < 5.0f && lives == 1) {
				CP_Sound_Play(player_death);
			}
			if (HPBARHEALTH < 0) {
				HPBARHEALTH = 0;
			}
			knight.iFrame = 1;
			knight.iFrameTimer = 1.0f;
			if (ClickedSlot == 1 && CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT)) { // if player and enemy collide, and player uses melee attack
				dummy[i].counter -= 1; // enemy taking damage
				if (dummy[i].counter < 0) {
					dummy[i].counter = 0;
				}
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
	if (current_map->char_moves[0] == exception[0] || current_map->char_moves[0] == exception[1])
	{
		for (int s = 0; s < MAX_BULLETS; s++)
		{
			if (webshots[s].state != 1) continue;

			if (Paused == 0 && GAMEOVER != 1)
			{
				webshots[s].x += webshots[s].speed_x * dt;
				webshots[s].y += webshots[s].speed_y * dt;
			}

			// off screen -> free the slot
			if (webshots[s].x < -50 || webshots[s].x > 1650 ||
				webshots[s].y < -50 || webshots[s].y > 950)
			{
				webshots[s].state = 0;
				continue;
			}

			// hits an obstacle -> disappear but not for birds
			if (current_map->char_moves[0] != exception[1])
			{
				for (int o = 0; o < max_obs; o++)
				{
					if (webshots[s].x <= obstacle[o].x + obstacle[o].width / 2.0f &&
						webshots[s].x >= obstacle[o].x - obstacle[o].width / 2.0f &&
						webshots[s].y <= obstacle[o].y + obstacle[o].height / 2.0f &&
						webshots[s].y >= obstacle[o].y - obstacle[o].height / 2.0f)
					{
						webshots[s].state = 0;
						break;
					}
				}
			}
			if (webshots[s].state == 0) continue;

			// hits the knight -> damage (respects i-frames)
			if (webshots[s].x <= knight.x + knight.width / 2.0f &&
				webshots[s].x >= knight.x - knight.width / 2.0f &&
				webshots[s].y <= knight.y + knight.height / 2.0f &&
				webshots[s].y >= knight.y - knight.height / 2.0f)
			{
				webshots[s].state = 0;
				if (GAMEOVER != 1)
				{
					HPBARHEALTH -= current_map->char_moves[0] == exception[0] ? 5 : 0.5f;
					if (HPBARHEALTH < 0) HPBARHEALTH = 0;
					CP_Sound_Play(player_damage);
				}
				continue;
			}

			if (current_map->char_moves[0] == exception[0]) CP_Image_DrawSubImage(web, webshots[s].x, webshots[s].y,32, 32, 96, 225, 128, 256, 255);
			else CP_Image_DrawSubImage(web, webshots[s].x, webshots[s].y, 64, 64, 32, 225, 64, 256, 255);
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
		CP_Graphics_DrawRect(x, y, boostWidth, boostHeight);

		// Pickup check
		if (IsSquareColliding2(x, y, boostWidth, boostHeight, knight.x, knight.y, knight.width, knight.height))
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
			effects[boostType].stacks++;
			boostCollected = 1;
		}
	}

	
	//Draw the Effect Status Icons + hover tooltip
	int slot = 0;
	int hovered = -1; // -1 means no effect is hovered
	float hoverX = 0.0f, hoverY = 0.0f; // Coordinates of the hovered effect icon

	for (int i = 0; i < BOOST_COUNT; i++) {
		if (effects[i].stacks > 0) {
			float cx = windowLength - 95.0f - (slot * 80.0f);
			float cy = windowHeight - 60.0f;

			CP_Settings_Fill(effects[i].color);
			CP_Graphics_DrawCircle(cx, cy, 70);


			if (euclidean_distance(mouseX, mouseY, cx, cy) <= (boostWidth / 2.0f) * (boostWidth / 2.0f)) {

				hovered = i; // Set the hovered effect index
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
		float boxY = hoverY - 45.0f - boxH;
		if (boxX < 0.0f)
		{
			boxX = 0.0f; //In case it exceeds out
		}

		// Background box
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(grey);
		CP_Graphics_DrawRect(boxX, boxY, boxW, boxH);

		// Text
		char buf[255];
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Settings_Fill(black);

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
	CP_Settings_Fill(red);
	if (HPBARHEALTH != 0)
	{
		for (int i = lives; i != 0; i--)
		{
			CP_Graphics_DrawCircle(15 + (i * 80), windowHeight - 60, 70);
		}
	}
	else
	{
		knight.state = DIE;
		if (knight.dead == 1)
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
				knight.dead = 0;
				knight.state = IDLE;
				Anim_Reset(&knight.die);
			}
		}
	}

	//Draw the pause button in the top left corner of the screen.
	CP_Settings_Fill(white);
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(50, 50, 50, 50);
	CP_Settings_Fill(black);
	CP_Settings_TextSize(30.0f);
	CP_Font_DrawText("| |", 50, 50);

	//Inventory Slot Selection
	if (CP_Input_KeyTriggered(KEY_1)) { ClickedSlot = 1; }
	if (CP_Input_KeyTriggered(KEY_2)) { ClickedSlot = 2; }
	if (CP_Input_KeyTriggered(KEY_3)) { ClickedSlot = 3; }

	//Draw the Inventory (3 boxes)
	for (int i = 1; i <= 3; i++) {
		float x = windowLength / 2.0f + ((i - 2) * 120);
		float y = windowHeight - 65;
		if (i == ClickedSlot) {
			CP_Settings_Fill(yellow); // yellow colour for the selected slot
		}
		else {
			CP_Settings_Fill(orange); // orange colour for the other slots
		}
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2.0f + ((i - 2) * 120), windowHeight - 65, 100, 100);


		CP_Settings_Fill(black); // black text
		CP_Settings_TextSize(20.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(slotNum[i], x - 42.0f, y - 42.0f);
	}
	
	CP_Image_Draw(sword, windowLength / 2.0f + ((-1) * 120), windowHeight - 65.0f, 48.0f, 100.0f, 255); // drawing sword in 1st inventory slot
	CP_Image_Draw(bow, windowLength / 2, windowHeight - 65.0f, 60.0f, 91.0f, 255); // drawing bow in 2nd inventory slot
	//bow cd
	//--------------
	if (ArrowUsed >= 5)
	{
		CP_Settings_Fill(CP_Color_Create(80, 80, 80, 150));
		CP_Graphics_DrawRect(windowLength / 2.0f + ((2 - 2) * 120), windowHeight - 65, 100, 100);
	}
	//--------------
	CP_Image_Draw(potion, windowLength / 2.0f + ((1) * 120), windowHeight - 65.0f, 48.0f, 78.0f, 255); // drawing health potion in 3rd inventory slot
	//potion cd
	//--------------
	if (usedState == 1)
	{
		CP_Settings_Fill(CP_Color_Create(80, 80, 80, 150));
		CP_Graphics_DrawRect(windowLength / 2.0f + ((3 - 2) * 120), windowHeight - 65, 100, 100);
	}
	//--------------
	if (current_map->char_moves[0] != exception[1])
	{
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(bg2, 0, 0, windowLength, windowHeight, 255);
	}
	// level indicators
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	if (current_level == 0) {
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Font_DrawText("LEVEL 1", (windowLength / 2) - 350.0f, windowHeight - 65.0f);
		
	}
	else if (current_level == 1) {
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Font_DrawText("LEVEL 2", (windowLength / 2) - 350.0f, windowHeight - 65.0f);

	}
	else if (current_level == 2) {
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Font_DrawText("LEVEL 3", (windowLength / 2) - 350.0f, windowHeight - 65.0f);

	}

	// when health potion is selected and used
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT) && Paused == 0 && ClickedSlot == 3 && usedState != 1) {
		if (HPBARHEALTH < knight.width) { // if player is not full health -> no effect when player is at full hp
			CP_Sound_Play(hp);
			HPBARHEALTH += 35; // regains 10 hp
			usedState = 1;
			PotionCD = 20.0f;
			if (HPBARHEALTH > knight.width) {
				HPBARHEALTH = knight.width;
			}
		}
	}

	if (usedState == 1)
	{
		PotionCD -= dt;
		if (PotionCD <= 0)
		{
			usedState = 0;
			PotionCD = 0;
		}
	}

	if (GameMode == 0) {
		//Timer Progression Code
		int totalSeconds = (int)(timer.timeLeft + 0.999f);

		snprintf(timer.text, sizeof(timer.text), "%02d:%02d", totalSeconds / 60, totalSeconds % 60);

		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(white); // white box
		CP_Graphics_DrawRect(windowLength / 2.0f, timer.y, timer.width, timer.height);

		if (timer.timeLeft <= 10.0f) {
			CP_Settings_Fill(red); // red in last 10 seconds
		}
		else {
			CP_Settings_Fill(black);   // black text
		}
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(timer.text, windowLength / 2.0f, timer.y);

		if (timer.timeLeft <= 0.0f) {
			CP_Settings_Fill(green);
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(windowLength - 200, killcount.y, 250, killcount.height);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(50.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("CONTINUE", windowLength - 200, killcount.y);
			level_complete = 1;
		}
	}
	else if (GameMode == 1) {
		// Kill-Based Game Mode Code
		snprintf(killcount.text, sizeof(killcount.text), "%02d / %02d", killcount.amount, killcount.total);

		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(white); // white box
		CP_Graphics_DrawRect(windowLength / 2.0f, killcount.y, killcount.width, killcount.height);

		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(killcount.text, windowLength / 2.0f, killcount.y);

		if (killcount.amount == killcount.total) {
			CP_Settings_Fill(green);
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Graphics_DrawRect(windowLength - 200, killcount.y, 250, killcount.height);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(50.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("CONTINUE", windowLength - 200, killcount.y);
			level_complete = 1;
		}
	}

	if (Paused == 1 && GAMEOVER == 0 && GAMEWIN == 0)
	{
		// drawing grey overlay
		CP_Settings_Fill(CP_Color_Create(130, 130, 130, 100));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2, windowLength, windowHeight);

		CP_Settings_Fill(black);
		CP_Settings_TextSize(100.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", windowLength / 2.0f, windowHeight / 2.0f);

		//If menu button is clicked, go back to main menu.
		if (IsAreaClicked(windowLength / 2 + 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			current_level = 0;
			CP_Sound_StopAll();
		}

		//If resume button is clicked, close pause mode and resume.
		if (IsAreaClicked(windowLength / 2 - 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			Paused = 0;
		}

		//Menu Button
		CP_Settings_Fill(white);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 + 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("MENU", windowLength / 2 + 300, windowHeight / 2 + 200);

		//Resume Button
		CP_Settings_Fill(white);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 - 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(black);
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
	if (CP_Input_KeyTriggered(KEY_LEFT)) // level 1
	{
		random_map[current_level] = random_range(0, 2); 
		current_level = 0;
		CP_Sound_StopAll();
		CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_DOWN)) // level 2
	{
		random_map[current_level] = random_range(0, 2);
		current_level = 1;
		CP_Sound_StopAll();
		CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_RIGHT)) // level 3
	{
		random_map[current_level] = random_range(0, 2);
		current_level = 2;
		CP_Sound_StopAll();
		CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}
	if (CP_Input_KeyTriggered(KEY_UP)) // boss level
	{
		current_level = 3;
		random_map[3] = 0;
		CP_Sound_StopAll();
		CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	}

	//if (CP_Input_KeyTriggered(KEY_UP) && current_level < 2) {
	//	current_level++;
	//	random_map[current_level] = random_range(0, 2);
	//	CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	//}
	//if (CP_Input_KeyTriggered(KEY_DOWN) && current_level != 0) {
	//	current_level--;
	//	random_map[current_level] = random_range(0, 2);
	//	CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
	//}

	// continue button to proceed to next level
	if (level_complete == 1)
	{
		if (current_level != 2)
		{
			if (IsAreaClicked(windowLength - 200, killcount.y, 250, killcount.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				current_level++;
				CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
				level_complete = 0;
			}
		}
		else {
			if (IsAreaClicked(windowLength - 200, killcount.y, 250, killcount.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				current_level++;
				GameMode = 2;
				CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
			}
		}
	}

	//if (IsAreaClicked(windowLength - 200, killcount.y, 250, killcount.height, CP_Input_GetMouseX(), CP_Input_GetMouseY()) && current_level == 2) {
	//	current_level++;
	//	GameMode = 2;
	//}

	//if (CP_Input_KeyDown(KEY_Q)) {
	//	current_level = 0;
	//	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	//}


	if (GAMEOVER == 1) { // when player dies
		Paused = 1; // pauses the game so the player has no inputs and no new game updates

		// drawing red overlay
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 100));
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2, windowHeight / 2, windowLength, windowHeight);

		// game over text
		CP_Settings_Fill(black);
		CP_Settings_TextSize(100.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("GAME OVER", windowLength / 2.0f, windowHeight / 2.0f);

		// reassurance over text
		CP_Settings_Fill(black);
		CP_Settings_TextSize(20.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("everybody makes mistakes...", windowLength / 2.0f, windowHeight / 2.0f + 60);

		// replay button
		CP_Settings_Fill(white);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 - 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("REPLAY", windowLength / 2 - 300, windowHeight / 2 + 200);

		// same logic as main menu play button previosuly
		if (IsAreaClicked(windowLength / 2 - 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameStateForced(Game_Init, Game_Update, Game_Exit);
			CP_Sound_StopAll();
		}

		// menu button
		CP_Settings_Fill(white);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(windowLength / 2 + 300, windowHeight / 2 + 200, 230, 100);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("MENU", windowLength / 2 + 300, windowHeight / 2 + 200);

		// same logic as menu button previosuly
		if (IsAreaClicked(windowLength / 2 + 300, windowHeight / 2 + 200, 200, 100, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			CP_Sound_StopAll();
		}
	}

	if (GameMode == 2) { // boss level
		// level indicator
		CP_Settings_Fill(black);
		CP_Settings_TextSize(50.0f);
		CP_Font_DrawText("BOSS LEVEL", (windowLength / 2) - 350.0f, windowHeight - 65.0f);

		// boss health bar
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(darkred);
		CP_Graphics_DrawRect(windowLength / 2 - 500, killcount.y - killcount.height / 2, 1000, killcount.height);
		CP_Settings_Fill(red);
		CP_Graphics_DrawRect(windowLength / 2 - 500, killcount.y - killcount.height / 2, 1000 * (boss.hp / boss.maxHp), killcount.height);
		CP_Settings_RectMode(CP_POSITION_CENTER);
	}
}

void Game_Exit(void)
{
	CP_Sound_StopAll();

	CP_Sound_Free(&bowarrow);
	CP_Sound_Free(&slash);
	CP_Sound_Free(&player_damage);
	CP_Sound_Free(&hp);
	CP_Sound_Free(&igm);
	CP_Sound_Free(&player_death);
	CP_Font_Free(font_menu);
	CP_Image_Free(bg);
	CP_Image_Free(bg2);
	CP_Image_Free(&arrow);
	Anim_Free(&knight.idle);
	Anim_Free(&knight.run);
	Anim_Free(&knight.attack);

	if (GameMode != 2)
	{
		for (int i = 0; i < MAX_DUMMY; i++)
		{
			Anim_Free(&dummy[i].idle);
			Anim_Free(&dummy[i].run);
			Anim_Free(&dummy[i].attack);
		}
	}
	else
	{
		Boss_Free(&boss);
	}
}
