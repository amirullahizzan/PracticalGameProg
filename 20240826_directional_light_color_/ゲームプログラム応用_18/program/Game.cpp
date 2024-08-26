#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	モデル用の変数
int ground_model;
int player_model;
int npc_model;
int arrow_model;

//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

Float3 arrow_pos;
Float3 arrow_rot;

Float3 light_dir;

int color_num;
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
	arrow_model = MV1LoadModel("data/arrow.mqoz");

	//	初期座標や向きの設定
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	arrow_pos.set(0.0f, 0.2f, -2.0f);

	color_num = 0;

	light_dir.set(1.0f, -1.0f, 1.0f);

}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	player_rot.y += 1.0f;

	//	元ベクトルを作成
	Float3 base(0.0f, 0.0f, 0.03f);
	//	Ｙ軸回転行列を作成
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	//	元ベクトルをその行列で変換して移動ベクトル作成
	Float3 mov = VTransform(base.VGet(), mat);
	//	その移動ベクトルで座標移動
	player_pos += mov;

	npc_rot.y += 0.5f;

	Float3 light_base(0.0f, -1.0f, 1.0f); //is actually a base direction which then later on modified by the arrow's vector
	//obtain facing of the arrow
	MATRIX light_mat = MGetRotY(TO_RADIAN(arrow_rot.y));

	light_dir = GetFloat3VTransform(light_base, light_mat);

	clsDx();
	printfDx("%f , %f , %f", light_dir.x, light_dir.y, light_dir.z);

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		arrow_rot.y += 3.0f;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		arrow_rot.y -= 3.0f;
	}

	if (PushHitKey(KEY_INPUT_F1)) 
	{
		color_num = 0;
	}
	if (PushHitKey(KEY_INPUT_F2)) {
		color_num = 1;
	}
	if (PushHitKey(KEY_INPUT_F3)) {
		color_num = 2;
	}
	if (PushHitKey(KEY_INPUT_F4)) {
		color_num = 3;
	}

	COLOR_F light_color[4] =
	{
		//RGBA
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		{ 0.0f,1.0f,0.0f,1.0f },
		{ 0.0f,0.0f,1.0f,1.0f }
	};

	SetLightDifColor(light_color[color_num]);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	SetUseLighting(true);
	ChangeLightTypeDir(light_dir.VGet());
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

	//Set arrow to lit (unaffected by light)
	SetUseLighting(false);

	//Arrow
	MV1SetScale(arrow_model, VGet(0.5f, 0.5f, 0.5f));
	MV1SetPosition(arrow_model, VGet(arrow_pos.x, arrow_pos.y, arrow_pos.z));
	MV1SetRotationXYZ(arrow_model, VGet(TO_RADIAN(arrow_rot.x), TO_RADIAN(arrow_rot.y), TO_RADIAN(arrow_rot.z)));
	MV1DrawModel(arrow_model);


	DrawString(16, 16, "←→キー：ライト方向の変更（ＸＺ方向）", GetColor(255, 255, 255));
	DrawString(16, 32, "Ｆ１～Ｆ４キー：ライトの色変更", GetColor(255, 255, 255));
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
