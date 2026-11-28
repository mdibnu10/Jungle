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
static CP_Image bg, digipen, team, credit;
CP_Sound music;

static float timer = 0.0f;
int isShow = 1;

int isquit, iscredit;

// first map level 1
float mapA1_x[] = { 976.03125f, 736.1818f, 1343.875f, 1311.8182f, 271.875f, 432.2727f, 173.5f, 335.1818f, 655.9545f, 512.2727f, 1007.96875f, 813.0f, 1183.1818f, 190.2273f };
float mapA1_y[] = { 135.375f, 224.3636f, 224.3125f, 687.2727f, 775.9375f, 135.5f, 448.3182f, 608.1364f, 392.0f, 567.6818f, 599.3125f, 720.0909f, 432.1818f, 256.0909f };
float mapA1_w[] = { 32.1875f, 95.6364f, 96.25f, 96.3636f, 32.5f, 32.7273f, 59.1818f, 62.9091f, 32.4545f, 32.9091f, 32.6875f, 58.5455f, 95.8182f, 60.6364f };
float mapA1_h[] = { 15.0f, 32.3636f, 32.625f, 34.1818f, 15.625f, 15.5455f, 32.8182f, 31.9091f, 16.3636f, 15.5455f, 14.875f, 32.3636f, 32.7273f, 32.3636f };
// second map level 1
float mapB1_x[] = { 880.00f, 1071.9375f, 1327.9375f, 1246.3182f, 1232.0455f, 1008.3636f, 864.0625f, 591.9783f, 701.6648f, 352.1818f, 240.0625f, 464.4545f, 287.75f, 607.9091f, 848.00f };
float mapB1_y[] = { 104.00f, 288.0625f, 223.875f, 432.1364f, 616.1364f, 720.2727f, 439.875f, 343.9565f, 271.9375f, 384.0909f, 232.125f, 136.1818f, 640.25f, 704.2727f, 352.3636f };
float mapB1_w[] = { 31.75f, 63.875f, 64.125f, 60.4545f, 32.2727f, 96.3636f, 32.125f, 31.9565f, 59.8295f, 64.7273f, 32.375f, 32.3636f, 96.5f, 65.2727f, 64.00f };
float mapB1_h[] = { 15.75f, 32.625f, 32.00f, 32.4545f, 16.0909f, 32.9091f, 15.875f, 16.2609f, 32.625f, 32.1818f, 16.75f, 16.5455f, 31.5f, 32.9091f, 32.00f};
// third map level 1
float mapC1_x[] = { 1216.13f, 1072.00f, 351.88f, 688.33f, 1488, 672.06f, 272.13f, 352.00f, 895.82f, 687.95f, 1184.00f, 1215.75f };
float mapC1_y[] = { 144.00f, 248.19f, 464.00f, 208, 232.25f, 464.06f, 784.13f, 231.91f, 408.36f, 600.00f, 600.14f, 784.00f };
float mapC1_w[] = { 64.25f, 32.75, 64.25f, 64.67f, 32.5f, 64.375f, 64.75f, 32.36f, 32, 32.09f, 32.36f, 64.5f };
float mapC1_h[] = { 32.5f, 16.125f, 32, 32.67f, 16.5f, 32.125f, 32.25f, 15.82f, 16, 16, 16.09f, 32.5f };
// first map level 2
float mapA2_x[] = { 176.25f, 536.36f, 352.25f, 1007.88f, 1007.83f, 1247.50f, 1207.83f, 1311.83f, 1391.67f, 656.17f, 373.00f, 752.09f, 847.67f, 911.50f };
float mapA2_y[] = { 256.25f, 143.82f, 360.25f, 376.13f, 216.17f, 256.00f, 432.33f, 583.83f, 735.83f, 663.67f, 552.00f, 248.00f, 712.33f, 567.67f };
float mapA2_w[] = { 33.50f, 48.36f, 32.00f, 32.25f, 32.33f, 96.33f, 49.00f, 32.33f, 64.00f, 32.00f, 43.33f, 32.55f, 33.33f, 95.67f };
float mapA2_h[] = { 32.50f, 32.36f, 16.00f, 15.75f, 15.67f, 32.67f, 32.67f, 15.00f, 31.67f, 16.33f, 16.67f, 16.36f, 16.67f, 48.00f };
// second map level 2
float mapB2_x[] = { 431.84f, 512.00f, 671.98f, 816.38f, 928.00f, 816.18f, 448.13f, 240.00f, 239.88f, 1167.95f, 1151.86f, 1407.75f, 1408.00f, 1088.00f };
float mapB2_y[] = { 103.91f, 296.00f, 408.02f, 448.13f, 552.13f, 776.00f, 664.13f, 776.13f, 455.94f, 103.86f, 295.95f, 455.50f, 775.91f, 664.27f };
float mapB2_w[] = { 32.19f, 32.00f, 31.87f, 33.25f, 32.25f, 32.36f, 32.25f, 32.00f, 32.50f, 32.27f, 32.09f, 32.50f, 32.00f, 32.36f };
float mapB2_h[] = { 15.94f, 16.18f, 15.96f, 32.25f, 16.75f, 16.36f, 15.75f, 16.25f, 16.13f, 16.27f, 16.27f, 16.00f, 16.18f, 16.18f };
// third map level 2
float mapC2_x[] = { 543.91f, 687.91f, 591.36f, 180.75f, 204.63f, 640.14f, 924.32f, 1072.06f, 1424.19f, 1333.44f, 1392.00f, 977.64f };
float mapC2_y[] = { 103.91f, 184.09f, 319.91f, 272.75f, 687.88f, 583.95f, 608.00f, 743.88f, 728.13f, 456.13f, 263.94f, 320.00f };
float mapC2_w[] = { 32.00f, 31.82f, 59.09f, 42.50f, 57.75f, 32.09f, 56.45f, 31.88f, 32.38f, 43.88f, 32.25f, 37.09f };
float mapC2_h[] = { 16.00f, 16.18f, 31.82f, 31.00f, 31.75f, 16.09f, 32.00f, 16.00f, 16.50f, 16.25f, 16.13f, 31.64f };
// boss map
float mapBoss_x[] = { 800.0f,  800.0f,   40.0f, 1555.0f };
float mapBoss_y[] = { 39.0f,  835.0f,  450.0f,  450.0f };
float mapBoss_w[] = { 1600.0f, 1600.0f,   80.0f,   90.0f };
float mapBoss_h[] = { 78.0f,  130.0f,  900.0f,  900.0f };

const char* SpiderMoves[] = { "Assets/Characters/Mobs/Spider_Idle.png", "Assets/Characters/Mobs/Spider_Run.png", "Assets/Characters/Mobs/Spider_Attack.png", "Assets/Characters/Mobs/Spider_Dead.png" };
const char* BearMoves[] = { "Assets/Characters/Mobs/Bear_Idle.png", "Assets/Characters/Mobs/Bear_Run.png", "Assets/Characters/Mobs/Bear_Attack.png", "Assets/Characters/Mobs/Bear_Dead.png"};
const char* WolfMoves[] = { "Assets/Characters/Mobs/Wolf_Idle.png", "Assets/Characters/Mobs/Wolf_Run.png", "Assets/Characters/Mobs/Wolf_Attack.png", "Assets/Characters/Mobs/Wolf_Dead.png" };
const char* BoarMoves[] = { "Assets/Characters/Mobs/Boar_Idle.png", "Assets/Characters/Mobs/Boar_Run.png", "Assets/Characters/Mobs/Boar_Attack.png", "Assets/Characters/Mobs/Boar_Dead.png" };
const char* PigeonMoves[] = {"Assets/Characters/Mobs/Pigeon_Fly.png", "Assets/Characters/Mobs/Pigeon_Fly.png", "Assets/Characters/Mobs/Pigeon_Fly.png", "Assets/Characters/Mobs/Bear_Dead.png" };

MapData maps[LEVELS][MAPS_PER_LEVEL] =
{
	// Level 1: real maps
	{ {"./Assets/Maps/A1.png", "./Assets/Maps/A1obs.png", BoarMoves, mapA1_x, mapA1_y, mapA1_w, mapA1_h, 14, 100, 100, 32, 32}, {"./Assets/Maps/B1.png", "./Assets/Maps/B1obs.png", WolfMoves, mapB1_x, mapB1_y, mapB1_w, mapB1_h, 15, 100, 100, 32, 32},
	{"./Assets/Maps/C1.png", "./Assets/Maps/C1obs.png", BearMoves, mapC1_x, mapC1_y, mapC1_w, mapC1_h, 12, 80, 80, 32, 32}},
	// Level 2: reuse lvl1 images for now as placeholders
	{ {"./Assets/Maps/A2.png", "./Assets/Maps/A2obs.png", PigeonMoves, mapA2_x, mapA2_y, mapA2_w, mapA2_h, 14, 48, 48, 32, 32}, {"./Assets/Maps/B2.png", "./Assets/Maps/B2obs.png", SpiderMoves, mapB2_x, mapB2_y, mapB2_w, mapB2_h, 14, 130, 100, 130, 100},
	{"./Assets/Maps/C2.png", "./Assets/Maps/C2obs.png", SpiderMoves, mapC2_x, mapC2_y, mapC2_w, mapC2_h, 12, 130, 100, 130, 100}},
	// Level 3: same placeholders
	{ {"./Assets/Maps/B2.png", "./Assets/Maps/B2obs.png", SpiderMoves, mapB2_x, mapB2_y, mapB2_w, mapB2_h, 14, 130, 100, 130, 100}, {"./Assets/Maps/C1.png", "./Assets/Maps/C1obs.png", PigeonMoves, mapC1_x, mapC1_y, mapC1_w, mapC1_h, 14, 48, 48, 32, 32},
	{"./Assets/Maps/C2.png", "./Assets/Maps/C2obs.png", BearMoves, mapC2_x, mapC2_y, mapC2_w, mapC2_h, 12, 80, 80, 32, 32}},
	// Boss level: only [0] is used
	{ {"./Assets/Maps/bossmap.png", NULL, BearMoves, mapBoss_x, mapBoss_y, mapBoss_w, mapBoss_h, 4, 80, 80, 32, 32}, {0}, {0} },
};
int random_map[LEVELS];
int current_level = 0;

// randomize map for each level
void randomize(void)
{
	for (int i = 0; i < 3; i++)
		random_map[i] = rand() % MAPS_PER_LEVEL;
}

void Main_Menu_Init(void)
{
	globalfont = CP_Font_Load("./Assets/Dalek.ttf");
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Graphics_ClearBackground(CP_Color_Create(80, 80, 80, 255));
	bg = CP_Image_Load("./Assets/newbg.png");
	digipen = CP_Image_Load("./Assets/DigiPen_BLACK.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	CP_System_SetWindowSize(CP_Image_GetWidth(bg), CP_Image_GetHeight(bg));
	music = CP_Sound_Load("./Assets/Audios/main menu music.wav");
	team = CP_Image_Load("./Assets/grplogo.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	credit = CP_Image_Load("./Assets/credit.png");
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);

	isquit = 0;
	iscredit = 0;
}

void Main_Menu_Update(void)
{
	if (GAMEWIN == 1 && !iscredit)
	{
		isShow = 0;
		iscredit = 1;
		CP_Sound_PlayAdvanced(music, 0.3f, 1.0f, TRUE, CP_SOUND_GROUP_MUSIC);
	}
	if (isShow) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		timer += CP_System_GetDt();
		float fadeDuration = 0.5f;
		float singleDuration = 2.0f; // Duration for ONE image
		float totalDuration = 4.0f;  // Total duration for BOTH images

		// Determine which image to draw and calculate its local 0.0 -> 2.0 timer
		CP_Image currentImage = (timer < singleDuration) ? digipen : team;
		float localTimer = (timer < singleDuration) ? timer : (timer - singleDuration);

		int alpha = 255;

		if (localTimer <= fadeDuration) // In the first 0.5 seconds of the current image
		{
			// Fade in: 0 -> 255
			alpha = (int)((localTimer / fadeDuration) * 255.0f);
		}
		else if (localTimer >= (singleDuration - fadeDuration)) // In the last 0.5 seconds
		{
			// Fade out: 255 -> 0
			alpha = (int)(((singleDuration - localTimer) / fadeDuration) * 255.0f);
		}

		if (alpha < 0) alpha = 0;
		if (alpha > 255) alpha = 255;

		// Draw only the active image for the current 2-second window
		CP_Image_Draw(currentImage, 0, 0, 1600, 900, alpha);

		if (timer >= totalDuration || CP_Input_KeyTriggered(KEY_ENTER)) {
			isShow = 0;
			CP_Sound_PlayAdvanced(music, 0.3f, 1.0f, TRUE, CP_SOUND_GROUP_MUSIC);
			CP_Graphics_ClearBackground(CP_Color_Create(30, 30, 30, 0));
		}
		return;
	}
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

	CP_Settings_TextSize(30);
	CP_Font_DrawText("Digipen Institute of Technology Singapore", 300, 15);

	if (IsAreaClicked(area_center_x, play_height, area_width, area_height, click_x, click_y) && isquit != 1) { // if play is clicked
		current_level = 0;
		randomize();
		CP_Sound_StopAll();
		CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
	}

	if (IsAreaClicked(area_center_x, cred_height, area_width, area_height, click_x, click_y)) {
		iscredit = 1;	
	}
	if (iscredit) {
		CP_Image_Draw(credit, 0, 0, 1600, 900, 255);
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
			iscredit = 0;
			GAMEWIN = 0;
		}
	}

	if (IsAreaClicked(area_center_x, exit_height, area_width, area_height, click_x, click_y) || isquit) { // if exit is clicked
		isquit = 1;
		if (isquit) {
			// drawing grey overlay
			CP_Settings_RectMode(CP_POSITION_CORNER);
			CP_Settings_Fill(CP_Color_Create(130, 130, 130, 100));
			CP_Graphics_DrawRect(0, 0, 1600, 900);

			// drawing pop up
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(CP_Color_Create(16, 45, 2, 255));
			CP_Graphics_DrawRect(area_center_x, 400, 600, 600);

			// drawing yes and no buttons
			CP_Settings_Fill(CP_Color_Create(127, 98, 72, 255));
			CP_Graphics_DrawRect(area_center_x - 150, 400, 200, 200);
			CP_Graphics_DrawRect(area_center_x + 150, 400, 200, 200);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255)); // setting font color
			CP_Settings_TextSize(50);
			CP_Font_DrawText("QUIT?", area_center_x, 200);
			CP_Font_DrawText("NO", area_center_x - 150, 400);
			CP_Font_DrawText("YES", area_center_x + 150, 400);

			// if yes
			if (IsAreaClicked(area_center_x + 150, 400, 200, 200, click_x, click_y)) {
				CP_Engine_Terminate();
			}

			// if no
			if (IsAreaClicked(area_center_x - 150, 400, 200, 200, click_x, click_y)) {
				isquit = 0;
			}
		}		
	}
}

void Main_Menu_Exit(void)
{
	CP_Font_Free(&globalfont);
}