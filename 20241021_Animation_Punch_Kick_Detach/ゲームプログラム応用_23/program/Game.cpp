#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	2.0f
#define ANIM_RATE_SPEED		0.05f

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;

//	カメラ座標と向き
Float3 camera_pos;
Float3 camera_rot;

//	アニメーションの種類
enum EAnimState
{
	ANIM_WALK,
	ANIM_PUNCH,

	ANIM_MAX,		//	アニメーション数
};
EAnimState playing_anim = ANIM_WALK;
//	アニメーションデータ用の変数
int anim_model[ANIM_MAX];
//	アタッチ番号用の変数
int anim_attach[ANIM_MAX];
//	このアニメーションの１ループにかかる時間
float anim_total[ANIM_MAX];
//	アニメーションをどのくらい進めるかの時間
float anim_time[ANIM_MAX];
//	それぞれのアニメーションをどのくらいの割合にするか（ 0.0f ～ 1.0f ）
float anim_rate[ANIM_MAX];

//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標の初期設定
	camera_pos.set(0.0f, 2.0f, -3.75f);
	//	カメラの向き
	camera_rot.set(13.0f, 0.0f, 0.0f);

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	//	プレイヤーモデル（すでにアニメーションを付けるやつを読み込んでいます）
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	アニメーションデータの読み込み
	anim_model[ANIM_WALK] = MV1LoadModel("data/player/anim_walk.mv1");
	anim_model[ANIM_PUNCH] = MV1LoadModel("data/player/anim_punch.mv1");

	//	初期座標や向きの設定
	player_pos.set(1.5f, 0.0f, 0.0f);
	player_rot.y = 230.0f;

	npc_pos.set(-1.5f, 0.0f, 1.5f);

	//	アニメーションの初期化
	for (int i = 0; i < ANIM_MAX; i++) {
		//	モデルにアニメーションをアタッチします（アタッチ番号を取得）
		anim_attach[i] = MV1AttachAnim(player_model, 1, anim_model[i], FALSE);
		//	このアニメーションの１ループにかかる時間
		anim_total[i] = MV1GetAttachAnimTotalTime(player_model, anim_attach[i]);
		//	最初から開始
		anim_time[i] = 0.0f;
		//	いったん割合も０初期化
		anim_rate[i] = 0.0f;
	}
	anim_rate[ANIM_WALK] = 1.0f;

	MV1DetachAnim(player_model, anim_attach[ANIM_PUNCH]);
	anim_attach[ANIM_PUNCH] = -1;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (CheckHitKey(KEY_INPUT_UP)) {
		anim_rate[ANIM_WALK] = 0.0f;
		anim_rate[ANIM_PUNCH] = 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		anim_rate[ANIM_WALK] = 1.0f;
		anim_rate[ANIM_PUNCH] = 0.0f;
	}

	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;


	switch (playing_anim)
	{
	case ANIM_WALK:
		if (PushHitKey(KEY_INPUT_SPACE))
		{
			if (anim_attach[ANIM_PUNCH] == -1)
			{
				playing_anim = ANIM_PUNCH;
				anim_attach[ANIM_PUNCH] = MV1AttachAnim(player_model, 1, anim_model[ANIM_PUNCH], false);
				anim_time[ANIM_PUNCH] = 0.0f;
				anim_rate[ANIM_PUNCH] = 0.0f;
			}
		}
		break;

	case ANIM_PUNCH:
		anim_rate[ANIM_WALK] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_PUNCH] += ANIM_RATE_SPEED;

		if (anim_rate[ANIM_PUNCH] >= 1.0f)
		{
			anim_time[ANIM_PUNCH] += 1.0f;

			if (anim_time[ANIM_PUNCH] >= anim_total[ANIM_PUNCH])
			{
				playing_anim = ANIM_WALK;
			}
		}

		break;

	default:
		break;
	}



	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	アニメーションの割合を 0.0f ～ 1.0f の間にする
		anim_rate[i] = max(0.0f, min(anim_rate[i], 1.0f));

		//	アニメーション時間を進めて
		anim_time[i] += 1.0f;
		//	アニメーション時間が最後まで行ったら最初から
		if (anim_time[i] >= anim_total[i]) {
			anim_time[i] = 0.0f;
		}
		//	その時間のアニメーションにする
		MV1SetAttachAnimTime(player_model, anim_attach[i], anim_time[i]);
		//	それぞれのアニメーションの割合分の再生にする
		MV1SetAttachAnimBlendRate(player_model, anim_attach[i], anim_rate[i]);
	}

	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	座標と向きを渡してカメラの設定
	SetCameraPositionAndAngle(camera_pos.VGet(), TO_RADIAN(camera_rot.x), TO_RADIAN(camera_rot.y), TO_RADIAN(camera_rot.z));

	//	地面モデルの描画
	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	プレイヤーモデルの描画
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	//	今回のモデルはＺマイナス方向に向いているので描画するときにＹ軸に対して１８０度回転させます
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	//	今回のモデルはめちゃめちゃでかいので小さくします
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	MV1DrawModel(player_model);

	DrawString(16, 16, "左右キー：プレイヤーの向きの変更", GetColor(255, 255, 255));
	DrawString(16, 32, "スペース：パンチアニメーション", GetColor(255, 255, 255));
	DrawString(16, 48, "上下キー：とりあえずアニメーションを変えてみます（確認したらすぐコメントにします）", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	for (int i = 0; i < ANIM_MAX; i++) {
		MV1DeleteModel(anim_model[i]);
	}
}
