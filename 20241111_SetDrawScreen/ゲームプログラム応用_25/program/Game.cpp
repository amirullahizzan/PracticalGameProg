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

#define MINI_SCREEN_W 160
#define MINI_SCREEN_H 90

int screen_image;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	screen_image = MakeScreen(MINI_SCREEN_W, MINI_SCREEN_H);
	//	カメラの設定
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 3.5f, -15.0f ), VGet( 0.0f, 0.0f, 0.0f ) );
	//	モデルの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	player_pos.set( 5.0f, 0.0f, 0.0f );
	player_rot.y = 0.0f;

	npc_pos.set( -5.0f, 0.0f, 5.0f );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	左右キーで向きの変更
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで座標移動
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		player_pos += mov;
	}

	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	SetDrawScreen(screen_image);

	ClearDrawScreen();

	SetCameraNearFar(0.1f , 3000.0f);

	SetupCamera_Perspective(TO_RADIAN(45.0f));

	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1.0f, -3.0f), VGet(0.0f, 0.0f, 0.0f));

	//	３Ｄモデルの描画
	MV1DrawModel( ground_model );

	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));

	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1.0f, -3.0f), VGet(0.0f, 0.0f, 0.0f));

	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	DrawGraph(600,20,screen_image, TRUE);

	DrawLineBox(600,20, 600 + MINI_SCREEN_W, 20 + MINI_SCREEN_H, GetColor(255,255,255));

	DrawString( 16, 16, "十字キー：プレイヤーの回転と移動", GetColor( 255, 255, 255 ) );
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
