#ifndef _PAS_MANAGER_H_
#define _PAS_MANAGER_H_

#include"../base/IncludeAll.h"
#include"../base/GameManagerComposite.h"
#include"LogManager.h"
#include"PASComputeTexture.h"
#include"../ManuManager/MenuManager.h"
#include"EarthViewController.h"

class PASManager : public GMComposite{

	MenuManager* MenuM;
	EarthViewController* m_pEarthView;

	// カメラ用変数
	vec3 BasePoint;
	vec3 Direc;
	vec3 UP;
	float length; // temp

	// PASCTからもらう太陽の方向ベクトルポインタ
	vec3* m_pSunVec;

	void MenuSetup();
public:

	PASManager();
	virtual ~PASManager();

	int Init();
	void Reset();
	void End();

	void InputControll_Mouse_Key();
	void InputControll_Pad();

	void TurnSunVec(float x, float );

	bool Move();
	void Draw();

#ifdef _DEBUG
	void DebugDraw();
#endif

};

#endif