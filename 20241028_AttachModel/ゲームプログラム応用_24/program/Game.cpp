#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	移動スピード
#define PLAYER_MOV_SPEED	0.025f
//	回転スピード
#define PLAYER_ROT_SPEED	2.0f

//	モデルデータ用の変数
int ground_model;
int player_model;
int npc_model;
int weapon_model;

//	カメラ座標と見る座標
Float3 camera_pos;
Float3 camera_look;

//	アニメーションデータ
int anim_model;
//	アタッチ番号
int anim_attach;
//	アニメーション１ループの時間
float anim_total;
//	アニメーションをどのくらい進めるか
float anim_time;

//	プレイヤー座標と向き
Float3 player_pos;
Float3 player_rot;
//	ＮＰＣ座標と向き
Float3 npc_pos;
Float3 npc_rot;

Float3 ball_pos;


MATRIX model_mat;
//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラ座標と見る座標の初期設定
	camera_pos.set( 0.0f, 2.0f, -5.0f );
	camera_look.set( 0.0f, 1.0f, 0.0f );

	//	モデルデータの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	weapon_model = MV1LoadModel( "data/weapon.mqoz" );
	

	//	アニメーションデータの読み込み
	anim_model = MV1LoadModel( "data/player/anim_walk.mv1" );

	//	初期座標や向きの設定
	player_pos.set( 2.0f, 0.0f, 0.0f );
	player_rot.y = -90.0f;

	npc_pos.set( -2.0f, 0.0f, 2.0f );

	//	アニメーションの初期化
	anim_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	anim_total = MV1GetAttachAnimTotalTime( player_model, anim_attach );
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	左右キーで向きの変更
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーを押していたら向いている方向に移動
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		player_pos += mov;
	}

	//	アニメーションの時間を進める
	anim_time += 1.0f;
	if( anim_time >= anim_total ){
		anim_time = 0.0f;
	}
	//	その時間のアニメーションにする
	MV1SetAttachAnimTime( player_model, anim_attach, anim_time );

	npc_rot.y += 0.5f;

	int frame = MV1SearchFrame(player_model, "mixamorig:RightHandIndex1");
	MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, frame);

	Float3 base(0.0f, 0.0f, 0.0f);

	ball_pos = GetFloat3VTransform(base,mat);

	model_mat = MV1GetFrameLocalWorldMatrix(player_model, frame);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	カメラ座標と見る座標を渡してカメラの設定
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	地面モデルの描画
	MV1DrawModel( ground_model );

	//	ＮＰＣモデルの描画
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	プレイヤーモデルの描画
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	MV1DrawModel( player_model );

	//Setting the weapon model into the Player's frame model
	MV1SetMatrix(weapon_model, model_mat);
	
	//VECTOR scale = VECTOR{ 0.0001f, 0.0001f, 0.0001f };
	//MV1SetScale(weapon_model, scale); //Is not possible because
	MV1DrawModel( weapon_model );

	DrawSphere3D( ball_pos.VGet(), 0.1f, 10, GetColor( 255, 255, 0 ), GetColor( 255, 255, 0 ), TRUE );

	DrawString( 16, 16, "十字キー：プレイヤーの回転・移動", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "プレイヤーの右手に球の描画", GetColor( 255, 255, 255 ) );
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
