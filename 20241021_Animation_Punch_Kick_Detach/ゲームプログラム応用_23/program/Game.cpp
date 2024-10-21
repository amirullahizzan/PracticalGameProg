#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f
#define ANIM_RATE_SPEED		0.05f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�J�������W�ƌ���
Float3 camera_pos;
Float3 camera_rot;

//	�A�j���[�V�����̎��
enum EAnimState
{
	ANIM_WALK,
	ANIM_PUNCH,

	ANIM_MAX,		//	�A�j���[�V������
};
EAnimState playing_anim = ANIM_WALK;
//	�A�j���[�V�����f�[�^�p�̕ϐ�
int anim_model[ANIM_MAX];
//	�A�^�b�`�ԍ��p�̕ϐ�
int anim_attach[ANIM_MAX];
//	���̃A�j���[�V�����̂P���[�v�ɂ����鎞��
float anim_total[ANIM_MAX];
//	�A�j���[�V�������ǂ̂��炢�i�߂邩�̎���
float anim_time[ANIM_MAX];
//	���ꂼ��̃A�j���[�V�������ǂ̂��炢�̊����ɂ��邩�i 0.0f �` 1.0f �j
float anim_rate[ANIM_MAX];

//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�̏����ݒ�
	camera_pos.set(0.0f, 2.0f, -3.75f);
	//	�J�����̌���
	camera_rot.set(13.0f, 0.0f, 0.0f);

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	//	�v���C���[���f���i���łɃA�j���[�V������t������ǂݍ���ł��܂��j
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model[ANIM_WALK] = MV1LoadModel("data/player/anim_walk.mv1");
	anim_model[ANIM_PUNCH] = MV1LoadModel("data/player/anim_punch.mv1");

	//	�������W������̐ݒ�
	player_pos.set(1.5f, 0.0f, 0.0f);
	player_rot.y = 230.0f;

	npc_pos.set(-1.5f, 0.0f, 1.5f);

	//	�A�j���[�V�����̏�����
	for (int i = 0; i < ANIM_MAX; i++) {
		//	���f���ɃA�j���[�V�������A�^�b�`���܂��i�A�^�b�`�ԍ����擾�j
		anim_attach[i] = MV1AttachAnim(player_model, 1, anim_model[i], FALSE);
		//	���̃A�j���[�V�����̂P���[�v�ɂ����鎞��
		anim_total[i] = MV1GetAttachAnimTotalTime(player_model, anim_attach[i]);
		//	�ŏ�����J�n
		anim_time[i] = 0.0f;
		//	�������񊄍����O������
		anim_rate[i] = 0.0f;
	}
	anim_rate[ANIM_WALK] = 1.0f;

	MV1DetachAnim(player_model, anim_attach[ANIM_PUNCH]);
	anim_attach[ANIM_PUNCH] = -1;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (CheckHitKey(KEY_INPUT_UP)) {
		anim_rate[ANIM_WALK] = 0.0f;
		anim_rate[ANIM_PUNCH] = 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		anim_rate[ANIM_WALK] = 1.0f;
		anim_rate[ANIM_PUNCH] = 0.0f;
	}

	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;


	switch (playing_anim)
	{
	case ANIM_WALK:
		if (PushHitKey(KEY_INPUT_SPACE))
		{
			if (anim_attach[ANIM_PUNCH] == -1)
			{
				playing_anim = ANIM_PUNCH;
				anim_attach[ANIM_PUNCH] = MV1AttachAnim(player_model, 1, anim_model[ANIM_PUNCH], false);
				anim_time[ANIM_PUNCH] = 0.0f;
				anim_rate[ANIM_PUNCH] = 0.0f;
			}
		}
		break;

	case ANIM_PUNCH:
		anim_rate[ANIM_WALK] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_PUNCH] += ANIM_RATE_SPEED;

		if (anim_rate[ANIM_PUNCH] >= 1.0f)
		{
			anim_time[ANIM_PUNCH] += 1.0f;

			if (anim_time[ANIM_PUNCH] >= anim_total[ANIM_PUNCH])
			{
				playing_anim = ANIM_WALK;
			}
		}

		break;

	default:
		break;
	}



	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	�A�j���[�V�����̊����� 0.0f �` 1.0f �̊Ԃɂ���
		anim_rate[i] = max(0.0f, min(anim_rate[i], 1.0f));

		//	�A�j���[�V�������Ԃ�i�߂�
		anim_time[i] += 1.0f;
		//	�A�j���[�V�������Ԃ��Ō�܂ōs������ŏ�����
		if (anim_time[i] >= anim_total[i]) {
			anim_time[i] = 0.0f;
		}
		//	���̎��Ԃ̃A�j���[�V�����ɂ���
		MV1SetAttachAnimTime(player_model, anim_attach[i], anim_time[i]);
		//	���ꂼ��̃A�j���[�V�����̊������̍Đ��ɂ���
		MV1SetAttachAnimBlendRate(player_model, anim_attach[i], anim_rate[i]);
	}

	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	���W�ƌ�����n���ăJ�����̐ݒ�
	SetCameraPositionAndAngle(camera_pos.VGet(), TO_RADIAN(camera_rot.x), TO_RADIAN(camera_rot.y), TO_RADIAN(camera_rot.z));

	//	�n�ʃ��f���̕`��
	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	�v���C���[���f���̕`��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	//	����̃��f���͂y�}�C�i�X�����Ɍ����Ă���̂ŕ`�悷��Ƃ��ɂx���ɑ΂��ĂP�W�O�x��]�����܂�
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	//	����̃��f���͂߂���߂���ł����̂ŏ��������܂�
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	MV1DrawModel(player_model);

	DrawString(16, 16, "���E�L�[�F�v���C���[�̌����̕ύX", GetColor(255, 255, 255));
	DrawString(16, 32, "�X�y�[�X�F�p���`�A�j���[�V����", GetColor(255, 255, 255));
	DrawString(16, 48, "�㉺�L�[�F�Ƃ肠�����A�j���[�V������ς��Ă݂܂��i�m�F�����炷���R�����g�ɂ��܂��j", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	for (int i = 0; i < ANIM_MAX; i++) {
		MV1DeleteModel(anim_model[i]);
	}
}
