#include "Main.h"
#include "Game.h"

int bg_image;
int player_image;
int npc_image;

#define TEST_MAX 2000
int bigImage[TEST_MAX];
int frameCount = 0;
enum
{
	SCENE_LOAD,
	SCENE_GAME,
};
int sceneNum = SCENE_LOAD;
//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	sceneNum = SCENE_LOAD;
	SetUseASyncLoadFlag(true);
	for (int i = 0; i < TEST_MAX; i++)
	{
		bigImage[i] = LoadGraph("data/test.bmp");

		//bigImage[i] = DxLib::MakeGraph(0, 0, false);
	}
	SetUseASyncLoadFlag(false);


	bg_image = LoadGraph("data/bg.bmp");

	player_image = LoadGraph("data/player.bmp");
	npc_image = LoadGraph("data/npc.bmp");
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
int loadCount;

void GameUpdate()
{

	switch (sceneNum)
	{
	case SCENE_LOAD:

		frameCount++; //print another timer instead of the data loaded so it can be modified
		loadCount = GetASyncLoadNum();
		if (loadCount == 0)
		{
			sceneNum = SCENE_GAME;
		}

		break;

	case SCENE_GAME:
		break;


	}

}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{

	clsDx();
	printfDx("%d", bigImage[0]);

	switch (sceneNum)
	{
	case SCENE_LOAD:
		DrawString(250, 250, "LOADING SCREEN", GetColor(255, 255, 255));
		if (frameCount / 30 % 2)
		{
		DrawFormatString(290, 350, GetColor(255, 255, 255), "Loading ...  %d% ", frameCount);
		}

		break;

	case SCENE_GAME:
		DrawGraph(0, 0, bg_image, TRUE);

		DrawRotaGraphF(600.0f, 200.0f, 1.0f, TO_RADIAN(180.0f), npc_image, TRUE);

		DrawRotaGraphF(200.0f, 200.0f, 1.0f, TO_RADIAN(0.0f), player_image, TRUE);

		DrawString(16, 16, "ゲーム画面", GetColor(255, 255, 255));

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
	for (int i = 0; i < TEST_MAX; i++)
	{
		DeleteGraph(bigImage[i]);
	}


	DeleteGraph(bg_image);
	DeleteGraph(player_image);
	DeleteGraph(npc_image);
}
