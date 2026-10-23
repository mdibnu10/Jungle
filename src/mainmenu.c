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

void Main_Menu_Init(void)
{
	globalfont = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
	bg = CP_Image_Load("./Assets/bg.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	CP_System_SetWindowSize(CP_Image_GetWidth(bg), CP_Image_GetHeight(bg));
}

void Main_Menu_Update(void)
{
	float click_x = CP_Input_GetMouseX();
	float click_y = CP_Input_GetMouseY();

	float area_center_x = 804.25f;
	float play_height = 408.0f;
	float cred_height = 575.0f;
	float exit_height = 745.0f;

	float area_width = 378.0f;
	float area_height = 143.0f;

	CP_Image_Draw(bg, 0.0f, 0.0f, 1600, 900, 255);

	CP_Settings_Fill(CP_Color_Create(255, 127, 127, 0)); // setting rectangle color
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(area_center_x, play_height, area_width, area_height); // drawing play rectangle
	CP_Graphics_DrawRect(area_center_x, cred_height, area_width, area_height); // drawing credits rectangle
	CP_Graphics_DrawRect(area_center_x, exit_height, area_width, area_height); // drawing exit rectangle

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); // setting font color
	CP_Settings_TextSize(70);
	CP_Font_DrawText("JUNGLE GAME", area_center_x, 175); // title
	CP_Settings_TextSize(50);
	CP_Font_DrawText("PLAY", area_center_x, play_height); // play text
	CP_Font_DrawText("CREDITS", area_center_x, cred_height); // credits text
	CP_Font_DrawText("EXIT", area_center_x, exit_height); // exit text

	if (IsAreaClicked(area_center_x, play_height, area_width, area_height, click_x, click_y)) { // if play is clicked
		CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
	}

	// add if statement for credits here (might need make new source file for the credits)

	if (IsAreaClicked(area_center_x, exit_height, area_width, area_height, click_x, click_y)) { // if exit is clicked
		CP_Engine_Terminate();
	}
}

void Main_Menu_Exit(void)
{
   
}