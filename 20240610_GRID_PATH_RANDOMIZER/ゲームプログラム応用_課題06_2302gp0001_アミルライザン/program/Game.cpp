#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	�p�l���T�C�Y�i����̃��f���f�[�^�̑傫���� 1.0f �ł��j
#define PANEL_SIZE	1.0f

//	�}�b�v�̃T�C�Y
#define MAP_W		37
#define MAP_H		25
//	�}�b�v�f�[�^
int MapData[MAP_H][MAP_W];

//	���f���f�[�^�ԍ��p�̕ϐ�
int panel_model[2];
FLOAT3 camPos = { 0,-20,-2 };

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -5.0f), VGet(0.0f, 0.0f, 0.0f));
	//	���f���f�[�^�̓ǂݍ���
	panel_model[0] = MV1LoadModel("data/panel0.mqoz");
	panel_model[1] = MV1LoadModel("data/panel1.mqoz");

	//	�}�b�v�̏����ݒ�
	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			//	��������O������
			MapData[h][w] = 0;
		}
	}
}

//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�X�y�[�X�L�[����������}�b�v���쐬
	if (CheckHitKey(KEY_INPUT_W))
	{
		camPos.y -= 0.1f;
	}
	else if (CheckHitKey(KEY_INPUT_S))
	{
		camPos.y += 0.1f;
	}

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		//	�}�b�v�̏����ݒ�
		for (int h = 0; h < MAP_H; h++) {
			for (int w = 0; w < MAP_W; w++) {
				//	��������ǂ��Ȃ��i 0 �j�ɂ��܂�
				MapData[h][w] = 0;
				//	�Q�F�ЂƂ�΂��Œ��i -1 �j�ɂ���
				if (w % 3 == 0 && h % 3 == 0)
				{
					//	���p�̔ԍ��i -1 �j������U�낤�Ǝv���܂�
					//	�i���̒���|������ǁi 0 �j�ɂ��Ă����j
					MapData[h][w] = -1;
				}
				//	�P�F����͂��ׂĕǁi 1 �j��
				if (w == 0)			MapData[h][w] = 1;
				if (h == 0)			MapData[h][w] = 1;
				if (w == MAP_W - 1)	MapData[h][w] = 1;
				if (h == MAP_H - 1)	MapData[h][w] = 1;
			}
		}

		//	�}�b�v��S�����Ă����āA������������㉺���E�̂ǂ����P�����Ƃ��̒���ǂɂ��܂�
		for (int h = 3; h < MAP_H - 3; h++)
		{
			for (int w = 3; w < MAP_W - 3; w++)
			{
				//	������������
				if (MapData[h][w] == -1) 
				{
					//	���̒���ǂɂ��܂�
					MapData[h][w] = 1;

					//	��ԏ�Ƃ���ȊO�̏��Ŏ擾���郉���_���̒l��ς��܂�
					int random_max = 3;
					if (h == 2)
					{
						//	��ԏ�̒��ł͂O�`�R�̃����_�����~����
						random_max = 3;
					}
					else {
						//	����ȊO�̏��ł͂O�`�Q�̃����_�����~����
						random_max = 2;
					}

					//	�ǂ���Ȃ��Ƃ����ǂɂ������̂ŁA�ȉ��̏�����ǂ���Ȃ��Ƃ����������܂ŌJ��Ԃ�
					while (true)
					{
						//	�����_���ŏ㉺���E�̂ǂ�����ǂɂ��܂�
						//	�O�F�E�@�P�F���@�Q�F���@�R�F��
						int random = GetRandomI(random_max);
						//	�ƂȂ�̔ԍ�������p�̕ϐ�
						int w1 = w;
						int h1 = h;

						int w2 = w;
						int h2 = h;
						//	�����_���̒l�ɍ��킹�Ă��ׂ̗̃}�X�̏ꏊ�̔ԍ������߂܂�
						if (random == 0) { w1 += 1;  w2 += 2; }
						if (random == 1) { h1 += 1;  h2 += 2; }
						if (random == 2) { w1 -= 1;  w2 -= 2; }
						if (random == 3) { h1 -= 1;  h2 -= 2; }

						//	�������ǂ���Ȃ�������
						if (MapData[h1][w1] == 0)
						{
							//	���̏ꏊ��ǂɂ��܂�	
							MapData[h1][w1] = 1;
							MapData[h2][w2] = 1;
							//	���̃��[�v�𔲂���
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
	//	�}�b�v�̕`��
	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			//	�}�b�v�ԍ��̎擾
			int num = MapData[h][w];
			//	�`�悷�邽�߂̍��W��ݒ�

			SetCameraPositionAndAngle(camPos, TO_RADIAN(-35.0f), 0, 0);

			FLOAT3 startPos = { w - 18.0f, h - 12,  20.0f };
			FLOAT3 rotation = { TO_RADIAN(-90.0f), 0.0f, 0.0f };

			if (num != 1)
			{
				MV1SetRotationXYZ(panel_model[0], rotation);
				MV1SetPosition(panel_model[0], startPos);
				MV1DrawModel(panel_model[0]);
			}
			else
			{
				MV1SetRotationXYZ(panel_model[1], rotation);
				MV1SetPosition(panel_model[1], startPos);
				MV1DrawModel(panel_model[1]);
			}

			//	���f���̕`����W��ݒ�

			//	���f���̕`��

		}
	}

	DrawString(16, 16, "�X�y�[�X�F�}�b�v�̍쐬", GetColor(255, 255, 255));
	DrawString(16, 32, "���@�R�c�̎l�p���f�����g���ĕ`��", GetColor(255, 255, 0));

	//	�ǂ�ȃ��f�������m�F���邽�߂����ɕ`�悵�Ă��܂��̂Ŏ�������ۂ͂������牺�̕`�揈���̓R�����g�ɂ��܂�

	//	���f���̕`����W��ݒ�
	//MV1SetPosition( panel_model[0], VGet( -1.0f, 0.0f, 0.0f ) );
	//	���f���̕`��
	//MV1DrawModel( panel_model[0] );

	//	���f���̕`����W��ݒ�
	//MV1SetPosition( panel_model[1], VGet( 1.0f, 0.0f, 0.0f ) );
	//	���f���̕`��
	//MV1DrawModel( panel_model[1] );

	DrawString(200, 270, "���f���m�O�n�F�ʂ��", GetColor(255, 255, 255));
	DrawString(450, 270, "���f���m�P�n�F�ʂ�Ȃ�", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(panel_model[0]);
	MV1DeleteModel(panel_model[1]);
}
