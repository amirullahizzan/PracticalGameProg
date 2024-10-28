#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.025f
//	��]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;
int weapon_model;

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

Float3 ball_pos;


MATRIX model_mat;
//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�ƌ�����W�̏����ݒ�
	camera_pos.set( 0.0f, 2.0f, -5.0f );
	camera_look.set( 0.0f, 1.0f, 0.0f );

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	weapon_model = MV1LoadModel( "data/weapon.mqoz" );
	

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model = MV1LoadModel( "data/player/anim_walk.mv1" );

	//	�������W������̐ݒ�
	player_pos.set( 2.0f, 0.0f, 0.0f );
	player_rot.y = -90.0f;

	npc_pos.set( -2.0f, 0.0f, 2.0f );

	//	�A�j���[�V�����̏�����
	anim_attach = MV1AttachAnim( player_model, 1, anim_model, FALSE );
	anim_total = MV1GetAttachAnimTotalTime( player_model, anim_attach );
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�������Ă���������Ă�������Ɉړ�
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		player_pos += mov;
	}

	//	�A�j���[�V�����̎��Ԃ�i�߂�
	anim_time += 1.0f;
	if( anim_time >= anim_total ){
		anim_time = 0.0f;
	}
	//	���̎��Ԃ̃A�j���[�V�����ɂ���
	MV1SetAttachAnimTime( player_model, anim_attach, anim_time );

	npc_rot.y += 0.5f;

	int frame = MV1SearchFrame(player_model, "mixamorig:RightHandIndex1");
	MATRIX mat = MV1GetFrameLocalWorldMatrix(player_model, frame);

	Float3 base(0.0f, 0.0f, 0.0f);

	ball_pos = GetFloat3VTransform(base,mat);

	model_mat = MV1GetFrameLocalWorldMatrix(player_model, frame);
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�J�������W�ƌ�����W��n���ăJ�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//	�n�ʃ��f���̕`��
	MV1DrawModel( ground_model );

	//	�m�o�b���f���̕`��
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	�v���C���[���f���̕`��
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	MV1DrawModel( player_model );

	//Setting the weapon model into the Player's frame model
	MV1SetMatrix(weapon_model, model_mat);
	
	//VECTOR scale = VECTOR{ 0.0001f, 0.0001f, 0.0001f };
	//MV1SetScale(weapon_model, scale); //Is not possible because
	MV1DrawModel( weapon_model );

	DrawSphere3D( ball_pos.VGet(), 0.1f, 10, GetColor( 255, 255, 0 ), GetColor( 255, 255, 0 ), TRUE );

	DrawString( 16, 16, "�\���L�[�F�v���C���[�̉�]�E�ړ�", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�v���C���[�̉E��ɋ��̕`��", GetColor( 255, 255, 255 ) );
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
