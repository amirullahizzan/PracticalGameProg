#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	���f���p�̕ϐ�
int ground_model[2];
int player_model;
int npc_model;

//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

int ground_num;
int light_handle;

Float3 light_pos;
Float3 light_dir;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̐ݒ�
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 5.0f, -10.5f ), VGet( 0.0f, 0.0f, 0.0f ) );

	//	���f���f�[�^�̓ǂݍ���
	ground_model[0] = MV1LoadModel( "data/ground.mqoz" );
	ground_model[1] = MV1LoadModel( "data/ground2.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	�������W������̐ݒ�
	player_pos.set( 2.5f, 0.0f, 0.0f );
	player_rot.y = 180.0f;

	npc_pos.set( -2.5f, 0.0f, 2.0f );

	ground_num = 0;

	light_pos.set( 0.0f, 3.0f, 0.0f );
	light_dir.set( 0.0f, -1.0f, 0.0f );

	//ChangeLightTypeSpot(light_pos.VGet(), light_dir.VGet(), TO_RADIAN(45.0f), TO_RADIAN(30.0f),100.0f,1.0f,0.0f,0.01f);

	light_handle = CreateSpotLightHandle(light_pos.VGet(), light_dir.VGet(), TO_RADIAN(45.0f), TO_RADIAN(30.0f), 100.0f, 1.0f, 0.0f, 0.01f);
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�v���C���[�̉�]
	player_rot.y += 1.0f;
	//	�v���C���[�̍��W�ړ�
	Float3 base( 0.0f, 0.0f, 0.03f );
	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
	Float3 mov = GetFloat3VTransform( base, mat );
	player_pos += mov;

	//	�m�o�b�̉�]
	npc_rot.y += 0.1f;

	if( PushHitKey( KEY_INPUT_SPACE ) ){
		ground_num = ( ground_num + 1 ) % 2;
	}

	if( CheckHitKey( KEY_INPUT_RIGHT ) )	light_dir.x += 0.1f;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		light_dir.x -= 0.1f;
	if( CheckHitKey( KEY_INPUT_UP ) )		light_dir.z += 0.1f;
	if( CheckHitKey( KEY_INPUT_DOWN ) )		light_dir.z -= 0.1f;

	//SetLightDirection(light_dir.VGet());
	SetLightDirectionHandle(light_handle , light_dir.VGet());
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel( ground_model[ground_num] );

	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	DrawString( 16, 16, "�\���L�[�F���C�g�����̕ύX", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�X�y�[�X�F�n�ʃ��f���̕ύX", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 64, GetColor( 255, 255, 255 ), "X:%5.2f Y:%5.2f Z:%5.2f", light_dir.x, light_dir.y, light_dir.z );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model[0] );
	MV1DeleteModel( ground_model[1] );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );
}
