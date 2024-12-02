#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"
#include "Hit.h"

//	���f���f�[�^
int ground_model;
int player_model;
int npc_model;

//	�A�j���[�V�����֌W�̕ϐ�
int anim_model;		//	�A�j���[�V�����f�[�^�p
int anim_attach;	//	�A�^�b�`�ԍ�
float anim_total;	//	�A�j���[�V�������t���[����
float anim_frame;	//	�Đ��t���[��

//	�J�������W�ƌ�����W
Float3 camera_pos;
Float3 camera_look;
//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

//	�X�N���[���V���b�g�̖���
#define SHOT_MAX	3

//	�X�N���[���V���b�g��\������傫��
#define SHOT_W	240
#define SHOT_H	135
//	�����_�[�^�[�Q�b�g�p�̉摜
int shot_image;
//	���̉摜�̕\�����W
Float2 shot_pos;

//	�X�N���[���V���b�g���Ƃ�t���O
bool screen_shot;
//	�u�ۑ����܂����v�̕������`�悷�鎞��
int save_frame;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�����_�[�^�[�Q�b�g�̍쐬
	//	�i��ʃT�C�Y�Ɠ����摜�Ƃ��č쐬�j
	shot_image = MakeScreen( SCREEN_W, SCREEN_H );
	//	���W�̐ݒ�
	shot_pos.x = 20.0f;
	shot_pos.y = 300.0f;

	//	�J�����̐ݒ�
	camera_pos.set( 0.0f, 1.35f, -3.0f );
	camera_look.set( 0.0f, 0.75f, 0.0f );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );
	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model = MV1LoadModel( "data/player/anim_punch.mv1" );

	//	�������W������̐ݒ�
	player_pos.set( 1.0f, 0.0f, 0.0f );
	player_rot.y = 230.0f;

	npc_pos.set( -2.0f, 0.0f, 2.0f );
	npc_rot.y = 90.0f;

	//	�A�^�b�`�ԍ��擾
	anim_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	//	���t���[�����擾
	anim_total = MV1GetAttachAnimTotalTime( player_model, anim_attach );
	//	�O�t���[������J�n
	anim_frame = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�A�j���[�V�����t���[����i�߂�
	anim_frame += 1.0f;
	//	�Ō�܂ōs������ŏ�����
	if( anim_frame >= anim_total ){
		anim_frame = 0.0f;
	}
	//	���̃t���[���ŃA�j���Đ�
	MV1SetAttachAnimTime( player_model, anim_attach, anim_frame );

	//	�m�o�b�̍��W�ړ�
	npc_pos.x += 0.05f;
	if( npc_pos.x > 5.0f ){
		npc_pos.x = -5.0f;
	}

	//	�X�y�[�X�L�[���������Ƃ�
	if( PushHitKey( KEY_INPUT_SPACE ) ){
		//	�X�N���[���V���b�g�������_�[�^�[�Q�b�g�ɕ`�����ނ��߂̃t���O�𗧂Ă�
		screen_shot = true;
	}

	//	�\�����Ԃ����炵�Ă���
	save_frame--;
	save_frame = max( 0, save_frame );

	//	����͉摜���N���b�N�����炻�̉摜�̕ۑ����J�n���܂�
	if( PushMouseInput() ){
		//	�P�b�ԁu�ۑ����܂����v��\���������̂�
		save_frame = 60;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�v���C���[�����񂩕`�悷��̂ō��W������̐ݒ�͂����ł܂Ƃ߂Ă���Ă����܂�
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	//	�m�o�b��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );

	//	�X�y�[�X�L�[���������^�C�~���O�̎�����
	if( screen_shot ){
		//	�`�����݂܂����̂Ńt���O�𗎂Ƃ�
		screen_shot = false;

		//	�����_�[�^�[�Q�b�g��؂�ւ��ĕ`������
		SetDrawScreen( shot_image );
		//	��ʂ̃N���A
		ClearDrawScreen();
		//	�J�����̐ݒ�i����͒ʏ�̃J�����Ɠ������̂��g���܂��j
		SetCameraNearFar( 0.1f, 3000.0f );
		SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
		SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
		//	���f���̕`��
		MV1DrawModel( ground_model );
		MV1DrawModel( npc_model );
		MV1DrawModel( player_model );
	}

	//	�����_�[�^�[�Q�b�g����ʂɖ߂�
	SetDrawScreen( DX_SCREEN_BACK );
	ClearDrawScreen();
	//	�J�����̐ݒ�
	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
	//	���f���̕`��
	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	//	�X�N���[���V���b�g�摜�̕`����W
	//	����̍��W
	float x1 = shot_pos.x;
	float y1 = shot_pos.y;
	//	�E���̍��W
	float x2 = x1 + SHOT_W;
	float y2 = y1 + SHOT_H;
	//	�g�̍��W�ɍ��킹�ăX�N���[���V���b�g�摜��`��
	DrawExtendGraphF( x1, y1, x2, y2, shot_image, TRUE );
	//	���̍��W���g���Ďl�p�̘g�̕`��
	DrawBoxAA( x1, y1, x2, y2, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "�X�y�[�X�L�[�F�X�N���[���V���b�g�i�R�쐬�j", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�P�F����̃J����", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "�Q�F�m�o�b������v���C���[������J����", GetColor( 255, 255, 0 ) );
	DrawString( 16, 64, "�R�F�v���C���[���߂����猩��J����", GetColor( 255, 255, 0 ) );

	if( save_frame > 0 ){
		//	�\�����Ԃ�����Ƃ�����������̕`��
		DrawString( 350, 200, "�ۑ����܂���", GetColor( 255, 255, 0 ) );
	}
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	MV1DeleteModel( anim_model );

	DeleteGraph( shot_image );
}
