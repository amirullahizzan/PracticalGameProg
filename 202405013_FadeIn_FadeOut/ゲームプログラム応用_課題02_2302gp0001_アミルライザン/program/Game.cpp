#include "Main.h"
#include "Game.h"

//	�^�C�g����ʂ̉摜�p�̕ϐ�
int title_image;
//	�X�e�[�W��ʂ̉摜�p�̕ϐ�
int stage_image;

//	�V�[���ԍ��̒萔
enum {
	SCENE_TITLE,	//	�^�C�g��
	SCENE_STAGE,	//	�X�e�[�W
};
//	���̃V�[���ԍ�������p�̕ϐ�
int scene_num;

//	�t�F�[�h�֌W
//	���̃t�F�[�h��ԗp�̒萔
enum {
	FADE_NONE,			//	�t�F�[�h��Ԃł͂Ȃ��i�ʏ�̏�ԁj
	//	�e�Q�L�[�F�����x�i�A���t�@�l�j��ύX�����t�F�[�h�i�����ς݁j
	FADE_ALPHA_OUT,		//	�t�F�[�h�A�E�g���i���񂾂񍕂��Ȃ��Ă����F�s�����ɂȂ��Ă����j
	FADE_ALPHA_IN,		//	�t�F�[�h�C�����i���񂾂񍕂��̂��Ȃ��Ȃ��Ă����F�����ɂȂ��Ă����j
	MOVE_RIGHT,
	UP_DOWN
	//	�e�R�L�[�F�����l�p�����Ɉړ�������t�F�[�h

	//	�e�S�L�[�F�㉺���獕���l�p���o�Ă���t�F�[�h�i�㉺�ɂQ�̎l�p��`�悵�Ă��܂��j

};
//	���͂ǂ�ȃt�F�[�h��ԂȂ̂�������p�̕ϐ�
int fade_mode;

int boxPositionX = 0;
int boxPositionY1 = 0;
int boxPositionY2 = SCREEN_H;

//	�����l�p�̓����x�𐧌䂷��ϐ�
//	�i���ǂ̂��炢�̓����x�Ȃ̂��@�O�F���� �` �Q�T�T�F�s�����j
int fade_alpha;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�^�C�g������J�n����
	scene_num = SCENE_TITLE;
	//	���ꂼ��̉摜�̓ǂݍ���
	title_image = LoadGraph("data/title.bmp");
	stage_image = LoadGraph("data/stage.bmp");

	//	�n�܂�Ƃ��̓t�F�[�h���Ă��Ȃ���Ԃ���n�߂�
	fade_mode = FADE_NONE;
	//	�����x�̓�������J�n
	fade_alpha = 0;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�e�V�[�����Ƃ̍X�V����
	switch (scene_num) {
	case SCENE_TITLE:	//	�^�C�g��
		//	���ꂼ��̃t�F�[�h��Ԃ𕪂��܂�
		switch (fade_mode) {
		case FADE_NONE:			//	�t�F�[�h������Ȃ��i�L�[���󂯕t�����ԁj
			//	�e�P�L�[����������X�e�[�W�ɍs�����߂ɃV�[���ԍ��̕ύX
			if (PushHitKey(KEY_INPUT_F1)) {
				scene_num = SCENE_STAGE;
			}
			//	�e�Q�L�[����������t�F�[�h���J�n
			if (PushHitKey(KEY_INPUT_F2)) {
				//	�P�F�t�F�[�h�A�E�g���J�n����
				fade_mode = FADE_ALPHA_OUT;
				//	�����l�p�𓧖�����J�n�i�����F�����x�O�j
				fade_alpha = 0;
			}
			if (PushHitKey(KEY_INPUT_F3))
			{
				boxPositionX = -SCREEN_W;
				fade_alpha = 255;
				fade_mode = MOVE_RIGHT;
			}
			if (PushHitKey(KEY_INPUT_F4))
			{
				fade_mode = UP_DOWN;
			}
			break;

		case FADE_ALPHA_OUT:	//	�t�F�[�h�A�E�g��
			//	�Q�F�t�F�[�h�A�E�g���Ă����i�����l�p�̓����x��s�����Ɂj
			fade_alpha += 10;
			//	�R�F���S�ɍ����Ȃ�����i�s�����ɂȂ�����j
			if (fade_alpha > 255) {
				//	�s�����̒l�ɐݒ�
				fade_alpha = 255;
				//	�X�e�[�W�ɍs���̂ŃV�[���ԍ���؂�ւ���
				scene_num = SCENE_STAGE;
				//	�S�F�X�e�[�W�ɍs������t�F�[�h�C�����������̂Łi����Ȃ��Ɖ�ʂ������܂܂Ȃ̂Łj
				fade_mode = FADE_ALPHA_IN;
			}
			break;
		case MOVE_RIGHT:	//	�t�F�[�h�A�E�g��
			boxPositionX += 25;
			if (boxPositionX > 0)
			{
				scene_num = SCENE_STAGE;
			}
			break;
		case UP_DOWN:	//	�t�F�[�h�C����
			boxPositionY1 += 25;
			boxPositionY2 -= 25;
			if (boxPositionY1 >= SCREEN_H / 2)
			{
				scene_num = SCENE_STAGE;
			}
			break;

		}

		break;

	case SCENE_STAGE:	//	�X�e�[�W

		//	�L�[��������̂̓t�F�[�h���I����Ă���
		switch (fade_mode) {
		case FADE_NONE:		//	�L�[���󂯕t����̂̓t�F�[�h���Ă��Ȃ��Ƃ�
			//	�L�[����������^�C�g���ɖ߂�
			if (PushHitKey(KEY_INPUT_F1) || PushHitKey(KEY_INPUT_F2) || PushHitKey(KEY_INPUT_F3) || PushHitKey(KEY_INPUT_F4)) {
				//	�V�[���ԍ����^�C�g����
				scene_num = SCENE_TITLE;
			}
			break;

		case FADE_ALPHA_IN:	//	�t�F�[�h�C����
			//	�T�F���񂾂񓧖��ɂ��Ă���
			fade_alpha -= 10;
			//	�U�F���S�ɓ����ɂȂ�����
			if (fade_alpha < 0) {
				fade_alpha = 0;
				//	�U�F�ʏ��Ԃɂ��܂�
				fade_mode = FADE_NONE;
			}
			break;

		case MOVE_RIGHT:	//	�t�F�[�h�C����
			if (boxPositionX <= SCREEN_W)
			{
				boxPositionX += 25;
			}
			if (boxPositionX >= SCREEN_W)
			{
				fade_mode = FADE_NONE;
			}
			break;
		case UP_DOWN:	//	�t�F�[�h�C����
			boxPositionY1 -= 25;
			boxPositionY2 += 25;
			if (boxPositionY1 < 0)
			{
				fade_mode = FADE_NONE;
			}
			break;
		}

		break;
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�e�V�[�����Ƃ̕`�揈��
	switch (scene_num) {
	case SCENE_TITLE:	//	�^�C�g��
		//	�^�C�g���p�̉摜�╶����̕`��
		DrawGraph(0, 0, title_image, TRUE);
		DrawString(16, 16, "�^�C�g�����", GetColor(255, 255, 255));
		DrawString(16, 32, "�e�P�L�[�F�����ɃX�e�[�W��ʂ�", GetColor(255, 255, 255));
		DrawString(16, 48, "�e�Q�L�[�F�t�F�[�h�A�E�g���ăX�e�[�W��ʂ�", GetColor(255, 255, 255));
		DrawString(16, 64, "�e�R�L�[�F�X���C�h������悤�Ȑ؂�ւ�", GetColor(255, 255, 0));
		DrawString(16, 80, "�e�S�L�[�F�㉺���獕���Ȃ�悤�Ȑ؂�ւ�", GetColor(255, 255, 0));
		break;

	case SCENE_STAGE:	//	�X�e�[�W
		//	�X�e�[�W�p�̉摜�╶����̕`��
		DrawGraph(0, 0, stage_image, TRUE);
		DrawString(16, 16, "�X�e�[�W���", GetColor(255, 255, 255));
		DrawString(16, 32, "�e�P�`�e�S�L�[�F�����Ƀ^�C�g����ʂ֖߂�", GetColor(255, 255, 255));
		break;
	}

	//	�t�F�[�h�֌W�̕`��
	switch (fade_mode) {
	case FADE_NONE:
		//	�t�F�[�h���Ă��Ȃ��Ƃ��͍����l�p�͓����Ō����Ȃ��̂ŕ`��͂��Ȃ�
		break;

		//	�t�F�[�h�A�E�g�����C�����������l�p��`�悵�悤�Ǝv���܂�
	case FADE_ALPHA_OUT:
	case MOVE_RIGHT:
	case FADE_ALPHA_IN:
		//	�����l�p�̔������ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		//	��ʃT�C�Y�̍����l�p�̕`��
		DrawFillBox(boxPositionX, 0, boxPositionX + SCREEN_W, SCREEN_H, GetColor(0, 0, 0));
		//	�����x�ݒ�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	case UP_DOWN:
		DrawFillBox(0, boxPositionY1, SCREEN_W, 0, GetColor(0, 0, 0));
		DrawFillBox(0, boxPositionY2, SCREEN_W, SCREEN_H, GetColor(0, 0, 0));
		break;
	}

}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph(title_image);
	DeleteGraph(stage_image);
}
