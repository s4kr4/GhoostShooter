#include "pch.h"
#include "define.h"
#include "back.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "key.h"

class CONTROL{
private:
	BACK *back;
	PLAYER *player;
	ENEMY *enemy[E_MAX];
	BOSS *boss;
	KEY *key;
	errno_t error;
	int stage_n;
	int e_num, e_max;
	int game_cntr;
	bool pause_flag;
	bool clear_flag;

private:
	CONTROL();
	~CONTROL();

public:
	void StageIni();
	void Calc();
	void Draw();
	void Start();
	void Menu();
	void Game();
	void GameEnd();
	int CtrlEnum(int num);
	int GetGameCntr();
	bool CtrlPauseFlag(bool flag);
	bool CtrlClearFlag(bool flag);
	void GetPlayerPosition(double *x, double *y);
	void GetPlayerInfo(int *level, int *life, int *score);
	bool GetEnemyPosition(int i, double *x, double *y);
	void GetEnemyShotPosition(int i, int j, double *x, double *y);
	bool CollisionDetection(double range1, double range2, double x1, double y1, double x2, double y2);
	void CollisionAll_E();
	void CollisionAll_B();
	void CheckKeyState();
	void SaveData();
	int ReadData();

	static CONTROL& GetInstance(){
		static CONTROL control;
		return control;
	}
};
