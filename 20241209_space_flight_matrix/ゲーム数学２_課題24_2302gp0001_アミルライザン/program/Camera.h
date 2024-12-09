#pragma once

#include "Float3.h"

//---------------------------------------------------------------------------------
//	カメラクラス
//---------------------------------------------------------------------------------
class Camera
{
public:
	//	カメラ座標
	Float3		m_pos;
	//	カメラの向き
	Float3		m_rot;

	void Init();		//	初期化処理
	void Update(Float3 player_pos, Float3 player_rot);
	void Render();		//	描画処理
	void Exit();		//	終了処理
};
