#include "windowSetUp.h"
#include "../dxCommon/dxCommon.h"
#include "../../commonVariables.h"

#pragma comment(lib,"d3d12.lib")

WNDCLASS WindowSetUp::SetWindow(LPCWSTR const windowTitle_)
{
#ifdef _DEBUG

	if (DxCommon::debugLayerSwitch)
	{
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			debugController->SetEnableGPUBasedValidation(TRUE);
		}
	}

#endif // _DEBUG

	WNDCLASS wc{};
	//ウィンドウプロシージャ(複数の処理を一つにまとめたもの)
	wc.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc.lpszClassName = windowTitle_;
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラスを登録する
	RegisterClass(&wc);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,(LONG)V_Common::kWindow_W,(LONG)V_Common::kWindow_H };
	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//[ ウィンドウの生成 ]
	hwnd = CreateWindow(
		wc.lpszClassName,		//利用するクラス名
		windowTitle_,			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//表示x座標
		CW_USEDEFAULT,			//表示y座標
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr);				//オプション

	return wc;
}

WNDCLASS WindowSetUp::Initialize(LPCWSTR const windowTitle_)
{
	WNDCLASS wc = SetWindow(windowTitle_);

	//[ ウィンドウを表示する ]
	ShowWindow(hwnd, SW_SHOW);

	return wc;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ImGuiにメッセージを渡す
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);

		return 0;
	}

	//標準メッセージの処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
