#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Player.h"

//	�v���C���[�̈ړ��X�s�[�h
#define PLAYER_MOV_SPEED	0.1f
//	�v���C���[�̉�]�X�s�[�h
#define PLAYER_ROT_SPEED	1.0f

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void Player::Init(int model)
{
	m_model = model;
	//	�x���W��������ɂ��Ă��܂�
	m_pos.set(0.0f, 1.0f, -5.0f);
	m_rot.clear();
	//	�s��̏�����
	m_mat = MGetIdent();
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void Player::Update()
{
	//	���E�L�[�Ō����̕ύX
	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_pos.y += 0.05f;
		if (m_pos.y > 3.0f)
		{
			m_pos.y = 3.0f;
			m_rot.x *= 0.9f;
		}
		else
		{
			m_rot.x -= PLAYER_ROT_SPEED;
		}
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_pos.y -= 0.05f;
		if (m_pos.y < 0.5f)
		{
			m_pos.y = 0.5f;
			m_rot.x *= 0.9f;
		}
		else
		{
			m_rot.x += PLAYER_ROT_SPEED;
		}
	}
	else
	{
		m_rot *= 0.9f;
	}

	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		//	�E�L�[�������Ă��鎞
		m_rot.y += PLAYER_ROT_SPEED;
		m_rot.z -= PLAYER_ROT_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		//	���L�[�������Ă��鎞
		m_rot.y -= PLAYER_ROT_SPEED;
		m_rot.z += PLAYER_ROT_SPEED;
	}
	else {
		//	�ǂ���������Ă��Ȃ��Ƃ�
		//	�y����]�p�x���ǂ�ǂ�� 0.0f �ɋ߂Â���
		m_rot.z *= 0.9f;
	}

	//	�y���̉�]�p�x�� -30.0f �` 30.0f �ɂ��܂�
	m_rot.z = max(-30.0f, min(m_rot.z, 30.0f));

	//	�����Ă�������Ɉړ��i�x���̉�]�̕����Ɂj
	Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
	MATRIX mat = MGetRotY(TO_RADIAN(m_rot.y));
	Float3 mov = GetFloat3VTransform(base, mat);
	m_pos += mov;

	//	�x���̉�]�s��
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	//	�y���̉�]�s��
	MATRIX mat_z = MGetRotZ(TO_RADIAN(m_rot.z));
	//	���̂Q�̍s����P�ɂ��܂�
	m_mat = MMult(mat_z, mat_y);
	m_mat = MMult(mat_x, m_mat);
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void Player::Render()
{
	//	�v���C���[���f���̕`��
	//	���f���̍��W�̐ݒ�
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));
	//	���f���̌����̐ݒ�i����͂��̏��Ԃ̉�]�ł͂Ȃ��̂ŃR�����g�j
//	MV1SetRotationXYZ( m_model, VGet( TO_RADIAN( m_rot.x ), TO_RADIAN( m_rot.y ), TO_RADIAN( m_rot.z ) ) );
	//	����͂�����ŗp�ӂ����s����g���ă��f���̉�]�������܂�
	MV1SetRotationMatrix(m_model, m_mat);
	//	���f���̕`��
	MV1DrawModel(m_model);

	//	�������̕`��ɂ��܂�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//	�v���C���[�̂���n�ʂ̍��W�ɉ~�̕`��
	Float3 pos = m_pos;
	//	�Ƃ肠�����n�ʂ̍����ɂ��܂����A�d�Ȃ�Ȃ��悤�ɏ���������ɂ����܂�
	pos.y = 0.01f;
	//	���̍��W�ɂw�y�����̉~�̕`��
	DrawCircle3D_XZ(pos, 0.5f, GetColor(0, 0, 0), true);
	//	���̕`��̂��߂ɔ������ݒ��߂��܂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void Player::Exit()
{
}
