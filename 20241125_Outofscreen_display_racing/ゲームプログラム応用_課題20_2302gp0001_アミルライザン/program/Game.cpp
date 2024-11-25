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
//	�J�������W�ƌ�����W
Float3 camera_pos;
Float3 camera_look;

//	�v���C���[�̍��W�����
Float3 player_pos;
Float3 player_rot;
//	�m�o�b�̍��W�����
Float3 npc_pos;
Float3 npc_rot;

//	�����_�[�^�[�Q�b�g�̑傫��
#define MINI_SCREEN_W	100
#define MINI_SCREEN_H	100
//	�v���C���[���A�b�v�ŕ`�悵�����̂�`�����ޗp�̉摜
int player_screen_image;

//	�v���C���[�̃A�b�v�`��Ɏg���w�i�摜
int mark_image;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�����_�[�^�[�Q�b�g�摜�̍쐬
	//	�`�悷����́i�v���C���[���f���j�ȊO�̕����͓����ɂ������i�����̍Ō�� TRUE �������܂��j
	player_screen_image = MakeScreen(MINI_SCREEN_W, MINI_SCREEN_H, TRUE);

	//	�S�̃J�����̐ݒ�
	camera_pos.set(0.0f, 15.0f, -7.0f);
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());
	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel("data/ground.mqoz");
	player_model = MV1LoadModel("data/player.mqoz");
	npc_model = MV1LoadModel("data/npc.mqoz");
	//	�ڈ�p�̉摜�̓ǂݍ���
	mark_image = LoadGraph("data/mark.bmp");

	//	�������W������̐ݒ�
	player_pos.set(3.0f, 0.0f, 0.0f);
	player_rot.y = 0.0f;

	npc_pos.set(-2.0f, 0.0f, 2.5f);
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_RIGHT))	player_rot.y += PLAYER_ROT_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�Ō����Ă�������Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP)) {
		Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
		MATRIX mat = MGetRotY(TO_RADIAN(player_rot.y));
		Float3 mov = GetFloat3VTransform(base, mat);
		player_pos += mov;
	}

	//	�m�o�b�͉�]�������Ă��܂�
	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�v���C���[�����񂩕`�悷��̂ō��W������̐ݒ�͂����ł܂Ƃ߂Ă���Ă����܂�
	MV1SetPosition(player_model, VGet(player_pos.x, player_pos.y, player_pos.z));
	MV1SetRotationXYZ(player_model, VGet(TO_RADIAN(player_rot.x), TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
	//	�m�o�b��
	MV1SetPosition(npc_model, VGet(npc_pos.x, npc_pos.y, npc_pos.z));
	MV1SetRotationXYZ(npc_model, VGet(TO_RADIAN(npc_rot.x), TO_RADIAN(npc_rot.y), TO_RADIAN(npc_rot.z)));

	//	�����_�[�^�[�Q�b�g�̐؂�ւ�
	SetDrawScreen(player_screen_image);
	ClearDrawScreen();
	//	�J�����ݒ�
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	//	�J�������W�ƌ�����W�̐ݒ�
	Float3 player_camera_pos = player_pos + Float3(0.0f, 2.0f, -3.0f);
	Float3 player_camera_look = player_pos + Float3(0.0f, 0.25f, 0.0f);
	//	���̍��W���g���ăJ�����ݒ�
	SetCameraPositionAndTarget_UpVecY(player_camera_pos.VGet(), player_camera_look.VGet());
	//	����̃A�b�v�摜�̓v���C���[�̂ݕ\���ł���΂����̂Ńv���C���[�̂ݕ`��
	MV1DrawModel(player_model);

	//	�����_�[�^�[�Q�b�g�����ɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	//	�J�����ݒ�
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(), camera_look.VGet());

	//	���f���̕`��
	MV1DrawModel(ground_model);
	MV1DrawModel(npc_model);
	MV1DrawModel(player_model);

	//	�`�悷�邩�ǂ����̃t���O
	bool draw_image = false;

	//	�v���C���[���W�i���[���h���W�j���Q�c��ʍ��W�ɕϊ�
	VECTOR pos2d = ConvWorldPosToScreenPos(player_pos.VGet());


	//	�v���C���[����ʊO�ɂ���Ƃ������`��



	//	�ϊ�������ʍ��W���J�����͈̔͊O�̎��͕`�悵�Ȃ�
	if (pos2d.z < 0.0f || pos2d.z > 1.0f) 
	{
		draw_image = false;
	}

	if(draw_image)
	{
		float draw_x = pos2d.x;
		float draw_y = pos2d.y;

		float W1 = (MINI_SCREEN_W / 2);
		float W2 = (SCREEN_W - W1);
		float H1 = (MINI_SCREEN_W / 2);
		float H2 = SCREEN_H - H1;

		draw_x = max(W1,min(draw_x, W2));
		draw_y = max(H1,min(draw_y, H2));

#if 1
		Float2 center(SCREEN_W / 2.0f, SCREEN_H/ 2.0f);

		Float2 ppos(pos2d.x, pos2d.y);
		Float2 dir = ppos - center;

		float rate_x = 1.0f;
		float rate_y = 1.0f;

		if (ppos.x < W1) rate_x = (W1 - center.x) / (ppos.x - center.x);
		if (ppos.x > W2) rate_x = (W2 - center.x) / (ppos.x - center.x);

		if (ppos.y < H1) rate_y = (H1 - center.x) / (ppos.y - center.y);
		if (ppos.y > H2) rate_y = (H2 - center.x) / (ppos.y - center.y);

		float rate = min(rate_x,rate_y);

		Float2 draw = center + dir * rate;

		draw_x = draw.x;
		draw_y = draw.y;
#endif
		float rot = 0.0f;

		float x = pos2d.x - draw_x;
		float y = pos2d.y - draw_y;

		rot = TO_DEGREE(atan2f(y, x));

		//	�ڈ�摜�i mark_image �j�̕`��
		DrawRotaGraphF(draw_x, draw_y, 1.0f, TO_RADIAN(rot), mark_image, TRUE);
		//	���̏ꏊ�ɉ摜��`��
		DrawRotaGraphF(draw_x, draw_y, 1.0f, TO_RADIAN(0.0f), player_screen_image, TRUE);
	}

	DrawString(16, 16, "�\���L�[�F�v���C���[�̉�]�E�ړ�", GetColor(255, 255, 255));
	DrawString(16, 32, "�v���C���[����ʊO�ɏo��ƕʉ�ʂ̕\��", GetColor(255, 255, 0));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(ground_model);
	MV1DeleteModel(player_model);
	MV1DeleteModel(npc_model);

	DeleteGraph(mark_image);

	//	�����_�[�^�[�Q�b�g�摜�̏I������
	DeleteGraph(player_screen_image);
}
