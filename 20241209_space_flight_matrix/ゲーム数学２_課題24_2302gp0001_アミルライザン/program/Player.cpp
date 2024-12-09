#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Player.h"

//	ƒvƒŒƒCƒ„[‚ÌˆÚ“®ƒXƒs[ƒh
#define PLAYER_MOV_SPEED	0.1f
//	ƒvƒŒƒCƒ„[‚Ì‰ñ“]ƒXƒs[ƒh
#define PLAYER_ROT_SPEED	1.0f

//---------------------------------------------------------------------------------
//	‰Šú‰»ˆ—
//---------------------------------------------------------------------------------
void Player::Init(int model)
{
	m_model = model;
	//	‚xÀ•W‚ð­‚µã‚É‚µ‚Ä‚¢‚Ü‚·
	m_pos.set(0.0f, 1.0f, -5.0f);
	m_rot.clear();
	//	s—ñ‚Ì‰Šú‰»
	m_mat = MGetIdent();
}
//---------------------------------------------------------------------------------
//	XVˆ—
//---------------------------------------------------------------------------------
void Player::Update()
{
	//	¶‰EƒL[‚ÅŒü‚«‚Ì•ÏX
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
		//	‰EƒL[‚ð‰Ÿ‚µ‚Ä‚¢‚éŽž
		m_rot.y += PLAYER_ROT_SPEED;
		m_rot.z -= PLAYER_ROT_SPEED;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		//	¶ƒL[‚ð‰Ÿ‚µ‚Ä‚¢‚éŽž
		m_rot.y -= PLAYER_ROT_SPEED;
		m_rot.z += PLAYER_ROT_SPEED;
	}
	else {
		//	‚Ç‚¿‚ç‚à‰Ÿ‚µ‚Ä‚¢‚È‚¢‚Æ‚«
		//	‚yŽ²‰ñ“]Šp“x‚ð‚Ç‚ñ‚Ç‚ñ‚Æ 0.0f ‚É‹ß‚Ã‚¯‚é
		m_rot.z *= 0.9f;
	}

	//	‚yŽ²‚Ì‰ñ“]Šp“x‚ð -30.0f ` 30.0f ‚É‚µ‚Ü‚·
	m_rot.z = max(-30.0f, min(m_rot.z, 30.0f));

	//	Œü‚¢‚Ä‚¢‚é•ûŒü‚ÉˆÚ“®i‚xŽ²‚Ì‰ñ“]‚Ì•ûŒü‚Éj
	Float3 base(0.0f, 0.0f, PLAYER_MOV_SPEED);
	MATRIX mat = MGetRotY(TO_RADIAN(m_rot.y));
	Float3 mov = GetFloat3VTransform(base, mat);
	m_pos += mov;

	//	‚xŽ²‚Ì‰ñ“]s—ñ
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	//	‚yŽ²‚Ì‰ñ“]s—ñ
	MATRIX mat_z = MGetRotZ(TO_RADIAN(m_rot.z));
	//	‚±‚Ì‚Q‚Â‚Ìs—ñ‚ð‚PŒÂ‚É‚µ‚Ü‚·
	m_mat = MMult(mat_z, mat_y);
	m_mat = MMult(mat_x, m_mat);
}
//---------------------------------------------------------------------------------
//	•`‰æˆ—
//---------------------------------------------------------------------------------
void Player::Render()
{
	//	ƒvƒŒƒCƒ„[ƒ‚ƒfƒ‹‚Ì•`‰æ
	//	ƒ‚ƒfƒ‹‚ÌÀ•W‚ÌÝ’è
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));
	//	ƒ‚ƒfƒ‹‚ÌŒü‚«‚ÌÝ’èi¡‰ñ‚Í‚±‚Ì‡”Ô‚Ì‰ñ“]‚Å‚Í‚È‚¢‚Ì‚ÅƒRƒƒ“ƒgj
//	MV1SetRotationXYZ( m_model, VGet( TO_RADIAN( m_rot.x ), TO_RADIAN( m_rot.y ), TO_RADIAN( m_rot.z ) ) );
	//	¡‰ñ‚Í‚±‚¿‚ç‚Å—pˆÓ‚µ‚½s—ñ‚ðŽg‚Á‚Äƒ‚ƒfƒ‹‚Ì‰ñ“]‚ð‚³‚¹‚Ü‚·
	MV1SetRotationMatrix(m_model, m_mat);
	//	ƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel(m_model);

	//	”¼“§–¾‚Ì•`‰æ‚É‚µ‚Ü‚·
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//	ƒvƒŒƒCƒ„[‚Ì‚¢‚é’n–Ê‚ÌÀ•W‚É‰~‚Ì•`‰æ
	Float3 pos = m_pos;
	//	‚Æ‚è‚ ‚¦‚¸’n–Ê‚Ì‚‚³‚É‚µ‚Ü‚·‚ªAd‚È‚ç‚È‚¢‚æ‚¤‚É­‚µ‚¾‚¯ã‚É‚ ‚°‚Ü‚·
	pos.y = 0.01f;
	//	‚±‚ÌÀ•W‚É‚w‚y•ûŒü‚Ì‰~‚Ì•`‰æ
	DrawCircle3D_XZ(pos, 0.5f, GetColor(0, 0, 0), true);
	//	ŽŸ‚Ì•`‰æ‚Ì‚½‚ß‚É”¼“§–¾Ý’è‚ð–ß‚µ‚Ü‚·
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//---------------------------------------------------------------------------------
//	I—¹ˆ—
//---------------------------------------------------------------------------------
void Player::Exit()
{
}
