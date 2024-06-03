#include "Main.h"
#include "Game.h"
#include "Random.h"

//	パネルサイズ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		16		//	横方向
#define MAP_H		 9		//	縦方向

//	パネル画像用の変数（０：消えている　１～３：それぞれの色のパネル）
int panel_image[4];

//	２次元配列（０～３の数字が入ります）
int MapData[MAP_H][MAP_W];

//	クリックしたポイント（座標ではない）
int click_w;
int click_h;

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void ChangePanel( int w, int h, int num )
{
	if (num == 0) return;
	if (MapData[h][w] != num) return;

	MapData[h][w] = 0;

	if (w + 1 < MAP_W) ChangePanel(w+1, h,num);
	if (w -1	 <= 0) ChangePanel(w-1, h,num);
	if (h + 1 < MAP_H) ChangePanel(w, h+1,num);
	if (h -1	 >= 0) ChangePanel(w, h-1,num);
}

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	パネル画像の読み込み
	panel_image[0] = LoadGraph( "data/panel0.bmp" );
	panel_image[1] = LoadGraph( "data/panel1.bmp" );
	panel_image[2] = LoadGraph( "data/panel2.bmp" );
	panel_image[3] = LoadGraph( "data/panel3.bmp" );

	//	マップデータの中身を１～３のランダムな値で埋めます
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	ランダムで１～３
			MapData[h][w] = GetRandomI( 1, 3 );
		}
	}
}

//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	マウスをクリック
	if( PushMouseInput() ){
		//	クリックした座標が配列的に何番目のところか
		click_w = GetMouseX() / PANEL_SIZE;
		click_h = GetMouseY() / PANEL_SIZE;

	}

	int num = MapData[click_h][click_w];

	if (num != 0)
	{
		ChangePanel(click_w, click_h, num);
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	パネルの描画
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	マップデータからそこの場所の番号を取得
			int num = MapData[h][w];
			//	画面のどこに表示するかの座標を求める
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	その場所にその画像を描画
			DrawGraph( x, y, panel_image[num], TRUE );

			//	そこのマップ番号を文字列で描画します
			DrawFormatString( x, y, GetColor( 255, 255, 255 ), "%d", num );
		}
	}

	DrawString( 16, 16, "クリック：パネルを消す（つながっている同じ色のパネルも）", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( panel_image[0] );
	DeleteGraph( panel_image[1] );
	DeleteGraph( panel_image[2] );
	DeleteGraph( panel_image[3] );
}
