#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;

//	アニメーションデータ用の変数
int anim_model;

Float3	player_pos;
Float3	player_rot;
int player_attach;
float player_total;
float player_frame;

Float3	npc_pos;
Float3	npc_rot;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ設定
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 2.0f, -5.0f ), VGet( 0.0f, 1.0f, -1.0f ) );

	//	モデルデータの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/chara/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	アニメーションデータの読み込み
	anim_model = MV1LoadModel( "data/chara/anim_walk.mv1" );

	player_pos.set( 1.5f, 0.0f, -1.0f );
	player_rot.y = 230.0f;

	//	プレイヤーアニメーションの初期設定
	player_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	player_total = MV1GetAttachAnimTotalTime( player_model, player_attach );
	player_frame = 0.0f;

	npc_pos.set( -1.5f, 0.0f, -1.0f );
	npc_rot.set( 0.0f, 130.0f, 0.0f );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	アニメーションの再生
	player_frame += 1.0f;
	if( player_frame >= player_total ){
		player_frame = 0.0f;
	}
	//	そのフレームのアニメーションにする
	MV1SetAttachAnimTime( player_model, player_attach, player_frame );

}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel( ground_model );

	//	ＮＰＣモデルの描画
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y + 180.0f ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	プレイヤーモデルの描画
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	MV1DrawModel( player_model );

	DrawString( 16, 16, "ＮＰＣモデルはプレイヤーモデルの複製を色変えしたもの", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	アニメーションデータの終了処理
	MV1DeleteModel( anim_model );
}
