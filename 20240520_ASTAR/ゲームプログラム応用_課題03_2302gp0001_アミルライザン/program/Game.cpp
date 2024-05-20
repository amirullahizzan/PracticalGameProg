#include "Main.h"
#include "Game.h"
#include "Float2.h"

//	�p�l���T�C�Y�̓T���v���̔����ɂ��Ă��܂�
#define PANEL_SIZE	25
//	�T�C�Y���������Ȃ����̂Ŕz��̐����������₵�Ă��܂�
#define BASE_X	150
#define BASE_Y	105

#define MAP_W		20 
#define MAP_H		13


//	�p�l���摜�p�̕ϐ�
//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
int panel_image[2];

//	�m�o�b�摜
int npc_image;
//	���F���g�摜
int target_image;
//	���[�g�G���A�\���p�摜
int area_image;

//	�m�o�b������|�C���g�i���W�ł͂Ȃ��j
int npc_w;
int npc_h;
//	�ړI�n�̃|�C���g�i���W�ł͂Ȃ��j
int target_w;
int target_h;

//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
//	�T�C�Y���������Ȃ����̂Ŕz��̐����������₵�Ă��܂�
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

//	�P�F�m�o�b���ړ����邽�߂̔ԍ�������p�̕ϐ�
int Route[MAP_H][MAP_W];

//	�Q�|�R�F��̔ԍ�������p�̕ϐ�
int route_num;

//	�m�o�b��������蓮�������߂̃J�E���^�[
int frame_count;

//	�}�b�v�S�̂͂��̊�̏ꏊ����`������悤�Ǝv���܂�
#define BASE_X	150
#define BASE_Y	105

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");
	npc_image = LoadGraph("data/npc.bmp");
	target_image = LoadGraph("data/mark.bmp");
	//	�G���A�摜�̓ǂݍ��݂����łɂ��Ă��܂�
	area_image = LoadGraph("data/area.bmp");

	//	�m�o�b�̊J�n�|�C���g
	npc_w = 13;
	npc_h = 5;
	//	�Ƃ肠�����ړI�n�̃|�C���g
	target_w = 1;
	target_h = 1;

	//	���[�g�p�̕ϐ��ɂP�O�O�O�����ď�����
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�}�X�ɕ����񂪓��肫��Ȃ��̂� 1000 �ł͂Ȃ� 99 �ɂ��Ă��܂�
			Route[h][w] = 99;
		}
	}
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�N���b�N���邱�ƂŖړI�n�̐ݒ�Ȃ�
	if (PushMouseInput()) {
		//	�Q�|�P�F�������񂷂ׂẴ��[�g�̒l������U���Ă��Ȃ��l����J�n
		for (int h = 0; h < MAP_H; h++)
		{
			for (int w = 0; w < MAP_W; w++)
			{
				//	�}�X�ɕ����񂪓��肫��Ȃ��̂� 1000 �ł͂Ȃ� 99 �ɂ��Ă��܂�
				Route[h][w] = 99;
			}
		}
		//	�N���b�N�����Ƃ��낪�Q�����z��I�ɉ��Ԗڂ�
		target_h = (GetMouseY() / PANEL_SIZE) - (BASE_Y / PANEL_SIZE);
		target_w = (GetMouseX() / PANEL_SIZE) - (BASE_X / PANEL_SIZE);
		//	�Q�|�Q�F�N���b�N�����Ƃ��낪�ʂ��Ƃ��낾������
		if (MapData[target_h][target_w] == 0) {
			//	�Q�F�N���b�N�����Ƃ���̃��[�g�ԍ����O�ɂ��ĊJ�n
			Route[target_h][target_w] = 0;
			//	�Q�|�R�F��̔ԍ����O�ɂ��ĊJ�n
			route_num = 0;
		}

		for (int i = 0; i < 10; i++)
		{
			for (int h = 0; h < MAP_H; h++) {
				for (int w = 0; w < MAP_W; w++) {
					//	�R�F��̔ԍ�����������
					if (Route[h][w] == route_num) {
						//	�R�F�㉺���E�̏ꏊ�̃��[�g�ԍ����u��ԍ��{�P�v�ɂ��܂�
						//	�@�@�����Ƃ��Ēʂ��ꏊ�̃��[�g�ԍ����傫���Ƃ��i����ɗׂ��z��O�ɂȂ�Ȃ��Ƃ��j
						if (w < MAP_W - 1 && MapData[h][w + 1] == 0 && Route[h][w + 1] > route_num)	Route[h][w + 1] = route_num + 1;
						if (w > 0 && MapData[h][w - 1] == 0 && Route[h][w - 1] > route_num)	Route[h][w - 1] = route_num + 1;
						if (h < MAP_H - 1 && MapData[h + 1][w] == 0 && Route[h + 1][w] > route_num)	Route[h + 1][w] = route_num + 1;
						if (h > 0 && MapData[h - 1][w] == 0 && Route[h - 1][w] > route_num)	Route[h - 1][w] = route_num + 1;
					}
				}
			}
			//	�S�F���̃X�y�[�X�L�[���������Ƃ��̂��߂Ɋ�̔ԍ����P���₵�܂�
			route_num++;
		}
	}

	//if (route_num < 10)
	
	//	�X�y�[�X�L�[�Ń��[�g���L����
	//	�i�ł����A�ۑ�ł͂�����P�O��J��Ԃ������Ȃ��܂��̂ŃX�y�[�X�L�[�̔���͎g���܂���j
	if (PushHitKey(KEY_INPUT_SPACE)) {

	}

	frame_count++;
	//	���Ԋu�łm�o�b�̈ړ�
	if (frame_count % 10 == 0) {
		//	������Ƃ���̃��[�g�ԍ�
		int route_center = Route[npc_h][npc_w];
		//	�㉺���E�̃��[�g�ԍ�
		int route_right = Route[npc_h][npc_w + 1];
		int route_left = Route[npc_h][npc_w - 1];
		int route_down = Route[npc_h + 1][npc_w];
		int route_up = Route[npc_h - 1][npc_w];
		//	�m�o�b�͏��Ȃ��l�̕��Ɉړ�����
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
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�Q�����z��̒l�����ĉ摜����ׂ�
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�}�b�v�z�񂩂�擾�����ԍ�
			int num = MapData[h][w];
			//	�`�悷����W�i�p�l���T�C�Y�����炵���Ƃ���j
			int x = w * PANEL_SIZE + BASE_X;
			int y = h * PANEL_SIZE + BASE_Y;
			//	�p�l���摜��`��
			DrawGraph(x, y, panel_image[num], TRUE);

			//	�ʂ��Ƃ��낾��������̕`��
			if (num == 0)
			{
				//	���[�g�ϐ��̔ԍ��𕶎���ŕ`��
				DrawFormatString(x, y, GetColor(255, 255, 255), "%d", Route[h][w]);
			}

			if (Route[h][w] != 0 && Route[h][w] != 99)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);	//	�������ݒ�
				DrawGraph(x, y, area_image, TRUE);			//	�G���A�摜�̕`��
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//	�������ݒ��߂�
			}
		}
	}
	//	�m�o�b�̃|�C���g�����ʍ��W�����߂ĕ`��
	int nx = npc_w * PANEL_SIZE + BASE_X;
	int ny = npc_h * PANEL_SIZE + BASE_Y;
	DrawGraph(nx, ny, npc_image, TRUE);

	//	�ړI�n�̃|�C���g�����ʍ��W�����߂ĕ`��
	int tx = target_w * PANEL_SIZE + BASE_X;
	int ty = target_h * PANEL_SIZE + BASE_Y;
	DrawGraph(tx, ty, target_image, TRUE);

	//DrawString( 16, 16, "�N���b�N�F�ړI�n�ݒ� �� �P�O�}�X�����[�g�쐬", GetColor( 255, 255, 0 ) );
	//DrawString( 16, 32, "�P�O�}�X���̃��[�g��������悤�ɃG���A�摜��`��i�������ɂ��Ă��܂��j", GetColor( 255, 255, 0 ) );
	//DrawString( 16, 48, "���@���[�g���͂�����m�o�b�͖ړI�n�Ɍ������Ĉړ�����", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 80, "�}�b�v�S�̂���ʂ̐^�񒆂�����ɂ���i BASE_X BASE_Y ����̍��W�Ƃ��āj", GetColor( 255, 255, 0 ) );
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
	DeleteGraph(area_image);
}
