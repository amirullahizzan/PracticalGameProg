#include "Main.h"
#include "Game.h"

//	画像データ用の変数
int bg_image;
int player_image;
int npc_image;

//	わざと読み込みを遅くするためにテストファイルをたくさん読み込ませていきます
#define TEST_MAX	2000
//	テストファイル用の変数
int test_image[TEST_MAX];

//	シーンの定義
enum {
	SCENE_LOAD,		//	ロード中
	SCENE_GAME,		//	ゲーム中
};
//	今どこのシーンか
int scene_num;

//	今読み込み中のファイルの数がいくつあるか
int loadfile_count;
//	NOW LOADING を動かしたりしたいので
int frame_count;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	ロードf画面から開始
	scene_num = SCENE_LOAD;

	//	背景画像の読み込み
	bg_image = LoadGraph("data/bg.bmp");
	//	プレイヤー画像の読み込み
	player_image = LoadGraph("data/player.bmp");
	//	ＮＰＣ画像の読み込み
	npc_image = LoadGraph("data/npc.bmp");
	//	１：今から呼び出す読み込み関数たちを非同期の設定にします
	SetUseASyncLoadFlag( TRUE );

	//	テストファイルの読み込み
	for( int i = 0; i < TEST_MAX; i++ ){
		test_image[i] = LoadGraph( "data/test.bmp" );
	}



	frame_count = 0;
	loadfile_count = 0;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	switch( scene_num ){
	case SCENE_LOAD:
		//	カウントを増やす
		frame_count++;
		//	今読み込み中のファイルの数を取得
		loadfile_count = GetASyncLoadNum();
		//	この数が０になったということはすべてのファイルを読み込んだ
		if( loadfile_count == 0 ){
			//	ゲームシーンに移行します
			scene_num = SCENE_GAME;
		}
		break;

	case SCENE_GAME:
		//	今回のプロジェクトではゲーう画面では特に何もしていません
		break;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	const float boxWidth = 250.0f;
	const float boxHeight = 50.0f;
	int loadingSize = 0;

	switch( scene_num ){
	case SCENE_LOAD:
		DrawString( 16, 16, "********** ロード画面 **********", GetColor( 255, 255, 255 ) );
		//	読み込み中のファイル数がいくつあるのかを文字列で描画して確認
		DrawFormatString( 16, 32, GetColor( 255, 255, 255 ), "読み込み中のファイル数：%d", loadfile_count );

		//	Now Loading の点滅
		if( ( frame_count / 30 ) % 2 ){
			DrawString( 350, 350, "Now Loading", GetColor( 255, 255, 255 ) );
		}
		//	NOW LOADING の移動
		DrawString( SCREEN_W - frame_count, 400, "NOW LOADING", GetColor( 255, 255, 255 ) );
		DrawRotaGraphF(SCREEN_W - frame_count - 40 , 400, 1.0f, TO_RADIAN(180.0f), npc_image, TRUE);
		DrawRotaGraphF(SCREEN_W - frame_count - 120, 400, 1.0f, TO_RADIAN(180.0f), player_image, TRUE);

		loadingSize = static_cast<float>(loadfile_count) / TEST_MAX * boxWidth /2;
		DrawBox(SCREEN_W/2 - boxWidth , 200, SCREEN_W/2 + boxWidth - loadingSize, 200 + boxHeight,GetColor(255,255,255),1);
		DrawLineBox(SCREEN_W / 2 - boxWidth, 200, SCREEN_W/2 + boxWidth, 200 + boxHeight, GetColor(255, 255, 255));
		break;

	case SCENE_GAME:
		//	ゲーム画面での描画処理
		DrawGraph( 0, 0, bg_image, TRUE );
		DrawRotaGraphF( 600.0f, 200.0f, 1.0f, TO_RADIAN( 180.0f ), npc_image, TRUE );
		DrawRotaGraphF( 200.0f, 200.0f, 1.0f, TO_RADIAN( 0.0f ), player_image, TRUE );

		DrawString( 16, 16, "ゲーム画面", GetColor( 255, 255, 255 ) );
		break;
	}
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	//	テストファイルの終了処理
	for( int i = 0; i < TEST_MAX; i++ ){
		DeleteGraph( test_image[i] );
	}

	DeleteGraph( bg_image );
	DeleteGraph( player_image );
	DeleteGraph( npc_image );
}
