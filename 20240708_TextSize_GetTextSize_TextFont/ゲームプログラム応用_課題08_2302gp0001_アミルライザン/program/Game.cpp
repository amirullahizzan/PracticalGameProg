#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Hit.h"

//	フォントハンドル
int font_handle;

//	メニュー項目数
#define MENU_MAX	4
//	各メニュー項目の文字列
const char* menu_text[MENU_MAX] = {
	"ゲームスタート",
	"チュートリアル",
	"ランキング",
	"ゲーム設定",
};
//	各メニュー文字列の座標
int menu_x[MENU_MAX];
int menu_y[MENU_MAX];
//	選択番号
int select_num;

//	「こういう感じで（発展）.exe」で使っている文字列です
const char* select_text[MENU_MAX] = {
	"ゲームを開始します",
	"遊び方をみます",
	"今の順位を確認します",
	"難易度や音量の設定をします",
};

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	フォントハンドルの作成
	font_handle = CreateFontToHandle("メイリオ", 50, -1);

	//	メニュー文字列の座標設定
	for (int i = 0; i < MENU_MAX; i++) {
		//	Ｘ座標はとりあえず画面左にしています

		menu_x[i] = SCREEN_W / 2.0f;
		//	Ｙ座標はここで設定しています（これ以上変更はありません）
		menu_y[i] = 50 + 80 * i;
	}
	//	選択番号０
	select_num = 0;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	上下キーで選択番号を更新
	if (PushHitKey(KEY_INPUT_DOWN))	select_num++;
	if (PushHitKey(KEY_INPUT_UP))	select_num--;
	//	メニューの上下を超えたら反対から
	if (select_num < 0)	select_num = MENU_MAX - 1;
	if (select_num >= MENU_MAX)	select_num = 0;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------

int contextPosX[2] = { 16 , SCREEN_W + 16 };

void GameRender()
{
	//	メニュー項目文字列の描画
	for (int i = 0; i < MENU_MAX; i++) {
		//	デフォルトは白色
		int color = GetColor(255, 255, 255);
		//	選択番号と同じ文字列は黄色
		const float menuXmoveSpeed = 9;
		if (select_num == i)
		{
			//	選択番号と同じ文字列は黄色
			color = GetColor(255, 255, 0);

			const int maxMoveMenuX = 280;
			if (menu_x[i] > maxMoveMenuX)
			{
				menu_x[i] -= menuXmoveSpeed;
			}

			for (int j = 0; j < 2; j++)
			{
				contextPosX[j]--;
				int buffX, buffY;
				GetDrawFormatStringSizeToHandle(&buffX, &buffY, NULL, font_handle, select_text[i]);

				if (contextPosX[j] + buffX <= 0)
				{
					contextPosX[j] = SCREEN_W + 16;
				}
			}

		}
		else
		{
			const int minMoveMenuX = SCREEN_W / 2.0f;

			if (menu_x[i] < minMoveMenuX)
			{
				menu_x[i] += menuXmoveSpeed;
			}
		}
		//	上で決めた色を使ってメニュー文字列の描画
		DrawStringToHandle(menu_x[i], menu_y[i], menu_text[i], color, font_handle);

		int buffX, buffY;
		GetDrawFormatStringSizeToHandle(&buffX, &buffY, NULL, font_handle, menu_text[i]);
		DrawLineBox(menu_x[i], menu_y[i], menu_x[i] + buffX, menu_y[i] + buffY, GetColor(255, 255, 0));
	}

	int contextBoxSizeY = 60.0f;
	int contextBoxPosY = SCREEN_H - 30;

	DrawBox(0, contextBoxPosY, SCREEN_W, contextBoxPosY - contextBoxSizeY, GetColor(0, 0, 0), 1);


	for (int i = 0; i < MENU_MAX; i++)
	{
		if (select_num == i)
		{
			for (int j = 0; j < 2; j++)
			{
				DrawStringToHandle(contextPosX[j], contextBoxPosY - contextBoxSizeY + 8, select_text[i], GetColor(255, 255, 255), font_handle);
			}
		}
	}


	


	//DrawString( 16, 16, "メニュー項目の文字列（上下キーで選択）", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 32, "選択している項目の色は既に変更してます", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 48, "→　文字列の長さに合わせた四角で囲む", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
}
