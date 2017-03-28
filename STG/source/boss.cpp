#include "pch.h"
#include "boss.h"
#include "control.h"

BOSS::BOSS(double x, double y, int first, int second, int third, int fourth, int fifth){
	this->x = x;
	this->y = y;
	spell[0] = first;
	spell[1] = second;
	spell[2] = third;
	spell[3] = fourth;
	spell[4] = fifth;
	hp = 1000;

	spell_n = 0;
	def = 0;
	px = py = 0;
	prev_x = prev_y = 0;
	move_x = move_y = 0;
	ptrn = 0;
	s_ptrn = 0;
	cntr = 0;
	w_cntr = 0;
	d_cntr = 0;
	flag = false;
	s_flag = false;
	shotsound_flag = false;
	m_flag = false;
	d_flag = false;
	angle = 0;

	s_angle = 0;
	s_raise = 0;
	s_cntr = 0;
	times = 0;
	s_num = 0;
	wait = false;

	for(int i=0; i<BSHOT_MAX; i++){
		b_shot[i].x = 0;
		b_shot[i].y = 0;
		b_shot[i].bx = 0;
		b_shot[i].by = 0;
		b_shot[i].spd = 0;
		b_shot[i].range = 0;
		b_shot[i].angle = 0;
		b_shot[i].ptrn = 0;
		b_shot[i].cntr = 0;
		b_shot[i].img = 0;
		b_shot[i].flag = false;
	}

	s_img[0] = LoadGraph("img/e_shot/red0.png");
	s_img[1] = LoadGraph("img/e_shot/green0.png");
	s_img[2] = LoadGraph("img/e_shot/blue0.png");
	s_img[3] = LoadGraph("img/e_shot/red1.png");
	s_img[4] = LoadGraph("img/e_shot/green1.png");
	s_img[5] = LoadGraph("img/e_shot/blue1.png");
	s_img[6] = LoadGraph("img/e_shot/red2.png");
	s_img[7] = LoadGraph("img/e_shot/green2.png");
	s_img[8] = LoadGraph("img/e_shot/blue2.png");
	s_img[9] = LoadGraph("img/e_shot/yellow2.png");
	s_img[10] = LoadGraph("img/e_shot/black0.png");

	s_snd[0] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[1] = LoadSoundMem("sound/e_shot1.mp3");
	s_snd[2] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[3] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[4] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[5] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[6] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[7] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[8] = LoadSoundMem("sound/e_shot0.mp3");
	s_snd[9] = LoadSoundMem("sound/e_shot0.mp3");

	s_range[0] = 10;
	s_range[1] = 5;
	s_range[2] = 0;
	s_range[3] = 0;
	s_range[4] = 0;
	s_range[5] = 0;
	s_range[6] = 0;
	s_range[7] = 0;
	s_range[8] = 0;
	s_range[9] = 0;
	//switch(stage_num){
	//case 1:
		img = LoadGraph("img/boss0.png");
	//	break;
	//}

	LoadDivGraph("img/burn.bmp", 16, 8, 2, 80, 80, d_img);
}

//状態管理
void BOSS::CtrlPhase(){
	CONTROL &control = CONTROL::GetInstance();

	if(cntr == 0){
		flag = m_flag = s_flag = true;
	}

	if(flag && !d_flag){
		if(m_flag) MoveAll();
		if(s_flag) ShotAll();
	}else if(!flag && !d_flag){
		control.CtrlClearFlag(true);
	}

	cntr++;
}

//動き方を変える
void BOSS::ChangePhase(int ptrn){
	this->ptrn = ptrn;
	IniS();

	for(int i=0; i<BSHOT_MAX; i++){
		ChangeShotFlag(i, false);
	}
}

//HP計算
void BOSS::ChangeHP(double psatk){
	hp -= (psatk - def);

	//HP0で弾幕5つ終わったらd_flag立てる
	//終わってなければ次の弾幕へ
	if(hp <= 0){
		spell_n++;
		if(spell_n == 5){
			flag = false;
			d_flag = true;
		}else{
			ChangePhase(spell[spell_n]);
			hp = 1000;
		}
	}
}

//描画
void BOSS::Draw(){
	if(flag) DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img, TRUE);

	for(int i=0; i<BSHOT_MAX; i++){
		if(b_shot[i].flag){
			DrawRotaGraphF((float)b_shot[i].x, (float)b_shot[i].y, 1.0, 0.0, b_shot[i].img, TRUE);
		}
	}

	//HPバー
	DrawBox(30, 30, int(30+400*hp/1000), 38, GetColor(255, 0, 0), TRUE);
	
	if(d_flag) DeadEf();
}

void BOSS::MoveAll(){
	switch(ptrn){
	case 0:
		Move0(230, 100);
		break;
	case 1:
		Move1();
		break;
	case 2:
		Move2();
		break;
	case 3:
		Move3();
		break;
	}
}

//指定した座標に移動する
void BOSS::Move0(double dest_x, double dest_y){
	if(angle == 0){
		//直前の座標
		prev_x = x;
		prev_y = y;

		//目的地への距離
		move_x = dest_x-x;
		move_y = dest_y-y;
	}
	angle += 2;

	x = prev_x + move_x*sin(angle*PI/180);
	y = prev_y + move_y*sin(angle*PI/180);

	//目的地まで移動したらショット
	if(angle == 90){
		angle = 0;
		s_flag = true;
	}
}

//その場で漂う
void BOSS::Move1(){
	angle += 2;
	y += sin(angle*PI/180);
}

void BOSS::Move2(){
}

void BOSS::Move3(){
}

//----------------------------------------------↓ボス弾幕↓---------------------------------------------------
void BOSS::ShotAll(){
	switch(ptrn){
	case 0:
		Shot0(100, 23.0, 2);
		break;
	case 1:
		Shot1(10, 2);
		break;
	case 2:
		Shot2(50, 6);
		break;
	case 3:
		Shot3(32, 1.39f);
		break;
	case 4:
		Shot4(32, 6);
		break;
	case 5:
		Shot5(6, 2, 15);
		break;
	case 6:
		Shot6(18);
		break;
	}

	//弾を進める
	if(flag){
		for(int i=0; i<BSHOT_MAX; i++){
			if(b_shot[i].flag){
				b_shot[i].x += b_shot[i].spd*cos(b_shot[i].angle*PI/180);
				b_shot[i].y += b_shot[i].spd*sin(b_shot[i].angle*PI/180);
				b_shot[i].cntr++;

				//画面外処理
				if(b_shot[i].x < F_HMIN-20 || b_shot[i].x > F_HMAX+20 ||
					b_shot[i].y < F_VMIN-20 || b_shot[i].y > F_VMAX+20){
					b_shot[i].flag = false;
				}
			}
		}
	}
	s_cntr++;
}

//ショットの情報をセット
void BOSS::SetBs(double x, double y, double spd, double angle, int img, int ptrn){
	for(int i=0; i<BSHOT_MAX; i++){
		if(!b_shot[i].flag){
			b_shot[i].x = x;
			b_shot[i].y = y;
			b_shot[i].spd = spd;
			b_shot[i].angle = angle;
			b_shot[i].range = s_range[img];
			b_shot[i].img = s_img[img];
			b_shot[i].ptrn = ptrn;
			b_shot[i].cntr = 0;
			b_shot[i].flag = true;
			break;
		}
	}
}

void BOSS::NWay(int way, int nshot, double spd, int img){
	for(int i=0; i<nshot; i++){
		for(int j=0; j<way; j++){
			SetBs(x, y, spd-((spd/2/nshot)*i), 10*(way-1)/2+atan2(py-y, px-x)*180/PI-(j*10), img);
		}
	}
}

//渦巻ショット(num = 弾数, raise = 弾の角度, spd = スピード)
void BOSS::Shot0(int num, double raise, double spd){
	static double angle = 0;
	if(s_num < num){
		if(s_cntr%3){
			SetBs(x+30*cos(angle/180*PI), y+30*sin(angle/180*PI), spd, s_angle, 1);
			SetBs(x+30*cos(angle/180*PI), y+30*sin(angle/180*PI), spd, s_angle+180, 2);
			s_angle += raise;
			s_num++;
			angle += 3;
		}
		if(s_cntr%8 == 0) PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);		
	}else{
		if(!Wait(50)) IniS();
	}
}

//曲がる列ショット(col = 列数, spd = スピード)
void BOSS::Shot1(int col, double spd){
	//弾の横間隔
	double space = double(350/(col-1));

	SetSAngle(s_cntr%180<90 ? s_cntr%90+45:135-s_cntr%90);

	if(s_cntr%35 == 0){
		PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
		for(col; col>=0; col--){
			SetBs(55+space*(col-1), y, spd, 90, 1);
		}
	}
}

//ばら撒き後、下に落ちる(num = 弾数, spd = 最高スピード)
void BOSS::Shot2(int num, double spd){
	SetSAngle(90, 60);
	SetSSpd(1, 60);

	if(s_cntr == 0) srand((unsigned int)time(NULL));

	if(s_num < num){
		if(s_cntr%3 == 0) PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
		
		if(s_cntr%3 == 0) SetBs(x, y, 3+double(rand()%((int)spd-3)), double(rand()%360), 0);
		if(s_cntr%3 == 1) SetBs(x, y, 3+double(rand()%((int)spd-3)), double(rand()%360), 1);
		if(s_cntr%3 == 2) SetBs(x, y, 3+double(rand()%((int)spd-3)), double(rand()%360), 2);

		s_num++;
	}else{
		if(!Wait(100)) IniS();
	}
}

//布都っぽい弾幕(d = n方向, raise = 弾をずらす倍率)
void BOSS::Shot3(int d, double raise){
	SetSSpd(0, 0, 60);
	SetSSpd(1, 0, 120);

	const double s_angle = 360.0/(d-1);

	if(s_cntr == 0)	s_raise = 1;

	if(times < 10){
		if(s_cntr%7 == 0){
			PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
			for(int i=0; i<d; i++){
				SetBs(x, y, 2.5, i*s_angle+s_raise, 1);
			}
			s_raise *= raise;
			times++;
		}
	}else{
		if(!Wait(200)) IniS();
		s_raise = 1;
	}
}

//交差弾
void BOSS::Shot4(int d, int num){
	SetSSpd(0, num*10+30);
	AddSAngle(40, num*10+30, 0, 1);
	AddSAngle(-40, num*10+30, 0, 2);
	SetSSpd(2, num*10+80);

	const double s_angle =  360.0/(d-1);
	if(s_cntr == 0)	s_num = 0;
	
	if(s_num < num){
		if(s_cntr >= 30 && s_cntr%10 == 0){
			PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
				for(int i=0; i<d; i++){
					SetBs(x+40*cos(i*s_angle/180*PI), y+40*sin(i*s_angle/180*PI), 3, i*s_angle, 1, 1);
				}
				for(int i=0; i<d; i++){
					SetBs(x+40*cos(i*s_angle/180*PI), y+40*sin(i*s_angle/180*PI), 3, i*s_angle, 2, 2);
				}
			s_num++;
		}
	}else{
		if(!Wait(150)) IniS();
	}
}

//col = 列数, odd = 自機狙いの列数, d = 固定n方位
void BOSS::Shot5(int col, int odd, int d){
	CONTROL &control = CONTROL::GetInstance();
	double px, py;
	double space = double(380/(col-1));
	static int tmp[10];

	SetSSpd(0, 0, 35, 1);
	SetSSpd(0, 0, 30, 2);

	control.GetPlayerPosition(&px, &py);

	if(s_cntr == 1){
		srand((unsigned int)time(NULL));
		s_angle =  360.0/(d-1);
	}

	//最初に出す弾
	if(s_cntr == 30){
		SetBs(40, 10, 6, 90, 0, 1);
		SetBs(420, 10, 6, 90, 0, 1);
		for(col; col>2; col--){
			SetBs(40+space*(col-2), 10, 6, 90, 0, 2);
		}
	}

	//n方位
	if(s_cntr >= 100 && s_cntr%50 == 0){
		for(int i=0; i<d; i++){
			SetBs(40, 220, 1, i*s_angle, 2);
			SetBs(420, 220, 1, i*s_angle, 2);
		}
	}

	//自機狙い
	if(s_cntr >= 200 && s_cntr%25 == 0){
		if(s_cntr == 200){
			for(int i=0; i<odd; i++){
				tmp[i] = rand()%(col-2)+1;
			}
		}
		for(col; col>0; col--){
			for(int i=0; i<odd; i++){
				if(col == tmp[i]){
					SetBs(40+space*col, 190, 3, atan2(py-190, px-(40+space*col))*180/PI, 0);
				}
			}
		}
	}

	if(s_cntr == 700){
		IniS();
		for(int i=0; i<BSHOT_MAX; i++){
			ChangeShotFlag(i, false);
		}
	}
}

void BOSS::Shot6(int d){
	SetSSpd(0, 50);
	if(s_cntr == 50){
		for(int i=0; i<BSHOT_MAX; i++){
			b_shot[i].img = s_img[10];
		}
	}
	SetSSpd(2, 100);
	if(s_cntr == 100){
		for(int i=0; i<BSHOT_MAX; i++){
			b_shot[i].img = s_img[1];
		}
		IniS();
	}

	const double s_angle = 180.0/(d-1);

	if(s_cntr%10 == 0 && s_num < 3){
		PlaySoundMem(s_snd[0], DX_PLAYTYPE_BACK);
		for(int i=0; i<d; i++){
			SetBs(x+40*cos(i*s_angle/180*PI), y+40*sin(i*s_angle/180*PI), 2, i*s_angle, 1);
		}
		s_num++;
	}
}

void BOSS::SetSAngle(double angle, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && b_shot[i].cntr == ss_cntr){
					b_shot[i].angle = angle;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].cntr == ss_cntr){
					b_shot[i].angle = angle;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					b_shot[i].angle = angle;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && this->s_cntr == s_cntr){
					b_shot[i].angle = angle;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
		for(int i=0; i<BSHOT_MAX; i++){
			if(b_shot[i].flag && b_shot[i].ptrn == ptrn){
				b_shot[i].angle = angle;
			}
		}
	}
}

void BOSS::AddSAngle(double angle, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && b_shot[i].cntr == ss_cntr){
					b_shot[i].angle += angle;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].cntr == ss_cntr){
					b_shot[i].angle += angle;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					b_shot[i].angle += angle;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && this->s_cntr == s_cntr){
					b_shot[i].angle += angle;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
		for(int i=0; i<BSHOT_MAX; i++){
			if(b_shot[i].flag && b_shot[i].ptrn == ptrn){
				b_shot[i].angle += angle;
			}
		}
	}
}

void BOSS::SetSSpd(double spd, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && b_shot[i].cntr == ss_cntr){
					b_shot[i].spd = spd;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].cntr == ss_cntr){
					b_shot[i].spd = spd;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && b_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					b_shot[i].spd = spd;
				}
			}
		}else{
			for(int i=0; i<BSHOT_MAX; i++){
				if(b_shot[i].flag && this->s_cntr == s_cntr){
					b_shot[i].spd = spd;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
			for(int i=0; i<BSHOT_MAX; i++){
			if(b_shot[i].flag && b_shot[i].ptrn == ptrn){
				b_shot[i].spd = spd;
			}
		}
	}
}

void BOSS::IniS(){
	s_num = 0;
	s_angle = 0;
	times = 0;
	s_cntr = 0;
	wait = false;
}

//----------------------------------------------↑ボス弾幕↑---------------------------------------------------

//死亡エフェクト
void BOSS::DeadEf(){
	static int i = 5;
	if(i == 5) srand((unsigned int)time(NULL));

	DrawRotaGraph(int((x-50)+rand()%100), int((y-30)+rand()%100), 1.0, 0.0, d_img[d_cntr], TRUE);
	d_cntr++;

	if(d_cntr == 16){
		i--;
		d_cntr = 0;
	}
	if(i == 0) d_flag = false;
}

//指定したフレーム数待機
bool BOSS::Wait(int frame){
	this->wait = true;
	w_cntr++;

	if(w_cntr >= frame){
		this->wait = false;
		w_cntr = 0;
	}
	return wait;
}

//ボス座標を返す
bool BOSS::GetPosition(double *x, double *y){
	if(this->flag){
		*x = this->x;
		*y = this->y;
		return true;
	}else{
		return false;
	}
}

//ショット座標を返す
bool BOSS::GetShotPosition(int i, double *x, double *y, double *range){
	if(b_shot[i].flag){
		*x = b_shot[i].x;
		*y = b_shot[i].y;
		*range = b_shot[i].range;
		return true;
	}else{
		return false;
	}
}

//ボスショットの個別フラグを変更
void BOSS::ChangeShotFlag(int i, bool flag){
	b_shot[i].flag = flag;
}
