#include "Main.h"
#include "Game.h"
#include "Float2.h"

//	パネルの大きさ
#define PANEL_SIZE	50
//	パネルを置ける数
#define MAP_W		16
#define MAP_H		 9

#define BASE_X 375
#define BASE_Y 200
//	パネル画像用の変数
//	０：通れる
//	１：通れない
int panel_image[2];

//	ＮＰＣ画像
int npc_image;
//	プレイヤー画像
int target_image;

//	ＮＰＣがいるポイント（座標ではない）
int npc_w;
int npc_h;
//	プレイヤーがいるポイント（目的地：座標ではない）
int target_w;
int target_h;

//	マップ用の２次元配列
//	０：通れる
//	１：通れない
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1 },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	ＮＰＣが移動するための番号を入れる用の変数
int Route[MAP_H][MAP_W];

//	ＮＰＣをゆっくり動かすためのカウンター
int frame_count;

//---------------------------------------------------------------------------------
//	ルート作成関数
//	（引数で渡された w h 番目の Route[h][w] の値を num にする）
//---------------------------------------------------------------------------------
void SetRoute(int w, int h, int num)
{
	//	今回のマップは周りが壁であることが前提ですが
	//	周りが壁じゃない場合は、２次元配列の中かどうかの判定も必要です
	if (w < 0)			return;
	if (w >= MAP_W)	return;
	if (h < 0)			return;
	if (h >= MAP_H)	return;

	//	通れないところまで行ったら終了
	if (MapData[h][w] == 1)	return;

	//	引数で渡された場所（ w h ）のルート番号を渡された値（ num ）に設定
	Route[h][w] = num;

	//	１個右の場所（ w + 1, h ）のルート番号を１大きくしたもの（ num + 1 ）で
	//	その１個右のルート番号が今から割り振りたい値よりも大きかったら
	if (Route[h][w + 1] > num + 1)		SetRoute(w + 1, h, num + 1);	//	右
	if (Route[h][w - 1] > num + 1)		SetRoute(w - 1, h, num + 1);	//	左
	if (Route[h + 1][w] > num + 1)		SetRoute(w, h + 1, num + 1);	//	下
	if (Route[h - 1][w] > num + 1)		SetRoute(w, h - 1, num + 1);	//	上
}

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	npc_image = LoadGraph("data/npc.bmp");
	//	このプロジェクトではプレイヤー画像を使います
	target_image = LoadGraph("data/player.bmp");

	//	ＮＰＣの開始ポイント
	npc_w = 13;
	npc_h = 5;
	//	このプロジェクトではプレイヤーのいるポイントです
	target_w = 1;
	target_h = 1;
	//	カウンターは０で開始
	frame_count = 0;
	//	ルート用の変数に１０００を入れて初期化
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			Route[h][w] = 1000;
		}
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
bool isPlayerTurn = true;
void GameUpdate()
{
	//	十字キーを押したときにプレイヤーを次のポイントに進めて（壁があったら移動はしない）
	//	進んだときにＮＰＣも１回だけ動かす
	if (isPlayerTurn)
	{
		int previousTargetH = target_h;
		int previousTargetW = target_w;
			if (PushHitKey(KEY_INPUT_RIGHT))
			{
				target_w += 1;
			}
			else if (PushHitKey(KEY_INPUT_LEFT))
			{
				target_w -= 1;
			}
			else if (PushHitKey(KEY_INPUT_DOWN))
			{
				target_h += 1;
			}
			else if (PushHitKey(KEY_INPUT_UP))
			{
				target_h -= 1;
			}

		if (PushHitKey(KEY_INPUT_RIGHT) || PushHitKey(KEY_INPUT_LEFT) || PushHitKey(KEY_INPUT_UP) || PushHitKey(KEY_INPUT_DOWN))
		{
			//	クリックポイントを取得します（とりあえずここを目的地にしています）
			//	（今回はこの処理も使いません：十字キーでポイントを移動させるので）
			//	クリックしたポイントが通れるところだったら
			if (MapData[target_h][target_w] == 0)
			{
				//	いったんすべてのルートの値を割り振っていないと分かる値にします
				for (int h = 0; h < MAP_H; h++) {
					for (int w = 0; w < MAP_W; w++) {
						//	けっこう大きめな数値を設定します
						Route[h][w] = 1000;
					}
				}
				//	ルートの作成をしていきます（ルートの target_w 番目 target_h 番目の値を 0 にします）
				SetRoute(target_w, target_h, 0);
				isPlayerTurn = false;
			}
			else
			{
				target_h = previousTargetH;
				target_w = previousTargetW;
			}
		}
	}



	frame_count++;
	//	一定間隔でＮＰＣの移動
	if (frame_count % 30 == 0 && !isPlayerTurn) {
		//	ＮＰＣが今いるポイントのルート番号
		int route_center = Route[npc_h][npc_w];
		//	上下左右のルート番号
		int route_right = Route[npc_h][npc_w + 1];
		int route_left = Route[npc_h][npc_w - 1];
		int route_down = Route[npc_h + 1][npc_w];
		int route_up = Route[npc_h - 1][npc_w];

		//	上下左右のどこを最初に見ていくかをランダムにしてみます
		//	どこから始めるかの番号（０：右　１：左　２：下　３：上）
		int start = GetRand(3);
		//	上下左右の４回繰り返す
		for (int i = start; i < start + 4; i++) {
			//	何番目を見るか
			int num = i % 4;
			//	num が０の時は右を見ます（さらに右のルート番号が小さかったら移動できる）
			if (num == 0 && route_center > route_right) { npc_w++;	break; }
			if (num == 1 && route_center > route_left) { npc_w--;	break; }
			if (num == 2 && route_center > route_down) { npc_h++;	break; }
			if (num == 3 && route_center > route_up) { npc_h--;	break; }
		}
		isPlayerTurn = true;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	マップの描画
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	その場所のマップ番号を取得
			int num = MapData[h][w];
			//	実際の描画座標を取得
			int x = w * PANEL_SIZE + BASE_X - (target_w * PANEL_SIZE);
			int y = h * PANEL_SIZE + BASE_Y - (target_h * PANEL_SIZE);
			//	そこの座標にその番号の四角画像を描画
			DrawGraph(x, y, panel_image[num], TRUE);

			//	通れるところだけ
			if (num == 0) {
				//	その場所のルート番号を文字列で描画
				DrawFormatString(x, y, GetColor(255, 255, 255), "%d", Route[h][w]);
			}
		}
	}
	//	ＮＰＣのいるポイントから画面座標を計算して描画
	int nx = npc_w * PANEL_SIZE + BASE_X - (target_w * PANEL_SIZE);
	int ny = npc_h * PANEL_SIZE + BASE_Y - (target_h * PANEL_SIZE);
	DrawGraph(nx, ny, npc_image, TRUE);
	//	目的地のポイントから画面座標を計算して描画

	int tx = BASE_X;
	int ty = BASE_Y;
	DrawGraph(tx, ty, target_image, TRUE);

	DrawString(16, 16, "十字キー：プレイヤー移動（壁があったら進めない・ＮＰＣが移動した後に再び動ける）", GetColor(255, 255, 0));
	DrawString(16, 32, "ＮＰＣはプレイヤーに向かって移動 → プレイヤーが１回動いたら１秒後にＮＰＣも１回動く", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(npc_image);
	DeleteGraph(target_image);
}
