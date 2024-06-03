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
int score = 0;
int captured = 0;

//---------------------------------------------------------------------------------
//	パネル番号を変更していく関数
//	マップデータの w 番目 h 番目の値が num だったら消す
//---------------------------------------------------------------------------------
void ChangePanel( int w, int h, int num )
{
	//	２－１：渡された番号が０だったら終了
	if( num == 0 )	return;
	//	２－２：そこの番号が基準番号と違っていたら終了
	if( MapData[h][w] != num )	return;

	//	ここまで来たらマップのこの場所の番号は基準番号と同じ
	//	２－３：そこの番号を消します
	MapData[h][w] = 0;

	//	２－４：上下左右の隣のパネルで同じ処理
	if( w + 1 <  MAP_W )	ChangePanel( w + 1, h,     num );	//	右
	if( w - 1 >=     0 )	ChangePanel( w - 1, h,     num );	//	左も
	if( h + 1 <  MAP_H )	ChangePanel( w,     h + 1, num );	//	下も
	if( h - 1 >=     0 )	ChangePanel( w,     h - 1, num );	//	上も

	score++;
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

		//	１：クリックしたところの番号を取得（この番号を基準にします）
		int num = MapData[click_h][click_w];
		//	取得した番号が消えていなかったら以下の処理をします
		if( num != 0 ){
			//	１：クリックしたところの場所と基準番号を渡す
			ChangePanel( click_w, click_h, num );
		}
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
		}
	}
	int currentBarWidth = (score * 400) / 144;
	//	文字列を見やすくするために黒の半透明の四角を描画しています
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 100 );
	DrawFillBox( 0, 0, 570, 120, GetColor( 0, 0, 0 ) );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );

	DrawFormatString(16,36, GetColor(255,255,255), "%d 枚消えました", score);
	DrawString( 16, 16, "クリック：パネルを消す（つながっている同じ色のパネルも）", GetColor( 255, 255, 255 ) );

	DrawFillBox(16, 80, 416, 110, GetColor(255, 255, 255));
	DrawFillBox(16, 80, 16 + currentBarWidth, 110, GetColor(12, 12, 255));
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
