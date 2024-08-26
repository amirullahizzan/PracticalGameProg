#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	モデル用の変数
int ground_model;
int player_model;
int npc_model;

//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

//	何番目の色を使うのか
COLOR_F currentColor = { 1.0f,1.0f,1.0f,1.0f };
int color_num;

//	ライトの色データ（ 0.0f ～ 1.0f ）
COLOR_F light_color[4] =
{
	//	 R     G     B     A
	//{ 1.0f, 1.0f, 1.0f, 1.0f },		//	白
	//{ 1.0f, 0.0f, 0.0f, 1.0f },		//	赤
	//{ 0.0f, 1.0f, 0.0f, 1.0f },		//	緑
	//{ 0.0f, 0.0f, 1.0f, 1.0f },		//	青

	//朝・昼・夕方・夜

	{ 0.65f, 0.65f, 0.8f, 1.0f },		//	Asa
	{ 1.0f, 1.0f, 1.0f, 1.0f },		//	Hiru
	{ 0.8f, 0.5f, 0.2f, 1.0f },		//	Yuugata
	{ 0.1f, 0.1f, 0.5f, 1.0f },		//	Yoru
};

template<typename T>
T lerp(T a, T b, float t)
{
	return a + t * (b - a);
}

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -4.5f), VGet(0.0f, 0.0f, 0.0f));

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	初期座標や向きの設定
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	color_num = 0;
	currentColor = light_color[color_num];
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	player_rot.y += 1.0f;

	//	プレイヤーの座標移動
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = VTransform(base.VGet(), mat);
	player_pos += mov;

	npc_rot.y += 0.5f;

	//	Ｆ１～Ｆ４キーで色番号を変更
	if (PushHitKey(KEY_INPUT_F1))
	{
		color_num = 0;
	}
	if (PushHitKey(KEY_INPUT_F2))
	{
		color_num = 1;
	}
	if (PushHitKey(KEY_INPUT_F3))
	{
		color_num = 2;
	}
	if (PushHitKey(KEY_INPUT_F4))
	{
		color_num = 3;
	}

	const int color_change_freq = 100;
	static int game_time = 0;
	if (game_time % color_change_freq == 0 || game_time == 0)
	{
		color_num++;
		if (color_num > 3)
		{
			color_num = 0;
		}
	}
	game_time++;

	float color_change_speed = 0.01f;
	float red = light_color[color_num].r;
	float green = light_color[color_num].g;
	float blue = light_color[color_num].b;
	currentColor.r = lerp(currentColor.r, red,   color_change_speed);
	currentColor.g = lerp(currentColor.g, green, color_change_speed);
	currentColor.b = lerp(currentColor.b, blue,  color_change_speed);

	
	clsDx();
	printfDx("%d\n", color_num);
	printfDx("%f , %f : \n", currentColor.r, red);

	//currentColor.r
	//	上の色を渡してライトの色の設定
	SetLightDifColor(currentColor);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	地面モデルの描画
	MV1DrawModel(ground_model);

	//	ＮＰＣ座標や向きを設定してモデルの描画
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	プレイヤー座標や向きを設定してモデルの描画
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	DrawString(16, 16, "一定時間でライトの色の変更（色は「朝→昼→夕方→夜」っぽく）", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);
}
