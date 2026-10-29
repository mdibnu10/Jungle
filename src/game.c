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

CP_Image bg;
CP_Image run_sprite;
CP_Image idle_sprite;
CP_Image dead_sprite;
CP_Image first;
CP_Image second;
CP_Image rock;
CP_Sound sound;

CP_Image run;
CP_Image att;
CP_Image idle;
float test_x;
float test_y;
int att_frame;
int attking;


float player_x;
float player_y;
int facing_right;
int run_frame;
int idle_frame;
int was_moving;
int stone;
int waterFrame;
float animation_timer;
float waterTimer;
float slashtimer;

void Game_Init(void)
{
    random = 1;
	bg = CP_Image_Load(maps[random]);
    if (random == 1)
    {
        //sound = CP_Sound_Load("Assets/sound.wav");
        //CP_Sound_Play(sound);
    }
    if (random == 2)
    {
        CP_Settings_ImageMode(CP_POSITION_CENTER);
    }
    run_sprite = CP_Image_Load("Assets/Run-Sheet.png");
    idle_sprite = CP_Image_Load("Assets/Idle-Sheet.png");
    dead_sprite = CP_Image_Load("Assets/Death-Sheet.png");
    rock = CP_Image_Load("Assets/Rocks.png");

    run = CP_Image_Load("Assets/_Run.png");
    att = CP_Image_Load("Assets/_AttackCombo.png");
    idle = CP_Image_Load("Assets/_Idle.png");

    player_x = 800.0f;
    player_y = 300.0f;
    was_moving = 0;
    idle_frame = 0;
    run_frame = 0;
    waterTimer = 0;
    waterFrame = 0;
    slashtimer = 0;
    

    att_frame = 0;
    test_x = 800.0f;
    test_y = 450.0f;

}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));

    // if second map is chosen execute this
    if (random == 1)
    {
        CP_Settings_ImageMode(CP_POSITION_CORNER);
        CP_Image_Draw(bg, 0.0f, 0.0f, 1600, 900, 255);
        CP_Settings_ImageMode(CP_POSITION_CENTER);
        int moving = 0;
 

        slashtimer += CP_System_GetDt();
        animation_timer += CP_System_GetDt();
        if (animation_timer >= 0.12f)
        {
            run_frame = (run_frame + 1) % 10;
            
            idle_frame = (idle_frame + 1) % 10;
            animation_timer = 0;
        }
        if (CP_Input_KeyDown(KEY_W))
        {
            test_y -= 3.0f;
            moving = 1;
        }

        if (CP_Input_KeyDown(KEY_S))
        {
            test_y += 3.0f;
            moving = 1;
        }

        if (CP_Input_KeyDown(KEY_A))
        {
            test_x -= 3.0f;
            moving = 1;
            facing_right = 0;

        }

        if (CP_Input_KeyDown(KEY_D))
        {
            test_x += 3.0f;
            moving = 1;
            facing_right = 1;

        }
        if (CP_Input_MouseClicked())
        {
            attking = 1;     
        }
        if (attking == 1)
        {
            float draw_width = facing_right ? 160.0f : -160.0f;
            CP_Image_DrawSubImage(
                att,
                test_x, test_y,
                draw_width, 120,
                att_frame * 120, 0,
                (att_frame + 1) * 120, 80,
                255);
            if (slashtimer >= 0.10f)
            {
                att_frame = (att_frame + 1) % 10;
                slashtimer = 0;
                if (att_frame == 9)
                    attking = 0;
            }
            
        }  
        else
        {
            if (moving == 1)
            {
                float draw_width = facing_right ? 160.0f : -160.0f;
                CP_Image_DrawSubImage(
                    run,
                    test_x, test_y,
                    draw_width, 120,
                    run_frame * 120, 0,
                    (run_frame + 1) * 120, 80,
                    255
                );
            }
            else
            {
                float draw_width = facing_right ? 160.0f : -160.0f;
                CP_Image_DrawSubImage(
                    idle,
                    test_x, test_y,
                    draw_width, 120,
                    idle_frame * 120, 0,
                    (idle_frame + 1) * 120, 80,
                    255
                );
            }
        }
    }

    // if third map is chosen execute this
    else if (random == 2)
    {
        CP_Image_Draw(
            bg,
            800.0f, 450.0f, // OG 363.45f
            726.9f, 900.0f,
            255
        );

        int moving = 0;

        if ((player_x > 305.0f && player_x < 385.0f) && (player_y > 524.83f && player_y < 620.9f))
            stone = 1;
        else
            stone = 0;


        if (CP_Input_KeyDown(KEY_W))
        {
            player_y -= 3.0f;
            moving = 1;
        }

        if (CP_Input_KeyDown(KEY_S))
        {
            player_y += 3.0f;
            moving = 1;
        }

        if (CP_Input_KeyDown(KEY_A))
        {
            player_x -= 3.0f;
            facing_right = 0;
            moving = 1;
        }

        if (CP_Input_KeyDown(KEY_D))
        {
            player_x += 3.0f;
            facing_right = 1;
            moving = 1;
        }



        if (moving != was_moving)
        {
            animation_timer = 0.0f;

            if (moving)
                run_frame = 0;
            else
                idle_frame = 0;
        }

        was_moving = moving;
        animation_timer += CP_System_GetDt();



        if (moving)
        {
            if (animation_timer >= 0.12f)
            {
                run_frame = (run_frame + 1) % 6;
                animation_timer = 0.0f;
            }

            float draw_width = facing_right ? 85.0f : -85.0f;

            CP_Image_DrawSubImage(
                run_sprite,
                player_x, player_y,
                draw_width, 85,
                run_frame * 64, 0,
                (run_frame + 1) * 64, 64,
                255
            );
        }
        else
        {
            if (animation_timer >= 0.20f)
            {
                idle_frame = (idle_frame + 1) % 4;
                animation_timer = 0.0f;
            }

            if (CP_Input_KeyDown(KEY_O))
            {
                CP_Image_DrawSubImage(
                    dead_sprite,
                    player_x, player_y,
                    128, 128,
                    idle_frame * 64, 0,
                    (idle_frame + 1) * 64, 64,
                    255
                );
            }

            float draw_width = facing_right ? 45.0f : -45.0f;

            CP_Image_DrawSubImage(
                idle_sprite,
                player_x, player_y + 20,
                draw_width, 45,
                idle_frame * 32, 0,
                (idle_frame + 1) * 32, 32,
                255
            );
        }
        CP_Image_DrawSubImage(
            rock,
            782.42f, 570.83f,
            69.12f, 103.68f,
            0, 16,
            32, 64,
            255
        );
    }

    else 
	{
		CP_Image_Draw(bg, 0.0f, 0.0f, 1600, 900, 255);
	}
	
}
void Game_Exit(void)
{
    
    CP_Image_Free(bg);
    CP_Image_Free(rock);
    CP_Image_Free(dead_sprite);
    CP_Image_Free(run_sprite);


}