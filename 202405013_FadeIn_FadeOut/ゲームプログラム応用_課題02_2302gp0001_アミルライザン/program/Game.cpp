#include "Main.h"
#include "Game.h"

//	タイトル画面の画像用の変数
int title_image;
//	ステージ画面の画像用の変数
int stage_image;

//	シーン番号の定数
enum {
	SCENE_TITLE,	//	タイトル
	SCENE_STAGE,	//	ステージ
};
//	今のシーン番号を入れる用の変数
int scene_num;

//	フェード関係
//	今のフェード状態用の定数
enum {
	FADE_NONE,			//	フェード状態ではない（通常の状態）
	//	Ｆ２キー：透明度（アルファ値）を変更したフェード（実装済み）
	FADE_ALPHA_OUT,		//	フェードアウト中（だんだん黒くなっていく：不透明になっていく）
	FADE_ALPHA_IN,		//	フェードイン中（だんだん黒いのがなくなっていく：透明になっていく）
	MOVE_RIGHT,
	UP_DOWN
	//	Ｆ３キー：黒い四角を横に移動させるフェード

	//	Ｆ４キー：上下から黒い四角が出てくるフェード（上下に２つの四角を描画しています）

};
//	今はどんなフェード状態なのかを見る用の変数
int fade_mode;

int boxPositionX = 0;
int boxPositionY1 = 0;
int boxPositionY2 = SCREEN_H;

//	黒い四角の透明度を制御する変数
//	（今どのくらいの透明度なのか　０：透明 ～ ２５５：不透明）
int fade_alpha;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	タイトルから開始する
	scene_num = SCENE_TITLE;
	//	それぞれの画像の読み込み
	title_image = LoadGraph("data/title.bmp");
	stage_image = LoadGraph("data/stage.bmp");

	//	始まるときはフェードしていない状態から始める
	fade_mode = FADE_NONE;
	//	透明度の透明から開始
	fade_alpha = 0;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	各シーンごとの更新処理
	switch (scene_num) {
	case SCENE_TITLE:	//	タイトル
		//	それぞれのフェード状態を分けます
		switch (fade_mode) {
		case FADE_NONE:			//	フェード中じゃない（キーを受け付ける状態）
			//	Ｆ１キーを押したらステージに行くためにシーン番号の変更
			if (PushHitKey(KEY_INPUT_F1)) {
				scene_num = SCENE_STAGE;
			}
			//	Ｆ２キーを押したらフェードを開始
			if (PushHitKey(KEY_INPUT_F2)) {
				//	１：フェードアウトを開始する
				fade_mode = FADE_ALPHA_OUT;
				//	黒い四角を透明から開始（透明：透明度０）
				fade_alpha = 0;
			}
			if (PushHitKey(KEY_INPUT_F3))
			{
				boxPositionX = -SCREEN_W;
				fade_alpha = 255;
				fade_mode = MOVE_RIGHT;
			}
			if (PushHitKey(KEY_INPUT_F4))
			{
				fade_mode = UP_DOWN;
			}
			break;

		case FADE_ALPHA_OUT:	//	フェードアウト中
			//	２：フェードアウトしていく（黒い四角の透明度を不透明に）
			fade_alpha += 10;
			//	３：完全に黒くなったら（不透明になったら）
			if (fade_alpha > 255) {
				//	不透明の値に設定
				fade_alpha = 255;
				//	ステージに行くのでシーン番号を切り替える
				scene_num = SCENE_STAGE;
				//	４：ステージに行ったらフェードインさせたいので（じゃないと画面が黒いままなので）
				fade_mode = FADE_ALPHA_IN;
			}
			break;
		case MOVE_RIGHT:	//	フェードアウト中
			boxPositionX += 25;
			if (boxPositionX > 0)
			{
				scene_num = SCENE_STAGE;
			}
			break;
		case UP_DOWN:	//	フェードイン中
			boxPositionY1 += 25;
			boxPositionY2 -= 25;
			if (boxPositionY1 >= SCREEN_H / 2)
			{
				scene_num = SCENE_STAGE;
			}
			break;

		}

		break;

	case SCENE_STAGE:	//	ステージ

		//	キーが押せるのはフェードが終わってから
		switch (fade_mode) {
		case FADE_NONE:		//	キーを受け付けるのはフェードしていないとき
			//	キーを押したらタイトルに戻る
			if (PushHitKey(KEY_INPUT_F1) || PushHitKey(KEY_INPUT_F2) || PushHitKey(KEY_INPUT_F3) || PushHitKey(KEY_INPUT_F4)) {
				//	シーン番号をタイトルへ
				scene_num = SCENE_TITLE;
			}
			break;

		case FADE_ALPHA_IN:	//	フェードイン中
			//	５：だんだん透明にしていく
			fade_alpha -= 10;
			//	６：完全に透明になったら
			if (fade_alpha < 0) {
				fade_alpha = 0;
				//	６：通常状態にします
				fade_mode = FADE_NONE;
			}
			break;

		case MOVE_RIGHT:	//	フェードイン中
			if (boxPositionX <= SCREEN_W)
			{
				boxPositionX += 25;
			}
			if (boxPositionX >= SCREEN_W)
			{
				fade_mode = FADE_NONE;
			}
			break;
		case UP_DOWN:	//	フェードイン中
			boxPositionY1 -= 25;
			boxPositionY2 += 25;
			if (boxPositionY1 < 0)
			{
				fade_mode = FADE_NONE;
			}
			break;
		}

		break;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	各シーンごとの描画処理
	switch (scene_num) {
	case SCENE_TITLE:	//	タイトル
		//	タイトル用の画像や文字列の描画
		DrawGraph(0, 0, title_image, TRUE);
		DrawString(16, 16, "タイトル画面", GetColor(255, 255, 255));
		DrawString(16, 32, "Ｆ１キー：すぐにステージ画面へ", GetColor(255, 255, 255));
		DrawString(16, 48, "Ｆ２キー：フェードアウトしてステージ画面へ", GetColor(255, 255, 255));
		DrawString(16, 64, "Ｆ３キー：スライドさせるような切り替え", GetColor(255, 255, 0));
		DrawString(16, 80, "Ｆ４キー：上下から黒くなるような切り替え", GetColor(255, 255, 0));
		break;

	case SCENE_STAGE:	//	ステージ
		//	ステージ用の画像や文字列の描画
		DrawGraph(0, 0, stage_image, TRUE);
		DrawString(16, 16, "ステージ画面", GetColor(255, 255, 255));
		DrawString(16, 32, "Ｆ１～Ｆ４キー：すぐにタイトル画面へ戻る", GetColor(255, 255, 255));
		break;
	}

	//	フェード関係の描画
	switch (fade_mode) {
	case FADE_NONE:
		//	フェードしていないときは黒い四角は透明で見えないので描画はしない
		break;

		//	フェードアウト中もイン中も黒い四角を描画しようと思います
	case FADE_ALPHA_OUT:
	case MOVE_RIGHT:
	case FADE_ALPHA_IN:
		//	黒い四角の半透明設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		//	画面サイズの黒い四角の描画
		DrawFillBox(boxPositionX, 0, boxPositionX + SCREEN_W, SCREEN_H, GetColor(0, 0, 0));
		//	透明度設定を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	case UP_DOWN:
		DrawFillBox(0, boxPositionY1, SCREEN_W, 0, GetColor(0, 0, 0));
		DrawFillBox(0, boxPositionY2, SCREEN_W, SCREEN_H, GetColor(0, 0, 0));
		break;
	}

}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(title_image);
	DeleteGraph(stage_image);
}
