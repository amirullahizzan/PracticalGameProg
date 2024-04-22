#include "Main.h"
#include "Game.h"

//	�摜�f�[�^�p�̕ϐ�
int bg_image;
int player_image;
int npc_image;

//	�킴�Ɠǂݍ��݂�x�����邽�߂Ƀe�X�g�t�@�C������������ǂݍ��܂��Ă����܂�
#define TEST_MAX	2000
//	�e�X�g�t�@�C���p�̕ϐ�
int test_image[TEST_MAX];

//	�V�[���̒�`
enum {
	SCENE_LOAD,		//	���[�h��
	SCENE_GAME,		//	�Q�[����
};
//	���ǂ��̃V�[����
int scene_num;

//	���ǂݍ��ݒ��̃t�@�C���̐����������邩
int loadfile_count;
//	NOW LOADING �𓮂������肵�����̂�
int frame_count;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	���[�hf��ʂ���J�n
	scene_num = SCENE_LOAD;

	//	�w�i�摜�̓ǂݍ���
	bg_image = LoadGraph("data/bg.bmp");
	//	�v���C���[�摜�̓ǂݍ���
	player_image = LoadGraph("data/player.bmp");
	//	�m�o�b�摜�̓ǂݍ���
	npc_image = LoadGraph("data/npc.bmp");
	//	�P�F������Ăяo���ǂݍ��݊֐�������񓯊��̐ݒ�ɂ��܂�
	SetUseASyncLoadFlag( TRUE );

	//	�e�X�g�t�@�C���̓ǂݍ���
	for( int i = 0; i < TEST_MAX; i++ ){
		test_image[i] = LoadGraph( "data/test.bmp" );
	}



	frame_count = 0;
	loadfile_count = 0;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	switch( scene_num ){
	case SCENE_LOAD:
		//	�J�E���g�𑝂₷
		frame_count++;
		//	���ǂݍ��ݒ��̃t�@�C���̐����擾
		loadfile_count = GetASyncLoadNum();
		//	���̐����O�ɂȂ����Ƃ������Ƃ͂��ׂẴt�@�C����ǂݍ���
		if( loadfile_count == 0 ){
			//	�Q�[���V�[���Ɉڍs���܂�
			scene_num = SCENE_GAME;
		}
		break;

	case SCENE_GAME:
		//	����̃v���W�F�N�g�ł̓Q�[����ʂł͓��ɉ������Ă��܂���
		break;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	const float boxWidth = 250.0f;
	const float boxHeight = 50.0f;
	int loadingSize = 0;

	switch( scene_num ){
	case SCENE_LOAD:
		DrawString( 16, 16, "********** ���[�h��� **********", GetColor( 255, 255, 255 ) );
		//	�ǂݍ��ݒ��̃t�@�C��������������̂��𕶎���ŕ`�悵�Ċm�F
		DrawFormatString( 16, 32, GetColor( 255, 255, 255 ), "�ǂݍ��ݒ��̃t�@�C�����F%d", loadfile_count );

		//	Now Loading �̓_��
		if( ( frame_count / 30 ) % 2 ){
			DrawString( 350, 350, "Now Loading", GetColor( 255, 255, 255 ) );
		}
		//	NOW LOADING �̈ړ�
		DrawString( SCREEN_W - frame_count, 400, "NOW LOADING", GetColor( 255, 255, 255 ) );
		DrawRotaGraphF(SCREEN_W - frame_count - 40 , 400, 1.0f, TO_RADIAN(180.0f), npc_image, TRUE);
		DrawRotaGraphF(SCREEN_W - frame_count - 120, 400, 1.0f, TO_RADIAN(180.0f), player_image, TRUE);

		loadingSize = static_cast<float>(loadfile_count) / TEST_MAX * boxWidth /2;
		DrawBox(SCREEN_W/2 - boxWidth , 200, SCREEN_W/2 + boxWidth - loadingSize, 200 + boxHeight,GetColor(255,255,255),1);
		DrawLineBox(SCREEN_W / 2 - boxWidth, 200, SCREEN_W/2 + boxWidth, 200 + boxHeight, GetColor(255, 255, 255));
		break;

	case SCENE_GAME:
		//	�Q�[����ʂł̕`�揈��
		DrawGraph( 0, 0, bg_image, TRUE );
		DrawRotaGraphF( 600.0f, 200.0f, 1.0f, TO_RADIAN( 180.0f ), npc_image, TRUE );
		DrawRotaGraphF( 200.0f, 200.0f, 1.0f, TO_RADIAN( 0.0f ), player_image, TRUE );

		DrawString( 16, 16, "�Q�[�����", GetColor( 255, 255, 255 ) );
		break;
	}
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	//	�e�X�g�t�@�C���̏I������
	for( int i = 0; i < TEST_MAX; i++ ){
		DeleteGraph( test_image[i] );
	}

	DeleteGraph( bg_image );
	DeleteGraph( player_image );
	DeleteGraph( npc_image );
}
