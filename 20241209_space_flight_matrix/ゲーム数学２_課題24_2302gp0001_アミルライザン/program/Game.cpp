#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float3.h"

#include "Camera.h"
#include "Ground.h"
#include "Player.h"

//	���f���f�[�^�p�̕ϐ�
int ground_model;
int player_model;

//	�N���X�̐錾
Camera	camera;
Ground	ground;
Player	player;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	���f���f�[�^�̓ǂݍ���
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );

	//	�N���X�̏�����
	camera.Init();
	ground.Init( ground_model );
	player.Init( player_model );
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	ground.Update();

	//	�v���C���[�̍X�V����
	player.Update();

	camera.Update(player.m_pos, player.m_rot);
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�J�����̐ݒ�
	camera.Render();
	//	���f���̕`��
	ground.Render();
	player.Render();

	DrawString( 16, 16, "���E�L�[�F�v���C���[�̉�]", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "�J�������v���C���[�̌���", GetColor( 255, 255, 0 ) );
	DrawString( 16, 48, "���@�v���C���[�̌X���ɍ��킹��悤�ɃJ�����̉�]", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	camera.Exit();
	ground.Exit();
	player.Exit();

	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
}
