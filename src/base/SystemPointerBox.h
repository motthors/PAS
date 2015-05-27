#ifndef _SYSTEMPOINTERBOX_H_
#define _SYSTEMPOINTERBOX_H_

#include"IncludeAll.h"
#include"VirtualController.h"
#include"SoundBox.h"
#include"FPSCounter.h"
#include"DataBox.h"
#include"CameraController.h"
#include"MatrixBox.h"
#include"DebugCommentManager.h"
#include"BillBoardBeamManager.h"
#include"../PAS/PASComputeTexture.h"
#include"../PAS/LogManager.h"
#include"../2DDrawer/2DDrawManager.h"

class SystemPointerWrap{
protected:
	VCon				*pInput;
	SoundBox			*pSoundBox;
	FPSCounter			*pfpsc;
	DataBox				*pDataBox;
	CameraController	*pcam;
	MatrixBoxManager	*pMatrixBox;
	DebugCommentManager *pDebugComM;
	BillBoardBeamManager *pBBBM;
	PASComputeTexture	*pPASCT;
	LogManager			*pLogM;
	My2DDrawManager		*p2DDrawer;
	LPDIRECT3DDEVICE9	pd3ddev;

public:
	SystemPointerWrap()
	{
		pInput		= nullptr;
		pSoundBox	= nullptr;
		pfpsc		= nullptr;
		pDataBox	= nullptr;
		pcam		= nullptr;
		pMatrixBox	= nullptr;
		pDebugComM	= nullptr;
		pBBBM		= nullptr;
		pLogM		= nullptr;
		p2DDrawer	= nullptr;
		pPASCT		= nullptr;
		pd3ddev		= nullptr;
	}
	void SetInputDevice(VCon* p){pInput=p;}
	void SetSoundevice	(SoundBox* p){pSoundBox=p;}
	void SetCounterDevice(FPSCounter* p){pfpsc=p;}
	void SetDataBoxDevice(DataBox* p){pDataBox=p;}
	void SetCameraDevice(CameraController* p){pcam=p;}
	void SetMatrixBox(MatrixBoxManager* p){pMatrixBox=p;}
	void SetDebugComM(DebugCommentManager* p){ pDebugComM = p; }
	void SetBBBM(BillBoardBeamManager* p){ pBBBM = p; }
	void SetPASCT(PASComputeTexture* p){ pPASCT = p; }
	void SetLogM(LogManager* p){ pLogM = p; }
	void Set2DDrawer(My2DDrawManager* p){ p2DDrawer = p; }

	void SetSystemPointer(SystemPointerWrap* p)
	{
		this->pInput		= p->pInput;
		this->pSoundBox		= p->pSoundBox;
		this->pfpsc			= p->pfpsc;
		this->pDataBox		= p->pDataBox;
		this->pcam			= p->pcam;
		this->pMatrixBox	= p->pMatrixBox;
		this->pDebugComM	= p->pDebugComM;
		this->pBBBM			= p->pBBBM;
		this->pPASCT		= p->pPASCT;
		this->pLogM			= p->pLogM;
		this->p2DDrawer		= p->p2DDrawer;
		this->pd3ddev		= p->pd3ddev;
	}
};

#endif