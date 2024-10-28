#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	移動スピード
#define PLAYER_MOV_SPEED	0.025f
//	回転スピード
#define PLAYER_ROT_SPEED	2.0f

#define RECORD_MAX 10

	//	線の開始座標
Float3 record_pos[RECORD_MAX];
//	線のゴール座標
Float3 record_goal[RECORD_MAX];

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;

//	カメラ座標と見る座標
Float3 camera_pos;
Float3 camera_look;

//	アニメーションデータ
int anim_model;
//	アタッチ番号
int anim_attach;
//	アニメーション１ループの時間
float anim_total;
//	アニメーションをどのくらい進めるか
float anim_time;

//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

//	ボールの座標（この座標をプレイヤーの右手の座標にします）
Float3 ball_pos;

bool isFPS = false;

int lineBlend[RECORD_MAX];

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の初期設定
	camera_pos.set(0.0f, 2.0f, -5.0f);
	camera_look.set(0.0f, 1.0f, 0.0f);

	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	アニメーションデータの読み込み
	anim_model = MV1LoadModel("data/player/anim_walk.mv1");

	//	初期座標や向きの設定
	player_pos.set(2.0f, 0.0f, 0.0f);
	player_rot.y = -90.0f;

	npc_pos.set(-3.5f, 0.0f, 2.0f);

	//	アニメーションの初期化
	anim_attach = MV1AttachAnim(player_model, 1, anim_model, FALSE);
	anim_total = MV1GetAttachAnimTotalTime(player_model, anim_attach);
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
int lineIndex = 0;

void ProcessLine()
{
	//	プレイヤーの右手にボールを付ける
//	１：右手の場所の名前を確認（ mixamorig:RightHandIndex1：この名前はモデルビューワーなので確認）
//	２：この名前の場所の番号を取得
	int frame = MV1SearchFrame(player_model, "mixamorig:RightHandIndex1");
	//	３：この番号の行列を取得（指定のフレームのローカル座標からワールド座標に変換する行列を得る）
	MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, frame);
	//	４：この行列を使って座標変換したものをボール座標にします
	//	元の座標を用意（原点の座標）
	Float3 base(0.0f, 0.0f, 0.0f); // no need adjustment
	//	この座標を行列で変換してボール座標にします
	ball_pos = GetFloat3VTransform(base, mat);
	record_pos[lineIndex] = ball_pos;

	//////////////GOAL

	// Get the position of the hand in world space
	Float3 goal = Float3(100.0f, 0, 0);
	//X of hand
	Float3 line_goal = GetFloat3VTransform(goal, mat);

	record_goal[lineIndex] = line_goal;

	lineBlend[lineIndex] = 255;
	lineIndex++;

	if (lineIndex >= RECORD_MAX)
	{
		lineIndex = 0;
	}
}

void GameUpdate()
{
	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーを押していたら向いている方向に移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	//	アニメーションの時間を進める
	anim_time += 1.0f;
	if (anim_time >= anim_total)
	{
		anim_time = 0.0f;
	}
	//	その時間のアニメーションにする
	MV1SetAttachAnimTime(player_model, anim_attach, anim_time);

	npc_rot.y += 0.5f;


	ProcessLine();

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		isFPS = !isFPS;
	}

	if (isFPS)
	{
		//	プレイヤーの右手にボールを付ける
//	１：右手の場所の名前を確認（ mixamorig:RightHandIndex1：この名前はモデルビューワーなので確認）
//	２：この名前の場所の番号を取得
		//	３：この番号の行列を取得（指定のフレームのローカル座標からワールド座標に変換する行列を得る）

		int headFrame = MV1SearchFrame(player_model, "mixamorig:Head");
		MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, headFrame);

		Float3 headBase(0.0f, 0.0f, 0.0f);
		camera_pos = GetFloat3VTransform(headBase, mat);

		Float3 headFront(0.0f, 0.0f, 1.0f);
		//camera_look = GetFloat3VTransform(headFront, mat); //if using model eyes look

		mat = MGetRotY(TO_RADIAN(player_rot.y));
		camera_look = camera_pos + GetFloat3VTransform(headFront, mat);
	}
	else
	{
		camera_pos.set(0.0f, 2.0f, -5.0f);
		camera_look.set(0.0f, 1.0f, 0.0f);
	}

}



//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------


void GameRender()
{
	//	カメラ座標と見る座標を渡してカメラの設定
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	地面モデルの描画
	MV1DrawModel(ground_model);

	//	ＮＰＣモデルの描画
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	プレイヤーモデルの描画
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));

	MV1DrawModel(player_model);

	//	黄色いボールの描画
	//DrawSphere3D(head_pos.VGet(), 0.1f, 50, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	DrawSphere3D(ball_pos.VGet(), 0.1f, 10, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	for (int i = 0; i < RECORD_MAX; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, lineBlend[i]);
		DrawLine3D(record_pos[i].VGet(), record_goal[i].VGet(), GetColor(200, 255, 200));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int sub = 255 / RECORD_MAX;
		lineBlend[i] -= sub;
	}

	DrawString(16, 16, "十字キー：プレイヤーの回転・移動", GetColor(255, 255, 255));
	DrawString(16, 32, "プレイヤーの右手に球の描画", GetColor(255, 255, 255));
	DrawString(16, 80, "スペースキーで固定カメラと自分視点でのカメラ切り替え", GetColor(255, 255, 0));
	DrawString(16, 96, "→　自分視点はプレイヤーの頭の座標から正面方向を見る", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	アニメーションデータの終了処理
	MV1DeleteModel(anim_model);
}
