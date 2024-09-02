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
Float3 	light_pos;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 5.0f, -10.5f), VGet(0.0f, 0.0f, 0.0f));

	//	���łɍׂ��߂̒n�ʃ��f�����g���Ă��܂�
	ground_model = MV1LoadModel("data/ground2.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�������W������̐ݒ�
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	//	�_�����̐ݒ�i�u��������������.exe�v�ł͂��̂P�s�͂���ȏ�ύX���Ă��܂���j
	ChangeLightTypePoint(VGet(0.0f, 5.0f, 0.0f), 100.0f, 1.0f, 0.0f, 0.01f);

	//	���C�g�̍��W�̐ݒ�
	SetLightPosition(VGet(0.0f, 5.0f, 0.0f));
	//	���C�g�̐F�̐ݒ�
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));
}

int lightMode = 0;
float light_travel_vector = 0;
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		lightMode++;
		if (lightMode > 1)
		{
			lightMode = 0;
		}
	}

	//	�v���C���[�̉�]
	player_rot.y += 1.0f;
	//	�v���C���[�̍��W�ړ�
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = VTransform(base.VGet(), mat);
	player_pos += mov;

	//	�m�o�b�̉�]
	npc_rot.y += 0.5f;

	//	�v���C���[�̑O���̍��W�����߂�

	//	���̍��W�����C�g���W�ɐݒ肷��

		//Light Blue
	if (lightMode == 0)
	{
		float extra_distance = 1.0f;
		base.y += 1.0f;
		base.z += extra_distance;
		light_pos = player_pos + VTransform(base.VGet(), mat);
		SetLightPosition(light_pos.VGet());
		//clsDx();
		//printfDx("light_pos %f, %f ", light_pos.x, light_pos.y);
		light_travel_vector = 0; //resets
		SetLightDifColor(GetColorF(0.3f, 0.3f, 1.0f, 0.0f));
	}
	else if (lightMode == 1)//Light Red
	{
		Float3 enemyBase(0.0f, 0.0f, 0.03f);

		MATRIX enemyMat = MGetRotY(TO_RADIAN(npc_rot.y));
		float extra_distance = 1.0f;
		enemyBase.y += 1.0f;
		enemyBase.z += extra_distance + light_travel_vector;
		light_pos = npc_pos + VTransform(enemyBase.VGet(), enemyMat);
		SetLightPosition(light_pos.VGet());
		SetLightDifColor(GetColorF(1.0f, 0.3f, 0.3f, 0.0f));
		const float light_movespeed = 0.5f;
		light_travel_vector+= light_movespeed;
	}

}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�n�ʃ��f���̕`��
	MV1DrawModel(ground_model);
	//	�m�o�b���W�������ݒ肵�ă��f���̕`��
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);
	//	�v���C���[���W�������ݒ肵�ă��f���̕`��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	MV1DrawModel(player_model);

	DrawString(16, 16, "���C�g���W�̕ύX�i�v���C���[�̑O���Ɂj�ƐF�̕ύX�i���ۂ��j", GetColor(255, 255, 0));
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
