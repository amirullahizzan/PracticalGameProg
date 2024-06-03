#include "Main.h"
#include "Game.h"
#include "Random.h"

//	�p�l���T�C�Y
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		16		//	������
#define MAP_H		 9		//	�c����

//	�p�l���摜�p�̕ϐ��i�O�F�����Ă���@�P�`�R�F���ꂼ��̐F�̃p�l���j
int panel_image[4];

//	�Q�����z��i�O�`�R�̐���������܂��j
int MapData[MAP_H][MAP_W];

//	�N���b�N�����|�C���g�i���W�ł͂Ȃ��j
int click_w;
int click_h;
int score = 0;
int captured = 0;

//---------------------------------------------------------------------------------
//	�p�l���ԍ���ύX���Ă����֐�
//	�}�b�v�f�[�^�� w �Ԗ� h �Ԗڂ̒l�� num �����������
//---------------------------------------------------------------------------------
void ChangePanel( int w, int h, int num )
{
	//	�Q�|�P�F�n���ꂽ�ԍ����O��������I��
	if( num == 0 )	return;
	//	�Q�|�Q�F�����̔ԍ�����ԍ��ƈ���Ă�����I��
	if( MapData[h][w] != num )	return;

	//	�����܂ŗ�����}�b�v�̂��̏ꏊ�̔ԍ��͊�ԍ��Ɠ���
	//	�Q�|�R�F�����̔ԍ��������܂�
	MapData[h][w] = 0;

	//	�Q�|�S�F�㉺���E�ׂ̗̃p�l���œ�������
	if( w + 1 <  MAP_W )	ChangePanel( w + 1, h,     num );	//	�E
	if( w - 1 >=     0 )	ChangePanel( w - 1, h,     num );	//	����
	if( h + 1 <  MAP_H )	ChangePanel( w,     h + 1, num );	//	����
	if( h - 1 >=     0 )	ChangePanel( w,     h - 1, num );	//	���

	score++;
}

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�p�l���摜�̓ǂݍ���
	panel_image[0] = LoadGraph( "data/panel0.bmp" );
	panel_image[1] = LoadGraph( "data/panel1.bmp" );
	panel_image[2] = LoadGraph( "data/panel2.bmp" );
	panel_image[3] = LoadGraph( "data/panel3.bmp" );

	//	�}�b�v�f�[�^�̒��g���P�`�R�̃����_���Ȓl�Ŗ��߂܂�
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	�����_���łP�`�R
			MapData[h][w] = GetRandomI( 1, 3 );
		}
	}
}

//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�}�E�X���N���b�N
	if( PushMouseInput() ){
		//	�N���b�N�������W���z��I�ɉ��Ԗڂ̂Ƃ��납
		click_w = GetMouseX() / PANEL_SIZE;
		click_h = GetMouseY() / PANEL_SIZE;

		//	�P�F�N���b�N�����Ƃ���̔ԍ����擾�i���̔ԍ�����ɂ��܂��j
		int num = MapData[click_h][click_w];
		//	�擾�����ԍ��������Ă��Ȃ�������ȉ��̏��������܂�
		if( num != 0 ){
			//	�P�F�N���b�N�����Ƃ���̏ꏊ�Ɗ�ԍ���n��
			ChangePanel( click_w, click_h, num );
		}
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------

void GameRender()
{
	//	�p�l���̕`��
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	�}�b�v�f�[�^���炻���̏ꏊ�̔ԍ����擾
			int num = MapData[h][w];
			//	��ʂ̂ǂ��ɕ\�����邩�̍��W�����߂�
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	���̏ꏊ�ɂ��̉摜��`��
			DrawGraph( x, y, panel_image[num], TRUE );
		}
	}
	int currentBarWidth = (score * 400) / 144;
	//	����������₷�����邽�߂ɍ��̔������̎l�p��`�悵�Ă��܂�
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 100 );
	DrawFillBox( 0, 0, 570, 120, GetColor( 0, 0, 0 ) );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );

	DrawFormatString(16,36, GetColor(255,255,255), "%d �������܂���", score);
	DrawString( 16, 16, "�N���b�N�F�p�l���������i�Ȃ����Ă��铯���F�̃p�l�����j", GetColor( 255, 255, 255 ) );

	DrawFillBox(16, 80, 416, 110, GetColor(255, 255, 255));
	DrawFillBox(16, 80, 16 + currentBarWidth, 110, GetColor(12, 12, 255));
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( panel_image[0] );
	DeleteGraph( panel_image[1] );
	DeleteGraph( panel_image[2] );
	DeleteGraph( panel_image[3] );
}
