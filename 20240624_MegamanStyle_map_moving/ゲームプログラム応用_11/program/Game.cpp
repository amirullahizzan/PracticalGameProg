#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float2.h"
#include "Hit.h"

//	�P�̎l�p�̑傫��
#define PANEL_SIZE	50
//	�l�p��u���鐔
#define MAP_W		16
#define	MAP_H		 9

//	�Q�����z��i�O�F�i�߂�@�P�F�i�߂Ȃ��j
int MapData[MAP_H][MAP_W] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//	�}�b�v�摜
int panel_image[2];

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_SPEED	5.0f

//	�v���C���[�摜
int player_image;
//	�v���C���[���W
Float2 player_pos;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	panel_image[0] = LoadGraph( "data/panel0.bmp" );
	panel_image[1] = LoadGraph( "data/panel1.bmp" );

	player_image = LoadGraph( "data/player.bmp" );
	player_pos.x = 75.0f;
	player_pos.y = 75.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	if( CheckHitKey( KEY_INPUT_RIGHT ) ){
		player_pos.x += PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_LEFT ) ){
		player_pos.x -= PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_DOWN ) ){
		player_pos.y += PLAYER_SPEED;
	}
	else if( CheckHitKey( KEY_INPUT_UP ) ){
		player_pos.y -= PLAYER_SPEED;
	}
	Float2 before;

	Float2 box_pos;
	Float2 box_size;

	Float2 panel_pos;
	Float2 panel_size;

	if (CheckBoxHit(box_pos, box_size, panel_pos, panel_size))
	{
		if (before.x < player_pos.x) player_pos.x = panel_pos.x - PANEL_SIZE;
		if (before.x > player_pos.x) player_pos.x = panel_pos.x + PANEL_SIZE;
		if (before.y < player_pos.x) player_pos.y = panel_pos.y - PANEL_SIZE;
		if (before.y > player_pos.x) player_pos.y = panel_pos.y + PANEL_SIZE;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�}�b�v�̕`��
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){
			//	�����̏ꏊ�̔ԍ�
			int num = MapData[h][w];
			//	�l�p��`�悷����W
			int x = w * PANEL_SIZE;
			int y = h * PANEL_SIZE;
			//	�����ɂ��̔ԍ��̎l�p�̕`��
			DrawGraph( x, y, panel_image[num], TRUE );
		}
	}

	//	�v���C���[�̍��W���g���ĕ`�悵�܂�
	float px = player_pos.x;
	float py = player_pos.y;
	DrawGraphF( px, py, player_image, TRUE );

	DrawString( 16, 16, "�\���L�[�F�v���C���[�ړ�", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( panel_image[0] );
	DeleteGraph( panel_image[1] );

	DeleteGraph( player_image );
}
