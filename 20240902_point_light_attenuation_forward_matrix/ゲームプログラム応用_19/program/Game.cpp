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

Float3 light_pos;

float attenuation[3];

	float light_radius = 200.0f;
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

	light_pos.set(0.0f,3.0f,0.0f);
	light_radius = 100.0f;
	attenuation[0] = 0.8f;
	attenuation[1] = 0.0f;
	attenuation[2] = 0.01f;
	ChangeLightTypePoint(light_pos.VGet(), 500.0f, attenuation[0], attenuation[1], attenuation[2]); //Reduction of : Closest, Mid, Falloff
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
	Float3 mov = VTransform( base.VGet(), mat );
	player_pos += mov;

	//	�m�o�b�̉�]
	npc_rot.y += 0.5f;

	if( PushHitKey( KEY_INPUT_SPACE ) ){
		ground_num = ( ground_num + 1 ) % 2;
	}

	if( CheckHitKey( KEY_INPUT_RIGHT ) )	light_pos.x += 0.1f;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		light_pos.x -= 0.1f;
	if( CheckHitKey( KEY_INPUT_UP ) )		light_pos.z += 0.1f;
	if( CheckHitKey( KEY_INPUT_DOWN ) )		light_pos.z -= 0.1f;

	if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT)||  CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN))
	{
		ChangeLightTypePoint(light_pos.VGet(), 500.0f, attenuation[0], attenuation[1], attenuation[2]); //Reduction of : Closest, Mid, Falloff
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	SetUseLighting( TRUE );

	//	�n�ʃ��f���̕`��
	MV1DrawModel( ground_model[ground_num] );

	//	�m�o�b���W�������ݒ肵�ă��f���̕`��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	�v���C���[���W�������ݒ肵�ă��f���̕`��
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	SetUseLighting( FALSE );

	DrawSphere3D( light_pos.VGet(), 0.1f, 100, GetColor( 255, 255, 0 ), GetColor( 255, 255, 0 ), TRUE );

	DrawString( 16, 16, "�\���L�[�F���C�g���W�̕ύX", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�X�y�[�X�F�n�ʃ��f���̕ύX", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 64, GetColor( 255, 255, 255 ), "X:%5.2f Y:%5.2f Z:%5.2f", light_pos.x, light_pos.y, light_pos.z );
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
