#include"CameraController.h"


CameraController::CameraController()
{
	ppCamera = nullptr;
	CameraIndex = 0;
}

CameraController::~CameraController()
{
	Release();
}

void CameraController::Init(int CamNum, DXDEVICE* pdev, DebugCommentManager* pdebm)
{
	Release();
	CameraNum = CamNum;
	ppCamera = new Camera*[CamNum];
	FOR(CameraNum)
	{
		ppCamera[i] = new Camera;
		ppCamera[i]->ResetFreeCamera();
	}
}
void CameraController::Release()
{
	if (ppCamera == nullptr) return;
	FOR(CameraNum)SAFE_DELETE(ppCamera[i]);
	SAFE_DELETEARRAY(ppCamera);
}


void CameraController::SetViewPort(int playerNum)
{
#ifdef _DEBUG
	if (playerNum > PadNum || playerNum <= 0)
	{
		ErrM.SetClassName(_T("CameraController::SetViewPort"));
		ErrM.SetErrorText(_T("‰æ–Ê•ªŠ„”‚Í1‚©‚ç4‚Ü‚Å"));
		throw &ErrM;
	}
#endif
	FOR(CameraNum)ppCamera[i]->SetViewPort(playerNum, i, WindowWidth, WindowHeight);
}

void CameraController::SetView( vec *pos,vec *dir,vec *up)
{
	FOR(CameraNum)ppCamera[i]->SetView(pos, dir, up);
}

void CameraController::SetPers(float fovYRad, float znear, float zfar)
{
	FOR(CameraNum)ppCamera[i]->SetPers(fovYRad, znear, zfar);
}

void CameraController::SetWindowSize(float width, float height)
{
	WindowWidth		= width;
	WindowHeight	= height;
	FOR(CameraNum)ppCamera[i]->SetViewPort(CameraNum, i, width, height);
}

Camera* CameraController::GetCameraPointer(int idx)
{
#ifdef _DEBUG
	if (idx > CameraNum - 1  || idx < 0)
	{
		ALERT(_T("CameraController::GetCameraPointer\nidx‚ª”ÍˆÍŠO"));
		throw -1;
	}
#endif
	return ppCamera[idx]; 
}

Camera* CameraController::GetCameraPointer()
{
	return ppCamera[CameraIndex];
}

void CameraController::ChangeCamera(int num)
{
	CameraIndex = num;
}



/////////////////////////////////////////////////////////////////////////

//void CameraController::CheckTransPosIndex(D3DXVECTOR3* pPos, POSINDEX* pIdx)
//{
//	aCamera[CameraIdx].Transfunction(&pPos->x, &pIdx->x, 1);
//	aCamera[CameraIdx].Transfunction(&pPos->y, &pIdx->y, 2);
//	aCamera[CameraIdx].Transfunction(&pPos->z, &pIdx->z, 3);
//}