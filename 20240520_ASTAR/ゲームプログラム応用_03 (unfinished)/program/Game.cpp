#include "Main.h"
#include "Game.h"

//	パネルの大きさ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		16
#define MAP_H		 9

//	パネル画像用の変数
int panel_image[2];

//	ＮＰＣ画像
int npc_image;
//	黄色い枠画像
int target_image;

int npc_w;
int npc_h;

int target_w;
int target_h;

int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

int PathCost[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};


//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{

	//	画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");
	npc_image = LoadGraph("data/npc.bmp");
	target_image = LoadGraph("data/mark.bmp");

	npc_w = 13;
	npc_h = 5;

	target_w = 1;
	target_h = 1;

	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			PathCost[h][w] = 1000;
		}
	}

}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
int cheapestCost = 1000;
int goToPathX = 1000;
int goToPathY = 1000;

void GameUpdate()
{
	static int mousePosX = 0;
	static int mousePosY = 0;
	GetMousePoint(&mousePosX, &mousePosY);

	static int frame_count = 0;
	frame_count++;

	static int costLayer = 0;

	if (PushMouseInput())
	{
		costLayer = 0;
		for (int h = 0; h < MAP_H; h++) {
			for (int w = 0; w < MAP_W; w++) {
				PathCost[h][w] = 1000;
			}
		}

		target_w = mousePosX / PANEL_SIZE;
		target_h = mousePosY / PANEL_SIZE;
		PathCost[target_h][target_w] = 0;
	}

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		costLayer++;
		if (MapData[target_h][target_w] == 1) return;
		MapData[target_h][target_w] = 0;

		//for (int h = 0 - costLayer; h < 1 + costLayer; h++)
		//{
		//	if (target_h + h < 0) continue;
		//	if (target_h + h > MAP_H) continue;
		//	for (int i = 0 - costLayer; i < 1 + costLayer; i++)
		//	{
		//		if (target_w + i < 0) continue;
		//		if (target_w + i > MAP_W) continue;
		//
		//		if (MapData[target_h + h][target_w + i] == 1) continue;
		//		if (PathCost[target_h + h][target_w + i] != 1000) continue;
		//
		//		if (PathCost[target_h + h][target_w + i] != 0)
		//		{
		//			
		//		}
		//	}
		//}

		for (int h = 0; h < MAP_H; h++)
		{
			for (int i = 0; i < MAP_W; i++)
			{
				if (i < MAP_W - 1 && MapData[h][i + 1] == 0 && PathCost[h][i + 1] > costLayer)PathCost[h][i + 1] = costLayer + 1;
				if (i > 0 && MapData[h][i + 1] == 0 && PathCost[h][i - 1] > costLayer)PathCost[h][i - 1] = costLayer + 1;
				if (h < MAP_H - 1 && MapData[h][i + 1] == 0 && PathCost[h + 1][i] > costLayer)PathCost[h + 1][i] = costLayer + 1;
				if (h > 0 && MapData[h][i + 1] == 0 && PathCost[h - 1][i] > costLayer)PathCost[h - 1][i] = costLayer + 1;
			}

		}
		costLayer++;
	}

	if (frame_count % 10 == 0)
	{
		int leftCost = 1000;
		int rightCost = 1000;
		int upCost = 1000;
		int downCost = 1000;

		for (int h = -1; h < 2; h++)
		{
			for (int i = -1; i < 2; i++)
			{
				int currentSpotCost = PathCost[npc_h][npc_w];
				leftCost = PathCost[npc_h][npc_w - 1];
				rightCost = PathCost[npc_h][npc_w + 1];
				downCost = PathCost[npc_h - 1][npc_w];
				upCost = PathCost[npc_h + 1][npc_w];

				if (rightCost < currentSpotCost)
				{
					npc_w++;
				}
				else if (leftCost < currentSpotCost)
				{
					npc_w--;
				}
				else if (upCost < currentSpotCost)
				{
					npc_h++;
				}
				else if (downCost < currentSpotCost)
				{
					npc_h--;
				}
			}
		}

	}



}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {

			int num = MapData[h][w];

			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;

			DrawGraph(x, y, panel_image[num], TRUE);

			///////////////////////
			//ROUTE
			///////////////////////
			if (num == 0)
			{
				DrawFormatStringF(x, y, GetColor(255, 255, 255), "%d", PathCost[h][w]);
			}

		}
	}

	int nx = npc_w * PANEL_SIZE;
	int ny = npc_h * PANEL_SIZE;
	DrawGraph(nx, ny, npc_image, TRUE);

	int tx = target_w * PANEL_SIZE;
	int ty = target_h * PANEL_SIZE;
	DrawGraph(tx, ty, target_image, TRUE);

	DrawString(16, 16, "クリック：目的地設定", GetColor(255, 255, 255));
	DrawString(16, 32, "スペース：ルートを１個広げる　→　ルートが届いたらＮＰＣは目的地に向かって移動する", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(npc_image);
	DeleteGraph(target_image);
}
