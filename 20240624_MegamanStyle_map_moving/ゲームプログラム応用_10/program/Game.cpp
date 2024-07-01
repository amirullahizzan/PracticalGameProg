#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"
#include <cmath>

//	�p�l���̑傫��
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		16
#define MAP_H		 9

//	�p�l���摜�p�̕ϐ�
//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
int panel_image[2];

//	�}�b�v�p�̂Q�����z��
//	�O�F�ʂ��
//	�P�F�ʂ�Ȃ�
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

//	�v���C���[�摜
int player_image;

int player_w;
int player_h;

Float2 player_pos;
Float2 player_target_pos;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	player_image = LoadGraph("data/player.bmp");
	player_w = 1;
	player_h = 1;

	player_pos.x = player_w * PANEL_SIZE;
	player_pos.y = player_h * PANEL_SIZE;

	player_target_pos.x = player_pos.x;
	player_target_pos.y = player_pos.y;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
Float2 Normalize(const Float2& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	return { v.x / length, v.y / length };
}

bool isMoving = false;
void GameUpdate()
{
	if (!isMoving)
	{
		if (PushHitKey(KEY_INPUT_RIGHT)) {
			if (MapData[player_h][player_w + 1] == 0) {
				player_w++;
			}
		}
		else if (PushHitKey(KEY_INPUT_LEFT)) {
			if (MapData[player_h][player_w - 1] == 0) {
				player_w--;
			}
		}
		else if (PushHitKey(KEY_INPUT_DOWN)) {
			if (MapData[player_h + 1][player_w] == 0) {
				player_h++;
			}
		}
		else if (PushHitKey(KEY_INPUT_UP)) {
			if (MapData[player_h - 1][player_w] == 0) {
				player_h--;
			}
		}
	}


	player_target_pos.x = player_w * PANEL_SIZE;
	player_target_pos.y = player_h * PANEL_SIZE;

	if (GetFloat2Distance(player_pos, player_target_pos) > 0.01f)
	{
		const float moveSpeed = 3.0f;
		Float2 direction = Normalize({ player_target_pos.x - player_pos.x, player_target_pos.y - player_pos.y });
		player_pos.x += direction.x * moveSpeed;
		player_pos.y += direction.y * moveSpeed;
		isMoving = true;
	}
	else
	{
		isMoving = false;
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
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	�����̍��W�ɂ��̔ԍ��̎l�p�摜��`��
			DrawGraph(x, y, panel_image[num], TRUE);
		}
	}


	DrawGraphF(player_pos.x, player_pos.y, player_image, TRUE);

	DrawString(16, 16, "�\���L�[�F�v���C���[�ړ�", GetColor(255, 255, 255));
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
