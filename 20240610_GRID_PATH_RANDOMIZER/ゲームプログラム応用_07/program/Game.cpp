#include "Main.h"
#include "Game.h"
#include "Random.h"

//	�摜�T�C�Y
#define PANEL_SIZE	25
//	�Q�����z��ŉ��ɕ��ׂ鐔
#define MAP_W		31
//	�c�̐�
#define MAP_H		17

//	�e�}�X�ɕ`�悷��摜
int panel_image[2];

//	�}�b�v
int MapData[MAP_H][MAP_W];

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			MapData[h][w] = 0;

		}
	}
}

//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		for (int h = 0; h < MAP_H; h++)
		{
			for (int w = 0; w < MAP_W; w++)
			{
				MapData[h][w] = 0;

				if (w % 2 == 0 && h % 2 == 0)
				{
					MapData[h][w] = -1;
				}

				if (w == 0)			MapData[h][w] == 1;
				if (w == 0)			MapData[h][w] == 1;
				if (w == MAP_W - 1) MapData[h][w] == 1;
				if (w == MAP_H - 1) MapData[h][w] == 1;
			}
		}

		for (int h = 1; h < MAP_H - 1; h++)
		{
			for (int w = 1; w < MAP_W - 1; w++)
			{
				if (MapData[h][w] == -1)
				{
					//MapData[h][w] = 1;

					int randomMax = 3;

					if (h == 2)
					{
						randomMax = 3;
					}
					else
					{
						randomMax = 2;
					}

					while (true)
					{
						int random = GetRandomI(randomMax);
						int w1 = w;
						int h1 = h;

						if (random == 0)	w1 += 1;
						if (random == 1)	h1 += 1;
						if (random == 2)    w1 -= 1;
						if (random == 3)    h1 -= 1;

						if (MapData[h1][w1] == 0)
						{
							MapData[h1][w1] = 1;
							break;
						}
					}
				}
			}
		}
	}

}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�}�b�v�ԍ����擾
			int num = MapData[h][w];
			//	�`����W�̌v�Z
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	�摜�̕`��
			DrawGraph(x, y, panel_image[num], TRUE);
		}
	}

	DrawString(16, 16, "�X�y�[�X�F�}�b�v�̍쐬", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);
}
