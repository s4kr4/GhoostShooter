#include "pch.h"
#include "player.h"
#include "control.h"

PLAYER::PLAYER(){
	x = P_INIX;
	y = P_INIY;
	level = 1;
	life = 3;
	pwr = 1;
	bom = 1;
	score = 0;
	flag = true;
	d_flag = false;
	end_flag = false;
	b_flag = false;
	cntr = 0;
	b_cntr = 0;
	p_kill_num = 0;
	b_kill_num = 0;
	
	//自機画像
	LoadDivGraph("img/player.png", 9, 3, 3, 128, 128, img);
	LoadDivGraph("img/thunder.png", 2, 2, 1, 150, 150, thunder_img);
	l_img = LoadGraph("img/l_speed.png");

	//自機ショット画像
	s_img[0] = LoadGraph("img/p_shot3.png");
	s_img[1] = LoadGraph("img/p_shot1.png");

	//ボム画像
	b_img[0] = LoadGraph("img/bom0.png");
	b_img[1] = LoadGraph("img/bom1.png");

	//自機ショット音
	s_snd[0] = LoadSoundMem("sound/p_shot0.wav");
	s_snd[1] = LoadSoundMem("sound/p_shot1.wav");
	d_snd = LoadSoundMem("sound/dead.mp3");

	//自機ショット初期化
	for(int i=0; i<PSHOT_MAX; i++){
		p_shot[i].x = 0;
		p_shot[i].y = 0;
		p_shot[i].spd = 0;
		p_shot[i].angle = 0;
		p_shot[i].range = 0;
		p_shot[i].type = 0;
		p_shot[i].atk = 0;
		p_shot[i].flag = false;
	}
}

void PLAYER::Parameter(){
	//敵を3体倒すごとに1パワーアップ(MAX:10)、10体倒すごとに1ボムゲット(MAX:3)
	if(pwr < 10 && p_kill_num%3 == 0 && p_kill_num != 0){
		pwr++;
		p_kill_num = 0;
	}
	if(bom < 3 && b_kill_num%10 == 0 && b_kill_num != 0){
		bom++;
		b_kill_num = 0;
	}

	if(pwr == 10){
		level = 3;
	}else if(pwr >= 5){
		level = 2;
	}else if(pwr >= 1){
		level = 1;
	}

	//死んでから80フレーム後に復帰
	if(d_flag){
		if(cntr == 80){
			x = P_INIX;
			y = P_INIY;
			flag = true;
			p_kill_num = b_kill_num = 0;
		}

		if(cntr == 250)	d_flag = false;
	}
}

void PLAYER::Move(){
	KEY &key = KEY::GetInstance();

	if(flag){
		bool right = false, left = false, down = false, up = false;
			
		if(key.right) right = true;
		if(key.left) left = true;
		if(key.down) down = true;
		if(key.up) up = true;

		//低速キーで速度変更
		if(key.slow){
			spd = P_SPD_L;
		}else{
			spd = P_SPD;
		}
		//斜め移動の速度調整
		if((right || left) && (down || up)) spd /= 1.41f;

		if(right && x < F_HMAX-15) x += spd;
		if(left && x > F_HMIN+15) x -= spd;
		if(down && y < F_VMAX-30) y += spd;
		if(up && y > F_VMIN+40) y -= spd;
	}
	cntr++;
}

void PLAYER::Shot(){
	KEY &key = KEY::GetInstance();

	if(flag){
		//5フレームごとに弾発射
		if(s_cntr%5 == 0 && key.shot){
			switch(level){
			case 3:
				SetPs(x+30, y, 15, -85*PI/180, 4, 1, s_img[1]);
				SetPs(x-30, y, 15, -95*PI/180, 4, 1, s_img[1]);
			case 2:
				SetPs(x+15, y+15, 18, -88*PI/180, 8, 2, s_img[0]);
				SetPs(x-15, y+15, 18, -92*PI/180, 8, 2, s_img[0]);
			case 1:
				SetPs(x+9, y+5, 18, -90*PI/180, 8, 2, s_img[0]);
				SetPs(x-9, y+5, 18, -90*PI/180, 8, 2, s_img[0]);
				break;
			}
			PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
		}

		if(bom != 0 && key.bom == 1){
			b_flag = true;
			bom--;
			srand((unsigned int)time(NULL));
		}

		//ボム
		if(b_flag){
			SetPs(x, y, 20, (-60-rand()%60)*PI/180, 20, 3, b_img[0]);
			SetPs(x, y, 20, (-60-rand()%60)*PI/180, 20, 3, b_img[0]);
			SetPs(x, y, 20, (-45-rand()%90)*PI/180, 20, 3, b_img[0]);
			if(b_cntr%5 == 0) PlaySoundMem(s_snd[1], DX_PLAYTYPE_BACK);

			b_cntr++;
			if(b_cntr == 250){
				b_flag = false;
				b_cntr = 0;
			}
		}
	}

	//弾を進める
	for(int i=0; i<PSHOT_MAX; i++){
		if(p_shot[i].flag){
			p_shot[i].x += p_shot[i].spd*cos(p_shot[i].angle);
			p_shot[i].y += p_shot[i].spd*sin(p_shot[i].angle);
			//画面外処理
			if(p_shot[i].x < F_HMIN-10 || p_shot[i].x > F_HMAX+10 || 
				p_shot[i].y < F_VMIN-10 || p_shot[i].y > F_VMAX+10){
				p_shot[i].flag = false;
			}
		}
	}
	s_cntr++;
}

void PLAYER::Draw(){
	KEY &key = KEY::GetInstance();
	//自機ショット描画
	for(int i=0; i<PSHOT_MAX; i++){
		if(p_shot[i].flag){
				DrawRotaGraphF((float)p_shot[i].x, (float)p_shot[i].y, 1.0, p_shot[i].angle+(90*PI/180), p_shot[i].img, TRUE);
		}
	}

	//自機描画
	if(flag){
		//どの画像を表示するか
		int img_n;

		if(key.right){
			img_n = cntr%30/10+3;
		}else if(key.left){
			img_n = cntr%30/10+6;
		}else{
			img_n = cntr%30/10;
		}

		//死んだかボムかで点滅
		if(d_flag || b_flag) if(cntr%10 == 0) SetDrawBright(0, 0, 0);

		//通常時
		DrawRotaGraphF((float)x, (float)y, 0.45, 0.0, img[img_n], TRUE);
		DrawRotaGraphF((float)x, (float)y, 0.45, 0.0, thunder_img[cntr%14/7], TRUE);
		SetDrawBright(255, 255, 255);

		//低速移動中
		if(key.slow){
			//当たり判定の◎表示
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, l_img, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
		}
		//パワーオプション
		for(int i=0; i<pwr; i++){
			DrawBox((int)x+18, (int)y+19-(i*4), (int)x+22, (int)y+22-(i*4), GetColor(255, 0, 0), TRUE);
		}
		//ボムオプション
		for(int i=0; i<bom; i++){
			DrawBox((int)x-22, (int)y+10-(i*13), (int)x-18, (int)y+22-(i*13), GetColor(0, 255, 0), TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}else{
		//死亡エフェクト
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(255-0.159375*cntr*cntr));
		DrawRotaGraphF((float)x, (float)y, cntr*0.2+0.5, 0.0, img[0], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void PLAYER::SetPs(double x, double y, double spd, double angle, double range, double atk, int img){
	for(int i=0; i<PSHOT_MAX; i++){
		if(!p_shot[i].flag){
			p_shot[i].flag = true;
			p_shot[i].spd = spd;
			p_shot[i].x = x;
			p_shot[i].y = y;
			p_shot[i].angle = angle;
			p_shot[i].range = range;
			p_shot[i].atk = atk;
			p_shot[i].img = img;
			break;
		}
	}
}

//自機位置を返す
bool PLAYER::GetPosition(double *x, double *y){
	if(this->flag){
		*x = this->x;
		*y = this->y;
		return true;
	}else{
		return false;
	}
}

//自機情報を返す
void PLAYER::GetInfo(int *level, int *life, int *score){
	*level = this->level;
	*life = this->life;
	*score = this->score;
}

//自機ショット情報を返す
bool PLAYER::GetShotInfo(int i, double *x, double *y, double *atk, double *range){
	if(p_shot[i].flag){
		*x = p_shot[i].x;
		*y = p_shot[i].y;
		*atk = p_shot[i].atk;
		*range = p_shot[i].range;
		return true;
	}else{
		return false;
	}
}

//自機フラグの操作(第一引数0:フラグを返す、1:フラグを変更して返す)
bool PLAYER::CtrlFlag(int num, bool flag){
	switch(num){
	case 0:
		break;
	case 1:
		if(!flag){
			cntr = 0;
			PlaySoundMem(d_snd, DX_PLAYTYPE_BACK);
		}
		this->flag = flag;
		break;
	}
	return this->flag;
}

//自機ダメージフラグの操作(第一引数0:フラグを返す、1:フラグを変更して返す)
bool PLAYER::CtrlDmgFlag(int num, bool flag){
	switch(num){
	case 0:
		break;
	case 1:
		return this->d_flag = flag;
		break;
	}
	return this->d_flag;
}

bool PLAYER::GetBomFlag(){
	return b_flag;
}

//ダメージカウンターを返す
int PLAYER::GetCntr(){
	return this->cntr;
}

//エンドフラグをチェック
bool PLAYER::CheckEndFlag(){
	if(this->life < 0){
		this->end_flag = true;
	}
	return end_flag;
}

//自機ショットフラグ変更
void PLAYER::ChangeShotFlag(int i, bool flag){
	this->p_shot[i].flag = flag;
}

//倒した数をかぞえる
void PLAYER::ChangeKillNum(int num){
	this->p_kill_num += num;
	this->b_kill_num += num;
}

//パワー計算
void PLAYER::ChangePwr(int pwr){
	this->pwr += pwr;
	if(this->pwr < 1) this->pwr = 1;
}

//スコア計算
void PLAYER::ChangeScore(int score){
	this->score += score;
}

//残機計算
void PLAYER::ChangeLife(int life){
	this->life += life;
}
