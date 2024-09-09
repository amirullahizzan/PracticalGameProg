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

//	プレイヤー用のスポットライトハンドル
int player_light;
int npc_light;

Float3 light_rot;
bool light_right;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -10.5f), VGet(0.0f, 0.0f, 0.0f));

	//	今回は頂点の多い細かい地面モデルを描画しています
	ground_model = MV1LoadModel("data/ground2.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	初期座標や向きの設定
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	//	デフォルトの平行光源のライトを暗めなライトで設定
	SetLightDifColor(GetColorF(0.2f, 0.2f, 0.2f, 1.0f));

	//	スポットライトの作成（ここは変更しません）
	player_light = CreateSpotLightHandle(VGet(0.0f, 1.0f, 0.0f), VGet(0.0f, -1.0f, 1.0f), TO_RADIAN(45.0f), TO_RADIAN(30.0f), 100.0f, 1.0f, 0.0f, 0.01f);

	//	↓以下のところを変更していきます（更新処理で）
	//	スポットライトの座標の設定（とりあえず原点の少し上に置いています）
	SetLightPositionHandle(player_light, VGet(0.0f, 1.0f, 0.0f));
	//	スポットライトの向きの設定（とりあえず真下を向かせています）
	SetLightDirectionHandle(player_light, VGet(0.0f, -1.0f, 0.0f));
	//	スポットライトの色の設定
	SetLightDifColorHandle(player_light, GetColorF(0.5f, 0.5f, 1.0f, 0.0f));
	npc_light = CreateSpotLightHandle(VGet(0.0f, 1.0f, 0.0f), VGet(0.0f, -1.0f, 1.0f), TO_RADIAN(45.0f), TO_RADIAN(30.0f), 100.0f, 1.0f, 0.0f, 0.01f);
	SetLightDifColorHandle(npc_light, GetColorF(1.0f, 0.5f, 0.5f, 0.0f));
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	プレイヤーの回転
	player_rot.y += 1.0f;
	//	プレイヤーの座標移動
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = GetFloat3VTransform(base, mat);
	player_pos += mov;

	Float3 pos1 = player_pos + Float3(0.0f, 2.0f, 0.0f);
	SetLightPositionHandle(player_light, pos1.VGet());

	Float3 base1(0.0f, -1.0f, 1.0f);
	Float3 dir1 = GetFloat3VTransform(base1, mat);
	SetLightPositionHandle(player_light, dir1.VGet());


	//	ＮＰＣの回転
	npc_rot.y += 0.1f;

	if (light_right)
	{
		light_rot.y += 1.0f;
		if (light_rot.y >= 30.0f)
		{
			light_right = false;
		}
	}
	else
	{
		light_rot.y -= 1.0f;
		if (light_rot.y <= -30.0f)
		{
			light_right = true;
		}
	}

	Float3 pos2 = npc_pos + Float3(0.0f, 0.5f, 0.0f);
	SetLightPositionHandle(npc_light, pos2.VGet());
	Float3 base2(0.0f, 0.0f, 1.0f);

	MATRIX mat2 = MGetRotY(TO_RADIAN(npc_rot.y + light_rot.y));
	Float3 dir2 = GetFloat3VTransform(base2, mat);
	SetLightDirectionHandle(npc_light, dir2.VGet());
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	DrawString(16, 16, "スポットライト以外が真っ暗にならないようにディレクショナルライトで全体も照らす（色は暗め）", GetColor(255, 255, 255));
	DrawString(16, 32, "スポットライトを使って、プレイヤーがライトを照らす感じ", GetColor(255, 255, 0));
	DrawString(16, 48, "→　プレイヤーの頭の上から前下方向に", GetColor(255, 255, 0));
	DrawString(16, 64, "→　ライトの色を青っぽく", GetColor(255, 255, 0));
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

