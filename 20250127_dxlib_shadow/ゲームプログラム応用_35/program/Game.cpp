#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	0.03f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	1.0f
//	ＮＰＣの回転スピード
#define NPC_ROT_SPEED		0.5f

//	モデル用の変数
int ground_model;
int player_model;
int npc_model;

//	カメラ座標
Float3 camera_pos;
//	見る座標
Float3 camera_look;

//	プレイヤー座標
Float3 player_pos;
//	プレイヤー向き
Float3 player_rot;

//	ＮＰＣ座標
Float3 npc_pos;
//	ＮＰＣ向き
Float3 npc_rot;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	モデルの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	カメラの初期設定
	camera_pos.set( 0.0f, 3.0f, -5.0f );
	camera_look.set( 0.0f, 0.0f, 0.0f );

	//	プレイヤーの初期設定
	player_pos.set( 2.5f, 0.0f, 0.0f );
	player_rot.y = 180.0f;

	//	ＮＰＣの初期設定
	npc_pos.set( -2.5f, 0.0f, 2.0f );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	プレイヤーの回転
	player_rot.y += PLAYER_ROT_SPEED;
	//	向いている方向に移動
	Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
	Float3 mov = GetFloat3VTransform( base, mat );
	player_pos += mov;
	//	ＮＰＣの回転
	npc_rot.y += NPC_ROT_SPEED;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	カメラ座標と見る座標を渡してカメラ設定
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	地面モデルの描画
	MV1DrawModel( ground_model );
	//	座標や向きを設定してＮＰＣモデルの描画
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );
	//	座標や向きを設定してプレイヤーモデルの描画
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );
}
