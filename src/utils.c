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

//<<<<<<< HEAD
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

void Anim_DrawBeam(const Animation* a, float x, float y, float w, float h, float angle)
{
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Image_DrawSubImageAdvanced(a->img, x, y, w, h,
	(float)(a->frame * a->img_w), 0,
	(float)((a->frame + 1) * a->img_w), (float)a->img_h, 255, angle);
	// print the first few frames of beam in small size but the rest is long		
}
//=======
float findQ(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
//>>>>>>> main
}
//---------------------------------------------------------
//Darrel
//---------------------------------------------------------
//int IsSquareCircleColliding(float square_center_x, float square_center_y, float square_width, float square_height, float circle_center_x, float circle_center_y, float circle_radius)
//{
//	if (
//		fabs(circle_center_x - square_center_x) <= (square_width / 2 + circle_radius) &&
//		fabs(circle_center_y - square_center_y) <= (square_height / 2 + circle_radius)
//		)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}

int IsSquareColliding(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h, float *overlapX, float *overlapY) {
	// Setting up boundary for Obstacle
	float ObsminX = sq1x - sq1w / 2.0f;
	float ObsmaxX = sq1x + sq1w / 2.0f;
	float ObsminY = sq1y - sq1h / 2.0f;
	float ObsmaxY = sq1y + sq1h / 2.0f;
	// Setting up boundary for dummy
	float DummmyminX = sq2x - sq2w / 2.0f;
	float DummymaxX = sq2x + sq2w / 2.0f;
	float DummyminY = sq2y - sq2h / 2.0f;
	float DummymaxY = sq2y + sq2h / 2.0f;
	// If knight currently outside boundary of Obstacle, continue with the rest of the code
	if (DummymaxX <= ObsminX || DummmyminX >= ObsmaxX ||
		DummymaxY <= ObsminY || DummyminY >= ObsmaxY) {
		return 0;
	}
	// Calculate the XY Overlap
	*overlapX = (sq2x < sq1x) ? (DummymaxX - ObsminX) : (ObsmaxX - DummmyminX);
	*overlapY = (sq2y < sq1y) ? (DummymaxY - ObsminY) : (ObsmaxY - DummyminY);
	
	return 1;
}

int IsSquareColliding2(float sq1x, float sq1y, float sq1w, float sq1h, float sq2x, float sq2y, float sq2w, float sq2h) {
	// Setting up boundary for Obstacle
	float ObsminX = sq1x - sq1w / 2.0f;
	float ObsmaxX = sq1x + sq1w / 2.0f;
	float ObsminY = sq1y - sq1h / 2.0f;
	float ObsmaxY = sq1y + sq1h / 2.0f;
	// Setting up boundary for dummy
	float DummmyminX = sq2x - sq2w / 2.0f;
	float DummymaxX = sq2x + sq2w / 2.0f;
	float DummyminY = sq2y - sq2h / 2.0f;
	float DummymaxY = sq2y + sq2h / 2.0f;
	// If knight currently outside boundary of Obstacle, continue with the rest of the code
	if (DummymaxX <= ObsminX || DummmyminX >= ObsmaxX ||
		DummymaxY <= ObsminY || DummyminY >= ObsmaxY) {
		return 0;
	}
	return 1;
}

int euclidean_distance(int x1, int y1, int x2, int y2) {
	float VDP_X = x1 - x2;
	float VDP_Y = y1 - y2;
	// Calculate the Euclidean distance using Pythagoras Theorem
	return (VDP_X * VDP_X) + (VDP_Y * VDP_Y);
}

unsigned int random_range(unsigned int min, unsigned int max)
{
	return min + rand() % (max - min + 1);
}


void SpawnBoost(BoostType* type, float* x, float* y, int* collected, float* timer, float windowW, float windowH)
{
	*type = random_range(0, BOOST_COUNT - 1);
	*x = random_range(100, (unsigned int)(windowW - 100));
	*y = random_range(130, (unsigned int)(windowH - 180));
	*collected = 0;
	*timer = 25.0f;
}

void Anim_Free(Animation* anim)
{
	CP_Image_Free(&anim->img);
}

void Boss_Init(Boss* b)
{
	b->x = 450.0f;  b->y = 100.0f;
	b->width = 150.0f;  b->height = 120.0f;
	b->draw_w = 180.0f; b->draw_h = 180.0f;
	b->speed = 180.0f;
	b->maxHp = 100.0f;
	b->hp = b->maxHp;
	b->hold_frame = 3.0f;
	b->has_hit = 0;
	b->beam_hit = 0;
	b->is_right = 1;
	b->spinTimer = 4.0f;
	b->state = BOSS_IDLE;
	b->stateTimer = 1.0f;
	b->attackCooldown = 2.0f;
	b->attack2Cooldown = 4.0f;
	b->attack3Cooldown = 0.0f;
	b->attack4Cooldown = 5.0f;
	b->attack5Cooldown = 5.0f;
	b->beamdir = CP_Image_Load("Assets/Characters/Boss/finaldirections.png");
	Anim_Init(&b->idle, "Assets/Characters/Boss/Boss_Idle.png", 180, 180, 0.12f, 1);
	Anim_Init(&b->run, "Assets/Characters/Boss/Boss_Run.png", 180, 180, 0.12f, 1);
	Anim_Init(&b->attack, "Assets/Characters/Boss/Boss_Attack.png", 180, 180, 0.10f, 0);
	Anim_Init(&b->attack2, "Assets/Characters/Boss/Boss_Attack2.png", 180, 180, 0.10f, 0);
	Anim_Init(&b->attack3, "Assets/Characters/Boss/Boss_Attack3.png", 180, 180, 0.10f, 0);
	Anim_Init(&b->attack4, "Assets/Characters/Boss/Boss_Attack4.png", 180, 180, 0.10f, 0);
	Anim_Init(&b->attack5, "Assets/Characters/Boss/Boss_Attack5.png", 180, 180, 0.05f, 1);
	Anim_Init(&b->beam, "Assets/Characters/Boss/Boss_Beam.png", 96, 96, 0.15f, 0);
}

void Boss_Update(Boss* b, float dt, float knightX, float knightY)
{
	if (b->state == BOSS_DEAD) return;

	b->is_right = knightX > b->x;

	switch (b->state) {
	case BOSS_IDLE:
		Anim_Update(&b->idle, dt);	
		break;   // later: pick next state
	case BOSS_CHASE:
		Anim_Update(&b->run, dt);
		break;   // later: move toward knight
	case BOSS_ATTACK1:
		if (Anim_Update(&b->attack, dt))
		{
			Anim_Reset(&b->attack);
			b->state = BOSS_IDLE;
			b->attackCooldown = 2.0f;
			b->has_hit = 1;
		}
		break;
	case BOSS_ATTACK2:
		if (b->slash == 0 && b->attack2.frame >= 3)
		b->slash = 1;
		if(Anim_Update(&b->attack2, dt))
		{
		Anim_Reset(&b->attack2);
		b->state = BOSS_IDLE;
		b->attack2Cooldown = 4.0f;
		}
		break;
	case BOSS_ATTACK3:
	{
		int f = b->attack3.frame;

		if (f >= F_AIRFRAME && f <= S_AIRFRAME)
		{
			// progress 0-1 across the mid-air frames
			float t = ((f - F_AIRFRAME) + b->attack3.timer / b->attack3.frame_time) / 2;
			b->x = CP_Math_LerpFloat(b->jumpStartX, b->jumpTargetX, t);
			b->y = CP_Math_LerpFloat(b->jumpStartY, b->jumpTargetY, t);
		}
		else if (f > S_AIRFRAME)
		{
			b->x = b->jumpTargetX;
			b->y = b->jumpTargetY;
		}
		if (Anim_Update(&b->attack3, dt))
		{
			Anim_Reset(&b->attack3);
			b->state = BOSS_IDLE;
			b->attack3Cooldown = 6.0f;
		}
		break;
	}
	case BOSS_ATTACK4:
		Anim_Update(&b->attack4, dt);
		b->hold_frame -= 0;
		if (Anim_Update(&b->beam, dt))
		{
			b->state = IDLE;
			b->attack4Cooldown = 5.0f;
			b->hold_frame = 3.0f;
			Anim_Reset(&b->attack4);
			Anim_Reset(&b->beam);
			b->beam_hit = 0;
			
		}
		break;
	case BOSS_ATTACK5:
		Anim_Update(&b->attack5, dt);
		b->spinTimer -= dt;
		if (b->spinTimer <= 0)
		{
			b->attack5Cooldown = 5.0f;
			b->state = BOSS_CHASE;
		}
		break;
	default: break;
	}
}

void Boss_Draw(Boss* b)
{
	if (b->state == BOSS_DEAD) return;
	float w = b->is_right ? b->draw_w : -b->draw_w;
	switch (b->state) {
	case BOSS_IDLE:
		Anim_Draw(&b->idle, b->x, b->y, w, b->draw_h);
		break; 
	case BOSS_CHASE:
		Anim_Draw(&b->run, b->x, b->y, w, b->draw_h);
		break; 
	case BOSS_ATTACK1: 	
		Anim_Draw(&b->attack, b->x, b->y, w, b->draw_h);
		break;
	case BOSS_ATTACK2:
		Anim_Draw(&b->attack2, b->x, b->y, w, b->draw_h);
		break;
	case BOSS_ATTACK3:
		Anim_Draw(&b->attack3, b->x, b->y, w, b->draw_h);
		break;
	case BOSS_ATTACK4:
		// do not print the last frame first 
		if (b->attack4.frame != 7)
		{
			Anim_Draw(&b->attack4, b->x, b->y, w, b->draw_h);
		}
		else if (b->facingdir == LEFT || b->facingdir == RIGHT)
		{
			if (b->hold_frame > 0) Anim_Draw(&b->attack4, b->x, b->y, w, b->draw_h);
		}
		else
		{
			if (b->hold_frame > 0)
			{
				
				CP_Image_DrawSubImage(b->beamdir, b->x, b->y, 360, 360,
					b->facingdir * 180, 0,
					(b->facingdir + 1) * 180, 180, 255);
			}
		}
		
		// start firing only when in position
		if (b->attack4.frame > 4)
		{
			// print the first few frames of beam in small size but the rest is long
			if ((b->beam.frame) < 4) Anim_DrawBeam(&b->beam, b->ball_x, b->ball_y, 180, 180, b->beam_angle);
			else Anim_DrawBeam(&b->beam, b->beam_x, b->beam_y, 960, 180, b->beam_angle);
		}
		break;
	case BOSS_ATTACK5:
		Anim_Draw(&b->attack5, b->x, b->y, 360, 360);
		break;

	default: break;
	}
}

void change_dir(Boss* b, float angle)
{
	if (angle >= 340 || angle <= 20) b->facingdir = RIGHT;
	else if (angle >= 20 && angle <= 70) b->facingdir = FOUR_QUARTER;
	else if (angle >= 70 && angle <= 110) b->facingdir = DOWN;
	else if (angle >= 110 && angle <= 160) b->facingdir = THIRD_QUARTER;
	else if (angle >= 160 && angle <= 200) b->facingdir = LEFT;
	else if (angle >= 200 && angle <= 250) b->facingdir = SEC_QUARTER;
	else if (angle >= 250 && angle <= 290) b->facingdir = UP;
	else b->facingdir = FIRST_QUARTER;

	switch (b->facingdir)
	{
	case UP:
		b->beam_angle = 270;
		b->ball_x = b->x;
		b->beam_x = b->x;
		b->ball_y = b->y - 150;
		b->beam_y = b->y - 550;
		break;
	case DOWN:
		b->beam_angle = 90;
		b->ball_x = b->x;
		b->beam_x = b->x;
		b->ball_y = b->y + 150;
		b->beam_y = b->y + 550;
		break;
	case LEFT:
		b->beam_angle = 180;
		b->ball_x = b->x - 200;
		b->beam_x = b->x - 600;
		b->ball_y = b->y;
		b->beam_y = b->y;
		break;
	case RIGHT:
		b->beam_angle = 0;
		b->ball_x = b->x + 200;
		b->beam_x = b->x + 600;
		b->ball_y = b->y;
		b->beam_y = b->y;
		break;
	case FIRST_QUARTER:
		b->beam_angle = 315;
		b->ball_x = b->x + 130;
		b->beam_x = b->x + 400;
		b->ball_y = b->y - 120;
		b->beam_y = b->y - 420;
		break;
	case SEC_QUARTER:
		b->beam_angle = 225;
		b->ball_x = b->x - 130;
		b->beam_x = b->x - 400;
		b->ball_y = b->y - 120;
		b->beam_y = b->y - 420;
		break;
	case THIRD_QUARTER:
		b->beam_angle = 135;
		b->ball_x = b->x - 130;
		b->beam_x = b->x - 400;
		b->ball_y = b->y + 120;
		b->beam_y = b->y + 420;
		break;
	case FOUR_QUARTER:
		b->beam_angle = 45;
		b->ball_x = b->x + 130;
		b->beam_x = b->x + 400;
		b->ball_y = b->y + 120;
		b->beam_y = b->y + 420;
		break;
	}
}
int BeamHitsKnight(const Boss* boss, float kx, float ky,
	float kw, float kh)
{
	// only the long, dangerous part of the beam can hurt
	if (boss->state != BOSS_ATTACK4 || boss->beam.frame < 4) return 0;

	float beam_length = 750.0f;
	float beam_thickness = 80.0f;   //

	// convert degrees to radians
	float angle_radians = boss->beam_angle * PI / 180.0f;
	float cos_angle = cosf(angle_radians);
	float sin_angle = sinf(angle_radians);

	// how far the knight is from the beam's centre, in normal screen space
	float offset_x = kx - boss->beam_x;
	float offset_y = ky - boss->beam_y;

	// rotate the knight's position backwards by the beam's angle,
	// so the beam becomes flat and the knight moves to match
	float flat_knight_x = offset_x * cos_angle + offset_y * sin_angle;
	float flat_knight_y = -offset_x * sin_angle + offset_y * cos_angle;

	// the flat beam now sits at (0, 0), so a normal AABB check works
	return IsSquareColliding2(0, 0, beam_length, beam_thickness,
		flat_knight_x, flat_knight_y,
		kw, kh);
}
void Boss_TakeDamage(Boss* b, float dmg)
{
	if (b->state == BOSS_DEAD) return;
	b->hp -= dmg;
	if (b->hp <= 0.0f) { b->hp = 0.0f; b->state = BOSS_DEAD; }
}

void Boss_Free(Boss* b)
{
	Anim_Free(&b->attack);
}