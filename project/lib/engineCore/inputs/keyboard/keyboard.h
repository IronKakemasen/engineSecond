#pragma once
#include <dinput.h>


struct KeyBoard
{
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	////mouse
	//IDirectInput8* directInputForMouse = nullptr;
	//IDirectInputDevice8* mouse = nullptr;

	void Finalize();
	void Initialize(HWND hwnd_, WNDCLASS wc_);
};
