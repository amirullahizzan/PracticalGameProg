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
//	カメラ座標と見る座標
Float3 camera_pos;
Float3 camera_look;

//	プレイヤーの座標や向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣの座標や向き
Float3 npc_pos;
Float3 npc_rot;

//	レンダーターゲットの大きさ
#define MINI_SCREEN_W	100
#define MINI_SCREEN_H	100
//	プレイヤーをアップで描画したものを描き込む用の画像
int player_screen_image;

//	プレイヤーのアップ描画に使う背景画像
int mark_image;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	レンダーターゲット画像の作成
	//	描画するもの（プレイヤーモデル）以外の部分は透明にしたい（引数の最後に TRUE を書きます）
	player_screen_image = MakeScreen(MINI_SCREEN_W, MINI_SCREEN_H, TRUE);

	//	全体カメラの設定
	camera_pos.set(0.0f, 15.0f, -7.0f);
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());
	//	モデルデータの読み込み
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");
	//	目印用の画像の読み込み
	mark_image = LoadGraph("data/mark.bmp");

	//	初期座標や向きの設定
	player_pos.set(3.0f, 0.0f, 0.0f);
	player_rot.y = 0.0f;

	npc_pos.set(-2.0f, 0.0f, 2.5f);
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	左右キーで向きの変更
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで向いている方向に移動
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	//	ＮＰＣは回転だけしています
	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	プレイヤーを何回か描画するので座標や向きの設定はここでまとめてやっておきます
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	//	ＮＰＣも
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));

	//	レンダーターゲットの切り替え
	SetDrawScreen(player_screen_image);
	ClearDrawScreen();
	//	カメラ設定
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	//	カメラ座標と見る座標の設定
	Float3 player_camera_pos = player_pos + Float3(0.0f, 2.0f, -3.0f);
	Float3 player_camera_look = player_pos + Float3(0.0f, 0.25f, 0.0f);
	//	この座標を使ってカメラ設定
	SetCameraPositionAndTarget_UpVecY(player_camera_pos.VGet(), player_camera_look.VGet());
	//	今回のアップ画像はプレイヤーのみ表示できればいいのでプレイヤーのみ描画
	MV1DrawModel(player_model);

	//	レンダーターゲットを元に戻す
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	//	カメラ設定
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	モデルの描画
	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	//	描画するかどうかのフラグ
	bool draw_image = false;

	//	プレイヤー座標（ワールド座標）を２Ｄ画面座標に変換
	VECTOR pos2d = ConvWorldPosToScreenPos(player_pos.VGet());


	//	プレイヤーが画面外にいるときだけ描画



	//	変換した画面座標がカメラの範囲外の時は描画しない
	if (pos2d.z < 0.0f || pos2d.z > 1.0f) 
	{
		draw_image = false;
	}

	if(draw_image)
	{
		float draw_x = pos2d.x;
		float draw_y = pos2d.y;

		float W1 = (MINI_SCREEN_W / 2);
		float W2 = (SCREEN_W - W1);
		float H1 = (MINI_SCREEN_W / 2);
		float H2 = SCREEN_H - H1;

		draw_x = max(W1,min(draw_x, W2));
		draw_y = max(H1,min(draw_y, H2));

#if 1
		Float2 center(SCREEN_W / 2.0f, SCREEN_H/ 2.0f);

		Float2 ppos(pos2d.x, pos2d.y);
		Float2 dir = ppos - center;

		float rate_x = 1.0f;
		float rate_y = 1.0f;

		if (ppos.x < W1) rate_x = (W1 - center.x) / (ppos.x - center.x);
		if (ppos.x > W2) rate_x = (W2 - center.x) / (ppos.x - center.x);

		if (ppos.y < H1) rate_y = (H1 - center.x) / (ppos.y - center.y);
		if (ppos.y > H2) rate_y = (H2 - center.x) / (ppos.y - center.y);

		float rate = min(rate_x,rate_y);

		Float2 draw = center + dir * rate;

		draw_x = draw.x;
		draw_y = draw.y;
#endif
		float rot = 0.0f;

		float x = pos2d.x - draw_x;
		float y = pos2d.y - draw_y;

		rot = TO_DEGREE(atan2f(y, x));

		//	目印画像（ mark_image ）の描画
		DrawRotaGraphF(draw_x, draw_y, 1.0f, TO_RADIAN(rot), mark_image, TRUE);
		//	この場所に画像を描画
		DrawRotaGraphF(draw_x, draw_y, 1.0f, TO_RADIAN(0.0f), player_screen_image, TRUE);
	}

	DrawString(16, 16, "十字キー：プレイヤーの回転・移動", GetColor(255, 255, 255));
	DrawString(16, 32, "プレイヤーが画面外に出ると別画面の表示", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	DeleteGraph(mark_image);

	//	レンダーターゲット画像の終了処理
	DeleteGraph(player_screen_image);
}
