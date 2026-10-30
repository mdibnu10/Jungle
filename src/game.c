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


Player knight;
Player archer;
CP_Image bg;

int is_right;

void Game_Init(void)
{

    knight.x = 800.0f;
    knight.y = 450.0f;
    knight.speed = 3.0f;
    knight.state = IDLE;


    archer.x = 1000.0f;
    archer.y = 450.0f;
    archer.speed = 3.0f;
    archer.state = IDLE;

    bg = CP_Image_Load(maps[current_level][random[current_level]].image_path);
    Anim_Init(&knight.idle, "Assets/Knight_Idle.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.run, "Assets/Knight_Run.png", 120, 80, 0.12f, 1);
    Anim_Init(&knight.attack, "Assets/Knight_Attack.png", 120, 80, 0.10f, 0);
    Anim_Init(&archer.idle, "Assets/Archer_Idle&Run.png", 64, 64, 0.12f, 1);
    Anim_Init(&archer.run, "Assets/Archer_Idle&Run.png.png", 64, 64, 0.12f, 1);
    Anim_Init(&archer.attack, "Assets/Archer_Attack.png", 64, 64, 0.10f, 1);
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
        is_right = 0;
        moving = 1;
    }
    if 
        (CP_Input_KeyDown(KEY_D))
    { 
        knight.x += knight.speed;
        archer.x += archer.speed;
        is_right = 1;
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
    float draw_width = is_right == 1 ? 160 : -160;
    
   
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
        Anim_Draw(&knight.attack, knight.x, knight.y, draw_width, 120);
        break;
    }

    switch (archer.state)
    {
    case IDLE:
  
        Anim_Update(&archer.idle, dt);
    
        Anim_Draw(&archer.idle, archer.x, archer.y, draw_width, 120);
        break;
    case RUN:
      
        Anim_Update(&archer.run, dt);

        Anim_Draw(&archer.run, archer.x, archer.y, draw_width, 120);
        break;
    case ATTACK:
        if (Anim_Update(&archer.attack, dt))
        {

            archer.state = IDLE;
        }
        Anim_Draw(&archer.attack, archer.x, archer.y, draw_width, 120);
        break;
    }
	
}


void Game_Exit(void)
{


}