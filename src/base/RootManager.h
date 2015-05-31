#ifndef _ROOT_MANAGER_H_
#define _ROOT_MANAGER_H_

#include"IncludeAll.h"
#include"WindowsBase.h"
#include"DirectX9Base.h"
#include"../dx11/DirectX11Base.h"

#include"MatrixBox.h"
#include"BillBoardBeamManager.h"
#include"DebugCommentManager.h"
#include"../2DDrawer/2DDrawManager.h"
#include"../PAS/LogManager.h"
#include"../PAS/PASComputeTexture.h"
#include"../Shader/ShaderBox.h"
#include"../Shader/ShaderManager.h"
#include"../Shader/DebugTextureViewer.h"

#include"BaseManager.h"
#include"FPSCounter.h"
#include"DataBox.h"
#include"CameraController.h"

//using namespace boost;
#include<thread>
#include<mutex>

class RootManager{

	//GameLoopÇ∆MessageLoopÇÃîrëºêßå‰
	//boost::mutex mtx_GameLoop;
	std::mutex mtx_GameLoop;
	bool EndFlag;

	//GameLoopÇ∆DrawÇÃîrëºêßå‰
	//boost::mutex mtx_Draw;
	std::mutex mtx_Draw;
	bool EnableDrawFlag;

	DirectX11Base	*DxBase;
	WindowsBase		*WndBase;

	PASComputeTexture	*pPASCT;

	ShaderBox			*pShaderBox;
	ShaderManager		*pShaderM;
	//BillBoardBeamManager *pBBBM;
	My2DDrawManager		*p2DDrawer;
	DebugTextureViewer	*pDebugTextureViewer;

	Input				*pInput;
	VCon				*pVCon;
	FPSCounter			*pfpsc;
	DataBox				*pDataBox;
	MatrixBoxManager	*pMatrixBox;
	DebugCommentManager	*pDebugComM;
	CameraController	*pcam;
	LogManager			*pLogM;

	BaseManager		*pGM;

public:
	RootManager();
	~RootManager();

	void Run(HINSTANCE hinst, int nCmd);
	void GameLoopNonPararell();
	void GameLoop();
	void SetMatrix();
	void Draw();

	void OutputLog(char *txt);
};

#endif