#pragma warning(disable : 4996)
#include "pch.h"
#include "control.h"

CONTROL::CONTROL(){
	stage_n = 1;
	back = new BACK;
}

CONTROL::~CONTROL(){
	delete player;

	for(int i=0; i<E_MAX; i++){
		if(enemy[i] != NULL){
			delete enemy[i];
		}
	}
	delete boss;
	delete key;
}

//ステージ初期化
void CONTROL::StageIni(){
	game_cntr = 0;
	e_num = e_max = 0;
	pause_flag = false;
	clear_flag = false;

	player = new PLAYER;

	struct ENEMYDATA{
		int type;
		int s_type;
		int pattern;
		int s_pattern;
		int nshot;
		int interval;
		int in_time;
		int stop_time;
		int shot_time;
		int out_time;
		double x;
		double y;
		double speed;
		double s_speed;
		int hp;
		int item;
	};

	struct BOSSDATA{
		double x;
		double y;
		int first;
		int second;
		int third;
		int fourth;
		int fifth;
	};

	FILE *e_fp;
	FILE *b_fp;
	ENEMYDATA e_data[E_MAX];
	BOSSDATA b_data;
	char e_buf[100];
	char b_buf[100];
	int c;

	memset(e_buf, 0, sizeof(e_buf));
	memset(b_buf, 0, sizeof(b_buf));

	//敵データ、ボスデータ読み込み
	switch(stage_n){
	case 1:
		back->Stage1_Ini();
		if(error = fopen_s(&e_fp, "edata/enemy.csv", "r") != 0){
			MessageBox(NULL, "敵データの読み込みに失敗しました", "警告", 0);
		}
		if(error = fopen_s(&b_fp, "edata/bossdata.csv", "r") != 0){
			MessageBox(NULL, "ボスデータの読み込みに失敗しました", "警告", 0);
		}
		break;
	case 2:
		break;
	case 3:
		break;
	}

//--------------------------------敵データ読み込み--------------------------------
	int col = 1;
	int row = 0;

	//一行目飛ばす
	while(fgetc(e_fp) != '\n');
	while(row<E_MAX){
		//文字判定(改行orカンマorEOFならブレイク、それ以外なら文字連結>>e_buf)
		while(1){
			c = fgetc(e_fp);
			if(c == ',' || c == '\n' || c == EOF){
				break;
			}else{
				strcat(e_buf, (const char*)&c);
			}
		}
		//EOFなら読み込み終了
		if(c == EOF) break;

		//列によって数値(e_buf)を代入、"u"の場合は前のデータを引き継ぐ
		if(e_buf[0] == 'u'){
			switch(col){
			case 1: e_data[row].type = e_data[row-1].type; break;
			case 2: e_data[row].s_type = e_data[row-1].s_type; break;
			case 3: e_data[row].pattern = e_data[row-1].pattern; break;
			case 4: e_data[row].s_pattern = e_data[row-1].s_pattern; break;
			case 5: e_data[row].nshot = e_data[row-1].nshot; break;
			case 6: e_data[row].interval = e_data[row-1].interval; break;
			case 7: e_data[row].in_time = e_data[row-1].in_time; break;
			case 8: e_data[row].stop_time = e_data[row-1].stop_time; break;
			case 9: e_data[row].shot_time = e_data[row-1].shot_time; break;
			case 10: e_data[row].out_time = e_data[row-1].out_time; break;
			case 11: e_data[row].x = e_data[row-1].x; break;
			case 12: e_data[row].y = e_data[row-1].y; break;
			case 13: e_data[row].speed = e_data[row-1].speed; break;
			case 14: e_data[row].s_speed = e_data[row-1].s_speed; break;
			case 15: e_data[row].hp = e_data[row-1].hp; break;
			case 16: e_data[row].item = e_data[row-1].item; break;
			}
		}else{
			switch(col){
			case 1: e_data[row].type = atoi(e_buf); break;
			case 2: e_data[row].s_type = atoi(e_buf); break;
			case 3: e_data[row].pattern = atoi(e_buf); break;
			case 4: e_data[row].s_pattern = atoi(e_buf); break;
			case 5: e_data[row].nshot = atoi(e_buf); break;
			case 6: e_data[row].interval = int(atof(e_buf)*60); break;
			case 7: e_data[row].in_time = int(atof(e_buf)*60); break;
			case 8: e_data[row].stop_time = int(atof(e_buf)*60); break;
			case 9: e_data[row].shot_time = int(atof(e_buf)*60); break;
			case 10: e_data[row].out_time = int(atof(e_buf)*60); break;
			case 11: e_data[row].x = atof(e_buf); break;
			case 12: e_data[row].y = atof(e_buf); break;
			case 13: e_data[row].speed = atof(e_buf); break;
			case 14: e_data[row].s_speed = atof(e_buf); break;
			case 15: e_data[row].hp = atoi(e_buf); break;
			case 16: e_data[row].item = atoi(e_buf); break;
			}
		}

		//バッファ初期化
		memset(e_buf, 0, sizeof(e_buf));
		col++;

		//行の最後まで来たら敵データ1体分作成し、次の行へ
		if(c == '\n'){
			enemy[row] = new ENEMY(e_data[row].type, e_data[row].s_type, e_data[row].pattern, e_data[row].s_pattern, e_data[row].nshot, e_data[row].interval,
				e_data[row].in_time, e_data[row].stop_time, e_data[row].shot_time, e_data[row].out_time, 
				e_data[row].x, e_data[row].y, e_data[row].speed, e_data[row].s_speed, e_data[row].hp, e_data[row].item);
			e_max++;
			col = 1;
			row++;
		}
	}

	fclose(e_fp);

//--------------------------------ボスデータ読み込み--------------------------------
	col = 1;
	row = 0;

	//一行目飛ばす
	while(fgetc(b_fp) != '\n');
	//ステージ番号に対応した行まで飛ばす
	for(row; row<stage_n-1; row++){
		while(fgetc(b_fp) != '\n');
	}
	while(1){
		//1セル分読み込み
		while(1){
			c = fgetc(b_fp);

			if(c != ',' && c != '\n'){
				strcat(b_buf, (const char*)&c);
			}else{
				break;
			}
		}
	
		//列によって数値を代入
		switch(col){
		case 1: b_data.x = double(atoi(b_buf)); break;
		case 2: b_data.y = double(atoi(b_buf)); break;
		case 3: b_data.first = atoi(b_buf); break;
		case 4: b_data.second = atoi(b_buf); break;
		case 5: b_data.third = atoi(b_buf); break;
		case 6: b_data.fourth = atoi(b_buf); break;
		case 7:
			b_data.fifth = atoi(b_buf);
			boss = new BOSS(b_data.x, b_data.y, b_data.first, b_data.second, b_data.third, b_data.fourth, b_data.fifth);
			break;
		}
		if(c == '\n') break;

		//バッファ初期化
		memset(b_buf, 0, sizeof(b_buf));
		col++;
	}

	fclose(b_fp);
}

//計算処理
void CONTROL::Calc(){
	KEY &key = KEY::GetInstance();

	if(key.esc == 1){
		pause_flag = true;
	}

	//背景
	back->Move();

	//自機
	player->Parameter();
	player->Move();
	player->Shot();

	//敵
	if(CtrlEnum(0) < e_max){
		for(int i=0;i<e_max; i++){
			if(enemy[i] != NULL){
				enemy[i]->MoveAll();
				enemy[i]->ShotAll();
			}
		}
		CollisionAll_E();
	}else{
		boss->CtrlPhase();
		CollisionAll_B();
	}

	game_cntr++;
}

//描画処理
void CONTROL::Draw(){
	//フィールド
	back->DrawField();
	
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//自機
	player->Draw();

	//敵
	if(CtrlEnum(0) < e_max){
		for(int i=0; i<e_max; i++){
			if(enemy[i] != NULL && !enemy[i]->Draw()){
				delete enemy[i];
				enemy[i] = NULL;
			}
		}
	}else{
		boss->Draw();
	}
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//周囲の背景
	back->DrawSurround();
}

//スタート画面
void CONTROL::Start(){
	back->Title();
}

//メニュー画面
void CONTROL::Menu(){
	back->Menu();
}

//ゲーム画面
void CONTROL::Game(){
	if(!pause_flag){
		this->Calc();
		this->Draw();
		//ライフが無くなったらゲームオーバー画面へ
		if(player->CheckEndFlag()) func_state = 3;
	}else{
		this->Draw();
		back->Pause();
	}
}

//ゲームオーバー画面
void CONTROL::GameEnd(){
	SaveData();
	if(clear_flag){
		back->GameClear();
	}else{
		back->GameOver();
	}
}

//e_numを増加して値を返す
int CONTROL::CtrlEnum(int num){
	return e_num += num;
}

//ゲームカウンターを返す
int CONTROL::GetGameCntr(){
	return game_cntr;
}

//ポーズフラグ変更
bool CONTROL::CtrlPauseFlag(bool flag){
	return pause_flag = flag;
}

//クリアフラグ変更
bool CONTROL::CtrlClearFlag(bool flag){
	func_state = 3;
	return clear_flag = flag;
}

//自機位置を他クラスに返す
void CONTROL::GetPlayerPosition(double *x, double *y){
	double px, py;
	player->GetPosition(&px, &py);

	*x = px;
	*y = py;
}

//自機情報を他クラスに返す
void CONTROL::GetPlayerInfo(int *level, int *life, int *score){
	int p_level, p_life, p_score;
	player->GetInfo(&p_level, &p_life, &p_score);

	*level = p_level;
	*life = p_life;
	*score = p_score;
}

//敵位置を他クラスに返す
bool CONTROL::GetEnemyPosition(int i, double *x, double *y){
	double ex, ey;
	
	if(enemy[i]->GetPosition(&ex, &ey)){
		*x = ex;
		*y = ey;
		return true;
	}else{
		return false;
	}
}

//敵ショット位置を他クラスに返す
void CONTROL::GetEnemyShotPosition(int i, int j, double *x, double *y){
	double esx, esy;

	enemy[i]->GetShotPosition(j, &esx, &esy);

	*x = esx;
	*y = esy;
}

//当たり判定
bool CONTROL::CollisionDetection(double range1, double range2, double x1, double y1, double x2, double y2){
	if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2) <= (range1+range2)*(range1+range2)){
		return true;
	}else{
		return false;
	}
}

//道中当たり判定
void CONTROL::CollisionAll_E(){
	//自機・敵の座標
	double px, py, ex, ey;
	//自機・敵のショット座標
	double psx, psy, esx, esy;
	//ショットの大きさ
	double psr, esr;
	//自機ショットの威力
	double psatk;

	//自弾と敵
	for(int i=0; i<e_max; i++){
		if(enemy[i] != NULL && enemy[i]->GetPosition(&ex, &ey)){
			for(int j=0; j<PSHOT_MAX; j++){
				if(player->GetShotInfo(j, &psx, &psy, &psatk, &psr)){
					if(CollisionDetection(psr, E_RANGE0, psx, psy, ex, ey)){
						player->ChangeShotFlag(j, false);
						player->ChangeScore(100);
						enemy[i]->ChangeHP(-psatk);
						if(!enemy[i]->CtrlFlag(1)){
							player->ChangeKillNum(1);
						}
					}
				}
			}
		}
	}

	//敵弾と自機
	if(player->GetPosition(&px, &py) && !player->GetBomFlag() && !player->CtrlDmgFlag(0)){
		for(int i=0; i<e_max; i++){
			if(enemy[i] != NULL){
				for(int j=0; j<ESHOT_MAX; j++){
					if(enemy[i]->GetShotPosition(j, &esx, &esy, &esr)){
						if(CollisionDetection(esr, P_RANGE1, esx, esy, px, py)){
							player->CtrlFlag(1, false);
							player->CtrlDmgFlag(1, true);
							player->ChangePwr(-3);
							player->ChangeLife(-1);
							enemy[i]->ChangeShotFlag(j, false);
						}
					}
				}
			}
		}
	}

	//自機と敵
	if(player->GetPosition(&px, &py) && !player->GetBomFlag() && !player->CtrlDmgFlag(0)){
		for(int i=0; i<e_max; i++){
			if(enemy[i] != NULL && enemy[i]->GetPosition(&ex, &ey)){
				if(CollisionDetection(P_RANGE1, E_RANGE0, px, py, ex, ey)){
					player->CtrlFlag(1, false);
					player->CtrlDmgFlag(1, true);
					player->ChangePwr(-3);
					player->ChangeLife(-1);
				}
			}
		}
	}

	//敵弾にヒット後、40フレームで弾消し
	if(player->CtrlDmgFlag(0) && player->GetCntr() == 40){
		for(int i=0; i<e_max; i++){
			if(enemy[i] != NULL && enemy[i]->CtrlFlag(0)){
				enemy[i]->ChangeClassFlag(false);
			}
		}
	}
}

//ボス当たり判定
void CONTROL::CollisionAll_B(){
	//自機・ボスの座標
	double px, py, bx, by;
	//自機・ボスのショット座標
	double psx, psy, bsx, bsy;
	//ショットの大きさ
	double psr, bsr;
	//自機ショットの威力
	double psatk;

	//自弾とボス
	if(boss->GetPosition(&bx, &by)){
		for(int i=0; i<PSHOT_MAX; i++){
			if(player->GetShotInfo(i, &psx, &psy, &psatk, &psr)){
				if(CollisionDetection(psr, B_RANGE0, psx, psy, bx, by)){
					player->ChangeShotFlag(i, false);
					player->ChangeScore(200);
					boss->ChangeHP(psatk);
				}
			}
		}
	}
	//ボス弾と自機
	if(player->GetPosition(&px, &py) && !player->GetBomFlag() && !player->CtrlDmgFlag(0)){
		for(int i=0; i<BSHOT_MAX; i++){
			if(boss->GetShotPosition(i, &bsx, &bsy, &bsr)){
				if(CollisionDetection(bsr, P_RANGE1, bsx, bsy, px, py)){
					player->CtrlFlag(1, false);
					player->CtrlDmgFlag(1, true);
					player->ChangePwr(-3);
					player->ChangeLife(-1);
				}
			}
		}
	}
	//自機とボス
	if(player->GetPosition(&px, &py) && !player->GetBomFlag() && !player->CtrlDmgFlag(0)){
		if(boss->GetPosition(&bx, &by)){
			if(CollisionDetection(P_RANGE1, B_RANGE0, px, py, bx, by)){
				player->CtrlFlag(1, false);
				player->CtrlDmgFlag(1, true);
				player->ChangePwr(-3);
				player->ChangeLife(-1);
			}
		}
	}

	if(player->CtrlDmgFlag(0) && player->GetCntr() == 40){
		for(int i=0; i<BSHOT_MAX; i++){
			boss->ChangeShotFlag(i, false);
		}
	}
}

//キー入力状態取得
void CONTROL::CheckKeyState(){
	key->GetKeyState();
}

//スコア書き出し
void CONTROL::SaveData(){
	int hiscore = 0;
	FILE *score_fp;
	if(error = fopen_s(&score_fp, "score.dat", "rb") != 0){
		MessageBox(NULL, "score.dat読込失敗", "警告", 0);
	}
	fread(&hiscore, sizeof(hiscore), 1, score_fp);
	fclose(score_fp);

	int p_level, p_life, p_score;
	player->GetInfo(&p_level, &p_life, &p_score);

	if(p_score > hiscore){
		if(error = fopen_s(&score_fp, "score.dat", "wb") != 0){
			MessageBox(NULL, "score.dat読込失敗", "警告", 0);
		}
		fwrite(&p_score, sizeof(p_score), 1, score_fp);
		fclose(score_fp);
	}
}

//スコア読み込み
int CONTROL::ReadData(){
	int hiscore;
	FILE *score_fp;
	if(error = fopen_s(&score_fp, "score.dat", "rb") != 0){
		MessageBox(NULL, "score.dat読込失敗", "警告", 0);
	}
	fread(&hiscore, sizeof(hiscore), 1, score_fp);
	fclose(score_fp);

	return hiscore;
}
