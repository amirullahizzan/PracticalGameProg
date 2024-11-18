#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Float2.h"

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

float player_speed;

//	�m�o�b�̍��W�����
Float3 npc_pos;
Float3 npc_rot;

#define MAP_W		200
#define MAP_H		200

int map_image;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	map_image = MakeScreen( MAP_W, MAP_H );

	//	�J�����ݒ�
	camera_pos.set( 0.0f, 7.0f, -15.0f );
	camera_look.clear();
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );
	//	�v���C���[�̏����ݒ�
	player_pos.set( 0.0f, 0.0f, -5.0f );
	player_rot.y = 0.0f;

	player_speed = 0.0f;

	npc_pos.set( -5.0f, 0.0f, 5.0f );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�v���C���[�̉�]�ƈړ�
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;

	if( CheckHitKey( KEY_INPUT_UP ) ){
	}

	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );

	//	�m�o�b�̉�]
	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );

	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );

	SetDrawScreen( DX_SCREEN_BACK );
	ClearDrawScreen();
	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	���f���̕`��
	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	DrawGraphF( 580.0f, 230.0f, map_image, TRUE );
	DrawBoxAA( 580.0f, 230.0f, 580.0f + MAP_W, 230.0f + MAP_H, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawBoxAA( 20.0f, 330.0f, 70.0f, 430.0f, GetColor( 255, 255, 255 ), FALSE, 5.0f );

	DrawString( 16, 16, "���E�L�[�F�v���C���[�̉�]", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "��L�[�@�F�v���C���[�̈ړ��X�s�[�h�𑝂₷�i�����ĂȂ������猸�炷�j", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "���@�����Ă�������ɂ��̃X�s�[�h�ňړ�", GetColor( 255, 255, 255 ) );
	DrawString( 16, 64, "�J�����̓v���C���[������", GetColor( 255, 255, 255 ) );
	DrawString( 16, 80, "��ʉE���ɑS�̂�������悤�ȃJ�����̉摜", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 290, GetColor( 255, 255, 255 ), "�X�s�[�h�F%.3f", player_speed );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	�����_�[�^�[�Q�b�g�̏I������
	DeleteGraph( map_image );
}
