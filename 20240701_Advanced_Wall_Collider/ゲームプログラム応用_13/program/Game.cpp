#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	１個の四角の大きさ
#define PANEL_SIZE	1.0f
//	半分の大きさ
#define PANEL_HALF	(PANEL_SIZE/2.0f)

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

int ground_model;
//	マップモデル
int panel_model[2];

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	0.1f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	3.0f

//	プレイヤーモデル
int player_model;
//	プレイヤー座標
Float3 player_pos;
//	プレイヤーの向き
Float3 player_rot;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラの設定
	SetCameraPositionAndTarget_UpVecY(VGet(7.5f, 11.0f, -5.0f), VGet(7.5f, 0.0f, 3.0f));

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");

	panel_model[0] = MV1LoadModel("data/panel0.mqoz");
	panel_model[1] = MV1LoadModel("data/panel1.mqoz");

	player_model = MV1LoadModel("data/player.mqoz");
	//	初期座標や向きの設定
	player_pos.x = 2.0f;
	player_pos.z = 2.0f;
	player_rot.clear();
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	Float3 before = player_pos;

	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで向いている方向に移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_pos.x += PLAYER_MOV_SPEED * sinf(TO_RADIAN(player_rot.y));
		player_pos.z += PLAYER_MOV_SPEED * cosf(TO_RADIAN(player_rot.y));
	}

	Float3 player_size(PANEL_HALF - 0.1f, PANEL_HALF - 0.1f, PANEL_HALF - 0.1f);
	Float3 panel_pos;
	Float3 panel_size(PANEL_HALF , PANEL_HALF, PANEL_HALF);

	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {

			//	マップ番号が０番とは当たり判定はしません
			if (MapData[h][w] == 0)	continue;

			panel_pos.x = w * PANEL_SIZE;
			panel_pos.z = h * PANEL_SIZE;

			if (CheckBoxHit3D(player_pos, player_size, panel_pos, panel_size))
			{
				if (before.z + panel_size.z >= panel_pos.z - panel_size.z && before.z - player_size.z <= panel_pos.z + panel_size.z)
				{
					if (before.x < player_pos.x) player_pos.x = max(before.x , panel_pos.x - PANEL_SIZE);
					if (before.x > player_pos.x) player_pos.x = min(before.x , panel_pos.x + PANEL_SIZE);
				}
				if (before.x + panel_size.x >= panel_pos.x - panel_size.x && before.x - player_size.x <= panel_pos.x + panel_size.x)
				{
					if (before.z < player_pos.z) player_pos.z = max(before.z, panel_pos.z - PANEL_SIZE);
					if (before.z > player_pos.z) player_pos.z = min(before.z, panel_pos.z + PANEL_SIZE);
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
	DrawSphere3D(VGet(0.0f, 0.0f, 0.0f), 0.2f, 100, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	//	マップの描画
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	マップ番号の取得
			int num = MapData[h][w];
			//	描画するための座標を設定
			Float3 pos;
			pos.x = w * PANEL_SIZE;
			pos.y = 0.0f;
			pos.z = h * PANEL_SIZE;

			MV1SetPosition(panel_model[num], pos.VGet());
			MV1DrawModel(panel_model[num]);
		}
	}


	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	Float3 pos = player_pos;

	pos.x = round(player_pos.x / PANEL_SIZE) * PANEL_SIZE;
	pos.z = round(player_pos.z / PANEL_SIZE) * PANEL_SIZE;

	DrawBox3D_XZ(pos, PANEL_HALF, PANEL_HALF, GetColor(255,255,0));

	DrawString(16, 16, "十字キー：プレイヤー回転・移動", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(panel_model[0]);
	MV1DeleteModel(panel_model[1]);
	MV1DeleteModel(player_model);
}
