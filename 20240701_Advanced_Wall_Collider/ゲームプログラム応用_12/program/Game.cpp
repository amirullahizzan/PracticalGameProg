#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"

//	�P�̎l�p�̑傫��
#define PANEL_SIZE	50

#define PANEL_HALF	(PANEL_SIZE/2)
//	�l�p��u���鐔
#define MAP_W		16
#define	MAP_H		 9

//	�Q�����z��i�O�F�i�߂�@�P�F�i�߂Ȃ��j
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	�}�b�v�摜
int panel_image[2];

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	15.0f
//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	3.0f

//	�v���C���[�摜
int player_image;
//	�v���C���[���W
Float2 player_pos;
//	�v���C���[�̌���
float player_rot;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	player_image = LoadGraph("data/player.bmp");
	//	�������W������̐ݒ�
	player_pos.x = 100.0f;
	player_pos.y = 100.0f;
	player_rot = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	Float2 before = player_pos;
	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot -= PLAYER_ROT_SPEED;
	//	��L�[�Ō����Ă�������Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_pos.x += PLAYER_MOV_SPEED * cosf(TO_RADIAN(player_rot));
		player_pos.y += PLAYER_MOV_SPEED * sinf(TO_RADIAN(player_rot));
	}

	Float2 p_pos(player_pos.x - 23.0f, player_pos.y - 23.0f);
	Float2 p_size(PANEL_SIZE - 4.0f, PANEL_SIZE - 4.0f);

	//used for advanced wall
	Float2 box_before(before.x - 23.0f, before.y - 23.0f);

	Float2 panel_pos;
	Float2 panel_size(PANEL_SIZE, PANEL_SIZE);

	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			if (MapData[h][w] == 0) continue;

			panel_pos.x = w * PANEL_SIZE;
			panel_pos.y = h * PANEL_SIZE;

			if (CheckBoxHit(p_pos, p_size, panel_pos, panel_size))
			{

				//simple wall
				//player_pos = before;

				//advanced wall
				if (box_before.y + p_size.y >= panel_pos.y && box_before.y <= panel_pos.y + panel_size.y)
				{
					//player_pos.x = before.x; //this returns player incorrectly if in high speed
					if (before.x < player_pos.x) player_pos.x = max(before.x, panel_pos.x - PANEL_HALF);
					if (before.x > player_pos.x) player_pos.x = min(before.x, panel_pos.x + PANEL_SIZE + PANEL_HALF);
				}
				else if (box_before.x + p_size.x >= panel_pos.x && box_before.x <= panel_pos.x + panel_size.x)
				{
					//player_pos.y = before.y; //this returns player incorrectly if in high speed
					if (before.y < player_pos.y) player_pos.y = max(before.y, panel_pos.y - PANEL_HALF);
					if (before.y > player_pos.y) player_pos.y = min(before.y, panel_pos.y + PANEL_SIZE + PANEL_HALF);
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
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�����̏ꏊ�̔ԍ�
			int num = MapData[h][w];
			//	�l�p��`�悷����W
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	�����ɂ��̔ԍ��̎l�p�̕`��
			DrawGraph(x, y, panel_image[num], TRUE);
		}
	}

	DrawRotaGraphF(player_pos.x, player_pos.y, 1.0f, TO_RADIAN(player_rot), player_image, TRUE);

	DrawString(16, 16, "�����L�[�F�v���C���[��]", GetColor(255, 255, 255));
	DrawString(16, 32, "���L�[�@�F�v���C���[�ړ�", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(panel_image[0]);
	DeleteGraph(panel_image[1]);

	DeleteGraph(player_image);
}
