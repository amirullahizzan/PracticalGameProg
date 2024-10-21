#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"

//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	2.0f
//	�A�j���[�V���������̕ύX�X�s�[�h
#define ANIM_RATE_SPEED		0.05f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;
int npc_model;

//	�J�������W�ƌ���
Float3 camera_pos;
Float3 camera_rot;

//	�A�j���[�V�����̎��
enum {
	ANIM_WALK,		//	����
	ANIM_PUNCH,		//	�p���`
	ANIM_KICK,		//	�p���`

	ANIM_MAX,		//	�A�j���[�V������
};
//	�A�j���[�V�����f�[�^�p�̕ϐ�
int anim_model[ANIM_MAX];
//	�A�^�b�`�ԍ��p�̕ϐ�
int anim_attach[ANIM_MAX];
//	���̃A�j���[�V�����̂P���[�v�ɂ����鎞��
float anim_total[ANIM_MAX];
//	�A�j���[�V�������ǂ̂��炢�i�߂邩�̎���
float anim_time[ANIM_MAX];
//	���ꂼ��̃A�j���[�V�������ǂ̂��炢�̊����ɂ��邩�i 0.0f �` 1.0f �j
float anim_rate[ANIM_MAX];

//	�ǂ̃A�j���[�V�������Đ����Ă���̂�
int playing_anim;

bool reserve_kick;

//	�v���C���[���W�ƌ���
Float3 player_pos;
Float3 player_rot;
//	�m�o�b���W�ƌ���
Float3 npc_pos;
Float3 npc_rot;

Float3 camera_offset;
Float3 pos;
Float3 npc_push;
//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�J�������W�̏����ݒ�
	camera_pos.set( 0.0f, 2.0f, -3.75f );
	//	�J�����̌���
	camera_rot.set( 13.0f, 0.0f, 0.0f );

	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	//	�v���C���[���f���i���łɃA�j���[�V������t������ǂݍ���ł��܂��j
	player_model = MV1LoadModel( "data/player/model.mv1" );
	npc_model = MV1LoadModel( "data/npc.mqoz" );

	//	�A�j���[�V�����f�[�^�̓ǂݍ���
	anim_model[ANIM_WALK] = MV1LoadModel( "data/player/anim_walk.mv1" );
	anim_model[ANIM_PUNCH] = MV1LoadModel( "data/player/anim_punch.mv1" );
	anim_model[ANIM_KICK] = MV1LoadModel( "data/player/anim_kick.mv1" );

	//	�������W������̐ݒ�
	player_pos.set( 1.5f, 0.0f, 0.0f );
	player_rot.y = 230.0f;

	npc_pos.set( -1.5f, 0.0f, 1.5f );

	//	�A�j���[�V�����̏�����
	for( int i = 0; i < ANIM_MAX; i++ )
	{
		//	���f���ɃA�j���[�V�������A�^�b�`���܂��i�A�^�b�`�ԍ����擾�j
		anim_attach[i] = MV1AttachAnim( player_model, 1, anim_model[i], FALSE );
		//	���̃A�j���[�V�����̂P���[�v�ɂ����鎞��
		anim_total[i] = MV1GetAttachAnimTotalTime( player_model, anim_attach[i] );
		//	�ŏ�����J�n
		anim_time[i] = 0.0f;
		//	�������񊄍����O������
		anim_rate[i] = 0.0f;
	}
	//	�����̊����� 1.0 �ŊJ�n
	anim_rate[ANIM_WALK] = 1.0f;

	//	�p���`�A�j���[�V�������f�^�b�`���܂�
	MV1DetachAnim( player_model, anim_attach[ANIM_KICK] );
	MV1DetachAnim( player_model, anim_attach[ANIM_PUNCH] );
	//	�f�^�b�`���܂����̂ŃA�^�b�`�ԍ��������̏�Ԃɂ��Ƃ��܂�
	anim_attach[ANIM_PUNCH] = -1;
	anim_attach[ANIM_KICK] = -1;

	//	�J�n�͕����A�j���[�V��������
	playing_anim = ANIM_WALK;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	���E�L�[�Ō����̕ύX
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;

	//	�A�j���[�V�����ɂ���ď����𕪂��܂�
	switch( playing_anim ){
	case ANIM_WALK:
		//	���ꂼ��̃A�j���[�V�����̊����̕ύX
		anim_rate[ANIM_WALK] += ANIM_RATE_SPEED;
		anim_rate[ANIM_PUNCH] -= ANIM_RATE_SPEED;
		//	�p���`�̊����� 0.0f �ɂȂ�����i����ɂ܂��A�^�b�`���Ȃ�΁j
		if( anim_rate[ANIM_PUNCH] <= 0.0f && anim_attach[ANIM_PUNCH] != -1 )
		{
			//	�p���`�A�j���[�V�������f�^�b�`���܂�
			MV1DetachAnim( player_model, anim_attach[ANIM_PUNCH] );
			//	�f�^�b�`���܂����̂ŃA�^�b�`�ԍ��������̏�Ԃɂ��Ƃ��܂�
			anim_attach[ANIM_PUNCH] = -1;
		}
		if (anim_rate[ANIM_KICK] <= 0.0f && anim_attach[ANIM_KICK] != -1)
		{
			//	�p���`�A�j���[�V�������f�^�b�`���܂�
			MV1DetachAnim(player_model, anim_attach[ANIM_KICK]);
			//	�f�^�b�`���܂����̂ŃA�^�b�`�ԍ��������̏�Ԃɂ��Ƃ��܂�
			anim_attach[ANIM_KICK] = -1;
		}

		//	�X�y�[�X�L�[����������
		if( PushHitKey( KEY_INPUT_SPACE ) ){
			//	�p���`���A�^�b�`����Ă��Ȃ�������
			if( anim_attach[ANIM_PUNCH] == -1 ){
				//	�p���`�A�j���[�V�������n�߂����̂�
				playing_anim = ANIM_PUNCH;
				//	�p���`�A�j���[�V�������A�^�b�`
				anim_attach[ANIM_PUNCH] = MV1AttachAnim( player_model, 1, anim_model[ANIM_PUNCH], FALSE );
				//	�A�j���̎��Ԃ͍ŏ�����
				anim_time[ANIM_PUNCH] = 0.0f;
				//	�A�j���[�V�����̊����� 0.0 ����
				anim_rate[ANIM_PUNCH] = 0.0f;
			}
		}

		if (PushHitKey(KEY_INPUT_K)) 
		{
			reserve_kick = true;
		}

		if (reserve_kick)
		{
			//	�p���`���A�^�b�`����Ă��Ȃ�������
			if (anim_attach[ANIM_KICK] == -1) {
				//	�p���`�A�j���[�V�������n�߂����̂�
				playing_anim = ANIM_KICK;
				//	�p���`�A�j���[�V�������A�^�b�`
				anim_attach[ANIM_KICK] = MV1AttachAnim(player_model, 1, anim_model[ANIM_KICK], FALSE);
				//	�A�j���̎��Ԃ͍ŏ�����
				anim_time[ANIM_KICK] = 0.0f;
				//	�A�j���[�V�����̊����� 0.0 ����
				anim_rate[ANIM_KICK] = 0.0f;
				reserve_kick = false;
			}
		}
		break;

	case ANIM_PUNCH:
		//	�����̊����������ăp���`�̊������グ��
		anim_rate[ANIM_WALK] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_KICK] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_PUNCH] += ANIM_RATE_SPEED;
		//	�p���`�̊����� 1.0f �ɂȂ�����
		if( anim_rate[ANIM_PUNCH] >= 1.0f ){
			//	���Ԃ�i�߂�
			anim_time[ANIM_PUNCH] += 1.0f;
			//	�Ō�܂ōs������
			if( anim_time[ANIM_PUNCH] >= anim_total[ANIM_PUNCH] ){
				//	�����ɖ߂�
				playing_anim = ANIM_WALK;
			}
		}
		break;

	case ANIM_KICK:
		//	�����̊����������ăp���`�̊������グ��
		anim_rate[ANIM_WALK] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_PUNCH] -= ANIM_RATE_SPEED;
		anim_rate[ANIM_KICK] += ANIM_RATE_SPEED;
		//	�p���`�̊����� 1.0f �ɂȂ�����
		if (anim_rate[ANIM_KICK] >= 1.0f) {
			//	���Ԃ�i�߂�
			anim_time[ANIM_KICK] += 1.0f;
			//	�Ō�܂ōs������
			if (anim_time[ANIM_KICK] >= anim_total[ANIM_KICK]) {
				//	�����ɖ߂�
				playing_anim = ANIM_WALK;
			}

			if (anim_time[ANIM_KICK] >= 40.0f && anim_time[ANIM_KICK] <= 55.0f)
			{
				camera_offset.x = GetRandomF(-0.1f,0.1f);
				camera_offset.y = GetRandomF(-0.1f,0.1f);
				camera_offset.z = GetRandomF(-0.1f,0.1f);

				npc_push = npc_pos - player_pos;
				npc_push.SetLength(0.1f);
			}
			else
			{
				camera_offset.clear();
			}
		}
		break;
	}

	//	���Ԃ�i�߂�i�����A�j���[�V�����̂݁j
	anim_time[ANIM_WALK] += 1.0f;
	if( anim_time[ANIM_WALK] >= anim_total[ANIM_WALK] ){
		//	�Ō�܂ōs������ŏ�����
		anim_time[ANIM_WALK] = 0.0f;
	}

	for( int i = 0; i < ANIM_MAX; i++ )
	{
		//	�A�j���[�V�����̊����� 0.0f �` 1.0f �̊Ԃɂ���
		anim_rate[i] = max( 0.0f, min( anim_rate[i], 1.0f ) );

		//	�A�^�b�`����Ă�����̂̂�
		if( anim_attach[i] >= 0 ){
			//	���̎��Ԃ̃A�j���[�V�����ɂ���
			MV1SetAttachAnimTime( player_model, anim_attach[i], anim_time[i] );
			//	���ꂼ��̃A�j���[�V�����̊������̍Đ��ɂ���
			MV1SetAttachAnimBlendRate( player_model, anim_attach[i], anim_rate[i] );
		}
	}

	
	npc_pos += npc_push;
	npc_push *= 0.9f;
	npc_rot.y += 0.5f;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	Float3 pos = camera_pos + camera_offset;

	//	���W�ƌ�����n���ăJ�����̐ݒ�
	SetCameraPositionAndAngle(pos.VGet(), TO_RADIAN( camera_rot.x ), TO_RADIAN( camera_rot.y ), TO_RADIAN( camera_rot.z ) );

	//	�n�ʃ��f���̕`��
	MV1DrawModel( ground_model );

	MV1SetPosition( npc_model, VGet( npc_pos.x, npc_pos.y, npc_pos.z ) );
	MV1SetRotationXYZ( npc_model, VGet( TO_RADIAN( npc_rot.x ), TO_RADIAN( npc_rot.y ), TO_RADIAN( npc_rot.z ) ) );
	MV1DrawModel( npc_model );

	//	�v���C���[���f���̕`��
	MV1SetPosition( player_model, VGet( player_pos.x, player_pos.y, player_pos.z ) );
	//	����̃��f���͂y�}�C�i�X�����Ɍ����Ă���̂ŕ`�悷��Ƃ��ɂx���ɑ΂��ĂP�W�O�x��]�����܂�
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y + 180.0f ), TO_RADIAN( player_rot.z ) ) );
	//	����̃��f���͂߂���߂���ł����̂ŏ��������܂�
	MV1SetScale( player_model, VGet( 0.01f, 0.01f, 0.01f ) );
	MV1DrawModel( player_model );

	DrawString( 16, 16, "���E�L�[�F�v���C���[�̌����̕ύX", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�X�y�[�X�F�p���`�A�j���[�V����", GetColor( 255, 255, 255 ) );
	DrawString( 16, 48, "�j�L�[�@�F�L�b�N�A�j���[�V����", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
	MV1DeleteModel( npc_model );

	for( int i = 0; i < ANIM_MAX; i++ ){
		MV1DeleteModel( anim_model[i] );
	}
}
