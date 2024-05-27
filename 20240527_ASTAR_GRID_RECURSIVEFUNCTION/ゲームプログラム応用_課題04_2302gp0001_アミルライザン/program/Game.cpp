#include "Main.h"
#include "Game.h"
#include "Float2.h"

//	�p�l���̑傫��
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		16
#define MAP_H		 9

#define BASE_X 375
#define BASE_Y 200
//	�p�l���摜�p�̕ϐ�
//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
int panel_image[2];

//	�m�o�b�摜
int npc_image;
//	�v���C���[�摜
int target_image;

//	�m�o�b������|�C���g�i���W�ł͂Ȃ��j
int npc_w;
int npc_h;
//	�v���C���[������|�C���g�i�ړI�n�F���W�ł͂Ȃ��j
int target_w;
int target_h;

//	�}�b�v�p�̂Q�����z��
//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
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

//	�m�o�b���ړ����邽�߂̔ԍ�������p�̕ϐ�
int Route[MAP_H][MAP_W];

//	�m�o�b��������蓮�������߂̃J�E���^�[
int frame_count;

//---------------------------------------------------------------------------------
//	���[�g�쐬�֐�
//	�i�����œn���ꂽ w h �Ԗڂ� Route[h][w] �̒l�� num �ɂ���j
//---------------------------------------------------------------------------------
void SetRoute(int w, int h, int num)
{
	//	����̃}�b�v�͎��肪�ǂł��邱�Ƃ��O��ł���
	//	���肪�ǂ���Ȃ��ꍇ�́A�Q�����z��̒����ǂ����̔�����K�v�ł�
	if (w < 0)			return;
	if (w >= MAP_W)	return;
	if (h < 0)			return;
	if (h >= MAP_H)	return;

	//	�ʂ�Ȃ��Ƃ���܂ōs������I��
	if (MapData[h][w] == 1)	return;

	//	�����œn���ꂽ�ꏊ�i w h �j�̃��[�g�ԍ���n���ꂽ�l�i num �j�ɐݒ�
	Route[h][w] = num;

	//	�P�E�̏ꏊ�i w + 1, h �j�̃��[�g�ԍ����P�傫���������́i num + 1 �j��
	//	���̂P�E�̃��[�g�ԍ��������犄��U�肽���l�����傫��������
	if (Route[h][w + 1] > num + 1)		SetRoute(w + 1, h, num + 1);	//	�E
	if (Route[h][w - 1] > num + 1)		SetRoute(w - 1, h, num + 1);	//	��
	if (Route[h + 1][w] > num + 1)		SetRoute(w, h + 1, num + 1);	//	��
	if (Route[h - 1][w] > num + 1)		SetRoute(w, h - 1, num + 1);	//	��
}

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	npc_image = LoadGraph("data/npc.bmp");
	//	���̃v���W�F�N�g�ł̓v���C���[�摜���g���܂�
	target_image = LoadGraph("data/player.bmp");

	//	�m�o�b�̊J�n�|�C���g
	npc_w = 13;
	npc_h = 5;
	//	���̃v���W�F�N�g�ł̓v���C���[�̂���|�C���g�ł�
	target_w = 1;
	target_h = 1;
	//	�J�E���^�[�͂O�ŊJ�n
	frame_count = 0;
	//	���[�g�p�̕ϐ��ɂP�O�O�O�����ď�����
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			Route[h][w] = 1000;
		}
	}
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
bool isPlayerTurn = true;
void GameUpdate()
{
	//	�\���L�[���������Ƃ��Ƀv���C���[�����̃|�C���g�ɐi�߂āi�ǂ���������ړ��͂��Ȃ��j
	//	�i�񂾂Ƃ��ɂm�o�b���P�񂾂�������
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
			//	�N���b�N�|�C���g���擾���܂��i�Ƃ肠����������ړI�n�ɂ��Ă��܂��j
			//	�i����͂��̏������g���܂���F�\���L�[�Ń|�C���g���ړ�������̂Łj
			//	�N���b�N�����|�C���g���ʂ��Ƃ��낾������
			if (MapData[target_h][target_w] == 0)
			{
				//	�������񂷂ׂẴ��[�g�̒l������U���Ă��Ȃ��ƕ�����l�ɂ��܂�
				for (int h = 0; h < MAP_H; h++) {
					for (int w = 0; w < MAP_W; w++) {
						//	���������傫�߂Ȑ��l��ݒ肵�܂�
						Route[h][w] = 1000;
					}
				}
				//	���[�g�̍쐬�����Ă����܂��i���[�g�� target_w �Ԗ� target_h �Ԗڂ̒l�� 0 �ɂ��܂��j
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
	//	���Ԋu�łm�o�b�̈ړ�
	if (frame_count % 30 == 0 && !isPlayerTurn) {
		//	�m�o�b��������|�C���g�̃��[�g�ԍ�
		int route_center = Route[npc_h][npc_w];
		//	�㉺���E�̃��[�g�ԍ�
		int route_right = Route[npc_h][npc_w + 1];
		int route_left = Route[npc_h][npc_w - 1];
		int route_down = Route[npc_h + 1][npc_w];
		int route_up = Route[npc_h - 1][npc_w];

		//	�㉺���E�̂ǂ����ŏ��Ɍ��Ă������������_���ɂ��Ă݂܂�
		//	�ǂ�����n�߂邩�̔ԍ��i�O�F�E�@�P�F���@�Q�F���@�R�F��j
		int start = GetRand(3);
		//	�㉺���E�̂S��J��Ԃ�
		for (int i = start; i < start + 4; i++) {
			//	���Ԗڂ����邩
			int num = i % 4;
			//	num ���O�̎��͉E�����܂��i����ɉE�̃��[�g�ԍ���������������ړ��ł���j
			if (num == 0 && route_center > route_right) { npc_w++;	break; }
			if (num == 1 && route_center > route_left) { npc_w--;	break; }
			if (num == 2 && route_center > route_down) { npc_h++;	break; }
			if (num == 3 && route_center > route_up) { npc_h--;	break; }
		}
		isPlayerTurn = true;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�}�b�v�̕`��
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	���̏ꏊ�̃}�b�v�ԍ����擾
			int num = MapData[h][w];
			//	���ۂ̕`����W���擾
			int x = w * PANEL_SIZE + BASE_X - (target_w * PANEL_SIZE);
			int y = h * PANEL_SIZE + BASE_Y - (target_h * PANEL_SIZE);
			//	�����̍��W�ɂ��̔ԍ��̎l�p�摜��`��
			DrawGraph(x, y, panel_image[num], TRUE);

			//	�ʂ��Ƃ��낾��
			if (num == 0) {
				//	���̏ꏊ�̃��[�g�ԍ��𕶎���ŕ`��
				DrawFormatString(x, y, GetColor(255, 255, 255), "%d", Route[h][w]);
			}
		}
	}
	//	�m�o�b�̂���|�C���g�����ʍ��W���v�Z���ĕ`��
	int nx = npc_w * PANEL_SIZE + BASE_X - (target_w * PANEL_SIZE);
	int ny = npc_h * PANEL_SIZE + BASE_Y - (target_h * PANEL_SIZE);
	DrawGraph(nx, ny, npc_image, TRUE);
	//	�ړI�n�̃|�C���g�����ʍ��W���v�Z���ĕ`��

	int tx = BASE_X;
	int ty = BASE_Y;
	DrawGraph(tx, ty, target_image, TRUE);

	DrawString(16, 16, "�\���L�[�F�v���C���[�ړ��i�ǂ���������i�߂Ȃ��E�m�o�b���ړ�������ɍĂѓ�����j", GetColor(255, 255, 0));
	DrawString(16, 32, "�m�o�b�̓v���C���[�Ɍ������Ĉړ� �� �v���C���[���P�񓮂�����P�b��ɂm�o�b���P�񓮂�", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(npc_image);
	DeleteGraph(target_image);
}
