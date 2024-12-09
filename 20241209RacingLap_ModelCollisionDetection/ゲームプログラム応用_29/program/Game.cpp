#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

#define PLAYER_MOV_SPEED	0.2f
#define PLAYER_ROT_SPEED	2.0f

//	モデルデータ用の変数
int ground_model;
int player_model;

//	プレイヤーの座標や向き
Float3 player_pos;
Float3 player_rot;

bool on_road;

int point_num;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 20.0f, -40.0f ), VGet( 0.0f, 0.0f, -10.0f ) );

	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );

	MV1SetupCollInfo( ground_model );

	player_pos.set( -15.0f, 0.0f, -15.0f );
	player_rot.clear();
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	MV1RefreshCollInfo( ground_model );

	//	左右キーで回転
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで向いている方向に移動
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		player_pos += mov;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	if( CheckHitKey( KEY_INPUT_SPACE ) ){
		MV1SetFrameVisible(ground_model, 2, TRUE);
	}
	else{
		MV1SetFrameVisible(ground_model, 2, FALSE);
	}
	MV1DrawModel( ground_model );

	MV1SetPosition( player_model, player_pos.VGet() );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	DrawString( 16, 16, "十字キー：プレイヤー回転・移動", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "スペース：ポイントの描画", GetColor( 255, 255, 255 ) );

	if( on_road )
	{
		DrawString( 16, 48, "プレイヤーはコース上にいます", GetColor( 255, 255, 0 ) );
	}else
	{
		DrawString( 16, 48, "コースアウトしています", GetColor( 0, 255, 0 ) );
	}

	if( point_num >= 0 ){
		DrawFormatString( 16, 64, GetColor( 0, 255, 255 ), "ポイント %d にいます", point_num );
	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
}
