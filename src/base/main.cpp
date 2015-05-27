#include"RootManager.h"
#include"IncludeAll.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow){

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(225);

	srand((UINT)hInstance);

	// アプリケーションの初期化
	RootManager RM;
	RM.Run(hInstance,nCmdShow);

   return 0;
}