#include"RootManager.h"
#include"DefineRendering.h"

using namespace std;

RootManager::RootManager()
{
	//pd3ddev		=0;
	//pd3dfont	=0;

	pInput		= nullptr;
	pVCon		= nullptr;
	pfpsc		= nullptr;
	pDataBox	= nullptr;
	pGM			= nullptr;
	pcam		= nullptr;
	pShaderBox	= nullptr;
	pShaderM	= nullptr;
	pMatrixBox	= nullptr;
	//pBBBM		= nullptr;
	pLogM		= nullptr;
	p2DDrawer	= nullptr;
	pPASCT		= nullptr;
	EndFlag			= true;
	EnableDrawFlag	= false;
	pDebugTextureViewer = nullptr;
}	

RootManager::~RootManager()
{
	SAFE_DELETE(pDebugTextureViewer);
	SAFE_DELETE(pShaderBox);
	SAFE_DELETE(pShaderM);
	//SAFE_DELETE(pBBBM);
	SAFE_DELETE(pLogM);
	SAFE_DELETE(pPASCT);
	SAFE_DELETE(p2DDrawer);
	SAFE_DELETE(pMatrixBox);
	SAFE_DELETE(pcam);
	SAFE_DELETE(pGM);
	SAFE_DELETE(pVCon);
	if(pInput)pInput->Del();
	SAFE_DELETE(pInput);
	SAFE_DELETE(pfpsc);
	SAFE_DELETE(pDebugComM);
	SAFE_DELETE(pDataBox);
	SAFE_DELETE(WndBase);
	SAFE_DELETE(DxBase);
}

void RootManager::Run(HINSTANCE hinst, int nCmd)
{
	try{

		//デバイス初期化
		WndBase = new WindowsBase();
		if (0 > WndBase->Init(hinst, nCmd))
			return;

		DxBase = new DirectX11Base();
		if (FAILED(DxBase->Init(WndBase->GetHWND(), WndBase->GetFlscflag())))
			return;

		pDebugComM = new DebugCommentManager(DxBase->GetDevice(), DxBase->GetContext());
		pDebugComM->SetDivideNum(4, WndBase->GetWindowSizeW(), WndBase->GetWindowSizeH());

		pInput = new Input();
		if ((pInput->Init(hinst, WndBase->GetHWND(), pDebugComM)) < 0)
			return;

		pVCon = new virtualController();
		pVCon->setInput(pInput);

		pfpsc = new FPSCounter();
		pfpsc->SetStdFPS(60);
		pfpsc->SetVcon(pInput);

		pcam = new CameraController();
		pcam->Init(1, DxBase->GetDevice(), pDebugComM);
		pcam->SetWindowSize((float)DefRender.RenderTargetX, (float)DefRender.RenderTargetY);

		pDataBox = new DataBox();
		pDataBox->SetD3Device(DxBase->GetDevice(), DxBase->GetContext());
		pDataBox->SetCamera(pcam);
		pDataBox->Init();
		
		//pMatrixBox = new MatrixBoxManager();
		//pMatrixBox->SetCameraPointer(pcam);

		pLogM = new LogManager(pDebugComM);
		pLogM->Init(10);


		//////////// Shader Group /////////////

		pShaderBox = new ShaderBox;
		pShaderBox->Init(DxBase);

		////pBBBM = new BillBoardBeamManager;
		////pBBBM->Init(DxBase->GetpD3DDev(), pDataBox, pcam);
		
		pPASCT = new PASComputeTexture;
		pPASCT->Init(DxBase, pDataBox, pMatrixBox, pLogM, pShaderBox);

		p2DDrawer = new My2DDrawManager;
		p2DDrawer->SetShaderBox(pShaderBox);
		p2DDrawer->Init(DxBase, pDataBox, DefRender.RenderTargetX, DefRender.RenderTargetY);

		pDebugTextureViewer = new DebugTextureViewer;
		pDebugTextureViewer->Init(DxBase, pShaderBox);

		pShaderM = new ShaderManager;
		pShaderM->SetDataBoxPointer( pDataBox );
		pShaderM->SetInputDevice( pInput );
		//pShaderM->SetMatrixBoxManager( pMatrixBox );
		////pShaderM->SetBBBManager(pBBBM);
		pShaderM->SetPASCmpTex(pPASCT);
		pShaderM->Set2DDrawer(p2DDrawer);
		pShaderM->SetDebugTexture(pDebugTextureViewer);
		pShaderM->Init(DxBase, pShaderBox);

		//SoundBox *sp=NULL;//temp;

#ifdef _DEBUG
		//pDataBox->DebugerInit();
#endif

		WndBase->ShowWnd();

		pGM = new BaseManager;
		pGM->SetInputDevice(pVCon);
		//pGM->SetSoundevice(sp);
		pGM->SetCounterDevice(pfpsc);
		pGM->SetDataBoxDevice(pDataBox);
		pGM->SetCameraDevice(pcam);
		//pGM->SetMatrixBox(pMatrixBox);
		pGM->SetDebugComM(pDebugComM);
		//pGM->SetBBBM(pBBBM);
		pGM->SetPASCT(pPASCT);
		pGM->Set2DDrawer(p2DDrawer);
		pGM->SetLogM(pLogM);
		pGM->Init();
	}
	catch (ErrorManager* p)
	{
		p->Alert(WndBase->GetHWND());
		return;
	}

	pfpsc->Start();

	//Create threads
#ifdef PARARELL_DRAW
	thread thr_GameLoop(bind(&RootManager::GameLoop, this));
	thread thr_DrawLoop(bind(&RootManager::Draw, this));
#else
	thread thr_GameLoop(bind(&RootManager::GameLoopNonPararell, this));
#endif

	MSG msg;
	// メッセージ ループ
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	{ // 排他制御でGameLoopを終了しにかかる
		ScopedLock(mtx_GameLoop);
		EndFlag = false;
	}

	thr_GameLoop.join();
#ifdef PARARELL_DRAW
	thr_DrawLoop.join();
#endif
}

void RootManager::GameLoopNonPararell()
{
	try{
		bool endflag = 1;
		while (endflag)
		{
			pfpsc->Update();
			pInput->Update();
			endflag = pGM->Move();
			{
				ScopedLock(mtx_GameLoop);
				//endflag = endflag & EndFlag;
				if (!endflag)
				{
					SendMessage(WndBase->GetHWND(), WM_CLOSE, 0, 0);
					break;
				}
				if (!EndFlag)
				{
					break;
				}
			}
			pfpsc->Draw(pDebugComM);
			Camera* pc = pcam->GetCameraPointer();
			pGM->Draw();
			pLogM->Draw();
			//pShaderM->SetPointerSet( pMatrixBox->GetPointerSet() );
			//pMatrixBox->BeginSet();
			pInput->debugDraw();

			//// 描画開始
			mat4 v;
			mat4 p;
			vec3 c;
			//XMStoreFloat4x4(&v, *pc->GetView());
			//XMStoreFloat4x4(&p, *pc->GetPers());
			//XMStoreFloat3(&c, *);
			pShaderBox->SetMatrixVector(pc->GetView(), pc->GetPers(), pc->GetCameraPos());
			pShaderM->BeginDraw();
			pShaderM->Draw();
			pDebugComM->Draw();
			pDebugComM->Reset();

			pShaderM->EndDraw();
			// 描画ここまで

			pfpsc->Wait();
			//pfpsc->Wait2();

		}
	}
	catch (ErrorManager* p)
	{
		p->Alert(WndBase->GetHWND());
		SendMessage(WndBase->GetHWND(), WM_CLOSE, 0, 0);
	}

}

void RootManager::GameLoop()
{
	
	bool endflag=1;
	while(endflag)
	{
		pInput->Update();
		endflag = pGM->Move();
		{
			//mutex::scoped_lock(mtx_GameLoop);
			mtx_GameLoop.lock();
			endflag = endflag & EndFlag;
			mtx_GameLoop.unlock();
		}
		pfpsc->Update();
		pfpsc->Draw(pDebugComM);
		SetMatrix();
		pfpsc->Wait();
	}

	{
		//mutex::scoped_lock(mtx_GameLoop);
		mtx_GameLoop.lock();
		EndFlag = false;
		mtx_GameLoop.unlock();
	}
	SendMessage(WndBase->GetHWND(), WM_CLOSE, 0, 0);
}

void RootManager::SetMatrix()
{
	
	//pShaderM->SetViewProj(pcam->GetView(), pcam->GetPers(), pcam->GetCameraPos());
	pGM->Draw();
	bool flag;
	{ // フラグが立ってたらまだ
		//mutex::scoped_lock(mtx_Draw);
		mtx_Draw.lock();
		flag = EnableDrawFlag;
		if(flag)OutputLog("NowDraw Waiting\n");
		else OutputLog("CanSetM\n");
		mtx_Draw.unlock();
	}
	while(flag) // フラグが立ってたらまだ
	{
		Sleep(5);// ちょっと待つ
		{ 
			//mutex::scoped_lock(mtx_Draw);
			mtx_Draw.lock();
			flag = EnableDrawFlag;
			if(flag)OutputLog("NowDraw Waited\n");
			else OutputLog("CanSetM\n");
			mtx_Draw.unlock();
		}
		{
			//mutex::scoped_lock(mtx_GameLoop);
			mtx_GameLoop.lock();
			if (!EndFlag)OutputLog("EndFlag on return\n");
			if(!EndFlag)return;
			mtx_GameLoop.unlock();
		}
	}
	pShaderM->SetPointerSet( pMatrixBox->GetPointerSet() );
	pMatrixBox->BeginSet();
	{
		//mutex::scoped_lock(mtx_Draw);
		mtx_Draw.lock();
		EnableDrawFlag = true;
		OutputLog("EndSetM WaitMode\n");
		mtx_Draw.unlock();
	}
}

void RootManager::Draw()
{
	Sleep(10); // ちょっと待つ

	while(1)
	{
		bool flag;
		{
			//mutex::scoped_lock(mtx_Draw);
			mtx_Draw.lock();
			flag = EnableDrawFlag;
			if(!flag)OutputLog("\t\t\t\tNowSeting Wait\n");
			else OutputLog("\t\t\t\tCanDrawM\n");
			mtx_Draw.unlock();
		}
		while(!flag) // Trueならwhile抜ける
		{ // まだMatrixSet終わってない
			Sleep(5); // ちょっと待つ
			{
				//mutex::scoped_lock(mtx_Draw);
				mtx_Draw.lock();
				flag = EnableDrawFlag;
				if(!flag)OutputLog("\t\t\t\tNowSeting Waiting\n");
				else OutputLog("\t\t\t\tCanDrawM\n");
				mtx_Draw.unlock();
			}
			{
				//mutex::scoped_lock(mtx_GameLoop);
				mtx_GameLoop.lock();
				if (!EndFlag)OutputLog("\t\t\t\tEndFlag on return\n");
				if(!EndFlag)return;
				mtx_GameLoop.unlock();
			}
		}
		// 描画開始
		//DxBase->BeginDraw();
		pShaderM->Draw();
		//pInput->debugDraw(DxBase->GetpD3DFont());
		//DxBase->EndDraw();
		// 描画が終わったらフラグを倒す
		{
			//mutex::scoped_lock(mtx_Draw); //描画終了後ここに入ってこなくなる時がある
			mtx_Draw.lock();
			EnableDrawFlag = false;
			OutputLog("\t\t\t\tEndDraw WaitMode\n");
			mtx_Draw.unlock();
		}
		{
			//mutex::scoped_lock(mtx_GameLoop);
			mtx_GameLoop.lock();
			if (!EndFlag)OutputLog("\t\t\t\tEndFlag on return\n");
			if(!EndFlag)return;
			mtx_GameLoop.unlock();
		}
	}
}


void RootManager::OutputLog(char *txt)
{
	//FILE *fp = fopen("./MutexLog.txt","w");
	//fprintf(fp,txt
	//OutputDebugStringA(txt);
}