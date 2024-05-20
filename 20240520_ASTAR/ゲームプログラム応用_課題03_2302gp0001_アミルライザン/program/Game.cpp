#include "Main.h"
#include "Game.h"
#include "Float2.h"

//	パネルサイズはサンプルの半分にしています
#define PANEL_SIZE	25
//	サイズが小さくなったので配列の数も少し増やしています
#define BASE_X	150
#define BASE_Y	105

#define MAP_W		20 
#define MAP_H		13


//	パネル画像用の変数
//	０：通れる
//	１：通れない
int panel_image[2];

//	ＮＰＣ画像
int npc_image;
//	黄色い枠画像
int target_image;
//	ルートエリア表示用画像
int area_image;

//	ＮＰＣがいるポイント（座標ではない）
int npc_w;
int npc_h;
//	目的地のポイント（座標ではない）
int target_w;
int target_h;

//	０：通れる
//	１：通れない
//	サイズが小さくなったので配列の数も少し増やしています
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 1,0,1,0,1,1,1,1,1,1,1,0,1,0,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1 },
	{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1 },
	{ 1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1 },
	{ 1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	１：ＮＰＣが移動するための番号を入れる用の変数
int Route[MAP_H][MAP_W];

//	２－３：基準の番号を入れる用の変数
int route_num;

//	ＮＰＣをゆっくり動かすためのカウンター
int frame_count;

//	マップ全体はこの基準の場所から描画をしようと思います
#define BASE_X	150
#define BASE_Y	105

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");
	npc_image = LoadGraph("data/npc.bmp");
	target_image = LoadGraph("data/mark.bmp");
	//	エリア画像の読み込みもすでにしています
	area_image = LoadGraph("data/area.bmp");

	//	ＮＰＣの開始ポイント
	npc_w = 13;
	npc_h = 5;
	//	とりあえず目的地のポイント
	target_w = 1;
	target_h = 1;

	//	ルート用の変数に１０００を入れて初期化
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	マスに文字列が入りきらないので 1000 ではなく 99 にしています
			Route[h][w] = 99;
		}
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	クリックすることで目的地の設定など
	if (PushMouseInput()) {
		//	２－１：いったんすべてのルートの値を割り振っていない値から開始
		for (int h = 0; h < MAP_H; h++)
		{
			for (int w = 0; w < MAP_W; w++)
			{
				//	マスに文字列が入りきらないので 1000 ではなく 99 にしています
				Route[h][w] = 99;
			}
		}
		//	クリックしたところが２次元配列的に何番目か
		target_h = (GetMouseY() / PANEL_SIZE) - (BASE_Y / PANEL_SIZE);
		target_w = (GetMouseX() / PANEL_SIZE) - (BASE_X / PANEL_SIZE);
		//	２－２：クリックしたところが通れるところだったら
		if (MapData[target_h][target_w] == 0) {
			//	２：クリックしたところのルート番号を０にして開始
			Route[target_h][target_w] = 0;
			//	２－３：基準の番号を０にして開始
			route_num = 0;
		}

		for (int i = 0; i < 10; i++)
		{
			for (int h = 0; h < MAP_H; h++) {
				for (int w = 0; w < MAP_W; w++) {
					//	３：基準の番号を見つけたら
					if (Route[h][w] == route_num) {
						//	３：上下左右の場所のルート番号を「基準番号＋１」にします
						//	　　条件として通れる場所のルート番号が大きいとき（さらに隣が配列外にならないとき）
						if (w < MAP_W - 1 && MapData[h][w + 1] == 0 && Route[h][w + 1] > route_num)	Route[h][w + 1] = route_num + 1;
						if (w > 0 && MapData[h][w - 1] == 0 && Route[h][w - 1] > route_num)	Route[h][w - 1] = route_num + 1;
						if (h < MAP_H - 1 && MapData[h + 1][w] == 0 && Route[h + 1][w] > route_num)	Route[h + 1][w] = route_num + 1;
						if (h > 0 && MapData[h - 1][w] == 0 && Route[h - 1][w] > route_num)	Route[h - 1][w] = route_num + 1;
					}
				}
			}
			//	４：次のスペースキーを押したときのために基準の番号を１増やします
			route_num++;
		}
	}

	//if (route_num < 10)
	
	//	スペースキーでルートを広げる
	//	（ですが、課題ではこれを１０回繰り返しおこないますのでスペースキーの判定は使いません）
	if (PushHitKey(KEY_INPUT_SPACE)) {

	}

	frame_count++;
	//	一定間隔でＮＰＣの移動
	if (frame_count % 10 == 0) {
		//	今いるところのルート番号
		int route_center = Route[npc_h][npc_w];
		//	上下左右のルート番号
		int route_right = Route[npc_h][npc_w + 1];
		int route_left = Route[npc_h][npc_w - 1];
		int route_down = Route[npc_h + 1][npc_w];
		int route_up = Route[npc_h - 1][npc_w];
		//	ＮＰＣは少ない値の方に移動する
		if (route_center > route_right) {
			npc_w++;
		}
		else if (route_center > route_left) {
			npc_w--;
		}
		else if (route_center > route_down) {
			npc_h++;
		}
		else if (route_center > route_up) {
			npc_h--;
		}
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	２次元配列の値を見て画像を並べる
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	マップ配列から取得した番号
			int num = MapData[h][w];
			//	描画する座標（パネルサイズ分ずらしたところ）
			int x = w * PANEL_SIZE + BASE_X;
			int y = h * PANEL_SIZE + BASE_Y;
			//	パネル画像を描画
			DrawGraph(x, y, panel_image[num], TRUE);

			//	通れるところだけ文字列の描画
			if (num == 0)
			{
				//	ルート変数の番号を文字列で描画
				DrawFormatString(x, y, GetColor(255, 255, 255), "%d", Route[h][w]);
			}

			if (Route[h][w] != 0 && Route[h][w] != 99)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);	//	半透明設定
				DrawGraph(x, y, area_image, TRUE);			//	エリア画像の描画
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//	半透明設定を戻す
			}
		}
	}
	//	ＮＰＣのポイントから画面座標を求めて描画
	int nx = npc_w * PANEL_SIZE + BASE_X;
	int ny = npc_h * PANEL_SIZE + BASE_Y;
	DrawGraph(nx, ny, npc_image, TRUE);

	//	目的地のポイントから画面座標を求めて描画
	int tx = target_w * PANEL_SIZE + BASE_X;
	int ty = target_h * PANEL_SIZE + BASE_Y;
	DrawGraph(tx, ty, target_image, TRUE);

	//DrawString( 16, 16, "クリック：目的地設定 → １０マス分ルート作成", GetColor( 255, 255, 0 ) );
	//DrawString( 16, 32, "１０マス分のルートが分かるようにエリア画像を描画（半透明にしています）", GetColor( 255, 255, 0 ) );
	//DrawString( 16, 48, "→　ルートが届いたらＮＰＣは目的地に向かって移動する", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 80, "マップ全体を画面の真ん中あたりにする（ BASE_X BASE_Y を基準の座標として）", GetColor( 255, 255, 0 ) );
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
	DeleteGraph(area_image);
}
