#include "pch.h"
#include "control.h"

int func_state = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	SetMainWindowText("Shoot The Ghost!!!");

	ChangeWindowMode(TRUE);
	if(MessageBox(NULL, "全画面でプレイしたいですか？", "質問", MB_YESNO) == IDYES){
		ChangeWindowMode(FALSE);
	}else{
		ChangeWindowMode(TRUE);
	}

	DxLib_Init(), SetGraphMode(640, 480, 32), SetDrawScreen(DX_SCREEN_BACK);

	CONTROL &control = CONTROL::GetInstance();
	KEY &key = KEY::GetInstance();

	//----------------------------------------------------↓メインループ↓-------------------------------------------------
	while(ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 && key.GetKeyState()==0){
		switch(func_state){
		case 0:
			control.Start();
			break;
		case 1:
			control.Menu();
			break;
		case 2:
			if(key.skip) control.Game();
			control.Game();
			break;
		case 3:
			control.GameEnd();
			break;
		default:
			DxLib_End();
			break;
		}
	}
	//----------------------------------------------------↑メインループ↑-------------------------------------------------

	DxLib_End();
	return 0;
}
