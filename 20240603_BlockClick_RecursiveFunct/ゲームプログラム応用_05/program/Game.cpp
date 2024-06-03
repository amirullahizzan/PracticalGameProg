#include "Main.h"
#include "Game.h"
#include "Random.h"

//	�p�l���T�C�Y
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		16		//	������
#define MAP_H		 9		//	�c����

//	�p�l���摜�p�̕ϐ�
int panel_image[4];

//	�Q�����z��
int MapData[MAP_H][MAP_W];

//	�N���b�N�����|�C���g
int click_w;
int click_h;
int target_num;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�p�l���摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");
	panel_image[2] = LoadGraph("data/panel2.bmp");
	panel_image[3] = LoadGraph("data/panel3.bmp");

	//	�}�b�v�f�[�^�̒��g���P�`�R�̃����_���Ȓl�Ŗ��߂܂�
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�����_���łP�`�R
			MapData[h][w] = GetRandomI(1, 3);
		}
	}
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�}�E�X���N���b�N
	if (PushMouseInput()) {
		//	�N���b�N�������W���z��I�ɉ��Ԗڂ̂Ƃ��납
		click_w = GetMouseX() / PANEL_SIZE;
		click_h = GetMouseY() / PANEL_SIZE;

	}

	if (MapData[click_h][click_w] > 0)
	{
		target_num = MapData[click_h][click_w];
	}

	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			if (MapData[h][w] == -1)
			{
				MapData[h][w] = 0;
			}
		}

				MapData[click_h][click_w] = -1;
	}

	//	�X�y�[�X�L�[������
	if (PushHitKey(KEY_INPUT_SPACE)) 
	{

		for (int h = 0; h < MAP_H; h++)
		{
			for (int w = 0; w < MAP_W; w++)
			{
				if (MapData[h][w] == -1)
				{
					if (w + 1 < MAP_W && MapData[h][w + 1] == target_num) {MapData[h][w+1] = -1;}
					if (w - 1 >= 0 && MapData[h][w - 1] == target_num)    {MapData[h][w-1] = -1;}
					if (h + 1 < MAP_H && MapData[h+1][w] == target_num) { MapData[h+1][w] = -1; }
					if (h - 1 >= 0 && MapData[h-1][w] == target_num) { MapData[h-1][w] = -1; }
				}
			}

			MapData[click_h][click_w] = -1;
		}
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�p�l���̕`��
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�}�b�v�f�[�^���炻���̏ꏊ�̔ԍ����擾
			int num = MapData[h][w];
			//	��ʂ̂ǂ��ɕ\�����邩�̍��W�����߂�
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			if (num >= 0) {
				//	���̏ꏊ�ɂ��̉摜��`��
				DrawGraph(x, y, panel_image[num], TRUE);
			}
			//	�����̃}�b�v�ԍ��𕶎���ŕ`�悵�܂�
			DrawFormatString(x, y, GetColor(255, 255, 255), "%d", num);
		}
	}

	DrawString(16, 16, "�N���b�N�F�p�l��������", GetColor(255, 255, 255));
	DrawString(16, 32, "�X�y�[�X�F�����Ă���p�l���̏㉺���E�ɂ��铯���F�̃p�l��������", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);
	DeleteGraph(panel_image[2]);
	DeleteGraph(panel_image[3]);
}
