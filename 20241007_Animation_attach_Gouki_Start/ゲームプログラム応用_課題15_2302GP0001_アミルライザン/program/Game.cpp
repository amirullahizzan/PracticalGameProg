#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	歩いているときの移動スピード
#define PLAYER_WALK_SPEED	0.025f
//	走っているときの移動スピード
#define PLAYER_RUN_SPEED	0.055f
//	回転スピード
#define PLAYER_ROT_SPEED	2.0f

//	プレイヤーとＮＰＣの距離がこれより短かったら走り始める
#define RUN_DISTANCE		5.0f

//	プレイヤーとＮＰＣの範囲用の半径（発展課題で使います）
#define CHARA_RADIUS		0.75f

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;

//	アニメーションの種類
enum {
	ANIM_STAND,		//	立ちアニメ
	ANIM_WALK,		//	歩きアニメ
	ANIM_RUN,		//	歩きアニメ

	ANIM_MAX,		//	アニメーション数
};
//	アニメーションデータ用の変数
int anim_model[ANIM_MAX];
//	アタッチ番号用の変数
int anim_attach[ANIM_MAX];
//	このアニメーションの1ループにかかる時間
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

int player_circle_color = GetColor(0, 0, 255);
//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -7.0f), VGet(0.0f, 1.0f, 0.0f));

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	//	プレイヤーモデル（すでにアニメーションを付けるやつを読み込んでいます）
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	アニメーションデータの読み込み
	anim_model[ANIM_STAND] = MV1LoadModel("data/player/anim_stand.mv1");
	anim_model[ANIM_WALK] = MV1LoadModel("data/player/anim_walk.mv1");
	anim_model[ANIM_RUN] = MV1LoadModel("data/player/anim_run.mv1");

	//	初期座標や向きの設定
	player_pos.set(3.0f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-3.0f, 0.0f, 2.0f);

	//	アニメーションの数分処理します
	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	モデルにアニメーションをアタッチします（アタッチ番号を取得）
		anim_attach[i] = MV1AttachAnim(player_model, 1, anim_model[i], FALSE);
		//	このアニメーションの1ループにかかる時間
		anim_total[i] = MV1GetAttachAnimTotalTime(player_model, anim_attach[i]);
		//	最初から開始
		anim_time[i] = 0.0f;
		//	いったん割合も０初期化
		anim_rate[i] = 0.0f;
	}
	//	立ちアニメーションから開始しようと思うのでその割合を 1.0f にします
	anim_rate[ANIM_STAND] = 1.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
bool IsHitCircleCircle(float x1, float y1, float x2, float y2, float r1, float r2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;

	float distanceSquared = dx * dx + dy * dy;

	float r = r1 + r2;
	float rSum = r * r;

	return distanceSquared < rSum;
}

void GameUpdate()
{

	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーを押していたら向いている方向に移動
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Float3 base(0.0f, 0.0f, PLAYER_WALK_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);

		static constexpr float speedMult = 2.5f;
		float distance = GetFloat3Distance(player_pos, npc_pos);
		if (distance <= CHARA_RADIUS + CHARA_RADIUS)
		{
			Float3 diff = player_pos - npc_pos;
			float length = CHARA_RADIUS + CHARA_RADIUS - distance;
			diff.SetLength(length);

			player_pos += diff;
		}

		if (IsHitCircleCircle(player_pos.x, player_pos.z, npc_pos.x, npc_pos.z, RUN_DISTANCE, 0.1f) && distance > CHARA_RADIUS + CHARA_RADIUS)
		{
			player_circle_color = GetColor(255, 255, 255);

			player_pos += mov * speedMult;

			anim_rate[ANIM_WALK] -= 0.1f;
			anim_rate[ANIM_STAND] -= 0.1f;
			anim_rate[ANIM_RUN] += 0.1f;
		}
		else
		{
			player_pos += mov;
			player_circle_color = GetColor(0, 0, 255);
			//	立ちアニメーションは割合を下げる
			anim_rate[ANIM_STAND] -= 0.1f;
			anim_rate[ANIM_RUN] -= 0.1f;
			//	歩きアニメーションの割合を上げていきたい（徐々に増やす）
			anim_rate[ANIM_WALK] += 0.1f;
		}

	}
	else
	{
		//	上キーを押していないとき（立ちを上げて歩きを下げる）
		anim_rate[ANIM_WALK] -= 0.1f;
		anim_rate[ANIM_RUN] -= 0.1f;
		anim_rate[ANIM_STAND] += 0.1f;
	}

	//	アニメーションを再生させる
	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	割合を 0.0f ～ 1.0f にする
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
	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	範囲が分かるように目安の円
	DrawCircle3D_XZ(npc_pos, RUN_DISTANCE, GetColor(255, 255, 0));
	DrawCircle3D_XZ(npc_pos, CHARA_RADIUS, GetColor(255, 0, 0));
	DrawCircle3D_XZ(player_pos, CHARA_RADIUS, player_circle_color);

	//	プレイヤーモデルの描画
	//	モデルの座標を設定
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	//	モデルの向き設定（今回のモデルはＺマイナス方向を向いているので描画の時にＹ軸に対して１８０度回転させています）
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	//	今回のモデルはめちゃめちゃ大きいので小さくしています
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	//	そのモデルを描画
	MV1DrawModel(player_model);

	DrawString(16, 16, "十字キー：プレイヤーの向き・座標の変更", GetColor(255, 255, 255));
	DrawString(16, 32, "ＮＰＣに近づいたら（プレイヤーが黄色い円の中に入ったら）", GetColor(255, 255, 0));
	DrawString(16, 48, "→　プレイヤーの移動スピードアップ（さらに走りアニメーションにする）", GetColor(255, 255, 0));
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
