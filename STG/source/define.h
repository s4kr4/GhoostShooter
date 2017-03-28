#ifndef _DEFINE
#define _DEFINE

//マクロ変数定義

//フィールド座標
#define F_HMAX 440
#define F_HMIN 20
#define F_VMAX 460
#define F_VMIN 20

//背景スクロールスピード
#define SCROLL_SPEED 2

//プレイヤー初期位置
#define P_INIX (F_HMAX+F_HMIN)/2
#define P_INIY 400
//プレイヤーの速度
#define P_SPD 5
#define P_SPD_L 2
//自機弾速度
#define PSHOT_MAX 100

//敵最大数
#define E_MAX 200
//敵弾最大数
#define ESHOT_MAX 1000
#define BSHOT_MAX 1000

//プレイヤーの当たり判定
#define P_RANGE1 0.5
#define PSHOT_RANGE 8
//敵の当たり判定
#define E_RANGE0 10
//ボスの当たり判定
#define B_RANGE0 20

//円周率
#define PI 3.14159265358979323846

//ゲームパッドのボタン数
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
