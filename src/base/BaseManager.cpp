#include"BaseManager.h"
//#include"MenuFlag.h"

BaseManager::BaseManager()
{
	PlayerNum		= 1;
	EndFlag		= true;
	pfield	   = nullptr;
	//MGM		   = nullptr;
	//BTLM	   = nullptr;
	//CEditM	   = nullptr;
	//FFM		   = nullptr;
	PASM	   = nullptr;
	NowGMC = 0;
}
BaseManager::~BaseManager()
{
	SAFE_DELETE(pfield);
	//SAFE_DELETE(MenuM);
	//SAFE_DELETE(MGM);
	//SAFE_DELETE(BTLM);
	//SAFE_DELETE(CEditM);
	//SAFE_DELETE(FFM);
	SAFE_DELETE(PASM);
}


void BaseManager::Init()
{
	pcam->SetViewPort(1);
	pcam->GetCameraPointer()->FreeLook(
		&XMVectorSet(-1250.f, -533.f, 2300.f, 0.f),
		&XMVectorSet(-1.f, 0.f, 0.f, 0.f),
		&XMVectorSet(0.f, 1.f, 0.f, 0.f),
		1.f,1.f,1.f);
	pcam->SetPers(XMConvertToRadians(65), 1.0f, 10000000.0f);

	//pfield = new FieldManager();
	//pfield->SetSystemPointer(this);
	//pfield->Init();

	//MenuM = new MenuManager;
	//MenuM->SetSystemPointer(this);
	//MenuM->SetParent(this);
	//MenuM->Init();

	//VSM = new MainGameVSMgr;
	//MGM = new MainGame;
	//MGM->SetSystemPointer(this);
	//MGM->SetParent(this);
	////if(FAILED(VSM->Init()))return -1;

	//BTLM = new BattleManager;
	//BTLM->SetSystemPointer(this);
	//BTLM->SetParent(this);

	//CEditM = new CourseEditManager;
	//CEditM->SetSystemPointer(this);
	//CEditM->SetParent(this);
	////if(FAILED(CEditM->Init()))return -1;

	//FFM = new FreeFlightM;
	//FFM->SetSystemPointer(this);
	//FFM->SetParent(this);

	PASM = new PASManager;
	PASM->SetSystemPointer(this);
	PASM->SetParent(this);
	PASM->Init();

	//AddGM(MenuM);
	//AddGM(VSM);
	//AddGM(CEditM);

	NowGMC = PASM;
}


void BaseManager::SetMainGameInfo()
{
	//int Course;
	//int OX_Option;

	//各メニュークラスのカーソル位置のデータを取得
	//Course = MenuM->GetSelectedData(SCENE_CRSSELECT);
	//OX_Option = MenuM->GetSelectedData(SCENE_MGOPTION);

	//以下、取得したデータからゲーム設定
	//int LapNum = (OX_Option & 0xFF);
	//int NpcNum = (OX_Option & 0xFF00) >> 8;

	//MGM->SetCourse(Course, LapNum);

	//int Machine = MenuM->GetSelectedData(SCENE_MCNSELECT);
	//PlayerNum = (Machine & 3u) + 1;
	//MGM->SetPlayerNum(PlayerNum);
	//int *MachineID = new int [PlayerNum];
	//FOR(PlayerNum)
	//{
	//	//MachineID[i] = (Machine >> (2+i*4)) & 15;
	////	MGM->SetMachine(MachineID[i],TRUE);
	//}
	//FOR(NpcNum)
	//{
	//	////MGM->SetMachine((int)(rand()/(float)RAND_MAX*4), FALSE);
	//}
	//delete [] MachineID;
}


void BaseManager::SetBattleGameInfo()
{
	//BTLM->SetCourse(2, 1);
	//BTLM->SetPlayerNum(1);

	//マシン登録
	//BTLM->SetMachine(0,TRUE);
	//BTLM->SetMachine((int)(rand()/(float)RAND_MAX*4), FALSE);

	pcam->SetViewPort(PlayerNum);
}


void BaseManager::ChangeMenuFlag(int flag, bool ResetFlag)
{
	//switch(flag){
	//case SCENE_MENU	: 
	//	//NowGMC=MenuM;
	//	PlayerNum = 1;
	//	pcam->ChangeCamera(0);
	//	pcam->SetViewPort(PlayerNum);
	//	pcam->ResetCamera();
	//	pcam->SetMenuMode();
	//	//MenuM->Init();
	//	break;
	//case SCENE_GAME	: 
	//	NowGMC=MGM;
	//	MGM->Reset();
	//	SetMainGameInfo();
	//	MGM->Init();
	//	break;
	//case SCENE_BATTLE: 
	//	NowGMC=BTLM;
	//	BTLM->Reset();
	//	SetBattleGameInfo();
	//	BTLM->Init();
	//	break;
	//case SCENE_EDIT	: 
	//	NowGMC=CEditM;
	//	CEditM->Init();
	//	//CEditM->SetCourse(MenuM->GetSelectedData(SCENE_CRSSEDIT));
	//	break;
	//case SCENE_FF	: 
	//	NowGMC=FFM;
	//	FFM->Init();
	//	break;
	//case SCENE_CONNECT: 
	//	//NowGMC=FFM;
	//	//FFM->Init();
	//	break;
	//case SCENE_END	:
	//	EndFlag = false; break;
	//}
}

bool BaseManager::Move()
{
	//pfield->Move();
	NowGMC->Move();
	return EndFlag;
}
void BaseManager::Draw()
{
	FOR(PlayerNum)
	{
		pcam->ChangeCamera(i);
		pcam->GetCameraPointer()->SetTransform(nullptr);
		NowGMC->Draw();
		//pfield->Draw();
	}
#ifdef _DEBUG
	//pcam->GetCameraPointer()->DebugDraw();
#endif
}

void BaseManager::DebugDraw(){}