#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.025f
//	��]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f

#define RECORD_MAX 10

	//	���̊J�n���W
Float3 record_pos[RECORD_MAX];
//	���̃S�[�����W
Float3 record_goal[RECORD_MAX];

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�J�������W�ƌ�����W
Float3 camera_pos;
Float3 camera_look;

//	�A�j���[�V�����f�[�^
int anim_model;
//	�A�^�b�`�ԍ�
int anim_attach;
//	�A�j���[�V�����P���[�v�̎���
float anim_total;
//	�A�j���[�V�������ǂ̂��炢�i�߂邩
float anim_time;

//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

//	�{�[���̍��W�i���̍��W���v���C���[�̉E��̍��W�ɂ��܂��j
Float3 ball_pos;

bool isFPS = false;

int lineBlend[RECORD_MAX];

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̏����ݒ�
	camera_pos.set(0.0f, 2.0f, -5.0f);
	camera_look.set(0.0f, 1.0f, 0.0f);

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player/model.mv1");
	npc_model = MV1LoadModel("data/npc.mqoz");

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model = MV1LoadModel("data/player/anim_walk.mv1");

	//	�������W������̐ݒ�
	player_pos.set(2.0f, 0.0f, 0.0f);
	player_rot.y = -90.0f;

	npc_pos.set(-3.5f, 0.0f, 2.0f);

	//	�A�j���[�V�����̏�����
	anim_attach = MV1AttachAnim(player_model, 1, anim_model, FALSE);
	anim_total = MV1GetAttachAnimTotalTime(player_model, anim_attach);
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
int lineIndex = 0;

void ProcessLine()
{
	//	�v���C���[�̉E��Ƀ{�[����t����
//	�P�F�E��̏ꏊ�̖��O���m�F�i mixamorig:RightHandIndex1�F���̖��O�̓��f���r���[���[�Ȃ̂Ŋm�F�j
//	�Q�F���̖��O�̏ꏊ�̔ԍ����擾
	int frame = MV1SearchFrame(player_model, "mixamorig:RightHandIndex1");
	//	�R�F���̔ԍ��̍s����擾�i�w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�j
	MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, frame);
	//	�S�F���̍s����g���č��W�ϊ��������̂��{�[�����W�ɂ��܂�
	//	���̍��W��p�Ӂi���_�̍��W�j
	Float3 base(0.0f, 0.0f, 0.0f); // no need adjustment
	//	���̍��W���s��ŕϊ����ă{�[�����W�ɂ��܂�
	ball_pos = GetFloat3VTransform(base, mat);
	record_pos[lineIndex] = ball_pos;

	//////////////GOAL

	// Get the position of the hand in world space
	Float3 goal = Float3(100.0f, 0, 0);
	//X of hand
	Float3 line_goal = GetFloat3VTransform(goal, mat);

	record_goal[lineIndex] = line_goal;

	lineBlend[lineIndex] = 255;
	lineIndex++;

	if (lineIndex >= RECORD_MAX)
	{
		lineIndex = 0;
	}
}

void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�������Ă���������Ă�������Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	//	�A�j���[�V�����̎��Ԃ�i�߂�
	anim_time += 1.0f;
	if (anim_time >= anim_total)
	{
		anim_time = 0.0f;
	}
	//	���̎��Ԃ̃A�j���[�V�����ɂ���
	MV1SetAttachAnimTime(player_model, anim_attach, anim_time);

	npc_rot.y += 0.5f;


	ProcessLine();

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		isFPS = !isFPS;
	}

	if (isFPS)
	{
		//	�v���C���[�̉E��Ƀ{�[����t����
//	�P�F�E��̏ꏊ�̖��O���m�F�i mixamorig:RightHandIndex1�F���̖��O�̓��f���r���[���[�Ȃ̂Ŋm�F�j
//	�Q�F���̖��O�̏ꏊ�̔ԍ����擾
		//	�R�F���̔ԍ��̍s����擾�i�w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�j

		int headFrame = MV1SearchFrame(player_model, "mixamorig:Head");
		MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, headFrame);

		Float3 headBase(0.0f, 0.0f, 0.0f);
		camera_pos = GetFloat3VTransform(headBase, mat);

		Float3 headFront(0.0f, 0.0f, 1.0f);
		//camera_look = GetFloat3VTransform(headFront, mat); //if using model eyes look

		mat = MGetRotY(TO_RADIAN(player_rot.y));
		camera_look = camera_pos + GetFloat3VTransform(headFront, mat);
	}
	else
	{
		camera_pos.set(0.0f, 2.0f, -5.0f);
		camera_look.set(0.0f, 1.0f, 0.0f);
	}

}



//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------


void GameRender()
{
	//	�J�������W�ƌ�����W��n���ăJ�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	�n�ʃ��f���̕`��
	MV1DrawModel(ground_model);

	//	�m�o�b���f���̕`��
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));
	MV1DrawModel(npc_model);

	//	�v���C���[���f���̕`��
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y + 180.0f), TO_RADIAN(player_rot.z)));
	MV1SetScale(player_model, VGet(0.01f, 0.01f, 0.01f));

	MV1DrawModel(player_model);

	//	���F���{�[���̕`��
	//DrawSphere3D(head_pos.VGet(), 0.1f, 50, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	DrawSphere3D(ball_pos.VGet(), 0.1f, 10, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);

	for (int i = 0; i < RECORD_MAX; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, lineBlend[i]);
		DrawLine3D(record_pos[i].VGet(), record_goal[i].VGet(), GetColor(200, 255, 200));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int sub = 255 / RECORD_MAX;
		lineBlend[i] -= sub;
	}

	DrawString(16, 16, "�\���L�[�F�v���C���[�̉�]�E�ړ�", GetColor(255, 255, 255));
	DrawString(16, 32, "�v���C���[�̉E��ɋ��̕`��", GetColor(255, 255, 255));
	DrawString(16, 80, "�X�y�[�X�L�[�ŌŒ�J�����Ǝ������_�ł̃J�����؂�ւ�", GetColor(255, 255, 0));
	DrawString(16, 96, "���@�������_�̓v���C���[�̓��̍��W���琳�ʕ���������", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	//	�A�j���[�V�����f�[�^�̏I������
	MV1DeleteModel(anim_model);
}
