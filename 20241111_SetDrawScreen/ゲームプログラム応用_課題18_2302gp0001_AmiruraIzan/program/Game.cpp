#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.1f
//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	3.0f

//	���f���p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�v���C���[�̍��W�����
Float3 player_pos;
Float3 player_rot;
//	�m�o�b�̍��W�����
Float3 npc_pos;
Float3 npc_rot;


int text_image;
const char* sample_text = "������Ɗp�x��`�悷�邾���̕ʉ�ʂ��摜���ĉ�ʉ��ɕ`��";
float text_x;

//	��ʂ̕`�悷��摜�T�C�Y
#define TEXT_SCREEN_W	400
#define TEXT_SCREEN_H	50

#define MINI_SCREEN_W	160
#define MINI_SCREEN_H	 90

//	�P�F�R�c�`����������ނ��߂̃e�N�X�`����p��
int screen_image;


Float2 image_pos;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�P�F�`���Ԃ̂��̂�`�����ނ��߂̉摜���쐬
	screen_image = MakeScreen(MINI_SCREEN_W, MINI_SCREEN_H);
	text_image = MakeScreen(TEXT_SCREEN_W, TEXT_SCREEN_H);

	//	�J�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 3.5f, -15.0f), VGet(0.0f, 0.0f, 0.0f));
	//	���f���̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");

	player_pos.set(5.0f, 0.0f, 0.0f);
	player_rot.y = 0.0f;

	npc_pos.set(-5.0f, 0.0f, 5.0f);
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�ō��W�ړ�
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	npc_rot.y += 0.5f;

	text_x -= 2;

	if (text_x <= -500)
	{
		text_x = TEXT_SCREEN_W;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	SetDrawScreen(text_image);
	ClearDrawScreen();

	DrawFillBox(0,0,TEXT_SCREEN_W, TEXT_SCREEN_H, GetColor(0,0,255));
	DrawString(text_x, 17, sample_text, GetColor(0,255,255));

	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	//	�v���C���[��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));

	SetDrawScreen(screen_image);
	ClearDrawScreen();
	//	�Q�|�Q�F�����_�[�^�[�Q�b�g��؂�ւ����̂ŃJ�����ݒ�
	//	�J�������`��ł��鋗��
	SetCameraNearFar(0.1f, 3000.0f);
	//	�J�����̉�p
	SetupCamera_Perspective(TO_RADIAN(45.0f));

	Float3 pos = player_pos + Float3(0.0f, 1.0f, -3.0f);
	Float3 look = player_pos + Float3(0.0f, 0.5f, 0.0f);

	SetCameraPositionAndTarget_UpVecY(pos.VGet(), look.VGet());

	MV1DrawModel(ground_model);	//	�n�ʃ��f���̕`��
	MV1DrawModel(npc_model);		//	�m�o�b���f���̕`��
	MV1DrawModel(player_model);	//	�v���C���[���f���̕`��

	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 3.5f, -15.0f), VGet(0.0f, 0.0f, 0.0f));

	MV1DrawModel(ground_model);	//	�n�ʃ��f���̕`��
	MV1DrawModel(npc_model);		//	�m�o�b���f���̕`��
	MV1DrawModel(player_model);	//	�v���C���[���f���̕`��

	VECTOR pos2d = ConvWorldPosToScreenPos(player_pos.VGet());

	if (pos2d.x > SCREEN_W / 2.0f)
	{
		image_pos.x += 10.0f;
	}
	else
	{
		image_pos.x -= 10.0f;
	}

	image_pos.x = max(-170.0f, min(image_pos.x, 10.0f));

	DrawGraph(image_pos.x, image_pos.y, screen_image, TRUE);

	DrawGraph(image_pos.x, image_pos.y, screen_image, TRUE);
	DrawLineBox(image_pos.x, image_pos.y, image_pos.x + MINI_SCREEN_W, image_pos.y + MINI_SCREEN_H, GetColor(255, 255, 255));

	DrawGraph(image_pos.x + SCREEN_W, image_pos.y, screen_image, TRUE);
	DrawLineBox(image_pos.x + SCREEN_W, image_pos.y, image_pos.x + MINI_SCREEN_W, image_pos.y + MINI_SCREEN_H, GetColor(255, 255, 255));

	DrawString(16, 16, "�\���L�[�F�v���C���[�̉�]�ƈړ�", GetColor(255, 255, 255));
	DrawString(16, 32, "��������ʂ̃J�������v���C���[������J�����ɂ���", GetColor(255, 255, 0));
	DrawString(16, 48, "�v���C���[�̉�ʈʒu�i�����E�j�ɂ���ď�������ʂ̕\���ꏊ��ύX�i�E���������j", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	�`�����ݗp�ɗp�ӂ����摜�̏I������
	DeleteGraph(screen_image);
}
