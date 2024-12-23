#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�m�o�b�̐��i�u���W�ۑ�v�Ŏg���萔�ł��j
#define NPC_MAX	5

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model[NPC_MAX];

//	�A�j���[�V�����f�[�^�p�̕ϐ�
int anim_model;

Float3	player_pos;
Float3	player_rot;
//	�v���C���[�̃A�j���[�V�����֌W
int player_attach;	//	�A�^�b�`�ԍ�
float player_total;	//	�A�j���[�V�����̃g�[�^������
float player_frame;	//	�A�j���[�V��������

Float3	npc_pos[NPC_MAX];
Float3	npc_rot[NPC_MAX];
int   npc_attach[NPC_MAX];	//	�A�^�b�`�ԍ�
float npc_total[NPC_MAX];	//	�A�j���[�V�����̃g�[�^������
float npc_frame[NPC_MAX];	//	�A�j���[�V��������
int npc_image_num[NPC_MAX];

int punch_anim_model;

//	�m�o�b���f���ɓ\��t����e�N�X�`���̐��i�S���łU�j
#define IMAGE_MAX	6
int sample_image[IMAGE_MAX];

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�����ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -5.0f), VGet(0.0f, 1.0f, -1.0f));

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/chara/model.mv1");
	//npc_model = MV1LoadModel( "data/npc.mqoz" );
	for (int i = 0; i < NPC_MAX; i++)
	{
		npc_model[i] = MV1DuplicateModel(player_model);
	}

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model = MV1LoadModel("data/chara/anim_walk.mv1");
	punch_anim_model = MV1LoadModel("data/chara/anim_punch.mv1");

	////////////////////////
	for (int i = 0; i < IMAGE_MAX; i++)
	{
		char text[256];
		sprintf(text, "data/sample%d.png", i);
		sample_image[i] = LoadGraph(text);
	}

	player_pos.set(1.5f, 0.0f, -1.0f);
	player_rot.y = 220.0f;

	//	�v���C���[�A�j���[�V�����̏����ݒ�
	player_attach = MV1AttachAnim(player_model, 1, anim_model, FALSE);
	player_total = MV1GetAttachAnimTotalTime(player_model, player_attach);
	player_frame = 0.0f;

	for (int i = 0; i < NPC_MAX; i++)
	{
		npc_pos[i].set(-1.5f, 0.0f, -1.0f);
		npc_rot[i].set(0.0f, 130.0f, 0.0f);

		npc_attach[i] = MV1AttachAnim(npc_model[i], 1, punch_anim_model, FALSE);
		npc_total[i] = MV1GetAttachAnimTotalTime(npc_model[i], npc_attach[i]);
		npc_frame[i] = 0.0f;

		npc_image_num[i] = 0;
		MV1SetTextureGraphHandle(npc_model[i], 2, sample_image[npc_image_num[i]], TRUE);
	}
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		for (int i = 0; i < NPC_MAX; i++)
		{

			npc_image_num[i] += GetRandomI(1, IMAGE_MAX - 1);
			npc_image_num[i] %= IMAGE_MAX;

			MV1SetTextureGraphHandle(npc_model[i], 2, sample_image[npc_image_num[i]], TRUE);
		}
	}

	//	�A�j���[�V�����̍Đ�
	player_frame += 1.0f;
	if (player_frame >= player_total) {
		player_frame = 0.0f;
	}
	//	���̃t���[���̃A�j���[�V�����ɂ���
	MV1SetAttachAnimTime(player_model, player_attach, player_frame);

	for (int i = 0; i < NPC_MAX; i++)
	{
		//	�A�j���[�V�����̍Đ�
		npc_frame[i] += 1.0f;
		if (npc_frame[i] >= npc_total[i]) {
			npc_frame[i] = 0.0f;
		}
		//	���̃t���[���̃A�j���[�V�����ɂ���
		MV1SetAttachAnimTime(npc_model[i], npc_attach[i], npc_frame[i]);
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel(ground_model);

	//	�m�o�b���f���̕`��
	for (int i = 0; i < NPC_MAX; i++)
	{
		MV1SetPosition(npc_model[i], VGet(npc_pos[i].x, npc_pos[i].y, npc_pos[i].z));
		MV1SetRotationXYZ(npc_model[i], VGet(TO_RADIAN(npc_rot[i].x), TO_RADIAN(npc_rot[i].y + 180.0f), TO_RADIAN(npc_rot[i].z)));
		MV1SetScale(npc_model[i], VGet(0.01f, 0.01f, 0.01f));
		MV1DrawModel(npc_model[i]);
	}

	//	�v���C���[���f���̕`��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	MV1DrawModel(player_model);

	DrawString(16, 16, "�m�o�b���v���C���[�Ɠ������f�����g�p�i�ʃ��f���Ƃ��āE�e�N�X�`�����ʂŊJ�n�j", GetColor(255, 255, 0));
	DrawString(16, 32, "���@�p���`�A�j���[�V�����̍Đ����J��Ԃ�", GetColor(255, 255, 0));
	DrawString(16, 48, "�X�y�[�X�F�m�o�b���f���̃e�N�X�`���؂�ւ��i�����_���ɂO�`�T�F�����e�N�X�`���ɂ͂Ȃ�Ȃ��j", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	for (int i = 0; i < NPC_MAX; i++)
	{
		MV1DeleteModel(npc_model[i]);
	}

	//	�A�j���[�V�����f�[�^�̏I������
	MV1DeleteModel(anim_model);
}
