#pragma once

#include <wrl.h>
#include <d3d12.h>


struct WindowSetUp
{

	//[ Dubug ]
#ifdef _DEBUG
	Microsoft::WRL::ComPtr <ID3D12Debug1> debugController = nullptr;
#endif 
	//ウィンドウハンドル
	HWND hwnd;

	//ウィンドウとかの初期化
	void SetWindow(LPCWSTR const windowTitle_);
	void Initialize(LPCWSTR const windowTitle_);
};

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam);



