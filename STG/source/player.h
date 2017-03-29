#include "pch.h"
#include "define.h"

#ifndef _PLAYER
#define _PLAYER

class PLAYER{
private:
	double x, y;
	double spd;
	//�����A�_���[�W�������A�Q�[���I�[�o�[�̃t���O
	bool flag, d_flag, end_flag;
	bool b_flag;
	int level;
	int life;
	int pwr;
	int bom;
	int score;
	int p_kill_num, b_kill_num;

	int img[9];
	int thunder_img[2];
	//�ᑬ���ɕ\������摜
	int l_img;
	int s_img[2];
	int b_img[2];

	int s_snd[2];
	int d_snd;

	int cntr;
	int s_cntr;
	int b_cntr;

	//�V���b�g�\����
	struct P_SHOT_t{
		double x, y;
		double spd;
		double angle;
		double range;
		int type;
		double atk;
		int img;
		bool flag;
	};
	
	P_SHOT_t p_shot[PSHOT_MAX];

public:
	PLAYER();
	void Parameter();
	void Move();
	void Shot();
	void Draw();
	void SetPs(double x, double y, double spd, double angle, double range, double atk, int img);
	bool GetPosition(double *x, double *y);
	void GetInfo(int *level, int *life, int *score);
	bool GetShotInfo(int i, double *x, double *y, double *atk, double *range);
	bool CtrlFlag(int num, bool flag = false);
	bool CtrlDmgFlag(int num, bool flag = false);
	bool GetBomFlag();
	int GetCntr();
	bool CheckEndFlag();
	void ChangeShotFlag(int i, bool flag);
	void ChangeKillNum(int num);
	void ChangePwr(int pwr);
	void ChangeScore(int score);
	void ChangeLife(int life);
};

#endif
