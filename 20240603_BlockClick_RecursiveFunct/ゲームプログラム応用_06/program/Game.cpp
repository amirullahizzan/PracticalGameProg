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

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void ChangePanel( int w, int h, int num )
{
	if (num == 0) return;
	if (MapData[h][w] != num) return;

	MapData[h][w] = 0;

	if (w + 1 < MAP_W) ChangePanel(w+1, h,num);
	if (w -1	 <= 0) ChangePanel(w-1, h,num);
	if (h + 1 < MAP_H) ChangePanel(w, h+1,num);
	if (h -1	 >= 0) ChangePanel(w, h-1,num);
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

	}

	int num = MapData[click_h][click_w];

	if (num != 0)
	{
		ChangePanel(click_w, click_h, num);
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

			//	�����̃}�b�v�ԍ��𕶎���ŕ`�悵�܂�
			DrawFormatString( x, y, GetColor( 255, 255, 255 ), "%d", num );
		}
	}

	DrawString( 16, 16, "�N���b�N�F�p�l���������i�Ȃ����Ă��铯���F�̃p�l�����j", GetColor( 255, 255, 255 ) );
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
