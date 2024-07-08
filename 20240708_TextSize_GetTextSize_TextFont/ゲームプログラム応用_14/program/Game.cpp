#include "Main.h"
#include "Game.h"

const char* sample_text[2] = {
	"違うフォントで描画",
	"もうひとつ違うフォントで",
};

int count;

int font_handle0 = -1;
int font_handle1 = -1;

int text_w0;

int text_w1;
int text_h1;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	font_handle0 = CreateFontToHandle("游明朝", 30, -1);
	font_handle1 = CreateFontToHandle("メイリオ", 30, -1);

	count = 0;

	text_w0 = GetDrawStringWidthToHandle(sample_text[0], -1, font_handle0); //Get string width

	GetDrawStringSizeToHandle(&text_w1, &text_h1, NULL, sample_text[1], -1, font_handle1);
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	count++;
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	DrawStringToHandle(50, 100, sample_text[0], GetColor(255, 255, 0), font_handle0);
	DrawLine(50, 130, 50 + text_w0, 130, GetColor(255, 255, 0));

	DrawStringToHandle(50, 200, sample_text[1], GetColor(0, 255, 255), font_handle1);
	DrawLineBox(50, 200, 50 + text_w0, 200 + text_h1, GetColor(255, 255, 0));

	DrawFormatStringToHandle(50, 300, GetColor(0, 255, 0), font_handle1, "カウント : %d ", count);
	int w, h;
	GetDrawFormatStringSizeToHandle(&w, &h, NULL, font_handle1, "カウント : %d ", count); //using handle to get text string width and height on runtime 
	DrawLineBox(50, 300, 50 + w, 300 + h, GetColor(0, 255, 0));

	DrawFormatString(20, 40, GetColor(255, 255, 255), "カウント：%d", count);

	DrawString(20, 20, "文字列表示いろいろ", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteFontToHandle(font_handle0);
	DeleteFontToHandle(font_handle1);
}
