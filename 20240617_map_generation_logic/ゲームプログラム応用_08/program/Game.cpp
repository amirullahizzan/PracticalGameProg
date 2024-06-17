#include "Main.h"
#include "Game.h"
#include "Random.h"

//	�p�l���̑傫��
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		7
#define MAP_H		9

//	�p�l���摜�p�̕ϐ�
int panel_image[2];

int pillar_image;
int wall_x_image;
int wall_y_image;

int mark_image;

int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1 },
	{ 1,0,1,0,0,0,1 },
	{ 1,0,1,1,1,0,1 },
	{ 1,0,0,0,0,0,1 },
	{ 1,0,1,1,1,0,1 },
	{ 1,0,1,0,0,0,1 },
	{ 1,1,1,0,1,0,1 },
	{ 1,0,0,0,1,0,1 },
	{ 1,1,1,1,1,1,1 },
};

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	pillar_image = LoadGraph("data/pillar.bmp");

	wall_x_image = LoadGraph("data/wall_x.bmp");
	wall_y_image = LoadGraph("data/wall_y.bmp");

	mark_image = LoadGraph("data/mark.bmp");
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�}�b�v�̕`��
	//for (int h = 0; h < MAP_H; h++) {
	//	for (int w = 0; w < MAP_W; w++) {
	//		//	���̏ꏊ�̔ԍ����擾
	//		int num = MapData[h][w];
	//		int x = w * PANEL_SIZE;
	//		int y = h * PANEL_SIZE;
	//
	//		//	�����Ɏl�p��`��
	//		DrawGraph(x, y, panel_image[num], TRUE);
	//	}
	//}

	//	Floors
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			if (w % 2 && h % 2)
			{
				int x = (w /2) * PANEL_SIZE;
				int y = (h /2) * PANEL_SIZE;

				//	�����Ɏl�p��`��
				DrawGraph(x, y, panel_image[0], TRUE);
			}
		}
	}
	//	Horizontal Walls
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			if (w % 2 && h % 2 == 0)
			{
				//	���̏ꏊ�̔ԍ����擾
				int num = MapData[h][w];
				if (num == 1)
				{

					int x = (w /2) * PANEL_SIZE;
					int y = (h /2) * PANEL_SIZE;

					//	�����Ɏl�p��`��
					DrawGraph(x, y, wall_x_image, TRUE);
				}
			}
		}
	}

	//	Vertical Walls
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			if (w % 2 == 0 && h % 2)
			{
				//	���̏ꏊ�̔ԍ����擾
				int num = MapData[h][w];
				if (num == 1)
				{

					int x = (w /2) * PANEL_SIZE;
					int y = (h /2) * PANEL_SIZE;

					//	�����Ɏl�p��`��
					DrawGraph(x, y, wall_y_image, TRUE);
				}
			}
		}
	}

	//	Pillars
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	���̏ꏊ�̔ԍ����擾
			if (w % 2 == 0 && h % 2 == 0)
			{
				int x = (w /2) * PANEL_SIZE;
				int y = (h /2) * PANEL_SIZE;

				//	�����Ɏl�p��`��
				DrawGraph(x, y, pillar_image, TRUE);
			}
		}
	}

	//	�}�b�v�̕`��
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	���̏ꏊ�̔ԍ����擾
			int num = MapData[h][w];
			int x = w * PANEL_SIZE + SCREEN_W / 2 + PANEL_SIZE;
			int y = h * PANEL_SIZE;

			//	�����Ɏl�p��`��
			DrawGraph(x, y, panel_image[num], TRUE);

			if (CheckHitKey(KEY_INPUT_1)) {
				if (w % 2 && h % 2) {
					DrawGraph(x, y, mark_image, TRUE);
				}
			}
			if (CheckHitKey(KEY_INPUT_2)) {
				if (w % 2 == 0 && h % 2 == 0) {
					DrawGraph(x, y, mark_image, TRUE);
				}
			}
			if (CheckHitKey(KEY_INPUT_3)) {
				if (w % 2 && h % 2 == 0) {
					if (num > 0) {
						DrawGraph(x, y, mark_image, TRUE);
					}
				}
			}
			if (CheckHitKey(KEY_INPUT_4)) {
				if (w % 2 == 0 && h % 2) {
					if (num > 0) {
						DrawGraph(x, y, mark_image, TRUE);
					}
				}
			}

		}
	}

	DrawString(350, 16, "�P�L�[�F��", GetColor(255, 255, 255));
	DrawString(350, 32, "�Q�L�[�F��", GetColor(255, 255, 255));
	DrawString(350, 48, "�R�L�[�F����", GetColor(255, 255, 255));
	DrawString(350, 64, "�S�L�[�F�c��", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(pillar_image);

	DeleteGraph(wall_x_image);
	DeleteGraph(wall_y_image);

	DeleteGraph(mark_image);
}
