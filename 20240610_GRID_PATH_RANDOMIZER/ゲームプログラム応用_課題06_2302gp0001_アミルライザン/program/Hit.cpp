#include "Main.h"
#include "Game.h"
#include "Hit.h"

//---------------------------------------------------------------------------------
//	球同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckBallHit( Float3 &center1, float radius1, Float3 &center2, float radius2 )
{
	float distance = GetFloat3Distance( center1, center2 );
	float radius = radius1 + radius2;
	if( distance <= radius ){
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	ボックス同士の当たり判定
//	（座標 pos を中心にサイズ size 分プラスマイナスしたボックス：２Ｄのような左上からの大きさでの判定ではないので注意）
//---------------------------------------------------------------------------------
bool CheckBoxHit3D( Float3 &box_pos1, Float3 &box_size1, Float3 &box_pos2, Float3 &box_size2 )
{
	float right1 = box_pos1.x + box_size1.x;
	float left1  = box_pos1.x - box_size1.x;
	float up1    = box_pos1.y + box_size1.y;
	float down1  = box_pos1.y - box_size1.y;
	float front1 = box_pos1.z + box_size1.z;
	float back1  = box_pos1.z - box_size1.z;

	float right2 = box_pos2.x + box_size2.x;
	float left2  = box_pos2.x - box_size2.x;
	float up2    = box_pos2.y + box_size2.y;
	float down2  = box_pos2.y - box_size2.y;
	float front2 = box_pos2.z + box_size2.z;
	float back2  = box_pos2.z - box_size2.z;

	if( right1 >= left2 && left1 <= right2 ){
		if( up1 >= down2 && down1 <= up2 ){
			if( front1 >= back2 && back1 <= front2 ){
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	直線と点の一番近い距離を取得する
//---------------------------------------------------------------------------------
float GetFloat3LinePointNearDistance( Float3 &line_start, Float3 &line_goal, Float3 &point )
{
	Float3 line_dir = line_goal - line_start;
	line_dir.normalize();

	Float3 point_dir = point - line_start;

	float line_dot = GetFloat3Dot( line_dir, point_dir );

	Float3 near_dir = line_dir * line_dot;

	Float3 near_pos = line_start + near_dir;

	float near_distance = 0.0f;

	Float3 dir1 = line_start - near_pos;
	Float3 dir2 = line_goal - near_pos;

	if( GetFloat3Dot( dir1, dir2 ) < 0.0f ){

		near_distance = GetFloat3Distance( point, near_pos );

	}else{

		near_distance = GetFloat3Distance( point, line_start );

		float distance = GetFloat3Distance( point, line_goal );

		if( near_distance > distance ){
			near_distance = distance;
		}
	}

	return near_distance;
}
//---------------------------------------------------------------------------------
//	線と球の当たり判定
//---------------------------------------------------------------------------------
bool CheckLineBallHit( Float3 &line_start, Float3 &line_goal, Float3 &ball_pos, float ball_radius )
{
	float near_distance = GetFloat3LinePointNearDistance( line_start, line_goal, ball_pos );

	if( near_distance <= ball_radius ){
		return true;
	}
	return false;
}
