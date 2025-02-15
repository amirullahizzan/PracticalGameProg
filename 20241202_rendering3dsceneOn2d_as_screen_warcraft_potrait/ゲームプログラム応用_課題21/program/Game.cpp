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

//	スクリーンショットの枚数
#define SHOT_MAX	3

//	スクリーンショットを表示する大きさ
#define SHOT_W	240
#define SHOT_H	135
//	レンダーターゲット用の画像
int shot_image;
//	その画像の表示座標
Float2 shot_pos;

//	スクリーンショットをとるフラグ
bool screen_shot;
//	「保存しました」の文字列を描画する時間
int save_frame;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	レンダーターゲットの作成
	//	（画面サイズと同じ画像として作成）
	shot_image = MakeScreen( SCREEN_W, SCREEN_H );
	//	座標の設定
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

	//	スペースキーを押したとき
	if( PushHitKey( KEY_INPUT_SPACE ) ){
		//	スクリーンショットをレンダーターゲットに描きこむためのフラグを立てる
		screen_shot = true;
	}

	//	表示時間を減らしていく
	save_frame--;
	save_frame = max( 0, save_frame );

	//	今回は画像をクリックしたらその画像の保存を開始します
	if( PushMouseInput() ){
		//	１秒間「保存しました」を表示したいので
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

	//	スペースキーを押したタイミングの時だけ
	if( screen_shot ){
		//	描き込みましたのでフラグを落とす
		screen_shot = false;

		//	レンダーターゲットを切り替えて描き込む
		SetDrawScreen( shot_image );
		//	画面のクリア
		ClearDrawScreen();
		//	カメラの設定（今回は通常のカメラと同じものを使います）
		SetCameraNearFar( 0.1f, 3000.0f );
		SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
		SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
		//	モデルの描画
		MV1DrawModel( ground_model );
		MV1DrawModel( npc_model );
		MV1DrawModel( player_model );
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

	//	スクリーンショット画像の描画座標
	//	左上の座標
	float x1 = shot_pos.x;
	float y1 = shot_pos.y;
	//	右下の座標
	float x2 = x1 + SHOT_W;
	float y2 = y1 + SHOT_H;
	//	枠の座標に合わせてスクリーンショット画像を描画
	DrawExtendGraphF( x1, y1, x2, y2, shot_image, TRUE );
	//	この座標を使って四角の枠の描画
	DrawBoxAA( x1, y1, x2, y2, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "スペースキー：スクリーンショット（３つ作成）", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "１：現状のカメラ", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "２：ＮＰＣ側からプレイヤーを見るカメラ", GetColor( 255, 255, 0 ) );
	DrawString( 16, 64, "３：プレイヤーを近くから見るカメラ", GetColor( 255, 255, 0 ) );

	if( save_frame > 0 ){
		//	表示時間があるときだけ文字列の描画
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
