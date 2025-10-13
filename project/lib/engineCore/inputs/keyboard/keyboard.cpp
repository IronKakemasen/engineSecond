#include "keyboard.h"
#include <assert.h>
#pragma comment(lib,"dinput8.lib")


void KeyBoard::Initialize(HWND hwnd_, WNDCLASS wc_)
{
	//DirectInputの初期化
	HRESULT hr = DirectInput8Create(wc_.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));
	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(
		hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}

void KeyBoard::Finalize()
{
	keyboard->Unacquire();
	directInput->Release();
	keyboard->Release();
}
