#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"

//	スピードの変更量
#define PLAYER_ACCEL		0.001f
//	プレイヤーの最大スピード
#define PLAYER_MOV_SPEED	0.2f
//	向きの変更スピード
#define PLAYER_ROT_SPEED	1.0f

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;

//	カメラの座標
Float3 camera_pos;
Float3 camera_look;
//	プレイヤーの座標や向き
Float3 player_pos;
Float3 player_rot;
//	プレイヤースピード
float player_speed;

//	ＮＰＣの座標や向き
Float3 npc_pos;
Float3 npc_rot;

//	レンダーターゲットの大きさ
#define MAP_W		200
#define MAP_H		200
//	レンダーターゲット画像用の変数
int map_image;

#define FRONT_W 160
#define FRONT_H 90
int front_image;

//	スピードメータ用の定数
#define CENTER_X	150.0f		//	円の中心Ｘ座標（サンプルの四角メーターと重ならないように「こういう感じで.exe」より少し右にしています）
#define CENTER_Y	380.0f		//	Ｙ座標
#define RADIUS		50.0f		//	半径

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	レンダーターゲットの作成
	map_image = MakeScreen(MAP_W, MAP_H);
	front_image = MakeScreen(FRONT_W, FRONT_H);

	//	カメラ設定（すぐにプレイヤーを見るカメラになりますがとりあえず）
	camera_pos.set(0.0f, 7.0f, -15.0f);
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());
	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");
	//	プレイヤーの初期設定
	player_pos.set(0.0f, 0.0f, -5.0f);
	player_rot.y = 0.0f;
	//	スピードは０で開始
	player_speed = 0.0f;

	npc_pos.set(-5.0f, 0.0f, 5.0f);
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	プレイヤーの回転と移動
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーを押したらスピードを上げていく
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_speed += PLAYER_ACCEL;
	}
	else {	//	じゃないとき（上キーを押していないとき）
		player_speed -= PLAYER_ACCEL;
	}
	//	プレイヤースピードを 0.0f ～ PLAYER_MOV_SPEED にする
	player_speed = max(0.0f, min(player_speed, PLAYER_MOV_SPEED));

	//	キーを押しているいないに関わらずスピードがあったら移動する
	//	プレイヤーが向いている角度に回転させるための回転行列作成
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	//	基準の移動ベクトル
	Float3 base(0.0f, 0.0f, player_speed);
	//	この基準ベクトルを回転行列で変換
	Float3 mov = GetFloat3VTransform(base, mat);
	//	この移動ベクトルを使って座標移動
	player_pos += mov;

	//	ＮＰＣの回転
	npc_rot.y += 0.5f;

	//	見る座標はプレイヤー座標（の少し上）
	camera_look = player_pos + Float3(0.0f, 0.5f, 0.0f);
	//	カメラ座標は見る座標から少し離れたところ
	camera_pos = camera_look + Float3(0.0f, 3.0f, -7.0f);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	描画を何回かおこないますので、座標や向きの設定はここでまとめてやっておきます
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	//	ＮＰＣも
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));

	SetDrawScreen(front_image);		//	初期化で用意した画像
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);				//	カメラの距離
	SetupCamera_Perspective(TO_RADIAN(45.0f));	//	カメラの画角

	Float3 look = player_pos + Float3(0.0f, 0.5f, 0.0f);

	MATRIX mat = MGetRotY(TO_RADIAN(45.0f));
	Float3 base(0.0f, 0.5f, 3.0f);
	Float3 mov = GetFloat3VTransform(base, mat);
	Float3 pos = look + mov;
	SetCameraPositionAndTarget_UpVecY(pos.VGet(), look.VGet());

	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	SetDrawScreen(map_image);		//	初期化で用意した画像
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);				//	カメラの距離
	SetupCamera_Perspective(TO_RADIAN(45.0f));	//	カメラの画角
	Float3 pos2 = player_pos + Float3(0.0f, 100.0f, 0.0f);
	SetCameraPositionAndAngle(pos2.VGet(), TO_RADIAN(90.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f));

	//	このままだとプレイヤーやＮＰＣがめっちゃ小さく描画されてしまう
	//	なので、この２つのモデルを大きくします
	MV1SetScale(player_model, VGet(10.0f, 10.0f, 10.0f));
	MV1SetScale(npc_model, VGet(10.0f, 10.0f, 10.0f));
	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	SetDrawScreen(DX_SCREEN_BACK);	//	画面に描画
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	いつもの画面描画をするときはプレイヤーやＮＰＣのモデルは通常の大きさにする
	MV1SetScale(player_model, VGet(1.0f, 1.0f, 1.0f));
	MV1SetScale(npc_model, VGet(1.0f, 1.0f, 1.0f));

	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	//	全体マップ画像の描画
	DrawGraphF(580.0f, 230.0f, map_image, TRUE);
	//	その画像を囲む白い枠の描画（線の太さを 5.0f にしています）
	DrawBoxAA(580.0f, 230.0f, 580.0f + MAP_W, 230.0f + MAP_H, GetColor(255, 255, 255), FALSE, 5.0f);

	DrawGraphF(620.0f, 20.0f, front_image, TRUE);
	DrawBoxAA(620.0f, 20.0f, 620.0f + FRONT_W, 20.0f + FRONT_H, GetColor(255, 255, 255), FALSE, 5.0f);

	//	プレイヤーのスピードに合わせて黄色い四角の描画
	//	player_speed：0.0f ～ 0.2f
	//	今のスピードが PLAYER_MOV_SPEED を最大としたときにどのくらいの割合か（ 0.0f ～ 1.0f ）
	float rate = player_speed / PLAYER_MOV_SPEED;
	float length = 100.0f * rate;
	DrawBoxAA(20.0f, 430.0f - length, 70.0f, 430.0f, GetColor(255, 255, 0), TRUE);
	DrawBoxAA(20.0f, 330.0f, 70.0f, 430.0f, GetColor(255, 255, 255), FALSE, 5.0f);

	int count = 360.0f * rate;
	for (int i = 0; i < count ; i++)
	{
		float rot1 = i - 90.0f;
		float x1 = CENTER_X + RADIUS * cosf(TO_RADIAN(rot1));
		float y1 = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot1));

		float rot2 = rot1 + 1.0f;
		float x2 = CENTER_X + RADIUS * cosf(TO_RADIAN(rot2));
		float y2 = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot2));

		DrawTriangle(CENTER_X, CENTER_Y ,x1,y1,x2,y2,GetColor(0,255,255), TRUE);
	}

	//
	float rot = 360.0f * rate - 90.0f;

	float x = CENTER_X + RADIUS * cosf(TO_RADIAN(rot));
	float y = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot));

	DrawLineAA(CENTER_X, CENTER_Y, x,y, GetColor(255, 255, 255), 5.0f);
	//	メーター枠の円の描画
	DrawCircleAA(CENTER_X, CENTER_Y, RADIUS, 100, GetColor(255, 255, 255), FALSE, 5.0f);

	DrawString(16, 16, "左右キー：プレイヤーの回転", GetColor(255, 255, 255));
	DrawString(16, 32, "上キー　：プレイヤーの移動スピードを増やす（押してなかったら減らす）", GetColor(255, 255, 255));
	DrawString(16, 48, "→　向いている方向にそのスピードで移動", GetColor(255, 255, 255));
	DrawString(16, 64, "カメラはプレイヤーを見る", GetColor(255, 255, 255));
	DrawString(16, 80, "画面右下に全体が見えるようなカメラの画像", GetColor(255, 255, 255));
	DrawString(16, 96, "画面右上にプレイヤーを正面から見たカメラの画像（バックミラーみたいな）", GetColor(255, 255, 0));

	DrawFormatString(16, 290, GetColor(255, 255, 0), "スピード：%.3f（メーターを円形に）", player_speed);
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	レンダーターゲットの終了処理
	DeleteGraph(map_image);
}
