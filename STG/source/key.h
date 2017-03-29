#include "pch.h"
#include "define.h"

#ifndef _KEY
#define _KEY

class KEY{
private:
	char KeyBuf[256];
	int KeyInput[256];
	int PadBuf;
	int PadInput[PAD_MAX];
	int PadTable[PAD_MAX];

	struct KEY_CFG_t{
		int left, up, right, down, shot, bom, slow[2], esc, skip[2];
	};

	KEY_CFG_t key_cfg;
	KEY_CFG_t pad_cfg;

	errno_t error;

private:
	KEY();

public:
	//入力フレーム数
	int left, up, right, down, shot, bom, slow, esc, skip, any;
	//btn番号
	int left_n, up_n, right_n, down_n, shot_n, bom_n, slow_n, skip_n, esc_n;

	void WriteDat();
	int GetInputKey(int hndl);
	int GetInputPad(int hndl);
	int SetConfig(int *hndl, int btn = -1);
	void CheckInputFrame();
	void SetInputState();
	int GetKeyState();
	static KEY& GetInstance(){
		static KEY key;
		return key;
	}
};

#endif
