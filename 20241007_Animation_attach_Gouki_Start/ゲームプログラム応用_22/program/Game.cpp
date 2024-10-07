#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�����Ă���Ƃ��̈ړ��X�s�[�h
#define PLAYER_WALK_SPEED	0.025f
//	��]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

enum {
	ANIM_STAND,
	ANIM_WALK,

	ANIM_MAX,
};

int anim_state = ANIM_STAND;

int anim_model[ANIM_MAX];
int anim_attach[ANIM_MAX];
float anim_total[ANIM_MAX];
float anim_time[ANIM_MAX];

float anim_blend[ANIM_MAX];

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
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -7.0f), VGet(0.0f, 0.0f, 0.0f));

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	anim_model[ANIM_STAND] = MV1LoadModel("data/player/anim_stand.mv1");
	anim_model[ANIM_WALK] = MV1LoadModel("data/player/anim_walk.mv1");

	//	�������W������̐ݒ�
	player_pos.set(3.0f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-3.0f, 0.0f, 2.0f);

	for (int i = 0; i < ANIM_MAX; i++)
	{
		anim_attach[i] = MV1AttachAnim(player_model, 1, anim_model[i], FALSE);
		anim_total[i] = MV1GetAttachAnimTotalTime(player_model, anim_attach[i]);
		anim_time[i] = 0.0f;
		anim_blend[i] = 0.0f;
	}
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
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
		player_pos += mov;

		anim_blend[ANIM_WALK] += 0.1f;
		anim_blend[ANIM_STAND] -= 0.1f;
	}
	else
	{
		anim_blend[ANIM_STAND] += 0.1f;
		anim_blend[ANIM_WALK] -= 0.1f;
	}

	//MV1SetAttachAnimTime(player_model, anim_attach[anim_state], anim_time[anim_state]);
	//MV1SetAttachAnimBlendRate(player_model, anim_attach[anim_state], anim_blend[anim_state]);

	for (int i = 0 ; i < ANIM_MAX;i++)
	{
		anim_blend[i] = max(0.0f, min(anim_blend[i], 1.0f));
		anim_time[i] += 1.0f;
		if (anim_time[i] >= anim_total[i])
		{
			anim_time[i] = 0.0f;
		}

		if (anim_time[i] >= anim_total[i])
		{
			anim_time[i] = 0.0f;
		}
		
		MV1SetAttachAnimTime(player_model, anim_attach[i], anim_time[i]);
		MV1SetAttachAnimBlendRate(player_model, anim_attach[i], anim_blend[i]);
	}

	clsDx();
	printfDx("%d", anim_state);

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

	//	�v���C���[���f���̕`��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));
	MV1DrawModel(player_model);

	DrawString(16, 16, "�\���L�[�F�v���C���[�̌����E���W�̕ύX", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	MV1DeleteModel(anim_model[0]);
	MV1DeleteModel(anim_model[1]);
}
