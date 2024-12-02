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

#define SHOT_W	240
#define SHOT_H	135

int shot_image;
Float2 shot_pos;

bool screen_shot;
int save_frame;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//shot_image = MakeScreen( SHOT_W, SHOT_H );
	shot_image = MakeScreen( SCREEN_W, SCREEN_H);

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

	if( PushHitKey( KEY_INPUT_SPACE ) )
	{
		screen_shot = true;
	}

	if( PushHitKey( KEY_INPUT_RETURN ) )
	{
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

	if (screen_shot)
	{
		screen_shot = false;

		SetDrawScreen(shot_image);
		ClearDrawScreen();

		SetCameraNearFar(0.1f,3000.0f);
		SetupCamera_Perspective(TO_RADIAN(45.0f));
		SetCameraPositionAndTarget_UpVecY(camera_pos.VGet(),camera_look.VGet());

		MV1DrawModel(ground_model);
		MV1DrawModel(npc_model);
		MV1DrawModel(player_model);
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

	if (save_frame == 60)
	{
		SaveDrawScreen(0,0,SCREEN_W,SCREEN_H,"data/sample1.bmp");
	}

	float x1 = shot_pos.x;
	float y1 = shot_pos.y;
	DrawGraphF( x1, y1, shot_image, TRUE );

	float x2 = x1 + SHOT_W;
	float y2 = y1 + SHOT_H;
	DrawBoxAA( x1, y1, x2, y2, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "�X�y�[�X�L�[�F�X�N���[���V���b�g", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�G���^�[�L�[�F��ʂ̕`���Ԃ��摜�ɂ��ĕۑ�", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "���@data �t�H���_�� sample.bmp �Ƃ����t�@�C���ŕۑ����܂�", GetColor( 255, 255, 255 ) );

	if( save_frame > 0 ){
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
