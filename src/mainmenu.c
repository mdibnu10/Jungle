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



void Main_Menu_Init(void)
{
	CP_Font globalfont;
	globalfont = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Settings_TextSize(67);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));

}

void Main_Menu_Update(void)
{
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); // setting font color
	CP_Font_DrawText("BOPLEHHHHHHHHHHHHHHH", 800, 400); // play text

}

void Main_Menu_Exit(void)
{
   
}