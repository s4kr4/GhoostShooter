#include "pch.h"
#include "key.h"

//キーとパッドのうち入力の長い方を代入
#define InputState(pad, key) ((pad) > (key) ? (pad) : (key))

KEY::KEY(){
	//キーボード
	key_cfg.down = KEY_INPUT_DOWN;
	key_cfg.left = KEY_INPUT_LEFT;
	key_cfg.right = KEY_INPUT_RIGHT;
	key_cfg.up = KEY_INPUT_UP;
	key_cfg.shot = KEY_INPUT_Z;
	key_cfg.bom = KEY_INPUT_X;
	key_cfg.slow[0] = KEY_INPUT_RSHIFT;
	key_cfg.slow[1] = KEY_INPUT_LSHIFT;
	key_cfg.esc = KEY_INPUT_ESCAPE;
	key_cfg.skip[0] = KEY_INPUT_RCONTROL;
	key_cfg.skip[1] = KEY_INPUT_LCONTROL;

	//コンフィグ読み込み
	int cfg_n[9] = {};
	FILE *key_fp;

	if(error = fopen_s(&key_fp, "key.dat", "rb") != 0){
		MessageBox(NULL, "コンフィグ読込失敗", "警告", 0);
	}

	fread(cfg_n, sizeof(cfg_n), 1, key_fp);
	fclose(key_fp);

	pad_cfg.down = down_n = cfg_n[0];
	pad_cfg.left = left_n = cfg_n[1];
	pad_cfg.right = right_n = cfg_n[2];
	pad_cfg.up = up_n = cfg_n[3];
	pad_cfg.shot = shot_n = cfg_n[4];
	pad_cfg.bom = bom_n = cfg_n[5];
	pad_cfg.slow[0] = slow_n = cfg_n[6];
	pad_cfg.esc = esc_n = cfg_n[7];
	pad_cfg.skip[0] = skip_n = cfg_n[8];
	
	for(int i=0; i<256; i++){
		KeyBuf[i] = 0;
		KeyInput[i] = 0;
	}
	for(int i=0; i<PAD_MAX; i++){
		PadInput[i] = 0;
	}
}

//コンフィグをdatに出力
void KEY::WriteDat(){
	FILE *key_fp;

	int cfg_n[9];
	cfg_n[0] = down_n;
	cfg_n[1] = left_n;
	cfg_n[2] = right_n;
	cfg_n[3] = up_n;
	cfg_n[4] = shot_n;
	cfg_n[5] = bom_n;
	cfg_n[6] = slow_n;
	cfg_n[7] = esc_n;
	cfg_n[8] = skip_n;

	if(error = fopen_s(&key_fp, "key.dat", "wb") != 0){
		MessageBox(NULL, "コンフィグ書込失敗", "警告", 0);
	}
	fwrite(cfg_n, sizeof(cfg_n), 1, key_fp);
	fclose(key_fp);
}

//キー入力フレーム数を返す
int KEY::GetInputKey(int hndl){
	return KeyInput[hndl];
}

//パッド入力フレーム数を返す
int KEY::GetInputPad(int hndl){
	return PadInput[hndl];
}

//キーコンフィグ設定
//変更したいbtnの番号を指定すると、押したbtn番号が代入される
//btnになにも指定しない場合、hndlに指定されたボタンに元々対応していたパッド番号を返す
int KEY::SetConfig(int *hndl, int btn){
	if(btn != -1){
		for(int i=0; i<PAD_MAX; i++){
			if(PadBuf & 1<<i){
				//上下左右キーが代入されるのを回避
				if(i != 0 && i != 1 && i != 2 && i != 3){
					*hndl = i;
					switch(btn){
					case 0:
						pad_cfg.shot = i;
						break;
					case 1:
						pad_cfg.bom = i;
						break;
					case 2:
						pad_cfg.slow[0] = i;
						break;
					case 3:
						pad_cfg.skip[0] = i;
						break;
					case 4:
						pad_cfg.esc = i;
						break;
					}
				}
			}
		}
	}

	return *hndl;
}

//キー・パッドの入力フレーム数を計算
void KEY::CheckInputFrame(){
	for(int i=0; i<256; i++){
		if(KeyBuf[i] == 1){
			KeyInput[i]++;
		}else{
			KeyInput[i] = 0;
		}
	}

	for(int i=0; i<PAD_MAX; i++){
		if(PadBuf & 1<<i){
			PadInput[i]++;
		}else{
			PadInput[i] = 0;
		}
	}
}

//各キー割り当て
void KEY::SetInputState(){
	//GetInputPad, GetInputKeyでパッド・キーの入力時間を両方取得、長い方を代入
	down = InputState(GetInputPad(pad_cfg.down), GetInputKey(key_cfg.down));
	left = InputState(GetInputPad(pad_cfg.left), GetInputKey(key_cfg.left));
	right = InputState(GetInputPad(pad_cfg.right), GetInputKey(key_cfg.right));
	up = InputState(GetInputPad(pad_cfg.up), GetInputKey(key_cfg.up));
	shot = InputState(GetInputPad(pad_cfg.shot), GetInputKey(key_cfg.shot));
	bom = InputState(GetInputPad(pad_cfg.bom), GetInputKey(key_cfg.bom));
	slow = InputState(GetInputPad(pad_cfg.slow[0]), GetInputKey(key_cfg.slow[0])) > InputState(GetInputPad(pad_cfg.slow[0]), GetInputKey(key_cfg.slow[1]))?
			InputState(GetInputPad(pad_cfg.slow[0]), GetInputKey(key_cfg.slow[0])) : InputState(GetInputPad(pad_cfg.slow[0]), GetInputKey(key_cfg.slow[1]));
	skip = InputState(GetInputPad(pad_cfg.skip[0]), GetInputKey(key_cfg.skip[0])) > InputState(GetInputPad(pad_cfg.skip[0]), GetInputKey(key_cfg.skip[1]))?
			InputState(GetInputPad(pad_cfg.skip[0]), GetInputKey(key_cfg.skip[0])) : InputState(GetInputPad(pad_cfg.skip[0]), GetInputKey(key_cfg.skip[1]));
	esc = InputState(GetInputPad(pad_cfg.esc), GetInputKey(key_cfg.esc));

	//移動キー以外のどのボタンが押されてもanyを増加(Push Any Key)
	if(shot || bom || slow || skip || esc){
		any++;
	}else{
		any = 0;
	}
}

int KEY::GetKeyState(){
	if(GetHitKeyStateAll(KeyBuf) == -1) return -1;
	PadBuf = GetJoypadInputState(DX_INPUT_PAD1);

	CheckInputFrame();
	SetInputState();

	return 0;
}
