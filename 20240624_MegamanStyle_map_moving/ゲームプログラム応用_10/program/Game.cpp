#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"
#include <cmath>

//	パネルの大きさ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		16
#define MAP_H		 9

//	パネル画像用の変数
//	０：通れる
//	１：通れない
int panel_image[2];

//	マップ用の２次元配列
//	０：通れる
//	１：通れない
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	プレイヤー画像
int player_image;

int player_w;
int player_h;

Float2 player_pos;
Float2 player_target_pos;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	player_image = LoadGraph("data/player.bmp");
	player_w = 1;
	player_h = 1;

	player_pos.x = player_w * PANEL_SIZE;
	player_pos.y = player_h * PANEL_SIZE;

	player_target_pos.x = player_pos.x;
	player_target_pos.y = player_pos.y;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
Float2 Normalize(const Float2& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	return { v.x / length, v.y / length };
}

bool isMoving = false;
void GameUpdate()
{
	if (!isMoving)
	{
		if (PushHitKey(KEY_INPUT_RIGHT)) {
			if (MapData[player_h][player_w + 1] == 0) {
				player_w++;
			}
		}
		else if (PushHitKey(KEY_INPUT_LEFT)) {
			if (MapData[player_h][player_w - 1] == 0) {
				player_w--;
			}
		}
		else if (PushHitKey(KEY_INPUT_DOWN)) {
			if (MapData[player_h + 1][player_w] == 0) {
				player_h++;
			}
		}
		else if (PushHitKey(KEY_INPUT_UP)) {
			if (MapData[player_h - 1][player_w] == 0) {
				player_h--;
			}
		}
	}


	player_target_pos.x = player_w * PANEL_SIZE;
	player_target_pos.y = player_h * PANEL_SIZE;

	if (GetFloat2Distance(player_pos, player_target_pos) > 0.01f)
	{
		const float moveSpeed = 3.0f;
		Float2 direction = Normalize({ player_target_pos.x - player_pos.x, player_target_pos.y - player_pos.y });
		player_pos.x += direction.x * moveSpeed;
		player_pos.y += direction.y * moveSpeed;
		isMoving = true;
	}
	else
	{
		isMoving = false;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	マップの描画
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	その場所のマップ番号を取得
			int num = MapData[h][w];
			//	実際の描画座標を取得
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	そこの座標にその番号の四角画像を描画
			DrawGraph(x, y, panel_image[num], TRUE);
		}
	}


	DrawGraphF(player_pos.x, player_pos.y, player_image, TRUE);

	DrawString(16, 16, "十字キー：プレイヤー移動", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(player_image);
}
