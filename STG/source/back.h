#include "pch.h"
#include "define.h"

#ifndef _BACK
#define _BACK

class BACK{
private:
	double b_x, b_y, f_x, f_y;
	//画像ハンドル
	int back_img, f_img;
	int title_img;
	int menu_img, start_img, settings_img, exit_img;
	int pause_img, restart_img, retry_img, gotitle_img;
	int gameclear_img, gameover_img;

	//音声ハンドル
	int select, decide;

	int p_level, p_life, p_score;
	int menu_opt, pause_opt, cfg_opt;

	bool cfg_flag;

public:
	BACK();
	void Stage1_Ini();
	void Title();
	void Menu();
	void Config();
	void Pause();
	void GameClear();
	void GameOver();
	void Move();
	void DrawField();
	void DrawSurround();
};

#endif
