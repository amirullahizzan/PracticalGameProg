#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

#define PLAYER_MOV_SPEED	0.2f
#define PLAYER_ROT_SPEED	2.0f

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;

//	�v���C���[�̍��W�����
Float3 player_pos;
Float3 player_rot;

//	�R�[�X��ɂ��邩�ǂ���
bool on_road;
//	�ǂ��̃|�C���g�̏�ɂ��邩
int point_num;

enum
{
	MODE_WAIT,
	MODE_PLAY,
	MODE_GOAL
};

int game_mode;
int frame_count;
int check_point;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 20.0f, -40.0f ), VGet( 0.0f, 0.0f, -10.0f ) );

	//	�n�ʃ��f���i���̃��f���̒���������擾���܂��j
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );

	//	�n�ʃ��f���Ƃ̓����蔻����������̂œ�������̍쐬
	MV1SetupCollInfo( ground_model );

	player_pos.set( -15.0f, 0.0f, -15.0f );
	player_rot.clear();

	game_mode = MODE_WAIT;
	frame_count = 0;
	check_point = 0;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	��������̍X�V
	MV1RefreshCollInfo( ground_model );

	//	���E�L�[�ŉ�]
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	��L�[�Ō����Ă�������Ɉړ�
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		if (on_road == false)
		{
			mov *= 0.2f;
		}
		player_pos += mov;
		if (game_mode == MODE_WAIT)
		{
			game_mode = MODE_PLAY;
		}
	}

	//	�R�[�X��ɂ��Ȃ��Ƃ������Ƃɂ���
	on_road = false;
	//	�|�C���g�̏�ɂ��Ȃ��Ƃ���
	point_num = -1;

	//	�v���C���[�̏�Ɖ��̍��W
	Float3 line_start = player_pos + Float3( 0.0f, 10.0f, 0.0f );
	Float3 line_goal = player_pos + Float3( 0.0f, -10.0f, 0.0f );

	//	��ō�������ƃ��f���̓����蔻��
	//	�n�ʃ��f���̒��̃t���[���P�ԂƓ������Ă��邩�ǂ����̎擾
	MV1_COLL_RESULT_POLY hit_info1 = MV1CollCheck_Line( ground_model, 1, line_start.VGet(), line_goal.VGet() );
	//	�������񂪎擾�ł����̂ł����
	if( hit_info1.HitFlag ){
		//	�R�[�X��ɂ���
		on_road = true;
	}

	//	�n�ʃ��f���̒��̃t���[���Q�Ɠ����蔻��
	MV1_COLL_RESULT_POLY hit_info2 = MV1CollCheck_Line( ground_model, 2, line_start.VGet(), line_goal.VGet() );
	if( hit_info2.HitFlag )
	{
		//	�t���[���Q�̏�ɏ���Ă���̂ŁA���ɂǂ̐F�̏�ɏ���Ă��邩���擾
		point_num = hit_info2.MaterialIndex;

		if (point_num == check_point + 1)
		{
			check_point = point_num;
		}

		if (point_num == 0 && check_point == 3)
		{
			game_mode = MODE_GOAL;
		}
	}

	if (game_mode == MODE_PLAY)
	{
	frame_count++;
	}

	if (PushHitKey(KEY_INPUT_R))
	{
		player_pos.set(-15.0f,0.0f,-15.0f);
		player_rot.clear();

		game_mode = MODE_WAIT;

		frame_count = 0;
		check_point = 0;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�X�y�[�X�L�[�Ń|�C���g�̎l�p�̕\���E��\��
	if( CheckHitKey( KEY_INPUT_SPACE ) ){
		//	�n�ʃ��f���̃t���[���Q�Ԃ�\�����܂�
		MV1SetFrameVisible( ground_model, 2, TRUE );
	}else{
		//	�n�ʃ��f���̃t���[���Q�Ԃ��\���ɂ��܂�
		MV1SetFrameVisible( ground_model, 2, FALSE );
	}
	//	�n�ʃ��f���̕`��
	MV1DrawModel( ground_model );

	MV1SetPosition( player_model, player_pos.VGet() );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	int minute = frame_count / (60 * 60);
	int second = (frame_count / 60);
	int ms = (frame_count % 60) * 16;
	DrawFormatString(16,112,GetColor(255,255,0), "%02d:%02d:%03d", minute,second,ms);

	DrawString( 16, 16, "�\���L�[�F�v���C���[��]�E�ړ�", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "���@�R�[�X�A�E�g���̓X�s�[�h���x���Ȃ�", GetColor( 255, 255, 0 ) );

	//	�ϐ��̏�Ԃɍ��킹�ĕ�����̕`��
	if( on_road ){
		DrawString( 16, 48, "�v���C���[�̓R�[�X��ɂ��܂�", GetColor( 255, 255, 255 ) );
	}else{
		DrawString( 16, 48, "�R�[�X�A�E�g���Ă��܂�", GetColor( 255, 255, 255 ) );
	}
	//	�ǂ����̃|�C���g�ɏ���Ă�����
	if( point_num >= 0 ){
		DrawFormatString( 16, 64, GetColor( 255, 255, 255 ), "�|�C���g %d �ɂ��܂�", point_num );
	}
	DrawString( 16, 80, "�R�[�X�����v���ɂP�����čĂу|�C���g�O�i����O�j�ɖ߂��Ă���܂ł̎��Ԃ��v��", GetColor( 255, 255, 0 ) );
	DrawString( 16, 96, "��L�[�ŃX�^�[�g������J�E���g�J�n�E�P�������玞�v���~�߂�", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
}
