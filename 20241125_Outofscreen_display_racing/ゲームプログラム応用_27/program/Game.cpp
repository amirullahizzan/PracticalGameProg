#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

#define FAR_LENGTH			30.0f

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.2f
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

#define MINI_SCREEN_W	100
#define MINI_SCREEN_H	100

int player_screen_image;

int mark_image;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	player_screen_image = MakeScreen( MINI_SCREEN_W, MINI_SCREEN_H );

	//	�S�̃J�����̐ݒ�
	camera_pos.set( 0.0f, 5.0f, -10.0f );
	camera_look.set( 0.0f, 1.0f, 0.0f );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );
	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	mark_image = LoadGraph( "data/mark.bmp" );

	//	�������W������̐ݒ�
	player_pos.set( 3.0f, 0.0f, 0.0f );
	player_rot.y = 0.0f;

	npc_pos.set( -2.0f, 0.0f, 2.5f );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�Ō����Ă�������Ɉړ�
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
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
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	//	�m�o�b��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );

	SetDrawScreen( player_screen_image );
	ClearDrawScreen();
	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );

	//	�����_�[�^�[�Q�b�g�����ɖ߂�
	SetDrawScreen( DX_SCREEN_BACK );
	ClearDrawScreen();

	SetCameraNearFar( 0.1f, 3000.0f );
	SetupCamera_Perspective( TO_RADIAN( 45.0f ) );
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	���f���̕`��
	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	Float3 area_pos = camera_pos;
	area_pos.y = 0.01f;
	DrawCircle3D_XZ( area_pos, FAR_LENGTH, GetColor( 255, 255, 0 ) );
	DrawString( 16, 16, "�\���L�[�F�v���C���[�̉�]�E�ړ�", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�v���C���[���J�������痣�ꂽ��傫��������\��", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	DeleteGraph( mark_image );

	DeleteGraph( player_screen_image );
}
