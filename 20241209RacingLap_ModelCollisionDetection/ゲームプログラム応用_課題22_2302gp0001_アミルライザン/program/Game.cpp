#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Random.h"
#include "Float2.h"
#include "Float3.h"

#define PLAYER_MOV_SPEED	0.2f
#define PLAYER_ROT_SPEED	2.0f

//	モデルデータ用の変数
int ground_model;
int player_model;

//	プレイヤーの座標や向き
Float3 player_pos;
Float3 player_rot;

//	コース上にいるかどうか
bool on_road;
//	どこのポイントの上にいるか
int point_num;

enum
{
	MODE_WAIT,
	MODE_PLAY,
	MODE_GOAL
};

int game_mode;
int frame_count;
int check_point;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 20.0f, -40.0f ), VGet( 0.0f, 0.0f, -10.0f ) );

	//	地面モデル（このモデルの中から情報を取得します）
	ground_model = MV1LoadModel( "data/ground.mqoz" );
	player_model = MV1LoadModel( "data/player.mqoz" );

	//	地面モデルとの当たり判定をしたいので当たり情報の作成
	MV1SetupCollInfo( ground_model );

	player_pos.set( -15.0f, 0.0f, -15.0f );
	player_rot.clear();

	game_mode = MODE_WAIT;
	frame_count = 0;
	check_point = 0;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	当たり情報の更新
	MV1RefreshCollInfo( ground_model );

	//	左右キーで回転
	if( CheckHitKey( KEY_INPUT_RIGHT ) )	player_rot.y += PLAYER_ROT_SPEED;
	if( CheckHitKey( KEY_INPUT_LEFT ) )		player_rot.y -= PLAYER_ROT_SPEED;
	//	上キーで向いている方向に移動
	if( CheckHitKey( KEY_INPUT_UP ) ){
		Float3 base( 0.0f, 0.0f, PLAYER_MOV_SPEED );
		MATRIX mat = MGetRotY( TO_RADIAN( player_rot.y ) );
		Float3 mov = GetFloat3VTransform( base, mat );
		if (on_road == false)
		{
			mov *= 0.2f;
		}
		player_pos += mov;
		if (game_mode == MODE_WAIT)
		{
			game_mode = MODE_PLAY;
		}
	}

	//	コース上にいないということにして
	on_road = false;
	//	ポイントの上にいないとして
	point_num = -1;

	//	プレイヤーの上と下の座標
	Float3 line_start = player_pos + Float3( 0.0f, 10.0f, 0.0f );
	Float3 line_goal = player_pos + Float3( 0.0f, -10.0f, 0.0f );

	//	上で作った線とモデルの当たり判定
	//	地面モデルの中のフレーム１番と当たっているかどうかの取得
	MV1_COLL_RESULT_POLY hit_info1 = MV1CollCheck_Line( ground_model, 1, line_start.VGet(), line_goal.VGet() );
	//	当たり情報が取得できたのであれば
	if( hit_info1.HitFlag ){
		//	コース上にいる
		on_road = true;
	}

	//	地面モデルの中のフレーム２と当たり判定
	MV1_COLL_RESULT_POLY hit_info2 = MV1CollCheck_Line( ground_model, 2, line_start.VGet(), line_goal.VGet() );
	if( hit_info2.HitFlag )
	{
		//	フレーム２の上に乗っているので、次にどの色の上に乗っているかを取得
		point_num = hit_info2.MaterialIndex;

		if (point_num == check_point + 1)
		{
			check_point = point_num;
		}

		if (point_num == 0 && check_point == 3)
		{
			game_mode = MODE_GOAL;
		}
	}

	if (game_mode == MODE_PLAY)
	{
	frame_count++;
	}

	if (PushHitKey(KEY_INPUT_R))
	{
		player_pos.set(-15.0f,0.0f,-15.0f);
		player_rot.clear();

		game_mode = MODE_WAIT;

		frame_count = 0;
		check_point = 0;
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	スペースキーでポイントの四角の表示・非表示
	if( CheckHitKey( KEY_INPUT_SPACE ) ){
		//	地面モデルのフレーム２番を表示します
		MV1SetFrameVisible( ground_model, 2, TRUE );
	}else{
		//	地面モデルのフレーム２番を非表示にします
		MV1SetFrameVisible( ground_model, 2, FALSE );
	}
	//	地面モデルの描画
	MV1DrawModel( ground_model );

	MV1SetPosition( player_model, player_pos.VGet() );
	MV1SetRotationXYZ( player_model, VGet( TO_RADIAN( player_rot.x ), TO_RADIAN( player_rot.y ), TO_RADIAN( player_rot.z ) ) );
	MV1DrawModel( player_model );

	int minute = frame_count / (60 * 60);
	int second = (frame_count / 60);
	int ms = (frame_count % 60) * 16;
	DrawFormatString(16,112,GetColor(255,255,0), "%02d:%02d:%03d", minute,second,ms);

	DrawString( 16, 16, "十字キー：プレイヤー回転・移動", GetColor( 255, 255, 255 ) );
	DrawString( 16, 32, "→　コースアウト中はスピードが遅くなる", GetColor( 255, 255, 0 ) );

	//	変数の状態に合わせて文字列の描画
	if( on_road ){
		DrawString( 16, 48, "プレイヤーはコース上にいます", GetColor( 255, 255, 255 ) );
	}else{
		DrawString( 16, 48, "コースアウトしています", GetColor( 255, 255, 255 ) );
	}
	//	どこかのポイントに乗っていたら
	if( point_num >= 0 ){
		DrawFormatString( 16, 64, GetColor( 255, 255, 255 ), "ポイント %d にいます", point_num );
	}
	DrawString( 16, 80, "コースを時計回りに１周して再びポイント０（左手前）に戻ってくるまでの時間を計測", GetColor( 255, 255, 0 ) );
	DrawString( 16, 96, "上キーでスタートしたらカウント開始・１周したら時計を止める", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel( ground_model );
	MV1DeleteModel( player_model );
}
