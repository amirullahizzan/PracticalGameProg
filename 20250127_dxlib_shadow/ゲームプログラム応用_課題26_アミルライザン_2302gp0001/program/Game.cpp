#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

//	�v���C���[�̈ړ��X�s�[�h�i�ۑ�p�ɏ����ύX���Ă��܂��j
#define PLAYER_MOV_SPEED	0.1f
//	�v���C���[�̉�]�X�s�[�h�i�ۑ�p�ɏ����ύX���Ă��܂��j
#define PLAYER_ROT_SPEED	0.35f
//	�m�o�b�̉�]�X�s�[�h
#define NPC_ROT_SPEED		0.5f

//	���f���p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

int chara_model;
int anim_model;
int anim_attach;
float anim_total;
float anim_time;

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

//	���C�g�̕����i���̕����ɉe���o����j
Float3 light_dir;
//	�V���h�E�}�b�v�n���h���p�̕ϐ�
int shadow_handle;

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

	//	�O�F���C�g�̕����ݒ�
	light_dir.set( 1.5f, -1.2f, 1.3f );
	//	���̕�����n���ă��C�g�̐ݒ�
	SetLightDirection( light_dir.VGet() );

	//	�P�F�V���h�E�}�b�v�n���h���̍쐬
	shadow_handle = MakeShadowMap( 2048, 2048 );
	//	�Q�F�V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection( shadow_handle, light_dir.VGet() );
	//	�R�F�V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	SetShadowMapDrawArea( shadow_handle, VGet( -10.0f, -1.0f, -10.0f ), VGet( 10.0f, 10.0f, 10.0f ) );

	chara_model = MV1LoadModel("data/chara/model.mv1");
	anim_model = MV1LoadModel("data/chara/anim_walk.mv1");
	anim_attach = MV1AttachAnim(chara_model, 1 ,anim_model,FALSE);
	anim_total = MV1GetAttachAnimTotalTime(chara_model,anim_attach);
	anim_time = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	anim_time += 1.0F;
	if (anim_time >= anim_total)
	{
		anim_time = 0.0f;
	}

	//	�v���C���[�̉�]
	player_rot.y += PLAYER_ROT_SPEED;
	//	�����Ă�������Ɉړ�
	Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
	MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
	Float3 mov = GetFloat3VTransform( base, mat );
	player_pos += mov;

	//	�v���C���[������J����
	//	������W�̓v���C���[���W
	camera_look = player_pos;
	//	�J�������W�͌�����W������̏�
	camera_pos = camera_look + Float3( 0.0f, 7.0f, -10.0f );

	//	�m�o�b�̉�]
	npc_rot.y += NPC_ROT_SPEED;

	//	�\���L�[�Ń��C�g�����̕ύX
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	light_dir.x += 0.1f;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		light_dir.x -= 0.1f;
	if( CheckHitKey( KEY_INPUT_UP ) )		light_dir.z += 0.1f;
	if( CheckHitKey( KEY_INPUT_DOWN ) )		light_dir.z -= 0.1f;

	SetShadowMapLightDirection(shadow_handle, light_dir.VGet());

	Float3 pos1 = camera_look + Float3(-20.0f,-1.0f,-20.0f);
	Float3 pos2 = camera_look + Float3(20.0f, 10.0f,20.0f);

	SetShadowMapDrawArea(shadow_handle, pos1.VGet(), pos2.VGet());
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	SetCameraPositionAndTarget_UpVecY( camera_pos.VGet(), camera_look.VGet() );

	//Toggles between shadow and object
	ShadowMap_DrawSetup( shadow_handle );

	MV1DrawModel( ground_model );
	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		MV1SetPosition(chara_model, VGet(player_pos.x,player_pos.y, player_pos.z));
		MV1SetRotationXYZ(chara_model,VGet(TO_RADIAN(player_pos.x),TO_RADIAN(player_rot.y), TO_RADIAN(player_rot.z)));
		MV1SetScale(chara_model,VGet(0.01f,0.01f,0.01f));
		MV1DrawModel(chara_model);
	}
	else
	{
		MV1DrawModel(player_model);
	}

	//Toggles between shadow and object
	ShadowMap_DrawEnd();

	SetUseShadowMap( 0, shadow_handle );

	MV1DrawModel( ground_model );
	MV1DrawModel( npc_model );
	MV1DrawModel( player_model );

	SetUseShadowMap( 0, -1 );

	DrawString( 16, 16, "�\���L�[�F���C�g�����̕ύX�i�w�y�����j", GetColor( 255, 255, 255 ) );

	DrawFormatString( 16, 32, GetColor( 255, 255, 255 ), "X:%5.2f Y:%5.2f Z:%5.2f", light_dir.x, light_dir.y, light_dir.z );

	DrawString( 16, 48, "���C�g�̕����ɍ��킹�ĉe�̕������ύX", GetColor( 255, 255, 0 ) );
	DrawString( 16, 64, "�v���C���[�̈ړ��ɍ��킹�ĉe�̕`��̈���ύX�i���_���痣��Ă��e�������Ȃ��悤�Ɂj", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	//	�P�O�F�V���h�E�}�b�v�̍폜
	DeleteShadowMap( shadow_handle );
}
