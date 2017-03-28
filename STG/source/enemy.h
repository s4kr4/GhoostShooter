#include "pch.h"
#include "define.h"
#include "mymath.h"

#ifndef _ENEMY
#define _ENEMY

class ENEMY{
private:
	//敵座標
	double x, y;
	//自機座標
	double px, py;
	//個体、クラス、移動、ショット、死亡のフラグ
	bool flag, c_flag, m_flag, s_flag, d_flag;
	int cntr, s_cntr, d_cntr;
	//出現、停止、ショット、帰還の時間
	int in_time, stop_time, shot_time, out_time;
	int img[4], s_img, b_img[16];
	int s_snd;
	double range, s_range;
	double angle;
	int type, s_type;
	int ptrn, s_ptrn;
	double spd, s_spd;	
	double hp;
	int item;
	//発射弾数
	int nshot;
	//発射間隔
	int interval;
	//どちら向きか
	int course;

	struct E_SHOT_t{
		double x, y;
		double ex, ey;
		double spd;
		double range;
		double angle;
		int ptrn;
		int cntr;
		int img;
		bool flag;
	};

	E_SHOT_t e_shot[ESHOT_MAX];

	MyMath *mymath;

public:
	ENEMY(int type, int s_type, int ptrn, int s_ptrn, int nshot, int interval, int in_time, int stop_time, int shot_time, int out_time,
		double x, double y, double spd, double s_spd, double hp, int item);
	bool Draw();
	void MoveAll();
	void Move0();
	void Move1();
	void Move2();
	void Move3();
	void Move4();
	void Move5();
	void Move6();
	void ShotAll();
	void SetEs(double x, double y, double spd, double angle, int img, int ptrn = 0);
	void NWay(int way);
	void Shot0();
	void Shot1();
	void Shot2();
	void Shot3();
	void Shot4();
	void Shot5();
	void Shot6();
	void Shot7();
	void SetSAngle(double angle, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void AddSAngle(double angle, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void SetSSpd(double spd, int s_cntr = 0, int ss_cntr = 0, int ptrn = 0);
	void DeadEf();
	bool GetPosition(double *x, double *y);
	bool GetShotPosition(int i, double *x, double *y, double *range = 0);
	bool CtrlFlag(int num, bool flag = false);
	void ChangeShotFlag(int i, bool flag);
	void ChangeClassFlag(bool flag);
	void ChangeHP(double psatk);
	bool InArea(double x, double y);
};

#endif