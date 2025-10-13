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

	WNDCLASS Initialize(LPCWSTR const windowTitle_);

private:
	//ウィンドウとかの初期化
	WNDCLASS SetWindow(LPCWSTR const windowTitle_);

};

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam);



