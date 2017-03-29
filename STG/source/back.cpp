#include "pch.h"
#include "back.h"
#include "control.h"

BACK::BACK(){
	back_img = LoadGraph("img/back.png");
	title_img = LoadGraph("img/title.png");
	menu_img = LoadGraph("img/menu.png");
	start_img = LoadGraph("img/start.png");
	settings_img = LoadGraph("img/settings.png");
	exit_img = LoadGraph("img/exit.png");
	pause_img = LoadGraph("img/pause.png");
	restart_img = LoadGraph("img/restart.png");
	retry_img = LoadGraph("img/retry.png");
	gotitle_img = LoadGraph("img/gotitle.png");
	gameclear_img = LoadGraph("img/gameclear.png");
	gameover_img = LoadGraph("img/gameover.png");

	select = LoadSoundMem("sound/select0.mp3");
	decide = LoadSoundMem("sound/select1.mp3");

	b_x = 0;
	b_y = 0;
	f_x = F_HMIN;
	f_y = F_VMIN;

	p_level = p_life = p_score = 0;
	menu_opt = 0;
	pause_opt = 0;
	cfg_opt = 0;
	
	cfg_flag = false;
}

//�X�e�[�W1������
void BACK::Stage1_Ini(){
	CONTROL &control = CONTROL::GetInstance();
	control.CtrlPauseFlag(false);
	f_img = LoadGraph("img/field.png");
}

//�^�C�g�����
void BACK::Title(){
	KEY &key = KEY::GetInstance();
	static int i = 0;

	DrawGraph(100, 70, title_img, TRUE);
	if(i%50/25)	DrawString(250, 400, "Push Any Key", GetColor(255, 255, 255));
	if(i > 30 && key.any){
		PlaySoundMem(decide, DX_PLAYTYPE_BACK);
		func_state = 1;
		i = 0;
	}
	i++;
}

//���j���[���
void BACK::Menu(){
	CONTROL &control = CONTROL::GetInstance();
	KEY &key = KEY::GetInstance();

	if(!cfg_flag){
		DrawGraph(100, 100, menu_img, TRUE);
		SetDrawBright(100, 100, 100);
		DrawGraph(100, 210, start_img, TRUE);
		DrawGraph(100, 290, settings_img, TRUE);
		DrawGraph(100, 370, exit_img, TRUE);

		switch(menu_opt%3){
		case 0:
			SetDrawBright(255, 255, 255);
			DrawGraph(100, 210, start_img, TRUE);
			break;
		case 1:
			SetDrawBright(255, 255, 255);
			DrawGraph(100, 290, settings_img, TRUE);
			break;
		case 2:
			SetDrawBright(255, 255, 255);
			DrawGraph(100, 370, exit_img, TRUE);
			break;
		}

		//�J�[�\���ړ�
		if(key.down == 1){
			menu_opt++;
			PlaySoundMem(select, DX_PLAYTYPE_BACK);
		}else if(key.up == 1){
			menu_opt += 2;
			PlaySoundMem(select, DX_PLAYTYPE_BACK);
		}else if(key.esc == 1){
			menu_opt = 2;
			PlaySoundMem(select, DX_PLAYTYPE_BACK);
		}

		//�I�����ڂɂ���ď���
		if(key.shot == 1){
			PlaySoundMem(decide, DX_PLAYTYPE_BACK);
			switch(menu_opt%3){
			//�Q�[���X�^�[�g
			case 0:
				func_state = 2;
				control.StageIni();
				break;
			//�ݒ��ʂ�
			case 1:
				cfg_flag = true;
				break;
			//�I��
			case 2:
				func_state = 4;
				break;
			}
			menu_opt = 0;
		}
	}else{
		Config();
	}
}

//�L�[�R���t�B�O���
void BACK::Config(){
	KEY &key = KEY::GetInstance();

	DrawFormatString(100, 70, GetColor(255, 255, 255), "�L�[�R���t�B�O�ݒ�");
	SetDrawBright(100, 100, 100);
	DrawFormatString(100, 100, GetColor(255, 255, 255), "�V���b�g   %02d", key.SetConfig(&key.shot_n));
	DrawFormatString(100, 120, GetColor(255, 255, 255), "�{��       %02d", key.SetConfig(&key.bom_n));
	DrawFormatString(100, 140, GetColor(255, 255, 255), "�ᑬ       %02d", key.SetConfig(&key.slow_n));
	DrawFormatString(100, 160, GetColor(255, 255, 255), "�X�L�b�v   %02d", key.SetConfig(&key.skip_n));
	DrawFormatString(100, 180, GetColor(255, 255, 255), "�ꎞ��~   %02d", key.SetConfig(&key.esc_n));
	DrawFormatString(100, 200, GetColor(255, 255, 255), "�߂�");

	switch(cfg_opt%6){
	case 0:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 100, GetColor(255, 255, 255), "�V���b�g   %02d", key.SetConfig(&key.shot_n, 0));
		break;
	case 1:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 120, GetColor(255, 255, 255), "�{��       %02d", key.SetConfig(&key.bom_n, 1));
		break;
	case 2:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 140, GetColor(255, 255, 255), "�ᑬ       %02d", key.SetConfig(&key.slow_n, 2));
		break;
	case 3:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 160, GetColor(255, 255, 255), "�X�L�b�v   %02d", key.SetConfig(&key.skip_n, 3));
		break;
	case 4:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 180, GetColor(255, 255, 255), "�ꎞ��~   %02d", key.SetConfig(&key.esc_n, 4));
		break;
	case 5:
		SetDrawBright(255, 255, 255);
		DrawFormatString(100, 200, GetColor(255, 255, 255), "�߂�");
		if(key.shot){
			cfg_opt = 0;
			cfg_flag = false;
			key.WriteDat();
		}
		break;
	}

	//�J�[�\���ړ�
	if(key.down == 1){
		cfg_opt++;
	}else if(key.up == 1){
		cfg_opt += 5;
	}
}

//�|�[�Y���
void BACK::Pause(){
	CONTROL &control = CONTROL::GetInstance();
	KEY &key = KEY::GetInstance();

	SetDrawBright(255, 255, 255);
	DrawGraph(100, 100, pause_img, TRUE);
	SetDrawBright(100, 100, 100);
	DrawGraph(100, 180, restart_img, TRUE);
	DrawGraph(100, 230, retry_img, TRUE);
	DrawGraph(100, 280, gotitle_img, TRUE);

	switch(pause_opt%3){
	case 0:
		SetDrawBright(255, 255, 255);
		DrawGraph(100, 180, restart_img, TRUE);
		break;
	case 1:
		SetDrawBright(255, 255, 255);
		DrawGraph(100, 230, retry_img, TRUE);
		break;
	case 2:
		SetDrawBright(255, 255, 255);
		DrawGraph(100, 280, gotitle_img, TRUE);
		break;
	}
	SetDrawBright(100, 100, 100);

	//�J�[�\���ړ�
	if(key.down == 1){
		pause_opt++;
		PlaySoundMem(select, DX_PLAYTYPE_BACK);
	}else if(key.up == 1){
		pause_opt += 2;
		PlaySoundMem(select, DX_PLAYTYPE_BACK);
	}

	//ESC�L�[�ňꎞ��~����
	if(key.esc == 1){
		SetDrawBright(255, 255, 255);
		pause_opt = 0;
		control.CtrlPauseFlag(false);
	}

	//�I�����ڂɂ���ď���
	if(key.shot == 1){
		PlaySoundMem(decide, DX_PLAYTYPE_BACK);
		switch(pause_opt%3){
		//�ꎞ��~����
		case 0:
			break;
		//�X�e�[�W�������A�n�߂���
		case 1:
			control.StageIni();
			func_state = 2;
			break;
		//�^�C�g����ʂ�
		case 2:
			func_state = 0;
			break;
		}
		SetDrawBright(255, 255, 255);
		pause_opt = 0;
		control.CtrlPauseFlag(false);
	}
}	

//�Q�[���N���A���
void BACK::GameClear(){
	KEY &key = KEY::GetInstance();
	DrawRotaGraph(320, 100, 1.0, 0.0, gameclear_img, TRUE);
	DrawFormatString(230, 250, WHITE, "SCORE %8d", p_score);

	if(key.shot == 1) func_state = 0;
}

//�Q�[���I�[�o�[���
void BACK::GameOver(){
	KEY &key = KEY::GetInstance();
	DrawRotaGraph(320, 100,1.0, 0.0, gameover_img, TRUE);
	DrawFormatString(230, 250, WHITE, "SCORE %8d", p_score);

	if(key.shot == 1) func_state = 0;
}

void BACK::Move(){
	f_y += SCROLL_SPEED;
}

//�t�B�[���h�w�i�`��
void BACK::DrawField(){
	DrawGraph((int)f_x, (int)f_y, f_img, FALSE);
	DrawGraph((int)f_x, (int)f_y-440, f_img, FALSE);

	if(f_y == F_VMAX) f_y = F_VMIN;
}

//�X�R�A�{�[�h���`��
void BACK::DrawSurround(){
	CONTROL &control = CONTROL::GetInstance();
	control.GetPlayerInfo(&p_level, &p_life, &p_score);
	int hiscore = control.ReadData();

	//���͂̉摜
	DrawGraph((int)b_x, (int)b_y, back_img, TRUE);

	//���̑��̏��\��
	DrawFormatString(480, 30, WHITE, "HISCORE %08d", p_score > hiscore ? p_score : hiscore);
	DrawFormatString(480, 50, WHITE, "SCORE   %08d", p_score);
	DrawFormatString(480, 70, WHITE, "LIFE    %d", p_life);
	DrawFormatString(480, 90, WHITE, "LEVEL   %d", p_level);
}
