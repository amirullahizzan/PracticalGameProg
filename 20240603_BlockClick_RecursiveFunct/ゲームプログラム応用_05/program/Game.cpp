#include "Main.h"
#include "Game.h"
#include "Random.h"

//	パネルサイズ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		16		//	横方向
#define MAP_H		 9		//	縦方向

//	パネル画像用の変数
int panel_image[4];

//	２次元配列
int MapData[MAP_H][MAP_W];

//	クリックしたポイント
int click_w;
int click_h;
int target_num;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	パネル画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");
	panel_image[2] = LoadGraph("data/panel2.bmp");
	panel_image[3] = LoadGraph("data/panel3.bmp");

	//	マップデータの中身を１～３のランダムな値で埋めます
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	ランダムで１～３
			MapData[h][w] = GetRandomI(1, 3);
		}
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	マウスをクリック
	if (PushMouseInput()) {
		//	クリックした座標が配列的に何番目のところか
		click_w = GetMouseX() / PANEL_SIZE;
		click_h = GetMouseY() / PANEL_SIZE;

	}

	if (MapData[click_h][click_w] > 0)
	{
		target_num = MapData[click_h][click_w];
	}

	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			if (MapData[h][w] == -1)
			{
				MapData[h][w] = 0;
			}
		}

				MapData[click_h][click_w] = -1;
	}

	//	スペースキーを押す
	if (PushHitKey(KEY_INPUT_SPACE)) 
	{

		for (int h = 0; h < MAP_H; h++)
		{
			for (int w = 0; w < MAP_W; w++)
			{
				if (MapData[h][w] == -1)
				{
					if (w + 1 < MAP_W && MapData[h][w + 1] == target_num) {MapData[h][w+1] = -1;}
					if (w - 1 >= 0 && MapData[h][w - 1] == target_num)    {MapData[h][w-1] = -1;}
					if (h + 1 < MAP_H && MapData[h+1][w] == target_num) { MapData[h+1][w] = -1; }
					if (h - 1 >= 0 && MapData[h-1][w] == target_num) { MapData[h-1][w] = -1; }
				}
			}

			MapData[click_h][click_w] = -1;
		}
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	パネルの描画
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	マップデータからそこの場所の番号を取得
			int num = MapData[h][w];
			//	画面のどこに表示するかの座標を求める
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			if (num >= 0) {
				//	その場所にその画像を描画
				DrawGraph(x, y, panel_image[num], TRUE);
			}
			//	そこのマップ番号を文字列で描画します
			DrawFormatString(x, y, GetColor(255, 255, 255), "%d", num);
		}
	}

	DrawString(16, 16, "クリック：パネルを消す", GetColor(255, 255, 255));
	DrawString(16, 32, "スペース：消えているパネルの上下左右にある同じ色のパネルも消す", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);
	DeleteGraph(panel_image[2]);
	DeleteGraph(panel_image[3]);
}
