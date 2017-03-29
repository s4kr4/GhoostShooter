#include "pch.h"
#include "enemy.h"
#include "control.h"
#include "mymath.h"

ENEMY::ENEMY(int type, int s_type, int ptrn, int s_ptrn, int nshot, int interval,
	int in_time, int stop_time, int shot_time, int out_time, double x, double y, double spd, double s_spd, double hp, int item){
	this->type = type;
	this->s_type = s_type;
	this->ptrn = ptrn;
	this->s_ptrn = s_ptrn;
	this->nshot = nshot;
	this->interval = interval;
	this->in_time = in_time;
	this->stop_time = stop_time;
	this->shot_time = shot_time;
	this->out_time = out_time;
	this->x = x;
	this->y = y;
	this->spd = spd;
	this->s_spd = s_spd;
	this->hp = hp;
	this->item = item;

	px = py = 0;
	range = 0;
	angle = 0;
	flag = false;
	c_flag = true;
	m_flag = false;
	s_flag = false;
	d_flag = false;
	cntr = 0;
	s_cntr = 0;
	d_cntr = 0;

	//�����ʒu�����S���E�������œ�����ς���
	//course = 0�������� course = 1���E����
	if(x > P_INIX){
		course = 0;
	}else{
		course = 1;
	}

	//�G�摜�ǂݍ���
	switch(type){
	case 0:
		LoadDivGraph("img/enemy/enemy0.png", 4, 4, 1, 50, 50, img);
		break;
	case 1:
		img[0] = LoadGraph("img/enemy/enemy1.png");
		break;
	case 2:
		LoadDivGraph("img/enemy/enemy2.png", 2, 2, 1, 44, 44, img);
		break;
	case 3:
		LoadDivGraph("img/enemy/enemy3.png", 4, 4, 1, 64, 48, img);
		break;
	case 4:
		LoadDivGraph("img/enemy/enemy4.png", 4, 4, 1, 50, 50, img);
		break;
	}

	//�G�e�摜�E���ˉ��ǂݍ���
	switch(s_type){
	case 0:
		s_img = LoadGraph("img/e_shot/red0.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 6;
		break;
	case 1:
		s_img = LoadGraph("img/e_shot/green0.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 6;
		break;
	case 2:
		s_img = LoadGraph("img/e_shot/blue0.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 6;
		break;
	case 3:
		s_img = LoadGraph("img/e_shot/red1.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 3;
		break;
	case 4:
		s_img = LoadGraph("img/e_shot/green1.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 3;
		break;
	case 5:
		s_img = LoadGraph("img/e_shot/blue1.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 3;
		break;
	case 6:
		s_img = LoadGraph("img/e_shot/red2.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 2.5;
		break;
	case 7:
		s_img = LoadGraph("img/e_shot/green2.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 2.5;
		break;
	case 8:
		s_img = LoadGraph("img/e_shot/blue2.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 2.5;
		break;
	case 9:
		s_img = LoadGraph("img/e_shot/yellow2.png");
		s_snd = LoadSoundMem("sound/e_shot0.mp3");
		s_range = 2.5;
		break;
	}

	LoadDivGraph("img/burn.bmp", 16, 8, 2, 80, 80, b_img);

	//�G�e������
	for(int i=0; i<ESHOT_MAX; i++){
		e_shot[i].x = x;
		e_shot[i].y = y;
		e_shot[i].ex = 0;
		e_shot[i].ey = 0;
		e_shot[i].spd = s_spd;
		e_shot[i].range = s_range;
		e_shot[i].angle = 0;
		e_shot[i].ptrn = s_ptrn;
		e_shot[i].cntr = 0;
		e_shot[i].img = s_img;
		e_shot[i].flag = false;
	}

	mymath = new MyMath;
}

//�`��
bool ENEMY::Draw(){
	if(flag){
		switch(type){
		case 0:
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img[cntr%40/10], TRUE, course);
			break;
		case 1:
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img[0], TRUE, course);
			break;
		case 2:
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img[cntr%20/10], TRUE, course);
			break;
		case 3:
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img[cntr%40/10], TRUE, course);
			break;
		case 4:
			DrawRotaGraphF((float)x, (float)y, 1.0, 0.0, img[cntr%40/10], TRUE, course);
			break;
		}
	}

	for(int i=0; i<ESHOT_MAX; i++){
		if(e_shot[i].flag){
			DrawRotaGraphF((float)e_shot[i].x, (float)e_shot[i].y, 1.0, (e_shot[i].angle+90)*PI/180, e_shot[i].img, TRUE);
		}
	}

	if(d_flag){
		DeadEf();
	}

	return c_flag;
}

//�S�ړ�����
void ENEMY::MoveAll(){
	CONTROL &control = CONTROL::GetInstance();

	if(in_time == control.GetGameCntr()){
		flag = true;
		m_flag = true;
	}

	//���S�̍ۂɑł̂����߂�
	if(out_time == control.GetGameCntr()) s_flag = false;

	if(flag){
		//�ړ��p�^�[������
		switch(ptrn){
		case 0:
			Move0();
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
		case 4:
			Move4();
			break;
		case 5:
			Move5();
			break;
		case 6:
			Move6();
			break;
		}

		if(m_flag){
			x += spd*cos(angle*PI/180);
			y += spd*sin(angle*PI/180);
		}

		//��ʊO����
		if(x < F_HMIN-100 || F_HMAX+100 < x || y < F_VMIN-100 || F_VMAX+100 < y){
			flag = false;
		}
		cntr++;
	}
}

//���~
void ENEMY::Move0(){
	CONTROL &control = CONTROL::GetInstance();

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	angle = 90;
}

//���~���㏸
void ENEMY::Move1(){
	CONTROL &control = CONTROL::GetInstance();

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	if(control.GetGameCntr() < stop_time){
		angle = 90;
	}else if(out_time < control.GetGameCntr()){
		angle = -90;
	}
}

//��
void ENEMY::Move2(){
	CONTROL &control = CONTROL::GetInstance();

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	if(course == 0){
		angle = 180;
	}else{
		angle = 0;
	}
}

//�΂�
void ENEMY::Move3(){
	CONTROL &control = CONTROL::GetInstance();

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	if(course == 0){
		angle = 150;
	}else{
		angle = 30;
	}
}

//�x�W�F�Ȑ�
void ENEMY::Move4(){
	CONTROL &control = CONTROL::GetInstance();

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	switch(course){
	case 0:{
		static double p0[2] = {x, y},
				p1[2] = {x, y+280},
				p2[2] = {x-240, y+270},
				p3[2] = {x-240, y+560};

		mymath->Bezier(x, y, int(spd*100), cntr, p0, p1, p2, p3, &angle);
		break;
		}
	case 1:{
		static double p0[2] = {x, y},
				p1[2] = {x, y+280},
				p2[2] = {x+240, y+270},
				p3[2] = {x+240, y+560};

		mymath->Bezier(x, y, int(spd*100), cntr, p0, p1, p2, p3, &angle);
		break;
		}
	}
}

//���@�Ǐ]
void ENEMY::Move5(){
	CONTROL &control = CONTROL::GetInstance();
	double px, py;
	control.GetPlayerPosition(&px, &py);

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	angle = atan2(py-y, px-x)*180/PI;
}

//���@�_��
void ENEMY::Move6(){
	CONTROL &control = CONTROL::GetInstance();
	double px, py;
	control.GetPlayerPosition(&px, &py);

	if(control.GetGameCntr() == stop_time) m_flag = false;
	if(control.GetGameCntr() == out_time) m_flag = true;

	if(cntr == 0) angle = atan2(py-y, px-x)*180/PI;
}

//�S�V���b�g����
void ENEMY::ShotAll(){
	CONTROL &control = CONTROL::GetInstance();

	if(shot_time == control.GetGameCntr()) s_flag = true;

	if(flag){
		if(s_flag){
			control.GetPlayerPosition(&px, &py);

			//�V���b�g�̎�ނ����߂�
			switch(s_ptrn){
			case 0:
				Shot0();
				break;
			case 1:
				NWay(1);
				break;
			case 2:
				NWay(3);
				break;
			case 3:
				NWay(5);
				break;
			case 4:
				NWay(2);
				break;
			case 5:
				Shot5();
				break;
			case 6:
				Shot6();
				break;
			}
			s_flag = false;
			s_cntr = 0;
		}
		s_cntr++;

		if(shot_time <= control.GetGameCntr()){
			//interval���ݒ肳��Ă�����s_flag���Ă�
			if(interval != 0 && s_cntr%interval == 0){
				s_flag = true;
			}
		}
	}
		//�e�̐����J�E���g
	int s_num = 0;

	//�e��i�߂�
	for(int i=0; i<ESHOT_MAX; i++){
		if(e_shot[i].flag){
			e_shot[i].x += e_shot[i].spd*cos(e_shot[i].angle*PI/180);
			e_shot[i].y += e_shot[i].spd*sin(e_shot[i].angle*PI/180);
			e_shot[i].cntr++;
			s_num++;

			//��ʊO����
			if(e_shot[i].x < F_HMIN-20 || e_shot[i].x > F_HMAX+20 || 
				e_shot[i].y < F_VMIN-20 || e_shot[i].y > F_VMAX+20){
				e_shot[i].flag = false;
			}
		}
	}

	//�c�e0���G�����݂��Ȃ������S�t���O��false�Ȃ�΁A���̓G�̃N���X�t���O������
	if(shot_time <= control.GetGameCntr()){
		if(!s_num && !flag && !d_flag){
			this->ChangeClassFlag(false);
		}
	}
}

//�V���b�g���ݒ�
void ENEMY::SetEs(double x, double y, double spd, double angle, int img, int ptrn){
	for(int i=0; i<ESHOT_MAX; i++){
		if(!e_shot[i].flag){
			e_shot[i].flag = true;
			e_shot[i].x = x;
			e_shot[i].y = y;
			e_shot[i].spd = spd;
			e_shot[i].angle = angle;
			e_shot[i].ptrn = ptrn;
			e_shot[i].img = img;
			e_shot[i].cntr = 0;
			break;
		}
	}
}

//N-Way�e
void ENEMY::NWay(int way){
	for(int i=0; i<nshot; i++){
		for(int j=0; j<way; j++){
			SetEs(x, y, s_spd-((s_spd/2/nshot)*i), 10*(way-1)/2+atan2(py-y, px-x)*180/PI-(j*10), s_img);
		}
	}
	PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
}

//�O��
void ENEMY::Shot0(){
	SetEs(x, y, s_spd, 90, s_img);
	PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
}

//1WAY
void ENEMY::Shot1(){
	double px, py;
	CONTROL &control = CONTROL::GetInstance();
	control.GetPlayerPosition(&px, &py);

	if(s_cntr == 0){
		for(int i=0; i<nshot; i++){
			SetEs(x, y, s_spd-((s_spd/2/nshot)*i), atan2(py-y, px-x)*180/PI, s_img);
		}
		PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
	}
}

//3WAY
void ENEMY::Shot2(){
	double px, py;
	CONTROL &control = CONTROL::GetInstance();
	control.GetPlayerPosition(&px, &py);

	if(s_cntr == 0){
		for(int i=0; i<nshot; i++){
			for(int j=0; j<3; j++){
				SetEs(x, y, s_spd-((s_spd/2/nshot)*i), atan2(py-y, px-x)*180/PI-10+(j*10), s_img);
			}
		}
		PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
	}
}

//5WAY
void ENEMY::Shot3(){
	double px, py;
	CONTROL &control = CONTROL::GetInstance();
	control.GetPlayerPosition(&px, &py);

	if(s_cntr == 0){
		for(int i=0; i<nshot; i++){
			for(int j=0; j<5; j++){
				SetEs(x, y, s_spd-((s_spd/2/nshot)*i), atan2(py-y, px-x)*180/PI-20+(j*10), s_img);
			}
		}
		PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
	}
}

//2WAY
void ENEMY::Shot4(){
	double px, py;
	CONTROL &control = CONTROL::GetInstance();
	control.GetPlayerPosition(&px, &py);

	if(s_cntr == 0){
		for(int i=0; i<nshot; i++){
			for(int j=0; j<2; j++){
				SetEs(x, y, s_spd-((s_spd/2/nshot)*i), atan2(py-y, px-x)*180/PI-4+(j*8) , s_img);
			}
		}
		PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
	}
}

//�S����
void ENEMY::Shot5(){
	double r = 360/nshot;
	for(int i=0; i<nshot; i++){
		SetEs(x, y, s_spd, i*r, s_img);
	}
	PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
}

//����
void ENEMY::Shot6(){
	if(s_cntr == 0) srand((unsigned int)time(NULL));

	SetEs(x, y, s_spd, atan2(py-y, px-x)*180/PI-60+(rand()%120), s_img);
	if(s_cntr%3 == 0) PlaySoundMem(s_snd, DX_PLAYTYPE_BACK);
}

void ENEMY::Shot7(){

}

void ENEMY::SetSAngle(double angle, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && e_shot[i].cntr == ss_cntr){
					e_shot[i].angle = angle;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].cntr == ss_cntr){
					e_shot[i].angle = angle;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					e_shot[i].angle = angle;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && this->s_cntr == s_cntr){
					e_shot[i].angle = angle;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
		for(int i=0; i<ESHOT_MAX; i++){
			if(e_shot[i].flag && e_shot[i].ptrn == ptrn){
				e_shot[i].angle = angle;
			}
		}
	}
}

void ENEMY::AddSAngle(double angle, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && e_shot[i].cntr == ss_cntr){
					e_shot[i].angle += angle;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].cntr == ss_cntr){
					e_shot[i].angle += angle;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					e_shot[i].angle += angle;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && this->s_cntr == s_cntr){
					e_shot[i].angle += angle;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
		for(int i=0; i<ESHOT_MAX; i++){
			if(e_shot[i].flag && e_shot[i].ptrn == ptrn){
				e_shot[i].angle += angle;
			}
		}
	}
}

void ENEMY::SetSSpd(double spd, int s_cntr, int ss_cntr, int ptrn){
	if(ss_cntr){
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && e_shot[i].cntr == ss_cntr){
					e_shot[i].spd = spd;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].cntr == ss_cntr){
					e_shot[i].spd = spd;
				}
			}
		}
	}else{
		if(ptrn){
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && e_shot[i].ptrn == ptrn && this->s_cntr == s_cntr){
					e_shot[i].spd = spd;
				}
			}
		}else{
			for(int i=0; i<ESHOT_MAX; i++){
				if(e_shot[i].flag && this->s_cntr == s_cntr){
					e_shot[i].spd = spd;
				}
			}
		}
	}

	if(!s_cntr && !ss_cntr){
			for(int i=0; i<ESHOT_MAX; i++){
			if(e_shot[i].flag && e_shot[i].ptrn == ptrn){
				e_shot[i].spd = spd;
			}
		}
	}
}

//���S�G�t�F�N�g
void ENEMY::DeadEf(){
	DrawRotaGraphF((float)x, (float)y, 1.0, cntr, b_img[d_cntr], TRUE);
	d_cntr++;
	if(d_cntr == 16) d_flag = false;
}

//�G���W���擾
bool ENEMY::GetPosition(double *x, double *y){
	if(this->flag){
		*x = this->x;
		*y = this->y;
		return true;
	}else{
		return false;
	}
}

//�G�V���b�g���W���擾
bool ENEMY::GetShotPosition(int i, double *x, double *y, double *range){
	if(e_shot[i].flag){
		*x = e_shot[i].x;
		*y = e_shot[i].y;
		*range = e_shot[i].range;
		return true;
	}else{
		return false;
	}
}

//�G�t���O�̑���(������0:�t���O��Ԃ��A1:HP = 0�Ńt���O��ύX���ĕԂ�)
bool ENEMY::CtrlFlag(int num, bool flag){
	switch(num){
	case 0:
		break;
	case 1:
		if(this->hp <= 0){
			this->flag = false;
			this->d_flag = true;
		}
		break;
	}
	return this->flag;
}

//�G�V���b�g�̌ʃt���O��ύX
void ENEMY::ChangeShotFlag(int i, bool flag){
	e_shot[i].flag = flag;
}

//�G�N���X�t���O��ύX�Afalse�Ȃ�e_num�𑝉�
void ENEMY::ChangeClassFlag(bool flag){
	CONTROL &control = CONTROL::GetInstance();

	this->c_flag = flag;
	if(!flag){
		control.CtrlEnum(1);
	}
}

//HP����
void ENEMY::ChangeHP(double psatk){
	this->hp += psatk;
}

//��ʓ��ɂ��邩
bool ENEMY::InArea(double x, double y){
	if(F_HMIN <= x && x <= F_HMAX && F_VMIN <= y && y <= F_VMAX){
		return true;
	}else{
		return false;
	}
}
