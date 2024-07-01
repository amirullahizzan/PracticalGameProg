#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"
#include <cmath>
//	�P�̎l�p�̑傫��
#define PANEL_SIZE	50
//	�l�p��u���鐔
#define MAP_W		32
#define	MAP_H		18

//	�Q�����z��i�O�F�i�߂�@�P�F�i�߂Ȃ��j
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },

	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1, },
	{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1, },
	{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1, },
	{ 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1, },
	{ 1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, },
};

//	�}�b�v�摜
int panel_image[2];

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_SPEED	5.0f

//	�v���C���[�摜
int player_image;
//	�v���C���[���W
Float2 player_pos;

int mapSection = 0;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	panel_image[0] = LoadGraph("data/panel0.bmp");
	panel_image[1] = LoadGraph("data/panel1.bmp");

	player_image = LoadGraph("data/player.bmp");
	player_pos.x = 600.0f;
	player_pos.y = 200.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�P�F�ړ��O�̍��W���Ƃ��Ă���
	Float2 before = player_pos;

	//	�Q�F�\���L�[�ō��W�ړ�
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		player_pos.x += PLAYER_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		player_pos.x -= PLAYER_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		player_pos.y += PLAYER_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_UP)) {
		player_pos.y -= PLAYER_SPEED;
	}

	//	�����蔻��p�Ƀv���C���[�̎l�p�i���������E���ɂ��炷�j
	Float2 box_pos(player_pos.x + 2.0f, player_pos.y + 2.0f);
	//	�v���C���[�̎l�p�̑傫���i�p�l���Ɠ����傫�����ƈ����������Ă��܂��̂Łj
	Float2 box_size(PANEL_SIZE - 4.0f, PANEL_SIZE - 4.0f);

	//	����p�Ƀp�l�����W�i���̒l�̓��[�v�̒��Ō��߂܂��j
	Float2 panel_pos;
	//	�p�l���̑傫��
	Float2 panel_size(PANEL_SIZE, PANEL_SIZE);

	//	�R�F�ړ�������̃v���C���[�Ɗe�u���b�N�œ����蔻��
	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			//	�ʂ��Ƃ���͔��肵�Ȃ�
			if (MapData[h][w] == 0)	continue;

			//	�p�l�����W��ݒ�
			panel_pos.x = w * PANEL_SIZE;
			panel_pos.y = h * PANEL_SIZE;

			//	�v���C���[�i�l�p�j�ƕǃp�l���i�l�p�j�œ����蔻��
			if (CheckBoxHit(box_pos, box_size, panel_pos, panel_size)) {
				//	�S�F�������Ă�������W��߂�
//				player_pos = before;
				//	��������{�b�N�X�̏��ɍ��W�������Ă���
				//	�E�Ɉړ����ē��������̂Ńu���b�N�̍��ɂ��܂�
				if (before.x < player_pos.x)	player_pos.x = panel_pos.x - PANEL_SIZE;
				if (before.x > player_pos.x)	player_pos.x = panel_pos.x + PANEL_SIZE;
				if (before.y < player_pos.y)	player_pos.y = panel_pos.y - PANEL_SIZE;
				if (before.y > player_pos.y)	player_pos.y = panel_pos.y + PANEL_SIZE;
			}
		}
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
Float2 Normalize(const Float2& v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y);
	return { v.x / length, v.y / length };
}

Float2 ApplyTransform(Float2 _pos, Float2& _currentOffset, Float2 _offsetTarget)
{
	if (GetFloat2Distance(_currentOffset, _offsetTarget) > 0.01f)
	{
		const float moveSpeed = 0.05f;
		Float2 direction = Normalize({ _offsetTarget.x - _currentOffset.x, _offsetTarget.y - _currentOffset.y });
		_currentOffset.x += direction.x * moveSpeed;
		_currentOffset.y += direction.y * moveSpeed;
	}
	return _pos - _currentOffset;
}

void GameRender() {
	static Float2 camOffset = { 0 ,0 };
	Float2 camOffsetTarget = { 0 ,0 };

	clsDx();
	printfDx("%f\n", camOffset.x);

	if (mapSection == 1)
	{
		camOffsetTarget = { SCREEN_W, 0 };
		
	}
	else if (mapSection == 0)
	{
		camOffsetTarget = { 0, 0 };
		
	}
	else if (mapSection == 3)
	{
		camOffsetTarget = { SCREEN_W, SCREEN_H };
	}
	else if (mapSection == 2)
	{
		camOffsetTarget = { 0, SCREEN_H };
	}

	// Update camOffset based on camOffsetTarget
	ApplyTransform(Float2{ 0, 0 }, camOffset, camOffsetTarget);

	// Map rendering
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			// Map tile number
			int num = MapData[h][w];
			// Coordinates to draw the tile
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			// Draw the tile at the transformed position
			Float2 pos = { (float)x, (float)y };
			Float2 screenPos = ApplyTransform(pos, camOffset, camOffsetTarget);
			DrawGraph(screenPos.x, screenPos.y, panel_image[num], TRUE);
		}
	}

	// Draw the player at the transformed position
	float px = player_pos.x;
	float py = player_pos.y;
	Float2 screenPos = ApplyTransform(Float2{ px, py }, camOffset, camOffsetTarget);

	DrawGraphF(screenPos.x, screenPos.y, player_image, TRUE);

	// Update mapSection based on player's position
	if (mapSection == 0 && screenPos.y > SCREEN_H)
	{
		mapSection = 2;
	}
	else if (mapSection == 2 && screenPos.x > SCREEN_W)
	{
		mapSection = 3;
	}
	else if (mapSection == 1 && screenPos.y < 0)
	{
		mapSection = 3;
	}
	else if (mapSection == 1 && screenPos.x < 0)
	{
		mapSection = 0;
	}
	else if (mapSection == 0 && screenPos.x > SCREEN_W)
	{
		mapSection = 1;
	}
	else if (mapSection == 3 && screenPos.y < 0)
	{
		mapSection = 1;
	}
	else if (mapSection == 1 && screenPos.y > SCREEN_H)
	{
		mapSection = 3;
	}
	else if (mapSection == 3 && screenPos.x < 0)
	{
		mapSection = 2;
	}
	

	DrawString(16, 16, "�\���L�[�F�v���C���[�ړ�", GetColor(255, 255, 255));
	DrawString(16, 32, "��ʂ̊O�ɍs������אڂ���}�b�v�ɐ؂�ւ���", GetColor(255, 255, 0));
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
