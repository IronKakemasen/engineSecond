#include "./lib/engineCore/dxCommon/dxCommon.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    DxCommon c;

    MSG msg{};
    //ウィンドウのxが押されるまでループ
    while (msg.message != WM_QUIT)
    {
        //Windowにメッセージが来てたら最優先で処理させる
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //ゲームの処理
        else
        {
            c.BeginFrame();


            c.EndFrame();

        }
    }

    c.Finalize();

    return 0;
}

