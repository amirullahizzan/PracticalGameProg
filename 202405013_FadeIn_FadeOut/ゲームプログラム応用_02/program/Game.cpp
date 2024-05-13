#include "Main.h"
#include "Game.h"

int title_image;
int stage_image;

enum {
	SCENE_TITLE,
	SCENE_STAGE,
};
int scene_num;

enum
{
	FADE_NONE,

	FADE_ALPHA_OUT,
	FADE_ALPHA_IN,
};
int fade_mode = FADE_NONE;
int fade_alpha = 0;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	scene_num = SCENE_TITLE;

	title_image = LoadGraph("data/title.bmp");
	stage_image = LoadGraph("data/stage.bmp");
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	switch (scene_num) {
	case SCENE_TITLE:

		switch (fade_mode)
		{
		case FADE_NONE:
			if (PushHitKey(KEY_INPUT_F1))
			{
				scene_num = SCENE_STAGE;
			}
			if (PushHitKey(KEY_INPUT_F2))
			{
				fade_mode = FADE_ALPHA_OUT;
				fade_alpha = 0;
			}
			break;

		case FADE_ALPHA_OUT:
			fade_alpha += 10;
			if (fade_alpha > 255)
			{
				fade_alpha = 255;
				scene_num = SCENE_STAGE;
				fade_mode = FADE_ALPHA_IN;
			}
			break;
		}

	case SCENE_STAGE:
		switch (fade_mode)
		{
		case FADE_NONE:
			if (PushHitKey(KEY_INPUT_F1) || PushHitKey(KEY_INPUT_F2))
			{
				if (fade_mode == FADE_NONE)
				{
					scene_num = SCENE_TITLE;
				}
			}
			break;
		case FADE_ALPHA_OUT:
			
			break;
		case FADE_ALPHA_IN:
			fade_alpha -= 10;

			if (fade_alpha < 0)
			{
				fade_alpha = 0;
				fade_mode = FADE_NONE;
			}

			break;
		}
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	switch (scene_num)
	{
	case SCENE_TITLE:
		DrawGraph(0, 0, title_image, TRUE);
		DrawString(16, 16, "タイトル画面", GetColor(255, 255, 255));
		DrawString(16, 32, "Ｆ１キー：すぐにステージ画面へ", GetColor(255, 255, 255));
		DrawString(16, 48, "Ｆ２キー：フェードアウトしてステージ画面へ", GetColor(255, 255, 255));
		break;

	case SCENE_STAGE:
		DrawGraph(0, 0, stage_image, TRUE);
		DrawString(16, 16, "ステージ画面", GetColor(255, 255, 255));
		DrawString(16, 32, "Ｆ１～Ｆ２キー：すぐにタイトル画面へ戻る", GetColor(255, 255, 255));
		break;
	}

	switch (fade_mode)
	{
	case FADE_NONE:
		break;
	case FADE_ALPHA_OUT:

	case FADE_ALPHA_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		DrawFillBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(title_image);
	DeleteGraph(stage_image);
}
