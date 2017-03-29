#ifndef _DEFINE
#define _DEFINE

//�}�N���ϐ���`

//�t�B�[���h���W
#define F_HMAX 440
#define F_HMIN 20
#define F_VMAX 460
#define F_VMIN 20

//�w�i�X�N���[���X�s�[�h
#define SCROLL_SPEED 2

//�v���C���[�����ʒu
#define P_INIX (F_HMAX+F_HMIN)/2
#define P_INIY 400
//�v���C���[�̑��x
#define P_SPD 5
#define P_SPD_L 2
//���@�e���x
#define PSHOT_MAX 100

//�G�ő吔
#define E_MAX 200
//�G�e�ő吔
#define ESHOT_MAX 1000
#define BSHOT_MAX 1000

//�v���C���[�̓����蔻��
#define P_RANGE1 0.5
#define PSHOT_RANGE 8
//�G�̓����蔻��
#define E_RANGE0 10
//�{�X�̓����蔻��
#define B_RANGE0 20

//�~����
#define PI 3.14159265358979323846

//�Q�[���p�b�h�̃{�^����
#define PAD_MAX 16

#define WHITE GetColor(255, 255, 255)
#define RED GetColor(255, 0, 0)
#define GREEN GetColor(0, 255, 0)
#define BLUE GetColor(0, 0, 255)

#define RED0
#define GREEN0
#define BLUE0
#define RED1
#define GREEN1
#define BLUE1
#define RED2
#define GREEN2
#define BLUE2
#define YELLOW2

extern int func_state;
extern errno_t error;

#endif
