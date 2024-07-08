#include "Main.h"
#include "Game.h"
#include "Random.h"
#include "Hit.h"

//	�t�H���g�n���h��
int font_handle;

//	���j���[���ڐ�
#define MENU_MAX	4
//	�e���j���[���ڂ̕�����
const char* menu_text[MENU_MAX] = {
	"�Q�[���X�^�[�g",
	"�`���[�g���A��",
	"�����L���O",
	"�Q�[���ݒ�",
};
//	�e���j���[������̍��W
int menu_x[MENU_MAX];
int menu_y[MENU_MAX];
//	�I��ԍ�
int select_num;

//	�u�������������Łi���W�j.exe�v�Ŏg���Ă��镶����ł�
const char* select_text[MENU_MAX] = {
	"�Q�[�����J�n���܂�",
	"�V�ѕ����݂܂�",
	"���̏��ʂ��m�F���܂�",
	"��Փx�≹�ʂ̐ݒ�����܂�",
};

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�t�H���g�n���h���̍쐬
	font_handle = CreateFontToHandle("���C���I", 50, -1);

	//	���j���[������̍��W�ݒ�
	for (int i = 0; i < MENU_MAX; i++) {
		//	�w���W�͂Ƃ肠������ʍ��ɂ��Ă��܂�

		menu_x[i] = SCREEN_W / 2.0f;
		//	�x���W�͂����Őݒ肵�Ă��܂��i����ȏ�ύX�͂���܂���j
		menu_y[i] = 50 + 80 * i;
	}
	//	�I��ԍ��O
	select_num = 0;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�㉺�L�[�őI��ԍ����X�V
	if (PushHitKey(KEY_INPUT_DOWN))	select_num++;
	if (PushHitKey(KEY_INPUT_UP))	select_num--;
	//	���j���[�̏㉺�𒴂����甽�΂���
	if (select_num < 0)	select_num = MENU_MAX - 1;
	if (select_num >= MENU_MAX)	select_num = 0;
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------

int contextPosX[2] = { 16 , SCREEN_W + 16 };

void GameRender()
{
	//	���j���[���ڕ�����̕`��
	for (int i = 0; i < MENU_MAX; i++) {
		//	�f�t�H���g�͔��F
		int color = GetColor(255, 255, 255);
		//	�I��ԍ��Ɠ���������͉��F
		const float menuXmoveSpeed = 9;
		if (select_num == i)
		{
			//	�I��ԍ��Ɠ���������͉��F
			color = GetColor(255, 255, 0);

			const int maxMoveMenuX = 280;
			if (menu_x[i] > maxMoveMenuX)
			{
				menu_x[i] -= menuXmoveSpeed;
			}

			for (int j = 0; j < 2; j++)
			{
				contextPosX[j]--;
				int buffX, buffY;
				GetDrawFormatStringSizeToHandle(&buffX, &buffY, NULL, font_handle, select_text[i]);

				if (contextPosX[j] + buffX <= 0)
				{
					contextPosX[j] = SCREEN_W + 16;
				}
			}

		}
		else
		{
			const int minMoveMenuX = SCREEN_W / 2.0f;

			if (menu_x[i] < minMoveMenuX)
			{
				menu_x[i] += menuXmoveSpeed;
			}
		}
		//	��Ō��߂��F���g���ă��j���[������̕`��
		DrawStringToHandle(menu_x[i], menu_y[i], menu_text[i], color, font_handle);

		int buffX, buffY;
		GetDrawFormatStringSizeToHandle(&buffX, &buffY, NULL, font_handle, menu_text[i]);
		DrawLineBox(menu_x[i], menu_y[i], menu_x[i] + buffX, menu_y[i] + buffY, GetColor(255, 255, 0));
	}

	int contextBoxSizeY = 60.0f;
	int contextBoxPosY = SCREEN_H - 30;

	DrawBox(0, contextBoxPosY, SCREEN_W, contextBoxPosY - contextBoxSizeY, GetColor(0, 0, 0), 1);


	for (int i = 0; i < MENU_MAX; i++)
	{
		if (select_num == i)
		{
			for (int j = 0; j < 2; j++)
			{
				DrawStringToHandle(contextPosX[j], contextBoxPosY - contextBoxSizeY + 8, select_text[i], GetColor(255, 255, 255), font_handle);
			}
		}
	}


	


	//DrawString( 16, 16, "���j���[���ڂ̕�����i�㉺�L�[�őI���j", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 32, "�I�����Ă��鍀�ڂ̐F�͊��ɕύX���Ă܂�", GetColor( 255, 255, 255 ) );
	//DrawString( 16, 48, "���@������̒����ɍ��킹���l�p�ň͂�", GetColor( 255, 255, 0 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
}
