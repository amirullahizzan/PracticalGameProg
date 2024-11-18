#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"

//	�X�s�[�h�̕ύX��
#define PLAYER_ACCEL		0.001f
//	�v���C���[�̍ő�X�s�[�h
#define PLAYER_MOV_SPEED	0.2f
//	�����̕ύX�X�s�[�h
#define PLAYER_ROT_SPEED	1.0f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�J�����̍��W
Float3 camera_pos;
Float3 camera_look;
//	�v���C���[�̍��W�����
Float3 player_pos;
Float3 player_rot;
//	�v���C���[�X�s�[�h
float player_speed;

//	�m�o�b�̍��W�����
Float3 npc_pos;
Float3 npc_rot;

//	�����_�[�^�[�Q�b�g�̑傫��
#define MAP_W		200
#define MAP_H		200
//	�����_�[�^�[�Q�b�g�摜�p�̕ϐ�
int map_image;

#define FRONT_W 160
#define FRONT_H 90
int front_image;

//	�X�s�[�h���[�^�p�̒萔
#define CENTER_X	150.0f		//	�~�̒��S�w���W�i�T���v���̎l�p���[�^�[�Əd�Ȃ�Ȃ��悤�Ɂu��������������.exe�v��菭���E�ɂ��Ă��܂��j
#define CENTER_Y	380.0f		//	�x���W
#define RADIUS		50.0f		//	���a

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�����_�[�^�[�Q�b�g�̍쐬
	map_image = MakeScreen(MAP_W, MAP_H);
	front_image = MakeScreen(FRONT_W, FRONT_H);

	//	�J�����ݒ�i�����Ƀv���C���[������J�����ɂȂ�܂����Ƃ肠�����j
	camera_pos.set(0.0f, 7.0f, -15.0f);
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());
	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");
	//	�v���C���[�̏����ݒ�
	player_pos.set(0.0f, 0.0f, -5.0f);
	player_rot.y = 0.0f;
	//	�X�s�[�h�͂O�ŊJ�n
	player_speed = 0.0f;

	npc_pos.set(-5.0f, 0.0f, 5.0f);
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�v���C���[�̉�]�ƈړ�
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[����������X�s�[�h���グ�Ă���
	if (CheckHitKey(KEY_INPUT_UP)) {
		player_speed += PLAYER_ACCEL;
	}
	else {	//	����Ȃ��Ƃ��i��L�[�������Ă��Ȃ��Ƃ��j
		player_speed -= PLAYER_ACCEL;
	}
	//	�v���C���[�X�s�[�h�� 0.0f �` PLAYER_MOV_SPEED �ɂ���
	player_speed = max(0.0f, min(player_speed, PLAYER_MOV_SPEED));

	//	�L�[�������Ă��邢�Ȃ��Ɋւ�炸�X�s�[�h����������ړ�����
	//	�v���C���[�������Ă���p�x�ɉ�]�����邽�߂̉�]�s��쐬
	MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
	//	��̈ړ��x�N�g��
	Float3 base(0.0f, 0.0f, player_speed);
	//	���̊�x�N�g������]�s��ŕϊ�
	Float3 mov = GetFloat3VTransform(base, mat);
	//	���̈ړ��x�N�g�����g���č��W�ړ�
	player_pos += mov;

	//	�m�o�b�̉�]
	npc_rot.y += 0.5f;

	//	������W�̓v���C���[���W�i�̏�����j
	camera_look = player_pos + Float3(0.0f, 0.5f, 0.0f);
	//	�J�������W�͌�����W���班�����ꂽ�Ƃ���
	camera_pos = camera_look + Float3(0.0f, 3.0f, -7.0f);
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�`������񂩂����Ȃ��܂��̂ŁA���W������̐ݒ�͂����ł܂Ƃ߂Ă���Ă����܂�
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	//	�m�o�b��
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));

	SetDrawScreen(front_image);		//	�������ŗp�ӂ����摜
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);				//	�J�����̋���
	SetupCamera_Perspective(TO_RADIAN(45.0f));	//	�J�����̉�p

	Float3 look = player_pos + Float3(0.0f, 0.5f, 0.0f);

	MATRIX mat = MGetRotY(TO_RADIAN(45.0f));
	Float3 base(0.0f, 0.5f, 3.0f);
	Float3 mov = GetFloat3VTransform(base, mat);
	Float3 pos = look + mov;
	SetCameraPositionAndTarget_UpVecY(pos.VGet(), look.VGet());

	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	SetDrawScreen(map_image);		//	�������ŗp�ӂ����摜
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);				//	�J�����̋���
	SetupCamera_Perspective(TO_RADIAN(45.0f));	//	�J�����̉�p
	Float3 pos2 = player_pos + Float3(0.0f, 100.0f, 0.0f);
	SetCameraPositionAndAngle(pos2.VGet(), TO_RADIAN(90.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f));

	//	���̂܂܂��ƃv���C���[��m�o�b���߂����Ꮼ�����`�悳��Ă��܂�
	//	�Ȃ̂ŁA���̂Q�̃��f����傫�����܂�
	MV1SetScale(player_model, VGet(10.0f, 10.0f, 10.0f));
	MV1SetScale(npc_model, VGet(10.0f, 10.0f, 10.0f));
	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	SetDrawScreen(DX_SCREEN_BACK);	//	��ʂɕ`��
	ClearDrawScreen();
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	�����̉�ʕ`�������Ƃ��̓v���C���[��m�o�b�̃��f���͒ʏ�̑傫���ɂ���
	MV1SetScale(player_model, VGet(1.0f, 1.0f, 1.0f));
	MV1SetScale(npc_model, VGet(1.0f, 1.0f, 1.0f));

	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	//	�S�̃}�b�v�摜�̕`��
	DrawGraphF(580.0f, 230.0f, map_image, TRUE);
	//	���̉摜���͂ޔ����g�̕`��i���̑����� 5.0f �ɂ��Ă��܂��j
	DrawBoxAA(580.0f, 230.0f, 580.0f + MAP_W, 230.0f + MAP_H, GetColor(255, 255, 255), FALSE, 5.0f);

	DrawGraphF(620.0f, 20.0f, front_image, TRUE);
	DrawBoxAA(620.0f, 20.0f, 620.0f + FRONT_W, 20.0f + FRONT_H, GetColor(255, 255, 255), FALSE, 5.0f);

	//	�v���C���[�̃X�s�[�h�ɍ��킹�ĉ��F���l�p�̕`��
	//	player_speed�F0.0f �` 0.2f
	//	���̃X�s�[�h�� PLAYER_MOV_SPEED ���ő�Ƃ����Ƃ��ɂǂ̂��炢�̊������i 0.0f �` 1.0f �j
	float rate = player_speed / PLAYER_MOV_SPEED;
	float length = 100.0f * rate;
	DrawBoxAA(20.0f, 430.0f - length, 70.0f, 430.0f, GetColor(255, 255, 0), TRUE);
	DrawBoxAA(20.0f, 330.0f, 70.0f, 430.0f, GetColor(255, 255, 255), FALSE, 5.0f);

	int count = 360.0f * rate;
	for (int i = 0; i < count ; i++)
	{
		float rot1 = i - 90.0f;
		float x1 = CENTER_X + RADIUS * cosf(TO_RADIAN(rot1));
		float y1 = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot1));

		float rot2 = rot1 + 1.0f;
		float x2 = CENTER_X + RADIUS * cosf(TO_RADIAN(rot2));
		float y2 = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot2));

		DrawTriangle(CENTER_X, CENTER_Y ,x1,y1,x2,y2,GetColor(0,255,255), TRUE);
	}

	//
	float rot = 360.0f * rate - 90.0f;

	float x = CENTER_X + RADIUS * cosf(TO_RADIAN(rot));
	float y = CENTER_Y + RADIUS * sinf(TO_RADIAN(rot));

	DrawLineAA(CENTER_X, CENTER_Y, x,y, GetColor(255, 255, 255), 5.0f);
	//	���[�^�[�g�̉~�̕`��
	DrawCircleAA(CENTER_X, CENTER_Y, RADIUS, 100, GetColor(255, 255, 255), FALSE, 5.0f);

	DrawString(16, 16, "���E�L�[�F�v���C���[�̉�]", GetColor(255, 255, 255));
	DrawString(16, 32, "��L�[�@�F�v���C���[�̈ړ��X�s�[�h�𑝂₷�i�����ĂȂ������猸�炷�j", GetColor(255, 255, 255));
	DrawString(16, 48, "���@�����Ă�������ɂ��̃X�s�[�h�ňړ�", GetColor(255, 255, 255));
	DrawString(16, 64, "�J�����̓v���C���[������", GetColor(255, 255, 255));
	DrawString(16, 80, "��ʉE���ɑS�̂�������悤�ȃJ�����̉摜", GetColor(255, 255, 255));
	DrawString(16, 96, "��ʉE��Ƀv���C���[�𐳖ʂ��猩���J�����̉摜�i�o�b�N�~���[�݂����ȁj", GetColor(255, 255, 0));

	DrawFormatString(16, 290, GetColor(255, 255, 0), "�X�s�[�h�F%.3f�i���[�^�[���~�`�Ɂj", player_speed);
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	�����_�[�^�[�Q�b�g�̏I������
	DeleteGraph(map_image);
}
