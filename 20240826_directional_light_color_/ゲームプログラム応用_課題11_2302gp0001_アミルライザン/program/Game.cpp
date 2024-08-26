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

//	���Ԗڂ̐F���g���̂�
COLOR_F currentColor = { 1.0f,1.0f,1.0f,1.0f };
int color_num;

//	���C�g�̐F�f�[�^�i 0.0f �` 1.0f �j
COLOR_F light_color[4] =
{
	//	 R     G     B     A
	//{ 1.0f, 1.0f, 1.0f, 1.0f },		//	��
	//{ 1.0f, 0.0f, 0.0f, 1.0f },		//	��
	//{ 0.0f, 1.0f, 0.0f, 1.0f },		//	��
	//{ 0.0f, 0.0f, 1.0f, 1.0f },		//	��

	//���E���E�[���E��

	{ 0.65f, 0.65f, 0.8f, 1.0f },		//	Asa
	{ 1.0f, 1.0f, 1.0f, 1.0f },		//	Hiru
	{ 0.8f, 0.5f, 0.2f, 1.0f },		//	Yuugata
	{ 0.1f, 0.1f, 0.5f, 1.0f },		//	Yoru
};

template<typename T>
T lerp(T a, T b, float t)
{
	return a + t * (b - a);
}

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -4.5f), VGet(0.0f, 0.0f, 0.0f));

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�������W������̐ݒ�
	player_pos.set(2.5f, 0.0f, 0.0f);
	player_rot.y = 180.0f;

	npc_pos.set(-2.5f, 0.0f, 2.0f);

	color_num = 0;
	currentColor = light_color[color_num];
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	player_rot.y += 1.0f;

	//	�v���C���[�̍��W�ړ�
	Float3 base(0.0f, 0.0f, 0.03f);
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	Float3 mov = VTransform(base.VGet(), mat);
	player_pos += mov;

	npc_rot.y += 0.5f;

	//	�e�P�`�e�S�L�[�ŐF�ԍ���ύX
	if (PushHitKey(KEY_INPUT_F1))
	{
		color_num = 0;
	}
	if (PushHitKey(KEY_INPUT_F2))
	{
		color_num = 1;
	}
	if (PushHitKey(KEY_INPUT_F3))
	{
		color_num = 2;
	}
	if (PushHitKey(KEY_INPUT_F4))
	{
		color_num = 3;
	}

	const int color_change_freq = 100;
	static int game_time = 0;
	if (game_time % color_change_freq == 0 || game_time == 0)
	{
		color_num++;
		if (color_num > 3)
		{
			color_num = 0;
		}
	}
	game_time++;

	float color_change_speed = 0.01f;
	float red = light_color[color_num].r;
	float green = light_color[color_num].g;
	float blue = light_color[color_num].b;
	currentColor.r = lerp(currentColor.r, red,   color_change_speed);
	currentColor.g = lerp(currentColor.g, green, color_change_speed);
	currentColor.b = lerp(currentColor.b, blue,  color_change_speed);

	
	clsDx();
	printfDx("%d\n", color_num);
	printfDx("%f , %f : \n", currentColor.r, red);

	//currentColor.r
	//	��̐F��n���ă��C�g�̐F�̐ݒ�
	SetLightDifColor(currentColor);
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

	DrawString(16, 16, "��莞�ԂŃ��C�g�̐F�̕ύX�i�F�́u���������[������v���ۂ��j", GetColor(255, 255, 0));
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
