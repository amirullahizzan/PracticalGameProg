#include "Main.h"
#include "Game.h"
#include "Float2.h"

#define PANEL_SIZE	50
#define MAP_W		16
#define MAP_H		 9

int panel_image[2];

int npc_image;
int target_image;

int npc_w;
int npc_h;

int target_w;
int target_h;

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

int Route[MAP_H][MAP_W];

int frame_count;
void SetRoute(int w, int h, int num)
{
	if (w < 0)	   return;
	if (w >= MAP_W) return;
	if (h < 0)	   return;
	if (h >= MAP_H) return;

	if (MapData[h][w] == 1) return;
	Route[h][w] == num;

	if (Route[h][w + 1] > num + 1) SetRoute(w + 1, h, num + 1);
	if (Route[h][w - 1] > num + 1) SetRoute(w - 1, h, num + 1);
	if (Route[h + 1][w] > num + 1) SetRoute(w, h + 1, num + 1);
	if (Route[h - 1][w] > num + 1) SetRoute(w, h - 1, num + 1);

}
//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	npc_image = LoadGraph("data/npc.bmp");
	target_image = LoadGraph("data/mark.bmp");

	npc_w = 13;
	npc_h = 5;

	target_w = 1;
	target_h = 1;
	frame_count = 0;

	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			Route[h][w] = 1000;
		}
	}
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushMouseInput()) {

		target_w = GetMouseX() / PANEL_SIZE;
		target_h = GetMouseY() / PANEL_SIZE;

		if (MapData[target_h][target_w] == 0)
		{
			for (int h = 0; h < MAP_H; h++)
			{
				for (int w = 0; w < MAP_W; w++)
				{
					Route[h][w] = 1000;
				}
			}
			SetRoute(target_w, target_h, 0);
		}

	}

	frame_count++;
	if (frame_count % 10 == 0) {
		int route_center = Route[npc_h][npc_w];

		int route_right = Route[npc_h][npc_w + 1];
		int route_left = Route[npc_h][npc_w - 1];
		int route_down = Route[npc_h + 1][npc_w];
		int route_up = Route[npc_h - 1][npc_w];

		/*if (route_center > route_right) {
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
		}*/

		int start = GetRand(3);
		for (int i = start; i < start + 4; i++)
		{
			int num = i % 4;
			if (num == 0 && route_center > route_right) { npc_w++; break; }
			if (num == 1 && route_center > route_left) { npc_w--; break; }
			if (num == 2 && route_center > route_down) { npc_h++; break; }
			if (num == 3 && route_center > route_up) { npc_h--; break; }
		}
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			int num = MapData[h][w];

			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			DrawGraph(x, y, panel_image[num], TRUE);

			if (num == 0) {
				DrawFormatString(x, y, GetColor(255, 255, 255), "%d", Route[h][w]);
			}
		}
	}

	int nx = npc_w * PANEL_SIZE;
	int ny = npc_h * PANEL_SIZE;
	DrawGraph(nx, ny, npc_image, TRUE);

	int tx = target_w * PANEL_SIZE;
	int ty = target_h * PANEL_SIZE;
	DrawGraph(tx, ty, target_image, TRUE);

	DrawString(16, 16, "クリック：目的地設定（ＮＰＣは目的地に向かって移動する）", GetColor(255, 255, 255));
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
