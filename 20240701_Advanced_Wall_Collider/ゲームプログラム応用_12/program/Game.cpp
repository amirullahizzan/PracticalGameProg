#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"

//	１個の四角の大きさ
#define PANEL_SIZE	50

#define PANEL_HALF	(PANEL_SIZE/2)
//	四角を置ける数
#define MAP_W		16
#define	MAP_H		 9

//	２次元配列（０：進める　１：進めない）
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

//	マップ画像
int panel_image[2];

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	15.0f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	3.0f

//	プレイヤー画像
int player_image;
//	プレイヤー座標
Float2 player_pos;
//	プレイヤーの向き
float player_rot;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	player_image = LoadGraph("data/player.bmp");
	//	初期座標や向きの設定
	player_pos.x = 100.0f;
	player_pos.y = 100.0f;
	player_rot = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	Float2 before = player_pos;
	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot -= PLAYER_ROT_SPEED;
	//	上キーで向いている方向に移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_pos.x += PLAYER_MOV_SPEED * cosf(TO_RADIAN(player_rot));
		player_pos.y += PLAYER_MOV_SPEED * sinf(TO_RADIAN(player_rot));
	}

	Float2 p_pos(player_pos.x - 23.0f, player_pos.y - 23.0f);
	Float2 p_size(PANEL_SIZE - 4.0f, PANEL_SIZE - 4.0f);

	//used for advanced wall
	Float2 box_before(before.x - 23.0f, before.y - 23.0f);

	Float2 panel_pos;
	Float2 panel_size(PANEL_SIZE, PANEL_SIZE);

	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			if (MapData[h][w] == 0) continue;

			panel_pos.x = w * PANEL_SIZE;
			panel_pos.y = h * PANEL_SIZE;

			if (CheckBoxHit(p_pos, p_size, panel_pos, panel_size))
			{

				//simple wall
				//player_pos = before;

				//advanced wall
				if (box_before.y + p_size.y >= panel_pos.y && box_before.y <= panel_pos.y + panel_size.y)
				{
					//player_pos.x = before.x; //this returns player incorrectly if in high speed
					if (before.x < player_pos.x) player_pos.x = max(before.x, panel_pos.x - PANEL_HALF);
					if (before.x > player_pos.x) player_pos.x = min(before.x, panel_pos.x + PANEL_SIZE + PANEL_HALF);
				}
				else if (box_before.x + p_size.x >= panel_pos.x && box_before.x <= panel_pos.x + panel_size.x)
				{
					//player_pos.y = before.y; //this returns player incorrectly if in high speed
					if (before.y < player_pos.y) player_pos.y = max(before.y, panel_pos.y - PANEL_HALF);
					if (before.y > player_pos.y) player_pos.y = min(before.y, panel_pos.y + PANEL_SIZE + PANEL_HALF);
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
	//	マップの描画
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	そこの場所の番号
			int num = MapData[h][w];
			//	四角を描画する座標
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	そこにその番号の四角の描画
			DrawGraph(x, y, panel_image[num], TRUE);
		}
	}

	DrawRotaGraphF(player_pos.x, player_pos.y, 1.0f, TO_RADIAN(player_rot), player_image, TRUE);

	DrawString(16, 16, "←→キー：プレイヤー回転", GetColor(255, 255, 255));
	DrawString(16, 32, "↑キー　：プレイヤー移動", GetColor(255, 255, 255));
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
