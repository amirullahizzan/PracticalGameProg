#include "Main.h"
#include "Game.h"
#include "Random.h"

//	�p�l���̑傫��
#define PANEL_SIZE	50
//	�p�l����u���鐔
#define MAP_W		5
#define MAP_H		5

int floor_image;

int wall_right_image;
int wall_down_image;
int wall_left_image;
int wall_up_image;

int WallData[MAP_H][MAP_W] =
{
	{ 12, 8, 8, 9,13 },
	{  6, 2, 0, 1, 5 },
	{ 12, 9, 6, 0, 1 },
	{  5, 4, 8, 2, 3 },
	{  7, 6, 2,10,11 },
};

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	floor_image = LoadGraph( "data/floor.bmp" );

	wall_right_image = LoadGraph( "data/wall_right.bmp" );
	wall_down_image  = LoadGraph( "data/wall_down.bmp" );
	wall_left_image  = LoadGraph( "data/wall_left.bmp" );
	wall_up_image    = LoadGraph( "data/wall_up.bmp" );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�}�b�v�̕`��
	for( int h = 0; h < MAP_H; h++ ){
		for( int w = 0; w < MAP_W; w++ ){

			int x = w * PANEL_SIZE + 100;
			int y = h * PANEL_SIZE + 100;

			DrawGraph( x, y, floor_image, TRUE );

			int wall = WallData[h][w];
		}
	}

	DrawString( 16, 16, "�Ǐ��̒l���g���Ă��ꂼ��̃}�X�̏㉺���E�ɕǉ摜��`��", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( floor_image );

	DeleteGraph( wall_right_image );
	DeleteGraph( wall_down_image );
	DeleteGraph( wall_left_image );
	DeleteGraph( wall_up_image );
}
