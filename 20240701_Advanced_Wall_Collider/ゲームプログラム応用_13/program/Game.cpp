#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	�P�̎l�p�̑傫��
#define PANEL_SIZE	1.0f
//	�����̑傫��
#define PANEL_HALF	(PANEL_SIZE/2.0f)

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

int ground_model;
//	�}�b�v���f��
int panel_model[2];

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.1f
//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	3.0f

//	�v���C���[���f��
int player_model;
//	�v���C���[���W
Float3 player_pos;
//	�v���C���[�̌���
Float3 player_rot;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(7.5f, 11.0f, -5.0f), VGet(7.5f, 0.0f, 3.0f));

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");

	panel_model[0] = MV1LoadModel("data/panel0.mqoz");
	panel_model[1] = MV1LoadModel("data/panel1.mqoz");

	player_model = MV1LoadModel("data/player.mqoz");
	//	�������W������̐ݒ�
	player_pos.x = 2.0f;
	player_pos.z = 2.0f;
	player_rot.clear();
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	Float3 before = player_pos;

	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�Ō����Ă�������Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_pos.x += PLAYER_MOV_SPEED * sinf(TO_RADIAN(player_rot.y));
		player_pos.z += PLAYER_MOV_SPEED * cosf(TO_RADIAN(player_rot.y));
	}

	Float3 player_size(PANEL_HALF - 0.1f, PANEL_HALF - 0.1f, PANEL_HALF - 0.1f);
	Float3 panel_pos;
	Float3 panel_size(PANEL_HALF , PANEL_HALF, PANEL_HALF);

	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {

			//	�}�b�v�ԍ����O�ԂƂ͓����蔻��͂��܂���
			if (MapData[h][w] == 0)	continue;

			panel_pos.x = w * PANEL_SIZE;
			panel_pos.z = h * PANEL_SIZE;

			if (CheckBoxHit3D(player_pos, player_size, panel_pos, panel_size))
			{
				if (before.z + panel_size.z >= panel_pos.z - panel_size.z && before.z - player_size.z <= panel_pos.z + panel_size.z)
				{
					if (before.x < player_pos.x) player_pos.x = max(before.x , panel_pos.x - PANEL_SIZE);
					if (before.x > player_pos.x) player_pos.x = min(before.x , panel_pos.x + PANEL_SIZE);
				}
				if (before.x + panel_size.x >= panel_pos.x - panel_size.x && before.x - player_size.x <= panel_pos.x + panel_size.x)
				{
					if (before.z < player_pos.z) player_pos.z = max(before.z, panel_pos.z - PANEL_SIZE);
					if (before.z > player_pos.z) player_pos.z = min(before.z, panel_pos.z + PANEL_SIZE);
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
	DrawSphere3D(VGet(0.0f, 0.0f, 0.0f), 0.2f, 100, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	//	�}�b�v�̕`��
	for (int h = 0; h < MAP_H; h++) {
		for (int w = 0; w < MAP_W; w++) {
			//	�}�b�v�ԍ��̎擾
			int num = MapData[h][w];
			//	�`�悷�邽�߂̍��W��ݒ�
			Float3 pos;
			pos.x = w * PANEL_SIZE;
			pos.y = 0.0f;
			pos.z = h * PANEL_SIZE;

			MV1SetPosition(panel_model[num], pos.VGet());
			MV1DrawModel(panel_model[num]);
		}
	}


	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	Float3 pos = player_pos;

	pos.x = round(player_pos.x / PANEL_SIZE) * PANEL_SIZE;
	pos.z = round(player_pos.z / PANEL_SIZE) * PANEL_SIZE;

	DrawBox3D_XZ(pos, PANEL_HALF, PANEL_HALF, GetColor(255,255,0));

	DrawString(16, 16, "�\���L�[�F�v���C���[��]�E�ړ�", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(panel_model[0]);
	MV1DeleteModel(panel_model[1]);
	MV1DeleteModel(player_model);
}
