#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.03f
//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	1.0f
//	�m�o�b�̉�]�X�s�[�h
#define NPC_ROT_SPEED		0.5f

//	���f���p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�J�������W
Float3 camera_pos;
//	������W
Float3 camera_look;

//	�v���C���[���W
Float3 player_pos;
//	�v���C���[����
Float3 player_rot;

//	�m�o�b���W
Float3 npc_pos;
//	�m�o�b����
Float3 npc_rot;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	���f���̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	�J�����̏����ݒ�
	camera_pos.set( 0.0f, 3.0f, -5.0f );
	camera_look.set( 0.0f, 0.0f, 0.0f );

	//	�v���C���[�̏����ݒ�
	player_pos.set( 2.5f, 0.0f, 0.0f );
	player_rot.y = 180.0f;

	//	�m�o�b�̏����ݒ�
	npc_pos.set( -2.5f, 0.0f, 2.0f );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�v���C���[�̉�]
	player_rot.y += PLAYER_ROT_SPEED;
	//	�����Ă�������Ɉړ�
	Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
	Float3 mov = GetFloat3VTransform( base, mat );
	player_pos += mov;
	//	�m�o�b�̉�]
	npc_rot.y += NPC_ROT_SPEED;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�J�������W�ƌ�����W��n���ăJ�����ݒ�
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	�n�ʃ��f���̕`��
	MV1DrawModel( ground_model );
	//	���W�������ݒ肵�Ăm�o�b���f���̕`��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );
	//	���W�������ݒ肵�ăv���C���[���f���̕`��
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );
}
