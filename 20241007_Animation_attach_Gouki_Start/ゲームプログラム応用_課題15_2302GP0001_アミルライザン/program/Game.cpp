#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�����Ă���Ƃ��̈ړ��X�s�[�h
#define PLAYER_WALK_SPEED	0.025f
//	�����Ă���Ƃ��̈ړ��X�s�[�h
#define PLAYER_RUN_SPEED	0.055f
//	��]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f

//	�v���C���[�Ƃm�o�b�̋�����������Z�������瑖��n�߂�
#define RUN_DISTANCE		5.0f

//	�v���C���[�Ƃm�o�b�͈̔͗p�̔��a�i���W�ۑ�Ŏg���܂��j
#define CHARA_RADIUS		0.75f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�A�j���[�V�����̎��
enum {
	ANIM_STAND,		//	�����A�j��
	ANIM_WALK,		//	�����A�j��
	ANIM_RUN,		//	�����A�j��

	ANIM_MAX,		//	�A�j���[�V������
};
//	�A�j���[�V�����f�[�^�p�̕ϐ�
int anim_model[ANIM_MAX];
//	�A�^�b�`�ԍ��p�̕ϐ�
int anim_attach[ANIM_MAX];
//	���̃A�j���[�V������1���[�v�ɂ����鎞��
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

int player_circle_color = GetColor(0, 0, 255);
//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -7.0f), VGet(0.0f, 1.0f, 0.0f));

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	//	�v���C���[���f���i���łɃA�j���[�V������t������ǂݍ���ł��܂��j
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model[ANIM_STAND] = MV1LoadModel("data/player/anim_stand.mv1");
	anim_model[ANIM_WALK] = MV1LoadModel("data/player/anim_walk.mv1");
	anim_model[ANIM_RUN] = MV1LoadModel("data/player/anim_run.mv1");

	//	�������W������̐ݒ�
	player_pos.set(3.0f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-3.0f, 0.0f, 2.0f);

	//	�A�j���[�V�����̐����������܂�
	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	���f���ɃA�j���[�V�������A�^�b�`���܂��i�A�^�b�`�ԍ����擾�j
		anim_attach[i] = MV1AttachAnim(player_model, 1, anim_model[i], FALSE);
		//	���̃A�j���[�V������1���[�v�ɂ����鎞��
		anim_total[i] = MV1GetAttachAnimTotalTime(player_model, anim_attach[i]);
		//	�ŏ�����J�n
		anim_time[i] = 0.0f;
		//	�������񊄍����O������
		anim_rate[i] = 0.0f;
	}
	//	�����A�j���[�V��������J�n���悤�Ǝv���̂ł��̊����� 1.0f �ɂ��܂�
	anim_rate[ANIM_STAND] = 1.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
bool IsHitCircleCircle(float x1, float y1, float x2, float y2, float r1, float r2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;

	float distanceSquared = dx * dx + dy * dy;

	float r = r1 + r2;
	float rSum = r * r;

	return distanceSquared < rSum;
}

void GameUpdate()
{

	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�������Ă���������Ă�������Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Float3 base(0.0f, 0.0f, PLAYER_WALK_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);

		static constexpr float speedMult = 2.5f;
		float distance = GetFloat3Distance(player_pos, npc_pos);
		if (distance <= CHARA_RADIUS + CHARA_RADIUS)
		{
			Float3 diff = player_pos - npc_pos;
			float length = CHARA_RADIUS + CHARA_RADIUS - distance;
			diff.SetLength(length);

			player_pos += diff;
		}

		if (IsHitCircleCircle(player_pos.x, player_pos.z, npc_pos.x, npc_pos.z, RUN_DISTANCE, 0.1f) && distance > CHARA_RADIUS + CHARA_RADIUS)
		{
			player_circle_color = GetColor(255, 255, 255);

			player_pos += mov * speedMult;

			anim_rate[ANIM_WALK] -= 0.1f;
			anim_rate[ANIM_STAND] -= 0.1f;
			anim_rate[ANIM_RUN] += 0.1f;
		}
		else
		{
			player_pos += mov;
			player_circle_color = GetColor(0, 0, 255);
			//	�����A�j���[�V�����͊�����������
			anim_rate[ANIM_STAND] -= 0.1f;
			anim_rate[ANIM_RUN] -= 0.1f;
			//	�����A�j���[�V�����̊������グ�Ă��������i���X�ɑ��₷�j
			anim_rate[ANIM_WALK] += 0.1f;
		}

	}
	else
	{
		//	��L�[�������Ă��Ȃ��Ƃ��i�������グ�ĕ�����������j
		anim_rate[ANIM_WALK] -= 0.1f;
		anim_rate[ANIM_RUN] -= 0.1f;
		anim_rate[ANIM_STAND] += 0.1f;
	}

	//	�A�j���[�V�������Đ�������
	for (int i = 0; i < ANIM_MAX; i++)
	{
		//	������ 0.0f �` 1.0f �ɂ���
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
	MV1DrawModel(ground_model);

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	�͈͂�������悤�ɖڈ��̉~
	DrawCircle3D_XZ(npc_pos, RUN_DISTANCE, GetColor(255, 255, 0));
	DrawCircle3D_XZ(npc_pos, CHARA_RADIUS, GetColor(255, 0, 0));
	DrawCircle3D_XZ(player_pos, CHARA_RADIUS, player_circle_color);

	//	�v���C���[���f���̕`��
	//	���f���̍��W��ݒ�
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	//	���f���̌����ݒ�i����̃��f���͂y�}�C�i�X�����������Ă���̂ŕ`��̎��ɂx���ɑ΂��ĂP�W�O�x��]�����Ă��܂��j
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	//	����̃��f���͂߂���߂���傫���̂ŏ��������Ă��܂�
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	//	���̃��f����`��
	MV1DrawModel(player_model);

	DrawString(16, 16, "�\���L�[�F�v���C���[�̌����E���W�̕ύX", GetColor(255, 255, 255));
	DrawString(16, 32, "�m�o�b�ɋ߂Â�����i�v���C���[�����F���~�̒��ɓ�������j", GetColor(255, 255, 0));
	DrawString(16, 48, "���@�v���C���[�̈ړ��X�s�[�h�A�b�v�i����ɑ���A�j���[�V�����ɂ���j", GetColor(255, 255, 0));
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
