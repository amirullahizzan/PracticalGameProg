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
Float3 	light_pos;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -10.5f), VGet(0.0f, 0.0f, 0.0f));

	//	すでに細かめの地面モデルを使っています
	ground_model = MV1LoadModel("data/ground2.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	初期座標や向きの設定
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	//	点光源の設定（「こういう感じで.exe」ではこの１行はこれ以上変更していません）
	ChangeLightTypePoint(VGet(0.0f, 5.0f, 0.0f), 100.0f, 1.0f, 0.0f, 0.01f);

	//	ライトの座標の設定
	SetLightPosition(VGet(0.0f, 5.0f, 0.0f));
	//	ライトの色の設定
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));
}

int lightMode = 0;
float light_travel_vector = 0;
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		lightMode++;
		if (lightMode > 1)
		{
			lightMode = 0;
		}
	}

	//	プレイヤーの回転
	player_rot.y += 1.0f;
	//	プレイヤーの座標移動
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = VTransform(base.VGet(), mat);
	player_pos += mov;

	//	ＮＰＣの回転
	npc_rot.y += 0.5f;

	//	プレイヤーの前方の座標を求める

	//	その座標をライト座標に設定する

		//Light Blue
	if (lightMode == 0)
	{
		float extra_distance = 1.0f;
		base.y += 1.0f;
		base.z += extra_distance;
		light_pos = player_pos + VTransform(base.VGet(), mat);
		SetLightPosition(light_pos.VGet());
		//clsDx();
		//printfDx("light_pos %f, %f ", light_pos.x, light_pos.y);
		light_travel_vector = 0; //resets
		SetLightDifColor(GetColorF(0.3f, 0.3f, 1.0f, 0.0f));
	}
	else if (lightMode == 1)//Light Red
	{
		Float3 enemyBase(0.0f, 0.0f, 0.03f);

		MATRIX enemyMat = MGetRotY(TO_RADIAN(npc_rot.y));
		float extra_distance = 1.0f;
		enemyBase.y += 1.0f;
		enemyBase.z += extra_distance + light_travel_vector;
		light_pos = npc_pos + VTransform(enemyBase.VGet(), enemyMat);
		SetLightPosition(light_pos.VGet());
		SetLightDifColor(GetColorF(1.0f, 0.3f, 0.3f, 0.0f));
		const float light_movespeed = 0.5f;
		light_travel_vector+= light_movespeed;
	}

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

	DrawString(16, 16, "ライト座標の変更（プレイヤーの前方に）と色の変更（青っぽく）", GetColor(255, 255, 0));
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
