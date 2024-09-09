#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	���f���p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

//	�v���C���[�p�̃X�|�b�g���C�g�n���h��
int player_light;
int npc_light;

Float3 light_rot;
bool light_right;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -10.5f), VGet(0.0f, 0.0f, 0.0f));

	//	����͒��_�̑����ׂ����n�ʃ��f����`�悵�Ă��܂�
	ground_model = MV1LoadModel("data/ground2.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�������W������̐ݒ�
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	//	�f�t�H���g�̕��s�����̃��C�g���Â߂ȃ��C�g�Őݒ�
	SetLightDifColor(GetColorF(0.2f, 0.2f, 0.2f, 1.0f));

	//	�X�|�b�g���C�g�̍쐬�i�����͕ύX���܂���j
	player_light = CreateSpotLightHandle(VGet(0.0f, 1.0f, 0.0f), VGet(0.0f, -1.0f, 1.0f), TO_RADIAN(45.0f), TO_RADIAN(30.0f), 100.0f, 1.0f, 0.0f, 0.01f);

	//	���ȉ��̂Ƃ����ύX���Ă����܂��i�X�V�����Łj
	//	�X�|�b�g���C�g�̍��W�̐ݒ�i�Ƃ肠�������_�̏�����ɒu���Ă��܂��j
	SetLightPositionHandle(player_light, VGet(0.0f, 1.0f, 0.0f));
	//	�X�|�b�g���C�g�̌����̐ݒ�i�Ƃ肠�����^�����������Ă��܂��j
	SetLightDirectionHandle(player_light, VGet(0.0f, -1.0f, 0.0f));
	//	�X�|�b�g���C�g�̐F�̐ݒ�
	SetLightDifColorHandle(player_light, GetColorF(0.5f, 0.5f, 1.0f, 0.0f));
	npc_light = CreateSpotLightHandle(VGet(0.0f, 1.0f, 0.0f), VGet(0.0f, -1.0f, 1.0f), TO_RADIAN(45.0f), TO_RADIAN(30.0f), 100.0f, 1.0f, 0.0f, 0.01f);
	SetLightDifColorHandle(npc_light, GetColorF(1.0f, 0.5f, 0.5f, 0.0f));
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�v���C���[�̉�]
	player_rot.y += 1.0f;
	//	�v���C���[�̍��W�ړ�
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = GetFloat3VTransform(base, mat);
	player_pos += mov;

	Float3 pos1 = player_pos + Float3(0.0f, 2.0f, 0.0f);
	SetLightPositionHandle(player_light, pos1.VGet());

	Float3 base1(0.0f, -1.0f, 1.0f);
	Float3 dir1 = GetFloat3VTransform(base1, mat);
	SetLightPositionHandle(player_light, dir1.VGet());


	//	�m�o�b�̉�]
	npc_rot.y += 0.1f;

	if (light_right)
	{
		light_rot.y += 1.0f;
		if (light_rot.y >= 30.0f)
		{
			light_right = false;
		}
	}
	else
	{
		light_rot.y -= 1.0f;
		if (light_rot.y <= -30.0f)
		{
			light_right = true;
		}
	}

	Float3 pos2 = npc_pos + Float3(0.0f, 0.5f, 0.0f);
	SetLightPositionHandle(npc_light, pos2.VGet());
	Float3 base2(0.0f, 0.0f, 1.0f);

	MATRIX mat2 = MGetRotY(TO_RADIAN(npc_rot.y + light_rot.y));
	Float3 dir2 = GetFloat3VTransform(base2, mat);
	SetLightDirectionHandle(npc_light, dir2.VGet());
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

	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	DrawString(16, 16, "�X�|�b�g���C�g�ȊO���^���ÂɂȂ�Ȃ��悤�Ƀf�B���N�V���i�����C�g�őS�̂��Ƃ炷�i�F�͈Â߁j", GetColor(255, 255, 255));
	DrawString(16, 32, "�X�|�b�g���C�g���g���āA�v���C���[�����C�g���Ƃ炷����", GetColor(255, 255, 0));
	DrawString(16, 48, "���@�v���C���[�̓��̏ォ��O��������", GetColor(255, 255, 0));
	DrawString(16, 64, "���@���C�g�̐F����ۂ�", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);
}

