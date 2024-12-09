#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

#include "Camera.h"
#include "Ground.h"
#include "Player.h"

//	モデルデータ用の変数
int ground_model;
int player_model;

//	クラスの宣言
Camera	camera;
Ground	ground;
Player	player;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	モデルデータの読み込み
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );

	//	クラスの初期化
	camera.Init();
	ground.Init( ground_model );
	player.Init( player_model );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	ground.Update();

	//	プレイヤーの更新処理
	player.Update();

	camera.Update(player.m_pos, player.m_rot);
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	カメラの設定
	camera.Render();
	//	モデルの描画
	ground.Render();
	player.Render();

	DrawString( 16, 16, "左右キー：プレイヤーの回転", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "カメラをプレイヤーの後ろに", GetColor( 255, 255, 0 ) );
	DrawString( 16, 48, "→　プレイヤーの傾きに合わせるようにカメラの回転", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	camera.Exit();
	ground.Exit();
	player.Exit();

	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
}
