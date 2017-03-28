#include "pch.h"
#include "define.h"

#ifndef _BOSS
#define _BOSS

class BOSS{
private:
	double x, y;
	double px, py;
	//移動に使う座標
	double prev_x, prev_y, move_x, move_y;
	double spd, s_spd;
	double s_range[10];
	int ptrn, s_ptrn;
	int cntr, w_cntr, d_cntr;
	int img, s_img[15], d_img[16];
	double hp;
	int s_snd[10];
	//弾幕の残り数
	int spell_n;
	double def;
	int spell[5];
	int first, second, third, fourth, fifth;
	int hp_f, hp_s, hp_t;
	int item;
	//移動に使う角度
	double angle;
	
	//弾幕用の諸変数
	double s_angle;
	double s_raise;
	int s_cntr;
	int times;
	int s_num;
	bool wait;

	//フラグ
	bool flag;
	//ショット、ショット音
	bool s_flag, shotsound_flag;
	//動き
	bool m_flag;
	//死亡
	bool d_flag;

	struct B_SHOT_t{
		double x, y;
		double bx, by;
		double spd;
		double angle;
		double range;
		int ptrn;
		int img;
		int cntr;
		bool flag;
	};

	B_SHOT_t b_shot[BSHOT_MAX];

public:
	BOSS(double x, double y, int first, int second, int third, int fourth, int fifth);
	void CtrlPhase();
	void ChangePhase(int ptrn);
	void ChangeHP(double psatk);
	void Draw();
	void MoveAll();
	void Move0(double dest_x, double dest_y);
	void Move1();
	void Move2();
	void Move3();
	void ShotAll();
	void SetBs(double x, double y, double spd, double angle, int img, int ptrn = 0);
	void NWay(int way, int nshot, double spd, int img);
	void Shot0(int num, double raise, double spd);
	void Shot1(int col, double spd);
	void Shot2(int num, double spd);
	void Shot3(int d, double raise);
	void Shot4(int d, int num);
	void Shot5(int col, int odd, int d);
	void Shot6(int d);
	void SetSAngle(double angle, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void AddSAngle(double angle, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void SetSSpd(double spd, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void DeadEf();
	bool Wait(int frame);
	void IniS();
	bool GetPosition(double *x, double *y);
	bool GetShotPosition(int i, double *x, double *y, double *range = 0);
	void ChangeShotFlag(int i, bool flag);
	void ChangeDeadFlag();
};

#endif