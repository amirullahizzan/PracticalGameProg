#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"
#include "Hit.h"

//	モデルデータ
int ground_model;
int player_model;
int npc_model;

//	アニメーション関係の変数
int anim_model;		//	アニメーションデータ用
int anim_attach;	//	アタッチ番号
float anim_total;	//	アニメーション総フレーム数
float anim_frame;	//	再生フレーム

//	カメラ座標と見る座標
Float3 camera_pos;
Float3 camera_look;
//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

#define SHOT_W	240
#define SHOT_H	135

int shot_image;
Float2 shot_pos;

bool screen_shot;
int save_frame;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//shot_image = MakeScreen( SHOT_W, SHOT_H );
	shot_image = MakeScreen( SCREEN_W, SCREEN_H);

	shot_pos.x = 20.0f;
	shot_pos.y = 300.0f;

	//	カメラの設定
	camera_pos.set( 0.0f, 1.35f, -3.0f );
	camera_look.set( 0.0f, 0.75f, 0.0f );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	モデルデータの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );
	//	アニメーションデータの読み込み
	anim_model = MV1LoadModel( "data/player/anim_punch.mv1" );

	//	初期座標や向きの設定
	player_pos.set( 1.0f, 0.0f, 0.0f );
	player_rot.y = 230.0f;

	npc_pos.set( -2.0f, 0.0f, 2.0f );
	npc_rot.y = 90.0f;

	//	アタッチ番号取得
	anim_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	//	総フレーム数取得
	anim_total = MV1GetAttachAnimTotalTime( player_model, anim_attach );
	//	０フレームから開始
	anim_frame = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	アニメーションフレームを進める
	anim_frame += 1.0f;
	//	最後まで行ったら最初から
	if( anim_frame >= anim_total ){
		anim_frame = 0.0f;
	}
	//	そのフレームでアニメ再生
	MV1SetAttachAnimTime( player_model, anim_attach, anim_frame );

	//	ＮＰＣの座標移動
	npc_pos.x += 0.05f;
	if( npc_pos.x > 5.0f ){
		npc_pos.x = -5.0f;
	}

	if( PushHitKey( KEY_INPUT_SPACE ) )
	{
		screen_shot = true;
	}

	if( PushHitKey( KEY_INPUT_RETURN ) )
	{
		save_frame = 60;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	プレイヤーを何回か描画するので座標や向きの設定はここでまとめてやっておきます
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	//	ＮＰＣも
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );

	if (screen_shot)
	{
		screen_shot = false;

		SetDrawScreen(shot_image);
		ClearDrawScreen();

		SetCameraNearFar(0.1f,3000.0f);
		SetupCamera_Perspective(TO_RADIAN(45.0f));
		SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(),camera_look.VGet());

		MV1DrawModel(ground_model);
		MV1DrawModel(npc_model);
		MV1DrawModel(player_model);
	}

	//	レンダーターゲットを画面に戻す
	SetDrawScreen( DX_SCREEN_BACK );
	ClearDrawScreen();
	//	カメラの設定
	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
	//	モデルの描画
	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	if (save_frame == 60)
	{
		SaveDrawScreen(0,0,SCREEN_W,SCREEN_H,"data/sample1.bmp");
	}

	float x1 = shot_pos.x;
	float y1 = shot_pos.y;
	DrawGraphF( x1, y1, shot_image, TRUE );

	float x2 = x1 + SHOT_W;
	float y2 = y1 + SHOT_H;
	DrawBoxAA( x1, y1, x2, y2, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "スペースキー：スクリーンショット", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "エンターキー：画面の描画状態を画像にして保存", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "→　data フォルダに sample.bmp というファイルで保存します", GetColor( 255, 255, 255 ) );

	if( save_frame > 0 ){
		DrawString( 350, 200, "保存しました", GetColor( 255, 255, 0 ) );
	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	MV1DeleteModel( anim_model );

	DeleteGraph( shot_image );
}
