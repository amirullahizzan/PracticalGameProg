#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	ＮＰＣの数（「発展課題」で使う定数です）
#define NPC_MAX	5

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model[NPC_MAX];

//	アニメーションデータ用の変数
int anim_model;

Float3	player_pos;
Float3	player_rot;
//	プレイヤーのアニメーション関係
int player_attach;	//	アタッチ番号
float player_total;	//	アニメーションのトータル時間
float player_frame;	//	アニメーション時間

Float3	npc_pos[NPC_MAX];
Float3	npc_rot[NPC_MAX];
int   npc_attach[NPC_MAX];	//	アタッチ番号
float npc_total[NPC_MAX];	//	アニメーションのトータル時間
float npc_frame[NPC_MAX];	//	アニメーション時間
int npc_image_num[NPC_MAX];

int punch_anim_model;

//	ＮＰＣモデルに貼り付けるテクスチャの数（全部で６個）
#define IMAGE_MAX	6
int sample_image[IMAGE_MAX];

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -5.0f), VGet(0.0f, 1.0f, -1.0f));

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/chara/model.mv1");
	//npc_model = MV1LoadModel( "data/npc.mqoz" );
	for (int i = 0; i < NPC_MAX; i++)
	{
		npc_model[i] = MV1DuplicateModel(player_model);
	}

	//	アニメーションデータの読み込み
	anim_model = MV1LoadModel("data/chara/anim_walk.mv1");
	punch_anim_model = MV1LoadModel("data/chara/anim_punch.mv1");

	////////////////////////
	for (int i = 0; i < IMAGE_MAX; i++)
	{
		char text[256];
		sprintf(text, "data/sample%d.png", i);
		sample_image[i] = LoadGraph(text);
	}

	player_pos.set(1.5f, 0.0f, -1.0f);
	player_rot.y = 220.0f;

	//	プレイヤーアニメーションの初期設定
	player_attach = MV1AttachAnim(player_model, 1, anim_model, FALSE);
	player_total = MV1GetAttachAnimTotalTime(player_model, player_attach);
	player_frame = 0.0f;

	for (int i = 0; i < NPC_MAX; i++)
	{
		npc_pos[i].set(-1.5f, 0.0f, -1.0f);
		npc_rot[i].set(0.0f, 130.0f, 0.0f);

		npc_attach[i] = MV1AttachAnim(npc_model[i], 1, punch_anim_model, FALSE);
		npc_total[i] = MV1GetAttachAnimTotalTime(npc_model[i], npc_attach[i]);
		npc_frame[i] = 0.0f;

		npc_image_num[i] = 0;
		MV1SetTextureGraphHandle(npc_model[i], 2, sample_image[npc_image_num[i]], TRUE);
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		for (int i = 0; i < NPC_MAX; i++)
		{

			npc_image_num[i] += GetRandomI(1, IMAGE_MAX - 1);
			npc_image_num[i] %= IMAGE_MAX;

			MV1SetTextureGraphHandle(npc_model[i], 2, sample_image[npc_image_num[i]], TRUE);
		}
	}

	//	アニメーションの再生
	player_frame += 1.0f;
	if (player_frame >= player_total) {
		player_frame = 0.0f;
	}
	//	そのフレームのアニメーションにする
	MV1SetAttachAnimTime(player_model, player_attach, player_frame);

	for (int i = 0; i < NPC_MAX; i++)
	{
		//	アニメーションの再生
		npc_frame[i] += 1.0f;
		if (npc_frame[i] >= npc_total[i]) {
			npc_frame[i] = 0.0f;
		}
		//	そのフレームのアニメーションにする
		MV1SetAttachAnimTime(npc_model[i], npc_attach[i], npc_frame[i]);
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel(ground_model);

	//	ＮＰＣモデルの描画
	for (int i = 0; i < NPC_MAX; i++)
	{
		MV1SetPosition(npc_model[i], VGet(npc_pos[i].x, npc_pos[i].y, npc_pos[i].z));
		MV1SetRotationXYZ(npc_model[i], VGet(TO_RADIAN(npc_rot[i].x), TO_RADIAN(npc_rot[i].y + 180.0f), TO_RADIAN(npc_rot[i].z)));
		MV1SetScale(npc_model[i], VGet(0.01f, 0.01f, 0.01f));
		MV1DrawModel(npc_model[i]);
	}

	//	プレイヤーモデルの描画
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	MV1DrawModel(player_model);

	DrawString(16, 16, "ＮＰＣもプレイヤーと同じモデルを使用（別モデルとして・テクスチャも別で開始）", GetColor(255, 255, 0));
	DrawString(16, 32, "→　パンチアニメーションの再生を繰り返す", GetColor(255, 255, 0));
	DrawString(16, 48, "スペース：ＮＰＣモデルのテクスチャ切り替え（ランダムに０～５：同じテクスチャにはならない）", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	for (int i = 0; i < NPC_MAX; i++)
	{
		MV1DeleteModel(npc_model[i]);
	}

	//	アニメーションデータの終了処理
	MV1DeleteModel(anim_model);
}
