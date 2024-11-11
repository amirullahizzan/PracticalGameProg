#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	0.1f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	3.0f

//	モデル用の変数
int ground_model;
int player_model;
int npc_model;

//	プレイヤーの座標や向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣの座標や向き
Float3 npc_pos;
Float3 npc_rot;


int text_image;
const char* sample_text = "文字列と角度を描画するだけの別画面を画像して画面下に描画";
float text_x;

//	画面の描画する画像サイズ
#define TEXT_SCREEN_W	400
#define TEXT_SCREEN_H	50

#define MINI_SCREEN_W	160
#define MINI_SCREEN_H	 90

//	１：３Ｄ描画を書き込むためのテクスチャを用意
int screen_image;


Float2 image_pos;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	１：描画状態のものを描きこむための画像を作成
	screen_image = MakeScreen(MINI_SCREEN_W, MINI_SCREEN_H);
	text_image = MakeScreen(TEXT_SCREEN_W, TEXT_SCREEN_H);

	//	カメラの設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 3.5f, -15.0f), VGet(0.0f, 0.0f, 0.0f));
	//	モデルの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	player_pos.set(5.0f, 0.0f, 0.0f);
	player_rot.y = 0.0f;

	npc_pos.set(-5.0f, 0.0f, 5.0f);
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで座標移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	npc_rot.y += 0.5f;

	text_x -= 2;

	if (text_x <= -500)
	{
		text_x = TEXT_SCREEN_W;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	SetDrawScreen(text_image);
	ClearDrawScreen();

	DrawFillBox(0,0,TEXT_SCREEN_W, TEXT_SCREEN_H, GetColor(0,0,255));
	DrawString(text_x, 17, sample_text, GetColor(0,255,255));

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	//	プレイヤーも
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));

	SetDrawScreen(screen_image);
	ClearDrawScreen();
	//	２－２：レンダーターゲットを切り替えたのでカメラ設定
	//	カメラが描画できる距離
	SetCameraNearFar(0.1f, 3000.0f);
	//	カメラの画角
	SetupCamera_Perspective(TO_RADIAN(45.0f));

	Float3 pos = player_pos + Float3(0.0f, 1.0f, -3.0f);
	Float3 look = player_pos + Float3(0.0f, 0.5f, 0.0f);

	SetCameraPositionAndTarget_UpVecY(pos.VGet(), look.VGet());

	MV1DrawModel(ground_model);	//	地面モデルの描画
	MV1DrawModel(npc_model);		//	ＮＰＣモデルの描画
	MV1DrawModel(player_model);	//	プレイヤーモデルの描画

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 3.5f, -15.0f), VGet(0.0f, 0.0f, 0.0f));

	MV1DrawModel(ground_model);	//	地面モデルの描画
	MV1DrawModel(npc_model);		//	ＮＰＣモデルの描画
	MV1DrawModel(player_model);	//	プレイヤーモデルの描画

	VECTOR pos2d = ConvWorldPosToScreenPos(player_pos.VGet());

	if (pos2d.x > SCREEN_W / 2.0f)
	{
		image_pos.x += 10.0f;
	}
	else
	{
		image_pos.x -= 10.0f;
	}

	image_pos.x = max(-170.0f, min(image_pos.x, 10.0f));

	DrawGraph(image_pos.x, image_pos.y, screen_image, TRUE);

	DrawGraph(image_pos.x, image_pos.y, screen_image, TRUE);
	DrawLineBox(image_pos.x, image_pos.y, image_pos.x + MINI_SCREEN_W, image_pos.y + MINI_SCREEN_H, GetColor(255, 255, 255));

	DrawGraph(image_pos.x + SCREEN_W, image_pos.y, screen_image, TRUE);
	DrawLineBox(image_pos.x + SCREEN_W, image_pos.y, image_pos.x + MINI_SCREEN_W, image_pos.y + MINI_SCREEN_H, GetColor(255, 255, 255));

	DrawString(16, 16, "十字キー：プレイヤーの回転と移動", GetColor(255, 255, 255));
	DrawString(16, 32, "小さい画面のカメラをプレイヤーを見るカメラにする", GetColor(255, 255, 0));
	DrawString(16, 48, "プレイヤーの画面位置（左か右）によって小さい画面の表示場所を変更（右下か左下）", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	描き込み用に用意した画像の終了処理
	DeleteGraph(screen_image);
}
