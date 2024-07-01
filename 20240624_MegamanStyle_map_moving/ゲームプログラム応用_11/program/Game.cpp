#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"

//	１個の四角の大きさ
#define PANEL_SIZE	50
//	四角を置ける数
#define MAP_W		16
#define	MAP_H		 9

//	２次元配列（０：進める　１：進めない）
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	マップ画像
int panel_image[2];

//	プレイヤーの移動スピード
#define PLAYER_SPEED	5.0f

//	プレイヤー画像
int player_image;
//	プレイヤー座標
Float2 player_pos;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	panel_image[0] = LoadGraph( "data/panel0.bmp" );
	panel_image[1] = LoadGraph( "data/panel1.bmp" );

	player_image = LoadGraph( "data/player.bmp" );
	player_pos.x = 75.0f;
	player_pos.y = 75.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if( CheckHitKey( KEY_INPUT_RIGHT ) ){
		player_pos.x += PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_LEFT ) ){
		player_pos.x -= PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_DOWN ) ){
		player_pos.y += PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_UP ) ){
		player_pos.y -= PLAYER_SPEED;
	}
	Float2 before;

	Float2 box_pos;
	Float2 box_size;

	Float2 panel_pos;
	Float2 panel_size;

	if (CheckBoxHit(box_pos, box_size, panel_pos, panel_size))
	{
		if (before.x < player_pos.x) player_pos.x = panel_pos.x - PANEL_SIZE;
		if (before.x > player_pos.x) player_pos.x = panel_pos.x + PANEL_SIZE;
		if (before.y < player_pos.x) player_pos.y = panel_pos.y - PANEL_SIZE;
		if (before.y > player_pos.x) player_pos.y = panel_pos.y + PANEL_SIZE;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	マップの描画
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	そこの場所の番号
			int num = MapData[h][w];
			//	四角を描画する座標
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	そこにその番号の四角の描画
			DrawGraph( x, y, panel_image[num], TRUE );
		}
	}

	//	プレイヤーの座標を使って描画します
	float px = player_pos.x;
	float py = player_pos.y;
	DrawGraphF( px, py, player_image, TRUE );

	DrawString( 16, 16, "十字キー：プレイヤー移動", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( panel_image[0] );
	DeleteGraph( panel_image[1] );

	DeleteGraph( player_image );
}
