#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"

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

float player_speed;

//	ＮＰＣの座標や向き
Float3 npc_pos;
Float3 npc_rot;

#define MAP_W		200
#define MAP_H		200

int map_image;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	map_image = MakeScreen( MAP_W, MAP_H );

	//	カメラ設定
	camera_pos.set( 0.0f, 7.0f, -15.0f );
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
	//	モデルデータの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );
	//	プレイヤーの初期設定
	player_pos.set( 0.0f, 0.0f, -5.0f );
	player_rot.y = 0.0f;

	player_speed = 0.0f;

	npc_pos.set( -5.0f, 0.0f, 5.0f );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	プレイヤーの回転と移動
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;

	if( CheckHitKey( KEY_INPUT_UP ) ){
	}

	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );

	//	ＮＰＣの回転
	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );

	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );

	SetDrawScreen( DX_SCREEN_BACK );
	ClearDrawScreen();
	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	モデルの描画
	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	DrawGraphF( 580.0f, 230.0f, map_image, TRUE );
	DrawBoxAA( 580.0f, 230.0f, 580.0f + MAP_W, 230.0f + MAP_H, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawBoxAA( 20.0f, 330.0f, 70.0f, 430.0f, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "左右キー：プレイヤーの回転", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "上キー　：プレイヤーの移動スピードを増やす（押してなかったら減らす）", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "→　向いている方向にそのスピードで移動", GetColor( 255, 255, 255 ) );
	DrawString( 16, 64, "カメラはプレイヤーを見る", GetColor( 255, 255, 255 ) );
	DrawString( 16, 80, "画面右下に全体が見えるようなカメラの画像", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 290, GetColor( 255, 255, 255 ), "スピード：%.3f", player_speed );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	レンダーターゲットの終了処理
	DeleteGraph( map_image );
}
