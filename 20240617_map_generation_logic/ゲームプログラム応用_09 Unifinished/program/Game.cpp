#include "Main.h"
#include "Game.h"
#include "Random.h"

//	パネルの大きさ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		5
#define MAP_H		5

int floor_image;

int wall_right_image;
int wall_down_image;
int wall_left_image;
int wall_up_image;

int WallData[MAP_H][MAP_W] =
{
	{ 12, 8, 8, 9,13 },
	{  6, 2, 0, 1, 5 },
	{ 12, 9, 6, 0, 1 },
	{  5, 4, 8, 2, 3 },
	{  7, 6, 2,10,11 },
};

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	floor_image = LoadGraph( "data/floor.bmp" );

	wall_right_image = LoadGraph( "data/wall_right.bmp" );
	wall_down_image  = LoadGraph( "data/wall_down.bmp" );
	wall_left_image  = LoadGraph( "data/wall_left.bmp" );
	wall_up_image    = LoadGraph( "data/wall_up.bmp" );
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	マップの描画
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){

			int x = w * PANEL_SIZE + 100;
			int y = h * PANEL_SIZE + 100;

			DrawGraph( x, y, floor_image, TRUE );

			int wall = WallData[h][w];
		}
	}

	DrawString( 16, 16, "壁情報の値を使ってそれぞれのマスの上下左右に壁画像を描画", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( floor_image );

	DeleteGraph( wall_right_image );
	DeleteGraph( wall_down_image );
	DeleteGraph( wall_left_image );
	DeleteGraph( wall_up_image );
}
