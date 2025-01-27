#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

//	プレイヤーの移動スピード（課題用に少し変更しています）
#define PLAYER_MOV_SPEED	0.1f
//	プレイヤーの回転スピード（課題用に少し変更しています）
#define PLAYER_ROT_SPEED	0.35f
//	ＮＰＣの回転スピード
#define NPC_ROT_SPEED		0.5f

//	モデル用の変数
int ground_model;
int player_model;
int npc_model;

int chara_model;
int anim_model;
int anim_attach;
float anim_total;
float anim_time;

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

//	ライトの方向（この方向に影が出来る）
Float3 light_dir;
//	シャドウマップハンドル用の変数
int shadow_handle;

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

	//	０：ライトの方向設定
	light_dir.set( 1.5f, -1.2f, 1.3f );
	//	この方向を渡してライトの設定
	SetLightDirection( light_dir.VGet() );

	//	１：シャドウマップハンドルの作成
	shadow_handle = MakeShadowMap( 2048, 2048 );
	//	２：シャドウマップが想定するライトの方向をセット
	SetShadowMapLightDirection( shadow_handle, light_dir.VGet() );
	//	３：シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea( shadow_handle, VGet( -10.0f, -1.0f, -10.0f ), VGet( 10.0f, 10.0f, 10.0f ) );

	chara_model = MV1LoadModel("data/chara/model.mv1");
	anim_model = MV1LoadModel("data/chara/anim_walk.mv1");
	anim_attach = MV1AttachAnim(chara_model, 1 ,anim_model,FALSE);
	anim_total = MV1GetAttachAnimTotalTime(chara_model,anim_attach);
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	anim_time += 1.0F;
	if (anim_time >= anim_total)
	{
		anim_time = 0.0f;
	}

	//	プレイヤーの回転
	player_rot.y += PLAYER_ROT_SPEED;
	//	向いている方向に移動
	Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
	Float3 mov = GetFloat3VTransform( base, mat );
	player_pos += mov;

	//	プレイヤーを見るカメラ
	//	見る座標はプレイヤー座標
	camera_look = player_pos;
	//	カメラ座標は見る座標あら一定の所
	camera_pos = camera_look + Float3( 0.0f, 7.0f, -10.0f );

	//	ＮＰＣの回転
	npc_rot.y += NPC_ROT_SPEED;

	//	十字キーでライト方向の変更
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	light_dir.x += 0.1f;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		light_dir.x -= 0.1f;
	if( CheckHitKey( KEY_INPUT_UP ) )		light_dir.z += 0.1f;
	if( CheckHitKey( KEY_INPUT_DOWN ) )		light_dir.z -= 0.1f;

	SetShadowMapLightDirection(shadow_handle, light_dir.VGet());

	Float3 pos1 = camera_look + Float3(-20.0f,-1.0f,-20.0f);
	Float3 pos2 = camera_look + Float3(20.0f, 10.0f,20.0f);

	SetShadowMapDrawArea(shadow_handle, pos1.VGet(), pos2.VGet());
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//Toggles between shadow and object
	ShadowMap_DrawSetup( shadow_handle );

	MV1DrawModel( ground_model );
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		MV1SetPosition(chara_model, VGet(player_pos.x,player_pos.y, player_pos.z));
		MV1SetRotationXYZ(chara_model,VGet(TO_RADIAN(player_pos.x),TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
		MV1SetScale(chara_model,VGet(0.01f,0.01f,0.01f));
		MV1DrawModel(chara_model);
	}
	else
	{
		MV1DrawModel(player_model);
	}

	//Toggles between shadow and object
	ShadowMap_DrawEnd();

	SetUseShadowMap( 0, shadow_handle );

	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	SetUseShadowMap( 0, -1 );

	DrawString( 16, 16, "十字キー：ライト方向の変更（ＸＺ方向）", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 32, GetColor( 255, 255, 255 ), "X:%5.2f Y:%5.2f Z:%5.2f", light_dir.x, light_dir.y, light_dir.z );

	DrawString( 16, 48, "ライトの方向に合わせて影の方向も変更", GetColor( 255, 255, 0 ) );
	DrawString( 16, 64, "プレイヤーの移動に合わせて影の描画領域も変更（原点から離れても影が消えないように）", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	１０：シャドウマップの削除
	DeleteShadowMap( shadow_handle );
}
