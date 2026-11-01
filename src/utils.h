//---------------------------------------------------------
// file:	utils.h
// author:	
// email:	
//
// brief:	
//		
//
// Copyright © 2026 DigiPen, All rights reserved.
//---------------------------------------------------------


int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);


typedef struct Animation
{
	CP_Image img;
	int   img_w, img_h;
	int   frame_count;
	float frame_time;   // seconds per frame
	int   frame;
	float timer;
	int   loop;         // 1 = loops, 0 = plays once
} Animation;

typedef enum { IDLE, RUN, ATTACK } PlayerState;

typedef struct Player
{
	float x, y;
	float speed;
	float size;
	int is_right;
	PlayerState state;
	Animation idle, run, attack;
} Player;

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int counter;
	int chase;
	int is_right;
	PlayerState state;
	Animation idle, run, attack;
} Enemy;

void Anim_Init(Animation* a, const char* path, int fw, int fh, float frame_time, int loop);
void Anim_Reset(Animation* a);
int  Anim_Update(Animation* a, float dt);
void Anim_Draw(const Animation* a, float x, float y, float w, float h);
//=======
float findQ(float value, float min, float max);

//>>>>>>> main
