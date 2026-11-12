//---------------------------------------------------------
// file:	mainmenu.c
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

CP_Font globalfont;
CP_Image bg;

// first map level 1
float mapA1_x[] = { 976.03125f, 736.1818f, 1343.875f, 1311.8182f, 271.875f, 432.2727f, 173.5f, 335.1818f, 655.9545f, 512.2727f, 1007.96875f, 813.0f, 1183.1818f, 190.2273f };
float mapA1_y[] = { 135.375f, 224.3636f, 224.3125f, 687.2727f, 775.9375f, 135.5f, 448.3182f, 608.1364f, 392.0f, 567.6818f, 599.3125f, 720.0909f, 432.1818f, 256.0909f };
float mapA1_w[] = { 32.1875f, 95.6364f, 96.25f, 96.3636f, 32.5f, 32.7273f, 59.1818f, 62.9091f, 32.4545f, 32.9091f, 32.6875f, 58.5455f, 95.8182f, 60.6364f };
float mapA1_h[] = { 15.0f, 32.3636f, 32.625f, 34.1818f, 15.625f, 15.5455f, 32.8182f, 31.9091f, 16.3636f, 15.5455f, 14.875f, 32.3636f, 32.7273f, 32.3636f };
// second map level 2
float mapA2_x[] = { 880.00f, 1071.9375f, 1327.9375f, 1246.3182f, 1232.0455f, 1008.3636f, 864.0625f, 591.9783f, 701.6648f, 352.1818f, 240.0625f, 464.4545f, 287.75f, 607.9091f, 848.00f };
float mapA2_y[] = { 104.00f, 288.0625f, 223.875f, 432.1364f, 616.1364f, 720.2727f, 439.875f, 343.9565f, 271.9375f, 384.0909f, 232.125f, 136.1818f, 640.25f, 704.2727f, 352.3636f };
float mapA2_w[] = { 31.75f, 63.875f, 64.125f, 60.4545f, 32.2727f, 96.3636f, 32.125f, 31.9565f, 59.8295f, 64.7273f, 32.375f, 32.3636f, 96.5f, 65.2727f, 64.00f };
float mapA2_h[] = {15.75f, 32.625f, 32.00f, 32.4545f, 16.0909f, 32.9091f, 15.875f, 16.2609f, 32.625f, 32.1818f, 16.75f, 16.5455f, 31.5f, 32.9091f, 32.00f};
// second map level 3
float mapA3_x[] = { 1216.13f, 1072.00f, 351.88f, 688.33f, 1488, 672.06f, 272.13f, 352.00f, 895.82f, 687.95f, 1184.00f, 1215.75f };
float mapA3_y[] = { 144.00f, 248.19f, 464.00f, 208, 232.25f, 464.06f, 784.13f, 231.91f, 408.36f, 600.00f, 600.14f, 784.00f };
float mapA3_w[] = { 64.25f, 32.75, 64.25f, 64.67f, 32.5f, 64.375f, 64.75f, 32.36f, 32, 32.09f, 32.36f, 64.5f };
float mapA3_h[] = { 32.5f, 16.125f, 32, 32.67f, 16.5f, 32.125f, 32.25f, 15.82f, 16, 16, 16.09f, 32.5f };

MapData maps[LEVELS][MAPS_PER_LEVEL] =
{
	// Level 1: real maps
	{ {"./Assets/A.png", "./Assets/obs1.png", mapA1_x, mapA1_y, mapA1_w, mapA1_h, 14}, {"./Assets/B.png", "./Assets/obs2.png", mapA2_x, mapA2_y, mapA2_w, mapA2_h, 15},
	{"./Assets/C.png", "./Assets/obs3.png", mapA3_x, mapA3_y, mapA3_w, mapA3_h, 12}},

	// Level 2: reuse lvl1 images for now as placeholders
	{ {"./Assets/A.png", "./Assets/obs1.png", mapA1_x, mapA1_y, mapA1_w, mapA1_h, 14}, {"./Assets/B.png", "./Assets/obs2.png", mapA2_x, mapA2_y, mapA2_w, mapA2_h, 15},
	{"./Assets/C.png", "./Assets/obs3.png", mapA3_x, mapA3_y, mapA3_w, mapA3_h, 12}},

	// Level 3: same placeholders
	{ {"./Assets/A.png"}, {"./Assets/B.png"}, {"./Assets/C.png"} },
};
int random_map[LEVELS];
int current_level = 0;

// randomize map for each level
void randomize(void)
{
	for (int i = 0; i < LEVELS; i++)
		random_map[i] = rand() % MAPS_PER_LEVEL;
}

void Main_Menu_Init(void)
{
	globalfont = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
	bg = CP_Image_Load("./Assets/newbg.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	CP_System_SetWindowSize(CP_Image_GetWidth(bg), CP_Image_GetHeight(bg));
}

void Main_Menu_Update(void)
{
	float click_x = CP_Input_GetMouseX();
	float click_y = CP_Input_GetMouseY();

	float area_center_x = 800.0f;
	float play_height = 505.0f;
	float cred_height = 648.0;
	float exit_height = 790.0f;

	float area_width = 230.0f;
	float area_height = 100.0f;

	CP_Image_Draw(bg, 0.0f, 0.0f, 1600, 900, 255);

	CP_Settings_Fill(CP_Color_Create(255, 127, 127, 0)); // setting rectangle color
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(area_center_x, play_height, area_width, area_height); // drawing play rectangle
	CP_Graphics_DrawRect(area_center_x, cred_height, area_width, area_height); // drawing credits rectangle
	CP_Graphics_DrawRect(area_center_x, exit_height, area_width, area_height); // drawing exit rectangle

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); // setting font color
	CP_Settings_TextSize(70);
	CP_Font_DrawText("MOTHER NATURE", area_center_x, 240); // title
	CP_Settings_TextSize(50);
	CP_Font_DrawText("PLAY", area_center_x, play_height); // play text
	CP_Font_DrawText("CREDITS", area_center_x, cred_height); // credits text
	CP_Font_DrawText("EXIT", area_center_x, exit_height); // exit text

	if (IsAreaClicked(area_center_x, play_height, area_width, area_height, click_x, click_y)) { // if play is clicked
		randomize();
		CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
	}

	// add if statement for credits here (might need make new source file for the credits)

	if (IsAreaClicked(area_center_x, exit_height, area_width, area_height, click_x, click_y)) { // if exit is clicked
		CP_Engine_Terminate();
	}
}

void Main_Menu_Exit(void)
{
	CP_Font_Free(&globalfont);
}