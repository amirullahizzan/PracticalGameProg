#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Float3.h"
#include "Hit.h"

//	パネルサイズ（今回のモデルデータの大きさは 1.0f です）
#define PANEL_SIZE	1.0f

//	マップのサイズ
#define MAP_W		37
#define MAP_H		25
//	マップデータ
int MapData[MAP_H][MAP_W];

//	モデルデータ番号用の変数
int panel_model[2];
FLOAT3 camPos = { 0,-20,-2 };

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	カメラの設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -5.0f), VGet(0.0f, 0.0f, 0.0f));
	//	モデルデータの読み込み
	panel_model[0] = MV1LoadModel("data/panel0.mqoz");
	panel_model[1] = MV1LoadModel("data/panel1.mqoz");

	//	マップの初期設定
	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			//	いったん０初期化
			MapData[h][w] = 0;
		}
	}
}

//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	スペースキーを押したらマップを作成
	if (CheckHitKey(KEY_INPUT_W))
	{
		camPos.y -= 0.1f;
	}
	else if (CheckHitKey(KEY_INPUT_S))
	{
		camPos.y += 0.1f;
	}

	if (PushHitKey(KEY_INPUT_SPACE))
	{
		//	マップの初期設定
		for (int h = 0; h < MAP_H; h++) {
			for (int w = 0; w < MAP_W; w++) {
				//	いったん壁をなし（ 0 ）にします
				MapData[h][w] = 0;
				//	２：ひとつ飛ばしで柱（ -1 ）にする
				if (w % 3 == 0 && h % 3 == 0)
				{
					//	柱用の番号（ -1 ）を割り振ろうと思います
					//	（この柱を倒したら壁（ 0 ）にしていく）
					MapData[h][w] = -1;
				}
				//	１：周りはすべて壁（ 1 ）に
				if (w == 0)			MapData[h][w] = 1;
				if (h == 0)			MapData[h][w] = 1;
				if (w == MAP_W - 1)	MapData[h][w] = 1;
				if (h == MAP_H - 1)	MapData[h][w] = 1;
			}
		}

		//	マップを全部見ていって、柱があったら上下左右のどこか１か所とその柱を壁にします
		for (int h = 3; h < MAP_H - 3; h++)
		{
			for (int w = 3; w < MAP_W - 3; w++)
			{
				//	柱があったら
				if (MapData[h][w] == -1) 
				{
					//	その柱を壁にします
					MapData[h][w] = 1;

					//	一番上とそれ以外の所で取得するランダムの値を変えます
					int random_max = 3;
					if (h == 2)
					{
						//	一番上の柱では０～３のランダムが欲しい
						random_max = 3;
					}
					else {
						//	それ以外の所では０～２のランダムが欲しい
						random_max = 2;
					}

					//	壁じゃないところを壁にしたいので、以下の処理を壁じゃないところを見つけるまで繰り返す
					while (true)
					{
						//	ランダムで上下左右のどこかを壁にします
						//	０：右　１：下　２：左　３：上
						int random = GetRandomI(random_max);
						//	となりの番号を入れる用の変数
						int w1 = w;
						int h1 = h;

						int w2 = w;
						int h2 = h;
						//	ランダムの値に合わせてその隣のマスの場所の番号を決めます
						if (random == 0) { w1 += 1;  w2 += 2; }
						if (random == 1) { h1 += 1;  h2 += 2; }
						if (random == 2) { w1 -= 1;  w2 -= 2; }
						if (random == 3) { h1 -= 1;  h2 -= 2; }

						//	ここが壁じゃなかったら
						if (MapData[h1][w1] == 0)
						{
							//	この場所を壁にします	
							MapData[h1][w1] = 1;
							MapData[h2][w2] = 1;
							//	このループを抜ける
							break;
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	マップの描画
	for (int h = 0; h < MAP_H; h++)
	{
		for (int w = 0; w < MAP_W; w++)
		{
			//	マップ番号の取得
			int num = MapData[h][w];
			//	描画するための座標を設定

			SetCameraPositionAndAngle(camPos, TO_RADIAN(-35.0f), 0, 0);

			FLOAT3 startPos = { w - 18.0f, h - 12,  20.0f };
			FLOAT3 rotation = { TO_RADIAN(-90.0f), 0.0f, 0.0f };

			if (num != 1)
			{
				MV1SetRotationXYZ(panel_model[0], rotation);
				MV1SetPosition(panel_model[0], startPos);
				MV1DrawModel(panel_model[0]);
			}
			else
			{
				MV1SetRotationXYZ(panel_model[1], rotation);
				MV1SetPosition(panel_model[1], startPos);
				MV1DrawModel(panel_model[1]);
			}

			//	モデルの描画座標を設定

			//	モデルの描画

		}
	}

	DrawString(16, 16, "スペース：マップの作成", GetColor(255, 255, 255));
	DrawString(16, 32, "→　３Ｄの四角モデルを使って描画", GetColor(255, 255, 0));

	//	どんなモデルかを確認するためだけに描画していますので実装する際はここから下の描画処理はコメントにします

	//	モデルの描画座標を設定
	//MV1SetPosition( panel_model[0], VGet( -1.0f, 0.0f, 0.0f ) );
	//	モデルの描画
	//MV1DrawModel( panel_model[0] );

	//	モデルの描画座標を設定
	//MV1SetPosition( panel_model[1], VGet( 1.0f, 0.0f, 0.0f ) );
	//	モデルの描画
	//MV1DrawModel( panel_model[1] );

	DrawString(200, 270, "モデル［０］：通れる", GetColor(255, 255, 255));
	DrawString(450, 270, "モデル［１］：通れない", GetColor(255, 255, 255));
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	MV1DeleteModel(panel_model[0]);
	MV1DeleteModel(panel_model[1]);
}
