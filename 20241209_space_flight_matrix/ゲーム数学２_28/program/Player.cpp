#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Player.h"

//	プレイヤーの移動スピード
#define PLAYER_MOV_SPEED	0.1f
//	プレイヤーの回転スピード
#define PLAYER_ROT_SPEED	1.0f

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void Player::Init( int model )
{
	m_model = model;

	m_pos.set( 0.0f, 1.0f, -5.0f );
	m_rot.clear();
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Player::Update()
{
	//	左右キーで向きの変更
	if( CheckHitKey( KEY_INPUT_RIGHT ) ){
		m_rot.y += PLAYER_ROT_SPEED;
	}
	if( CheckHitKey( KEY_INPUT_LEFT ) ){
		m_rot.y -= PLAYER_ROT_SPEED;
	}

	//	向いている方向に移動
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( m_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		m_pos += mov;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void Player::Render()
{
	MV1SetPosition( m_model, VGet( m_pos.x, m_pos.y, m_pos.z ) );
	MV1SetRotationXYZ( m_model, VGet( TO_RADIAN( m_rot.x ), TO_RADIAN( m_rot.y ), TO_RADIAN( m_rot.z ) ) );
	MV1DrawModel( m_model );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Player::Exit()
{
}
