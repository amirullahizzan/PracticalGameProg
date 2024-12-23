#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�A�j���[�V�����f�[�^�p�̕ϐ�
int anim_model;

Float3	player_pos;
Float3	player_rot;
int player_attach;
float player_total;
float player_frame;

Float3	npc_pos;
Float3	npc_rot;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�����ݒ�
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 2.0f, -5.0f ), VGet( 0.0f, 1.0f, -1.0f ) );

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/chara/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model = MV1LoadModel( "data/chara/anim_walk.mv1" );

	player_pos.set( 1.5f, 0.0f, -1.0f );
	player_rot.y = 230.0f;

	//	�v���C���[�A�j���[�V�����̏����ݒ�
	player_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	player_total = MV1GetAttachAnimTotalTime( player_model, player_attach );
	player_frame = 0.0f;

	npc_pos.set( -1.5f, 0.0f, -1.0f );
	npc_rot.set( 0.0f, 130.0f, 0.0f );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�A�j���[�V�����̍Đ�
	player_frame += 1.0f;
	if( player_frame >= player_total ){
		player_frame = 0.0f;
	}
	//	���̃t���[���̃A�j���[�V�����ɂ���
	MV1SetAttachAnimTime( player_model, player_attach, player_frame );

}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	MV1DrawModel( ground_model );

	//	�m�o�b���f���̕`��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y + 180.0f ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	�v���C���[���f���̕`��
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	MV1DrawModel( player_model );

	DrawString( 16, 16, "�m�o�b���f���̓v���C���[���f���̕�����F�ς���������", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	�A�j���[�V�����f�[�^�̏I������
	MV1DeleteModel( anim_model );
}
